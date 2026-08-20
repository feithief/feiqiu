#!/usr/bin/env python3
"""Generate the lightweight AUTOSAR LIN Slave communication configuration.

Only unconditional LIN frames/signals and the standard MRF/SRF routing are
generated.  DCM services and DID tables are intentionally out of scope.
"""

from __future__ import annotations

import argparse
import re
import shutil
import sys
from dataclasses import dataclass, field
from datetime import datetime
from pathlib import Path


class LdfError(ValueError):
    pass


@dataclass(frozen=True)
class Signal:
    name: str
    size: int
    initial: int
    publisher: str
    subscribers: tuple[str, ...]


@dataclass(frozen=True)
class FrameSignal:
    name: str
    offset: int


@dataclass(frozen=True)
class Frame:
    name: str
    frame_id: int
    publisher: str
    length: int
    signals: tuple[FrameSignal, ...]


@dataclass
class NodeAttributes:
    protocol: str = ""
    configured_nad: int | None = None
    supplier_id: int | None = None
    function_id: int | None = None
    variant: int | None = None
    response_error: str | None = None
    configurable_frames: list[str] = field(default_factory=list)


@dataclass
class Network:
    protocol: str
    speed_bps: int
    master: str
    slaves: list[str]
    signals: dict[str, Signal]
    frames: list[Frame]
    diagnostic_frames: list[tuple[str, int]]
    node_attributes: dict[str, NodeAttributes]


@dataclass(frozen=True)
class SelectedFrame:
    frame: Frame
    direction: str
    ipdu_index: int
    pdu_name: str
    pdu_id: int
    initial_data: tuple[int, ...]


@dataclass(frozen=True)
class SelectedSignal:
    signal: Signal
    frame: SelectedFrame
    offset: int
    symbol: str


def strip_comments(text: str) -> str:
    text = re.sub(r"/\*.*?\*/", "", text, flags=re.DOTALL)
    return re.sub(r"//[^\r\n]*", "", text)


def find_closing_brace(text: str, opening: int) -> int:
    depth = 0
    for index in range(opening, len(text)):
        if text[index] == "{":
            depth += 1
        elif text[index] == "}":
            depth -= 1
            if depth == 0:
                return index
    raise LdfError("LDF中存在未闭合的'{'。")


def find_block(text: str, keyword: str, required: bool = True) -> str:
    match = re.search(rf"(?m)^\s*{re.escape(keyword)}\s*\{{", text)
    if match is None:
        if required:
            raise LdfError(f"LDF缺少 {keyword} 块。")
        return ""
    opening = text.find("{", match.start())
    closing = find_closing_brace(text, opening)
    return text[opening + 1 : closing]


def find_named_block(text: str, name: str) -> str:
    match = re.search(rf"(?m)^\s*{re.escape(name)}\s*\{{", text)
    if match is None:
        return ""
    opening = text.find("{", match.start())
    closing = find_closing_brace(text, opening)
    return text[opening + 1 : closing]


def parse_int(value: str, context: str) -> int:
    value = value.strip()
    try:
        return int(value, 0)
    except ValueError as exc:
        raise LdfError(f"{context}不是有效整数: {value}") from exc


def parse_nodes(block: str) -> tuple[str, list[str]]:
    master_match = re.search(
        r"\bMaster\s*:\s*([A-Za-z_]\w*)\s*,", block, re.DOTALL
    )
    slave_match = re.search(r"\bSlaves\s*:\s*([^;]+);", block, re.DOTALL)
    if master_match is None or slave_match is None:
        raise LdfError("Nodes块中的Master或Slaves格式不正确。")
    slaves = [item.strip() for item in slave_match.group(1).split(",")]
    slaves = [item for item in slaves if item]
    if not slaves:
        raise LdfError("Nodes块没有从节点。")
    return master_match.group(1), slaves


def parse_signals(block: str) -> dict[str, Signal]:
    result: dict[str, Signal] = {}
    pattern = re.compile(r"([A-Za-z_]\w*)\s*:\s*([^;]+);")
    for match in pattern.finditer(block):
        name = match.group(1)
        fields = [item.strip() for item in match.group(2).split(",")]
        if len(fields) < 4:
            raise LdfError(f"信号{name}至少需要size/init/publisher/subscriber。")
        size = parse_int(fields[0], f"信号{name}长度")
        initial = parse_int(fields[1], f"信号{name}初值")
        if not 1 <= size <= 16:
            raise LdfError(f"信号{name}长度为{size}；当前轻量COM支持1..16 bit。")
        if not 0 <= initial < (1 << size):
            raise LdfError(f"信号{name}初值{initial}超出{size} bit范围。")
        if name in result:
            raise LdfError(f"信号{name}重复定义。")
        result[name] = Signal(
            name, size, initial, fields[2], tuple(fields[3:])
        )
    if not result:
        raise LdfError("Signals块没有普通信号。")
    return result


def parse_frames(block: str) -> list[Frame]:
    frames: list[Frame] = []
    header = re.compile(
        r"([A-Za-z_]\w*)\s*:\s*([^,{}]+)\s*,\s*"
        r"([A-Za-z_]\w*)\s*,\s*([^,{}\s]+)\s*\{"
    )
    cursor = 0
    while True:
        match = header.search(block, cursor)
        if match is None:
            break
        opening = block.find("{", match.start())
        closing = find_closing_brace(block, opening)
        entries = []
        for entry in re.finditer(
            r"([A-Za-z_]\w*)\s*,\s*([^;]+);", block[opening + 1 : closing]
        ):
            entries.append(
                FrameSignal(
                    entry.group(1),
                    parse_int(entry.group(2), f"帧{match.group(1)}信号偏移"),
                )
            )
        frame = Frame(
            match.group(1),
            parse_int(match.group(2), f"帧{match.group(1)} ID"),
            match.group(3),
            parse_int(match.group(4), f"帧{match.group(1)}长度"),
            tuple(entries),
        )
        frames.append(frame)
        cursor = closing + 1
    if not frames:
        raise LdfError("Frames块没有无条件帧。")
    return frames


def parse_diagnostic_frames(block: str) -> list[tuple[str, int]]:
    if not block:
        return []
    result = []
    for match in re.finditer(
        r"([A-Za-z_]\w*)\s*:\s*([^\s{]+)\s*\{", block
    ):
        result.append(
            (match.group(1), parse_int(match.group(2), "诊断帧ID"))
        )
    return result


def parse_node_attributes(block: str, slaves: list[str]) -> dict[str, NodeAttributes]:
    result: dict[str, NodeAttributes] = {}
    for slave in slaves:
        node_block = find_named_block(block, slave)
        attrs = NodeAttributes()
        if node_block:
            match = re.search(r'\bLIN_protocol\s*=\s*"([^"]+)"\s*;', node_block)
            if match:
                attrs.protocol = match.group(1)
            match = re.search(r"\bconfigured_NAD\s*=\s*([^;]+);", node_block)
            if match:
                attrs.configured_nad = parse_int(match.group(1), "configured_NAD")
            match = re.search(r"\bproduct_id\s*=\s*([^;]+);", node_block)
            if match:
                values = [parse_int(item, "product_id") for item in match.group(1).split(",")]
                if len(values) != 3:
                    raise LdfError(f"{slave}的product_id必须有3个值。")
                attrs.supplier_id, attrs.function_id, attrs.variant = values
            match = re.search(
                r"\bresponse_error\s*=\s*([A-Za-z_]\w*)\s*;", node_block
            )
            if match:
                attrs.response_error = match.group(1)
            configurable = find_block(node_block, "configurable_frames", False)
            if configurable:
                attrs.configurable_frames = re.findall(
                    r"(?m)^\s*([A-Za-z_]\w*)\s*(?:=\s*[^;]+)?;",
                    configurable,
                )
        result[slave] = attrs
    return result


def parse_ldf(path: Path) -> Network:
    try:
        text = path.read_text(encoding="utf-8-sig")
    except UnicodeDecodeError:
        text = path.read_text(encoding="gb18030")
    text = strip_comments(text)
    protocol_match = re.search(
        r'\bLIN_protocol_version\s*=\s*"([^"]+)"\s*;', text
    )
    speed_match = re.search(
        r"\bLIN_speed\s*=\s*([0-9]+(?:\.[0-9]+)?)\s*kbps\s*;",
        text,
        re.IGNORECASE,
    )
    if protocol_match is None or speed_match is None:
        raise LdfError("缺少LIN_protocol_version或LIN_speed。")
    master, slaves = parse_nodes(find_block(text, "Nodes"))
    signals = parse_signals(find_block(text, "Signals"))
    frames = parse_frames(find_block(text, "Frames"))
    diagnostic = parse_diagnostic_frames(
        find_block(text, "Diagnostic_frames", False)
    )
    attributes = parse_node_attributes(
        find_block(text, "Node_attributes", False), slaves
    )
    return Network(
        protocol_match.group(1),
        int(round(float(speed_match.group(1)) * 1000.0)),
        master,
        slaves,
        signals,
        frames,
        diagnostic,
        attributes,
    )


def c_name(name: str) -> str:
    value = re.sub(r"[^A-Za-z0-9_]", "_", name).upper()
    value = re.sub(r"_+", "_", value).strip("_")
    if not value:
        raise LdfError(f"名称无法转换为C标识符: {name}")
    if value[0].isdigit():
        value = "N_" + value
    return value


def pack_initial(frame: Frame, signals: dict[str, Signal], fill_byte: int) -> tuple[int, ...]:
    data = [fill_byte] * 8
    occupied: set[int] = set()
    for entry in frame.signals:
        if entry.name not in signals:
            raise LdfError(f"帧{frame.name}引用未定义信号{entry.name}。")
        signal = signals[entry.name]
        if entry.offset + signal.size > frame.length * 8:
            raise LdfError(f"帧{frame.name}中的信号{signal.name}超出DLC。")
        for bit in range(signal.size):
            absolute = entry.offset + bit
            if absolute in occupied:
                raise LdfError(f"帧{frame.name}的bit {absolute}被多个信号占用。")
            occupied.add(absolute)
            mask = 1 << (absolute & 7)
            if signal.initial & (1 << bit):
                data[absolute >> 3] |= mask
            else:
                data[absolute >> 3] &= ~mask
    return tuple(value & 0xFF for value in data)


def select_configuration(
    network: Network, node: str, fill_byte: int
) -> tuple[list[SelectedFrame], list[SelectedSignal], NodeAttributes]:
    if node not in network.slaves:
        raise LdfError(
            f"节点{node}不在Slaves列表中；可选: {', '.join(network.slaves)}"
        )
    attrs = network.node_attributes.get(node, NodeAttributes())
    frame_by_name = {frame.name: frame for frame in network.frames}
    ordered: list[Frame] = []
    for name in attrs.configurable_frames:
        if name not in frame_by_name:
            raise LdfError(f"configurable_frames引用未知帧{name}。")
        if frame_by_name[name] not in ordered:
            ordered.append(frame_by_name[name])
    ordered.extend(frame for frame in network.frames if frame not in ordered)

    selected_frames: list[SelectedFrame] = []
    used_ids: set[int] = set()
    for frame in ordered:
        if not 1 <= frame.length <= 8:
            raise LdfError(f"帧{frame.name}的DLC必须为1..8。")
        if not 0 <= frame.frame_id <= 0x3F:
            raise LdfError(f"帧{frame.name}的ID必须为0x00..0x3F。")
        if frame.frame_id in used_ids:
            raise LdfError(f"普通帧ID 0x{frame.frame_id:02X}重复。")
        relevant_entries = [
            entry for entry in frame.signals if entry.name in network.signals
        ]
        if frame.publisher == node:
            direction = "TX"
        elif any(
            node in network.signals[entry.name].subscribers
            for entry in relevant_entries
        ):
            direction = "RX"
        else:
            continue
        used_ids.add(frame.frame_id)
        index = len(selected_frames)
        selected_frames.append(
            SelectedFrame(
                frame,
                direction,
                index,
                f"LINIF_PDU_{c_name(frame.name)}",
                index,
                pack_initial(frame, network.signals, fill_byte),
            )
        )
    if not selected_frames:
        raise LdfError(f"LDF中没有与从节点{node}相关的普通帧。")

    selected_signals: list[SelectedSignal] = []
    symbols: set[str] = set()
    response_found = False
    for selected_frame in selected_frames:
        entries = sorted(selected_frame.frame.signals, key=lambda item: item.offset)
        for entry in entries:
            signal = network.signals[entry.name]
            relevant = (
                signal.publisher == node
                if selected_frame.direction == "TX"
                else node in signal.subscribers
            )
            if not relevant:
                continue
            symbol = f"COM_SIG_{selected_frame.direction}_{c_name(signal.name)}"
            if symbol in symbols:
                raise LdfError(f"生成的信号标识符重复: {symbol}")
            symbols.add(symbol)
            if attrs.response_error == signal.name:
                response_found = True
            selected_signals.append(
                SelectedSignal(signal, selected_frame, entry.offset, symbol)
            )
    if not selected_signals:
        raise LdfError(f"LDF中没有与从节点{node}相关的普通信号。")
    if attrs.response_error and not response_found:
        raise LdfError(
            f"response_error信号{attrs.response_error}不属于{node}的TX普通帧。"
        )
    return selected_frames, selected_signals, attrs


def generate_header(
    network: Network,
    node: str,
    frames: list[SelectedFrame],
    signals: list[SelectedSignal],
    attrs: NodeAttributes,
) -> str:
    diagnostics = {frame_id: name for name, frame_id in network.diagnostic_frames}
    diag_count = int(0x3C in diagnostics) + int(0x3D in diagnostics)
    rx_frames = [frame for frame in frames if frame.direction == "RX"]
    lines = [
        "/* Generated from LDF. Do not edit manually. */",
        "#ifndef COM_CFG_H",
        "#define COM_CFG_H",
        "",
        '#include "LinIf.h"',
        "",
        f"#define LIN_CONFIG_BAUDRATE           ({network.speed_bps}UL)",
        f"#define LIN_NUMBER_OF_FRAMES          ({len(frames)}U)",
        f"#define LINIF_FRAME_COUNT              ({len(frames) + diag_count}U)",
        f"#define LINIF_APPLICATION_FRAME_COUNT  ({len(frames)}U)",
        f"#define COM_IPDU_COUNT                 ({len(frames)}U)",
    ]
    for frame in frames:
        lines.append(
            f"#define {frame.pdu_name:<38} ((PduIdType){frame.pdu_id}U)"
        )
    next_pdu = len(frames)
    if 0x3C in diagnostics:
        lines.append(
            f"#define LINIF_PDU_MASTER_REQUEST       ((PduIdType){next_pdu}U)"
        )
        next_pdu += 1
    else:
        lines.append(
            "#define LINIF_PDU_MASTER_REQUEST       ((PduIdType)0xFFFFU)"
        )
    if 0x3D in diagnostics:
        lines.append(
            f"#define LINIF_PDU_SLAVE_RESPONSE       ((PduIdType){next_pdu}U)"
        )
    else:
        lines.append(
            "#define LINIF_PDU_SLAVE_RESPONSE       ((PduIdType)0xFFFFU)"
        )
    response_frame = next(
        (
            signal.frame.ipdu_index
            for signal in signals
            if attrs.response_error == signal.signal.name
        ),
        None,
    )
    if response_frame is None:
        response_frame = next(
            (frame.ipdu_index for frame in frames if frame.direction == "TX"),
            0xFF,
        )
    lines.append(
        f"#define LINIF_RESPONSE_ERROR_FRAME_INDEX ({response_frame}U)"
    )
    lines.extend(
        [
            "#define COM_INVALID_PDU_ID             ((PduIdType)0xFFFFU)",
            "#define COM_INVALID_SIGNAL_ID          (0xFFFFU)",
            "",
            "typedef enum",
            "{",
        ]
    )
    for index, signal in enumerate(signals):
        suffix = " = 0" if index == 0 else ""
        lines.append(f"    {signal.symbol}{suffix},")
    lines.extend(["    COM_SIG_COUNT", "} Com_LinSignalIdType;", "", "typedef enum", "{"])
    for index, frame in enumerate(rx_frames):
        suffix = " = 0" if index == 0 else ""
        lines.append(
            f"    COM_SIGNAL_GROUP_{c_name(frame.frame.name)}{suffix},"
        )
    lines.extend(
        [
            "    COM_SIGNAL_GROUP_COUNT",
            "} Com_LinSignalGroupIdType;",
            "",
            "#define COM_SIGNAL_GROUP_STORAGE_COUNT \\",
            "    ((COM_SIGNAL_GROUP_COUNT > 0U) ? COM_SIGNAL_GROUP_COUNT : 1U)",
        ]
    )
    response = next(
        (signal.symbol for signal in signals if attrs.response_error == signal.signal.name),
        None,
    )
    if response:
        lines.append(
            f"#define COM_RESPONSE_ERROR_SIGNAL_ID    ((uint16_t){response})"
        )
    else:
        lines.append(
            "#define COM_RESPONSE_ERROR_SIGNAL_ID    (COM_INVALID_SIGNAL_ID)"
        )
    lines.extend(
        [
            "",
            "typedef enum",
            "{",
            "    COM_SIGNAL_DIRECTION_RX = 0,",
            "    COM_SIGNAL_DIRECTION_TX",
            "} Com_SignalDirectionType;",
            "",
            "typedef struct",
            "{",
            "    uint8_t IpduIndex;",
            "    uint8_t BitPosition;",
            "    uint8_t BitLength;",
            "    Com_SignalDirectionType Direction;",
            "} Com_SignalConfigType;",
            "",
            "typedef struct",
            "{",
            "    PduIdType PduId;",
            "    uint8_t Length;",
            "    Com_SignalDirectionType Direction;",
            "    uint8_t InitData[8];",
            "} Com_IpduConfigType;",
            "",
            "extern const Com_SignalConfigType Com_SignalConfig[COM_SIG_COUNT];",
            "extern const Com_IpduConfigType Com_IpduConfig[COM_IPDU_COUNT];",
            "extern const uint8_t",
            "    Com_SignalGroupIpduIndex[COM_SIGNAL_GROUP_STORAGE_COUNT];",
            "extern LinIf_FrameConfigType LinIf_FrameConfig[LINIF_FRAME_COUNT];",
            "",
            "#endif",
            "",
        ]
    )
    return "\n".join(lines)


def byte_list(data: tuple[int, ...]) -> str:
    return ", ".join(f"0x{value:02X}U" for value in data)


def generate_source(
    network: Network,
    node: str,
    frames: list[SelectedFrame],
    signals: list[SelectedSignal],
) -> str:
    diagnostics = {frame_id: name for name, frame_id in network.diagnostic_frames}
    protocol = network.node_attributes.get(node, NodeAttributes()).protocol
    protocol = protocol or network.protocol
    checksum = "LIN_CLASSIC_CS" if protocol.startswith("1.3") else "LIN_ENHANCED_CS"
    rx_frames = [frame for frame in frames if frame.direction == "RX"]
    lines = [
        "/* Generated from LDF. Do not edit manually. */",
        '#include "Com_Cfg.h"',
        '#include "Lin.h"',
        "",
        "const Com_SignalConfigType Com_SignalConfig[COM_SIG_COUNT] =",
        "{",
    ]
    for signal in signals:
        direction = f"COM_SIGNAL_DIRECTION_{signal.frame.direction}"
        lines.append(
            f"    {{{signal.frame.ipdu_index}U, {signal.offset}U, "
            f"{signal.signal.size}U, {direction}}}, /* {signal.signal.name} */"
        )
    lines.extend(["};", "", "const Com_IpduConfigType Com_IpduConfig[COM_IPDU_COUNT] =", "{"])
    for frame in frames:
        direction = f"COM_SIGNAL_DIRECTION_{frame.direction}"
        lines.extend(
            [
                f"    {{{frame.pdu_name}, {frame.frame.length}U, {direction},",
                f"     {{{byte_list(frame.initial_data)}}}}}, /* {frame.frame.name} */",
            ]
        )
    lines.extend(["};", "", "const uint8_t", "    Com_SignalGroupIpduIndex[COM_SIGNAL_GROUP_STORAGE_COUNT] =", "{"])
    if rx_frames:
        lines.append("    " + ", ".join(f"{frame.ipdu_index}U" for frame in rx_frames))
    else:
        lines.append("    0U")
    lines.extend(["};", "", "LinIf_FrameConfigType LinIf_FrameConfig[LINIF_FRAME_COUNT] =", "{"])
    frame_rows = []
    for frame in frames:
        frame_rows.append(
            f"    {{0x{frame.frame.frame_id:02X}U, LIN_FRAMERESPONSE_{frame.direction}, "
            f"{checksum}, {frame.frame.length}U, {frame.pdu_name}}}"
        )
    if 0x3C in diagnostics:
        frame_rows.append(
            "    {0x3CU, LIN_FRAMERESPONSE_RX, LIN_CLASSIC_CS, 8U, "
            "LINIF_PDU_MASTER_REQUEST}"
        )
    if 0x3D in diagnostics:
        frame_rows.append(
            "    {0x3DU, LIN_FRAMERESPONSE_TX, LIN_CLASSIC_CS, 8U, "
            "LINIF_PDU_SLAVE_RESPONSE}"
        )
    for index, row in enumerate(frame_rows):
        lines.append(row + ("," if index + 1 < len(frame_rows) else ""))
    lines.extend(
        [
            "};",
            "",
            "const Lin_ConfigType Lin_Config =",
            "{",
            f"    {network.speed_bps}UL,",
            "    5U,",
            "    4000U",
            "};",
            "",
        ]
    )
    return "\n".join(lines)


def backup_existing(output: Path, names: tuple[str, ...]) -> Path | None:
    existing = [output / name for name in names if (output / name).exists()]
    if not existing:
        return None
    backup = output / "LdfConfigBackup" / datetime.now().strftime("%Y%m%d_%H%M%S")
    backup.mkdir(parents=True, exist_ok=False)
    for path in existing:
        shutil.copy2(path, backup / path.name)
    return backup


def write_generated(output: Path, header: str, source: str, backup: bool) -> Path | None:
    output.mkdir(parents=True, exist_ok=True)
    backup_path = backup_existing(output, ("Com_Cfg.h", "Com_Cfg.c")) if backup else None
    (output / "Com_Cfg.h").write_text(header, encoding="utf-8", newline="\n")
    (output / "Com_Cfg.c").write_text(source, encoding="utf-8", newline="\n")
    return backup_path


def build_argument_parser() -> argparse.ArgumentParser:
    parser = argparse.ArgumentParser(
        description="从LDF生成轻量AUTOSAR LIN Slave通信配置（不生成DID）"
    )
    parser.add_argument("ldf", type=Path, help="输入LDF文件")
    parser.add_argument("--node", help="目标从节点名称；LDF只有一个Slave时可省略")
    parser.add_argument(
        "--output", type=Path, default=Path(__file__).parent / "Generated", help="输出目录"
    )
    parser.add_argument(
        "--fill-byte", type=lambda value: int(value, 0), default=0xFF,
        help="未使用位的填充值，默认0xFF（与IHR量产配置一致）"
    )
    parser.add_argument("--backup", action="store_true", help="覆盖前备份现有Com_Cfg.c/.h")
    parser.add_argument("--check", action="store_true", help="只解析和检查，不生成文件")
    return parser


def main(argv: list[str] | None = None) -> int:
    parser = build_argument_parser()
    args = parser.parse_args(argv)
    try:
        if not args.ldf.is_file():
            raise LdfError(f"找不到LDF文件: {args.ldf}")
        if not 0 <= args.fill_byte <= 0xFF:
            raise LdfError("--fill-byte必须为0x00..0xFF。")
        network = parse_ldf(args.ldf)
        if args.node:
            node = args.node
        elif len(network.slaves) == 1:
            node = network.slaves[0]
        else:
            raise LdfError("LDF包含多个Slave，请用--node指定目标从节点。")
        frames, signals, attrs = select_configuration(network, node, args.fill_byte)
        header = generate_header(network, node, frames, signals, attrs)
        source = generate_source(network, node, frames, signals)
        if not args.check:
            backup = write_generated(args.output, header, source, args.backup)
            print(f"已生成: {args.output / 'Com_Cfg.h'}")
            print(f"已生成: {args.output / 'Com_Cfg.c'}")
            if backup:
                print(f"原配置备份: {backup}")
        print(
            f"节点={node}, 波特率={network.speed_bps}, "
            f"普通帧={len(frames)}, 普通信号={len(signals)}, "
            f"诊断传输帧={len(network.diagnostic_frames)}"
        )
        print("DID生成=关闭（本工具不会读取或修改DCM/DID配置）")
        return 0
    except LdfError as exc:
        print(f"配置错误: {exc}", file=sys.stderr)
        return 2


if __name__ == "__main__":
    raise SystemExit(main())

#!/usr/bin/env python3
"""Build the AmbientDevice OS/runtime learning diagram from an AI-tech seed."""

from __future__ import annotations

import argparse
import copy
import html as html_module
import json
import math
import sys
import textwrap
import urllib.parse
import xml.etree.ElementTree as ET
from dataclasses import dataclass
from pathlib import Path
from typing import Dict, Iterable, List, Optional, Sequence, Tuple


WORKSPACE = Path(__file__).resolve().parent.parent
DEFAULT_MOTHER_SEED = Path("C:/Users/SH2304013/Desktop/AI科技界面_母Seed.drawio")
PAGE_WIDTH = 1760
PAGE_HEIGHT = 1000


@dataclass(frozen=True)
class Module:
    key: str
    class_id: str
    name: str
    summary: str
    x: int
    y: int
    width: int
    height: int
    source: str
    line: int
    comment: str
    stub: str
    os_note: str


@dataclass(frozen=True)
class SeedVisualTemplates:
    """Literal visual cells cloned from the supplied mother Seed XML."""

    module_full_halo: ET.Element
    module_shell: ET.Element
    module_top_accent: ET.Element
    module_header_halo: ET.Element
    module_header: ET.Element
    module_logic: ET.Element
    module_header_label: str
    module_logic_label: str
    process_edge: ET.Element
    gate_edge: ET.Element


PAGES: List[Tuple[str, str, List[Module]]] = [
    (
        "01_进程启动与线程",
        "从 Linux 创建进程，一路走到 GUI 与 LIN Worker 两个事件循环",
        [
            Module("loader", "H01", "Linux 装载器 / 虚拟地址空间", "创建单进程及代码、堆、栈映射", 70, 165, 265, 110, "AmbientDevice/main.cpp", 5, "工程只创建一个 Linux 进程；Qt 对象在同一虚拟地址空间内协作。", "用桌面 Kit 启动时确认 Q_OS_LINUX 分支不会碰真实串口；目标机启动时检查 /proc/<pid>/maps。", "“虚拟地址空间不存在于任何物理实体中，虚拟地址是一套规则。”（笔记第1页）"),
            Module("main", "S01", "main() 进程入口", "创建 QApplication、DebugStore、MainWindow", 405, 165, 265, 110, "AmbientDevice/main.cpp", 5, "依赖对象按明确次序构造；DebugStore 的生命周期覆盖窗口和 Worker。", "在 main() 后设置断点；验证 DebugAppState 首值为 Application starting，窗口关闭后无悬挂对象。", "程序被装入后才成为运行中的进程；代码与静态数据属于该进程的地址空间。"),
            Module("eventloop", "I01", "QApplication 事件循环", "a.exec() 分发点击、定时器与排队信号", 740, 165, 265, 110, "AmbientDevice/main.cpp", 14, "GUI 不使用 while 死循环；所有界面事件都由主线程事件循环串行处理。", "注入连续按钮/滑块事件，确认界面仍能刷新；关闭窗口后 a.exec() 正常返回。", "“本质上是调度实体的切换才会引起上下文切换。”（笔记第6页）"),
            Module("window", "S02", "MainWindow / 页面对象", "校验 Profile，创建控制、诊断与 Debug 页面", 1075, 165, 300, 110, "AmbientDevice/mainwindow.cpp", 62, "所有 QWidget 只在 GUI 线程创建和更新，禁止 Worker 直接改控件。", "提供错误 LinLayout，验证界面阻止启动总线并显示 Debug 错误；检查节点按钮按 uiSlot 排列。", "“每个线程都拥有自己的栈，意味着有自己的上下文。”（笔记第11页）"),
            Module("debug", "S03", "DebugStore 运行镜像", "跨线程记录 TX/RX、超时、诊断和错误", 1405, 350, 285, 110, "AmbientDevice/debugstore.cpp", 44, "Debug 只观察业务状态，不能被业务逻辑反向读取；10 个 Reserved 槽保持独立。", "双线程并发写计数与读取 snapshot，检查 revision 单调递增、无表格越界和死锁。", "“互斥锁：拿不到锁就睡眠，等待唤醒。”（笔记第10页）"),
            Module("scheduler", "S04", "AmbientLinScheduler", "GUI/Worker 边界、请求编号与控制合并", 1075, 390, 300, 115, "AmbientDevice/ambientlinscheduler.cpp", 12, "Scheduler 只协调线程，不直接访问串口；对外 API 固定在 GUI 线程调用。", "快速切换目标、读写诊断和关闭窗口，检查请求 ID 唯一且过期信号不会覆盖新状态。", "同一进程的线程共享堆与全局数据，因此边界必须通过同步明确所有权。"),
            Module("qthread", "I02", "QThread + QueuedConnection", "参数复制进 Worker 事件队列", 740, 390, 265, 115, "AmbientDevice/ambientlinscheduler.cpp", 65, "moveToThread 后槽函数在 Worker 线程执行；QueuedConnection 是线程内消息传递，不是 IPC。", "在发送端记录 GUI threadId、接收端记录 Worker threadId；验证队列次序及关闭时无 QThread destroyed 警告。", "“线程本质上是一个执行实体。”（笔记第11页）"),
            Module("worker", "S05", "LinBusWorker 初始化", "校验布局、打开设备、启动单次调度定时器", 740, 650, 285, 120, "AmbientDevice/linbusworker.cpp", 53, "串口、Schedule、诊断 FIFO 只由该线程维护，以单一所有者代替大范围加锁。", "使用不可打开设备、非法布局和中断请求三种桩，确认均安全退出且 DebugLastError 可定位。", "竞争条件可通过同步防止；这里优先用单一所有者串行化关键资源。"),
            Module("device", "H02", "/dev/ttySAC3 与内核驱动", "用户态 open/ioctl/poll/read/write 的硬件入口", 1085, 650, 300, 120, "AmbientDevice/ambientlincomm.cpp", 69, "桌面平台明确返回不可用；ARM Linux 才进入真实 ttySAC3 驱动契约。", "用伪设备/系统调用包装桩注入 open、ioctl、poll、read、write 失败，逐一核对错误码与资源关闭。", "“软件中断由指令触发，用于系统调用。”（笔记第5页）"),
        ],
    ),
    (
        "02_TX_RX_IRQ运行链",
        "蓝色 TX 向下，绿色 RX 向上；紫色 IRQ/唤醒独立于 LIN Payload",
        [
            Module("ui", "S06", "BCMMasterFrame 用户操作", "按钮、滑块、30 个组合快捷项", 65, 145, 255, 110, "AmbientDevice/bcmmasterframe.cpp", 481, "GUI 只修改 BCMSignal 或选择生成的组合，不直接拼接 LIN 字节。", "连续点击颜色、Enable、地址与组合按钮，检查最终 UI 值与发出的主控制 Payload 一致。", "GUI 主线程有自己的栈和执行上下文，只应处理短小的界面工作。"),
            Module("coalesce", "I03", "20 ms 合并定时器", "连续控制只保留最新值", 65, 315, 255, 105, "AmbientDevice/ambientlinscheduler.cpp", 187, "QTimer 不创建新线程，只把超时事件重新放入 GUI 事件队列。", "1 ms 间隔注入 100 次亮度变化，验证 Worker 收到少量更新且最后值不丢失。", "CPU 调度决定线程何时运行；Qt 定时器只决定事件何时具备被处理的条件。"),
            Module("queue", "I04", "QueuedConnection 事件队列", "跨线程按值复制控制与诊断命令", 65, 485, 255, 105, "AmbientDevice/ambientlinscheduler.cpp", 75, "GUI 不等待总线 I/O；消息进入 Worker 队列后由其事件循环顺序消费。", "阻塞 Worker 50 ms 后连续排队三类命令，检查参数未被发送端后续修改污染、顺序符合设计。", "线程通信发生在同一进程内，不等同于管道、共享内存或 Socket IPC。"),
            Module("priority", "S07", "LinBusWorker 优先控制", "更新缓存 Payload，并抢占到最近控制槽", 65, 655, 275, 115, "AmbientDevice/linbusworker.cpp", 145, "优先帧后强制让出至少一个普通槽，兼顾按钮响应与状态轮询，避免饥饿。", "让15个从机全部超时后点击按钮，量测控制帧延迟；再持续拖动，验证普通状态槽仍推进。", "“本质上是调度实体的切换才会引起上下文切换。”（笔记第6页）"),
            Module("layout", "S08", "LinLayout 位域编码", "依据 LDF 生成的 startBit/bitLength 写 Payload", 405, 655, 260, 115, "AmbientDevice/linlayout.cpp", 1111, "运行时代码不认识具体车型信号；所有边界、默认值和位宽来自生成布局。", "对每个1/2/8/16/32位信号做边界值与越界值桩；比较编码后字节和 LDF 位定义。", "生成表位于进程的静态数据区域，线程共享读取但不修改。"),
            Module("comm", "S09", "AmbientLinComm 系统调用封装", "PID、Checksum、超时和驱动状态检查", 730, 655, 260, 115, "AmbientDevice/ambientlincomm.cpp", 170, "把 LIN 协议字节和 Linux I/O 错误统一封装，Worker 只接收成功/失败及错误文本。", "为 sendFrame/readResponse 提供 echo、PID错、checksum错、超时和驱动状态非0五类桩。", "用户态不能直接控制硬件，只能通过系统调用请求内核服务。"),
            Module("cpu", "H03", "CPU", "", 1110, 125, 220, 90, "AmbientDevice/ambientlinscheduler.cpp", 142, "Linux 调度器在 GUI 与 Worker 等执行实体间分配 CPU；NormalPriority 不是硬实时保证。", "记录高负载下线程唤醒、上下文切换和最坏按钮延迟，避免把平均值当实时上限。", "“线程本质上是一个执行实体。”（笔记第11页）"),
            Module("sysbus", "H04", "系统总线 / 寄存器访问", "内核驱动把系统调用转换为控制器寄存器操作", 1080, 290, 280, 100, "AmbientDevice/ambientlincomm.cpp", 139, "应用不直接写寄存器；ioctl 进入驱动后才配置 UART-to-LIN 模式。", "驱动桩记录 ioctl 参数与调用顺序，检查设备未成功打开时不进入寄存器配置。", "系统调用形成用户态到内核态的受控入口，具体中断实现由 ARM Linux 驱动负责。"),
            Module("controller", "H05", "LIN / UART Controller", "CONFIG → REG OK → ENABLE/START → READY", 1050, 500, 340, 125, "AmbientDevice/ambientlincomm.cpp", 102, "控制器生命周期必须完成配置和 Ready 反馈后才允许 TX/RX；Break/Sync 由驱动契约处理。", "分别卡住 CONFIG、ENABLE 和 READY 阶段，验证上层不会误报总线可用，并能关闭文件描述符。", "硬件中断先由内核响应，应用看到的是 poll/read/write 的返回结果。"),
            Module("linbus", "H06", "LIN PHY / 总线 / 从机", "Break + Sync + PID + Data + Checksum", 1050, 735, 340, 115, "AmbientDevice/ambientlincomm.cpp", 233, "TX 蓝线只承载主机 Payload，RX 绿线只承载从机响应；IRQ 紫线不混入数据路径。", "接真实分析仪检查 Raw ID、PID、Break 长度、Checksum；断开从机确认超时只影响对应轮询槽。", "中断是硬件异步事件；用户态通过驱动提供的等待和读写接口间接感知。"),
            Module("status", "S10", "状态校验、解码与 GUI 回显", "PID/Checksum 通过后按 LinLayout 解码", 1430, 260, 270, 120, "AmbientDevice/linlayout.cpp", 1182, "坏帧不更新在线/错误状态；正确状态通过排队信号回到 GUI 线程。", "构造正常、短帧、PID错、checksum错和每个故障位，验证 SlaveStatus 与界面颜色。", "同一进程线程共享数据，但 QWidget 更新仍必须回到 GUI 线程执行。"),
        ],
    ),
    (
        "03_诊断_Debug_LDF母Seed",
        "构建期数据、运行期调度、诊断状态机、可观测性和安全退出",
        [
            Module("ldf", "S11", "LDF + 语义 Overlay", "LDF 给传输事实，JSON 补 UI/私有 DID", 55, 145, 250, 105, "profiles/xiaopeng_ambient.profile.json", 1, "协议事实与应用语义分离；新车型不应修改线程和串口框架。", "对新 LDF 先跑 init，逐项审查 TODO；错误信号名、NAD、帧长和 DID 必须被生成器拒绝。", "数据文件由工具进程读取，生成后成为目标进程可验证的静态布局。"),
            Module("generator", "S12", "LDF Profile Generator", "解析、合并、严格校验并生成 C++", 365, 145, 270, 105, "tools/ldf_profile_gen.py", 1053, "生成器是离线构建工具，不参与目标机实时调度；输出固定文件名供 .pro 引用。", "分别运行 generate/check；改动输入后 check 必须失败，重新 generate 后才恢复一致。", "每个进程拥有独立地址空间；生成工具和 Qt 上位机是两个不同生命周期的进程。"),
            Module("generated", "S13", "Generated LinLayout", "帧、信号、节点、Schedule、诊断服务表", 695, 145, 275, 105, "AmbientDevice/generated/linprofile_generated.cpp", 160, "运行时只读生成表；快捷组合也被解析为明确的位位置和值。", "读取 report.json 对比 LDF 哈希、节点数、控制帧和 Schedule 槽数；禁止手改 generated C++。", "同一进程线程可共享代码段和静态数据，但应把生成布局视为不可变。"),
            Module("validate", "S14", "启动 Layout 校验", "非法数据在打开设备之前被阻止", 1030, 145, 275, 105, "AmbientDevice/linlayout.cpp", 382, "检查指针/数量配对、索引、位宽、帧边界、服务与 Schedule，避免坏表进入总线。", "逐项破坏指针、count、primary index 和 preset 位宽，验证错误文本精确且不会 open 设备。", "“一个进程的任一线程出现段错误，都会终止整个进程。”（笔记第12页）"),
            Module("schedule", "I05", "正常 LIN Schedule", "单次 QTimer 每次推进一个槽", 55, 410, 270, 110, "AmbientDevice/linbusworker.cpp", 473, "不是忙等死循环；Profile 决定控制帧与节点轮询顺序，Worker 只执行通用算法。", "用虚拟时钟推进 100 个槽，校验 index、delay、优先控制插入和普通槽公平性。", "CPU Scheduler、Qt 事件调度和 LIN Schedule 是三个不同层次。"),
            Module("fifo", "S15", "诊断有界 FIFO", "读/写/标定请求进入 Worker 单线程队列", 385, 410, 270, 110, "AmbientDevice/linbusworker.cpp", 265, "队列只在 Worker 内维护；入口检查 requestId、NAD、Bus 状态和最大深度。", "填满队列再入队，验证明确拒绝；取消中间任务后其余请求顺序不乱且均有结果。", "竞争条件通过同步和所有权控制，关键队列不允许多线程直接并发修改。"),
            Module("diagnostic", "I06", "诊断状态机 / P2 / ST", "完整服务之间才穿插控制帧", 715, 410, 285, 110, "AmbientDevice/linbusworker.cpp", 561, "传输层交换不能被拆开；失败、取消或完成后恢复正常 Schedule 和初始 NAD。", "注入首帧/连续帧、负响应、超时、取消和临时 NAD 失败，检查每条退出路径都恢复状态。", "等待 I/O 时线程会睡眠并在事件或超时后唤醒，避免无意义占用 CPU。"),
            Module("debugsnapshot", "S16", "DebugStore Mutex 快照", "短锁保护写入、计数和只读 snapshot", 1060, 410, 280, 110, "AmbientDevice/debugstore.cpp", 64, "锁内不等待串口、不调用 UI；业务从不读取 Debug 值作控制条件。", "Worker 高频写 TX/RX，GUI 每250 ms取快照，检查互斥等待短、计数不回退、10个Reserved仍存在。", "“互斥锁：拿不到锁就睡眠，等待唤醒。”（笔记第10页）"),
            Module("panel", "I07", "DebugPanel 250 ms 刷新", "F12 展示状态镜像，不阻塞业务", 1400, 410, 285, 110, "AmbientDevice/debugpanel.cpp", 90, "面板定时读取完整快照；诊断界面只提示成功 OK，失败细节留在 Debug。", "打开面板运行10分钟，检查内存、行数、时间戳、ActiveSignalPreset 和关闭后 timer 停止。", "观察工具不能变成新的耦合点；GUI 事件循环只做快照显示。"),
            Module("stop", "S17", "关闭窗口 / 请求停止", "中断 Worker、拒绝新任务并等待最多5秒", 385, 690, 285, 115, "AmbientDevice/ambientlinscheduler.cpp", 145, "先通知、再等待、最后释放 QThread；不使用 terminate() 强杀线程。", "分别在正常轮询、诊断等待、设备失败时关闭窗口，验证均在上限内退出且没有残留线程。", "线程属于进程；必须在进程结束前有序回收其执行上下文和资源。"),
            Module("close", "H07", "关闭设备与资源回收", "停定时器、close(fd)、发 stopped、退出线程", 760, 690, 300, 115, "AmbientDevice/linbusworker.cpp", 927, "文件描述符只由 Worker 打开和关闭，避免跨线程 close 与正在进行的 I/O 竞争。", "给 close、等待和 stopped 信号加计数桩，确认每次生命周期只执行一次并且 fd 归零。", "系统调用资源属于进程；退出前必须把内核对象显式释放或交由内核回收。"),
        ],
    ),
]


# Re-layout the runtime pages as explicit snake-shaped flowcharts.  Keeping
# coordinates here avoids burying the reading order inside the long content
# records above.
SEED_MODULE_WIDTH = 289
SEED_MODULE_HEIGHT = 62


FLOW_LAYOUTS: Dict[str, Tuple[int, int, int, int]] = {
    # Mother Seed page 03 uses 289 x 62 as its normal runtime module.  Keep
    # that geometry instead of inventing larger cards for this project.
    # Page 1: GUI row -> queued handoff -> Worker row.
    "loader": (60, 150, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "main": (390, 150, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "eventloop": (720, 150, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "window": (1050, 150, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "scheduler": (1050, 350, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "qthread": (720, 350, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "worker": (720, 650, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "device": (1050, 650, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "debug": (1410, 350, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    # Page 2: TX input row -> controller row -> LIN/RX row.
    "ui": (35, 145, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "coalesce": (365, 145, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "queue": (695, 145, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "priority": (1075, 145, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "layout": (1075, 400, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "comm": (745, 400, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "controller": (365, 400, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "linbus": (365, 700, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "status": (745, 700, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "cpu": (35, 400, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "sysbus": (35, 520, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    # Page 3: build row -> diagnostic row -> observe/shutdown row.
    "ldf": (55, 145, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "generator": (375, 145, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "generated": (695, 145, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "validate": (1015, 145, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "schedule": (1015, 410, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "fifo": (695, 410, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "diagnostic": (375, 410, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "debugsnapshot": (375, 690, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "panel": (695, 690, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "stop": (1015, 690, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
    "close": (1335, 690, SEED_MODULE_WIDTH, SEED_MODULE_HEIGHT),
}

for _page_index in (0, 1, 2):
    _title, _subtitle, _modules = PAGES[_page_index]
    PAGES[_page_index] = (
        _title,
        _subtitle,
        [
            Module(
                _module.key,
                _module.class_id,
                _module.name,
                _module.summary,
                *FLOW_LAYOUTS.get(
                    _module.key,
                    (_module.x, _module.y, _module.width, _module.height),
                ),
                _module.source,
                _module.line,
                _module.comment,
                _module.stub,
                _module.os_note,
            )
            for _module in _modules
        ],
    )


# Chinese functional titles and logic-only body text.  C++/Qt identifiers may
# appear as supporting terms, but the reader never has to infer a module's job
# from an English class name or a source path.
MODULE_COPY: Dict[str, Tuple[str, str]] = {
    "loader": ("程序装载与内存映射（Linux）", "创建进程，建立代码、堆、栈和共享库映射"),
    "main": ("程序入口 main()", "依次创建应用对象、调试仓库和主窗口"),
    "eventloop": ("主事件循环（QApplication）", "串行分发按钮、定时器和排队信号"),
    "window": ("主界面管理 MainWindow", "创建控制、诊断、调试页面并校验布局"),
    "debug": ("运行状态镜像 DebugStore", "跨线程记录收发、超时、诊断与错误状态"),
    "scheduler": ("任务协调器（LIN Scheduler）", "接收界面请求并投递给 LIN 工作线程"),
    "qthread": ("跨线程消息投递（Qt 队列）", "复制请求参数并送入 Worker 事件队列"),
    "worker": ("工作线程初始化（LinBusWorker）", "校验布局、打开设备并启动单次调度定时器"),
    "device": ("串口设备与内核驱动", "通过系统调用访问串口和内核 LIN 驱动"),
    "ui": ("用户控制界面", "把按钮、滑块和组合预设转换为信号值"),
    "coalesce": ("连续操作合并定时器", "合并连续操作，只提交最后一次控制值"),
    "queue": ("跨线程命令队列", "按顺序把控制和诊断命令送到工作线程"),
    "priority": ("控制帧优先调度", "控制帧优先发送，同时保证普通轮询继续"),
    "layout": ("信号位域编码（LDF）", "按 LDF 位域把信号值编码到 LIN 数据区"),
    "comm": ("系统调用封装（AmbientLinComm）", "统一处理 PID、校验和、超时及驱动状态"),
    "cpu": ("处理器（CPU）", "调度 GUI 线程和 LIN 工作线程执行"),
    "sysbus": ("内核总线与寄存器访问", "把系统调用转换为内核寄存器访问"),
    "controller": ("硬件控制器（LIN/UART）", "按配置、启动、就绪顺序驱动 LIN 控制器"),
    "linbus": ("物理总线与从机（LIN）", "发送主机帧并接收从机响应"),
    "status": ("从机状态解析与界面回显", "校验并解码响应，再排队回到 GUI 显示"),
    "ldf": ("协议输入（LDF 与语义配置）", "提供帧、信号、节点及应用语义配置"),
    "generator": ("上位机代码生成器（LDF）", "解析 LDF 与语义配置并生成通用 C++ 表"),
    "generated": ("生成的布局表（LIN）", "保存帧、信号、调度表和诊断服务定义"),
    "validate": ("启动前布局校验", "设备打开前阻止非法布局进入运行阶段"),
    "schedule": ("周期调度器（LIN Schedule）", "定时推进控制帧和从机轮询槽"),
    "fifo": ("诊断请求队列", "有界保存读、写、标定等诊断请求"),
    "diagnostic": ("诊断传输状态机（LIN）", "按 P2/ST 时序串行完成诊断传输"),
    "debugsnapshot": ("调试数据互斥快照", "用短互斥锁生成一致的只读调试快照"),
    "panel": ("调试界面周期刷新", "周期读取调试快照并显示定位信息"),
    "stop": ("安全停止工作线程", "拒绝新任务，通知 Worker 停止并等待退出"),
    "close": ("关闭串口并释放资源", "停止定时器、关闭设备并释放线程资源"),
}

for _page_index, (_title, _subtitle, _modules) in enumerate(PAGES):
    PAGES[_page_index] = (
        _title,
        _subtitle,
        [
            Module(
                _module.key,
                _module.class_id,
                MODULE_COPY[_module.key][0],
                MODULE_COPY[_module.key][1],
                _module.x,
                _module.y,
                _module.width,
                _module.height,
                _module.source,
                _module.line,
                _module.comment,
                _module.stub,
                _module.os_note,
            )
            for _module in _modules
        ],
    )


COLORS: Dict[str, Tuple[str, str, str]] = {
    "H": ("#1d1208", "#f59e0b", "硬件 / 内核"),
    "S": ("#071a28", "#32d5ff", "软件模块"),
    "I": ("#120d22", "#b68cff", "事件 / 中断"),
}

HEADER_COLORS: Dict[str, str] = {
    "H": "#351d06",
    "S": "#0b3047",
    "I": "#251642",
}

# These two stencils and every size below are copied from page 03 of the
# actual mother Seed.  Do not replace them with a hand-made rounded card.
SEED_ROUNDED_STENCIL = (
    "stencil(nZBLDsIwDERP4y0KiSp1X8oFOEFEDLEISZWW7+lJOyBVXbDozp559khD"
    "pum97Zi06oecLvwQN3gyO9JaoucsQ5nItGSaU8p8zukWHfbOFlKrSWGHm+1Ia2"
    "Xz8SBvhlZtqrqGjsNruo+BT9gK1uv7QGHfAw0SZ+jPXAX/QVeCi/gyTJWgLAkB"
    "jc79ZYVFQv2m/QA=)"
)
SEED_TEXT_STENCIL = (
    "stencil(nZBLDoAgDERP0z3SIyjew0SURgSD+Lu9kMZoXLhwN9O+tukAlrNpJg1S"
    "zDH4QW/URgNYgZTkjA4UkwJUgGXng+6DX1zLfmoymdXo17xh5zmRJ6Q42BWCfc"
    "2oJfdAr+Yv+AP9Cb7OJ3H/2JG1HNGz/84klThPVCc=)"
)
SEED_HEADER_HEIGHT = 21
SEED_OUTER_STROKE = 2
SEED_TOP_ACCENT_HEIGHT = 2
SEED_MODULE_HALO_WIDTH = 9
SEED_MODULE_HALO_OPACITY = 13
SEED_FLOW_STROKE = 3
SEED_FLOW_END_SIZE = 5
SEED_FLOW_GLOW_WIDTH = 8
SEED_FLOW_GLOW_OPACITY = 16


def style_join(*parts: str) -> str:
    return ";".join(part.strip(";") for part in parts if part) + ";"


def add_geometry(parent: ET.Element, x: float, y: float, width: float, height: float) -> ET.Element:
    return ET.SubElement(
        parent,
        "mxGeometry",
        {
            "x": str(x),
            "y": str(y),
            "width": str(width),
            "height": str(height),
            "as": "geometry",
        },
    )


def add_cell(
    root: ET.Element,
    cell_id: str,
    value: str,
    style: str,
    x: float,
    y: float,
    width: float,
    height: float,
    parent_id: str = "1",
    link: Optional[str] = None,
    tooltip: Optional[str] = None,
) -> ET.Element:
    attrs = {
        "id": cell_id,
        "value": value,
        "style": style,
        "parent": parent_id,
        "vertex": "1",
    }
    if link:
        attrs["link"] = link
    if tooltip:
        attrs["tooltip"] = tooltip
    cell = ET.SubElement(root, "mxCell", attrs)
    add_geometry(cell, x, y, width, height)
    return cell


def add_user_object(
    root: ET.Element,
    object_id: str,
    label: str,
    style: str,
    x: float,
    y: float,
    width: float,
    height: float,
    parent_id: str,
    link: str,
    tooltip: str,
    extra: Optional[Dict[str, str]] = None,
) -> ET.Element:
    attrs = {
        "id": object_id,
        "label": label,
        "link": link,
        "tooltip": tooltip,
    }
    if extra:
        attrs.update(extra)
    obj = ET.SubElement(root, "UserObject", attrs)
    cell = ET.SubElement(
        obj,
        "mxCell",
        {
            "id": object_id + "-cell",
            "style": style,
            "parent": parent_id,
            "vertex": "1",
        },
    )
    add_geometry(cell, x, y, width, height)
    return obj


def extract_seed_visual_templates(mxfile: ET.Element) -> SeedVisualTemplates:
    """Extract the exact module and arrow cells; do not reconstruct styles."""
    diagrams = mxfile.findall("diagram")
    if len(diagrams) < 3:
        raise RuntimeError("mother Seed must contain three visual template pages")
    runtime = diagrams[2]

    def raw_cell(cell_id: str) -> ET.Element:
        cell = runtime.find(".//mxCell[@id='{0}']".format(cell_id))
        if cell is None:
            raise RuntimeError("mother Seed is missing template cell " + cell_id)
        return copy.deepcopy(cell)

    def object_cell(object_id: str) -> Tuple[ET.Element, str]:
        obj = runtime.find(".//UserObject[@id='{0}']".format(object_id))
        if obj is None:
            raise RuntimeError("mother Seed is missing template object " + object_id)
        cell = obj.find("mxCell")
        if cell is None:
            raise RuntimeError("mother Seed template object has no mxCell: " + object_id)
        return copy.deepcopy(cell), obj.get("label", "")

    # Page 03, W1 TX software queue: one complete 289 x 62 runtime module.
    shell, _ = object_cell("14")
    header, header_label = object_cell("15")
    logic, logic_label = object_cell("16")
    process_edge, _ = object_cell("63")

    gate_edge: Optional[ET.Element] = None
    for cell in diagrams[0].iter("mxCell"):
        style = cell.get("style", "")
        if (
            cell.get("edge") == "1"
            and "strokeWidth=2;" in style
            and "endArrow=block;" in style
            and "endSize=7;" in style
            and "dashed=1;" in style
        ):
            gate_edge = copy.deepcopy(cell)
            break
    if gate_edge is None:
        raise RuntimeError("mother Seed is missing the dashed gate arrow template")

    return SeedVisualTemplates(
        module_full_halo=raw_cell("ai-neon-halo-3-7"),
        module_shell=shell,
        module_top_accent=raw_cell("ai-top-accent-3-5"),
        module_header_halo=raw_cell("ai-neon-halo-3-8"),
        module_header=header,
        module_logic=logic,
        module_header_label=header_label,
        module_logic_label=logic_label,
        process_edge=process_edge,
        gate_edge=gate_edge,
    )


def recolor_seed_style(style: str, class_id: str) -> str:
    fill, accent, _ = COLORS[class_id[0]]
    return (
        style.replace("#071a28", fill)
        .replace("#0b3047", HEADER_COLORS[class_id[0]])
        .replace("#32d5ff", accent)
    )


def set_shifted_seed_geometry(cell: ET.Element, dx: float, dy: float) -> None:
    geometry = cell.find("mxGeometry")
    if geometry is None:
        raise RuntimeError("cloned mother Seed cell has no geometry")
    if geometry.get("x") is not None:
        geometry.set("x", str(float(geometry.get("x", "0")) + dx))
    if geometry.get("y") is not None:
        geometry.set("y", str(float(geometry.get("y", "0")) + dy))


def add_edge(
    root: ET.Element,
    templates: SeedVisualTemplates,
    edge_id: str,
    source: str,
    target: str,
    label: str,
    kind: str,
    points: Sequence[Tuple[int, int]] = (),
) -> ET.Element:
    colors = {
        "tx": "#32d5ff",
        "rx": "#46ad92",
        "irq": "#b68cff",
        "control": "#f59e0b",
        "gate": "#22d3ee",
        "flow": "#5a7183",
    }
    color = colors.get(kind, colors["flow"])
    # Clone the actual mother Seed edge cell.  No generated glow is added:
    # the Seed does not pair every connector with a glow, and doing so made
    # the runtime diagram visibly heavier than its source.
    cell = copy.deepcopy(templates.gate_edge if kind == "gate" else templates.process_edge)
    cell.set("id", edge_id)
    # UserObject 63 in the mother Seed has an empty label.  Keep it empty;
    # inline text changes the visual weight and cuts the connector shaft.
    cell.set("value", "")
    cell.set("parent", "1")
    cell.set("source", source)
    cell.set("target", target)
    cell.set("edge", "1")
    cell.set("flowRole", "gate-control" if kind == "gate" else "process")
    cell.set("seedKind", kind)
    seed_color = "#22d3ee" if kind == "gate" else "#32d5ff"
    cell.set("style", cell.get("style", "").replace(seed_color, color))
    geometry = cell.find("mxGeometry")
    if geometry is None:
        geometry = ET.SubElement(cell, "mxGeometry", {"relative": "1", "as": "geometry"})
    else:
        geometry.set("relative", "1")
        geometry.set("as", "geometry")
        for child in list(geometry):
            geometry.remove(child)
    array = ET.SubElement(geometry, "Array", {"as": "points"})
    for x, y in points:
        ET.SubElement(array, "mxPoint", {"x": str(x), "y": str(y)})
    root.append(cell)
    return cell


def vscode_link(source: str, line: int) -> str:
    absolute = (WORKSPACE / source).resolve().as_posix()
    encoded = urllib.parse.quote(absolute, safe="/:")
    return "vscode://file/{0}:{1}".format(encoded, line)


def module_style(class_id: str) -> str:
    fill, accent, _ = COLORS[class_id[0]]
    return style_join(
        "fillColor=" + fill,
        "gradientColor=none",
        "shape=" + SEED_ROUNDED_STENCIL,
        "strokeColor=" + accent,
        "strokeWidth=" + str(SEED_OUTER_STROKE),
        "spacingTop=-2",
        "spacingBottom=-2",
        "spacingLeft=1",
        "spacingRight=1",
        "points=[]",
        "labelBackgroundColor=none",
        "rounded=1",
        "html=1",
        "whiteSpace=wrap",
        "fontColor=#e7f0f5",
        "shadow=1",
    )


def module_header_style(class_id: str) -> str:
    accent = COLORS[class_id[0]][1]
    return style_join(
        "verticalAlign=middle",
        "align=center",
        "overflow=width",
        "rounded=1",
        "html=1",
        "whiteSpace=wrap",
        "fillColor=" + HEADER_COLORS[class_id[0]],
        "gradientColor=none",
        "shape=" + SEED_ROUNDED_STENCIL,
        "strokeColor=" + accent,
        "strokeWidth=0",
        "spacingTop=-2",
        "spacingBottom=-2",
        "spacingLeft=1",
        "spacingRight=1",
        "points=[]",
        "labelBackgroundColor=none",
        "fontColor=#f3f8fb",
        "fontFamily=Microsoft YaHei",
        "shadow=1",
    )


def module_body_style() -> str:
    return style_join(
        "verticalAlign=middle",
        "align=center",
        "overflow=width",
        "html=1",
        "whiteSpace=wrap",
        "fillColor=none",
        "gradientColor=none",
        "shape=" + SEED_TEXT_STENCIL,
        "strokeColor=none",
        "spacingTop=-2",
        "spacingBottom=-2",
        "spacingLeft=1",
        "spacingRight=1",
        "points=[]",
        "labelBackgroundColor=none",
        "rounded=0",
        "fontColor=#e7f0f5",
        "fontFamily=Microsoft YaHei",
    )


def add_module(root: ET.Element, module: Module, templates: SeedVisualTemplates) -> None:
    """Clone the six original W1 module cells and replace content only."""
    if module.width != 289 or module.height != 62:
        raise RuntimeError("normal module must retain the cloned mother Seed size 289x62")
    link = vscode_link(module.source, module.line)
    tooltip = "点击打开对应代码"
    header_value = templates.module_header_label.replace(
        "W1 TX软件队列【S01】", html_module.escape(module.name)
    )
    logic_value = templates.module_logic_label.replace(
        "吸收应用与硬件发送速度差；明确报文所有权。",
        html_module.escape(module.summary),
    )
    dx = module.x - 68
    dy = module.y - 473
    parts = (
        (templates.module_full_halo, module.key + "-halo", None, False),
        (templates.module_shell, module.key, "", True),
        (templates.module_top_accent, module.key + "-top-accent", None, False),
        (templates.module_header_halo, module.key + "-header-halo", None, False),
        (templates.module_header, module.key + "-header", header_value, True),
        (templates.module_logic, module.key + "-logic", logic_value, True),
    )
    for template, cell_id, value, clickable in parts:
        cell = copy.deepcopy(template)
        cell.set("id", cell_id)
        cell.set("parent", "1")
        cell.set("vertex", "1")
        cell.set("style", recolor_seed_style(cell.get("style", ""), module.class_id))
        if value is not None:
            cell.set("value", value)
        else:
            cell.attrib.pop("value", None)
        if clickable:
            cell.set("link", link)
            cell.set("tooltip", tooltip)
        if cell_id == module.key:
            cell.set("seedClass", module.class_id[0])
        set_shifted_seed_geometry(cell, dx, dy)
        root.append(cell)


def action_link(detail_id: str, all_detail_ids: Sequence[str]) -> str:
    actions: List[Dict[str, Dict[str, List[str]]]] = []
    other = [item for item in all_detail_ids if item != detail_id]
    if other:
        actions.append({"hide": {"cells": other}})
    actions.append({"toggle": {"cells": [detail_id]}})
    return "data:action/json," + json.dumps(
        {"actions": actions}, ensure_ascii=False, separators=(",", ":")
    )


def close_link(detail_id: str) -> str:
    return "data:action/json," + json.dumps(
        {"actions": [{"toggle": {"cells": [detail_id]}}]},
        ensure_ascii=False,
        separators=(",", ":"),
    )


def add_header(root: ET.Element, page_index: int, title: str, subtitle: str) -> None:
    add_cell(
        root,
        "page-{0}-title".format(page_index),
        '<div style="font-size:28px;letter-spacing:1px"><b>AMBIENTDEVICE · {0}</b></div>'.format(html_module.escape(title)),
        style_join("text", "html=1", "align=left", "verticalAlign=middle", "fontColor=#e7f0f5", "strokeColor=none", "fillColor=none"),
        55,
        30,
        1180,
        42,
    )
    add_cell(
        root,
        "page-{0}-subtitle".format(page_index),
        html_module.escape(subtitle),
        style_join("text", "html=1", "align=left", "fontColor=#65cfe8", "fontSize=14", "strokeColor=none", "fillColor=none"),
        58,
        76,
        1120,
        30,
    )
    add_cell(
        root,
        "page-{0}-legend".format(page_index),
        '<span style="color:#f59e0b"><b>H</b> 硬件/内核</span>　'
        '<span style="color:#32d5ff"><b>S</b> 软件</span>　'
        '<span style="color:#b68cff"><b>I</b> 事件/中断</span>　'
        '<span style="color:#7f9aa8">点击标注看测试桩 · 点击模块进 VS Code</span>',
        style_join("rounded=1", "html=1", "align=center", "verticalAlign=middle", "fillColor=#07131f", "strokeColor=#1a4254", "fontColor=#dcebf1", "fontSize=12"),
        1180,
        35,
        525,
        48,
    )


def add_lane(root: ET.Element, lane_id: str, title: str, x: int, y: int, width: int, height: int, color: str) -> None:
    add_cell(
        root,
        lane_id,
        '<div style="font-size:12px;letter-spacing:2px;color:{0}"><b>{1}</b></div>'.format(color, html_module.escape(title)),
        style_join("rounded=1", "arcSize=8", "html=1", "align=left", "verticalAlign=top", "spacingTop=8", "spacingLeft=12", "fillColor=#040d18", "opacity=70", "strokeColor=" + color, "dashed=1", "dashPattern=4 8", "strokeWidth=1"),
        x,
        y,
        width,
        height,
    )


def add_tag_and_card(
    root: ET.Element,
    module: Module,
    wrapper_index: int,
    all_detail_ids: Sequence[str],
) -> None:
    wrapper_id = "node-H{0:02d}".format(wrapper_index)
    detail_id = "layer-detail-H{0:02d}".format(wrapper_index)
    fill, accent, category = COLORS[module.class_id[0]]
    tag_x = module.x + module.width - 68
    tag_y = module.y - 34
    tag_style = style_join(
        "rounded=1",
        "arcSize=50",
        "whiteSpace=wrap",
        "html=1",
        "align=center",
        "verticalAlign=middle",
        "fillColor=#030914",
        "strokeColor=" + accent,
        "strokeWidth=2",
        "fontColor=" + accent,
        "fontSize=14",
        "fontStyle=1",
        "shadow=1",
    )
    add_user_object(
        root,
        wrapper_id,
        '<b>{0}</b>'.format(module.class_id),
        tag_style,
        tag_x,
        tag_y,
        58,
        28,
        "1",
        action_link(detail_id, all_detail_ids),
        "点击展开；再次点击收起 {0} · {1}".format(module.class_id, module.name),
        {
            "tags": "annotation init-" + module.class_id[0],
            "init_ids": module.class_id,
        },
    )

    longest_text = max(len(module.comment), len(module.stub), len(module.os_note))
    card_width = max(480, min(570, 480 + max(0, longest_text - 48) * 2))
    chars_per_line = max(30, int((card_width - 42) / 13))
    wrapped_line_count = sum(
        max(1, int(math.ceil(len(text) / float(chars_per_line))))
        for text in (module.comment, module.stub, module.os_note)
    )
    body_height = 80 + (wrapped_line_count * 18)
    card_height = 78 + body_height + 45
    card_x = module.x + module.width + 28 if module.x < 850 else module.x - card_width - 28
    card_x = max(30, min(PAGE_WIDTH - card_width - 30, card_x))
    card_y = max(125, min(PAGE_HEIGHT - card_height - 35, module.y - 70))
    add_cell(
        root,
        "panel-H{0:02d}-bg".format(wrapper_index),
        "",
        style_join("rounded=1", "arcSize=10", "fillColor=#050c16", "strokeColor=" + accent, "strokeWidth=3", "shadow=1"),
        card_x,
        card_y,
        card_width,
        card_height,
        detail_id,
    )
    add_cell(
        root,
        "panel-H{0:02d}-title".format(wrapper_index),
        '<div style="font-size:19px;color:{0}"><b>[{1}] {2}</b></div>'
        '<div style="font-size:11px;color:#7894a2;margin-top:3px">{3} · {4}:{5}</div>'.format(
            accent,
            module.class_id,
            html_module.escape(module.name),
            category,
            html_module.escape(module.source),
            module.line,
        ),
        style_join("text", "html=1", "align=left", "verticalAlign=middle", "spacing=8", "fillColor=none", "strokeColor=none", "fontColor=#e7f0f5"),
        card_x + 18,
        card_y + 14,
        card_width - 75,
        58,
        detail_id,
    )
    body = (
        '<div style="font-size:13px;line-height:1.48;color:#e7f0f5">'
        '<div><b style="color:{0}">功能批注</b><br/>{1}</div>'
        '<div style="margin-top:10px"><b style="color:#ffd166">重要测试桩</b><br/>{2}</div>'
        '<div style="margin-top:10px"><b style="color:#8ae6c1">操作系统笔记映射</b><br/>{3}</div>'
        "</div>"
    ).format(
        accent,
        html_module.escape(module.comment),
        html_module.escape(module.stub),
        html_module.escape(module.os_note),
    )
    add_cell(
        root,
        "panel-H{0:02d}-body".format(wrapper_index),
        body,
        style_join("text", "html=1", "whiteSpace=wrap", "overflow=fill", "align=left", "verticalAlign=top", "spacing=10", "fillColor=none", "strokeColor=none", "fontColor=#e7f0f5"),
        card_x + 16,
        card_y + 78,
        card_width - 32,
        body_height,
        detail_id,
    )
    add_user_object(
        root,
        "panel-H{0:02d}-code".format(wrapper_index),
        "在 VS Code 打开源码 ↗",
        style_join("rounded=1", "arcSize=15", "html=1", "align=center", "verticalAlign=middle", "fillColor=" + fill, "strokeColor=" + accent, "fontColor=#ffffff", "fontSize=12", "fontStyle=1"),
        card_x + 18,
        card_y + card_height - 37,
        185,
        27,
        detail_id,
        vscode_link(module.source, module.line),
        "打开 {0}:{1}".format(module.source, module.line),
    )
    add_user_object(
        root,
        "panel-H{0:02d}-close".format(wrapper_index),
        "× 关闭",
        style_join("rounded=1", "arcSize=15", "html=1", "align=center", "verticalAlign=middle", "fillColor=#281017", "strokeColor=#ff6b7d", "fontColor=#ffffff", "fontSize=12", "fontStyle=1"),
        card_x + card_width - 105,
        card_y + card_height - 37,
        85,
        27,
        detail_id,
        close_link(detail_id),
        "关闭当前批注",
    )


def page_edges(page_index: int, root: ET.Element, templates: SeedVisualTemplates) -> None:
    if page_index == 1:
        edges = [
            ("loader", "main", "", "flow", ()),
            ("main", "eventloop", "", "flow", ()),
            ("eventloop", "window", "", "flow", ()),
            ("window", "scheduler", "", "tx", ()),
            ("scheduler", "qthread", "", "irq", ()),
            ("qthread", "worker", "", "irq", ()),
            ("worker", "device", "", "tx", ()),
            ("worker", "debug", "状态镜像", "rx", ((864, 810), (1554, 810), (1554, 412))),
            ("debug", "window", "调试快照", "rx", ((1720, 381), (1720, 250), (1355, 250), (1355, 181))),
        ]
        for index, (source, target, label, kind, points) in enumerate(edges, 1):
            add_edge(root, templates, "p1-edge-{0}".format(index), source, target, label, kind, points)
    elif page_index == 2:
        edges = [
            ("ui", "coalesce", "", "tx", ()),
            ("coalesce", "queue", "", "tx", ()),
            ("queue", "priority", "", "tx", ()),
            ("priority", "layout", "", "tx", ()),
            ("layout", "comm", "", "tx", ()),
            ("comm", "tx-gate", "", "tx", ()),
            ("tx-gate", "controller", "", "tx", ()),
            ("controller", "linbus", "", "tx", ()),
            ("linbus", "status", "", "rx", ()),
            ("status", "ui", "RX 返回界面", "rx", ((1465, 731), (1465, 850), (10, 850), (10, 176))),
            ("controller", "cpu", "", "irq", ()),
            ("cpu", "sysbus", "", "control", ()),
            ("sysbus", "controller", "配置 / 就绪", "control", ((344, 551), (344, 431))),
            ("queue", "tx-gate", "发送许可", "gate", ((839, 330), (699, 330))),
        ]
        for index, (source, target, label, kind, points) in enumerate(edges, 1):
            add_edge(root, templates, "p2-edge-{0}".format(index), source, target, label, kind, points)
    else:
        edges = [
            ("ldf", "generator", "", "flow", ()),
            ("generator", "generated", "", "flow", ()),
            ("generated", "validate", "", "flow", ()),
            ("validate", "schedule", "", "tx", ()),
            ("schedule", "fifo", "", "irq", ()),
            ("fifo", "diagnostic", "", "irq", ()),
            ("diagnostic", "debugsnapshot", "诊断镜像", "rx", ((320, 441), (320, 721))),
            ("debugsnapshot", "panel", "", "rx", ()),
            ("panel", "stop", "", "control", ()),
            ("stop", "close", "", "control", ()),
            ("diagnostic", "schedule", "诊断完成 · 恢复轮询", "rx", ((519, 300), (1159, 300))),
            ("schedule", "debugsnapshot", "写入运行镜像", "rx", ((1159, 610), (519, 610))),
        ]
        for index, (source, target, label, kind, points) in enumerate(edges, 1):
            add_edge(root, templates, "p3-edge-{0}".format(index), source, target, label, kind, points)


def page_lanes(page_index: int, root: ET.Element) -> None:
    if page_index == 1:
        add_lane(root, "p1-lane-gui", "GUI 主线程 / 事件循环", 35, 115, 1685, 360, "#32d5ff")
        add_lane(root, "p1-lane-worker", "LIN 工作线程 / 单一总线所有者", 360, 585, 1055, 180, "#b68cff")
    elif page_index == 2:
        add_lane(root, "p2-lane-tx", "① TX 输入与排队", 20, 105, 1380, 155, "#32d5ff")
        add_lane(root, "p2-lane-hw", "③ CPU / 寄存器 / IRQ", 20, 365, 315, 250, "#f59e0b")
        add_lane(root, "p2-lane-encode", "② 编码 → 门控 → 控制器", 345, 365, 1055, 175, "#32d5ff")
        add_lane(root, "p2-lane-rx", "④ LIN 总线 / RX 返回", 345, 665, 750, 150, "#46ad92")
        add_cell(
            root,
            "tx-gate",
            "REQ\nGATE",
            style_join("rhombus", "html=1", "align=center", "verticalAlign=middle", "fillColor=#24170c", "strokeColor=#ff9d5c", "strokeWidth=2", "fontColor=#ffffff", "fontSize=10"),
            674,
            406,
            50,
            50,
        )
    else:
        add_lane(root, "p3-lane-build", "① 构建期 · LDF 母 SEED", 30, 110, 1315, 145, "#32d5ff")
        add_lane(root, "p3-lane-runtime", "② 运行期 · SCHEDULE / DIAGNOSTIC", 330, 365, 1010, 155, "#b68cff")
        add_lane(root, "p3-lane-observe", "③ 观察 → 用户关闭 → 安全回收", 330, 645, 1335, 155, "#46ad92")


def build_drawio(seed_clone: Path, output: Path) -> Tuple[List[Module], Dict[str, int]]:
    tree = ET.parse(seed_clone)
    mxfile = tree.getroot()
    diagrams = mxfile.findall("diagram")
    if len(diagrams) < 3:
        raise RuntimeError("mother Seed must contain at least three pages")
    templates = extract_seed_visual_templates(mxfile)

    all_modules = [module for _, _, modules in PAGES for module in modules]
    wrapper_by_key = {module.key: index for index, module in enumerate(all_modules, 1)}
    detail_ids = ["layer-detail-H{0:02d}".format(index) for index in range(1, len(all_modules) + 1)]
    class_ids = [module.class_id for module in all_modules]
    if len(class_ids) != len(set(class_ids)):
        raise RuntimeError("H/S/I class IDs must be unique")

    mxfile.set("pages", "3")
    mxfile.set("generatedFromMotherSeed", "AI科技界面_母Seed.drawio")
    mxfile.set("sourceMotherSeed", "C:/Users/SH2304013/Desktop/AI科技界面_母Seed.drawio")
    mxfile.set("generatedTitle", "AmbientDevice Qt Linux LIN Runtime")
    mxfile.set("detailAction", "exclusive-toggle")
    mxfile.set("initClassScheme", "H=hardware/kernel;S=software;I=event/interrupt")
    mxfile.set("initClassIds", ",".join(class_ids))
    mxfile.set("hardwarePayloadRule", "H/S/I annotations stay outside modules and TX/RX/IRQ paths")
    mxfile.set("flowArrowContract", "literal-clone:page03/UserObject63;gate-only-dashed")
    mxfile.set(
        "moduleVisualContract",
        "literal-clone:page03/W1-cells14-16+halos3-7,3-8+accent3-5",
    )

    module_by_key = {module.key: module for module in all_modules}
    for page_index, (diagram, page_data) in enumerate(zip(diagrams[:3], PAGES), 1):
        page_name, subtitle, modules = page_data
        old_model = diagram.find("mxGraphModel")
        if old_model is None:
            raise RuntimeError("page {0} has no mxGraphModel".format(page_index))
        overlay = old_model.find(".//mxCell[@id='ai-native-flow-{0}']".format(page_index))
        if overlay is None:
            raise RuntimeError("page {0} has no native waterfall overlay".format(page_index))
        overlay = copy.deepcopy(overlay)

        diagram.set("name", page_name)
        diagram.remove(old_model)
        model = ET.SubElement(
            diagram,
            "mxGraphModel",
            {
                "dx": str(PAGE_WIDTH),
                "dy": str(PAGE_HEIGHT),
                "grid": "0",
                "gridSize": "10",
                "guides": "1",
                "tooltips": "1",
                "connect": "1",
                "arrows": "1",
                "fold": "1",
                "page": "1",
                "pageScale": "1",
                "pageWidth": str(PAGE_WIDTH),
                "pageHeight": str(PAGE_HEIGHT),
                "background": "#030914",
                "math": "0",
                "shadow": "0",
            },
        )
        root = ET.SubElement(model, "root")
        ET.SubElement(root, "mxCell", {"id": "0"})
        ET.SubElement(root, "mxCell", {"id": "1", "parent": "0"})

        for module in modules:
            wrapper_index = wrapper_by_key[module.key]
            ET.SubElement(
                root,
                "mxCell",
                {
                    "id": "layer-detail-H{0:02d}".format(wrapper_index),
                    "value": "{0} · {1}".format(module.class_id, module.name),
                    "parent": "0",
                    "visible": "0",
                },
            )

        add_header(root, page_index, page_name, subtitle)
        page_lanes(page_index, root)
        for module in modules:
            add_module(root, module, templates)
        page_edges(page_index, root, templates)
        for module in modules:
            add_tag_and_card(root, module, wrapper_by_key[module.key], detail_ids)

        # Preserve the mother Seed's fixed, pointer-transparent native waterfall
        # as the final visual cell so it remains in the foreground.
        root.append(overlay)

    # Remove unused extra pages if a future seed contains more than three.
    for diagram in diagrams[3:]:
        mxfile.remove(diagram)

    ET.indent(tree, space="  ")
    output.parent.mkdir(parents=True, exist_ok=True)
    tree.write(output, encoding="utf-8", xml_declaration=True)
    return all_modules, wrapper_by_key


def write_html(drawio_path: Path, output: Path) -> None:
    xml = drawio_path.read_text(encoding="utf-8")
    config = {
        "center": True,
        "highlight": "#32D5FF",
        "nav": True,
        "resize": True,
        "fit": True,
        "toolbar": "zoom layers lightbox",
        "xml": xml,
    }
    config_attr = html_module.escape(
        json.dumps(config, ensure_ascii=False, separators=(",", ":")),
        quote=True,
    )
    document = """<!doctype html>
<html lang="zh-CN">
<head>
  <meta charset="utf-8">
  <meta name="viewport" content="width=device-width,initial-scale=1">
  <title>AmbientDevice · Qt / Linux / LIN 全链路运行原理</title>
  <style id="drawio-ai-export-shell">
    html,body{margin:0!important;min-height:100%!important;background:#030914!important;}
    body{min-height:100vh!important;overflow-x:auto!important;text-align:center!important;color:#e7f0f5;font-family:"Microsoft YaHei",sans-serif;}
    .mxgraph{display:block!important;width:fit-content!important;max-width:100%!important;margin:0 auto!important;min-height:100vh!important;background:#030914!important;border-color:#030914!important;text-align:left!important;}
    .loading-note{position:fixed;z-index:2;right:18px;bottom:14px;padding:8px 12px;border:1px solid #1a4b60;border-radius:8px;background:rgba(3,9,20,.82);color:#78cfe9;font-size:12px;pointer-events:none;}
  </style>
</head>
<body>
  <div class="mxgraph" style="display:block;width:fit-content;max-width:100%;margin:0 auto;min-height:100vh;background:#030914;border:1px solid #030914;text-align:left;" data-mxgraph="__CONFIG__"></div>
  <div class="loading-note">H/S/I：点击查看测试桩与功能批注 · 模块：点击打开 VS Code</div>
  <script src="https://viewer.diagrams.net/js/viewer-static.min.js"></script>
</body>
</html>
""".replace("__CONFIG__", config_attr)
    output.write_text(document, encoding="utf-8", newline="\n")


def write_preview_png(output: Path) -> None:
    """Render a compact three-page visual proof from the same module geometry."""
    from PIL import Image, ImageDraw, ImageFont

    scale = 0.58
    page_width = int(PAGE_WIDTH * scale)
    page_height = int(PAGE_HEIGHT * scale)
    margin = 34
    gap = 28
    canvas_width = (page_width * 2) + (margin * 2) + gap
    canvas_height = (page_height * 2) + (margin * 2) + gap + 55
    image = Image.new("RGB", (canvas_width, canvas_height), "#020711")
    draw = ImageDraw.Draw(image)
    font_path = Path("C:/Windows/Fonts/msyh.ttc")
    if not font_path.is_file():
        font_path = Path("C:/Windows/Fonts/simhei.ttf")
    title_font = ImageFont.truetype(str(font_path), 22)
    module_font = ImageFont.truetype(str(font_path), 13)
    module_title_font = ImageFont.truetype(str(font_path), 11)
    small_font = ImageFont.truetype(str(font_path), 10)
    tag_font = ImageFont.truetype(str(font_path), 11)

    page_origins = [
        (margin, margin + 35),
        (margin + page_width + gap, margin + 35),
        ((canvas_width - page_width) // 2, margin + page_height + gap + 35),
    ]
    preview_edges = [
        [
            ("loader", "main", "flow"), ("main", "eventloop", "flow"),
            ("eventloop", "window", "flow"), ("window", "scheduler", "tx"),
            ("scheduler", "qthread", "irq"), ("qthread", "worker", "irq"),
            ("worker", "device", "tx"),
        ],
        [
            ("ui", "coalesce", "tx"), ("coalesce", "queue", "tx"),
            ("queue", "priority", "tx"), ("priority", "layout", "tx"),
            ("layout", "comm", "tx"), ("comm", "controller", "tx"),
            ("controller", "linbus", "tx"), ("linbus", "status", "rx"),
            ("cpu", "sysbus", "control"),
        ],
        [
            ("ldf", "generator", "flow"), ("generator", "generated", "flow"),
            ("generated", "validate", "flow"), ("validate", "schedule", "tx"),
            ("schedule", "fifo", "irq"), ("fifo", "diagnostic", "irq"),
            ("debugsnapshot", "panel", "rx"),
            ("panel", "stop", "control"),
            ("stop", "close", "control"),
        ],
    ]
    edge_colors = {
        "tx": "#32d5ff", "rx": "#46ad92", "irq": "#b68cff",
        "control": "#f59e0b", "flow": "#5a7183",
    }

    def point(origin: Tuple[int, int], x: float, y: float) -> Tuple[int, int]:
        return (origin[0] + int(x * scale), origin[1] + int(y * scale))

    def draw_arrow(start: Tuple[int, int], end: Tuple[int, int], color: str) -> None:
        dx, dy = end[0] - start[0], end[1] - start[1]
        length = max(1.0, math.hypot(dx, dy))
        ux, uy = dx / length, dy / length
        nx, ny = -uy, ux
        # Preview the mother Seed's 3 px connector and endSize=5 block head,
        # not the former wide flexArrow polygon.
        draw.line((start, end), fill=color, width=2)
        head_half = 3.0
        head_length = min(5.0, length * 0.28)
        base_x, base_y = end[0] - ux * head_length, end[1] - uy * head_length
        polygon = [
            (base_x + nx * head_half, base_y + ny * head_half),
            end,
            (base_x - nx * head_half, base_y - ny * head_half),
        ]
        draw.polygon(polygon, fill=color)

    def draw_routed_arrow(route: Sequence[Tuple[int, int]], color: str) -> None:
        if len(route) < 2:
            return
        draw.line(route, fill=color, width=2, joint="curve")
        draw_arrow(route[-2], route[-1], color)

    draw.text((margin, 8), "AmbientDevice · Qt / Linux / LIN AI 科技运行原理 · 三页预览", fill="#e7f0f5", font=title_font)
    for page_index, ((page_name, subtitle, modules), origin) in enumerate(zip(PAGES, page_origins)):
        page_box = (origin[0], origin[1], origin[0] + page_width, origin[1] + page_height)
        draw.rounded_rectangle(page_box, radius=10, fill="#030914", outline="#1e5367", width=2)
        for gx in range(origin[0] + 14, origin[0] + page_width, 32):
            draw.line((gx, origin[1], gx, origin[1] + page_height), fill="#071422", width=1)
        for gy in range(origin[1] + 14, origin[1] + page_height, 32):
            draw.line((origin[0], gy, origin[0] + page_width, gy), fill="#071422", width=1)
        draw.text(point(origin, 45, 24), page_name, fill="#ffffff", font=title_font)
        draw.text(point(origin, 48, 68), subtitle, fill="#5fdcff", font=small_font)
        by_key = {module.key: module for module in modules}
        for source, target, kind in preview_edges[page_index]:
            if source not in by_key or target not in by_key:
                continue
            source_module = by_key[source]
            target_module = by_key[target]
            source_cx = source_module.x + source_module.width / 2
            source_cy = source_module.y + source_module.height / 2
            target_cx = target_module.x + target_module.width / 2
            target_cy = target_module.y + target_module.height / 2
            dx, dy = target_cx - source_cx, target_cy - source_cy
            if abs(dx) >= abs(dy):
                if dx >= 0:
                    start_xy = (source_module.x + source_module.width, source_cy)
                    end_xy = (target_module.x, target_cy)
                else:
                    start_xy = (source_module.x, source_cy)
                    end_xy = (target_module.x + target_module.width, target_cy)
            elif dy >= 0:
                start_xy = (source_cx, source_module.y + source_module.height)
                end_xy = (target_cx, target_module.y)
            else:
                start_xy = (source_cx, source_module.y)
                end_xy = (target_cx, target_module.y + target_module.height)
            start = point(origin, *start_xy)
            end = point(origin, *end_xy)
            draw_arrow(start, end, edge_colors[kind])
        routed_by_page = {
            0: [
                ([(864, 712), (864, 810), (1554, 810), (1554, 412)], "rx"),
                ([(1699, 381), (1720, 381), (1720, 250), (1355, 250), (1355, 181), (1339, 181)], "rx"),
            ],
            1: [
                ([(1034, 731), (1465, 731), (1465, 850), (10, 850), (10, 176), (35, 176)], "rx"),
                ([(324, 551), (344, 551), (344, 431), (365, 431)], "control"),
            ],
            2: [
                ([(375, 441), (320, 441), (320, 721), (375, 721)], "rx"),
            ],
        }
        for route, kind in routed_by_page.get(page_index, []):
            draw_routed_arrow([point(origin, x, y) for x, y in route], edge_colors[kind])
        for module in modules:
            fill, accent, _ = COLORS[module.class_id[0]]
            x1, y1 = point(origin, module.x, module.y)
            x2, y2 = point(origin, module.x + module.width, module.y + module.height)
            draw.rounded_rectangle((x1 - 1, y1 - 1, x2 + 1, y2 + 1), radius=6, outline="#173747", width=4)
            draw.rounded_rectangle((x1, y1, x2, y2), radius=6, fill=fill, outline=accent, width=2)
            header_y2 = y1 + max(12, int(SEED_HEADER_HEIGHT * scale))
            draw.rounded_rectangle(
                (x1 + 1, y1, x2 - 1, header_y2),
                radius=5,
                fill=HEADER_COLORS[module.class_id[0]],
                outline=None,
            )
            draw.line((x1 + 4, y1 + 1, x2 - 4, y1 + 1), fill=accent, width=1)
            tag_x, tag_y = point(origin, module.x + module.width - 66, module.y - 28)
            draw.rounded_rectangle((tag_x, tag_y, tag_x + 38, tag_y + 18), radius=8, fill="#030914", outline=accent, width=1)
            draw.text((tag_x + 6, tag_y + 1), module.class_id, fill=accent, font=tag_font)
            title_max = max(8, int((module.width * scale - 20) / 11))
            title = module.name if len(module.name) <= title_max else module.name[: title_max - 1] + "…"
            title_box = draw.textbbox((0, 0), title, font=module_title_font)
            title_w = title_box[2] - title_box[0]
            draw.text((x1 + max(5, (x2 - x1 - title_w) // 2), y1 + 1), title, fill="#ffffff", font=module_title_font)
            body_max = max(9, int((module.width * scale - 16) / 10))
            summary = textwrap.wrap(module.summary, width=body_max)[:2]
            body_text = "\n".join(summary)
            body_box = draw.multiline_textbbox((0, 0), body_text, font=small_font, spacing=1, align="center")
            body_w = body_box[2] - body_box[0]
            draw.multiline_text((x1 + max(4, (x2 - x1 - body_w) // 2), header_y2 + 3), body_text, fill="#b9cbd3", font=small_font, spacing=1, align="center")

    output.parent.mkdir(parents=True, exist_ok=True)
    image.save(output, format="PNG", optimize=True)


def main(argv: Optional[Sequence[str]] = None) -> int:
    parser = argparse.ArgumentParser()
    parser.add_argument("--mother-seed", type=Path, default=DEFAULT_MOTHER_SEED)
    parser.add_argument("--output", type=Path, default=WORKSPACE / "qt-runtime-code-flow.drawio")
    parser.add_argument("--html", type=Path, default=WORKSPACE / "qt-runtime-code-flow.html")
    parser.add_argument("--preview", type=Path, default=WORKSPACE / "qt-runtime-code-flow-preview.png")
    args = parser.parse_args(argv)
    try:
        modules, _ = build_drawio(args.mother_seed, args.output)
        write_html(args.output, args.html)
        write_preview_png(args.preview)
        print("generated {0}: {1} modules with one H/S/I annotation each".format(args.output, len(modules)))
        print("generated dark centered HTML: " + str(args.html))
        print("generated preview: " + str(args.preview))
        return 0
    except (OSError, ET.ParseError, RuntimeError, ValueError, ImportError) as exc:
        print("error: " + str(exc), file=sys.stderr)
        return 2


if __name__ == "__main__":
    sys.exit(main())

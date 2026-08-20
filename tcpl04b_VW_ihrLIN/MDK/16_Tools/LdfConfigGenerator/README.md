# LDF生成AUTOSAR LIN Slave通信配置

本工具把LDF中的普通帧和普通信号生成成当前工程使用的：

- `04_BSW_COM/Com_Cfg.h`：Signal ID、PDU ID、帧数量和配置类型；
- `04_BSW_COM/Com_Cfg.c`：帧表、信号位布局、方向、DLC、checksum、初值和波特率。

工具不会生成DID，不读取也不修改`Dcm_Cfg.c`、`Dcm_Dsp.c`或任何诊断业务表。LDF中如果存在`Diagnostic_frames`，只生成标准MRF `0x3C`和SRF `0x3D`的LinTp路由。

## 最简单的用法

把LDF文件拖到`生成LIN通信配置.cmd`上。结果生成到本目录的`Generated`，不会覆盖工程。

批处理会自动寻找Codex自带的Python 3或系统Python 3，不会误用本机旧的Python 2.7。

确认结果后，把LDF拖到`生成并应用到当前工程.cmd`上。工具会：

1. 完整解析并检查LDF；
2. 备份原来的`Com_Cfg.c/.h`到`04_BSW_COM/LdfConfigBackup/时间戳`；
3. 写入新配置；
4. 不改任何IHR协议内核、MCAL、应用代码和DID。

如果LDF只有一个Slave，不需要填写节点名。包含多个Slave时，在命令行指定：

```powershell
python ldf_to_autosar.py network.ldf --node RGB_Slave
```

只检查、不生成：

```powershell
python ldf_to_autosar.py network.ldf --node RGB_Slave --check
```

## 支持范围

- LIN 1.3/2.x无条件帧；
- LIN Slave RX/TX方向自动判定；
- 1～8字节帧；
- 1～16 bit标量信号；
- LDF信号初值和bit offset；
- 未使用bit默认填充`1`（`0xFF`），保持IHR量产配置习惯；可用`--fill-byte 0`修改；
- `configurable_frames`顺序；
- LIN 1.3 classic checksum、LIN 2.x enhanced checksum；
- MRF/SRF固定使用classic checksum。

不生成Master schedule，不生成事件触发帧、sporadic frame、DID或诊断服务。

## 新LDF后的必要检查

- RGB应用固定使用`Rte_LightCommandType`的`red/green/blue/brightness/dimming_time`，不使用LDF原始名称；
- 如果LDF信号名称变化，生成的`COM_SIG_RX_*`/`COM_SIG_TX_*`名称也会变化，只在`02_RTE/Rte_Com.c`更新Signal ID到固定应用字段的映射，不修改ASW；
- 如果业务语义变化，应用层必须同步修改；
- IHR协议内核、LinTp、DCM和MCAL不需要因普通信号增删而修改；
- 生成后必须重新编译，并在CANoe上检查PID、DLC、方向、checksum和信号值。

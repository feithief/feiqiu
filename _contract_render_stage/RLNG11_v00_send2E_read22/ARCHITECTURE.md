# AmbientDevice 模块化运行架构

## 1. 总体原则

工程保持 **一个进程、两个线程**。LIN 总线只有一个物理串口，增加多个 I/O
线程或进程只会争抢报文，因此所有总线操作必须串行化到唯一 Worker。

依赖只能从上向下：

`界面功能 -> LinRuntime 接口 -> AmbientLinScheduler -> LinBusWorker -> LinTransport 接口 -> AmbientLinComm`

界面、调度、协议、布局、串口和调试存储均有独立边界。上层只依赖接口，
禁止直接访问下层对象、文件描述符或可变状态。

## 2. 进程与线程

| 执行单元 | 负责内容 | 禁止内容 |
|---|---|---|
| GUI 主线程 | Qt 控件、页面导航、输入合并、请求编号、结果显示 | 串口访问、等待报文、诊断状态机 |
| LIN Worker 线程 | 周期调度、控制帧、诊断队列、P2/P2*、Flash 等待、统一回读、串口 I/O | 直接操作控件 |

`main.cpp` 是唯一组装入口：创建 `DebugStore`、传输工厂、具体
`AmbientLinScheduler`，再以 `LinRuntime*` 注入 `MainWindow`。其余界面文件
不得创建或包含具体调度器。

## 3. 模块职责

| 模块 | 单一职责 | 可替换点 |
|---|---|---|
| `LinRuntime` | 面向界面的稳定异步 API | 可替换成模拟器或测试桩 |
| `AmbientLinScheduler` | GUI 线程门面、命令合并、跨线程投递、生命周期 | 不含串口和诊断业务 |
| `LinBusWorker` | 单线程总线仲裁、周期帧与诊断任务状态机 | 不含界面 |
| `LinTransport` | Worker 使用的传输端口 | 可注入假串口 |
| `AmbientLinComm` | Linux LIN/UART 驱动和唯一文件描述符 | 不含调度和业务 |
| `LinLayout` / generated profile | 帧、信号、NAD、DID、时序和功能能力 | 新从机主要修改这里 |
| `DebugSink` | 只写的观测接口 | 业务不得读取并参与决策 |
| `DebugSnapshotSource` | F12 面板只读快照接口 | 面板不能修改运行状态 |
| `DebugStore` | 线程安全快照与 F12 显示数据 | 唯一允许的共享互斥区 |
| 各 QWidget 页面 | 输入、显示、页面内状态 | 只依赖 `LinRuntime` |

## 4. 同步规则

1. GUI 到 Worker 只使用 `Qt::QueuedConnection`，参数均按值传递。
2. Worker 内的周期帧、控制帧和诊断帧共用一个事件循环，不加业务锁。
3. 高频滑块输入在 GUI 线程合并 20 ms；按钮预设作为完整快照立即投递。
4. 诊断请求进入有界 FIFO；一个任务完成后才启动下一个任务。
5. 单个完整诊断事务中不插帧，事务之间允许让出一次控制帧。
6. `AmbientLinComm` 在 Worker 中创建、使用、销毁，并用线程断言保护。
7. 只有 `DebugStore` 使用 `QMutex`；锁内仅复制小型调试值，绝不做 I/O。
8. `stop()` 只在关闭应用时同步等待；正常功能不得阻塞 GUI 线程。

写配置严格执行：

`全部 DID 写入 -> Flash 等待 1000 ms -> 全部 DID 统一回读 -> 一次性返回 OK/Failed`

## 5. 扩展时改哪里

- 新增界面功能：新建独立 QWidget，只注入 `LinRuntime*`；不改 Worker 和串口。
- 新增一组快捷信号：只增加 profile 的 preset 数组；通用分页界面自动读取。
- 新增从机/帧/信号：只更新生成 profile 与布局校验；界面按节点能力生成。
- 新增诊断 DID：增加服务描述、编码映射和读写序列；不改页面导航和传输层。
- 更换 Linux 驱动：实现新的 `LinTransport` 和工厂；不改协议、调度和界面。
- 做无硬件测试：注入假 `LinTransport`；可验证 NAD、帧字节、超时与回读顺序。

新增模块不得反向包含上层头文件，也不得把新的全局变量、单例、共享队列或
第二个串口访问线程带入工程。

## 6. F12 可观测信息

固定变量包含 GUI/LIN 线程 ID、诊断队列深度、活动请求 ID、当前 PID/NAD、
最近 TX/RX、I/O 结果、超时/校验计数、诊断状态与独立诊断错误。另保留
`Reserved.01` 至 `Reserved.10` 十个独立开发槽位。

调试值只用于定位问题，不得成为任何功能判断的输入。

## 7. 关闭顺序

`停止接收新命令 -> 取消队列 -> 当前事务恢复 Initial NAD -> 停止定时器 ->
关闭串口 -> Worker 发出 stopped -> QThread 退出 -> 窗口销毁`

此顺序保证不会出现线程仍运行时销毁 `QThread`、跨线程关闭文件描述符或
写配置后遗留临时 NAD 的情况。

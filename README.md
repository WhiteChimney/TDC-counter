# TDC-counter
* 主要功能：对 TDC 的数据进行采集与处理
* 实际效果图：
![捕获1](https://user-images.githubusercontent.com/44578389/196106119-fd9da0a8-fd7d-4e06-afcc-d686a0568cc7.PNG)
![捕获2](https://user-images.githubusercontent.com/44578389/196106153-ee4e9df8-33cc-42fc-953e-46e0508ec6b6.PNG)
![捕获3](https://user-images.githubusercontent.com/44578389/196106165-5cf4e69a-e54c-46d1-8791-3bc89d1d9cbc.PNG)
* 项目 exe 发行地址：[Github](https://github.com/WhiteChimney/TDC-counter/releases)

## 硬件信息
* 仪器型号：Agilent Acqiris TC890 U1051A（详见 resources/Acqiris\ Manuals）
* 系统支持：Windows x86, x64

## Features
* 可视化配置，实时数据处理与显示
* 可同时显示单道计数、自定义符合计数，以及信号到达时间的统计直方图
* 参数均可调，且配置自动保存
* 单道计数与符合计数的数据可保存
* 可通过统计面板进行长时间数据累计分析（实时给出平均值与标准差）

## 项目信息
* 版本：Qt5.12.12 ，Qwt-6.2.0
* 编译项目时，AqDrv4(_x64).dll 需要放置在运行目录下，或者确保其在系统环境变量目录中（安装过 Acqiris 软件就已经在了）

### UI Classes
* Widget: 主窗口
* TestWidget: 测试窗口
* CoincidenceWidget: 符合计数窗口
* HistogramWidget: 直方图窗口
* StatisticsWidget: 统计窗口

### 功能 Classes
* AcquisitionThread: 采集线程
* StatisticsChannel: 进行统计的通道
* QSimpleLed: 自定义状态指示灯（来自 [CSDN 网友](https://blog.csdn.net/weixin_37818081/article/details/119803320)）

### 子函数
* configTDC: 配置 TDC 参数
* computeSingleChannelCount: 计算单道计数
* computeCoincidenceCount: 计算符合计数
* computeCoincidenceCountMulti: 计算多（大于 2）通道的符合计数
* computeHistogramCounts: 计算直方图的数据
* mergeDataFiles: 合并并生成最终数据文件
> *Copyright Agilent Technologies, Inc. 2006-2008*

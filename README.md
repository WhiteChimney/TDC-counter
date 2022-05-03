# TDC-counter
* 主要功能：对 TDC 的数据进行采集与处理
* 实际效果图：
![捕获](https://user-images.githubusercontent.com/44578389/166425249-36da424d-c8e9-4022-8d07-3bf5dcd86e99.PNG)
* 项目 exe 发行地址：[Github](https://github.com/WhiteChimney/TDC-counter/releases)
* 项目备用地址：[Gitee](https://gitee.com/white8chimney/TDC-counter)

# 硬件信息
* 仪器型号：Agilent Acqiris TC890 U1051A
* 系统支持：Windows x86, x64

# Features
* 可视化配置，实时数据处理与显示
* 可同时显示单道计数、自定义符合计数，以及信号到达时间的统计直方图
* 参数均可调，且配置自动保存
* 单道计数与符合计数的数据可保存

# 项目结构
## Classes
* Widget: 主窗口
* AcquisitionThread: 采集线程
* TestWidget: 测试窗口
* CoincidenceWidget: 符合窗口
* HistogramWidget: 直方图窗口

## 子函数
* testTC890: 测试用子函数
* configTDC: 配置 TDC 参数
* algorithmSingleChannelCount: 计算单道计数
* algorithmCoincidenceCount: 计算符合
* algorithmHistogramCounts: 计算直方图的数据
* mergeDataFiles: 合并并生成最终数据文件
*Copyright Agilent Technologies, Inc. 2006-2008*

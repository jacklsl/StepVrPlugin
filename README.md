# StepVrPlugin
StepVr大空间动作捕捉设备与UE4对接插件

UE4插件说明

配置Windos
一	将解压后的 StepVrPlugin文件夹 放到目标工程的Plugins目录下（如果没有Plugins文件夹，需要在Content同级目录下创建）
二	将标定的矩阵文件 TransMat.txt 放到对应版本引擎的Engine\Binaries\Win64下（所有工程都可以使用），项目打包后需将矩阵复制到WindowsNoEditor\xxx\Binaries\Win64下


使用
一	按键事件					：在项目设置->输入中 查找StepVr开头相关

二	手柄和枪					：使用MotionController时设置  左手柄：Left   右手柄：Right   枪：Pad

三	获取所有节点TransForm信息	：在StepVrComponent组件中获取
需先创建一个StepVrComponent对象
获取CurrentNodeState成员变量
查看FStepVRNode结构体，获取相应的节点数据

注意事项
一	使用前需先插上设备，待设备指示灯变成绿色，然后再从  VS启动编辑器（拔插AP需要重新启动编辑器）  或者  开始独立游戏
，否则获取不到数据
二	不插Ap接收器，运行项目可能会引起崩溃
三	查看下设备id号是否是对应，可能会疏忽导致获取不到数据
四	没有数据查看矩阵文件是否放在正确位置


参数
左手柄ID：1
右手柄ID：2
枪ID：  4
头ID：	6

其他全身标准件ID待定，如果设备ID有变化，需要在StepVrComponent.cpp 中的TickComponent中  设置对应的Id节点号

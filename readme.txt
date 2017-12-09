基于brep边界表示的扫成操作

文件说明：
主函数main（）在demo.cpp中
顶点数据的修改在init函数中construct_out（控制扫成面的外轮廓构建）与construct_inner（扫成面的内环的构建）两个函数控制
sweep（Solid* solid，int[3] direction，int distan）

操作说明：
左键控制旋转，右键控制平移，中间滚轮按下后上下滚动控制放缩

显示说明：
表面由红色显示，蓝色表示边框
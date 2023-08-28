AIP_T:
	用来实现nv12格式到bgra格式的转换。
	参考sample aip_covent

Features
	支持nv12格式到bgra格式的转换；
	转换参数精度为20bit；
	转换图像范围：2x2 ~ 4096 x 2048;
	支持chain配置寄存器，chain放在ddr中；
	支持task多行输出启动功能，仅在非chain模式；
	chain模式不能启动task功能。

AIP_F
	实现图像的缩放功能。
	参考samplea aip_resize
Features
	数据插值精度为11bit，缩放参数精度16bit；
	支持的resize图像格式如下：
	图像格式	SRC(MAX)（WxH）	SRC(MIN)（WxH）	DST(MAX)（WxH）	DST(MIN)（WxH）	每个像素点描述
	Y			4096x2048		1x1				4096x2048		1x1				nv12格式的Y分量，1byte
	C																			nv12格式的C分量，2byte
	bgra																		bgra格式，4byte
	feature2	1920x1080		1x1				1920x1080		1x1				2bit位宽，32channel，8byte
	feature4																	4bit位宽,32channel，16byte
	feature8																	8bit位宽,32channel，32byte
	feature8_h																	8bit位宽,16channel，16byte
	nv12		4096x2048		2x2				4096x2048		2x2				nv12格式

	注：
	bgra代表色彩空间，输入输出格式一致，输入可以是b,g,r,a的任意排列组合。
	nv12格式输入输出必须都是偶行偶列。
	支持chain模式配置寄存器，chain放在ddr中；
	支持ddr/oram 总线选择，原图像、目标图像可以放在ddr或oram中。

AIP_P
	图像数据进行nv12/bgr格式到nv12/bgr格式的缩放、仿射、透视变换。
	参考sample aip_affine  aip_perspective
Features
	支持只进行resize、affine仿射、透视功能；
	数据插值精度5bit；透视参数位宽48bit，转换参数精度20bit；
	支持进行affine仿射、透视格式转换功能+格式转换（先做转换，再做插值）
	转换模式		SRC（MAX）	SRC(MIN)		DST(MAX)	DST(MIN)
	NV12---> BGRA	2048x1080	2*2	2048x1080	1*1
	NV12---> NV12	2048x1080	2*2	2048x1080	2*2
	BGRA---> BGRA	2048x1080	1*1	2048x1080	1*1
	BGRA---> NV12	不支持
	Note: 先做格式转换，再做插值

	支持chain模式配置寄存器，寄存器配置放在ddr中；
	源图像、目标图像可以放在ddr或oram 中。

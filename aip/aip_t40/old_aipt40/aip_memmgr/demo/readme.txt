文件说明：
1、DRIVERS：驱动源码
2、yolov3_xxxx_public：测试用例在document文件目录下
依赖库： libvenus.so libdrivers.so
依赖文件： t40_xxx.bin
可直接修改驱动源码，替换 libdrivers.so

nmem 测试用例在test文件目录下
1、msg_send.c 为nmem使用情况测试代码
2、操作：在开发板执行 msg_send  执行./yolov3_run_uclibc_test_bin_public 可在串口打印nmem 使用情况
备注：./msg_send ./yolov3_run_uclibc_test_bin_public 无严格的先后顺序


仅用于内部测试

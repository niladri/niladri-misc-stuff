entry: all

gl_test01: gl_test01.c
	arm-linux-gcc -Wall -o gl_test01 gl_test01.c -I$(MMROOT)/linux/include \
	-L$(MMROOT)/linux/lib/cortexA9-linux-gnu-mop500_ed \
	-L/prj/multimedia_shared/common/STLinux/cortex/devkit/arm/target/lib \
	-lGLESv1_CM -lEGL -lMali -lc

gl2_test01: gl2_test01.c
	arm-linux-gcc -Wall -o gl2_test01 gl2_test01.c -I$(MMROOT)/linux/include \
	-L$(MMROOT)/linux/lib/cortexA9-linux-gnu-mop500_ed \
	-L/prj/multimedia_shared/common/STLinux/cortex/devkit/arm/target/lib \
	-lGLESv2 -lEGL -lMali

all: gl_test01 gl2_test01

clean:
	rm gl_test01 gl2_test01

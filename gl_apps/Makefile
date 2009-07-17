INCDIRS=-I$(MMROOT)/linux/include
LIBDIRS=-L$(MMROOT)/linux/lib/cortexA9-linux-gnu-mop500_ed -L/prj/multimedia_shared/common/STLinux/cortex/devkit/arm/target/lib
LIBS=-lGLESv1_CM -lGLESv2 -lEGL -lMali -lc

entry: all

gl_test01: gl_test01.c
	arm-linux-gcc -Wall -o gl_test01 gl_test01.c $(INCDIRS) $(LIBDIRS) $(LIBS)

gl2_test01: gl2_test01.c
	arm-linux-gcc -Wall -o gl2_test01 gl2_test01.c $(INCDIRS) $(LIBDIRS) $(LIBS)

all: gl_test01 gl2_test01

install:
	cp gl_test01 gl2_test01 $(MMROOT)/linux/bin/cortexA9-linux-gnu-mop500_ed/

clean:
	rm gl_test01 gl2_test01

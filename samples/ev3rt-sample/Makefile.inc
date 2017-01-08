APPL_COBJS += balancer_param.o
APPL_COBJS += balancer.o
APPL_CXXOBJS += ev3way.o

APPL_LIBS += -lsharaku-sys.ev3rt.arm -lsharaku-dev.ev3rt.arm -lsharaku-mod.ev3rt.arm
CFLAGS += -Wunused-function
CFLAGS += -I/usr/local/include/libsharaku/arm-itron/
CFLAGS += -L/usr/local/lib/libsharaku/arm-itron/
CXXFLAGS += ${CFLAGS}
CXXFLAGS += ${CFLAGS}
SRCLANG := c++


# Makefile

.PHONY: all clean

CC       =g++
CFLAGS   =-Wall -ansi -pedantic
TARGET   =libsharaku-unittest
SRC      =gtest_list.cpp
SRC      +=gtest_pid.cpp
SRC      +=gtest_plist.cpp
SRC      +=gtest_slab.cpp
HEADERS  =
INCLUDES =-I/usr/local/include/ -I../../../tools/integration/deproy/libsharaku/usr/local/include/libsharaku/x86-linux
LDFLAGS  =-L/usr/local/lib/ -L../../../tools/integration/deproy/libsharaku/usr/local/lib/libsharaku/x86-linux
LIBS     =-pthread -lgtest -lgtest_main -lsharaku-dev.linux.x86 -lsharaku-mod.linux.x86  -lsharaku-sys.linux.x86

all: $(TARGET) test

$(TARGET): $(SRC) $(HEADERS) Makefile
	$(CC) -o $@ $(CFLAGS) $(SRC) $(INCLUDES) $(LDFLAGS) $(LIBS)

clean:
	rm -f $(TARGET)

test: $(TARGET)
#	./$(TARGET) --gtest_filter=list.* --gtest_output=xml:test_result.xml
	./$(TARGET) --gtest_output=xml:test_result.xml


ifeq ($(CROSS),)
#CROSS	= arm-hisiv100nptl-linux-
endif
CXX := $(CROSS)g++
CC  := 	$(CROSS)gcc
SRC_CPP_FILES := $(wildcard *.cpp)
SRC_CPP_OBJS := $(SRC_CPP_FILES:.cpp=.o)
#SRC_C_FILES := $(wildcard *.c ./ghttp/*.c ./cJSON/*c)
SRC_C_FILES := $(wildcard *.c  ./cJSON/*c)
SRC_C_OBJS := $(SRC_C_FILES:.c=.o)
SRC_OBJS := $(SRC_CPP_OBJS) $(SRC_C_OBJS)
LDFLAGS += $(EXP_LDFLAGS)
CPPFLAGS += -g  -Wall -D__UNIX
CFLAGS += -g  -Wall -D__UNIX

#${info c======= $(SRC_C_FILES)}

EXP_CPPFLAGS :=  -lpthread #如果此处不定义，就接受外部的传参
CPPFLAGS += $(EXP_CPPFLAGS)
CFLAGS += $(EXP_CPPFLAGS)


SRC_SERVER_OBJS := main.o  send_read_socket.o  message_parse.o
SRC_CLIENT_OBJS := client.o

CURRENT_TARGET := server client
TARGET_SERVER := server
TARGET_CLIENT := client

#QUIET := @
ifeq ($(QUIET),)
LINK	= $(LINK.c)
COMPILE_CPP	= $(COMPILE.cpp)
COMPILE_C = $(COMPILE.c)
INSTALL = install -m 777 -D
#INSTALL = install -m 777 -s --strip-program=arm-hisiv100nptl-linux-strip -D 
else
LINK	= $(QUIET)echo " G++	$@  "; $(LINK.cc)
COMPILE_CPP	= $(QUIET)echo " G++	$@"; $(COMPILE.cpp)
COMPILE_C	= $(QUIET)echo " GCC	$@"; $(COMPILE.c)
INSTALL =  $(QUIET)echo " install $(CURRENT_TARGET) to $(installPath)";install -m 777 -D
#INSTALL = $(QUIET)echo " install $(CURRENT_TARGET) to $(installPath)";install -m 777 -s --strip-program=arm-hisiv100nptl-linux-strip -D 
endif

#ALL := $(CURRENT_TARGET)

installPath := ./bin #change this to real directory

INC := -I ./ -I ./ghttp -I ./cJSON
CPPFLAGS += $(INC)
CFLAGS +=  $(INC)

.PHONY:$(TARGET_SERVER) $(TARGET_CLIENT) $(ALL) lib
all:lib $(TARGET_SERVER) $(TARGET_CLIENT) $(ALL)


$(TARGET_SERVER):$(SRC_SERVER_OBJS)
	$(LINK) -o $@ $^ -lpthread -lm
	
$(TARGET_CLIENT):$(SRC_CLIENT_OBJS)
	$(LINK) -o $@ $^ -lpthread -lm
	
	
#$(CURRENT_TARGET):$(SRC_OBJS)
#	$(LINK) -o $@ $^ -lpthread -lm
#	cp -rf $@ /nfsroot/ulucu/IPC
lib:
#	make --directory=upYun	
install:
	$(INSTALL) $(CURRENT_TARGET) $(strip $(installPath))/$(CURRENT_TARGET)	
.cpp.o:
	$(COMPILE_CPP) -o $@ $<
.c.o:
	$(COMPILE_C) -o $@ $<
dist:
	tar cvzf dev_skeV$(ver).tgz *
clean:
#	make --directory=upYun clean
	@echo "clean *.o $(CURRENT_TARGET) *.d";$(RM) $(SRC_OBJS) $(CURRENT_TARGET) $(TARGET_SERVER) $(TARGET_CLIENT) *.d* $(installPath)/$(CURRENT_TARGET) ./cJSON/*.d* ./ghttp/*.d*

%.d: %.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

%.d: %.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
			  
sinclude $(SRC_CPP_FILES:.cpp=.d)
sinclude $(SRC_C_FILES:.c=.d)	

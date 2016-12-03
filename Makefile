include devenv.mk  # from https://github.com/wordworld/common/make

DEFINES	+= -D_DEBUG 

CFLAGS	+= -Wall -g -pthread

LDFLAGS	+= 

INCLUDES += -I$(DIR_DEVLIBS)/include

vpath %.a $(DIR_DEVLIBS)/lib

SRC	= zdemo.cpp
MAIN	= fzip.cpp
OBJS	= $(MAIN:.cpp=.o) $(SRC:.cpp=.o)
EXE	= $(MAIN:.cpp=.exe)

all:$(EXE)

$(EXE):$(OBJS) -lz
	$(LD) $(LDFLAGS) $^ -o $@ 

.c.o:
	$(CC) -c $(DEFINES) $(INCLUDES) $(CFLAGS) $< -o $@

.cpp.o:
	$(CPP) -c $(DEFINES) $(INCLUDES) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS) $(EXE) *.fz

run:
	./$(EXE)

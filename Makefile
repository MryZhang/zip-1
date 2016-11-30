include devenv.mk  # from https://github.com/wordworld/common/make

DEFINES	+= -D_DEBUG 

CFLAGS	+= -Wall -g -pthread

LDFLAGS	+= 

INCLUDES += -I$(DIR_DEVLIBS)/include

vpath %.a $(DIR_DEVLIBS)/lib

OBJS	= fzip.o
EXE	= fzip


all:$(EXE)

$(EXE):$(OBJS) -lz
	$(LD) $(LDFLAGS) $^ -o $@ 

.C.o:
	$(CC) -c $(DEFINES) $(INCLUDES) $(CFLAGS) $< -o $@

.cpp.o : fzip.cpp
	$(CPP) -c $(DEFINES) $(INCLUDES) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS)
	rm -f $(EXE)

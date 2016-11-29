include devenv.mk  # from https://github.com/wordworld/common/make

DEFINES += -D_DEBUG 

CFLAGS += -Wall 

LDFLAGS +=

INCLUDES += -I.


OBJS = *.obj
EXE = fzip

all:$(EXE)

$(EXE):$(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

.C.O:
	$(CC) -c $(DEFINES) $(INCLUDES) $(CFLAGS) $< -o $@

.cpp.o:
	$(CC) -c $(DEFINES) $(INCLUDES) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJS)
	rm -f $(EXE)

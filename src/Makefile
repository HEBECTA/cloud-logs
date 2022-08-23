BIN:=cloud_logs
SRC:=$(wildcard *.c)
OBJS:=$(SRC:.c=.o)
LDFLAGS:=-lubus -lblobmsg_json -lubox -liotp-as-device -largp

.PHONY: all clean

all: $(BIN)

$(BIN):$(OBJS) 
	$(CC) -o $@ $^ $(LDFLAGS)

%.o : %.c
	$(CC) -o $@ -c $^

clean:
	$(RM) $(BIN) $(OBJS)
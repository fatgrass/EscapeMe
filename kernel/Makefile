AS		:= nasm

TARGET	:= kernel.elf kernel.bin
#TARGET	:= kernel.bin

SRCS 	:= startup.s kernel.c
CSRCS	:= $(filter %.c,$(SRCS))
SSRCS	:= $(filter %.s,$(SRCS))

OBJS	:= $(SSRCS:.s=.o) $(CSRCS:.c=.o)
DEPS	:= $(CSRCS:.c=.d)
SUB_OBJS := memory/memory.a service/service.a elf/elf.a utils/utils.a

CFLAGS	:= -Wall -I.. -masm=intel -fno-stack-protector -fPIE
LDFLAGS	:= -nostdlib -Ttext 0x0

export CFLAGS
export CTF_FLAG1

.PHONY: all
all: $(TARGET)

-include $(DEPS)

%.elf: $(OBJS) $(SUB_OBJS)
	$(LD) $(LDFLAGS) $^ -o $@

%.bin: $(OBJS) $(SUB_OBJS)
	$(LD) $(LDFLAGS) --oformat=binary $^ -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c -MMD -MP $<

%.o: %.s
	$(AS) -f elf64 $^

$(SUB_OBJS): FORCE
	$(MAKE) -C $(dir $@) $(notdir $@)

.PHONY: clean
clean:
	dirname $(SUB_OBJS) | xargs -l $(MAKE) clean -C
	$(RM) $(DEPS) $(OBJS) $(TARGET)

FORCE:

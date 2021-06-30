CFLAGS += -mcpu=cortex-m0plus -mthumb -O0 -g3 -ffunction-sections -Wall -Wextra \
-ffreestanding --specs=nano.specs --specs=nosys.specs

DEFINE += "-D__ATSAMD21E18A__"

INCLUDE += \
-I"samd21a/include" \
-I"CMSIS/Core/Include"

OBJS += \
main.o \
samd21a/gcc/gcc/startup_samd21.o \
samd21a/gcc/system_samd21.o

OBJS_ARGS += \
"main.o" \
"samd21a/gcc/gcc/startup_samd21.o" \
"samd21a/gcc/system_samd21.o"

vpath %.c .
vpath %.s .
vpath %.S .

all: example.elf

clean:
	find . -type f | xargs touch
	rm $(OBJS)
	rm example.elf

%.o: %.c
	"arm-none-eabi-gcc" $(CFLAGS) $(INCLUDE) $(DEFINE) -o $@ -c $<

example.elf: $(OBJS)
	mkdir -p target
	"arm-none-eabi-gcc" -o example.elf $(OBJS_ARGS) $(CFLAGS) -Wl,--start-group -lm -Wl,--end-group \
	-Wl,--gc-sections -mcpu=cortex-m0plus -T"samd21a/gcc/gcc/samd21e18a_flash.ld" \
	-L"samd21a/gcc/gcc"

size: example.elf
	arm-none-eabi-size example.elf --format=sysv
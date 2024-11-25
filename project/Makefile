# makfile configuration

COMMON_OBJECTS  =

CPU             = msp430g2553

CFLAGS          = -mmcu=${CPU} -I../h

LDFLAGS		= -L/opt/ti/msp430_gcc/include



#switch the compiler (for the internal make rules)

CC              = msp430-elf-gcc

AS              = msp430-elf-as



all: button.elf



#additional rules for files

button.elf: ${COMMON_OBJECTS} main.o pipesCollision.o ../lib/libTimer.a ../lcdLib/lcdLib.a

	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^



load: button.elf

	msp430loader.sh button.elf



clean:

	rm -f *.o *.elf
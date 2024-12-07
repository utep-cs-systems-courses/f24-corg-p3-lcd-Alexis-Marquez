# makfile configuration

CPU             	= msp430g2553

CFLAGS          	= -mmcu=${CPU} -Os -I../h

LDFLAGS		= -L../lib -L/opt/ti/msp430_gcc/include/



#switch the compiler (for the internal make rules)

CC              = msp430-elf-gcc

AS              = msp430-elf-gcc -mmcu=${CPU} -c



all:game.elf



#additional rules for files

game.elf: ${COMMON_OBJECTS} main.o buzzer.o p1_interrupt_handler.o wdt_handler.s pipesCollision.o jump.o

	${CC} ${CFLAGS} ${LDFLAGS} -o $@ $^ -lTimer -lLcd



load: game.elf

	msp430loader.sh $^



clean:

	rm -f *.o *.elf

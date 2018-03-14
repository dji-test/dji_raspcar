objs	:= main.o pwm.o motor.o ultrasonic.o delay.o
CFLAGS	:= -Wall -g

main	:$(objs)
	gcc $(CFLAGS) -o $@ $^

%.o:%.c
	gcc $(CFLAGS) -c -o $@ $<
clean:
	rm *.o

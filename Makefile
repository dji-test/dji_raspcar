cobjs	:= ultrasonic.o motor.o pwm.o delay.o
cppobjs := main.o camera.o
CFLAGS	:= -Wall -g
CPPFLAGS:= -lopencv_core -lopencv_imgproc -lopencv_highgui

main : $(cobjs) $(cppobjs)
	g++ $(CFLAGS) $(CPPFLAGS) -o $@ $^

%.o:%.cpp
	 g++ $(CPPFLAGS) -c -o $@ $<

%.o:%.c
	gcc $(CFLAGS) -c -o $@ $<
clean:
	rm *.o

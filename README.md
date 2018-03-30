# dji_raspcar
To use uart0:
1. first type this command "sudo systemctl disable hciuart";
2. add "init_uart_clock=48000000" to /boot/config.txt and reboot;
3. use command "vcgencmd measure_clock uart" to verify that the uart clock is 48MHz;
4. use baud rate 9600bps for pc's serial software; 
5. the format to be sent is begin with '^' and end with '\n', for example "^3.0,0,10\n".

Note: the application's running needs root's privilege.
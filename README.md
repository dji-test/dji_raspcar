# dji_raspcar
To use uart0:
1. first type this command "sudo systemctl disable hciuart";
2. add "init_uart_clock=48000000" to /boot/config.txt and reboot;
3. use command "vcgencmd measure_clock uart" to verify that the uart clock is 48MHz.

Note: the application's running needs root's privilege.
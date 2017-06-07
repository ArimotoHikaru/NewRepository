#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/time.h>
#include <wiringPi.h>
#include <stdint.h>
#include <stdbool.h>

unsigned char rx_buf[255];
unsigned char *p_rx_buf;

int main(int argc, char* argv[])
{
    int fd = -1;
    struct termios options;
    bool loop = true;

    int len;

    unsigned char tx_buffer[20];

    //fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode
    fd = open("/dev/ttyS0", O_RDWR | O_NOCTTY);

    if (fd == -1){
	    //ERROR - CAN'T OPEN SERIAL PORT
	    printf("Error - Unable to open UART.  Ensure it is not in use by another application\n");
        return -1;
    }
    /*****UARTの設定*****/
    tcgetattr(fd, &options);

    options.c_cflag = B9600| CS8 | CLOCAL | CSTOPB | CREAD;// stop bit 2
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &options);
    /********************/

    while(loop){

		p_rx_buf = &rx_buf[0];

		tx_buffer[0] = 0x00;
		tx_buffer[1] = 0x54;
		len = write(fd,tx_buffer,2);

		usleep(70000);//70ms

	  	len = read(fd, p_rx_buf, 2);
	   printf("[0]:0x%02X  [1]:0x%02X  distance:%4dcm\n"
		,rx_buf[0],rx_buf[1],(rx_buf[0]<<8) | rx_buf[1]);

    	}
    close(fd);
    return 0;
}


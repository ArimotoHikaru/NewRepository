//Copyright© 2017 Hikaru Arimoto
//Raspberry pi でSRF02 使う
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/time.h>
#include <wiringPi.h>
#include <stdint.h>
#include <stdbool.h>

#define DEV /dev/ttyS0

unsigned char rx_buf[255];

int main(int argc, char* argv[])
{
    int fd = -1,len;
    struct termios options;
    bool loop = true;
    unsigned char tx_buffer[20];

    fd = open(DEV, O_RDWR | O_NOCTTY);

    if (fd == -1){
        printf("Error:Unable to open UART\n");
        return -1;
    }
    /*****UARTの設定*****/
    tcgetattr(fd, &options);
    //9600bps 8バイト モデム制御なし ストップビット2 受信を有効
    options.c_cflag = B9600| CS8 | CLOCAL | CSTOPB | CREAD;// 
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;

    tcflush(fd, TCIFLUSH);
    tcsetattr(fd, TCSANOW, &options);
    /********************/

    while(loop){

        tx_buffer[0] = 0x00;//デバイスのアドレス
        tx_buffer[1] = 0x54;//Real Ranging Mode 距離をセンチメートルで返す
        len = write(fd,tx_buffer,2);//送信

        usleep(70000);//70ms

        //2バイト読み込み
        len = read(fd, rx_buf, 2);//受信
        printf("[0]:0x%02X  [1]:0x%02X  distance:%4dcm\n"
        ,rx_buf[0],rx_buf[1],(rx_buf[0]<<8) | rx_buf[1]);

    }
    close(fd);
    return 0;
}


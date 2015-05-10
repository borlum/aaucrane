#include <stdio.h>
#include <termios.h>
#include <fcntl.h>
#include <termios.h>

void main() {
  struct termios termiosv;
 
  int FD_serial = open("/dev/ttyACM2", O_RDONLY | O_NOCTTY);
  if (FD_serial == -1) {
    perror("open_port: Unable to open port");
    exit(1);
  }
  
  tcgetattr( FD_serial, &termiosv );

  termiosv.c_cc[VMIN]  = 4;
  
  tcsetattr( FD_serial, TCSANOW, &termiosv );

  /* get current serial port settings */
  tcgetattr(FD_serial, &termiosv);
  /* set 9600 baud both ways */
  cfsetispeed(&termiosv, B9600);
  cfsetospeed(&termiosv, B9600);
  /* 8 bits, no parity, no stop bits */
  termiosv.c_cflag &= ~PARENB;
  termiosv.c_cflag &= ~CSTOPB;
  termiosv.c_cflag &= ~CSIZE;
  termiosv.c_cflag |= CS8;
  /* Canonical mode */
  termiosv.c_lflag |= ICANON;
  /* commit the serial port settings */
  tcsetattr(FD_serial, TCSANOW, &termiosv);

  usleep(1000000 * 2);  
  tcflush(FD_serial, TCIOFLUSH);


  
  char buffer[5] = {0};
  while(1){
    read(FD_serial, buffer, 4);
    if(buffer[3] == ';'){
      printf("%d\n", atoi(buffer));
    }
    else{
      printf("ERROR: %s\n", buffer);
      tcflush(FD_serial, TCIOFLUSH);
    }
  }
  
}

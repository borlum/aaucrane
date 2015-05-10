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

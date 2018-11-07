#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<errno.h>
#include<fcntl.h>
//#include<string.h>
#include<unistd.h>

#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

static uint8_t btn_in;     ///< The receive buffer from the LKM
 
int gamepad_test(){
   /* Init gamepad */
   /*FILE *fp;
   char *command;
   fp = popen(command,"w"); 
   fprintf(fp,"mknod /dev/gamepad c 253 252; modprobe driver-gamepad");
   fclose(fp);*/
   system("mknod /dev/gamepad c 253 252");
   system("modprobe driver-gamepad");

   int ret, fd, run;
   printf("Starting device test code example...\n");
   fd = open("/dev/gamepad", O_RDWR);             // Open the device with read/write access
   if (fd < 0){
      perror("Failed to open the device...");
      return errno;
   }
   printf("Starting program...\n");
   do {
      ret = read(fd, &btn_in, 1);
      if (ret < 0){
         perror("Failed to read the message from the device.");
         return errno;
      }
      printf("Buttons: 0x%02x\n", ~btn_in);
      printf("aka "BYTE_TO_BINARY_PATTERN"\n", BYTE_TO_BINARY(btn_in));
      puts("Run again? [0=no, 1=yes]");
      fflush(stdin);
      scanf("%u", &run); puts("");
   } while (run);

   return 0;
}
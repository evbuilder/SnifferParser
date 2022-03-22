/* SnifferParser:
 * Deciphers ToyotaSniffer output
 */
#include<stdio.h>
#include<stdint.h>

int main (int argv, char *argc[])
{
  FILE* file_p;
  uint8_t message_length, i, count=40;
  uint8_t byte;
  uint8_t message[128];
  //open file
  file_p = fopen("log3.txt","r");
  if(!file_p)
  {
    printf("Couldn't open log1.txt\n");
    return -1;
  }
  while(fread(&byte,1,1,file_p)) // print text header
  {
    printf("%c",byte);
    if(byte=='\n')
      break;
    if(!count--)
    {  printf("no end of first line\n");return 0;}
  }
  count=0;
  printf("\n");
  fseek(file_p,-1,SEEK_CUR); //backup a char
  while(fread(&byte,1,1,file_p))
  {
//    if(!count--)
//      break;
    if(byte!='\n')
    {
        printf("(%u)",byte);
        continue;
    }
    if(!fread(&byte,1,1,file_p))  // get message prefix
      break;
    if(byte=='\n')
    {
      printf("\\n");
      fread(&byte,1,1,file_p);
    }
    printf("%c,",byte);
    if(!fread(&byte,1,1,file_p))  // get message length
      break;
    printf("Len:%u,",byte); message_length = byte;
    if(fread(message,1,message_length,file_p)!=message_length)
      break;
    for(i=0;i<message_length;i++) // display the message content
    {
      printf("%u,",message[i]);
    }
    if(!fread(&byte,1,1,file_p)) // check for timeout flag
      break;
    if(byte=='+')
      printf("+,");
    else
      fseek(file_p,-1,SEEK_CUR); //backup a char
    printf("\n");
  }
  printf("End of file \n");
  return 0;
}

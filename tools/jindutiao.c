#include <stdio.h>                                        
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#define N 100
 
#define NONE "\e[0m"
#define RED  "\e[0;31m"
#define BLUE "\e[0;34m"
  
 
int main()
{       
    char bar[N];
    int per = 0;
    //void *memset(void *s, int ch, size_t n);
    //将s中当前位置后面的n个字节 （typedef unsigned int size_t ）用 ch 替换并返回 s 。
    //memset：作用是在一段内存块中填充某个给定的值，它是对较大的结构体或数组进行清零操作的一种最快方法。
    //memset()函数原型是extern void *memset(void *buffer, int c, int count) buffer：为指针或是数组,c：是赋给buffer的值,count：是buffer的长度.
    memset(bar,'\0',sizeof(bar));
    while(per<=N)
    {
      bar[per]= '-';
      printf(RED"[%-100s][%d%%]\r"NONE,bar,per+1);
      fflush(stdout);
      per++;
      usleep(100000);
    }
    printf("\n");
    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
void func()
{
  printf("nihao!!\n");
  printf("nihao!!\n");
  printf("nihao!!\n");
}

int main(int argc, char *argv[])
{
  int i;
  printf("--------\n");
  for (i = 0; i < argc; i++)
  {
    printf("argv[%d]=[%s]\n", i, argv[i]);
  }
  func();
  printf("hello world!!\n");
  char *ptr = NULL;
  memcpy(ptr, "nihao!!", 7);
  printf("hello %s\n", ptr);
  return 0;
  }





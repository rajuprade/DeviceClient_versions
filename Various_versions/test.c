#include<stdio.h>

int main() 
{
   creat("/tmp/foo1", 0777);
   chmod("/tmp/foo1",0777);
   return 0;
}


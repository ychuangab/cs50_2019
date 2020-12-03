#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
   for (int i = 0; i < 10; i++)
   {
      char filename[] = "test000.jpg";
      sprintf(filename, "test%03i.jpg", i);
      FILE *img = fopen(filename, "w");
      fclose(img);
   }


}


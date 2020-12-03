#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef uint8_t BYTE;

/* pseudo code
1. open card file
2. repeat until the end of card:
     read 512 bytes into the buffer
     start of a new JPEG file?
          yes --> ...
          no  --> ...

     already found a JPEG?
          yes --> ...
          no  --> ...

    close any remaining file
*/
int buffer_size = 512;

int main(int argc, char *argv[])
{
     // check argc equals to 2
     if(argc != 2)
     {
         fprintf(stderr, "Usage: ./recover image\n");
         return 1;
     }

     // open the card file into inptr
     char *file = argv[1];
     //printf("%p", file);
     FILE *inptr = fopen(file, "r");
     if (inptr == NULL)
     {
          fprintf(stderr, "Could not open %s.\n", file);
          return 2;
     }

     // defiend related vars
     //int i_test = 0;
     BYTE *buffer = malloc(buffer_size * sizeof(BYTE));
     bool found_jpeg_start = false;
     bool recovering_jpeg = false;
     int jpeg_file_cnt = 0;
     int read_return = 1;
     char filename[] = "000.jpg";
     FILE *outptr = fopen(filename, "w");
     fclose(outptr);

     // repeat until the end of card
     while (read_return != 0)
     {
          read_return = fread(buffer, 1, buffer_size, inptr); // read in 1 block which is 512 bytes long

          // check if the buffer is a JPEG file or not
          // note that buffer[3] use "bitwise AND" since it should equal to 0xe*
          if (buffer[0] == 0xff &&
              buffer[1] == 0xd8 &&
              buffer[2] == 0xff &&
              (buffer[3] & 0xf0) == 0xe0)
          {
               if (recovering_jpeg)
               {
                    // close privious file
                    fclose(outptr);
               }

               // new jpeg is found, prepare to write
               recovering_jpeg = true;
               sprintf(filename, "%03i.jpg", jpeg_file_cnt);
               outptr = fopen(filename, "w");
               jpeg_file_cnt++;

               // case when it could not open and write the jpg file
               if (outptr == NULL)
               {
                    fclose(inptr);
                    fprintf(stderr, "Could not open and write %s.\n", filename);
                    return 3;
               }

               // open new jpeg ok, write buffer in
               if (read_return != buffer_size)
               {
                    fwrite(buffer, read_return, 1, outptr);
                    recovering_jpeg = false;
               }

               else
               {
                    fwrite(buffer, buffer_size, 1, outptr);
               }
          }

          else if (recovering_jpeg)
          {
               if (read_return != buffer_size)
               {
                    fwrite(buffer, read_return, 1, outptr);
                    recovering_jpeg = false;
               }

               else
               {
                    fwrite(buffer, buffer_size, 1, outptr);
               }
          }

          else
          {
              continue;
          }
     }

     // free memory
     free(buffer);
}

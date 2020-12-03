#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

/*
int check_bmp_header(BITMAPFILEHEADER bf, BITMAPINFOHEADER bi){

}
*/

int main(int argc, char *argv[]){
    // ensure proper usage
    if (argc != 4)
    {
        fprintf(stderr, "Usage: resize f infile outfile\n");
        return 1;
    }

    // resize ratio
    double resize_ratio = atof(argv[1]);

    if(resize_ratio <= 0 || resize_ratio > 100)
    {
        fprintf(stderr, "The resize factor, must satisfy 0 < n <= 100.\n");
        return 1;
    }

    // remenber filenames
    char *infile = argv[2];
    char *outfile = argv[3];

    // open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL){
        fprintf(stderr, "Could not open %s.\n", infile);
        return 2;
    }

    // open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL){
        fclose(inptr);
        fprintf(stderr, "Could not create %s.\n", outfile);
        return 3;
    }

    // read infile's BITMAPFILEHEADER
    // recall sizeof(BITMAPFILEHEADER) = 14 bytes
    // 第三個 element 1 表示從inptr所指向的內容 1 次讀取至 &bf 位置的 buffer
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // read infile's BITMAPINFOHEADER
    // recall sizeof(BITMAPINFOHEADER) = 40 bytes
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    // bfOffBits = 14 + 40 = 54
    // 0x4d42 表示 BM
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        fprintf(stderr, "Unsupported file format.\n");
        return 4;
    }

    // determine original padding for scanlines
    // width must be multiplication of 4
    int inp_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // calculate the width & height of the output file
    bi.biWidth = (int)(resize_ratio * bi.biWidth);
    bi.biHeight = (int)(resize_ratio * bi.biHeight);

    // determine padding for scanlines
    // width must be multiplication of 4
    int out_padding = (4 - (bi.biWidth * sizeof(RGBTRIPLE)) % 4) % 4;

    // calculate new biSizeImage & bfSize
    bi.biSizeImage = (bi.biWidth * sizeof(RGBTRIPLE) + out_padding) * abs(bi.biHeight);
    bf.bfSize = bi.biSizeImage + sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);


    // write outfile's BITMAPFILEHEADER & BITMAPINFOHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    int i_, j_ = 0;

    // iterate over outfile's scanlines
    for (int i = 0, biHeight = abs(bi.biHeight); i < biHeight; i++)
    {
        i_ = 3 * (int)(i/resize_ratio) * (int)(bi.biWidth/resize_ratio) + (int)(i/resize_ratio) * inp_padding;
        int seek_i = ftell(inptr);
        //fseek(inptr, bf.bfOffBits + i_, SEEK_SET);
        //seek_i = ftell(inptr);
        // iterate over pixels in scanline
        for (int j = 0; j < bi.biWidth; j++)
        {
            // temporary storage
            RGBTRIPLE triple;

            // move the input file cursor
            seek_i = ftell(inptr);
            j_ = 3 * (int)(j/resize_ratio);
            fseek(inptr, bf.bfOffBits + i_ + j_, SEEK_SET);
            seek_i = ftell(inptr);

            // read RGB triple from infile
            fread(&triple, sizeof(RGBTRIPLE), 1, inptr);

            // write RGB triple to outfile
            fwrite(&triple, sizeof(RGBTRIPLE), 1, outptr);
        }

        // skip over padding, if any
        // fseek(inptr, inp_padding, SEEK_CUR);

        // add padding back
        for (int k = 0; k < out_padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // close infile
    fclose(inptr);


    // close outfile
    fclose(outptr);

    // success
    return 0;





}

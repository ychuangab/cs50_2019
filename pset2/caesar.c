#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

// check if the char is a number or not
int isNum(char c)
{
    if(c <= '9' && c >= '0')
    {
        return true;
    }

    else
    {
        return false;
    }
}

// check if the char is an alphabet or not
int isAlpha(char c)
{
    if((c <= 'z' && c >= 'a') || (c <= 'Z' && c >= 'A'))
    {
        return true;
    }

    else
    {
        return false;
    }
}


// check string arg is an integer
int argvCheck(string s)
{
    int len = strlen(s);
    for(int i = 0; i <= len - 1; i++)
    {
        if(!isNum(s[i]))
        {
            // printf("%c\n", s[i]);
            return false;
        }
    }
    return true;
}


int main(int argc, string argv[])
{
    int input_cnt = argc;
    if(input_cnt != 2)
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }

    string s = argv[1];
    if(argvCheck(s))
    {
        //printf("success!!!!\n");
        int k = atoi(argv[1]);
        string plaintext = get_string("plaintext: ");
        string ciphertext = plaintext;
        int len = strlen(plaintext);
        for(int i = 0; i <= len - 1; i++)
        {
            if(isAlpha(ciphertext[i]))
            {    
                int tmp = ciphertext[i] + (k % 26);
                //printf("%i\n", tmp);

                // Capital out boundary case
                if(tmp > 'Z' && tmp < 'a')
                {
                    ciphertext[i] = 'A' + (tmp - 'Z' - 1);
                }
                //ciphertext[i] = (ciphertext[i]) + (k % 26);
                
                // Lower out boundary case
                else if(tmp > 'z')
                {
                    ciphertext[i] = 'a' + (tmp - 'z' - 1);
                }

                // Normal case
                else
                {
                    ciphertext[i] = (ciphertext[i]) + (k % 26);
                }
                //printf("%i, %i\n", plaintext[i], ciphertext[i]);

            }
        }
        printf("ciphertext: %s\n", ciphertext);
    }

    else
    {
        printf("Usage: ./caesar key\n");
        return 1;
    }
}


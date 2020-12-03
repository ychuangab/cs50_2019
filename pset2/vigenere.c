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
        if(!isAlpha(s[i]))
        {
            // printf("%c\n", s[i]);
            return false;
        }
    }
    return true;
}


int* argv_to_num(string s)
{
    int key_len = strlen(s);
    int *res = malloc(key_len * sizeof(int));
    for(int i = 0; i < key_len; i++)
    {
        char c = s[i];
        if(c <= 'z' && c >= 'a')
        {
            res[i] = (int)s[i] - (int)('a');
        }

        else
        {
            res[i] = (int)s[i] - (int)('A');
        }

        //printf("%i\n", res[i]);
    }
    
    return res;
}


int main(int argc, string argv[])
{
    int input_cnt = argc;
    if(input_cnt != 2)
    {
        printf("Usage: ./vigenere keyword\n");
        return 1;
    }

    string s = argv[1];
    if(argvCheck(s))
    {
        //printf("success!!!!\n");
        string plaintext = get_string("plaintext: ");
        string ciphertext = plaintext;
        int key_len = strlen(argv[1]);
        int *key;
        key = argv_to_num(argv[1]);
        int plaintext_len = strlen(plaintext);


        for(int i = 0, j = 0; i <= plaintext_len - 1; i++)
        {
            if(isAlpha(ciphertext[i]))
            {   
                if(j + 1 > key_len)
                {
                    j = 0;
                }
                int k = key[j];
                
                //printf("%i, %i, %i, %i\n", i, j, k, key_len);
                int tmp = ciphertext[i] + (k % 26);
                //printf("%i\n", tmp);

                // Capital out boundary case
                if(ciphertext[i] <= 'Z' && tmp > 'Z')
                {
                    ciphertext[i] = 'A' + (tmp - 'Z' - 1);
                }
                //ciphertext[i] = (ciphertext[i]) + (k % 26);
                
                // Lower out boundary case
                else if(ciphertext[i] <= 'z' && tmp > 'z')
                {
                    ciphertext[i] = 'a' + (tmp - 'z' - 1);
                }

                // Normal case
                else
                {
                    ciphertext[i] = (ciphertext[i]) + (k % 26);
                }
                //printf("%i, %i\n", plaintext[i], ciphertext[i]);
                //printf("%i, %i\n", k, tmp);
                j++;

            }
        }
        printf("ciphertext: %s\n", ciphertext);
    }

    else
    {
        printf("Usage: ./vigenere keyword\n");
        return 1;
    }
}


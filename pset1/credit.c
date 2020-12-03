#include <cs50.h>
#include <stdio.h>

int check_digits(long num)
{
    if (num == 0)
    {
        return 1;
    }

    int digits = 0;
    while (num != 0)
    {
        num /= 10;
        digits += 1;
    }
    return digits;
}

// 除以10的n次方之後，再和10取餘數
int get_n_digit(long num, int n)
{
    long denominator = 1;
    int i = 1; 
    int res;

    while (i != n)
    {
        denominator *= 10;
        i += 1;
    }

    res = num / denominator % 10;
    return res;
}

void luhn_alg(long num, int digits)
{
    int sum = 0;
    int i;
    int tmp;

    for(i = 1; i <= digits; i++)
    {
        int i_digit = get_n_digit(num, i);
        //printf("%i\n", get_n_digit(num, i));
        if (i % 2)
        {
            sum += i_digit;
        }
        else
        {
            tmp = 2*i_digit;
            sum += ((tmp % 10) + (tmp / 10));
        }
    }

    //printf("%i\n", sum);

    if (sum % 10)
    {
        printf("INVALID\n");
    }

    else
    {
        int head_check = 10 * get_n_digit(num, digits) + get_n_digit(num, digits - 1);
        if (digits == 15 && (head_check == 34 || head_check == 37))
        {
            printf("AMEX\n");
        }

        else if (digits == 13 || digits == 16)
        {
            //printf("%i\n", mc_check);
            if (head_check >= 51 && head_check <= 55)
            {
                printf("MASTERCARD\n");
            }
            else if (head_check / 10 == 4)
            {
                printf("VISA\n");
            }
            else
            {
                printf("INVALID\n");
            }
        }
        else
        {
            printf("INVALID\n");
        }
        
    }
}


int main(void)
{
    long num = get_long("Number: ");
    //printf("%li\n", num);
    
    // Make sure that num > 0
    while (num < 0)
    {
        num = get_long("Number: ");
    }

    int digits = check_digits(num);

    if (digits == 13 || digits == 15 || digits == 16)
    {
        // Luhn’s Algorithm
        luhn_alg(num, digits);
        return 0;
    }

    else 
    {
        printf("INVALID\n");
        return 0;
    }
}

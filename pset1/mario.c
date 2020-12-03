#include <cs50.h>
#include <stdio.h>

// Print the paramids.
void hash_pyramids(int height_cnt)
{
    // Declare int vars
    int layer;
    int space;
    int hash;

    // The outter most for loop: layer
    for (layer = 1; layer <= height_cnt; layer++)
    {
        // Keying enough spaces
        for (space = height_cnt - layer; space > 0; space--)
        {
            printf(" ");
        }

        // Keying enough hashes
        for (hash = 1; hash <= layer; hash++)
        {
            printf("#");
        }

        // Always keep 2 spaces between the hashs for the same row
        printf("  ");

        // Keying enough hashes after 2 spaces
        for (hash = 1; hash <= layer; hash++)
        {
            printf("#");
        }
        // Change to next layer
        printf("\n");
        
    }
}

int main(void)
{
    int input_height = get_int("Height: ");
    
    // Repeat asking until user input the correct height.
    while (input_height < 1 || input_height > 8)
    {
        input_height = get_int("Height: ");
    }

    // Print the paramids if user input 1~8.
    hash_pyramids(input_height);
}

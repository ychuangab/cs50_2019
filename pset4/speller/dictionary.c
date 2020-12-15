// Implements a dictionary's functionality

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents number of children for each node in a trie
#define N 27

// Represents a node in a trie
typedef struct node
{
    bool is_word; //記錄某點是否為單字的結尾
    struct node* children[N]; //26字母 + 1 null
}
node;

// Represents a trie (claim first, allocate memory at "load")
node* root;
unsigned int total_words = 0;
unsigned int total_words_1 = 0;


// convert the char of word into a~z: [0~25] ; ': 26
int convert_char(char c)
{
    int res;
    if(c <= 'z' && c >= 'a')
    {
        res = c - 'a';
    }
    else if(c <= 'Z' && c >= 'A')
    {
        res = c - 'A';
    }
    else if(c == '\'')
    {
        res = 26;
    }
    else
    {
        res = 100;
    }
    return res;
}


// Loads dictionary into memory, returning true if successful else false
bool load(const char* dictionary)
{
    // Initialize trie
    root = malloc(sizeof(node));
    if (root == NULL)
    {
        return false;
    }
    root->is_word = false;
    for (int i = 0; i < N; i++)
    {
        root->children[i] = NULL;
    }

    // Open dictionary
    FILE* file = fopen(dictionary, "r");
    if (file == NULL)
    {
        unload();
        return false;
    }

    // Buffer for a word
    char word[LENGTH + 1];
    int word_len;
    int i_cur_char;
    node* cur_node;
    node* new_node;

    // Insert words into trie
    /*
        scanf()的返回值為成功輸入的數據個數
        如scanf（"%d%d%s",&a,&b,s）;執行成功返回3.
        scanf（"%d%d",&a,&b）;執行成功返回2.
        如果在輸入的過程中scanf("%d%d",&a,&b);由於某種原因只有a輸入成功了則返回1，a、b都沒成功則返回0
        如果遇到錯誤或遇到end of file，返回EOF（一般宏定義EOF為-1）。
    */
    while (fscanf(file, "%s", word) != EOF)
    // https://zhuanlan.zhihu.com/p/69038922
    {
        // TODO  - done
        //printf("%s, %lu, ", word, strlen(word));
        //printf("%s\n", root->is_word ? "true" : "false");
        word_len = strlen(word);
        cur_node = root;

        for(int i = 0; i < word_len; i++)
        {
            i_cur_char = convert_char(word[i]);

            // Not a valid word, break for next word
            if(i_cur_char == 100)
            {
                break;
            }

            else
            {
                if(cur_node->children[i_cur_char] == NULL)
                {
                    new_node = malloc(sizeof(node));

                    // Case when unable to allocate new one
                    if (new_node == NULL)
                    {
                        unload();
                        return false;
                    }

                    new_node->is_word = false;
                    for (int j = 0; j < N; j++)
                    {
                        new_node->children[j] = NULL;
                    }
                    cur_node->children[i_cur_char] = new_node;

                }
                cur_node = cur_node->children[i_cur_char];

                if(i == word_len - 1)
                {
                    cur_node->is_word = true;
                    total_words++;
                }
            }
        }

    }
    // Close dictionary
    fclose(file);

    // Indicate success
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{
    // TODO - done
    //printf("%i\n", total_words);

    //unsigned int res = 0;
    //return res;

    return total_words;
}

unsigned int cur_node_size(node* cur_node)
{
    for(int i = 0; i < N; i++)
    {
        if(cur_node->children[i])
        {
            cur_node_size(cur_node->children[i]);
        }

    }
    if(cur_node->is_word)
    {
        total_words_1++;
    }
    return total_words_1;
}

// Returns true if word is in dictionary else false
bool check(const char* word)
{
    // TODO
    // printf("%lu\n", strlen(word));
    int word_len = strlen(word);
    int i_cur_char;
    node* cur_node = root;
    bool res = false;

    // check the string
    for(int i = 0; i < word_len; i++)
    {
        i_cur_char = convert_char(word[i]);

        // Not a valid word, retrurn false for misspelled.
        if(i_cur_char == 100)
        {
            //printf("%s\n", res ? "true" : "false");
            return res;
        }

        else
        {
            // children[i] exist, go to next node
            if(cur_node->children[i_cur_char] != NULL)
            {
                cur_node = cur_node->children[i_cur_char];
            }

            // children[i] not exist, return false
            else
            {
                //printf("%s\n", res ? "true" : "false");
                return res;
            }
        }
    }

    // return true if end_node.is_word else false
    if(cur_node->is_word)
    {
        res = true;
        //printf("%s\n", res ? "true" : "false");
        return res;
    }

    //printf("%s\n", res ? "true" : "false");
    return res;
}

//
void unload_cur_node(node* cur_node)
{
    for(int i = 0; i < N; i++)
    {
        if(cur_node->children[i])
        {
            unload_cur_node(cur_node->children[i]);
        }
    }
    free(cur_node);
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    // TODO
    unload_cur_node(root);
    return true;
}

/*
int main(void)
{
    const char* word = "Hello";
    load("dictionaries/small");
    size();
    check(word);
    unload();
    //printf("%i\n", convert_char('Z'));
}
*/

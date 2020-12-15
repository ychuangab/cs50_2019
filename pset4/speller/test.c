#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

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
node* root1;


typedef struct linked
{
    char c;
    struct linked* next;
}
linked;
linked* root2;




int main(void)
{
  // Initialize trie
  node* children[N];
  linked* next;
  bool is_word;
  unsigned long a = sizeof(node);
  unsigned long b = sizeof(is_word);
  unsigned long c = sizeof(children);

  unsigned long d = sizeof(linked);
  unsigned long e = sizeof(next);
  unsigned long f = sizeof(char);

  printf("%lu\n", sizeof(node));
  printf("%lu\n", sizeof(linked));

  //printf("%d\n",sizeof(a));
  root1 = malloc(sizeof(node));
  if (root1 == NULL)
  {
      return false;
  }
}
// Declares a dictionary's functionality

#ifndef DICTIONARY_H
#define DICTIONARY_H

#include <stdbool.h> //Define bool type. don't needed it before, since the presence of CS50

// Maximum length for a word
// (e.g., pneumonoultramicroscopicsilicovolcanoconiosis)
#define LENGTH 45

// Prototypes
bool load(const char *dictionary); //string
unsigned int size(void);
bool check(const char *word);  //string
bool unload(void);

#endif // DICTIONARY_H

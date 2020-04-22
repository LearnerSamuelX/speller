// Implements a dictionary's functionality

#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "dictionary.h"

// Represents a node in a hash table
// !!! Each node can store a word that can have maximum 45 characters in it
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;
//check function
node *dicword;   //a word from a text file;


// Number of buckets in hash table
const unsigned int N = 26;
node *hashtable[N]={NULL}; //initialize an array of node pointers with each node linking to empty lists
node *cursor; //only has address
int wordCount = 0;
int hashindex;
node *newWord; //create a node variable with the name newWord:   top: a new word   bottom: a pointer
// node *list = NULL; //this node was created in place of hashtable for the purpose of simplifying the problem

// Hashtable
int result; //used in spelling comparsion, if right = 0 , if wrong = whatever the difference is
node *table[N];
char lcword[LENGTH];
int checkhash;  //the hash value of the word from the file, implemented in the check function


// temporary nodes in unloading
node *temp;

// Returns true if word is in dictionary else false
bool check(const char *word)
//this function takes only ONE word, NOT THE WHOLE FILE
{

    strcpy(lcword, word);
    int len = strlen(lcword);

    // for (int i=0;i<len;i++)
    // {
    //     lcword[i]=tolower(word[i]);   //make lowercase word as the ONLY standard here
    // }
    // the above part generates segmentation fault

    for (int i = 0; i<len+1; i++)
    {
        lcword[i] = tolower(lcword[i]);
    }
    // conversion works

    checkhash=hash(lcword);  //return the hash value that the word in a document, NOT DICTIONARY is affliated to (first charater)
    // checkhash works
    // checkhash value is consistent with hashindex, see below

    if (hashtable[checkhash]==NULL)
    {
        return false;
    }

    else
    {
        dicword=hashtable[checkhash]->next;
        while(hashtable[checkhash]!=NULL)
        {
            result = strcmp(dicword->word,lcword);
            if(result==0)
            {
                return true;  //found the matched word in dictionary
            }
            else if (result<0||dicword->next==NULL)
            {
                return false;
            }
            else
            {
                dicword=dicword->next;
            }
        }
        return false;
    }
}


// Hashes word to a number, hash function
// 25 if statement?? THAT would be silly
unsigned int hash(const char *str)
{
    unsigned long hash = 5381;
    int c;

    // c = *str;
    // hash = ((hash << 5) + hash) + c;

    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c;
    }

    return hash % N;
}

// Loads dictionary into memory, returning true if successful else false

bool load(const char *dictionary)
{
    char record[LENGTH + 1];
    FILE *fp; //it is a FILE pointer fp
    fp=fopen(dictionary,"r");
    if(fp==NULL)
    {
        printf("No file is being loaded.\n");
        return false;
    }
    else
    {
        while (fscanf(fp, "%s", record) !=EOF)
        {
            wordCount++;
            //bringing in the hashfunction
            hashindex=hash(record);
            //something is missing here???
            newWord=malloc(sizeof(node));
            if (newWord==NULL)
            {
                fclose(fp);
                unload();
                return false;
            }
            strcpy(newWord->word,record);
            newWord->next=hashtable[hashindex];  //replace list with a hashtable[value]
            hashtable[hashindex] = newWord; //replace list with a hashtable[value]
            // printf("%i",hashindex);
        }
        // strcpy(hashtable[hashindex]->word,record); //replace list with a hashtable[value]
    }
    // printf("\nThere are %i words copied from the Dictionary.\n",wordCount);
    fclose(fp);
    return true;
}

// Returns number of words in dictionary if loaded else 0 if not yet loaded
unsigned int size(void)
{

    return wordCount;
}

// Unloads dictionary from memory, returning true if successful else false
bool unload(void)
{
    for (int k=0;k<N;k++)  //looping through the hash array which is consisted of N "head" nodes
    {
        cursor = hashtable[k]->next;
        temp = cursor;
        cursor = cursor->next;
        free(temp);
    }
    return true;
}


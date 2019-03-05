/**************************
 * Elijah Johnson
 * Lab Secion 001
 * Tests all of the functions in hashtree.c
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "hashtree.h"

struct TreeNode* createHashTree(char **data, int numData);

/****************
 * Prints out a proof
 * @prama *proof the proof to print out that is a linked list
 */
void printProof(struct HashNode* proof)
{ /*Print out the linked list*/
  if (proof)
  {
    printf("%s\n", proof->hash);
    if (!proof->next)
    {
      printf("\n");
    }
    printProof(proof->next);
  }
}

/*************************
 * Free a linked list
 * @param *proof the linked list to free
 */
void freeProof(struct HashNode* proof)
{
  if (proof)
  {
    freeProof(proof->next);
    free(proof->hash);
    free(proof);
  }
}

/********************
 * Testing data for hashtree.c
 */
int main () 
{
  int i;
  char *data[32] = {"bobby", "bobby", "bobby", "banana", 
    "missippissi", "mango", "fruits", "steak", "United States", "words", 
    "this is a long long long long test string", "this is a test string", "a",
    "trombone", "trumpet", "saxophone", "test", "New Mexico", "UNM", "CS241"};
  /*Create 20 hash trees and test random things in the hash trees */
  for (i = 0; i < 20; i++)
  {
    struct TreeNode *root = createHashTree(data, i);
    struct HashNode *proof = giveProof(root, data[(i * 7 + 12) % 20]);
    if (!proof)
    {
      printf("%s is not in the hash tree\n", data[(i * 7 + 12) % 20]);
    }
    else
    {
      printf("%s is in hash tree\n", data[(i * 7 + 12) % 20]);
      printf("Proof: \n");
      printProof(proof);
    }
    printTree(root);
    freeTree(root);
    freeProof(proof);
  }
  /* return */
  return 0;
}

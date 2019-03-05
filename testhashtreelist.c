/*************************
 * Elijah Johnson
 * Lab Section 001
 * Tests the functions in hashtreelist.c
 */

#include <stdio.h>
#include <stdlib.h>
#include "hashtreelist.h"

struct TreeNode* createHashTree(char **data, int numData);


/************
 * Main method
 */
int main()
{
  /*Create variables needed */
  struct TreeListNode* last = NULL;
  int i;
  char *data[3][8] = { 
    {"test", "empty", "saxophone", "to", "be", "cat", "test2", "elijah"}, 
    {"hashtree", "welcome", "brandon", "likes", "cats", "because", "cats", "like"},
    {"dominic", "these", "are", "test", "words", "steak", "rare", "blue"}
  };
  /*Loop through and create hashtrees from the data and add them to the list*/
  for (i = 0; i < 3; i++)
  {
    struct TreeNode *root = createHashTree(data[i], 8);
    last = addNode(last, root);
  }
  /*Test functions*/
  printf("find 'test' %d\n", findData(last, "test"));
  printf("find 'not in a tree' %d\n", findData(last, "not in a tree"));
  printf("find 'empty' %d\n", findData(last, "empty"));
  printf("check wrong hash %d\n", checkHash(last, 2, 
        "ECD8E42BD81299925445FBE7B166A8627CDFF8913A3E7F4F4B35EB9F1341BA01"));
  printf("check correct hash %d\n", checkHash(last, last->prev->height, 
        last->prev->root->data));
  freeList(last);
  return 0;
}

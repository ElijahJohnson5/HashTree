#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtree.h"
#include "hashtreelist.h"
#include "readtree.h"

int main(void)
{
  struct TreeListNode* h1;
  struct TreeListNode* h2;
  h1 = readInFiles("hashtree.in"); /* You might have to edit this to be an absolute file path*/
  h2 = addNode(NULL,createBranchNode(createLeafNode("charlie"), NULL));
  printf("charlie in chain: %d\n",findData(h1,"charlie"));
  printf("charlies in chain: %d\n",findData(h1,"charlies"));
  printf("apparatus in chain: %d\n",findData(h1,"apparatus"));
  printf("Apparatus in chain: %d\n",findData(h1,"Apparatus"));
  printf("0 hash at height 3: %d\n",checkHash(h1,3,"0"));
  printf("Correct hash at height 1: %d\n",checkHash(h1,4,h2->root->data));
  freeList(h1);
  freeList(h2);
  return 0;
}

/************************
 * Elijah Johnson
 * Lab Section 001
 * Implements the treelistNode creating and adding of nodes
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtreelist.h"

/***********************
 * Creates a node 
 * @param root the root treenode to set in the Tree list node
 * @return the newly created node or NULL
 */
struct TreeListNode* createNode(struct TreeNode* root)
{
  struct TreeListNode* newNode = malloc(sizeof(struct TreeListNode));
  if (!newNode)
  {
    return NULL;
  }

  newNode->root = root;
  newNode->prev = NULL;
  newNode->height = 0;
  
  return newNode;
}

/*********************
 * Add a node to the tree list
 * @param lastNode the last node of the list
 * @param root the root of the new node
 * @return the newly created node which is now the end of the list
 */
struct TreeListNode* addNode(struct TreeListNode* lastNode, struct TreeNode* root)
{
  struct TreeListNode* newNode = createNode(root);
  newNode->prev = lastNode;
  if (lastNode)
  {
    /*Increment height */
    newNode->height = lastNode->height + 1;
  }
  return newNode;
}

/*********************
 * Checks if the data is in the tree list and returns the height if it
 * is
 * @param *lastNode the lastNode of the list
 * @param *data the data to try and find
 * @return -1 if it is not found or the height if it is found
 */
int findData(struct TreeListNode* lastNode, char *data)
{
  if (!lastNode)
  {
    return -1;
  }

  if (verifyDataInTree(lastNode->root, data))
  {
    return lastNode->height;
  }
  
  return findData(lastNode->prev, data);
}

/*******************
 * Checks if the *hash matches the root hash at the height
 * @param *lastNode the list of TreeLIstNodes
 * @param height the height to check the hash at
 * @param *hash the hash to compare with the root hash
 */
int checkHash(struct TreeListNode* lastNode, int height, char* hash)
{
  if (!lastNode || lastNode->height < height)
  {
    return 0;
  }

  if (lastNode->height == height && !strcmp(lastNode->root->data, hash))
  {
    return 1;
  }
  
  return checkHash(lastNode->prev, height, hash);
}

/**********************
 * Frees the TreeListNode list
 * @param *lastNode the end of the list to free
 */
void freeList(struct TreeListNode* lastNode)
{
  if (lastNode)
  {
    freeList(lastNode->prev);
    freeTree(lastNode->root);
    free(lastNode);
  }
}

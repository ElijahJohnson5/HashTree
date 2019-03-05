/*********************
 * Elijah Johnson
 * File that handles dealing with a hash tree
 * creating and freeing and giving the merkle proof for data
 * in the hash tree
 * Lab Section 001
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "hashtree.h"

#define LONESHA256_IMPLEMENTATION

#include "lonesha256.h"

/*******************
 * Calculates the number of nodes that will be in the hashtree
 * @param numLeafs the number of leafNodes  
 */
int calcNumberOfNodes(int numLeafs)
{
  int sum = numLeafs;
  while (numLeafs > 1)
  {
    /* Sum the levels of the hashTree */
    sum += (numLeafs % 2) ? (numLeafs / 2) + 1 : (numLeafs / 2);
    numLeafs = (numLeafs % 2) ? (numLeafs / 2) + 1 : (numLeafs / 2);
  }

  return sum;
}

/****************************
 * Creates a hashtree with numData number of leaves
 * @param **data the array of strings of the leaves data
 * @param numData the number of leaf data
 * @return the root of the hash tree or NULL if it fail
 */
struct TreeNode* createHashTree(char **data, int numData)
{
  int i;
  int currentRowAmount = numData;
  int max = calcNumberOfNodes(numData);
  struct TreeNode* root = NULL;
  /*Malloc the number of leaves and branches thatll be in the tree*/ 
  struct TreeNode** leaves = malloc(sizeof(struct TreeNode*) * numData);
  struct TreeNode** branches = malloc(sizeof(struct TreeNode*) * max);
  if (numData == 0)
  {
    free(leaves);
    free(branches);
    return NULL;
  }
  /*Create leaves*/
  for (i = 0; i < numData; i++)
  {
    leaves[i] = createLeafNode(data[i]);
  }
  /*Create the branches in the right order */
  for (i = 0; i < max; i++)
  {
  if (currentRowAmount <= 0)
    {
      currentRowAmount = numData;
    }

    if (i < numData)
    {
      branches[i] = createBranchNode(leaves[i], NULL);
    }
    else if (currentRowAmount > 1)
    {
      branches[i] = createBranchNode(branches[i - numData], branches[i - (numData - 1)]);
      numData--;
      currentRowAmount -= 2;
    }
    else
    {
      branches[i] = createBranchNode(branches[i - numData], NULL);
      currentRowAmount--;
    }
    
  }
  /*Get the root branch and free malloced data*/
  root = branches[max - 1];
  free(leaves);
  free(branches);
  return root;
}

/*******************
 * Hashes the given string with a sha256 hashing function
 * @param *data a pointer to the string to hash
 * @return the newly hashed string as a string of hex characters
 */
char *hash(char* data)
{
  int i;
  /* malloc a string to hold the new hash */
  char *temp = malloc(sizeof(char) * (64 + 1));
  /*Out paramater for the sha256 function*/
  unsigned char out[32];
  if (!temp)
  {
    return NULL;
  }
  /*Call the lonesha256 function, if it returns anything but 0
   * return NULL because it failed */
  if (lonesha256(out, (const unsigned char *)data, strlen(data)))
  {
    free(temp);
    return NULL;
  }
  /*Read the bytes of the out string into the temp string*/
  for (i = 0; i < 32; i++)
  {
    sprintf((temp + (i * 2)), "%02X", out[i]);
  }
  /*Make sure it is null terminated */
  temp[64] = '\0';
  return temp;
}

/***************************
 * Creates a new leaf node with the data given to it
 * @param *data the data to be stored in the leaf node
 * @return the newly created TreeNode with the correct data
 */
struct TreeNode* createLeafNode(char *data)
{
  /*Malloc memory*/
  struct TreeNode* newLeaf = malloc(sizeof(struct TreeNode));
  if (!newLeaf)
  {
    return NULL;
  }
  /*It is a leaf */
  newLeaf->isLeaf = 1;
  /*Malloc memory for string */
  newLeaf->data = calloc(strlen(data) + 1, sizeof(char));
  strncpy(newLeaf->data, data, strlen(data));
  /*Point left and right to NULL */
  newLeaf->left = NULL;
  newLeaf->right = NULL;
  return newLeaf;
}

/**********************
 * Creates a branch node with two children nodes
 * @param child1 the left child for the branch can be NULL
 * @param child2 the right child node for the branch can be NULL
 * @return the newly created treenode that is a branch
 */
struct TreeNode* createBranchNode(struct TreeNode* child1, struct TreeNode* child2)
{
  char *dataToHash;
  /*Malloc memory*/
  struct TreeNode* newBranch = malloc(sizeof(struct TreeNode));
  /*Check if malloc failed*/
  if (!newBranch)
  {
    return NULL;
  }
  /*If both children are NULL free the memory and return NULL*/
  if (child1 == NULL && child2 == NULL)
  {
    free(newBranch);
    return NULL;
  }
  
  /*It is not a leaf*/
  newBranch->isLeaf = 0;
  
  /*If one child or the other is NULL */
  if (!child1 || !child2)
  {
    /*Set data to hash to the childs data that is not NULL */
    dataToHash = (child1) ? (child1->data) : child2->data;
    /*Hash the data*/
    newBranch->data = hash(dataToHash);
    /*Set the left child to the child that is not NULL*/
    newBranch->left = (child1) ? child1 : child2;
    newBranch->right = NULL;
  }
  else
  {
    /*We have both children concat the hashes together*/
    dataToHash = malloc(strlen(child1->data) + strlen(child2->data) + 1);
    strncpy(dataToHash, child1->data, strlen(child1->data) + 1);
    strncat(dataToHash, child2->data, strlen(child2->data) + 1);
    /*hash the concatenated data */
    newBranch->data = hash(dataToHash);
    /*Free the memory we are done with*/
    free(dataToHash);
    dataToHash = NULL;
    /*Set the left and right to the correct children*/
    newBranch->left = child1;
    newBranch->right = child2;
  }
  /*Return the newly created branch*/
  return newBranch;
}

/******************************
 * Preform a depth first search on a hashtree
 * looking for data in the leaves
 * @param *hashTree the hashTree to search can be NULL
 * @param *data the data to search for cannot be NULL
 * @return 0 if not found otherwise 1
 */
int dfs(struct TreeNode* hashTree, char *data)
{
  int returnData;

  if (hashTree && data)
  {
    if (hashTree->isLeaf && !strcmp(hashTree->data, data))
    {
      return 1;
    }
    else
    { /*Search left then right for data, making sure to return if we 
        find it */
      returnData = dfs(hashTree->left, data);
      if (returnData)
      {
        return returnData;
      }
      returnData = dfs(hashTree->right, data);
      if (returnData)
      {
        return returnData;
      }
    }
  }

  return 0;
}

/*******************
 * Verifys that the *data is in the tree, returns 0 if it isnt and 1 if it is
 * @param *hashTree the hashtree to search can be NULL
 * @param *data the data to search for cannot be NULL
 * @return either 1 if found or 0 if not
 */
int verifyDataInTree(struct TreeNode* hashTree, char* data)
{
  if (!hashTree)
  {
    return 0;
  }
  return dfs(hashTree, data);
}

/**********************
 * Helper function for print tree, prints out the tree
 * in order
 * @param *root the current node we are on
 */
void printTreeHelper(struct TreeNode* root)
{
  if (root)
  {
    printTreeHelper(root->left);
    printf("%s ", root->data);
    printTreeHelper(root->right);
  }
}

/***************
 * Prints out a tree inorder
 * @param root the root of the tree
 */
void printTree(struct TreeNode* root)
{
  printTreeHelper(root);
  printf("\n");
}

/*************************
 * Frees a hash tree 
 * @param root the hashtree to free
 */
void freeTree(struct TreeNode* root)
{
  if (root)
  {
    freeTree(root->left);
    freeTree(root->right);
    free(root->data);
    free(root);
  }
}

/*************************
 * Creates a new hashnode with hash as its hash
 * @param *hash the hash to store in the hashNode
 * @return the newly created hashNode
 */ 
struct HashNode* createHashNode(char *hash)
{
  /*Malloc the memory*/
  struct HashNode* tmp = malloc(sizeof(struct HashNode));
  if (!tmp)
  {
    return NULL;
  }
  /*Malloc the string and copy in the data */
  tmp->hash = malloc(strlen(hash) + 1);
  strcpy(tmp->hash, hash);

  tmp->next = NULL;

  return tmp;
}

/**********************************
 * Helper recursive function for give proof
 * @param *hashTree the current hashtree that we are giving proof for the data
 * @param *data the data to give the proof that it is in the hash tree
 * @param *head the head of the linked list that is being build
 * @param *currentNode the current last node of the linked list
 * @param *hashedData the hash of the data so we can check the branches for matches
 * @return the head of the linked list once we are at a leaf
 */
struct HashNode* giveProofHelper(struct TreeNode* hashtree, char *data, 
          struct HashNode* head, struct HashNode* currentNode, char *hashedData)
{
  struct HashNode* temp = NULL;
  short leftLeaf = 0;
  short rightLeaf = 0;
  /*Base case if we are right above a leaf*/
  if ((hashtree->left && hashtree->left->isLeaf) || (hashtree->right && hashtree->right->isLeaf))
  {
    return head;
  }
  
  /*Check if the data of the left is the same as the hashed data
   * if not add it to the list */
  if (hashtree->left && strcmp(hashedData, hashtree->left->data))
  {
    temp = createHashNode(hashtree->left->data);
    currentNode->next = temp;
    currentNode = temp;
    leftLeaf = 1;
  }
  
  /*Check if the data of the right is the same as the hashed dta
   * if not add it to the list*/
  if (hashtree->right && strcmp(hashedData, hashtree->right->data))
  {
    temp = createHashNode(hashtree->right->data);
    currentNode->next = temp;
    currentNode = temp;
    rightLeaf = 1;
  }
  
  /*If we have the same data on the left and right leaves pick the right
   * one arbitrarillay*/
  if (!leftLeaf && !rightLeaf)
  {
    temp = createHashNode(hashtree->right->data);
    currentNode->next = temp;
    currentNode = temp;
  }
  
  /*Recursively call on subtree that has the data */
  if (verifyDataInTree(hashtree->left, data))
  {
    return giveProofHelper(hashtree->left, data, head, currentNode, hashedData);
  }
  else
  {
    return giveProofHelper(hashtree->right, data, head, currentNode, hashedData);
  }
}

/**************************
 * Gives a merkle proof for the data or NULL if it is not in the tree
 * @param *hashtree the tree to give the proof for the data
 * @param *data the data to prove is in the tree
 * @return the head of the list created or NULL if the data isnt in the list
 */
struct HashNode* giveProof(struct TreeNode* hashtree, char* data)
{
  /*Set head to NULL and hash the data*/
  struct HashNode* head = NULL;
  char *hashedData = hash(data);
  if (!hashtree)
  {
    free(hashedData);
    return NULL;
  }
  /*If it isnt in the tree then return NULL*/
  if (!(verifyDataInTree(hashtree, data)))
  {
    free(hashedData);
    return NULL;
  }
  /*Always add the root*/
  head = createHashNode(hashtree->data);
  /*Call the recursive helper */
  head = giveProofHelper(hashtree, data, head, head, hashedData);
  /*Free memory allocated */
  free(hashedData);
  return head;
}

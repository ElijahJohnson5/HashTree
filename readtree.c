/**************************
 * Elijah Johnson
 * Lab Section 001
 * CS 241
 * Creates trees from words specified in files
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "readtree.h"
#include "hashtreelist.h"

#define BUF_SIZE 512

struct TreeNode* createHashTree(char **data, int numData);


/*******************
 * Reads words from a file and puts them into a hash tree 
 * @param filename the name of the file to read words from
 * @return the newly created hash tree from the words
 */
struct TreeNode* putFileIntoHashTree(char *filename)
{
  int lines = 0;
  int i = 0;
  char buf[BUF_SIZE];
  char **data = NULL;
  FILE *file = fopen(filename, "r");
  struct TreeNode* root = NULL;
  if (!file)
  {
    return NULL;
  }
  /*Count number of lines in file*/
  while (fgets(buf, BUF_SIZE, file) != NULL)
  {
    lines++;
  }
  /*Put file back to begining*/
  fseek(file, 0, SEEK_SET);
  data = malloc(sizeof(char *) * lines);
  /*Loop through and read all of the words and put them into an array */
  while (fgets(buf, BUF_SIZE, file) != NULL)
  {
    buf[strcspn(buf, "\n")] = '\0';
    data[i] = malloc(sizeof(char) * strlen(buf) + 1);
    strcpy(data[i], buf);
    i++;
  }
  /*Create the hash tree */
  root = createHashTree(data, lines);
  
  /*Free all nessecary data and close the file*/
  for (i = 0; i < lines; i++)
  {
    free(data[i]);
  }
  free(data);
  fclose(file);
  return root;
}

/**************************
 * Reads in one file that contains the names of other files
 * @param *filename the filename to be read in
 * @returns the treelist node that is created from the list of files
 */
struct TreeListNode* readInFiles(char *filename)
{
  char buf[BUF_SIZE];
  FILE *file = fopen(filename, "r");
  struct TreeListNode* last = NULL;
  if (!file)
  {
    return NULL;
  }
  /*Read in a line at a time and call putFileIntoHashTree*/
  while (fgets(buf, BUF_SIZE, file) != NULL)
  {
    buf[strcspn(buf, "\n")] = '\0';
    last = addNode(last, putFileIntoHashTree(buf));
  }

  fclose(file);
  return last;
}

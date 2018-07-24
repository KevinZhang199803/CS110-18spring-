#include"skiplist.h"
#include<stdlib.h>
#define RAND_HALF (RAND_MAX)/2
/* Head files and defined consts*/


/*
* First, implement the createSkipList function
* We need to create a skiplist and a header

*/
list createSkipList(int height_limit,
                         int (*cmp)(const void *a, const void *b),
                         void *(*alloc)(const void *),
                         void (*del)(void *))
{
	skip_list_node * empty = (skip_list_node *) malloc(sizeof(skip_list_node));
	/* Create the header for the skip list. */
	int i;
	skip_list * EmptyList = NULL; 
	/* Create an empty list */
	/* For C90 standard, pointers must be initialized. */
	empty->val = NULL;
	/* Make the header's value becomes NULL*/
	empty->key = NULL;
	/* Make the header's key becomes NULL*/
	empty->forward = NULL;
	/* Make the header's forward becomes NULL*/
	for (i=0;i<=height_limit;i++) empty->forward[i] = NULL;
	EmptyList->level = 1; 
	/* For an empty list, the level is 1. */
	EmptyList->height_limit = height_limit; 
	/* height_limit is given by the given parameter. */
	EmptyList->header = empty;
	/* Make the header as an empty node.*/
	EmptyList->cmp = cmp;
	/* give the comparison function to skiplist */
	EmptyList->alloc = *alloc;
	/* give the memory allocation function to skiplist */
	EmptyList->del = del;
	/* give the deletion function to skiplist */
	return EmptyList;
}
/* createSkipList ends */

/*
 * A function to search for the value corresponding to the key.
 * Return value:
 *      If the key exists, return a void * pointer to the value
 *      If the key does not exist, return NULL
 */


/* search a Node into the list */
/*
 * A function to search for the value corresponding to the key.
 * Return value:
 *      If the key exists, return a void * pointer to the value
 *      If the key does not exist, return NULL
 *		"Key" is the "location" of value.
 */
void *searchNode(list s, void *key)
{
	/* Level_Now is the variable used to record current level position.*/
	/* Loop index*/	
	skip_list_node *CurrentNode = s->header->forward[0];
	/* CurrentNode is the pointer to each node. */
	if (key == NULL) return NULL;
	/* For Special cases, NULL, return NULL */
	if (s->header->forward == NULL) return NULL;
	/* For an empty skiplist return NULL */
	while (CurrentNode -> forward[0] != NULL)
	{
		if (s -> cmp(CurrentNode -> key, key) == 0) return CurrentNode -> val;
		else CurrentNode = CurrentNode -> forward[0];
	}
	return NULL;
}
/* searchNode part ends*/


/*
 * A function to insert a new node with key & value determined by "key" into a list.
 * Return value:
 *      If the key does not exist before, insert the new key & value and return 0
 *      If the key already exists before, replace the value with the new one, and return 1
 *      return -1 on any kind of failure
 */
int insertNode(list s, void *key, void *val)
{
	int i,node_level = 0;
	skip_list_node *CurrentNode = s->header->forward[0];

	if (key == NULL || val == NULL) return -1;
	while (node_level < s->height_limit - 1 && rand() < RAND_HALF) ++node_level;
	if (s->header->forward == NULL)
	{
		skip_list_node * new_node = (skip_list_node *) malloc(sizeof(skip_list_node));
		new_node -> key = key;
		new_node -> val = val;
		new_node -> forward = NULL;
		for (i=0;i<=s->height_limit;i++) new_node -> forward[i] = NULL;
		for (i=0;i<=node_level;i++) s -> header -> forward[i] = new_node;
		if (node_level > s -> level) s -> level = node_level;

		return 0;
	}else{
		if (CurrentNode -> key == key){
			CurrentNode -> val = val;
			return 1;
		}
		while (CurrentNode -> forward[0] != NULL && CurrentNode -> forward[0] -> key < key) CurrentNode = CurrentNode -> forward[0];

		if (CurrentNode -> forward[0] -> key == key)
		{
			CurrentNode -> forward[0] -> val = val;
			return 1;
		}else if (CurrentNode -> forward[0] -> key > key){
			skip_list_node * new_node = (skip_list_node *) malloc(sizeof(skip_list_node));
			new_node -> key = key;
			new_node -> val = val;
			new_node -> forward = NULL;
			for (i=0;i<=s->height_limit;i++) new_node -> forward[i] = NULL;
			for (i=0;i<node_level;i++)
			{
				skip_list_node * find_node = s -> header -> forward[i];
				while (find_node -> forward[i] != NULL && find_node -> forward[i] -> key < key) find_node = find_node -> forward[i];
				new_node -> forward[i] = find_node -> forward[i];
				find_node -> forward[i] = new_node;
			}
			if (node_level > s -> level) s -> level = node_level;
			return 0;
		}
		else if (CurrentNode -> forward[0] == NULL){
			skip_list_node * new_node = (skip_list_node *) malloc(sizeof(skip_list_node));
			new_node -> key = key;
			new_node -> val = val;
			new_node -> forward = NULL;
			for (i=0;i<=s->height_limit;i++) new_node -> forward[i] = NULL;
			for (i=0;i<node_level;i++)
			{
				skip_list_node * find_node = s -> header -> forward[i];
				while (find_node -> forward[i] != NULL) find_node = find_node -> forward[i];
				new_node -> forward[i] = find_node -> forward[i];
				find_node -> forward[i] = new_node;
			}
			if (node_level > s -> level) s -> level = node_level;
			return 0;
		}
	}
	return -1;
}

/*
 * A function to remove a node with value given by "key" from a list.
 * Return value:
 *      return 0 on success
 *      return -1 on failure
 */
int deleteNode(list s, void *key)
{
	if (searchNode(s, key) == NULL)
		/*it means that there is no such node.*/
	{
		return -1;
		/*return -1*/
	}else{
		int i;
		/*initialize loop index*/
		skip_list_node *current = NULL;
		/*initialize pointer current to current processing node*/
		skip_list_node *temp = NULL;
		/*initialize pointer temporarily storing the node.*/
		for (i = 0;i <= s -> level; i++)
			/*correct the pointer before the deleted node to the next one*/
		{
			current = s->header->forward[i];
			/*start from the first node*/
			while (current -> forward[i] -> key != key && current != NULL) current = current->forward[i];
			/*keep going until meeting the key or to the end*/
			if (current -> forward[i] -> key == key)
				/*if the key is in the level and we find it*/
			{
				temp = current -> forward[i];
				current -> forward[i] = current -> forward[i] -> forward[i];
				/*change the pointer to the deleted pointer behind*/
			}
		}
		free(temp);
		/*free the memory allocated to the deleted node*/
		for (i=0;i<=s->height_limit;i++)
		{
			if (s->header->forward[i] == NULL) break;
		}
		/*this is for detecting the level of the node now.*/
		/*when the header directly points to NULL, it means it reaches the top.*/
		s->level = i-1;
		/*change the level of the whole skip_list.*/
		return 0;
	}
}

/*
 * A function to free all the dynamically allocated memory used by the tree.
 */
void freeSkipList(list s)
{
	skip_list_node *current = s -> header -> forward[0];
	/* From head to the end free*/
	skip_list_node *mid = NULL;
	/*used as temp varibles tp store the pointer*/
	while (current != NULL)
	{
		mid = current -> forward[0];
		/*store the memory the pointer points*/
		free(current);
		/*free the memory*/
		current = mid;
		/*pass the pointer to the next*/
	}
	free(s->header);
	/*free the final pointer, header*/
}

/*
The comparison function with inputs a and b will return -1 if a < b, return 0 if a==b and
    return 1 if a > b.
*/
int compareInt(const void *a, const void *b)
{
	int diff = *(int*) a - *(int*) b;
	/*Make difference between a and b */
	if (diff > 0)
	{
		/*diff positive, a>b*/
		return 1;
	}else if (diff == 0){
		/*0, equal */
		return 0;
	}else{
		/*negative, a<b*/
		return -1;
	}
}

void deleteInt(void *a)
{
	free(a);
	/*Just free the pointer to a.*/
}

void *allocInt(const void *a)
{
	int *loc = (int*) malloc(sizeof(int));
	/*loc is the  location of the allocated memory*/
	*loc = *(int*) a;
	/*give the location value, *a*/
	return loc;
	/*return the location pointer*/
}
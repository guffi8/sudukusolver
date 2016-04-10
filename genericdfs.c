/**
 * @file genericdfs.c
 * @author guffi
 * 
 * @brief This file implment the getbest function by dfs with stack.
 */
//--------------------------------------include----------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "genericdfs.h"
//------------------------------------const definitions--------------------------------------------
#define ERROR -1

/**
 * @def struct Node 
 * @brief define node struct each node include - data value (pNode) and pointer to the next Node 
 */
typedef struct Node
{
	pNode _data;
	struct Node * _next;
} Node;

/**
 * @def struct Stack
 * @brief define the stack struct
 */
typedef struct Stack
{
	Node * _top;
	size_t _elementSize;
} Stack;
//-------------------------------------------functions---------------------------------------------

/**
 * @def stackAlloc(size_t elementSize)
 * @brief alloc the stack
 * @param size_t elementSize - stack size
 * @return the stack
 */
Stack* stackAlloc(size_t elementSize)
{
	Stack* stack = (Stack*)malloc(sizeof(Stack));
	if(stack == NULL)
	{
		//allocation faild;
		return NULL;
	}
	stack->_top = NULL;
	stack->_elementSize = elementSize;
	return stack;
}

/**
 * @def freeStack
 * @brief free the stack from memory
 * @param the stack that need to be free
 */
void freeStack(Stack* stack)
{
	Node* p1;
	Node* p2;
	freeNodeFunc freeNode;
	
	if(!(stack == NULL))
	{
		p1 = stack->_top;
		while(p1)
		{
			p2 = p1;
			p1 = p1->_next;
			freeNode(p2->_data);
			free(p2);
		}
		free(stack);
		stack = NULL;
	}
}

/**
 * @def push 
 * @brief copy given pNode into the stack 
 * @param stack - where the new pNode is copy to
 * @param data - the pNode we need to copy do the stack
 * @param copyNode - the copy function for pNode
 */
void push(Stack* stack, pNode data, copyNodeFunc copyNode)
{
	Node* node = (Node*)malloc(sizeof(Node));
	// check allocation success
	if(node == NULL)
	{
		//allocation failed
	}
	
	node->_data = copyNode(data);
	node->_next = stack->_top;
	stack->_top = node;
}
/**
 * @def pop 
 * @brief pop the head pNode from the given stack 
 * @param stack - the stack that we want to pop it head out
 * @param headData points to the head of the stack
 * @param copyNode - the copy function for pNode
 * @param freeNode - the free function for pNode
 */
int pop(Stack* stack, pNode* headData, copyNodeFunc copyNode, freeNodeFunc freeNode)
{
	if(stack == NULL)
	{
		return 0;
	}
	if(stack->_top == NULL)
	{
		return 0;
	}
	Node *node = stack->_top;

	*headData = copyNode(node->_data);
	stack->_top = node->_next;
	
	freeNode(node->_data);
	free(node);
	
	return 1; 
}

/**
 * @brief getBest This function returns the best valued node in a tree using
 * DFS algorithm.
 * @param head The head of the tree
 * @param getChildren A function that gets a node and a pointer to array of nodes.
 * the function allocates memory for an array of all the children of the node, and
 * returns the number of the children.
 * @param getVal A function that gets a node and returns its value, as int
 * @param freeNode A function that frees node from memory.
 * this function will be called for each Node returns by getChildren.
 * @param best The best available value for a node, when the function encounters
 * a node with that value it stops looking and returns it.
 * If the best value can't be determined, pass UINT_MAX (defined in limits.h)
 * for that param.
 * @param copy A function that do deep copy of Node.
 * @return The best valued node in the tree
 * In case of an error, or when all the nodes in the tree valued zero the returns
 * Node is NULL.
 * If some nodes shares the best valued, the function returns the first one it encounters.
 */
pNode getBest(pNode head, getNodeChildrenFunc getChildren, getNodeValFunc getVal, 
			  freeNodeFunc freeNode, copyNodeFunc copy, unsigned int best)
{
	// check if given tree is NULL 
	if(head == NULL)
	{ 
		printf("no tree is given\n");
		return NULL;
	}
	
	Stack* stack = stackAlloc(sizeof(pNode));
	if(stack == NULL)
	{
		freeNode(head);
		return NULL;
	}
	
	push(stack, head, copy);
	freeNode(head);
	head = NULL;
	
	pNode currentNode;
	pNode* children = NULL;
	int childrenAmount, childIndex;
	unsigned int maxValue = 0, currValue;
	pNode maxNode = NULL;
	
	while(pop(stack, &currentNode, copy, freeNode))
	{ 
		currValue = getVal(currentNode);
		// check if currValue is the best we need to find then we finish
		if(currValue == best)
		{	
			free(children);
			children = NULL;
			freeStack(stack);
			stack = NULL;
			freeNode(maxNode);
			maxNode = copy(currentNode);
			freeNode(currentNode);
			break;
		}
		// if currValue isn't the best but better than the maxValue we find until now
		if(currValue > maxValue)
		{
			freeNode(maxNode);
			maxValue = currValue;
			maxNode = copy(currentNode);
		}
		
		childrenAmount = getChildren(currentNode, &children);
		for(childIndex = childrenAmount - 1 ; childIndex >= 0; childIndex--)
		{
			push(stack, children[childIndex], copy);
			freeNode(children[childIndex]);
		}
		free(children);
		children = NULL;
		freeNode(currentNode);
	}	
	
	currentNode = NULL;
	
	freeStack(stack);
	free(children);
	
	return maxNode;
}
	
				  
				 
	

/**
 * @file sudukutree.h
 * @author guffi
 * 
 * @brief The header file of sudukutree
 */ 
#ifndef SUDUKOTREE_H
#define SUDUKOTREE_H

/**
 * @def struct Suduku
 * @brief each suduku define as struct that includes 3 params: the table size, amount of full slot
 * 		in the table and the table itself.
 */
typedef struct Suduku
{
	int tableSize;
	int fullSlots;
	int* table;
} Suduku;

/**
 * @def struct SlotInex
 * @brief struct that represent the slot index by row & col.
 */
typedef struct SlotIndex
{
	int row;
	int col;
} SlotIndex;

/**
 * @def getNodeChildren
 * @brief get all valid children of each node
 * @param void* suduku -  the suduku node
 * @param void*** optionalSolutions - pointer to array of children
 * @return the number of node children
 */
int getNodeChildren(void* suduku, void*** optionalSolutions);

/**
 * @def getNodeVal
 * @brief get the value of given suduku table - it full slot amount.
 * @param void* suduku- the suduku Node
 * @return the number of full slot in given table
 */
unsigned int getNodeVal(void* suduku);

/**
 * @def freeNode
 * @brief free from memory the suduku Node
 * @param void* suduku- the suduku Node
 */	
void freeNode(void* suduku);

/**
 * @def copyNode
 * @brief A function that do deep copy of Node.
 * @param suduku the node need to be copy
 * @return the coppied node
 */
void* copyNode(void* suduku);

/**
 * @def findSqrt
 * @brief check if the given size has sqrt or not
 * @param int size
 * @return the sqrt if exist otherwise invalid command 
 */	
int findSqrt(const int size);

/**
 * @def createSudukuTable
 * @brief creates the suduku table by the given size
 * @param int tableSize - suduku table size
 */
void* createSudukuTable(const int tableSize);

/**
 * @def check row
 * @brief check for validation in given slot row
 * @param Suduku* suduku - the suduku node with the table
 * @param SlotIndex* index - the index of the slot that need to check validation for
 * @param int* possibleValues - the array that represent the possible values 
 */
void checkRow(Suduku* suduku, SlotIndex* index, int* PossibleValues);

/**
 * @def check col
 * @brief check for validation in given slot col
 * @param Suduku* suduku - the suduku node with the table
 * @param SlotIndex* index - the index of the slot that need to check validation for
 * @param int* possibleValues - the array that represent the possible values 
 */
void checkCol(Suduku* suduku, SlotIndex* index, int* PossibleValues);

/**
 * @def check sub Squre
 * @brief check for validation in given slot sub squre
 * @param Suduku* suduku - the suduku node with the table
 * @param SlotIndex* index - the index of the slot that need to check validation for
 * @param int* possibleValues - the array that represent the possible values 
 */
void checksubSqure(Suduku* suduku, SlotIndex* index, int* PossibleValues);

#endif

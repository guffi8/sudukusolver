/**
 * @file sudukutree.c
 * @author guffi
 * 
 * @brief This file implement the sudukutree.h function. 
 */
//-------------------------------------include------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include "sudukutree.h"

//------------------------------------const definitions---------------------------------------------
/**
 * @def TABLE(N, row, col) ((N * row) + col)
 * @brief How to access the cell structure of the table
 */
#define TABLE(N, row, col) ((N * row) + col) 
/**
 * @def DEFAULT_ROW_INDEX -1
 * @brief default row index 
 */
#define DEFAULT_ROW_INDEX -1
/**
 * @def DEFAULT_COL_INDEX -1
 * @brief default col index 
 */
#define DEFAULT_COL_INDEX -1
/**
 * @def  EMPTY_SLOT_VAL 0
 * @brief  the val for empty slot in the table - used for initialize
 */
#define EMPTY_SLOT_VAL 0
/**
 * @def POSSIBLE_VALUE_INITIALIZE 0 
 * @brief initialize value for possiable values array 
 */
#define POSSIBLE_VALUE_INITIALIZE 0 
/**
 * @def MAX_SQRT_VAL 10
 * @brief max possible sqrt value
 */
#define MAX_SQRT_VAL 10

#define TRUE 1
#define FALSE 0
/**
 * @def INVALID_COMMAND -1
 * @brief value for all invalid command
 */
#define INVALID_COMMAND -1

//-------------------------------------------functions----------------------------------------------


/**
 * @overload sudukutree.h
 */	
int findSqrt(const int size)
{
	
	if(size == 0)
	{
		return INVALID_COMMAND;
	}
	
	int sqrt;
	for(sqrt = 1; sqrt <= MAX_SQRT_VAL; sqrt++)
	{
		if(sqrt * sqrt == size)
		{
			return sqrt;
		}
	}
	
	return INVALID_COMMAND; 
}

/**
 * @def firstEmptySlot
 * @brief finds the first empty slot in the table
 * @param Suduku* suduku the table
 * @param SlotIndex* slotIndex - the index that will update by the first empty slot index
 */
void firstEmptySlot(Suduku* suduku, SlotIndex* slotIndex)
{
	int rowIndex, colIndex;
	
	for(rowIndex = 0; rowIndex < suduku->tableSize; rowIndex++)
	{ 		
		 for(colIndex = 0; colIndex < suduku->tableSize; colIndex++)
		 {
			 if(suduku->table[TABLE(suduku->tableSize, rowIndex, colIndex)] == 0)
			 {
				 slotIndex->row = rowIndex;
				 slotIndex->col = colIndex;
				 return;
			 }
		 }
	}
	return;
}
	
/**
 * @overload sudukutree.h
 */	
void* createSudukuTable(const int tableSize)
{
	Suduku* suduku = (Suduku*)malloc(sizeof(Suduku));
	
	if(suduku == NULL)
	{
		//allocation failed
		printf("allocation failed\n");
		return NULL;
	}
	
	suduku-> tableSize = tableSize;
	suduku-> table = (int*)malloc(tableSize * tableSize * sizeof(int));
	if(suduku-> table == NULL)
	{
		//allocation failed
		printf("allocation failed\n");
		free(suduku);
		return NULL;
	}
	
	suduku-> fullSlots = 0;
	
	return suduku;
}

/**
 * @overload sudukutree.h
 */	
void freeNode(void* suduku)
{
	if(suduku != NULL)
	{
		Suduku* temp_table = (Suduku*)suduku;
		if(temp_table != NULL)
		{
			free(temp_table->table);
			temp_table->table = NULL;
			free(suduku);
			suduku = NULL;
		}
	}
}

/**
 * @overload sudukutree.h
 */
unsigned int getNodeVal(void* suduku)
{
	Suduku* sudukuTable = (Suduku*)suduku;
	return sudukuTable->fullSlots;
}

/**
 * @overload sudukutree.h
 */
void* copyNode(void* suduku)
{
	Suduku* sudukuTempTable = (Suduku*)suduku;
	
	Suduku* sudukuNewTable = (Suduku*)createSudukuTable(sudukuTempTable->tableSize);
	if(sudukuNewTable == NULL)
	{
		//allocation faild
		return NULL;
	}
	
	sudukuNewTable->fullSlots = sudukuTempTable-> fullSlots;
	
	int tableIndex;
	
	for(tableIndex = 0 ; tableIndex < (sudukuNewTable->tableSize)*(sudukuNewTable->tableSize);\
		tableIndex++)
	{
		(sudukuNewTable->table)[tableIndex] = (sudukuTempTable->table)[tableIndex];
	}
	
	return sudukuNewTable;
}

/**
 * @overload sudukutree.h
 */
void checkRow(Suduku* suduku, SlotIndex* index, int* possibleValues)
{
	int i;
	int slotVal;
	for(i = 0; i<suduku->tableSize; i++)
	{
		slotVal = suduku->table[TABLE(suduku->tableSize, index->row, i)];
		if(slotVal != EMPTY_SLOT_VAL)
		{
			possibleValues[slotVal - 1]++;
		}
	}
}

/**
 * @overload sudukutree.h
 */
void checkCol(Suduku* suduku, SlotIndex* index, int* possibleValues)
{
	int i;
	int slotVal;
	
	for(i = 0; i<suduku->tableSize; i++)
	{
		slotVal = suduku->table[TABLE(suduku->tableSize, i, index->col)];
		if(slotVal != EMPTY_SLOT_VAL)
		{
			possibleValues[slotVal - 1]++;
		}
	}
}

/**
 * @overload sudukutree.h
 */
void checksubSqure(Suduku* suduku, SlotIndex* index, int* possibleValues)
{
	// we want to find the root of the sub string to check for PossibleValues
	int sqrt = findSqrt(suduku->tableSize); 
	
	int subSqureRow = (int)(index->row / sqrt)*(sqrt);
	int subSqureCol = (int)(index->col / sqrt)*(sqrt);
	
	int col, row, slotVal;
	for(row = subSqureRow; row < subSqureRow + sqrt; row++)
	{
		for(col = subSqureCol; col < subSqureCol + sqrt; col++)
		{
			slotVal = suduku->table[TABLE(suduku->tableSize, row, col)];
			if(slotVal != EMPTY_SLOT_VAL)
			{
				possibleValues[slotVal - 1]++;
			}
		}
	}
}

/**
 * @overload sudukutree.h
 */
int getNodeChildren(void* suduku, void*** optionalSolutions)
{
	
	Suduku* tempSuduku = (Suduku*)suduku;
	SlotIndex emptySlotIndex = {DEFAULT_ROW_INDEX, DEFAULT_COL_INDEX};
	firstEmptySlot(tempSuduku, &emptySlotIndex);
	
	//reach to the end of the table
	if(emptySlotIndex.col == DEFAULT_COL_INDEX || emptySlotIndex.row == DEFAULT_ROW_INDEX)
	{
		return 0;
	}
	
	int* possibleValues = (int*)malloc(tempSuduku->tableSize * sizeof(int));
	if(possibleValues == NULL)
	{
		//allocation faild
		return 0;
	}
	int index;
	//initalize possibleValues array 
	for(index = 0; index < tempSuduku->tableSize; index++)
	{
		*(possibleValues + index) = POSSIBLE_VALUE_INITIALIZE;
	}
	//check for valid possible Values 
	checkCol(suduku, &emptySlotIndex, possibleValues);
	checkRow(suduku, &emptySlotIndex, possibleValues);
	checksubSqure(suduku, &emptySlotIndex, possibleValues);
	
	//check how mach possible Values the are
	int childrenCounter = 0;
	for(index = 0; index < tempSuduku->tableSize; index++)
	{
		if(*(possibleValues + index) == POSSIBLE_VALUE_INITIALIZE)
		{
			childrenCounter++;	
		}
	}
	
	//create an array of children - each children is the suduku table with different possible value
	Suduku** childrenArray;
	
	childrenArray = (Suduku**)malloc(childrenCounter * sizeof(Suduku*));
	//need to check for faild allocation
	if(childrenArray == NULL)
	{
		//allocation faild
		return 0;
	}
	
	*optionalSolutions = (void**)childrenArray;
	
	Suduku* child = NULL;
	
	for(index = 0; index < tempSuduku->tableSize; index++)
	{
		//if current value is optional
		if(*(possibleValues + index) == POSSIBLE_VALUE_INITIALIZE)
		{
			child = (Suduku*)copyNode(tempSuduku);
			child->table[TABLE(tempSuduku->tableSize, emptySlotIndex.row, emptySlotIndex.col)] = \
			index + 1;
			child->fullSlots++;
			*childrenArray = child;
			childrenArray++;
			
		}	
	}
	
	free(possibleValues);
	possibleValues = NULL;
	
	return childrenCounter;	
}


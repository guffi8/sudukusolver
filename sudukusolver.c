/**
 * @file sudukusolver.c
 * @author guffi
 * 
 * @brief The main of the suduku solver program.
 * 
 */
//-------------------------------------include------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>
#include "genericdfs.h"
#include "sudukutree.h"

//------------------------------------const definitions---------------------------------------------
/**
 * @def VALID_RECEIEVED_ARGS_NUM 2
 * @brief the valid number of args in the command
 */
#define VALID_RECEIEVED_ARGS_NUM 2
/**
 * @def FILE_NAME 1
 * @brief the file name location in the givan argv[]
 */
#define FILE_NAME 1
/**
 * @def TABLE_SIZE_FORMAT "%d\n"
 * @brief the table size line format in the given file 
 */
#define TABLE_SIZE_FORMAT "%d\n"
/**
 * @def INVALID_COMMAND -1
 * @brief value for all invalid command
 */
#define INVALID_COMMAND -1
/**
 * @def POSSIBLE_VALUE_INITIALIZE 0 
 * @brief initialize value for possiable values array 
 */
#define POSSIBLE_VALUE_INITIALIZE 0 
/**
 * @def TABLE(N, row, col) ((N * row) + col)
 * @brief How to access the cell structure of the table
 */
#define TABLE(N, row, col) ((N * row) + col)

#define TRUE 1
#define FALSE 0
#define NEW_LINE '\n'
#define SPACE ' ' 

 
//errors print
#define USAGE_ERROR "usage: sudukusolver <filename>\n"
#define FILE_SUPPLY "please supply a file!\n"
#define FILE_NOT_FOUND "FILE_IS_NOT_FOUND\n"
#define NOT_VALID_FILE "%s: not a valid suduku file\n"
#define NO_SOLUTION "no solution!\n"
//-------------------------------------------functions----------------------------------------------
/**
 * @def solutionPrinter
 * @brief print the suduku solution table
 * @param Suduku* sudukuTable - the suduku table
 */
void solutionPrinter(const Suduku* sudukuTable)
{
	int colIndex, rowIndex;
	printf("%d\n", sudukuTable->tableSize);
	for(rowIndex = 0; rowIndex < sudukuTable->tableSize; rowIndex++)
	{
		for(colIndex = 0; colIndex < sudukuTable->tableSize; colIndex++)
		{
			printf("%d", sudukuTable->table[TABLE(sudukuTable->tableSize, rowIndex, colIndex)]);
			
			if(colIndex == sudukuTable->tableSize - 1)
			{
				printf("%c", NEW_LINE);
			}
			else
			{
				printf("%c", SPACE);
			}
		}
	} 
}
/**
 * @def parser
 * @brief parser the given file
 * @param the file name
 */
void* parser(const char* fileName)
{
	FILE* currentFile = fopen(fileName, "r");   	
	//check if file exist
	if(currentFile == NULL)
	{
		printf(FILE_NOT_FOUND);
		return NULL;
	}
	int tableSize;
	
	if(fscanf(currentFile, TABLE_SIZE_FORMAT, &tableSize) != 1 || findSqrt(tableSize) == \
	   INVALID_COMMAND)
	{ 
		fclose(currentFile);
		printf(NOT_VALID_FILE, fileName);
		return NULL;
	}
	
	// create suduko table
	Suduku* sudukuTable = createSudukuTable(tableSize);
	
	int tableIndex = 0;
	int val;
	char nextChar;
	
	while(fscanf(currentFile, "%d", &val) == 1)
	{	
		// check valid structre of the file
		nextChar = fgetc(currentFile);
	
		if((tableIndex + 1) % tableSize == 0)
		{
			if(nextChar != NEW_LINE && nextChar != EOF)
			{
				fclose(currentFile);
				freeNode(sudukuTable);
				printf(NOT_VALID_FILE, fileName);
				return NULL;
			}
		}
		else
		{
			if(nextChar != SPACE)
			{
				fclose(currentFile);
				freeNode(sudukuTable);
				printf(NOT_VALID_FILE, fileName);
				return NULL;
			}
		}
		
		if(val < 0 || val > tableSize)
		{	
			fclose(currentFile);
			freeNode(sudukuTable);
			printf(NOT_VALID_FILE, fileName);
			return NULL;
		}
		if(val != 0)
		{
			sudukuTable->fullSlots ++;
		}
		
		sudukuTable->table[tableIndex] = val;
		tableIndex++;
	
	}
	
	if(tableIndex != tableSize*tableSize)
	{
		fclose(currentFile);
		freeNode(sudukuTable);
		printf(NOT_VALID_FILE, fileName);
		return NULL;
	}
	
	fclose(currentFile);
	return sudukuTable;		

}
/**
 * @def checkReapets
 * @brief check for valid number of reaoetes in possiable value - at most one 
 * @param Suduku* sudukuTable - the suduku table
 * @param int* possibleValues - array that represent the amount of reapets of each value
 * @return False if there is to much repetition otherwise True
 */
int checkReapets(Suduku* sudukuTable, int* possibleValues)
{
	int index;
	for(index = 0; index < sudukuTable->tableSize; index++)
	{	
		// Number of repetitions for each value is at most one
		if(*(possibleValues + index) > 1)
		{
			free(possibleValues);
			return FALSE;
		}
					//intialize the array for the next loop
		*(possibleValues + index) = POSSIBLE_VALUE_INITIALIZE;
	}	
	return TRUE;
}
/**
 * @def checkValidation
 * @brief check that the given table is valid 
 * @param Suduku* sudukuTable the suduku table 
 * @return True if given valid otherwise False
 */
int checkValidation(Suduku* sudukuTable)
{
	int* possibleValues = (int*)malloc(sudukuTable->tableSize * sizeof(int));
	if(possibleValues == NULL)
	{
		//allocation faild
		return 0;
	}
	
	int index;
	//initalize possibleValues array 
	for(index = 0; index < sudukuTable->tableSize; index++)
	{
		*(possibleValues + index) = POSSIBLE_VALUE_INITIALIZE;
	}
	
	SlotIndex currSlot;
	
	//valid Sudoku table will contain no more than one organ in each row, column or sub-square, 
	//so we examine the diagonal and in this way we cover all the possibilities.
	for(index = 0; index < (sudukuTable->tableSize); index++)
	{
		currSlot.row = index;
		currSlot.col = index;
		//check for valid possible Values 
		checkCol(sudukuTable, &currSlot, possibleValues);
		if(!(checkReapets(sudukuTable, possibleValues)))
		{
			return FALSE;
		}
		
		checkRow(sudukuTable, &currSlot, possibleValues);
		if(!(checkReapets(sudukuTable, possibleValues)))
		{
			return FALSE;
		}
		checksubSqure(sudukuTable, &currSlot, possibleValues);
		if(!(checkReapets(sudukuTable, possibleValues)))
		{
			return FALSE;
		}
	}
	free(possibleValues);
	return TRUE;
}


int main(int argc, char* argv[])
{
	
	if(argc != VALID_RECEIEVED_ARGS_NUM)
	{
		printf(FILE_SUPPLY);
		printf(USAGE_ERROR);
		return INVALID_COMMAND;
	}
	
	Suduku* sudukuTable = parser(argv[FILE_NAME]);
	//check that given suduku table is valid and exsist!
	if(sudukuTable == NULL)
	{
		return 0;
	}
	
	// validation check for the given table - no illegal repetitions
	if(!checkValidation(sudukuTable))
	{
		printf(NOT_VALID_FILE, argv[FILE_NAME]);
		return 0;
	}

	// the value function return the number of full slot in the table we want to reach the first 
	// full table in the tree so the best val will be equal to the (table size)*(table size)
	unsigned int bestVal = (sudukuTable->tableSize * sudukuTable->tableSize);	
	sudukuTable = getBest(sudukuTable, getNodeChildren, getNodeVal, freeNode, copyNode, bestVal);
	
	//check that best suduku value is equal to best val if true -> perfect match ~ suduku solved! 
	//otherwise suduku has no solution
	
	if(sudukuTable == NULL || sudukuTable->fullSlots != sudukuTable->tableSize * \
	   sudukuTable->tableSize)
	{
		printf(NO_SOLUTION);
		freeNode(sudukuTable);
		return 0;
	}
	
	solutionPrinter(sudukuTable);
	freeNode(sudukuTable);
	return 0;
}




	
	
	

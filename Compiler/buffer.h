/*************************************************************
* File name: buffer.c
* Compiler: MS Visual Studio 2019
* Author: Hamza Eliraqy
* ID : 040976448
* Course: CST 8152 – Compilers, Lab Section: 011
* Assignment: A1.
* Date: Feb 20 2021
* Professor: Paulo Sousa / Abdulah
*/

// TODO101: Adjust file header

#ifndef BUFFER_H_
#define BUFFER_H_

/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* constant definitions */
#define RT_FAIL_1 (-1)			// TODO102
#define RT_FAIL_2 (-2)			// TODO103
#define LOAD_FAIL (-2)			// TODO104

#define DEFAULT_SIZE 200        // TODO105
#define DEFAULT_INCREMENT 15   // TODO106

/* You should add your own constant definitions here */
#define MAX_SIZE (SHRT_MAX-1)   /* maximum capacity*/ 

/* Buffer Modes */
#define FIXMODE 0         // TODO107
#define ADDMODE 1         // TODO108
#define MULMODE (-1)         // TODO109

/* Add your bit-masks constant definitions here */
#define DEFAULT_FLAGS 0x3FFF 	// 0011.1111 1111.1111
#define SET_EOB 0x8000          // TODO110  1000.0000.0000.0000
#define RESET_EOB 0x7FFF        // TODO111  0111.1111.1111.1111
#define CHECK_EOB 0x8000        // TODO112  1000.0000.0000.0000 // 1100.1111.1111.1111
#define SET_R_FLAG 0x4000       // TODO113	0100.0000.0000.0000
#define RESET_R_FLAG 0xBFFF     // TODO114	1011.1111.1111.1111
#define CHECK_R_FLAG 0x4000     // TODO115	0100.0000.0000.0000

/* Constants used in buffer */
#define MAXINCREMENT 100 	// TODO116
#define MAX_VALUE (SHRT_MAX-1) 		// TODO117
#define RT_INC_FAIL 0x100		// TODO118

/* user data type declarations */
typedef struct Buffer {
	char* content;         /* pointer to the beginning of character array (character buffer) */
	short size;            /* current dynamic memory size (in bytes) allocated to  buffer */
	char  increment;       /* character array increment factor */
	char  mode;            /* operational mode indicator*/
	short addCOffset;      /* the offset (in chars) to the add-character location */
	short getCOffset;      /* the offset (in chars) to the get-character location */
	short markOffset;      /* the offset (in chars) to the mark location */
	unsigned short flags;  /* contains character array reallocation and end-of-buffer flag */
} bStructure, * bPointer;

/* Function declarations */
// TODO119
bPointer bCreate(short size, char increment, char mode);
bPointer bAddCh(bPointer const pBuffer, char ch);
int bClean(bPointer const pBuffer);
int bFree(bPointer const pBuffer);
int bIsFull(bPointer const pBuffer);
short bGetAddChOffset(bPointer const pBuffer);
short bGetSize(bPointer const pBuffer);
int bGetMode(bPointer const pBuffer);
short bGetMarkOffset(bPointer const pBuffer);
short bSetMarkOffset(bPointer const pBuffer, short mark);
bPointer bFinish(bPointer const pBuffer, char ch);
int bDisplay(bPointer const pBuffer, char nl);
int bLoad(bPointer const pBuffer, FILE* const fi);
int bIsEmpty(bPointer const pBuffer);
char bGetCh(bPointer const pBuffer);
int bRewind(bPointer const pBuffer);
bPointer bRetract(bPointer const pBuffer);
short bRestore(bPointer const pBuffer);
short bGetChOffset(bPointer const pBuffer);
size_t bGetIncrement(bPointer const pBuffer);
char* bGetContent(bPointer const pBuffer, short chPosition);
short bufferAddCPosition(bStructure* const pBuffer);
unsigned short bGetFlags(bPointer const pBuffer);
//New definitions
int bLoad(bPointer const, FILE* const);
bPointer bRetract(bPointer const);
short bSetMarkOffset(bPointer const, short);

#endif

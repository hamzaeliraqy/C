/**************************************************************
* File name: buffer.c
* Compiler: MS Visual Studio 2019
* Author: Hamza Eliraqy
* Function list: bCreate, bAddCh, bClean, bFree, bIsFull, bGetAddChOffset, bGetSize, bGetMode, bGetMarkOffset
bSetMarkOffset, bFinish, bDisplay, bLoad, bIsEmpty, bGetCh, bRewind, bRetract, bRestore, bGetChOffset, bGetIncrement, bGetCotent, bufferAddCPosition, bGetFlags.
*************************************************************/


#include "buffer.h"




/************************************************************
*	Function name: bCreate
*	Purpose: allocate memory for buffer structure
*	Author:	Hamza Eliraqy
*	History/Versions: Feb 20
*	Called functions:
*	Parameters: short, char, char
*	Return Value: bPointer
*	Algorithm: checks size and adjust increment, size based on mode, pass the size bPointer
**************************************************************/
bPointer bCreate(short size, char increment, char mode) {

	

	bPointer b;
	char bMode;
	if (size<0 || size>MAX_SIZE)
		return NULL;
	if (!size) {
		size = DEFAULT_SIZE;
		increment = DEFAULT_INCREMENT;
	}
	if (!increment)
		mode = 'f';
	switch (mode) {
	case 'a':
		bMode = ADDMODE;
		break;
	case 'f':
		bMode = FIXMODE;
		increment = 0;
		break;
	case 'm':
		if ((unsigned char)increment > MAXINCREMENT)
			return NULL;
		bMode = MULMODE;
		break;
	default:
		return NULL;
	}
	b = (bPointer)calloc(1, sizeof(bStructure));
	if (!b)
		return NULL;
	b->content = (char*)malloc(size);
	if (!b->content) {
		free(b);
		return NULL;
	}
	b->mode = bMode;
	b->size = size;
	b->increment = increment;
	b->flags = DEFAULT_FLAGS;

	return b;
}


/************************************************************
*	Function name: bAddCh
*	Purpose: add the character to the buffer
*	Author:	Hamza Eliraqy
*	History/Versions: Feb 20
*	Called functions: bLoad
*	Parameters: bPointer, char
*	Return Value: bPointer
*	Algorithm: checks if the buffer is full then allocate new space based on the mode and adds the character to content
**************************************************************/
bPointer bAddCh(bPointer const pBuffer, char ch) {

	char* tempbuf;
	short availSpace, newSize, newInc;

	if (!pBuffer) {
		return NULL;
	}
	pBuffer->flags = pBuffer->flags & RESET_R_FLAG;
	if (pBuffer->addCOffset * sizeof(char) == MAX_SIZE ||
		pBuffer->addCOffset * sizeof(char) == SHRT_MAX)
		return NULL;
	if (pBuffer->size == pBuffer->addCOffset * (int)sizeof(char)) {
		switch (pBuffer->mode) {
		case ADDMODE:
			newSize = pBuffer->size + (unsigned char)pBuffer->increment;
			if (newSize > 0)
				if (newSize == SHRT_MAX)
					newSize = MAX_SIZE;
			break;
		case FIXMODE:
			return NULL;
		case MULMODE:
			availSpace = MAX_SIZE - pBuffer->size;
			newInc = (short int)(availSpace * (pBuffer->increment / 100.0F));
			newSize = pBuffer->size + newInc;
			if (newSize <= pBuffer->size && pBuffer->size < MAX_SIZE)
				newSize = MAX_SIZE;
			break;
		default:
			return NULL;
		}
		tempbuf = (char*)realloc(pBuffer->content, newSize);
		if (!tempbuf)
			return NULL;
		if (tempbuf != pBuffer->content) {
			pBuffer->flags = pBuffer->flags | SET_R_FLAG;
			pBuffer->content = tempbuf;
		}
		pBuffer->size = newSize;
	}
	pBuffer->content[pBuffer->addCOffset++] = ch;
	
	return pBuffer;
}

/************************************************************
*   Function name: bClean
*	Purpose: re-intiliaze data memebers of the buffer structure
*	Author:	Hamza Eliraqy
*	History/Versions: Feb 20
*	Called functions:
*	Parameters:	bPointer
*	Return Value: int
*	Algorithm:
**************************************************************/
int bClean(bPointer const pBuffer) {

	if (!pBuffer)
		return RT_FAIL_1;
	pBuffer->addCOffset = pBuffer->markOffset = pBuffer->getCOffset = 0;
	pBuffer->flags = pBuffer->flags & RESET_EOB;
	pBuffer->flags = pBuffer->flags & RESET_R_FLAG;

	return 0;
}

/************************************************************
*   Function name: bFree
*	Purpose: free the content buffer
*	Author:	Hamza Eliraqy
*	History/Versions: Feb 20
*	Called functions:
*	Parameters: bPointer
*	Return Value: int
*	Algorithm:
**************************************************************/
int bFree(bPointer const pBuffer) {

	if (pBuffer) {
		free(pBuffer->content);
		free(pBuffer);
		return 1;
	}
	return 0;
}

/************************************************************
*   Function name: bIsFull
*	Purpose: checks if the buffer is full
*	Author: Hamza Eliraqy
*	History/Versions: Feb 20
*	Called functions: bAddCh
*	Parameters: bPointer
*	Return Value: int
*	Algorithm: compare content and size
**************************************************************/
int bIsFull(bPointer const pBuffer) {

	if (!pBuffer)
		return RT_FAIL_1;
	return (pBuffer->size == pBuffer->addCOffset * (int)sizeof(char));
}

/************************************************************
*   Function name: bGetAddChOffset
*	Purpose: return current addCOffset
*	Author: Hamza Eliraqy
*	History/Versions: Feb 20
*	Called functions:
*	Parameters: bPointer
*	Return Value: short
*	Algorithm:r
**************************************************************/
short bGetAddChOffset(bPointer const pBuffer) {


	if (!pBuffer) {
		return RT_FAIL_1;
	}

	return pBuffer->addCOffset;
}

/************************************************************
*   Function name: bGetSize
*	Purpose: return buffer size
*	Author: Hamza Eliraqy
*	History/Versions: Feb 20
*	Called functions:
*	Parameters: bPointer
*	Return Value: short
*	Algorithm:
**************************************************************/
short bGetSize(bPointer const pBuffer) {


	if (!pBuffer) {
		return RT_FAIL_1;
	}

	return pBuffer->size;
}

/************************************************************
*   Function name: bGetMode
*	Purpose: return buffer mode
*	Author: Hamza Eliraqy
*	History/Versions: Feb 20
*	Called functions:
*	Parameters: bPointer
*	Return Value: int
*	Algorithm:
**************************************************************/
int bGetMode(bPointer const pBuffer) {

	if (!pBuffer) {
		return RT_FAIL_2;
	}

	return pBuffer->mode;
}

/************************************************************
*   Function name: bGetMarkOffset
*	Purpose: return getCOffset
*	Author: Hamza Eliraqy
*	History/Versions: Feb 20
*	Called functions:
*	Parameters: bPointer
*	Return Value: short
*	Algorithm:
**************************************************************/
short bGetMarkOffset(bPointer const pBuffer) {

	if (!pBuffer) {
		return RT_FAIL_1;
	}

	return pBuffer->getCOffset;
}

/************************************************************
*   Function name: bSetMarkOffset
*	Purpose: return markOffset
*	Author: Hamza Eliraqy
*	History/Versions: Feb 20
*	Called functions:
*	Parameters: bPointer, short
*	Return Value: short
*	Algorithm:
**************************************************************/
short bSetMarkOffset(bPointer const pBuffer, short mark) {


	if (!pBuffer || mark<0 || mark > pBuffer->addCOffset)
		return RT_FAIL_1;
	pBuffer->markOffset = mark;
	return 0;
}

/************************************************************
*   Function name: bFinish
*	Purpose: adjust the buffer size
*	Author: Hamza Eliraqy
*	History/Versions: Feb 20
*	Called functions:
*	Parameters: bPointer, char
*	Return Value: bPointer
*	Algorithm: add char to the end of content
**************************************************************/
bPointer bFinish(bPointer const pBuffer, char ch) {

	char* tempbuf;
	short size;
	if (!pBuffer || pBuffer->addCOffset * sizeof(char) == SHRT_MAX)
		return NULL;
	pBuffer->flags = pBuffer->flags & RESET_R_FLAG;
	if (pBuffer->size - pBuffer->addCOffset * sizeof(char) == 1) {
		pBuffer->content[pBuffer->addCOffset++] = ch;
		return pBuffer;
	}
	size = (pBuffer->addCOffset + 1) * sizeof(char);
	tempbuf = (char*)realloc(pBuffer->content, size);
	if (!tempbuf)
		return NULL;
	if (pBuffer->content != tempbuf) {
		pBuffer->flags = pBuffer->flags | SET_R_FLAG;
		pBuffer->content = tempbuf;
	}
	pBuffer->size = size;
	pBuffer->content[pBuffer->addCOffset++] = ch;
	return pBuffer;
}

/************************************************************
*   Function name: bDisplay
*	Purpose: print content of buffer
*	Author: Hamza Eliraqy
*	History/Versions: Feb 20
*	Called functions:
*	Parameters: bPointer, char
*	Return Value: int
*	Algorithm: loop through buffer, checks if char is not empty and prints it, returns number of printed char
**************************************************************/
int bDisplay(bPointer const pBuffer, char nl) {

	int cont = 0;
	char c;
	if (!pBuffer || !(pBuffer->content))
		return RT_FAIL_1;
	c = bGetCh(pBuffer);
	while (!(pBuffer->flags & CHECK_EOB)) {
		cont++;
		printf("%c", c);
		c = bGetCh(pBuffer);
	}

	if (nl)
		printf("\n");
	return cont;
}

/************************************************************
*   Function name: bLoad
*	Purpose: reads input file
*	Author: Hamza Eliraqy
*	History/Versions: Feb 20
*	Called functions:
*	Parameters: bPointer, FILE*
*	Return Value: int
*	Algorithm: loops and gets char if char is NULL it unget it, return number of characters
**************************************************************/
int bLoad(bPointer const pBuffer, FILE* const fi) {

	int size = 0;
	char c;
	if (!fi || !pBuffer)
		return RT_FAIL_1;
	c = (char)fgetc(fi);
	while (!feof(fi)) {
		if (!bAddCh(pBuffer, c)) {
			ungetc(c, fi);
			return LOAD_FAIL;
		}
		c = (char)fgetc(fi);
		size++;
	}
	if (ferror(fi))
		return RT_FAIL_1;
	return size;
}

/************************************************************
*   Function name: bIsEmpty
*	Purpose: checks addCOffset
*	Author: Hamza Eliraqy
*	History/Versions: Feb 20
*	Called functions:
*	Parameters: bPointer
*	Return Value: int
*	Algorithm:
**************************************************************/
int bIsEmpty(bPointer const pBuffer) {

	if (!pBuffer)
		return RT_FAIL_1;
	return (pBuffer->addCOffset == 0);
}


/************************************************************
*   Function name: bGetCh
*	Purpose: reads the buffer
*	Author: Hamza Eliraqy
*	History/Versions: Feb 20
*	Called functions: bDisplay
*	Parameters: bPointer
*	Return Value: char
*	Algorithm: compares getCOffset and addCOffset and changes flags
**************************************************************/
char bGetCh(bPointer const pBuffer) {

	if (!pBuffer)
		return RT_FAIL_1;
	if (pBuffer->getCOffset == pBuffer->addCOffset) {
		pBuffer->flags = pBuffer->flags | SET_EOB;
		return '\0';
	}
	pBuffer->flags = pBuffer->flags & RESET_EOB;
	return pBuffer->content[pBuffer->getCOffset++];
}


/************************************************************
*   Function name: bRewind
*	Purpose: set getCOffest and markOffset to 0
*	Author: Hamza Eliraqy
*	History/Versions: Feb 20
*	Called functions:
*	Parameters: bPointer
*	Return Value: int
*	Algorithm:
**************************************************************/
int bRewind(bPointer const pBuffer) {

	if (!pBuffer) {
		return RT_FAIL_1;
	}
	pBuffer->getCOffset = 0;
	pBuffer->markOffset = 0;
	return 0;
}


/************************************************************
*   Function name: bRetract
*	Purpose: decrements getCOffset
*	Author: Hamza Eliraqy
*	History/Versions: Feb 20
*	Called functions:
*	Parameters: bPointer
*	Return Value: bPointer
*	Algorithm:
**************************************************************/
bPointer bRetract(bPointer const pBuffer) {

	if (!pBuffer || pBuffer->getCOffset == 0) {
		return NULL;
	}
	pBuffer->getCOffset--;
	return pBuffer;
}

/************************************************************
*   Function name: bRestore
*	Purpose: set value of getCOffset to markOffset
*	Author: Hamza Eliraqy
*	History/Versions: Feb 20
*	Called functions:
*	Parameters: bPointer
*	Return Value: short
*	Algorithm:
**************************************************************/
short bRestore(bPointer const pBuffer) {

	if (!pBuffer) {
		return -1;
	}
	pBuffer->getCOffset = pBuffer->markOffset;
	return pBuffer->getCOffset;
}

/************************************************************
*   Function name: bGetChOffset
*	Purpose: return getCOffset
*	Author: Hamza Eliraqy
*	History/Versions: Feb 20
*	Called functions:
*	Parameters: bPointer
*	Return Value: short
*	Algorithm:
**************************************************************/
short bGetChOffset(bPointer const pBuffer) {

	if (!pBuffer) {
		return RT_FAIL_1;
	}
	return  pBuffer->getCOffset;
}


/************************************************************
*   Function name: bGetIncrement
*	Purpose: return DEFAULT_INCREMENT
*	Author: Hamza Eliraqy
*	History/Versions: Feb 20
*	Called functions: bAddCh
*	Parameters: bPointer
*	Return Value: size_t
*	Algorithm:
**************************************************************/
size_t bGetIncrement(bPointer const pBuffer) {

	if (!pBuffer) {
		return RT_INC_FAIL;
	}

	return (unsigned char)pBuffer->increment;
}


/************************************************************
*   Function name: bGetContent
*	Purpose: return pointer to location of character buffer
*	Author: Hamza Eliraqy
*	History/Versions: Feb 20
*	Called functions:
*	Parameters: bPointer, short
*	Return Value: char*
*	Algorithm:
**************************************************************/
char* bGetContent(bPointer const pBuffer, short chPosition) {


	if (!pBuffer || chPosition < 0 || chPosition > pBuffer->addCOffset)
		return NULL;
	return pBuffer->content + chPosition;
}


/************************************************************
*   Function name: bufferAddCPositon
*	Purpose: return getCOffset
*	Author: Hamza Eliraqy
*	History/Versions: Feb 20
*	Called functions:
*	Parameters:
*	Return Value: short
*	Algorithm:
**************************************************************/
short bufferAddCPosition(bStructure* const pBuffer) {

	if (!pBuffer) {
		return RT_FAIL_1;
	}

	return pBuffer->getCOffset;
}


/************************************************************
*   Function name: bGetFlags
*	Purpose: return flag from buffer
*	Author: Hamza Eliraqy
*	History/Versions: Feb 20
*	Called functions:
*	Parameters: bPointer
*	Return Value: unsigned short
*	Algorithm:
**************************************************************/
#define FLAGS_
#undef FLAGS_
#ifndef FLAGS_
unsigned short bGetFlags(bPointer const pBuffer) {

	if (!pBuffer) {
		return (unsigned short)RT_FAIL_1;
	}

	return  pBuffer->flags;
}
#else
#define bGetFlags(pBuffer) ((pBuffer) ? (pBuffer->flags) : (RT_FAIL_1))
#endif

/**************************************************************
* File name: evalParser.c
* Compiler: MS Visual Studio 2019
* Author: Hamza Eliraqy
*************************************************************/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h> /* Constants for calls to exit()*/

#include <string.h>
#include <stdarg.h>

#include "buffer.h"
#include "token.h"

  /* Input buffer parameters */
#define INIT_SIZE 200 /* initial buffer capacity */
#define INIT_INCREMENT 15       /* increment factor */
/* String Literal Table parameters */
#define STR_INIT_SIZE 100 /* initial string literal table capacity */
#define STR_INCREMENT  50   /* initial string literal table capacity inc */

/*check for ANSI C compliancy */
#define ANSI_C 0
#if defined(__STDC__)
#undef ANSI_C
#define ANSI_C 1
#endif

/* Global objects - variables */
static bPointer sourceBuffer; /* pointer to input (source) buffer */
bPointer stringLiteralTable; /* this buffer implements String Literal Table */
				  /* it is used as a repository for string literals */
int errorNumber;     /* run-time error number = 0 by default (ANSI) */
/*external objects */
extern int syntaxErrorNumber /* number of syntax errors reported by the parser */;
extern int line; /* source code line number - defined in scanner.c */
/* function declarations (prototypes) */
extern void startParser(void);
extern void startScanner(bPointer sc_buf);
/* For testing purposes */
/* extern Token malpar_next_token(void);*/

static void printParserError(char* fmt, ...);
static void displayParser(bPointer ptrBuffer);
static long getParserFileSize(char* fname);
static void callGarbageCollector(void);


/*************************************************************
 *  Main function takes a PLATYPUS source file as a command line argument.
 *  Usage: parser source_file_name
************************************************************/

int mainParser(int argc, char** argv) {

	FILE* fi;       /* input file handle */
	int loadsize = 0; /*the size of the file loaded in the buffer */
	int ansi_c = !ANSI_C; /* ANSI C flag */
    /* Check if the compiler option is set to compile ANSI C */
    /* __DATE__, __TIME__, __LINE__, __FILE__, __STDC__ are predefined preprocessor macros*/
	if (ansi_c) {
		printParserError("Date: %s  Time: %s", __DATE__, __TIME__);
		printParserError("ERROR: Compiler is not ANSI C compliant!\n");
		exit(1);
	}

	/*check for correct arrguments - source file name */
	if (argc <= 1) {
		/* __DATE__, __TIME__, __LINE__, __FILE__ are predefined preprocessor macros*/
		printParserError("Date: %s  Time: %s", __DATE__, __TIME__);
		printParserError("Runtime error at line %d in file %s", __LINE__, __FILE__);
		printParserError("%s%s%s", argv[0], ": ", "Missing source file name.");
		printParserError("%s%s%s", "Usage: ", "parser", "  source_file_name");
		exit(EXIT_FAILURE);
	}

	/* create a source code input buffer - multiplicative mode */
	sourceBuffer = bCreate(INIT_SIZE, INIT_INCREMENT, 'm');
	if (sourceBuffer == NULL) {
		printParserError("%s%s%s", argv[0], ": ", "Could not create source buffer");
		exit(EXIT_FAILURE);
	}

	/*open source file */
	if ((fi = fopen(argv[1], "r")) == NULL) {
		printParserError("%s%s%s%s", argv[0], ": ", "Cannot open file: ", argv[1]);
		exit(1);
	}

	/* load source file into input buffer  */
	printf("Reading file %s ....Please wait\n", argv[1]);
	loadsize = bLoad(sourceBuffer, fi);
	if (loadsize == RT_FAIL_1)
		printParserError("%s%s%s", argv[0], ": ", "Error in loading buffer.");

	/* close source file */
	fclose(fi);
	/*find the size of the file  */
	if (loadsize == LOAD_FAIL) {
		printf("The input file %s %s\n", argv[1], "is not completely loaded.");
		printf("Input file size: %ld\n", getParserFileSize(argv[1]));
	}
	/* Add SEOF (EOF) to input buffer and display the source buffer */
	if (bFinish(sourceBuffer, EOF)) {
		displayParser(sourceBuffer);
	}

	/* create string Literal Table */
	stringLiteralTable = bCreate(INIT_SIZE, INIT_INCREMENT, 'a');
	if (stringLiteralTable == NULL) {
		printParserError("%s%s%s", argv[0], ": ", "Could not create string literal buffer");
		exit(EXIT_FAILURE);
	}

	/*registrer exit function */
	atexit(callGarbageCollector);

	/*Testbed for buffer, scanner,symbol table and parser*/

	/* Initialize scanner  */
	startScanner(sourceBuffer);

	/* Start parsing */
	printf("\nParsing the source file...\n\n");

	startParser();

	return (EXIT_SUCCESS); /* same effect as exit(0) */

} /*end of main */

/*************************************************************
 * Error printing function with variable number of arguments
*************************************************************/

void printParserError(char* fmt, ...) {

	va_list ap;
	va_start(ap, fmt);

	(void)vfprintf(stderr, fmt, ap);
	va_end(ap);

	/* Move to new line */
	if (strchr(fmt, '\n') == NULL)
		fprintf(stderr, "\n");
}

/*************************************************************
* The function return the size of an open file
*************************************************************/

long getParserFileSize(char* fname) {
	FILE* input;
	long flength;
	input = fopen(fname, "r");
	if (input == NULL) {
		printParserError("%s%s", "Cannot open file: ", fname);
		return 0;
	}
	fseek(input, 0L, SEEK_END);
	flength = ftell(input);
	fclose(input);
	return flength;
}

/*************************************************************
* The function display buffer contents
*************************************************************/

void displayParser(bStructure* ptrBuffer) {
	printf("\nPrinting input buffer parameters:\n\n");
	printf("The capacity of the buffer is:  %d\n", bGetSize(ptrBuffer));
	printf("The current size of the buffer is:  %d\n", bGetAddChOffset(ptrBuffer));
	printf("\nPrinting input buffer contents:\n\n");
	bRewind(ptrBuffer);
	bDisplay(ptrBuffer, 20);
}

/*************************************************************
* The function frees all dynamically allocated memory.
* This function is always called
* despite how the program terminates - normally or abnormally.
*************************************************************/

void callGarbageCollector(void) {
	if (syntaxErrorNumber)
		printf("\nSyntax errors: %d\n", syntaxErrorNumber);
	printf("\nCollecting garbage...\n");
	bFree(sourceBuffer);
	bFree(stringLiteralTable);
}



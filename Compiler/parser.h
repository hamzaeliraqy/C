/*************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Winter, 2021
*************************************************************
* File name: parser.h
* Compiler: MS Visual Studio 2019
* Author: Svillen Ranev - Paulo Sousa - Abdulah
* Course: CST 8152 – Compilers, Lab Section: [011, 012, 013, or 014]
* Assignment: A3.
* Date: Sep 01 2020
* Purpose: This file is the main header for Parser (.h)
* Function list: (...).
*************************************************************/
/*************************************************************
 * COMPILERS COURSE - Algonquin College
 * Code version: 1.0
 * Author: Hamza Eliraqy
 * Student No: 040976448
 *************************************************************

/* TODO_101: Adjust the function header */

/* Inclusion section */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "token.h"
#include "buffer.h"

/* Global vars */
static Token lookahead;
int syntaxErrorNumber = 0;
extern bStructure* stringLiteralTable;
extern int line;
extern Token tokenizer();
extern char* keywordTable[];

/* Mock code: remove this UNKNOWN when variables defined */
//#define UNKNOWN 0

/* TODO_102: Create ALL constants for keywords (sequence given in table.h) */
/* Constants */
#define	NO_ATTR	-1
#define MAIN 	0
#define IF		1
#define THEN	2
#define	ELSE	3
#define WHILE	4
#define DO		5
#define READ	6
#define WRITE	7
#define TRUE	8
#define FALSE	9
// Continue the code

/* Function definitions */
void startParser(void);
void matchToken(int, int);
void syncErrorHandler(int);
void printError();
//void printMessage(char*);

/* TODO_103: Place ALL non-terminal function declarations */
void program(void);
void optionalStatements(void);
void statements(void);
void variableList(void);
// Continue the code
void additiveArithmeticExpression(void);
void additiveArithmeticExpressionPrime(void);
void arithmeticExpression(void);
void assignmentExpression(void);
void assignmentStatement(void);
void conditionalExpression(void);
void inputStatement(void);
void iterationStatement(void);
void logicalAndExpression(void);
void logicalAndExpressionPrime(void);
void logicalNotExpression(void);
void logicalOrExpression(void);
void logicalOrExpressionPrime(void);
void multiplicativeArithmeticExpression(void);
void multiplicativeArithmeticExpressionPrime(void);
void optionalStatements(void);
void outputStatement(void);
void outputVariableList(void);
void preCondition(void);
void primaryArithmeticExpression(void);
void primaryRelationalArithmeticExpression(void);
void primaryRelationalStringExpression(void);
void primaryRelationalStringExpressionPrime(void);
void primaryStringExpression(void);
void relationalArithmeticExpression(void);
void relationalArithmeticOperator(void);
void relationalExpression(void);
void relationalStringExpression(void);
void relationalStringOperator(void);
void slectionStatement(void);
void statement(void);
void statements(void);
void statementsPrime(void);
void stringExpression(void);
void stringExpressionPrime(void);
void variableIdentifier(void);
void variableList(void);
void variableListPrime(void);
void optionalVariableList(void);
void unaryArithmeticExpression(void);
void outputStatementPrime(void);
void primaryArithmeticRelationalExpressionPrime(void);
void primaryRelationalArithmeticExpression(void);
void primaryRelationalArithmeticExpressionPrime(void);
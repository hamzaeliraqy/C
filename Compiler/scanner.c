/********************************************************************
 * Filename: scanner.c
 * PURPOSE:
 *    Functions implementing a Lexical Analyzer (Scanner)
 *    as required for CST8152, Assignment #2
 *  Author: Hamza Eliraqy
 *******************************************************************
 */

 

 /* The #define _CRT_SECURE_NO_WARNINGS should be used in MS Visual Studio projects
  * to suppress the warnings about using "unsafe" functions like fopen()
  * and standard sting library functions defined in string.h.
  * The define does not have any effect in Borland compiler projects.
  */
#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>   /* standard input / output */
#include <ctype.h>   /* conversion functions */
#include <stdlib.h>  /* standard library functions and constants */
#include <string.h>  /* string functions */
#include <limits.h>  /* integer types constants */
#include <float.h>   /* floating-point types constants */
#include <stdbool.h> 

  /*#define NDEBUG        to suppress assert() call */
#include <assert.h>  /* assert() prototype */

/* project header files */
#include "buffer.h"
#include "token.h"
#include "table.h"

#define DEBUG  /* for conditional processing */
#undef  DEBUG

/* Global objects - variables */
/* This buffer is used as a repository for string literals.
   It is defined in platy_st.c */
extern bPointer stringLiteralTable;		/* String literal table */
int line;								/* current line number of the source code */
extern int errorNumber;					/* defined in platy_st.c - run-time error number */

static char debugMode = 0;				/* optional for debugging */

/* Local(file) global objects - variables */
static bPointer lexemeBuffer;			/* pointer to temporary lexeme buffer */
static bPointer sourceBuffer;			/* pointer to input source buffer */
/* No other global variable declarations/definitiond are allowed */

/* scanner.c static(local) function  prototypes */
static int nextClass(char c);			/* character class function */
static int nextState(int, char);		/* state machine function */
static int isKeyword(char* kw_lexeme);	/* keywords lookup function */

/************************************************************
* Function name: startScanner
* History/Versions: Ver 1.0
* Called functions: bClean(), bIsEmpty(), bRewind()
* Parameters:
*  bPointer
* Return value: int
**************************************************************/
/*************************************************************
 * Intitializes scanner
 *		This function initializes the scanner using defensive programming.
 ************************************************************/
int startScanner(bPointer psc_buf) {
	if (bIsEmpty(psc_buf))
		return EXIT_FAILURE; /*1*/
	/* in case the buffer has been read previously  */
	bRewind(psc_buf);
	bClean(stringLiteralTable);
	line = 1;
	sourceBuffer = psc_buf;
	return EXIT_SUCCESS; /*0*/
}

/************************************************************
* Function name: tokenizer
* Author: Hamza Eliraqy
* History/Versions: Ver 1.0
* Called functions: bGetCh(), bRetract(), bFree(), bGetContent(), bGetChOffset(), bSetMarkOffset(), nextState(), bAddCh()
* Parameters:
*  void
* Return value: Token
**************************************************************/
/*************************************************************
 * Process Token
 *		Main function of buffer, responsible to classify a char (or sequence
 *		of chars). In the first part, a specific sequence is detected (reading
 *		from buffer). In the second part, a pattern (defined by Regular Expression)
 *		is recognized and the appropriate function is called (related to final states
 *		in the Transition Diagram).
 ************************************************************/
Token tokenizer(void) {
	Token currentToken = { 0 }; /* token to return after pattern recognition. Set all structure members to 0 */
	unsigned char c;	/* input symbol */
	int state = 0;		/* initial state of the FSM */
	short lexStart = 0;		/* start offset of a lexeme in the input char buffer (array) */
	short lexEnd;		/* end offset of a lexeme in the input char buffer (array)*/

	int lexLength;		/* token length */
	int i;				/* counter */
	unsigned char newc;	/* new char */

	while (1) { /* endless loop broken by token returns it will generate a warning */
		c = bGetCh(sourceBuffer);

		/* ------------------------------------------------------------------------
			Part 1: Implementation of token driven scanner.
			Every token is possessed by its own dedicated code
			-----------------------------------------------------------------------
		*/

		switch (c) {
			//comments
		case '%':
			newc = bGetCh(sourceBuffer);
			if (newc == '%') {

				while (newc != '\n') {
					newc = bGetCh(sourceBuffer);
					if (newc == CHARSEOF255 || newc == CHARSEOF0) {
						bRetract(sourceBuffer);

						break;
					}
				}
				if (newc == '\n') {
					line++;
				}
				break;
			}
			else if (newc != '%') {
				currentToken.code = ERR_T;
				newc = bGetCh(sourceBuffer);
				while (newc != '\n') {
					if (newc == CHARSEOF0 || newc == CHARSEOF255) {
						bRetract(sourceBuffer);
						break;
					}
				}
				if (newc == '\n') {
					line++;
				}
				return currentToken;
				break;
			}

			//SEPARATORS
		case '{':
			currentToken.code = LBR_T;
			return currentToken;
			break;

		case '}':
			currentToken.code = RBR_T;
			return currentToken;
			break;
		case '(':
			currentToken.code = LPR_T;
			return currentToken;
			break;
		case ')':
			currentToken.code = RPR_T;
			return currentToken;
			break;
		case ',':
			currentToken.code = COM_T;
			return currentToken;
			break;
		case ';':
			currentToken.code = EOS_T;
			return currentToken;
			//END OF FILE
		case CHARSEOF0:
			currentToken.code = SEOF_T;
			currentToken.attribute.codeType = SEOF_0;
			return currentToken;
			break;
		case CHARSEOF255:
			currentToken.code = SEOF_T;
			currentToken.attribute.codeType = SEOF_EOF;
			return currentToken;
			break;
			//Relational OP
		case '=':
			newc = bGetCh(sourceBuffer);
			currentToken.code = ASS_OP_T;
			//Token newcurrrentToken = { 0 };
			//unsigned char newc2 = bGetCh(sourceBuffer);
			//currentToken.code = ASS_OP_T;
			if (newc == '=') {
				currentToken.code = REL_OP_T;
				currentToken.attribute.relationalOperator = EQ;
				//bRetract(sourceBuffer);
				bRetract(sourceBuffer);
				return currentToken;
			}
			/*if(newc == '+'){
				currentToken.code = ART_OP_T;					//FIND SOLUTION TO THIS SHOULD ACCEPT BOTH = AND +
				currentToken.attribute.codeType = ADD;
				//newcurrrentToken.code = ART_OP_T;
				//newcurrrentToken.attribute.codeType = ADD;
				bRetract(sourceBuffer);
				//return currentToken, newcurrrentToken;
				}*/
				/*else {
					bRetract(sourceBuffer);
					currentToken.code = ASS_OP_T;
					//bRetract(sourceBuffer);

				}*/
			bRetract(sourceBuffer);
			return currentToken;


		case '<':
			currentToken.code = REL_OP_T;
			currentToken.attribute.codeType = LT;
			return currentToken;
			break;

		case '>':
			currentToken.code = REL_OP_T;
			currentToken.attribute.codeType = GT;
			return currentToken;
			break;
		case '!':
			newc = bGetCh(sourceBuffer);
			char newc2 = bGetCh(sourceBuffer);
			if (newc == '=') {
				currentToken.code = REL_OP_T;
				currentToken.attribute.codeType = NE;
				//bRetract(sourceBuffer);
				//return currentToken;
			}
			else {
				bRetract(sourceBuffer);
				currentToken.code = ERR_T;
				currentToken.attribute.errLexeme[0] = '!';
				currentToken.attribute.errLexeme[1] = '\0';
				//return currentToken;
			}
			//bRetract(sourceBuffer);
			return currentToken;
			break;
			//Logical OP
		case '.':
		{
			char newc1 = bGetCh(sourceBuffer);
			char newc2 = bGetCh(sourceBuffer);
			char newc3 = bGetCh(sourceBuffer);
			char newc4 = bGetCh(sourceBuffer);

			if (newc1 == 'O' && newc2 == 'R' && newc3 == '.') {
				currentToken.code = LOG_OP_T;
				currentToken.attribute.codeType = OR;
				return currentToken;

			}
			else if (newc1 == 'N' && newc2 == 'O' && newc3 == 'T' && newc4 == '.') {
				currentToken.code = LOG_OP_T;
				currentToken.attribute.codeType = NOT;
				return currentToken;

			}
			else if (newc1 == 'A' && newc2 == 'N' && newc3 == 'D' && newc4 == '.') {
				currentToken.code = LOG_OP_T;
				currentToken.attribute.codeType = AND;
				return currentToken;
			}
			else {
				currentToken.code = ERR_T;
			}
			for (int i = 0; i < 2; i++) {
				bRetract(sourceBuffer);
			}

			return currentToken;
			break;
		}
		case '\n':
			line++;
			break;
			//Arithmetic OP
		case '+':
			newc = bGetCh(sourceBuffer);
			currentToken.code = ART_OP_T;
			currentToken.attribute.codeType = ADD;
			if (newc == '+') {
				currentToken.code = SCC_OP_T;
				bRetract(sourceBuffer);
			}
			/*else {
				currentToken.code = ART_OP_T;
				currentToken.attribute.codeType = ADD;
			}*/
			return currentToken;
			break;

		case '-':
			currentToken.code = ART_OP_T;
			currentToken.attribute.codeType = SUB;
			return currentToken;
			break;

		case '*':
			currentToken.code = ART_OP_T;
			currentToken.attribute.codeType = MUL;
			return currentToken;
			break;

		case '/':
			currentToken.code = ART_OP_T;
			currentToken.attribute.codeType = DIV;
			return currentToken;
			break;

		case ' ':
			break;

			//test all sequences of next chars of all possibility AND / OR / ..
			//continue of other cases
		/* ------------------------------------------------------------------------
			Part 2: Implementation of Finite State Machine (DFA)
					   or Transition Table driven Scanner
					   Note: Part 2 must follow Part 1 to catch the illegal symbols
			-----------------------------------------------------------------------
		*/

	

		default: // general case
			if (isalpha(c) || isalnum(c) || c == '$') {
				state = nextState(state, c);
				lexStart = bGetChOffset(sourceBuffer);

				int mark = bSetMarkOffset(sourceBuffer, lexStart);
				while (stateType[state] == NOAS) {
					newc = bGetCh(sourceBuffer);
					if (newc == '.') {
						newc = bGetCh(sourceBuffer);
					}
					if (c == '\'') {
						while (newc != '\'') {
							newc = bGetCh(sourceBuffer);
						}
					}
					state = nextState(state, newc);
				}
				if (stateType[state] == ASWR) {
					bRetract(sourceBuffer);
				}

				lexEnd = bGetChOffset(sourceBuffer);

				lexLength = lexEnd - lexStart;
				lexLength = abs(lexLength) + 1;
				lexemeBuffer = bCreate(lexLength + 1, FIXMODE, 'f');
				bRestore(sourceBuffer);

				bAddCh(lexemeBuffer, c);

				for (i = 0; i < lexLength - 1; i++) {
					bAddCh(lexemeBuffer, bGetCh(sourceBuffer));

				}

				bAddCh(lexemeBuffer, '\0');

				currentToken = finalStateTable[state](bGetContent(lexemeBuffer, mark));

				bFree(lexemeBuffer);
			}
			else {
				currentToken.code = ERR_T;
				currentToken.attribute.errLexeme[0] = c;
				currentToken.attribute.errLexeme[1] = '\0';
			}

			return currentToken;

		} // switch

	} //while

} // tokenizer


/* DO NOT MODIFY THE CODE / COMMENT OF THIS FUNCTION */
/*************************************************************
 * Get Next State
	The assert(int test) macro can be used to add run-time diagnostic to programs
	and to "defend" from producing unexpected results.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	(*) assert() is a macro that expands to an if statement;
	if test evaluates to false (zero) , assert aborts the program
	(by calling abort()) and sends the following message on stderr:
	(*) Assertion failed: test, file filename, line linenum.
	The filename and linenum listed in the message are the source file name
	and line number where the assert macro appears.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	If you place the #define NDEBUG directive ("no debugging")
	in the source code before the #include <assert.h> directive,
	the effect is to comment out the assert statement.
	- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
	The other way to include diagnostics in a program is to use
	conditional preprocessing as shown bellow. It allows the programmer
	to send more details describing the run-time problem.
	Once the program is tested thoroughly #define DEBUG is commented out
	or #undef DEBUF is used - see the top of the file.
 ************************************************************/

int nextState(int state, char c) {
	int col;
	int next;
	col = nextClass(c);
	next = transitionTable[state][col];
#ifdef DEBUG
	printf("Input symbol: %c Row: %d Column: %d Next: %d \n", c, state, col, next);
#endif
	assert(next != IS);
#ifdef DEBUG
	if (next == IS) {
		printf("Scanner Error: Illegal state:\n");
		printf("Input symbol: %c Row: %d Column: %d\n", c, state, col);
		exit(1);
	}
#endif
	return next;
}

/************************************************************
* Function name: nextClass
* History/Versions: Ver 1.0
* Called functions: isalpha(), isdigit()
* Parameters:
*  char
* Return value: int
**************************************************************/
/*************************************************************
 * Get Next Token Class
	* Create a function to return the column number in the transition table:
	* Considering an input char c, you can identify the "class".
	* For instance, a letter should return the column for letters, etc.
************************************************************/
int nextClass(char c) {
	int val = -1;

	if (isalpha(c)) {
		val = 0;
	}
	else if (isdigit(c)) {
		val = 1;
	}
	else if (c == '.') {
		val = 2;
	}
	else if (c == '$') {
		val = 3;
	}
	else if (c == '\'') {
		val = 4;
	}
	else if (c == SEOF_EOF || c == SEOF_0) {
		val = 5;
	}
	else {
		val = 6;
	}

	return val;
}

/************************************************************
* Function name: funcAVID
* Author: Hamza Eliraqy
* History/Versions: Ver 1.0
* Called functions: isKeyword(), srtlen(), strcpy()
* Parameters:
*  char*
* Return value: Token
**************************************************************/
/*************************************************************
 * Acceptance State Function AVID
 *		In this function, the pattern for AVID must be recognized.
 *		Since keywords obey the same pattern, is required to test if
 *		the current lexeme matches with KW from language.
 *	- Remember to respect the limit defined for lexemes (VID_LEN) and
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *    Remember to end each token with the \0.
 *  - Suggestion: Use "strncpy" function.
 ************************************************************/

Token funcAVID(char* lexeme) {
	Token currentToken = { 0 };

	if (isKeyword(lexeme) != -1) {
		currentToken.code = KW_T;
		int keyword = isKeyword(lexeme);
		currentToken.attribute.keywordIndex = isKeyword(lexeme);
		return currentToken;
	}

	if (strlen(lexeme) < VID_LEN) {
		strcpy(currentToken.attribute.vidLexeme, lexeme);
		currentToken.code = AVID_T;
	}
	else {
		currentToken.code = ERR_T;
	}

	return currentToken;
}

/************************************************************
* Function name: funcSVID
* Author: Hamza Eliraqy
* History/Versions: Ver 1.0
* Called functions: srtlen(), strcpy()
* Parameters:
*  char*
* Return value: Token
**************************************************************/
/*************************************************************
 * Acceptance State Function SVID
 *		In this function, the pattern for SVID must be recognized.
 *	- Remember to respect the limit defined for lexemes (VID_LEN),
 *	  also observing the delimiters. Remember to end also with \0.
 *	  set the lexeme to the corresponding attribute (vidLexeme).
 *  - Suggestion: Use "strncpy" function.
 ************************************************************/

Token funcSVID(char* lexeme) {
	Token currentToken = { 0 };

	currentToken.code = SVID_T;

	if (strlen(lexeme) > VID_LEN) {
		for (int i = 0; i < VID_LEN - 1; i++) {
			currentToken.attribute.vidLexeme[i] = lexeme[i];
		}
		currentToken.attribute.vidLexeme[VID_LEN - 1] = '$';
		currentToken.attribute.vidLexeme[VID_LEN] = SEOF_EOF;
		return currentToken;
	}
	else {
		currentToken.code = ERR_T;
	}
	strcpy(currentToken.attribute.vidLexeme, lexeme);
	return currentToken;
}

/************************************************************
* Function name: funcIL
* Author: Hamza Eliraqy
* History/Versions: Ver 1.0
* Called functions: atol(), strlen(), funcFPL()
* Parameters:
*  char*
* Return value: Token
**************************************************************/
/*************************************************************
 * Acceptance State Function IL
 *		Function responsible to identify IL (integer literals).
 * - It is necessary respect the limit (ex: 2-byte integer in C).
 * - In the case of larger lexemes, error shoul be returned.
 * - Only first ERR_LEN characters are accepted and eventually,
 *   additional three dots (...) should be put in the output.
 ************************************************************/

Token funcIL(char* lexeme) {
	Token currentToken = { 0 };

	long il;
	il = atol(lexeme);
	int len = strlen(lexeme);
	int op = 0;
	for (int i = 0; i < len; i++) {
		if (lexeme[i] == '.') {
			op = 1;
		}
	}

	if (op == 1) {
		return currentToken = funcFPL(lexeme);
	}
	else {
		if (len <= INL_LEN) {
			currentToken.code = INL_T;
			currentToken.attribute.intValue = il;
		}
		else {
			currentToken.code = ERR_T;
		}
	}


	return currentToken;
}

/************************************************************
* Function name: funcFPL
* Author: Hamza Eliraqy
* History/Versions: Ver 1.0
* Called functions: atof(), strlen()
* Parameters:
*  char*
* Return value: Token
**************************************************************/
/*************************************************************
 * Acceptance State Function FPL
 *		Function responsible to identify FPL (float point literals).
 * - It is necessary respect the limit (ex: 4-byte integer in C).
 * - In the case of larger lexemes, error shoul be returned.
 * - Only first ERR_LEN characters are accepted and eventually,
 *   additional three dots (...) should be put in the output.
 ************************************************************/

Token funcFPL(char* lexeme) {
	Token currentToken = { 0 };
	/*
	float fpl;
	fpl = atof(lexeme);
	if (strlen(lexeme) < FLT_MAX) {
		currentToken.code = FPL_T;
		currentToken.attribute.floatValue = fpl;
	}
	else {
		currentToken.code = ERR_T;
	}
	*/
	double fpl = 0.0;
	float minValue = 1.175494351e-38F;
	float maxValue = 3.402823466e+38F;
	fpl = atof(lexeme);
	if ((fpl < 0.0f && (fpl < -maxValue || fpl > -minValue) || fpl > 0.0f && (fpl < minValue || fpl > maxValue))) {
		return funcErr(lexeme);
	}
	if (fpl == 1.2) {
		currentToken.code = FPL_T;
		currentToken.attribute.floatValue = (float)fpl;
		//return currentToken;
	}
	currentToken.code = FPL_T;
	currentToken.attribute.floatValue = (float)fpl;
	return currentToken;
}

/************************************************************
* Function name: funcSL
* Author: Hamza Eliraqy
* History/Versions: Ver 1.0
* Called functions: bGetAddChOffset(), srtlen(), bAddCh()
* Parameters:
*  char*
* Return value: Token
**************************************************************/
/*************************************************************
 * Acceptance State Function SL
 *		Function responsible to identify SL (string literals).
 * - The lexeme must be stored in the String Literal Table
 *   (stringLiteralTable). You need to include the literals in
 *   this structure, using offsets. Remember to include \0 to
 *   separate the lexemes. Remember also to incremente the line.
 ************************************************************/


Token funcSL(char* lexeme) {
	/*Token currentToken = { 0 };

	currentToken.code = STR_T;

	currentToken.attribute.contentString = bGetAddChOffset(stringLiteralTable);
	for (int i = 0; i < strlen(lexeme); i++) {
		if (lexeme[i] == '\'') {
			lexeme[i] = ' ';

		}
		bAddCh(stringLiteralTable, lexeme[i]);
	}

	bAddCh(stringLiteralTable, '\0');
	*/
	Token currentToken = { 0 };
	int index = 1;
	currentToken.code = STR_T;
	currentToken.attribute.contentString = bGetAddChOffset(stringLiteralTable);
	while (index < (signed)strlen(lexeme)) {
		if (lexeme[index] == '\n') {
			line++;
		}
		else if (lexeme[index] != '\'') {
			bAddCh(stringLiteralTable, lexeme[index]);
		}
		index++;
	}
	bAddCh(stringLiteralTable, CHARSEOF0);
	return currentToken;
}

/************************************************************
* Function name: funcErr
* Author: Hamza Eliraqy
* History/Versions: Ver 1.0
* Called functions: strcpy(), srtlen()
* Parameters:
*  char*
* Return value: Token
**************************************************************/
/*************************************************************
 * Acceptance State Function Error
 *		Function responsible to deal with ERR token.
 * - This function uses the errLexeme, respecting the limit given
 *   by ERR_LEN. If necessary, use three dots (...) to use the
 *   limit defined. The error lexeme contains line terminators,
 *   so remember to increment line.
 ************************************************************/

Token funcErr(char* lexeme) {
	Token currentToken = { 0 };

	currentToken.code = ERR_T;

	if (strlen(lexeme) < ERR_LEN) {
		strcpy(currentToken.attribute.errLexeme, lexeme);
	}
	for (int i = 0; i < strlen(lexeme); i++) {
		if (lexeme[i] == '\n') {
			line++;
		}
	}

	return currentToken;
}

/************************************************************
* Function name: isKeyword
* Author: Hamza Eliraqy
* History/Versions: Ver 1.0
* Called functions:
* Parameters:
*  char*
* Return value: int
**************************************************************/
/*************************************************************
 * This function checks if one specific lexeme is a keyword.
 ************************************************************/


int isKeyword(char* lexeme) {
	int i = -1;

	for (int j = 0; j < 10; j++) {
		char* kt = keywordTable[j];
		if (kt[0] == lexeme[0] && kt[1] == lexeme[1]) {
			return j;
			break;
		}
	}

	return i;
}



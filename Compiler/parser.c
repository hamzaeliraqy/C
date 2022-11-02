/**************************************************************
* File name: parser.c
* Compiler: MS Visual Studio 2019
* Author: Hamza Eliraqy
*************************************************************/





#include "parser.h"

 /*************************************************************
  * Process Parser
  ************************************************************/
void startParser(void) {
	lookahead = tokenizer();
	program();
	matchToken(SEOF_T, NO_ATTR);
	printf("%s\n", "PLATY: Source file parsed");
}

/*************************************************************
 * Match Token
 ************************************************************/
void matchToken(int tokenCode, int tokenAttribute) {
	int matchFlag = 1;
	switch (lookahead.code) {
	case KW_T:
	case REL_OP_T:
	case ART_OP_T:
	case LOG_OP_T:
		if (lookahead.attribute.codeType != tokenAttribute) {
			matchFlag = 0;
		}
	default:
		if (lookahead.code != tokenCode) {
			matchFlag = 0;
		}
	}
	if (matchFlag) {
		lookahead = tokenizer();
		if (lookahead.code == ERR_T) {
			printError();
			lookahead = tokenizer();
			syntaxErrorNumber++;
		}
	}
	else {
		syncErrorHandler(tokenCode);
	}


}

/*************************************************************
 * Syncronize Error Handler
 ************************************************************/

void syncErrorHandler(int syncTokenCode) {
	printError();
	syntaxErrorNumber++;
	while (lookahead.code != syncTokenCode) {
		if (lookahead.code == SEOF_T) {
			exit(syntaxErrorNumber);
		}
		lookahead = tokenizer();
	}
	if (lookahead.code != SEOF_T) {
		lookahead = tokenizer();
	}

}


/*************************************************************
 * Print Error
 ************************************************************/
void printError() {
	Token t = lookahead;
	printf("PLATY: Syntax error:  Line:%3d\n", line);
	printf("*****  Token code:%3d Attribute: ", t.code);
	switch (t.code) {
	case ERR_T:
		printf("%s\n", t.attribute.errLexeme);
		break;
	case SEOF_T:
		printf("%d\n", t.attribute.seofType);
		break;
	case AVID_T:
	case SVID_T:
		printf("%s\n", t.attribute.vidLexeme);
		break;
	case FPL_T:
		printf("%f\n", t.attribute.floatValue);
		break;
	case INL_T:
		printf("%d\n", t.attribute.intValue);
		break;
	case STR_T:
		printf("%p\n", bGetContent(stringLiteralTable, t.attribute.logicalOperator));
		break;
	case SCC_OP_T:
		printf("NA\n");
		break;
	case ASS_OP_T:
		printf("NA\n");
		break;
	case ART_OP_T:
		printf("%d\n", t.attribute.intValue);
		break;
	case REL_OP_T:
		printf("%d\n", t.attribute.intValue);
		break;
	case LOG_OP_T:
		printf("%d\n", t.attribute.intValue);
		break;
	case LPR_T:
		printf("NA\n");
		break;
	case RPR_T:
		printf("NA\n");
		break;
	case LBR_T:
		printf("NA\n");
		break;
	case RBR_T:
		printf("NA\n");
		break;
	case KW_T:
		printf("%s\n", keywordTable[t.attribute.intValue]);
		break;
	case COM_T:
		printf("NA\n");
		break;
	case EOS_T:
		printf("NA\n");
		break;
	default:
		printf("PLATY: Scanner error: invalid token code : %d\n", t.code);

	}
}


/*************************************************************
 * Program statement
 * BNF: <program> -> PLATYPUS { <opt_statements> }
 * FIRST(<program>)= {KW_T (MAIN)}.
 ************************************************************/
void program(void) {
	matchToken(KW_T, MAIN);
	matchToken(LBR_T, NO_ATTR);
	optionalStatements();
	matchToken(RBR_T, NO_ATTR);
	printf("%s\n", "PLATY: Program parsed");
}


/*************************************************************
 * Optional statement
 * <opt_statements> -> <statements> | ϵ
 * FIRST(<opt_statements>) = {AVID_T, SVID_T, KW_T(IF), KW_T(WHILE), KW_T(READ), KW_T(WRITE), ϵ}
 ************************************************************/
void optionalStatements(void) {
	switch (lookahead.code) {
	case AVID_T:
		statements();
		break;
	case SVID_T:
		statements();
		break;
	case KW_T:
		if (lookahead.attribute.codeType == IF
			|| lookahead.attribute.codeType == WHILE
			|| lookahead.attribute.codeType == READ
			|| lookahead.attribute.codeType == WRITE) {
			statements();
			break;
		}
	default:
		break;
	}
	printf("%s\n", "PLATY: Optional statements parsed");
}


/*************************************************************
 * Input Statement
 * <input statement> -> READ (<variable list>);
 * FIRST(<input statement>) = {KW_T(READ)}
 ************************************************************/
void inputStatement(void) {
	matchToken(KW_T, READ);
	matchToken(LPR_T, NO_ATTR);
	variableList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s\n", "PLATY: Input statement parsed");
}



/*************************************************************
 * Statements
 * <statement> ->  <assignment statement> | <selection statement> | <iteration statement>
| <input statement> | <output statement>

 *FIRST(<statement>) = {AVID_T, SVID_T, KW_T(IF), KW_T(WHILE), KW_T(READ), KW_T(WRITE)}
 ************************************************************/
void statements(void) {
	
	statement();
	statementsPrime();
	printf("PLATY: Statements parsed\n");
}

/*************************************************************
 * Variable List
 * <variable list>  -> <variable identifier> <variable listPrime>
			 <variable listPrime><variable identifier><variable listPrime> | ϵ

 * FIRST(<variable list>) = {AVID_T, SVID_T}
		FIRST(<variable listPrime>) = {COM_T,   ϵ}

 ************************************************************/
void variableList(void) {
	// TODO
	variableIdentifier();
	variableListPrime();
	printf("%s\n", "PLATY: Variable list parsed");
}

/*************************************************************
 * additiveArithmeticExpression
 *<additive arithmetic expression> -> <multiplicative arithmetic expression><additive arithmetic expressionPrime>
		<additive arithmetic expressionPrime> +<multiplicative arithmetic expression><additive arithmetic expressionPrime> | -<multiplicative arithmetic expression><additive arithmetic expressionPrime> | ϵ



 * FIRST(<additive arithmetic expression>) = {AVID_T, FPL_T, LPR_T }
		FIRST(<additive arithmetic expressionPrime>) = {+, - , ϵ}


 ************************************************************/
void additiveArithmeticExpression(void) {
	multiplicativeArithmeticExpression();
	additiveArithmeticExpressionPrime();
}

void additiveArithmeticExpressionPrime(void) {
	if (lookahead.code == ART_OP_T) {
		if (lookahead.attribute.arithmeticOperator == ADD) {
			matchToken(ART_OP_T, ADD);
			multiplicativeArithmeticExpression();
			additiveArithmeticExpressionPrime();

			//printf("%s\n", "PLATY: Arithmetic expression parsed");

		}
		if (lookahead.attribute.arithmeticOperator == SUB) {
			matchToken(ART_OP_T, SUB);
			multiplicativeArithmeticExpression();
			additiveArithmeticExpressionPrime();

			//printf("%s\n", "PLATY: Arithmetic expression parsed");
		}
	}
}
/*************************************************************
 * Arithmetic Expression
 * <arithmetic expression> ->  <unary arithmetic expression> | <additive arithmetic expression>

 * FIRST(<arithmetic expression>) = {-, +, AVID_T, FPL_T, INL_T, LPR_T}

 ************************************************************/
void arithmeticExpression(void) {
	switch (lookahead.code) {
	case ART_OP_T:
		if (lookahead.attribute.arithmeticOperator == ADD || lookahead.attribute.arithmeticOperator == SUB) {
			unaryArithmeticExpression();
		}
		else {
			printError();
			break;
		}
		//printf("%s\n", "PLATY: Arithmetic expression parsed");
		break;
	case AVID_T:
		additiveArithmeticExpression();
		//printf("%s\n", "PLATY: Arithmetic expression parsed");
		break;
	case FPL_T:
		additiveArithmeticExpression();
		//printf("%s\n", "PLATY: Arithmetic expression parsed");
		break;
	case INL_T:
		additiveArithmeticExpression();
		//printf("%s\n", "PLATY: Arithmetic expression parsed");
		break;
	case LPR_T:
		additiveArithmeticExpression();

		break;
	default:
		printError();
		break;
	}
	printf("%s\n", "PLATY: Arithmetic expression parsed");
}
/*************************************************************
 * Unary Arithmetic Expression
 * <unary arithmetic expression>  ->  - <primary arithmetic expression>
				| + <primary arithmetic expression>


 * FIRST(<unary arithmetic expression>) = {-,+}

 ************************************************************/
void unaryArithmeticExpression(void) {
	switch (lookahead.code) {
	case ART_OP_T:
		if (lookahead.attribute.arithmeticOperator == ADD) {
			matchToken(ART_OP_T, ADD);
			primaryArithmeticExpression();
		}
		if (lookahead.attribute.arithmeticOperator == SUB) {
			matchToken(ART_OP_T, SUB);
			primaryArithmeticExpression();
		}

		break;
	default:
		printError();
		break;
	}

	printf("%s\n", "PLATY: Unary Arithmetic expression parsed");

}
/*************************************************************
 * Assignment Expression
 * <assignment expression>  ->  AVID = <arithmetic expression> | SVID = <string expression>

 * FIRST(<assignment expression>) = {AVID_T, SVID_T}

 ************************************************************/
void assignmentExpression(void) {
	switch (lookahead.code) {
	case AVID_T:
		matchToken(AVID_T, NO_ATTR);
		matchToken(ASS_OP_T, EQ);
		arithmeticExpression();
		printf("%s\n", "PLATY: Assignment Expression parsed");
		break;
	case SVID_T:
		matchToken(SVID_T, NO_ATTR);
		matchToken(ASS_OP_T, EQ);
		stringExpression();
		printf("%s\n", "PLATY: Assignment Expression parsed");
		break;
	default:
		printError();
		break;
	}
}
/*************************************************************
 * Assignment Statement
 * <assignment statement>  ->    <assignment expression>;

 * FIRST(<assignment statement>) = {AVID_T, SVID_T}

 ************************************************************/
void assignmentStatement(void) {
	assignmentExpression();
	matchToken(EOS_T, NO_ATTR);
	printf("%s\n", "PLATY: Assignment statement parsed");
}
/*************************************************************
 * Conditional Expression
 * <conditional expression> ->  <logical OR  expression>

 * FIRST(<conditional expression>) = { AVID_T, FPL_T, INL_T, SVID_T, STR_T}

 ************************************************************/
void conditionalExpression(void) {
	logicalOrExpression();
	printf("%s\n", "PLATY: Conditional expression parsed");
}

/*************************************************************
 * Iteration Statement
 * <iteration statement> ->   WHILE <pre-condition> (<conditional expression>)
	  DO { <statements>};
* <pre-condition> -> TRUE | FALSE

 * FIRST(<iteration statement>) = {KW_T(WHILE)}
 * FIRST(<pre-condition>) = KW_T(TRUE), KW_T(FALSE)

 ************************************************************/
void iterationStatement(void) {
	matchToken(KW_T, WHILE);
	preCondition();
	matchToken(LPR_T, NO_ATTR);
	conditionalExpression();
	matchToken(RPR_T, NO_ATTR);

	matchToken(KW_T, DO);
	matchToken(LBR_T, NO_ATTR);
	statements();
	matchToken(RBR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);

	printf("%s\n", "PLATY: Iteration statement parsed");
}
/*************************************************************
 * Logical And Expression
 * <logical AND expression>-><logical AND expression> .AND. <logical NOT expression> | <logical NOT expression>
<logical AND expression><logical NOT expression><logical AND expressionPrime>
<logical NOT expressionPrime>.AND. <logical NOT expression><logical AND expressionPrime> | ϵ


 * FIRST(<logical AND expression>) = {AVID_T, FPL_T, INL_T, SVID_T, STR_T}
		FIRST<logical AND expressionPrime> = {.AND., ϵ}


 ************************************************************/
void logicalAndExpression(void) {
	relationalExpression();
	logicalAndExpressionPrime();
}

void logicalAndExpressionPrime(void) {
	if (lookahead.code == LOG_OP_T) {
		if (lookahead.attribute.logicalOperator == AND) {
			matchToken(LOG_OP_T, AND);
			relationalExpression();
			logicalAndExpressionPrime();
			//printf("%s\n", "PLATY: Logical AND expression parsed");
		}
	}
}
/*************************************************************
 * Logical Not Expression
 * <logical NOT expression> ->  .NOT. <relational expression> | <relational expression>


* FIRST(<logical NOT expression>) = {LOG_OP_T(NOT), AVID_T, INL_T, FPL_T, SVID_T, STR_T}

 ************************************************************/
void logicalNotExpression(void) {
	if (lookahead.code == LOG_OP_T) {
		if (lookahead.attribute.logicalOperator == NOT) {
			matchToken(LOG_OP_T, NOT);
			relationalExpression();
		}
	}
}
/*************************************************************
 * Logical OR Expression
 * <logical  OR expression> ->  <logical OR expression>  .OR.  <logical AND expression> | <logical AND expression>
		<logical  OR expression>-> <logical AND expression><logical OR expressionPrime>
<logical OR expressionPrime> .OR. <logical AND expression><logical OR expressionPrime> | ϵ



* FIRST(<logical  OR expression>) = {AVID_T, FPL_T, INL_T, SVID_T, STR_T}
		FIRST(<logical  OR expression>) = {.OR., ϵ}


 ************************************************************/
void logicalOrExpression(void) {
	logicalAndExpression();
	logicalOrExpressionPrime();
}

void logicalOrExpressionPrime(void) {
	if (lookahead.code == LOG_OP_T) {
		if (lookahead.attribute.logicalOperator == OR) {
			matchToken(LOG_OP_T, OR);
			logicalAndExpression();
			logicalOrExpressionPrime();
			//printf("%s\n", "PLATY: Logical OR expression parsed");
		}

	}
}
/*************************************************************
 * Logical Not Expression
 * <multiplicative arithmetic expression>-><primary arithmetic expression><multiplicative arithmetic expressionPrime>

<multiplicative arithmetic expressionPrime>->*<primary arithmetic expression><multiplicative arithmetic expressionPrime> | /<primary arithmetic expression><multiplicative arithmetic expressionPrime> | ϵ

* FIRST(<multiplicative arithmetic expressionPrime>) = {AVID_T, FPL_T, INL_T, LPR_T}
	FIRST(<multiplicative arithmetic expressionPrime>) = {*, /, ϵ}


 ************************************************************/
void multiplicativeArithmeticExpression(void) {
	primaryArithmeticExpression();
	multiplicativeArithmeticExpressionPrime();
}

void multiplicativeArithmeticExpressionPrime(void) {
	if (lookahead.code == ART_OP_T) {
		if (lookahead.attribute.arithmeticOperator == DIV) {
			matchToken(ART_OP_T, DIV);
			primaryArithmeticExpression();
			multiplicativeArithmeticExpressionPrime();
			//printf("%s\n", "PLATY: Multiplicative arithmetic expression parsed");
		}
		if (lookahead.attribute.arithmeticOperator == MUL) {
			matchToken(ART_OP_T, MUL);
			primaryArithmeticExpression();
			multiplicativeArithmeticExpressionPrime();
			//printf("%s\n", "PLATY: Multiplicative arithmetic expression parsed");
		}
	}
}


/*************************************************************
 * Output Statement
 * <output statement> -> WRITE (<output statementPrime>);
<output statementPrime> -> <opt_variable list> | STR_T

* FIRST(<output statement>) = {KW_T(WRITE)}
		FIRST(<output statementPrime>) = {AVID_T, SVID_T, STR_T, ϵ}

 ************************************************************/
void outputStatement(void) {
	matchToken(KW_T, WRITE);
	matchToken(LPR_T, NO_ATTR);
	outputStatementPrime();							//outputVariableList();
	matchToken(RPR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);
	printf("%s\n", "PLATY: Output statement parsed");
}

void outputStatementPrime(void) {
	switch (lookahead.code) {
	case AVID_T:
		optionalVariableList();
		break;
	case SVID_T:
		optionalVariableList();
		break;
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		//printf("%s\n", "PLATY: Output variable list parsed");
		break;
	default:
		//printf("%s\n", "PLATY: Output variable list parsed");
		break;
	}
}

void outputVariableList(void) {
	switch (lookahead.code) {
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		printf("%s\n", "PLATY: Output list (string literal) parsed");
		break;
	default:
		optionalVariableList();
		break;
	}
}
/*************************************************************
 * Optional Variable List
 * <output statement> -> OUTPUT (<output list>);
<output_list> -> <opt_variable list> | STR_T


* FIRST(<output statement>) = {KW_T(WRITE)}
		FIRST(<output statementPrime>) = {AVID_T, SVID_T, STR_T, ϵ}

 ************************************************************/
void optionalVariableList(void) {

	variableList();
}

void preCondition(void) {
	switch (lookahead.code) {
	case KW_T:
		if (lookahead.attribute.intValue == TRUE) {
			matchToken(KW_T, TRUE);
		}
		else if (lookahead.attribute.keywordIndex == FALSE) {
			matchToken(KW_T, FALSE);
		}
		else {
			printError();
		}
		break;
	default:
		//printError();
		break;
	}
	printf("%s\n", "PLATY: Pre-condition parsed\n");
}
/*************************************************************
 * Relational Arithmetic Expression
 * <relational a_expression> -><primary a_relational expression> <primary a_relational expressionPrime>
<primary a_relational expressionPrime>    -> == <primary a_relational expression> | != <primary a_relational expression> | >   <primary a_relational expression> |  < <primary a_relational expression>



* FIRST(<relational a_expression>) = {AVID_T, FPL_T, INL_T}
		FIRST(<primary a_relational expressionPrime> ) = {LOG_OP_T(EQ), LOG_OP_T(NE), LOG_OP_T(GT), LOG_OP_T(LT)}


 ************************************************************/
void primaryArithmeticExpression(void) {
	switch (lookahead.code) {
	case AVID_T:
		matchToken(AVID_T, NO_ATTR);
		printf("%s\n", "PLATY: Primary arithmetic expression parsed");
		break;
	case FPL_T:
		matchToken(FPL_T, NO_ATTR);
		printf("%s\n", "PLATY: Primary arithmetic expression parsed");
		break;
	case INL_T:
		matchToken(INL_T, NO_ATTR);
		printf("%s\n", "PLATY: Primary arithmetic expression parsed");
		break;
	case LPR_T:
		matchToken(LPR_T, NO_ATTR);
		arithmeticExpression();
		matchToken(RPR_T, NO_ATTR);
		printf("%s\n", "PLATY: Primary arithmetic expression parsed");
		break;
	default:
		printError();
		break;
	}
}
/*************************************************************
 * Primary Arithmetic Relational Expression
 * <primary a_relational expression> ->  AVID_T  | FPL_T  | INL_T


* FIRST(<primary a_relational expression>) = {AVID_T, FPL_T, INL_T}

 ************************************************************/
void primaryRelationalArithmeticExpression(void) {
	switch (lookahead.code) {
	case AVID_T:
		matchToken(AVID_T, NO_ATTR);
		break;
	case FPL_T:
		matchToken(FPL_T, NO_ATTR);
		break;
	case INL_T:
		matchToken(INL_T, NO_ATTR);
		break;
	default:
		printError();
		break;
	}
	printf("%s\n", "PLATY: Primary Relational Arithmetic expression parsed");
}
/*************************************************************
 * Primary String Relational Expression
 * <primary s_relational expression><primary s_relational expressionPrime><primary s_relational expressionPrime> == <primary s_relational expression> | != <primary s_relational expression> | > <primary s_relational expression> | < <primary s_relational expression>



* FIRST(<primary s_relational expression>) = {SVID_T, STR_T}
		FIRST(<primary s_relational expressionPrime>) = {LOG_OP_T(EQ), LOG_OP_T(NE), LOG_OP_T(GT), LOG_OP_T(LT) }


 ************************************************************/
void primaryRelationalStringExpression(void) {
	primaryStringExpression();
	printf("%s\n", "PLATY: Primary Relational String expression parsed");
}
/*************************************************************
 * Primary String Expression
 * <primary string expression>    SVID_T  |  STR_T

* FIRST(<primary string expression>) = {SVID_T, STR_T}


 ************************************************************/
void primaryStringExpression(void) {
	switch (lookahead.code) {
	case SVID_T:
		matchToken(SVID_T, NO_ATTR);
		printf("%s\n", "PLATY: Primary String expression parsed");
		break;
	case STR_T:
		matchToken(STR_T, NO_ATTR);
		printf("%s\n", "PLATY: Primary String expression parsed");
		break;
	default:
		printError();
		break;
	}
}

void relationalArithmeticExpression(void) {

}

void relationalArithmeticOperator(void) {

}
/*************************************************************
 * Relational Arithmetic Expression
 * <relational expression> 
		  <relational a_expression>  | <relational s_expression>

* FIRST(<relational expression>) = {AVID_T, INL_T, FPL_T, SVID_T, STR_T}

 ************************************************************/
void relationalExpression(void) {
	switch (lookahead.code) {
	case AVID_T:
		primaryRelationalArithmeticExpression();
		//relationalStringOperator();
		primaryRelationalArithmeticExpressionPrime();
		break;
	case FPL_T:
		primaryRelationalArithmeticExpression();
		//relationalStringOperator();
		primaryRelationalArithmeticExpressionPrime();
		break;
	case INL_T:
		primaryRelationalArithmeticExpression();
		//relationalStringOperator();
		primaryRelationalArithmeticExpressionPrime();
		break;
	case SVID_T:
		primaryRelationalStringExpression();
		//relationalStringOperator();
		primaryRelationalStringExpressionPrime();
		break;
	case STR_T:
		primaryRelationalStringExpression();
		//relationalStringOperator();
		primaryRelationalStringExpressionPrime();
		break;
	default:
		printError();
		break;
	}
	printf("%s\n", "PLATY: Relational expression parsed");
}

void primaryRelationalArithmeticExpressionPrime(void) {
	if (lookahead.code == REL_OP_T) {
		if (lookahead.attribute.relationalOperator == EQ) {
			matchToken(REL_OP_T, EQ);
			primaryRelationalStringExpression();
		}
		if (lookahead.attribute.relationalOperator == NE) {
			matchToken(REL_OP_T, NE);
			primaryRelationalStringExpression();
		}
		if (lookahead.attribute.relationalOperator == LT) {
			matchToken(REL_OP_T, LT);
			primaryRelationalStringExpression();
		}
		if (lookahead.attribute.relationalOperator == GT) {
			matchToken(REL_OP_T, GT);
			primaryRelationalStringExpression();
		}
		else {
			printError();
		}
	}
}

void primaryRelationalStringExpressionPrime(void) {
	if (lookahead.code == REL_OP_T) {
		if (lookahead.attribute.relationalOperator == EQ) {
			matchToken(REL_OP_T, EQ);
			primaryRelationalStringExpression();
		}
		if (lookahead.attribute.relationalOperator == NE) {
			matchToken(REL_OP_T, NE);
			primaryRelationalStringExpression();
		}
		if (lookahead.attribute.relationalOperator == LT) {
			matchToken(REL_OP_T, LT);
			primaryRelationalStringExpression();
		}
		if (lookahead.attribute.relationalOperator == GT) {
			matchToken(REL_OP_T, GT);
			primaryRelationalStringExpression();
		}
		else {
			printError();
		}
	}
}

void primaryArithmeticRelationalExpressionPrime(void) {
	if (lookahead.code == REL_OP_T) {
		if (lookahead.attribute.relationalOperator == EQ) {
			matchToken(REL_OP_T, EQ);
			primaryRelationalArithmeticExpression();
		}
		if (lookahead.attribute.relationalOperator == NE) {
			matchToken(REL_OP_T, NE);
			primaryRelationalArithmeticExpression();
		}
		if (lookahead.attribute.relationalOperator == GT) {
			matchToken(REL_OP_T, GT);
			primaryRelationalArithmeticExpression();
		}
		if (lookahead.attribute.relationalOperator == LT) {
			matchToken(REL_OP_T, LT);
			primaryRelationalArithmeticExpression();
		}
		else {
			printError();
		}
	}
}

void relationalStringExpression(void) {

}

void relationalStringOperator(void) {
	if (lookahead.code == REL_OP_T) {
		matchToken(REL_OP_T, lookahead.attribute.logicalOperator);
	}
	else {
		printError();
	}
}

void slectionStatement(void) {
	matchToken(KW_T, IF);
	preCondition();
	matchToken(LPR_T, NO_ATTR);
	conditionalExpression();
	matchToken(RPR_T, NO_ATTR);

	matchToken(KW_T, THEN);
	matchToken(LBR_T, NO_ATTR);
	optionalStatements();
	matchToken(RBR_T, NO_ATTR);

	matchToken(KW_T, ELSE);
	matchToken(LBR_T, NO_ATTR);
	optionalStatements();
	matchToken(RBR_T, NO_ATTR);
	matchToken(EOS_T, NO_ATTR);

	printf("%s\n", "PLATY: Selection statement parsed");
}

void statement(void) {
	switch (lookahead.code) {
	case AVID_T:
		assignmentStatement();
		break;

	case SVID_T:
		assignmentStatement();
		break;
	case KW_T:
		if (lookahead.attribute.intValue == IF) {
			slectionStatement();
			break;
		}
		if (lookahead.attribute.intValue == WHILE) {
			iterationStatement();
			break;
		}
		if (lookahead.attribute.intValue == READ) {
			inputStatement();
			break;
		}
		if (lookahead.attribute.intValue == WRITE) {
			outputStatement();
			break;
		}
		break;

	default:
		printError();
		break;
	}
	printf("PLATY: Statement parsed\n");
}


void statementsPrime(void) {
	switch (lookahead.code) {
	case AVID_T:
		statement();
		statementsPrime();
		break;
	case SVID_T:
		statement();
		statementsPrime();
		break;
	case KW_T:
		if (lookahead.attribute.keywordIndex == IF ||
			lookahead.attribute.keywordIndex == WHILE ||
			lookahead.attribute.keywordIndex == READ ||
			lookahead.attribute.keywordIndex == WRITE) {
			statement();
			statementsPrime();
		}
		break;
	}

}

void stringExpression(void) {
	primaryStringExpression();
	stringExpressionPrime();
	printf("%s\n", "PLATY: String expression parsed");
}

void stringExpressionPrime(void) {
	if (lookahead.code == SCC_OP_T) {
		matchToken(SCC_OP_T, NO_ATTR);
		primaryStringExpression();
		stringExpressionPrime();
	}
}

void variableIdentifier(void) {
	switch (lookahead.code) {
	case AVID_T:
		matchToken(AVID_T, NO_ATTR);
		break;
	case SVID_T:
		matchToken(SVID_T, NO_ATTR);
		break;
	default:
		printError();
		break;
	}
}

void variableListPrime(void) {
	switch (lookahead.code) {
	case COM_T:
		matchToken(COM_T, NO_ATTR);
		variableIdentifier();
		variableListPrime();
		break;
	default:
		break;
	}
}





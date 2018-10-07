#include "lexical_analyzer.h"
#include "data.h"
#include "token.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // Declares isalpa, isdigit, isalnum

/* ************************************************************************** */
/* Enumarations, Typename Aliases, Helpers Structs ************************** */
/* ************************************************************************** */

typedef enum {
    ALPHA,   // a, b, .. , z, A, B, .. Z
    DIGIT, // 0, 1, .. , 9
    SPECIAL, // '>', '=', , .. , ';', ':'
    INVALID  // Invalid symbol
} SymbolType;

/**
 * Following struct is recommended to use to keep track of the current state
 * .. of the lexer, and modify the state in other functions by passing pointer
 * .. to the state as argument.
 * */
typedef struct {
    int lineNum;         // the line number currently being processed
    int charInd;         // the index of the character currently being processed
    char* sourceCode;    // null-terminated source code string
    LexErr lexerError;   // LexErr to be filled when Lexer faces an error
    TokenList tokenList; // list of tokens
} LexerState;

/* ************************************************************************** */
/* Declarations ************************************************************* */
/* ************************************************************************** */

/**
 * Initializes the LexerState with the given null-terminated source code string.
 * Sets the other fields of the LexerState to their inital values.
 * Shallow copying is done for the source code field.
 * */
void initLexerState(LexerState*, char* sourceCode);

/**
 * Returns 1 if the given character is valid.
 * Returns 0 otherwise.
 * */
int isCharacterValid(char);

/**
 * Returns 1 if the given character is one of the special symbols of PL/0,
 * .. such as '/', '=', ':' or ';'.
 * Returns 0 otherwise.
 * */
int isSpecialSymbol(char);

/**
 * Returns the symbol type of the given character.
 * */
SymbolType getSymbolType(char);

/**
 * Checks if the given symbol is one of the reserved token.
 * If yes, returns the numerical value assigned to the corresponding token.
 * If not, returns -1.
 * For example, calling the function with symbol "const" returns 28.
 * */
int checkReservedTokens(char* symbol);

/**
 * Deterministic-finite-automaton to be entered when an alpha character is seen.
 * Simulating a state machine, consumes the source code and changes the state
 * .. of the lexer (LexerState) as required. Possibly, adds new tokens to the
 * .. token list field of the LexerState.
 * If an error is encountered, sets the LexErr field of LexerState, sets the
 * .. line number field and returns.
 * */
void DFA_Alpha(LexerState*);

/**
 * Deterministic-finite-automaton to be entered when a digit character is seen.
 * Simulating a state machine, consumes the source code and changes the state
 * .. of the lexer (LexerState) as required. Possibly, adds new tokens to the
 * .. token list field of the LexerState.
 * If an error is encountered, sets the LexErr field of LexerState, sets the
 * .. line number field and returns.
 * */
void DFA_Digit(LexerState*);

/**
 * Deterministic-finite-automaton to be entered when a special character is seen.
 * Simulating a state machine, consumes the source code and changes the state
 * .. of the lexer (LexerState) as required. Possibly, adds new tokens to the
 * .. token list field of the LexerState.
 * If an error is encountered, sets the LexErr field of LexerState, sets the
 * .. line number field and returns.
 * */
void DFA_Special(LexerState*);

/* ************************************************************************** */
/* Definitions ************************************************************** */
/* ************************************************************************** */

void initLexerState(LexerState* lexerState, char* sourceCode)
{
    lexerState->lineNum = 0;
    lexerState->charInd = 0;
    lexerState->sourceCode = sourceCode;
    lexerState->lexerError = NONE;
    
    initTokenList(&lexerState->tokenList);
}

int isCharacterValid(char c)
{
    return isalnum(c) || isspace(c) || isSpecialSymbol(c);
}

int isSpecialSymbol(char c)
{
    return c == '+' || c == '-' || c == '*' || c == '/' ||
           c == '(' || c == ')' || c == '=' || c == ',' ||
           c == '.' || c == '<' || c == '>' || c == ';' ||
           c == ':';
}

SymbolType getSymbolType(char c)
{
         if(isalpha(c))         return ALPHA;
    else if(isdigit(c))         return DIGIT;
    else if(isSpecialSymbol(c)) return SPECIAL;
    else                        return INVALID;
}

int checkReservedTokens(char* symbol)
{
    for(int i = firstReservedToken; i <= lastReservedToken; i++)
    {
        if( !strcmp(symbol, tokens[i]) )
        {
            // Symbol is the reserved token at index i.
            return i;
        }
    }

    // Symbol is not found among the reserved tokens
    return -1;
}


/**
 * Deterministic-finite-automaton to be entered when an alpha character is seen.
 * Simulating a state machine, consumes the source code and changes the state
 * .. of the lexer (LexerState) as required. Possibly, adds new tokens to the
 * .. token list field of the LexerState.
 * If an error is encountered, sets the LexErr field of LexerState, sets the
 * .. line number field and returns.
 * */
void DFA_Alpha(LexerState* lexerState)
{
    // There are two possible cases for symbols starting with alpha:
    // Case.1) A reversed token (a reserved word or 'odd')
    // Case.2) An ident

    // In both cases, symbol should not exceed 11 characters.
    // Read 11 or less alpha-numeric characters
    // If it exceeds 11 alnums, fill LexerState error and return
    // Otherwise, try to recognize if the symbol is reserved.
    //   If yes, tokenize by one of the reserved symbols
    //   If not, tokenize as ident.

    // For adding a token to tokenlist, you could create a token, fill its 
    // .. fields as required and use the following call:
    // addToken(&lexerState->tokenList, token);

    /**
     * TODO
     * Implement this function
     * */

    // TODO: Remove the below message after your implementation
    // Until implementing, let's just consume the current character and return.
    char c = lexerState->sourceCode[lexerState->charInd];
    int length = 0;
    char exp[100];
    Token token;
    
    for (int i = 0; isalpha(c); i++)
    {
        exp[i] = c;
        lexerState->charInd++;
        length++;
        c = lexerState->sourceCode[lexerState->charInd];
    }
    
    if (length > 11)
    {
        lexerState->lexerError = 1;
        lexerState->charInd++;
        return;
    }
    
    else
    {
        if (strcmp(exp, tokens[beginsym]))
        {
            token.id = beginsym;
            strcpy(token.lexeme, tokens[beginsym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[endsym]))
        {
            token.id = endsym;
            strcpy(token.lexeme, tokens[endsym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[ifsym]))
        {
            token.id = ifsym;
            strcpy(token.lexeme, tokens[ifsym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[ifsym]))
        {
            token.id = ifsym;
            strcpy(token.lexeme, tokens[ifsym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[thensym]))
        {
            token.id = thensym;
            strcpy(token.lexeme, tokens[thensym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[whilesym]))
        {
            token.id = whilesym;
            strcpy(token.lexeme, tokens[whilesym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[dosym]))
        {
            token.id = dosym;
            strcpy(token.lexeme, tokens[dosym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[callsym]))
        {
            token.id = callsym;
            strcpy(token.lexeme, tokens[callsym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[constsym]))
        {
            token.id = constsym;
            strcpy(token.lexeme, tokens[constsym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[varsym]))
        {
            token.id = varsym;
            strcpy(token.lexeme, tokens[varsym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[procsym]))
        {
            token.id = procsym;
            strcpy(token.lexeme, tokens[procsym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[writesym]))
        {
            token.id = writesym;
            strcpy(token.lexeme, tokens[writesym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[readsym]))
        {
            token.id = readsym;
            strcpy(token.lexeme, tokens[readsym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[elsesym]))
        {
            token.id = thensym;
            strcpy(token.lexeme, tokens[thensym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[oddsym]))
        {
            token.id = oddsym;
            strcpy(token.lexeme, tokens[oddsym]);
            addToken(&lexerState->tokenList, token);
        }
        else
        {
            token.id = identsym;
            strcpy(token.lexeme, exp);
            addToken(&lexerState->tokenList, token);
        }
    }
    
    lexerState->charInd++;
    return;
}


/**
 * Deterministic-finite-automaton to be entered when a digit character is seen.
 * Simulating a state machine, consumes the source code and changes the state
 * .. of the lexer (LexerState) as required. Possibly, adds new tokens to the
 * .. token list field of the LexerState.
 * If an error is encountered, sets the LexErr field of LexerState, sets the
 * .. line number field and returns.
 * */
void DFA_Digit(LexerState* lexerState)
{
    // There are three cases for symbols starting with number:
    // Case.1) It is a well-formed number
    // Case.2) It is an ill-formed number exceeding 5 digits - Lexer Error!
    // Case.3) It is an ill-formed variable name starting with digit - Lexer Error!

    // Tokenize as numbersym only if it is case 1. Otherwise, set the required
    // .. fields of lexerState to corresponding LexErr and return.

    // For adding a token to tokenlist, you could create a token, fill its 
    // .. fields as required and use the following call:
    // addToken(&lexerState->tokenList, token);

    /**
     * TODO
     * Implement this function
     * */

    // TODO: Remove the below message after your implementation
    // Until implementing, let's just consume the current character and return.
    int length = 1;
    char c = lexerState->sourceCode[lexerState->charInd];
    char digits[100];
    for (int i=0; c <= '9' && c >= 0 ; i++)
    {
        digits[i] = c;
        length++;
        lexerState->charInd++;
        c = lexerState->sourceCode[lexerState->charInd];
        
    }
    
    if (length > 5)
    {
        lexerState->lexerError = 1;
        return;
    }
    
    else if (isalpha(c))
    {
        lexerState->lexerError = 1;
        return;
    }
    
    else
    {
        Token token;
        token.id = numbersym;
        strcpy(token.lexeme, digits);
        addToken(&lexerState->tokenList, token);
    }
    
    lexerState->charInd++;
    return;
}

void DFA_Special(LexerState* lexerState)
{
    Token token;
    // There are three cases for symbols starting with special:
    // Case.1: Beginning of a comment: "/*"
    // Case.2: Two character special symbol: "<>", "<=", ">=", ":="
    // Case.3: One character special symbol: "+", "-", "(", etc.

    // For case.1, you are recommended to consume all the characters regarding
    // .. the comment, and return. This way, lexicalAnalyzer() func can decide
    // .. what to do with the next character.

    // For case.2 and case.3, you could consume the characters, add the 
    // .. corresponding token to the tokenlist of lexerState, and return.

    // For adding a token to tokenlist, you could create a token, fill its 
    // .. fields as required and use the following call:
    // addToken(&lexerState->tokenList, token);

    switch (lexerState->sourceCode[lexerState->charInd])
    {
        case '+':
            token.id = plussym;
            token.lexeme[1] = *tokens[plussym];
            addToken(&lexerState->tokenList, token);
            break;
        case '-':
            token.id = minussym;
            token.lexeme[1] = *tokens[minussym];
            addToken(&lexerState->tokenList, token);
            break;
        case '(':
            token.id = lparentsym;
            token.lexeme[1] = *tokens[lparentsym];
            addToken(&lexerState->tokenList, token);
            break;
        case ')':
            token.id = rparentsym;
            token.lexeme[1] = *tokens[rparentsym];
            addToken(&lexerState->tokenList, token);
            break;
        case '*':
            token.id = multsym;
            token.lexeme[1] = *tokens[multsym];
            addToken(&lexerState->tokenList, token);
            break;
        case '=':
            token.id = eqsym;
            token.lexeme[1] = *tokens[eqsym];
            addToken(&lexerState->tokenList, token);
            break;
        case ',':
            token.id = commasym;
            token.lexeme[1] = *tokens[commasym];
            addToken(&lexerState->tokenList, token);
            break;
        case ';':
            token.id = semicolonsym;
            token.lexeme[1] = *tokens[semicolonsym];
            addToken(&lexerState->tokenList, token);
            break;
        case '.':
            token.id = periodsym;
            token.lexeme[1] = *tokens[periodsym];
            addToken(&lexerState->tokenList, token);
            break;
        case ':':
            if (lexerState->sourceCode[lexerState->charInd + 1] == '=')
            {
                token.id = becomessym;
                token.lexeme[1] = ':';
                token.lexeme[2] = '=';
                addToken(&lexerState->tokenList, token);
                lexerState->charInd++;
            }
            break;
        case '>':
            if (lexerState->sourceCode[lexerState->charInd + 1] == '=')
            {
                token.id = geqsym;
                token.lexeme[1] = '>';
                token.lexeme[2] = '=';
                addToken(&lexerState->tokenList, token);
                lexerState->charInd++;
            }
            else
            {
                token.id = gtrsym;
                token.lexeme[1] = '>';
                addToken(&lexerState->tokenList, token);
            }
            break;
            
        case '<':
            if (lexerState->sourceCode[lexerState->charInd + 1] == '=')
            {
                token.id = leqsym;
                token.lexeme[1] = '<';
                token.lexeme[2] = '=';
                addToken(&lexerState->tokenList, token);
                lexerState->charInd++;
            }
            if (lexerState->sourceCode[lexerState->charInd + 1] == '>')
            {
                token.id = neqsym;
                token.lexeme[1] = '<';
                token.lexeme[2] = '=';
                addToken(&lexerState->tokenList, token);
                lexerState->charInd++;
            }
            
            else
            {
                token.id = lessym;
                token.lexeme[1] = '<';
                addToken(&lexerState->tokenList, token);
            }
            break;
        case '/':
            if (lexerState->sourceCode[lexerState->charInd + 1] == '*')
            {
                lexerState->charInd++;
                for (int i = 0; lexerState->sourceCode[lexerState->charInd] != '/'; i++)
                {
                    lexerState->charInd++;
                }
            }
            
            else
            {
                token.id = slashsym;
                token.lexeme[1] = '/';
                addToken(&lexerState->tokenList, token);
            }
            break;
            
        default:
            printf("NO VALID SPECIAL CHARACTER %c \n", lexerState->sourceCode[lexerState->charInd]);
            break;
    }
    return;
}

LexerOut lexicalAnalyzer(char* sourceCode)
{
    if(!sourceCode)
    {
        fprintf(stderr, "ERROR: Null source code string passed to lexicalAnalyzer()\n");
        
        LexerOut lexerOut;
        lexerOut.lexerError = NO_SOURCE_CODE;
        lexerOut.errorLine = -1;

        return lexerOut;
    }

    // Create & init lexer state
    LexerState lexerState;
    initLexerState(&lexerState, sourceCode);

    // While not end of file, and, there is no lexer error
    // .. continue lexing
    while( lexerState.sourceCode[lexerState.charInd] != '\0' &&
        lexerState.lexerError == NONE )
    {
        char currentSymbol = lexerState.sourceCode[lexerState.charInd];

        // Skip spaces or new lines until an effective character is seen
        while(currentSymbol == ' ' || currentSymbol == '\n')
        {
            // Advance line number if required
            if(currentSymbol == '\n')
                lexerState.lineNum++;

            // Advance to the following character
            currentSymbol = lexerState.sourceCode[++lexerState.charInd];
        }

        // After recognizing spaces or new lines, make sure that the EOF was
        // .. not reached. If it was, break the loop.
        if(lexerState.sourceCode[lexerState.charInd] == '\0')
        {
            break;
        }

        // Take action depending on the current symbol's type
        switch(getSymbolType(currentSymbol))
        {
            case ALPHA:
                DFA_Alpha(&lexerState);
                break;
            case DIGIT:
                DFA_Digit(&lexerState);
                break;
            case SPECIAL:
                DFA_Special(&lexerState);
                break;
            case INVALID:
                lexerState.lexerError = INV_SYM;
                break;
        }
    }

    // Prepare LexerOut to be returned
    LexerOut lexerOut;

    if(lexerState.lexerError != NONE)
    {
        // Set LexErr
        lexerOut.lexerError = lexerState.lexerError;

        // Set the number of line the error encountered
        lexerOut.errorLine = lexerState.lineNum;

        lexerOut.tokenList = lexerState.tokenList;
    }
    else
    {
        // No error!
        lexerOut.lexerError = NONE;
        lexerOut.errorLine = -1;
        
        // Copy the token list

        // The scope of LexerState ends here. The ownership of the tokenlist
        // .. is being passed to LexerOut. Therefore, neither deletion of the
        // .. tokenlist nor deep copying of the tokenlist is required.
        lexerOut.tokenList = lexerState.tokenList;
    }

    return lexerOut;
}

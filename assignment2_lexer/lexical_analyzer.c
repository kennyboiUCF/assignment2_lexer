/*
 COP3402
 LEXER
 ASSIGNMENT #2
 Kenneth J. Rosario Rodríguez
 
 */

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
    char c = lexerState->sourceCode[lexerState->charInd]; // c is the current character
    int length = 0; //length of string exp
    char exp[11]; // exp will be used to store characters for words there
    Token token; // the token that will be extracted
    
    for (int i = 0; isalpha(c); i++) //compile all the words together and get the length.
    {
        if (length > 11) //if its longer than 11, you're outta' here.
        {
            lexerState->lexerError = NAME_TOO_LONG;
            lexerState->charInd++;
            return;
        }
        
        exp[i] = c;
        lexerState->charInd++;
        length++;
        c = lexerState->sourceCode[lexerState->charInd];
    }
    
        if (strcmp(exp, tokens[beginsym]) == 0)
        {
            token.id = beginsym;
            strcpy(token.lexeme, tokens[beginsym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[endsym]) == 0)
        {
            token.id = endsym;
            strcpy(token.lexeme, tokens[endsym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[ifsym]) == 0)
        {
            token.id = ifsym;
            strcpy(token.lexeme, tokens[ifsym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[ifsym]) == 0)
        {
            token.id = ifsym;
            strcpy(token.lexeme, tokens[ifsym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[thensym]) == 0)
        {
            token.id = thensym;
            strcpy(token.lexeme, tokens[thensym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[whilesym]) == 0)
        {
            token.id = whilesym;
            strcpy(token.lexeme, tokens[whilesym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[dosym]) == 0)
        {
            token.id = dosym;
            strcpy(token.lexeme, tokens[dosym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[callsym]) == 0)
        {
            token.id = callsym;
            strcpy(token.lexeme, tokens[callsym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[constsym]) == 0)
        {
            token.id = constsym;
            strcpy(token.lexeme, tokens[constsym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[varsym]) == 0)
        {
            token.id = varsym;
            strcpy(token.lexeme, tokens[varsym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[procsym]) == 0)
        {
            token.id = procsym;
            strcpy(token.lexeme, tokens[procsym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[writesym]) == 0)
        {
            token.id = writesym;
            strcpy(token.lexeme, tokens[writesym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[readsym]) == 0)
        {
            token.id = readsym;
            strcpy(token.lexeme, tokens[readsym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[elsesym]) == 0)
        {
            token.id = thensym;
            strcpy(token.lexeme, tokens[thensym]);
            addToken(&lexerState->tokenList, token);
        }
        else if (strcmp(exp, tokens[oddsym]) == 0)
        {
            token.id = oddsym;
            strcpy(token.lexeme, tokens[oddsym]);
            addToken(&lexerState->tokenList, token);
        }
        else // if word isnt a reserved word, put it in as a indent.
        {
            token.id = identsym;
            strcpy(token.lexeme, exp);
            addToken(&lexerState->tokenList, token);
        }
    
    
    lexerState->charInd++; // on to the next one!
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
    
    int length = 0; //starts at 0 but indicates length of number string.
    char c = lexerState->sourceCode[lexerState->charInd];
    char digits[5]; // string of numbers
    
    for (int i=0; isdigit(c); i++) // compile all numbers into the char array digits
    {
        if (length > 5) // if its longer than 5 then it wont work.
        {
            lexerState->lexerError = NUM_TOO_LONG;
            return;
        }
        
        else if (isalpha(c)) // if that new character is alpha-numeric, that won't work either.
        {
            lexerState->lexerError = NONLETTER_VAR_INITIAL;
            return;
        }
        
        else
        {
            digits[i] = c;
            length++;
            lexerState->charInd++;
            c = lexerState->sourceCode[lexerState->charInd];
        }
        
    }
    
    Token token;
    token.id = numbersym;
    strcpy(token.lexeme, digits);
    addToken(&lexerState->tokenList, token);
    
    
    lexerState->charInd++; // on to the next one before you leave.
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
    
    char c = lexerState->sourceCode[lexerState->charInd]; // c is the current special symbol.

    switch (c) // lots of cases in this one.
    {
        case '+':
            token.id = plussym;
            strcpy(token.lexeme, tokens[plussym]);
            addToken(&lexerState->tokenList, token);
            break;
        case '-':
            token.id = minussym;
            strcpy(token.lexeme, tokens[minussym]);
            addToken(&lexerState->tokenList, token);
            break;
        case '(':
            token.id = lparentsym;
            strcpy(token.lexeme, tokens[lparentsym]);
            addToken(&lexerState->tokenList, token);
            break;
        case ')':
            token.id = rparentsym;
            strcpy(token.lexeme, tokens[rparentsym]);
            addToken(&lexerState->tokenList, token);
            break;
        case '*':
            token.id = multsym;
            strcpy(token.lexeme, tokens[multsym]);
            addToken(&lexerState->tokenList, token);
            break;
        case '=':
            token.id = eqsym;
            strcpy(token.lexeme, tokens[eqsym]);
            addToken(&lexerState->tokenList, token);
            break;
        case ',':
            token.id = commasym;
            strcpy(token.lexeme, tokens[commasym]);
            addToken(&lexerState->tokenList, token);
            break;
        case ';':
            token.id = semicolonsym;
            strcpy(token.lexeme, tokens[semicolonsym]);
            addToken(&lexerState->tokenList, token);
            break;
        case '.':
            token.id = periodsym;
            strcpy(token.lexeme, tokens[periodsym]);
            addToken(&lexerState->tokenList, token);
            break;
        case ':': // interesting case. : is not used in any other context so it doesnt have it's separate symbol.
            if (lexerState->sourceCode[lexerState->charInd + 1] == '=')
            {
                token.id = becomessym;
                strcpy(token.lexeme, tokens[becomessym]);
                lexerState->charInd++; // you'll see these in other two char cases. I just judged which char came next and forwarded the index counter to that specific char. there is one at the end as well for all cases to move on to the next one, so it doesnt stay and count the second part.
            }
            break;
        case '>':
            if (lexerState->sourceCode[lexerState->charInd + 1] == '=')
            {
                token.id = geqsym;
                strcpy(token.lexeme, tokens[geqsym]);
                addToken(&lexerState->tokenList, token);
                lexerState->charInd++;
            }
            else
            {
                token.id = gtrsym;
                strcpy(token.lexeme, tokens[gtrsym]);
                addToken(&lexerState->tokenList, token);
            }
            break;
            
        case '<':
            if (lexerState->sourceCode[lexerState->charInd + 1] == '=')
            {
                token.id = leqsym;
                strcpy(token.lexeme, tokens[leqsym]);
                addToken(&lexerState->tokenList, token);
                lexerState->charInd++;
            }
            if (lexerState->sourceCode[lexerState->charInd + 1] == '>')
            {
                token.id = neqsym;
                strcpy(token.lexeme, tokens[neqsym]);
                addToken(&lexerState->tokenList, token);
                lexerState->charInd++;
            }
            
            else
            {
                token.id = lessym;
                strcpy(token.lexeme, tokens[lessym]);
                addToken(&lexerState->tokenList, token);
            }
            break;
        case '/':
            if (lexerState->sourceCode[lexerState->charInd + 1] == '*')
            {
                lexerState->charInd++; // move to the '*' character in the comment.
                for (int i = 0; lexerState->sourceCode[lexerState->charInd] != '/'; i++)
                {
                    lexerState->charInd++; // comments dont count, so i'm just scrolling past all the "worthless" (not in a bad way) text until i get to that last '/' character.
                }
            }
            
            else //it's just a lonely slash.
            {
                token.id = slashsym;
                strcpy(token.lexeme, tokens[slashsym]);
                addToken(&lexerState->tokenList, token);
            }
            break;
            
        default:
            printf("NO VALID SPECIAL CHARACTER %c \n", lexerState->sourceCode[lexerState->charInd]);
            break;
    }
    lexerState->charInd++; // move on to the next one to avoid any redundancies.
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

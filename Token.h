#ifndef TOKEN_H
#define TOKEN_H

#include <string>
//#include "LongNumber.h"

enum class TokenName
{
    PLUS,
    MINUS,
    MULTIPLY,
    OPERAND,
    LEFT_BRACKET,
    RIGHT_BRACKET
};

struct Token
{
    TokenName name;
    LongNumber value;
};

#endif

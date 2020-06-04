
#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <string>
#include <algorithm>
#include <vector>
#include "LongNumber.h"
#include "Token.h"

class Calculator
{
public:
    static LongNumber evaluate(const std::string&);
    static std::vector<Token> tokenize(const std::string&);
    static std::vector<Token> infix2rpn(std::vector<Token>&);
private:
   
    static LongNumber evaluateRpn(const std::vector<Token>&);
    static int getPriority(const Token&);
};

#endif

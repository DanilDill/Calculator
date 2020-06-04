
#include <cctype>
#include <exception>
#include "Calculator.h"

LongNumber Calculator::evaluate(const std::string& input)
{
    try
    {
        auto infix = tokenize(input);
        auto rpn = infix2rpn(infix);
        return evaluateRpn(rpn);
    }
    catch (...)
    {
        throw;
    }
}



std::vector<Token> Calculator::tokenize(const std::string& input)
{
    std::vector<Token> tokens;

    std::string buffer;
    for (size_t i = 0; i < input.size(); ++i)
    {
        bool isPlus = input[i] == '+';
        bool isMinus = input[i] == '-';
        bool isMultiply = input[i] == '*';
        bool isOperator = isPlus || isMinus || isMultiply;
        bool isSeparator = input[i] == ' ';
        bool isDigit = isdigit(input[i]);
        bool isLeftBracket = input[i] == '(';
        bool isRightBracket = input[i] == ')';

        if ((isSeparator || isOperator || isLeftBracket || isRightBracket
            ) && !buffer.empty())
        {
            Token token;
            token.name = TokenName::OPERAND;
            token.value = buffer;
            buffer.clear();
            tokens.push_back(token);
        }

        if (isLeftBracket || isRightBracket || isOperator)
        {
            Token token;
            TokenName tokenName;
            if (isLeftBracket)
            {
                tokenName = TokenName::LEFT_BRACKET;
            }
            else if (isRightBracket)
            {
                tokenName = TokenName::RIGHT_BRACKET;
            }
            else if (isPlus)
            {
                tokenName = TokenName::PLUS;
            }
            else if (isMinus)
            {
                tokenName = TokenName::MINUS;
            }
            else
            {
                tokenName = TokenName::MULTIPLY;
            }
            token.name = tokenName;
            tokens.push_back(token);
        }
        else if (isDigit)
        {
            buffer += input[i];
        }
        
        if (!isOperator && !isDigit && !isSeparator
            && !isLeftBracket && !isRightBracket)
        {
            throw std::invalid_argument("Wrong input");
        }
    }
    if (!buffer.empty())
    {
        Token token;
        token.name = TokenName::OPERAND;
        token.value = buffer;
        buffer.clear();
        tokens.push_back(token);
    }
    return tokens;
}



std::vector<Token> Calculator::infix2rpn(std::vector<Token>& infix)
{ 
    for (size_t i = 0; i < infix.size(); ++i)
    {
        
        if (i + 1 < infix.size() && i + 2 < infix.size())
        {
            if (infix[i].name == TokenName::MULTIPLY &&
                infix[i + 1].name == TokenName::LEFT_BRACKET &&
                infix[i + 2].name == TokenName::MINUS)
            {
                infix[i + 3].value.m_sign = Sign::MINUS;
                auto begin = infix.cbegin();
                infix.erase(begin + i+2);
            }
            else if (infix[i].name == TokenName::PLUS &&
                infix[i + 1].name == TokenName::LEFT_BRACKET &&
                infix[i + 2].name == TokenName::MINUS)
            {
                infix[i + 3].value.m_sign = Sign::MINUS;
                auto begin = infix.cbegin();
                infix.erase(begin + i + 2);
            }
            else if (infix[i].name == TokenName::MULTIPLY &&
                infix[i + 1].name == TokenName::LEFT_BRACKET &&
                infix[i + 2].name == TokenName::PLUS)
            {
                infix[i + 3].value.m_sign = Sign::PLUS;
                auto begin = infix.cbegin();
                infix.erase(begin + i + 2);
            }
        }

    }
    std::vector<Token> rpn;
    std::stack<Token> operatorsAndBrackets;

    for (auto& token : infix)
    {
        if (token.name == TokenName::OPERAND)
        {
            rpn.push_back(token);
        }
        else if (token.name == TokenName::LEFT_BRACKET)
        {
            operatorsAndBrackets.push(token);
        }
        else if (token.name == TokenName::RIGHT_BRACKET)
        {
            if (operatorsAndBrackets.empty())
            {
                throw std::invalid_argument("Wrong input");
            }
            while (operatorsAndBrackets.top().name != TokenName::LEFT_BRACKET)
            {
                rpn.push_back(operatorsAndBrackets.top());
                operatorsAndBrackets.pop();

                if (operatorsAndBrackets.empty())
                {
                    throw std::invalid_argument("Wrong input");
                }
            }
            operatorsAndBrackets.pop();
        }
        else if (token.name == TokenName::PLUS
            || token.name == TokenName::MINUS
            || token.name == TokenName::MULTIPLY)
        {
            while (!operatorsAndBrackets.empty()
                && getPriority(operatorsAndBrackets.top())
                >= getPriority(token))
            {
                rpn.push_back(operatorsAndBrackets.top());
                operatorsAndBrackets.pop();
            }
            operatorsAndBrackets.push(token);
        }
    }
    while (!operatorsAndBrackets.empty())
    {
        auto top = operatorsAndBrackets.top().name;
        if (!(top == TokenName::PLUS
            || top == TokenName::MINUS
            || top == TokenName::MULTIPLY))
        {
            throw std::invalid_argument("Wrong input");
        }
        rpn.push_back(operatorsAndBrackets.top());
        operatorsAndBrackets.pop();
    }

    return rpn;
}



LongNumber Calculator::evaluateRpn(const std::vector<Token>& rpn)
{
    std::stack<Token> operands;
    for (auto& token : rpn)
    {
        if (token.name == TokenName::OPERAND)
        {
            operands.push(token);
        }
        else if (token.name == TokenName::PLUS
            || token.name == TokenName::MINUS
            || token.name == TokenName::MULTIPLY)
        {
            if (operands.size() < 2)
            {
                throw std::invalid_argument("Wrong input");
            }
            LongNumber right(operands.top().value);
            operands.pop();
            LongNumber left(operands.top().value);
            operands.pop();
            LongNumber temp;
            switch (token.name)
            {
            case TokenName::PLUS: temp = left + right; break;
            case TokenName::MINUS: temp = left - right; break;
            case TokenName::MULTIPLY: temp = left * right; break;
            default:
                break;
            }
            Token tokenOperand;
            tokenOperand.name = TokenName::OPERAND;
            tokenOperand.value = temp;
            operands.push(tokenOperand);
        }
    }

    return operands.top().value;
}



int Calculator::getPriority(const Token& token)
{
    switch (token.name)
    {
    case TokenName::MULTIPLY: return 2;
    case TokenName::PLUS:
    case TokenName::MINUS: return 1;
    default: return 0;
    }
}

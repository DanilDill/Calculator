
#ifndef LONG_NUMBER_H
#define LONG_NUMBER_H

#include <string>
#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>
#include <stack>

enum class Sign
{
    MINUS,
    PLUS
};

class LongNumber
{
public:
    LongNumber();
    LongNumber::LongNumber(const LongNumber& other);
    LongNumber::LongNumber(const std::string& str);
    ~LongNumber();
    void Read(const std::string& str);
    std::string Print() const;
    
    static int compare(const LongNumber left, const LongNumber right);

private:
    static LongNumber multiplyByDigit(LongNumber& left,
        unsigned char right);
    void trim();
    friend class Calculator;
    friend LongNumber operator+(LongNumber left,  LongNumber& right);
    friend LongNumber operator-(LongNumber& left, LongNumber& right);
    friend LongNumber operator*(LongNumber& left, LongNumber& right);
    
    std::vector<unsigned char> m_longNumber;
    Sign m_sign = Sign::PLUS;
};

#endif

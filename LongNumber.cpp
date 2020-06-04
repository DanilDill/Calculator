
#include "LongNumber.h"

LongNumber::LongNumber()
{
}



LongNumber::LongNumber(const LongNumber& other) :
    m_longNumber(other.m_longNumber),
    m_sign(other.m_sign)
{
}



LongNumber::LongNumber(const std::string& str)
{
    Read(str);
}



LongNumber::~LongNumber()
{
}



void LongNumber::Read(const std::string& str)
{
    std::string::const_iterator iter = str.cbegin();
    if (!str.empty() && str[0] == '-')
    {
        m_sign = Sign::MINUS;
        ++iter;
    }
    /*for (; iter != str.cend(); ++iter)
    {
        m_longNumber.insert(m_longNumber.begin(), *iter - '0');
    }*/

    std::for_each(iter, str.cend(),
        [&](auto value)
    { m_longNumber.insert(m_longNumber.begin(), value - '0'); });
}



std::string LongNumber::Print() const
{
    std::string number;
    
    if (m_sign == Sign::MINUS)
    {
        number += '-';
    }
    if (m_longNumber.empty())
    {
        number = '0';
    }
    std::for_each(m_longNumber.rbegin(), m_longNumber.rend(),
        [&](unsigned char value) { number += static_cast<int>(value) + '0'; });
    return number;
}



 int LongNumber::compare(const LongNumber left, LongNumber right) 
{
     size_t leftSize = left.m_longNumber.size();
     size_t rightSize = right.m_longNumber.size();
    if (leftSize<rightSize)
    {
        return -1;
    }
    if (leftSize > rightSize)
    {
        return 1;
    }
    for (size_t i = 0; i < leftSize; ++i)
    {
        if (left.m_longNumber[leftSize - i-1] < right.m_longNumber[leftSize - i-1])
        {
            return -1;
        }
        if (left.m_longNumber[leftSize - i-1] > right.m_longNumber[leftSize - i-1])
        {
            return 1;
        }
    }
    return 0;
}

LongNumber operator+(LongNumber left, LongNumber& right)
{   
    LongNumber result;
    size_t leftSize = left.m_longNumber.size();
    size_t rightSize = right.m_longNumber.size();
    size_t maxlen = std::max(leftSize, rightSize);
    if (left.m_sign == Sign::MINUS && right.m_sign == Sign::PLUS)
    {
        return right - left;
    }
    else if (left.m_sign == Sign::PLUS && right.m_sign == Sign::MINUS)
    {
        right.m_sign = Sign::PLUS;
        return left - right;
    }
    else if (left.m_sign == Sign::MINUS && right.m_sign == Sign::MINUS)
    {
        result.m_sign = Sign::MINUS;
    }
    unsigned char carry = 0;
    for (size_t i = 0; i < maxlen; ++i)
    {
        unsigned char leftDigit = (i < leftSize ? left.m_longNumber[i] : 0);
        unsigned char rightDigit = (i < rightSize ? right.m_longNumber[i] : 0);
        carry = leftDigit + rightDigit + carry;
        result.m_longNumber.push_back(carry % 10);
        carry /= 10;
    }
    if (carry > 0)
    {
        result.m_longNumber.push_back(carry);
    }
    return result;
}



LongNumber operator-(LongNumber& left_,  LongNumber& right_)
{
     LongNumber result;
     LongNumber* left = &left_;
     LongNumber* right = &right_;
    if (LongNumber::compare(left_, right_) < 0)
    {
        std::swap(left, right);
        result.m_sign = Sign::MINUS;
    }
    int buff = 0;
    size_t leftSize = (*left).m_longNumber.size();
    size_t rightSize = (*right).m_longNumber.size();
  
    result.m_longNumber.reserve(leftSize);

    for (size_t i = 0; i < (*left).m_longNumber.size(); ++i)
    {
        unsigned char leftDigit = (i<leftSize ? (*left).m_longNumber[i] : 0 );
        unsigned char rightDigit = (i < rightSize
            ? (*right).m_longNumber[i] : 0);
        const unsigned char RADIX = 10;
        buff = buff + leftDigit - rightDigit + RADIX;
        result.m_longNumber.push_back(buff % RADIX);
        buff = (buff < RADIX ? -1 : 0);
    }
    result.trim();
    return result;
}



void LongNumber::trim()
{
    while (!this->m_longNumber.empty()
        && this->m_longNumber.back() == 0)
    {
        this->m_longNumber.pop_back();
    }
}



LongNumber LongNumber::multiplyByDigit(LongNumber& left, unsigned char right)
{
    LongNumber result;
    unsigned char temp = 0;

    for (size_t i = 0; i <left.m_longNumber.size() ; ++i)
    {
        temp = left.m_longNumber[i] * right + temp;
        result.m_longNumber.push_back(temp % 10);
        temp /= 10;
    }
    if (temp > 0)
    {
        result.m_longNumber.push_back(temp);

    }
    return result;

}



LongNumber operator*(LongNumber& left_, LongNumber& right_)
{
    LongNumber* left = &left_;
    LongNumber* right = &right_;
    if (LongNumber::compare(left_, right_) < 0)
    {
        std::swap(left, right);
    }
    LongNumber result;
    size_t rightSize = right->m_longNumber.size();

    for (size_t i = 0; i < rightSize; ++i)
    {
        auto temp = LongNumber::multiplyByDigit(*left, right->m_longNumber[i]);
        temp.m_longNumber.insert(temp.m_longNumber.begin(), i, 0);
        result = result + temp;
    }
    if (left->m_sign == Sign::MINUS && right->m_sign == Sign::PLUS
        || left->m_sign == Sign::PLUS && right->m_sign == Sign::MINUS)
    {
        result.m_sign = Sign::MINUS;
    }

    result.trim();
    return result;
 }

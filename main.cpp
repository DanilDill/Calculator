
#include <iostream>
#include <string>
#include <cstdlib>
#include "LongNumber.h"
#include "Calculator.h"

int main()
{
    using namespace std;
    setlocale(LC_ALL, "Russian");
    string str = "123";
    str.pop_back();
    string expression;
    cout << "¬ведите пример  дл€ вычислени€: " << endl;
    cin >> expression;
    if (expression[expression.size()-1] == '+' || 
        expression[expression.size() - 1] == '-' ||
        expression[expression.size() - 1] == '*')
    {
        throw std::invalid_argument("Wrong input");
    }
    try
    {
        LongNumber result = Calculator::evaluate(expression);
        cout << result.Print() << endl;
    }
    catch (exception e)
    {
        cout << e.what() << endl;
    }
    system("pause");
    return 0;
}


#include <bits/stdc++.h>

using namespace std;

// a character is an operator or not
bool isOperator(char c) 
{
    return (c == '+' || c == '*' || c == '|' || c == '?');
}

// precedence of an operator
int opPrecedence(char op) 
{
    if (op == '+' || op == '?')
        return 1;

    else if (op == '*')
        return 2;

    else if (op == '|')
        return 3;

    return 0; 
}


// Function to convert infix expression to postfix
string infixToPostfix(string& infix) 
{
    string postfix;
    stack <char> stack;

    for (char c : infix) 
    {
        if (isalnum(c)) 
            postfix += c;
        
        else if (c == '(')
            stack.push(c);

        else if (c == ')') 
        {
            while (!stack.empty() && stack.top() != '(') 
            {
                postfix += stack.top();
                stack.pop();
            }

            stack.pop(); // pop '('

        } 
        
        else if (isOperator(c)) 
        {
            while (!stack.empty() && opPrecedence(c) <= opPrecedence(stack.top())) 
            {
                postfix += stack.top();
                stack.pop();
            }

            stack.push(c);
        }
    }

    while (!stack.empty()) 
    {
        postfix += stack.top();
        stack.pop();
    }

    return postfix;
}

int main() 
{
    string infixExpression = "((((a)(a))+)(b))";
    string postfixExpression = infixToPostfix(infixExpression);
    cout << "Postfix expression: " << postfixExpression << std::endl;

    return 0;
}

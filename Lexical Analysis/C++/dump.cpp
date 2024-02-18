#include <bits/stdc++.h>

using namespace std;

string insertConcat(string s)
{
    string result;
    for (size_t i = 0; i < s.size(); ++i) 
    {
        result += s[i];
        if (i < s.size() - 1 && (isalnum(s[i]) || s[i] == ')' || s[i] == '*') && (isalnum(s[i + 1]) || s[i + 1] == '('))
            result += '.';
    }

    return result;
}


// a character is an operator or not
bool isOperator(char c) 
{
    return (c == '+' || c == '*' || c == '|' || c == '?' || c == '.');
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

    else if (op == '.')
        return 4;

    return 0; 
}

// Function to convert infix expression to postfix
string infixToPostfix(string& infix) 
{
    string postfix;
    stack <char> stack;

    for (int i = 0; i < infix.size(); i++) 
    {
        char c = infix[i];

        if (isalnum(c)) 
        {
            postfix += c;
            // If the next character is also an operand or an opening parenthesis, append a concatenation operator to the output string.
            if (i + 1 < infix.size() && (isalnum(infix[i + 1]) || infix[i + 1] == '(') && infix[i + 1] != ')')
            {
                postfix += '.';
                stack.push('.');
            }
        }
        
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

            // If the next character is also an operand or an opening parenthesis, append a concatenation operator to the output string.
            if (i + 1 < infix.size() && (isalnum(infix[i + 1]) || infix[i + 1] == '(') && infix[i + 1] != ')')
            {
                postfix += '.';
                stack.push('.');
            }
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
    string infixExpression = insertConcat("((b)|((a)+))");
    cout << infixExpression << endl;
    string postfixExpression = infixToPostfix(infixExpression);
    cout << "Postfix expression: " << postfixExpression << std::endl;

    return 0;
}
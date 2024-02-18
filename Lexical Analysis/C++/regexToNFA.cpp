// function to convert regex string to eNFA
#include <bits/stdc++.h>

using namespace std;

// store nfa
struct nfa
{
    int initialState;
    map <int, map <char, set <int>>> transition; // state --> symbol --> {set of next states}
    set <int> finalStates;
};

// convert to postfix
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

// convert postfix regex to nfa
nfa regexToNFA(string s)
{
    // convert to postfix
    s = insertConcat(s);
    string str = infixToPostfix(s);
    cout << str << endl;

    // stack to keep nfa's
    stack <nfa> nfaStack;

    // indexing the state
    int counter = 0;

    // iterate over string
    for (int i = 0; i < str.size(); i++)
    {
        char c = str[i];

        // if its a single character
        if (c == 'a' || c == 'b')
        {
            // create a new 2 states nfa and push it to the stack
            nfa NFA;
            NFA.initialState = counter++;
            NFA.finalStates = {counter++};
            NFA.transition[NFA.initialState].insert({c, NFA.finalStates});

            // push it to stack
            nfaStack.push(NFA);
        }

        // if its union '|'
        else if (c == '|')
        {
            // pop last two nfa's
            nfa second = nfaStack.top(); nfaStack.pop();
            nfa first = nfaStack.top(); nfaStack.pop();

            // make a new start and a new NFA
            nfa NFA;
            NFA.initialState = counter++;

            // make the new end state
            int end = counter++;
            NFA.finalStates = {end};

            // connect it to start states of second and first
            NFA.transition[NFA.initialState] = {{'e', {first.initialState, second.initialState}}};

            // connect end states of first and second to this new end
            for (int state : first.finalStates)
            {
                if (NFA.transition.find(state) == NFA.transition.end())
                    NFA.transition[state] = {{'e', {end}}};
                else
                    NFA.transition[state]['e'].insert(end);
            }
            
            for (int state : second.finalStates)
            {
                if (NFA.transition.find(state) == NFA.transition.end())
                    NFA.transition[state] = {{'e', {end}}};
                else
                    NFA.transition[state]['e'].insert(end);
            }

            // add remaining states of first and second to NFA, including epsilon transitions
            for (auto &it : first.transition)
            {

                for (auto &trans : it.second)
                {
                    NFA.transition[it.first][trans.first].insert(trans.second.begin(), trans.second.end());
                }
            }

            for (auto &it : second.transition)
            {
                for (auto &trans : it.second)
                {
                    NFA.transition[it.first][trans.first].insert(trans.second.begin(), trans.second.end());
                }
            }

            // push it to stack
            nfaStack.push(NFA);
        }

        else if (c == '.')
        {
            // pop last two nfa's
            nfa second = nfaStack.top(); nfaStack.pop();
            nfa first = nfaStack.top(); nfaStack.pop();

            // make a new start and a new NFA
            nfa NFA;

            // make the new start and end states
            NFA.initialState = first.initialState;
            NFA.finalStates = second.finalStates;

            // add remaining states of first and second to NFA, including epsilon transitions
            for (auto &it : first.transition)
            {
                for (auto &trans : it.second)
                {
                    NFA.transition[it.first][trans.first].insert(trans.second.begin(), trans.second.end());
                }
            }

            for (auto &it : second.transition)
            {
                for (auto &trans : it.second)
                {
                    NFA.transition[it.first][trans.first].insert(trans.second.begin(), trans.second.end());
                }
            }

            // connect last states of first to start state of second
            for (int state : first.finalStates)
            {
                NFA.transition[state]['e'].insert(second.initialState);
            }

            // push it to stack
            nfaStack.push(NFA);
        }

        else if (c == '*')
        {
            // pop the last state
            nfa last = nfaStack.top(); nfaStack.pop();

            // from end state to start state
            for (int x : last.finalStates)
            {
                last.transition[x]['e'].insert(last.initialState);
            }

            // make epsilon from new first state to end of last
            last.transition[last.initialState]['e'].insert(last.finalStates.begin(), last.finalStates.end());
            nfaStack.push(last);
        }

        else if (c == '+')
        {
            // pop the last state
            nfa last = nfaStack.top(); nfaStack.pop();

            // make epsilon from end of last to new first state
            for (int x : last.finalStates)
            {
                last.transition[x]['e'].insert(last.initialState);
            }
            
            // push it to stack
            nfaStack.push(last);
        }

        else if (c == '?')
        {
            // pop the last state
            nfa last = nfaStack.top(); nfaStack.pop();

            // make epsilon from new first state to end of last
            last.transition[last.initialState]['e'].insert(last.finalStates.begin(), last.finalStates.end());

            // push it to stack
            nfaStack.push(last);
        }
    }

    return nfaStack.top();
}

int main() 
{
    string s;
    cin >> s;
    nfa answer = regexToNFA(s);
    for (auto it : answer.transition)
    {
        for (auto xt : it.second)
        {
            cout << it.first << "-> ";
            cout << xt.first << ": {";
            for (int state : xt.second)
                cout << state << ",";
            cout << "}" << endl;
        }
        cout << endl;
    }

    cout << "initial: " << answer.initialState << endl;
    cout << "final: ";
    for (int state : answer.finalStates)
        cout << state << " ";
    cout << endl;
}
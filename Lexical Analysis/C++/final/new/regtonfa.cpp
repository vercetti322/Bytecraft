#include <iostream>
#include <stack>
#include <vector>

using namespace std;

// Define the NFA class
class NFA {
public:
    int initialState;
    vector<int> finalStates;
    vector<vector<pair<char, int>>> transitions;

    NFA(int numStates) {
        initialState = 0;
        finalStates = vector<int>(numStates, 0);
        transitions = vector<vector<pair<char, int>>>(numStates, vector<pair<char, int>>());
    }

    void addTransition(int from, char symbol, int to) {
        transitions[from].push_back(make_pair(symbol, to));
    }

    void setFinalState(int state) {
        finalStates[state] = 1;
    }
};

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

// Function to convert a regular expression to an NFA
NFA regexToNFA(string regex) {
    stack<NFA> nfaStack;

    for (char c : regex) {
        if (c == '*') {
            NFA nfa = nfaStack.top();
            nfaStack.pop();

            int numStates = nfa.finalStates.size();
            NFA newNFA(numStates + 2);

            newNFA.addTransition(0, 'e', 1);
            for (int i = 0; i < numStates; i++) {
                for (auto transition : nfa.transitions[i]) {
                    newNFA.addTransition(i + 1, transition.first, transition.second + 1);
                }
                if (nfa.finalStates[i]) {
                    newNFA.addTransition(i + 1, 'e', numStates + 1);
                    newNFA.addTransition(i + 1, 'e', 1);
                }
            }
            newNFA.addTransition(0, 'e', numStates + 1);

            newNFA.setFinalState(0);
            newNFA.setFinalState(numStates + 1);

            nfaStack.push(newNFA);
        } else if (c == '|') {
            NFA nfa2 = nfaStack.top();
            nfaStack.pop();
            NFA nfa1 = nfaStack.top();
            nfaStack.pop();

            int numStates1 = nfa1.finalStates.size();
            int numStates2 = nfa2.finalStates.size();
            NFA newNFA(numStates1 + numStates2 + 2);

            newNFA.addTransition(0, 'e', 1);
            for (int i = 0; i < numStates1; i++) {
                for (auto transition : nfa1.transitions[i]) {
                    newNFA.addTransition(i + 1, transition.first, transition.second + 1);
                }
                if (nfa1.finalStates[i]) {
                    newNFA.addTransition(i + 1, 'e', numStates1 + numStates2 + 1);
                }
            }
            newNFA.addTransition(0, 'e', numStates1 + 1);

            for (int i = 0; i < numStates2; i++) {
                for (auto transition : nfa2.transitions[i]) {
                    newNFA.addTransition(i + numStates1 + 1, transition.first, transition.second + numStates1 + 1);
                }
                if (nfa2.finalStates[i]) {
                    newNFA.addTransition(i + numStates1 + 1, 'e', numStates1 + numStates2 + 1);
                }
            }
            newNFA.setFinalState(numStates1 + numStates2 + 1);

            nfaStack.push(newNFA);
        } else if (c == '.') {
            NFA nfa2 = nfaStack.top();
            nfaStack.pop();
            NFA nfa1 = nfaStack.top();
            nfaStack.pop();

            int numStates1 = nfa1.finalStates.size();
            int numStates2 = nfa2.finalStates.size();
            NFA newNFA(numStates1 + numStates2);

            for (int i = 0; i < numStates1; i++) {
                for (auto transition : nfa1.transitions[i]) {
                    newNFA.addTransition(i, transition.first, transition.second);
                }
                if (nfa1.finalStates[i]) {
                    newNFA.addTransition(i, 'e', numStates1);
                }
            }

            for (int i = 0; i < numStates2; i++) {
                for (auto transition : nfa2.transitions[i]) {
                    newNFA.addTransition(i + numStates1, transition.first, transition.second + numStates1);
                }
                if (nfa2.finalStates[i]) {
                    newNFA.setFinalState(i + numStates1);
                }
            }

            nfaStack.push(newNFA);
        } 
        
        // if char is '?', which means either char or epsilon
        else if (c == '?') {
            NFA nfa = nfaStack.top();
            nfaStack.pop();

            int numStates = nfa.finalStates.size();
            NFA newNFA(numStates + 2);

            newNFA.addTransition(0, 'e', 1);
            for (int i = 0; i < numStates; i++) {
                for (auto transition : nfa.transitions[i]) {
                    newNFA.addTransition(i + 1, transition.first, transition.second + 1);
                }
                if (nfa.finalStates[i]) {
                    newNFA.addTransition(i + 1, 'e', numStates + 1);
                }
            }
            newNFA.addTransition(0, 'e', numStates + 1);

            newNFA.setFinalState(0);
            newNFA.setFinalState(numStates + 1);

            nfaStack.push(newNFA);
        }
        
        else {
            NFA newNFA(2);
            newNFA.addTransition(0, c, 1);
            newNFA.setFinalState(1);
            nfaStack.push(newNFA);
        }
    }

    return nfaStack.top();
}

int main() {
    string regex;
    cout << "Enter an infix regular expression: ";
    cin >> regex;

    regex = insertConcat(regex);
    cout << "Postfix regular expression: " << infixToPostfix(regex) << endl;
    NFA nfa = regexToNFA(infixToPostfix(regex));

    // Print the NFA transitions
    cout << "NFA Transitions:" << endl;
    for (int i = 0; i < nfa.transitions.size(); i++) {
        for (auto transition : nfa.transitions[i]) {
            cout << i << " --" << transition.first << "--> " << transition.second << endl;
        }
    }

    return 0;
}

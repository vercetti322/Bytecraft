#include<bits/stdc++.h>

using namespace std;

// alphabet set : ('a', 'b')

/*
    Base Part
    *
    *
    * 
    * 
    * 
    * 
    * 
    * 
    * 
    Structures to hold nfa

*/

struct nfaNode
{
    int state;
    bool acceptance; // to check for final state
    map <char, set <int>> transition;

    // constructor to initialize nfaNode
    nfaNode() : state(-1), acceptance(false), transition() {}
};

// struct to hold nfa
struct nfa
{
    vector <nfaNode> states;
    int initialState;

    // constructor to initialize the DFA
    nfa(int numStates, int initial) : states(numStates), initialState(initial) {}
};

/*
    First Part
    *
    *
    * 
    * 
    * 
    * 
    * 
    * 
    * 
    Functions to convert infix to postfix
*/
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

/*
    Second Part
    *
    *
    * 
    * 
    * 
    * 
    * 
    * 
    * 
    Functions to convert postfix to nfa
*/

// function to make regex postfix to nfa with epsilon transitions
nfa regexToNFA(string regex)
{
    int stateCounter = 0; 

    // initialise nfa stack
    stack <nfa> nfaStack;

    // iterate over postfix
    for (int i = 0; i < regex.size(); i++)
    {
        char c = regex[i];

        // if c is a symbol from alphabet
        if (c == 'a' || c == 'b')
        {
            // create two nfa states (x -> y)
            nfaNode start;
            start.state = stateCounter++;
            start.acceptance = false;

            nfaNode end;
            end.state = stateCounter++;
            end.acceptance = true;

            // make transition
            start.transition[c].insert(end.state);

            // fill in nfa
            nfa NFA(0, 0); 
            NFA.states.push_back(start);
            NFA.states.push_back(end);
            NFA.initialState = start.state;
            NFA.states[0].acceptance = false; 
            NFA.states[1].acceptance = true;

            // push to stack
            nfaStack.push(NFA);
        }

        else if (c == '.')
        {
            if (nfaStack.size() < 2)
                throw runtime_error("Invalid regular expression");

            nfa NFA2 = nfaStack.top();
            nfaStack.pop();

            nfa NFA1 = nfaStack.top();
            nfaStack.pop();

            // join final states of NFA1 to initial of NFA2
            for (nfaNode &node : NFA1.states)
            {
                if (node.acceptance)
                {
                    node.acceptance = false;
                    node.transition['e'].insert(NFA2.initialState);
                }
            }

            // create nfa
            nfa NFA(0, 0);
            NFA.states.reserve(NFA1.states.size() + NFA2.states.size());
            NFA.states.resize(NFA1.states.size() + NFA2.states.size());

            // Copy states from NFA1 to NFA, excluding start and end states
            for (int i = 0; i < NFA1.states.size(); i++)
            {
                NFA.states[i] = NFA1.states[i];
            }

            // Copy states from NFA2 to NFA, excluding start and end states
            for (int i = 0; i < NFA2.states.size(); i++)
            {
                NFA.states[i + NFA1.states.size()] = NFA2.states[i];
                if (NFA2.states[i].acceptance)
                        NFA.states[i + NFA1.states.size()].acceptance = true;
            }

            NFA.initialState = NFA1.initialState;

            // push to stack
            nfaStack.push(NFA);
        }


        else if (c == '|')
        {
            if (nfaStack.size() < 2)
                throw runtime_error("Invalid regular expression");

            nfa NFA1 = nfaStack.top();
            nfaStack.pop();

            nfa NFA2 = nfaStack.top();
            nfaStack.pop();

            // make new start state and end state
            nfaNode start;
            start.state = stateCounter++;
            start.acceptance = false;

            nfaNode end;
            end.state = stateCounter++;
            end.acceptance = true;

            // joint new start state to initial states of NFA's
            start.transition['e'].insert(NFA1.initialState);
            start.transition['e'].insert(NFA2.initialState);

            // join final states of NFA's to end
            for (nfaNode &node: NFA1.states)
            {
                if (node.acceptance)
                {
                    node.acceptance = false;
                    node.transition['e'].insert(end.state);
                }
            }

            for (nfaNode &node: NFA2.states)
            {
                if (node.acceptance)
                {
                    node.acceptance = false;
                    node.transition['e'].insert(end.state);
                }
            }


            // create nfa
            nfa NFA(0, 0);
            NFA.states.push_back(start); // Add start state

            for (const auto &state : NFA1.states)
            {
                if (state.state != start.state && state.state != end.state)
                {
                    NFA.states.push_back(state);
                }
            }

            for (const auto &state : NFA2.states)
            {
                if (state.state != start.state && state.state != end.state)
                {
                    NFA.states.push_back(state);
                }
            }

            NFA.states.push_back(end); 
            NFA.initialState = start.state;
            
            // push to stack
            nfaStack.push(NFA);
        }

        else if (c == '*')
        {
            if (nfaStack.empty())
                throw runtime_error("Invalid regular expression");
            
            nfa NFA = nfaStack.top();
            nfaStack.pop();

            // connect last state to first state
            for (nfaNode &node : NFA.states)
            {
                if (node.acceptance)
                {
                    node.acceptance = false;
                    node.transition['e'].insert(NFA.initialState);
                }
            }

            // make start state as acceptance state
            NFA.states[NFA.initialState].acceptance = true;

            nfaStack.push(NFA);
        }

        else if (c == '+')
        {
            if (nfaStack.empty())
                throw runtime_error("Invalid regular expression");
            
            nfa NFA = nfaStack.top();
            nfaStack.pop();

            // connect last state to first state
            for (nfaNode &node : NFA.states)
            {
                if (node.acceptance)
                {
                    node.transition['e'].insert(NFA.initialState);
                }
            }

            nfaStack.push(NFA);
        }

        else if (c == '?')
        {
            if (nfaStack.empty())
                throw runtime_error("Invalid regular expression");
            
            nfa NFA = nfaStack.top();
            nfaStack.pop();

            // push an end state
            nfaNode end;
            end.state = stateCounter++;

            // Add an epsilon transition from the start state to the new end state
            int x = NFA.initialState;

            // define the transition here only
            for (nfaNode &node : NFA.states)
            {
                if (node.acceptance)
                {
                    node.acceptance = false;
                    node.transition['e'].insert(end.state);
                }

                else if (node.state == x)
                {
                    node.transition['e'].insert(end.state);
                }
            }

            end.acceptance = true;
            NFA.states.push_back(end);

            nfaStack.push(NFA);
        }
    }

    return nfaStack.top();
}

int main() 
{
    // a*b.a*ba*...*
    string regex = "a*b.a*ba*...*";
    nfa result = regexToNFA(regex);

    for (nfaNode &node : result.states)
    {
        if (node.state == result.initialState)
            cout << "Start State: " << node.state << endl;
        else if (node.acceptance)
            cout << "Final State: " << node.state << endl;

        else
            cout << "State: " << node.state << endl;
        cout << "Acceptance: " << node.acceptance << endl;
        for (auto it : node.transition)
        {
            cout << "Transition: " << it.first << " -> ";
            for (int i : it.second)
            {
                cout << i << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
    return 0;
}
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
    Structures to hold nfa and dfa

*/

struct nfaNode
{
    int state;
    bool acceptance; // to check for final state
    mutable map <char, set <int>> transition;

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

    // second constructor to initialize the NFA
    nfa(int numStates) : states(numStates)
    {
        for (int i = 0; i < numStates; i++)
            states[i].state = i; 
    }
};

// struct to hold dfa transition
struct dfaNode
{
    int state;
    bool acceptance; // to check for final state
    unordered_map <char, int> transition;
    dfaNode() : state(-1), acceptance(false) {}
};

// struct to hold dfa
struct dfa
{
    vector <dfaNode> states;
    int initialState;
    // constructor to initialize the DFA
    dfa(int numStates, int initial) : states(numStates), initialState(initial) {}

    // second constructor to initialize the DFA
    dfa(int initial) : initialState(initial) {}
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
    std :: stack <nfa> nfaStack;

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

/*  
    Third Part
    *
    *
    * 
    * 
    * 
    * 
    * 
    * 
    * 
    Epslion NFA to NFA
*/

// function to calculate epsilon closure
set <int> epClosure(int currentState, const vector <nfaNode>& NFA)
{
    set <int> closure;
    stack <int> stateStack;
    set <int> visited;  // New set to track visited states

    // Push the initial state onto the stack and mark it as visited
    stateStack.push(currentState);
    closure.insert(currentState);
    visited.insert(currentState);

    while (!stateStack.empty())
    {
        currentState = stateStack.top();
        stateStack.pop();

        // Check if epsilon transition exists
        auto epsilonTransition = NFA[currentState].transition.find('e');

        if (epsilonTransition != NFA[currentState].transition.end()) 
        {
            for (int trans : epsilonTransition->second)
            {
                // Check if the state is not in closure and not visited
                if (closure.find(trans) == closure.end() && visited.find(trans) == visited.end()) 
                {
                    closure.insert(trans);
                    stateStack.push(trans);
                    visited.insert(trans);
                }
            }
        }
    }

    return closure;
}

// function to make NFA from e-NFA
nfa eNfatoNFA(const nfa& eNFA)
{
    int numStates = eNFA.states.size();
    nfa NFA(numStates);

    // iterate over each state
    for (const nfaNode& state : eNFA.states)
    {
        // calculate epsilon closure of state
        set <int> first = epClosure(state.state, eNFA.states);

        for (char symbol : {'a', 'b'})
        {
            // do the required transition
            set <int> second;
            
            for (int value : first)
            {
                set <int> temp = eNFA.states[value].transition[symbol];
                if (!temp.empty())
                    second.insert(temp.begin(), temp.end());
            }

            // take epsilon closure again
            set <int> ans;

            for (int value : second)
            {
                set <int> arr = epClosure(value, eNFA.states);
                if (!arr.empty())
                    ans.insert(arr.begin(), arr.end());
            }

            // assign it to the state of NFA
            NFA.states[state.state].transition[symbol] = ans;
            NFA.states[state.state].acceptance = false;
        }
    }

    for (int state = 0; state < NFA.states.size(); state++)
    {
        bool isAccepting = false;
        set <int> closure = epClosure(state, eNFA.states);
        for (int x : closure)
            if (eNFA.states[x].acceptance)
                isAccepting = true;

        NFA.states[state].acceptance = isAccepting;
    }

    return NFA;
}

/*  
    Fourth Part
    *
    *
    * 
    * 
    * 
    * 
    * 
    * 
    * 
    NFA to DFA
*/

dfa nfaToDFA(nfa NFA)
{
    // initialise a dfa with initial state of NFA
    dfa DFA(0);
    DFA.states.push_back(dfaNode());

    // map to assign a unique number to each set of states
    map <set <int>, int> stateNumbers;
    stateNumbers[{0}] = 0;

    // set to keep track of visited states
    set <set <int>> visited;

    // queue to keep track of states to be visited
    queue <set <int>> stateQueue;

    // push the initial state of NFA onto the queue
    stateQueue.push({0});
    visited.insert({0});

    while (!stateQueue.empty())
    {
        set <int> currentState = stateQueue.front();
        stateQueue.pop();

        // get the transition states for the current state
        set <int> zeroTransition, oneTransition;
        for (int state : currentState) 
        {
            zeroTransition.insert(NFA.states[state].transition['a'].begin(), NFA.states[state].transition['a'].end());
            oneTransition.insert(NFA.states[state].transition['b'].begin(), NFA.states[state].transition['b'].end());
        }

        // check if the state is not visited
        if (visited.find(zeroTransition) == visited.end())
        {
            // mark the state as visited
            visited.insert(zeroTransition);

            // push the state onto the queue
            stateQueue.push(zeroTransition);

            // add the state to the DFA
            DFA.states.push_back(dfaNode());

            // assign a unique number to the state
            stateNumbers[zeroTransition] = DFA.states.size() - 1;
        }

        // check if the state is not visited
        if (visited.find(oneTransition) == visited.end())
        {
            // mark the state as visited
            visited.insert(oneTransition);

            // push the state onto the queue
            stateQueue.push(oneTransition);

            // add the state to the DFA
            DFA.states.push_back(dfaNode());

            // assign a unique number to the state
            stateNumbers[oneTransition] = DFA.states.size() - 1;
        }

        // set the transition states for the current state
        DFA.states[stateNumbers[currentState]].transition['a'] = stateNumbers[zeroTransition];
        DFA.states[stateNumbers[currentState]].transition['b'] = stateNumbers[oneTransition];
    }

    // set the acceptance states for the DFA
    for (int i = 0; i < NFA.states.size(); i++)
    {
        if (NFA.states[i].acceptance)
        {
            for (const auto& state : visited)
            {
                if (state.find(i) != state.end())
                {
                    DFA.states[stateNumbers[state]].acceptance = true;
                }
            }
        }
    }

    return DFA;
}

/*
    Fifth Part
    *
    *
    * 
    * 
    * 
    * 
    * 
    * 
    * 
    Check string acceptance
*/
// function to check acceptance of a string by dfa
bool dfaAcceptance(dfa DFA, string input)
{
    int currentState = DFA.initialState;

    // iterating over...
    for (char c : input)
    {
        if (DFA.states[currentState].transition.find(c) == DFA.states[currentState].transition.end())
        {
            cout << "Invalid input!" << endl;
            return false;
        }

        currentState = DFA.states[currentState].transition[c];
    }

    if (DFA.states[currentState].acceptance)
        return true;

    return false;
}

/*
    Main Function
    *
    *
    * 
    * 
    * 
    * 
    * 
    * 
    * 
    Main Function

*/

int main() 
{
    // a*b.a*ba*...*
    string input = "(((a)+)(b))";
    string regex = insertConcat(input);
    cout << regex << endl;
    regex = infixToPostfix(regex);
    cout << regex << endl;
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

    nfa NFA = eNfatoNFA(result);
    for (int i = 0; i < NFA.states.size(); i++)
    {
        cout << "State " << i << ": ";
        for (const auto& transition : NFA.states[i].transition)
        {
            cout << "On " << transition.first << " -> ";
            for (int state : transition.second)
            {
                cout << state << " ";
            }
        }
        if (NFA.states[i].acceptance)
        {
            cout << "Acceptance";
        }
        cout << endl;
    }

    dfa DFA = nfaToDFA(NFA);
    // print the DFA
    for (int i = 0; i < DFA.states.size(); i++)
    {
        cout << "State " << i << ": ";
        for (const auto& transition : DFA.states[i].transition)
        {
            cout << "On " << transition.first << " -> " << transition.second << " ";
        }
        if (DFA.states[i].acceptance)
        {
            cout << "Acceptance";
        }
        cout << endl;
    }

    // check string acceptance
    string s;
    getline(cin, s); // also handles empty string
    bool acceptance = dfaAcceptance(DFA, s);
    cout << "Acceptance: " << acceptance << endl;
    return 0;
}
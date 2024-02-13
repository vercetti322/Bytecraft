#include <bits/stdc++.h>

using namespace std;

// alphabet set: ('0', '1')

struct nfaNode
{
    int state;
    bool acceptance; // to check for the final state
    mutable map <char, set <int>> transition;
    nfaNode(int s = -1) : state(s), acceptance(false) {}
};

// struct to hold nfa
struct nfa
{
    vector <nfaNode> states;
    int initialState;

    // constructor to initialize the NFA
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
    map <char, int> transition;
    dfaNode() : state(-1), acceptance(false) {}
};

// struct to hold dfa
struct dfa
{
    vector <dfaNode> states;
    int initialState;
    // constructor to initialize the DFA
    dfa(int initial) : initialState(initial) {}
};

// function to get epsilon closure
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

        for (char symbol : {'0', '1'})
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

dfa nfaToDFA(nfa NFA)
{
    dfa DFA(0);
    DFA.states.push_back(dfaNode());

    map <set <int>, int> stateNumbers;
    stateNumbers[{0}] = 0;

    map <set <int>, bool> visited;
    queue <set <int>> stateQueue;

    set <int> initialState = {0};
    stateQueue.push(initialState);
    visited[initialState] = true;
    stateNumbers[initialState] = 0;

    while (!stateQueue.empty())
    {
        set<int> currentState = stateQueue.front();
        stateQueue.pop();

        for (char symbol : {'0', '1'})
        {
            set<int> nextState;

            for (int nfaState : currentState)
            {
                if (NFA.states[nfaState].transition.find(symbol) != NFA.states[nfaState].transition.end())
                {
                    nextState.insert(NFA.states[nfaState].transition[symbol].begin(), NFA.states[nfaState].transition[symbol].end());
                }
            }

            if (!nextState.empty())
            {
                auto it = stateNumbers.find(nextState);
                int nextStateNumber;

                if (it == stateNumbers.end())
                {
                    nextStateNumber = stateNumbers.size();
                    stateQueue.push(nextState);
                    visited[nextState] = true;
                    stateNumbers[nextState] = nextStateNumber;
                    DFA.states.push_back(dfaNode());
                }
                else
                {
                    nextStateNumber = it -> second;
                }

                DFA.states[stateNumbers[currentState]].transition[symbol] = stateNumbers[nextState];
            }

            else
            {
                std::cerr << "Empty nextState for state " << stateNumbers[currentState] << " and symbol " << symbol << std::endl;
            }
        }
    }

    for (const auto &state : stateNumbers)
    {
        for (int x : state.first)
        {
            if (NFA.states[x].acceptance)
            {
                DFA.states[state.second].acceptance = true;
                break;
            }
        }
    }

    return DFA;
}

int main()
{
    nfa eNFA(2);

    // State 0 transitions
    eNFA.states[0].transition['e'] = {1};  
    eNFA.states[0].transition['0'] = {0};  

    // State 1 transitions
    eNFA.states[1].transition['1'] = {1};  
    eNFA.states[1].acceptance = true;

    // convert the epsilon-NFA to a regular NFA
    nfa NFA = eNfatoNFA(eNFA);

    // convert the NFA to a DFA
    dfa DFA = nfaToDFA(NFA);

    // print the DFA
    cout << "DFA: " << endl;
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

    return 0;
}
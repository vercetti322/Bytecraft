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

// function to concatenate a set of ints
int concat(set <int> states)
{
    int ans = 0;
    int k = 1;
    for (auto it = states.rend(); it != states.rbegin(); it--)
    {
        ans += *(it) * k;
        k *= 10;
    }

    return ans;   
}

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

// function to convert an NFA (without ep) to DFA
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
            zeroTransition.insert(NFA.states[state].transition['0'].begin(), NFA.states[state].transition['0'].end());
            oneTransition.insert(NFA.states[state].transition['1'].begin(), NFA.states[state].transition['1'].end());
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
        DFA.states[stateNumbers[currentState]].transition['0'] = stateNumbers[zeroTransition];
        DFA.states[stateNumbers[currentState]].transition['1'] = stateNumbers[oneTransition];
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

int main() {}
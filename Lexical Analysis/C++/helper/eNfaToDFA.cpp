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
    set <int> initialState = {0};
    stateQueue.push(initialState);
    visited.insert(initialState);
    stateNumbers[initialState] = 0;

    while (!stateQueue.empty())
    {
        set <int> currentState = stateQueue.front();
        stateQueue.pop();

        // get the transition states for the current state
        set <int> zeroTransition, oneTransition;

        for (int state : currentState) 
        {
            for (int nextState : NFA.states[state].transition['0'])
            {
                zeroTransition.insert(nextState);
            }
            for (int nextState : NFA.states[state].transition['1'])
            {
                oneTransition.insert(nextState);
            }
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

int main()
{
    // create an epsilon-NFA
    nfa eNFA(3);
    eNFA.states[0].transition['e'] = {1};
    eNFA.states[0].transition['2'] = {0};
    eNFA.states[1].transition['e'] = {2};
    eNFA.states[1].transition['1'] = {1};
    eNFA.states[2].transition['1'] = {1};
    eNFA.states[2].transition['2'] = {1};
    eNFA.states[2].acceptance = true;

    for (int x : epClosure(0, eNFA.states))
        cout << x << endl;

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
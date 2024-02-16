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

int main()
{
    // create an NFA
    nfa NFA(2);
    NFA.states[0].transition['1'] = {1, 0};
    NFA.states[1].transition['0'] = {1};
    NFA.states[0].acceptance = true;

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

int main()
{
    // create an epsilon-NFA
    nfa NFA(2);
    NFA.states[0].transition['1'] = {1};
    NFA.states[1].transition['0'] = {1};
    NFA.states[0].acceptance = true;

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
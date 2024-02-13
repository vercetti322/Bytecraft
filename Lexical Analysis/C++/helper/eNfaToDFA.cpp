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
    nfa(int numStates, int initial) : states(numStates), initialState(initial)
    {
        for (int i = 0; i < numStates; i++)
            states[i].state = i; 
    }
};

struct dfaNode
{
    set <int> states;
    bool acceptance; // to check for the final state
    map <char, set <int>> transition;
    dfaNode() : acceptance(false) {}
};

// struct to hold dfa
struct dfa
{
    vector <dfaNode> states;
    int initialState;
    // constructor to initialize the DFA
    dfa(int initial) : initialState(initial), states(1)
    {
        states[0].states.insert(initial);
    }
};

// function to get epsilon closure
set <int> epClosure(int currentState, const vector <nfaNode>& NFA)
{
    set <int> closure;
    stack <int> stateStack;

    // Push the initial state onto the stack
    stateStack.push(currentState);
    closure.insert(currentState);

    while (!stateStack.empty())
    {
        currentState = stateStack.top();
        stateStack.pop();

        // Check if epsilon transition exists
        auto epsilonTransition = NFA[currentState].transition.find('e');

        if (epsilonTransition != NFA[currentState].transition.end()) 
            for (int trans : epsilonTransition -> second)
                if (closure.find(trans) == closure.end()) 
                {
                    closure.insert(trans);
                    stateStack.push(trans);
                }
    }

    return closure;
}

nfa eNfaToNfa(const nfa& eNFA)
{
    nfa NFA = eNFA;

    // Remove epsilon transitions by computing epsilon closures
    for (int i = 0; i < NFA.states.size(); i++)
    {
        // Copy the current transitions before modification
        map <char, set <int>> originalTransitions = NFA.states[i].transition;
        set <int> currClosure = epClosure(i, NFA.states);

        // Print epsilon closure information
        cout << "Epsilon Closure for State " << i << ": { ";
        for (int state : currClosure) {
            cout << state << " ";
        }
        cout << "}\n";

        for (char symbol : {'0', '1'})
        {
            set <int> nextState;

            for (int state : currClosure)
            {
                set <int> finState = NFA.states[state].transition[symbol];
                nextState.insert(finState.begin(), finState.end());
            }

            // Print transition information
            cout << "Transition: State " << i << " on symbol '" << symbol << "' -> { ";
            for (int state : nextState) {
                cout << state << " ";
            }
            cout << "}\n";

            // update the state
            NFA.states[i].transition[symbol] = nextState;
        }

        // Print acceptance property update information
        cout << "Updated Acceptance Property for State " << i << ": " << (NFA.states[i].acceptance ? "true" : "false") << "\n";
    }

    // Remove epsilon transitions outside the symbol loop
    for (auto& state : NFA.states)
        state.transition.erase('e');

    // Print final NFA
    cout << "Final NFA after epsilon-NFA to NFA conversion:\n";
    // Print NFA states, transitions, and acceptance status here

    return NFA;
}

dfa nfaToDFA(const nfa& NFA)
{
    int initial = NFA.initialState;
    dfa DFA(initial);

    queue <set <int>> storage;
    storage.push(epClosure(initial, NFA.states));

    map <set <int>, int> stateToDFAIndex;
    stateToDFAIndex[epClosure(initial, NFA.states)] = 0;

    while (!storage.empty())
    {
        set <int> currentState = storage.front();
        storage.pop();

        cout << "Processing State: { ";
        for (int state : currentState)
        {
            cout << state << " ";
        }
        cout << "}\n";

        bool isAccepting = false;
        for (int state : currentState)
        {
            if (NFA.states[state].acceptance)
            {
                isAccepting = true;
                break;
            }
        }

        int currentStateIndex = stateToDFAIndex[currentState];
        DFA.states[currentStateIndex].acceptance = isAccepting;

        for (char symbol : {'0', '1'})
        {
            set<int> nextState;

            for (int state : currentState)
            {
                set<int> finState = NFA.states[state].transition[symbol];
                nextState.insert(finState.begin(), finState.end());
            }

            if (nextState.empty())
            {
                continue;
            }

            cout << "Transition: { ";
            for (int state : currentState)
            {
                cout << state << " ";
            }
            cout << "} on symbol '" << symbol << "' -> { ";
            for (int state : nextState)
            {
                cout << state << " ";
            }
            cout << "}\n";

            if (stateToDFAIndex.find(nextState) == stateToDFAIndex.end())
            {
                int newStateIndex = DFA.states.size();
                stateToDFAIndex[nextState] = newStateIndex;
                storage.push(nextState);

                cout << "New State: { ";
                for (int state : nextState)
                {
                    cout << state << " ";
                }
                cout << "} added to the queue.\n";

                dfaNode newState;
                newState.states = nextState;
                newState.acceptance = false;
                DFA.states.push_back(newState);
            }

            DFA.states[currentStateIndex].transition[symbol].insert(stateToDFAIndex[nextState]);
        }
    }

    cout << "Final DFA States:\n";
    for (int i = 0; i < DFA.states.size(); ++i)
    {
        cout << "DFA State " << i << ": { ";
        for (int state : DFA.states[i].states)
        {
            cout << state << " ";
        }
        cout << "}, Acceptance: " << (DFA.states[i].acceptance ? "true" : "false") << ", Transitions: {";
        for (const auto& transition : DFA.states[i].transition)
        {
            cout << " '" << transition.first << "' -> { ";
            for (int state : transition.second)
            {
                cout << state << " ";
            }
            cout << "},";
        }
        cout << " }\n";
    }

    return DFA;
}


int main()
{
    // Construct the e-NFA
    nfa eNFA(3, 0);

    eNFA.states[0].acceptance = true;
    eNFA.states[1].acceptance = true;

    eNFA.states[0].transition['0'] = {0};
    eNFA.states[0].transition['1'] = {1, 2};
    eNFA.states[1].transition['e'] = {2};
    eNFA.states[2].transition['1'] = {1};

    // Convert e-NFA to NFA
    nfa NFA = eNfaToNfa(eNFA);

    cout << endl << "------------------------------------" << endl;

    // Convert NFA to DFA
    dfa DFA = nfaToDFA(NFA);
    return 0;
}

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

// function to get epsilon closure
set <int> epClosure(int currentState, const vector <nfaNode>& NFA)
{
    set <int> closure;
    stack <int> stateStack;
    set<int> visited;  // New set to track visited states

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

int main() 
{
    // Create an epsilon-NFA
    nfa eNFA(3);

    eNFA.states[0].transition['e'] = {1};
    eNFA.states[1].transition['0'] = {2};
    eNFA.states[2].acceptance = true;


    // Convert epsilon-NFA to NFA
    nfa NFA = eNfatoNFA(eNFA);

    // Display the resulting NFA
    cout << "Resulting NFA:\n";
    for (const nfaNode& state : NFA.states) 
    {
        cout << "State " << state.state << " Acceptance: " << (state.acceptance ? "True" : "False") << "\n";
        for (const auto& trans : state.transition) 
        {
            cout << "  '" << trans.first << "' -> {";

            for (int value : trans.second)
                cout << value << " ";

            cout << "}\n";
        }
    }

    // Flush the output stream
    cout << flush;

    return 0;
}



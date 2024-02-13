#include<bits/stdc++.h>

using namespace std;

// alphabet set : ('a', 'b')

struct nfaNode
{
    int state;
    bool acceptance; // to check for final state
    map <char, set <int>> transition;
    nfaNode() : acceptance(false) {}
};

// struct to hold nfa
struct nfa
{
    vector <nfaNode> states;
    int initialState;

    // constructor to initialize the DFA
    nfa(int numStates, int initial) : states(numStates), initialState(initial) {}
};

// or (regex to e-nfa)
struct nfa orRegexToNFA(int a)
{
    // let start be a
    nfa NFA(2, a);

    NFA.states[0].state = 0;
    NFA.states[1].state = a;
    NFA.states[1].acceptance = true;

    // create transitions from 0 -> a
    NFA.states[0].transition['a'].insert(a);
    NFA.states[0].transition['e'].insert(0);
    NFA.states[1].transition['a'].insert(a);

    return NFA;
}

int main()
{
    // Create NFA for a* with a = 1
    int a = 1;
    auto NFA = orRegexToNFA(a);

    // Print NFA transitions
    for (const auto& node : NFA.states) {
        std::cout << "State " << node.state << ":\n";
        for (const auto& transition : node.transition) {
            char symbol = transition.first;
            const auto& nextStates = transition.second;
            std::cout << "  " << symbol << " -> {";
            for (int nextState : nextStates) {
                std::cout << nextState << ", ";
            }
            std::cout << "}\n";
        }
        std::cout << "--------------\n";
    }

    return 0;
}
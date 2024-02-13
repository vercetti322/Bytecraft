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
struct nfa orRegexToNFA(int a, int b)
{
    // let start be 0, a and b and final state be std::max(a, b) + 1
    // create a 4 state nfa
    nfa NFA(4, 0);
    int fin = std::max(a, b) + 1;
    NFA.states[0].state = 0;
    NFA.states[1].state = a;
    NFA.states[2].state = b;
    NFA.states[3].state = fin;

    // create epsilon transitions from a + b -> a and b
    NFA.states[0].transition['e'].insert(a);
    NFA.states[0].transition['e'].insert(b);

    // create epsilon transtions from a and b -> a * b
    NFA.states[a].transition['e'].insert(fin);
    NFA.states[b].transition['e'].insert(fin);

    return NFA;
}

int main()
{
    // Create NFA for a | b with a = 1 and b = 2
    int a = 1, b = 2;
    auto NFA = orRegexToNFA(a, b);

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
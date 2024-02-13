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
    // let start be 0, (a + b) / 2 and final state be std::max(a, b) + 1
    // create a 3 state nfa
    nfa NFA(3, 0);
    int fin = std::max(a, b) + 1;
    int mid = (a + b) / 2;
    NFA.states[0].state = 0;
    NFA.states[1].state = mid;
    NFA.states[2].state = fin;
    NFA.states[2].acceptance = true;

    // create epsilon transitions from 0 -> mid
    NFA.states[0].transition['a'].insert(mid);

    // create epsilon transtions from mid -> fin
    NFA.states[mid].transition['b'].insert(fin);

    return NFA;
}

int main()
{
    // Create NFA for ab with a = 1 and b = 2
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
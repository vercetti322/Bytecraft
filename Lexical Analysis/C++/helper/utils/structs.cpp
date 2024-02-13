#include<bits/stdc++.h>

using namespace std;

// DFA structure
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
};
















// NFA structure
// struct to hold nfa transition
struct nfaNode
{
    int state;
    bool acceptance; // to check for final state
    unordered_map <char, unordered_set <int>> transition;
    nfaNode() : state(-1), acceptance(false) {}
};

// struct to hold nfa
struct nfa
{
    vector <nfaNode> states;
    int initialState;

    // constructor to initialize the DFA
    nfa(int numStates, int initial) : states(numStates), initialState(initial) {}
};


int main() {}
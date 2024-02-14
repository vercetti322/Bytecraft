#include<bits/stdc++.h>

using namespace std;

// alphabet set : ('a', 'b')

struct nfaNode
{
    int state;
    bool acceptance; // to check for final state
    map <char, set <int>> transition;

    // constructor 1
    nfaNode(int state) : state(state), acceptance(false), transition() {}

    // constructor 2
    nfaNode() : state(-1), acceptance(false), transition() {}
};

// struct to hold nfa
struct nfa
{
    vector <nfaNode> states;
    int initialState;

    // constructor to initialize the DFA
    nfa(int numStates, int initial) : states(numStates), initialState(initial) {}
};

// function to make regex postfix to nfa with epsilon transitions
nfa regexToNFA(string regex)
{
    int stateCounter = 0; 

    // initialise nfa stack
    stack <nfa> nfaStack;

    // iterate over postfix
    for (char c : regex)
    {
        // if c is a symbol from alphabet
        if (c == 'a' || c == 'b')
        {
            // create two nfa states (x -> y)
            nfaNode start(stateCounter++);
            start.acceptance = false;

            nfaNode end(stateCounter++);
            end.acceptance = true;

            // make transition
            start.transition[c].insert(end.state);

            // fill in nfa
            nfa NFA(2, start.state);
            NFA.states[start.state] = start;
            NFA.states[end.state] = end;

            // push to the stack
            nfaStack.push(NFA);
        }

        else if (c == '.')
        {
            if (nfaStack.size() < 2)
                throw runtime_error("Invalid regular expression");

            nfa NFA1 = nfaStack.top();
            nfaStack.pop();

            nfa NFA2 = nfaStack.top();
            nfaStack.pop();

            // join final states of NFA1 to initial of NFA2
            for (nfaNode &node: NFA1.states)
            {
                if (node.acceptance)
                {
                    node.acceptance = false;
                    node.transition['e'].insert(NFA2.initialState);
                }
            }

            // create nfa
            nfa NFA(NFA1.states.size() + NFA2.states.size(), NFA1.initialState);
            copy(NFA1.states.begin(), NFA1.states.end(), NFA.states.begin());
            copy(NFA2.states.begin(), NFA2.states.end(), NFA.states.begin() + NFA1.states.size());

            // push to stack
            nfaStack.push(NFA);
        }

        else if (c == '|')
        {
            if (nfaStack.size() < 2)
                throw runtime_error("Invalid regular expression");

            nfa NFA1 = nfaStack.top();
            nfaStack.pop();

            nfa NFA2 = nfaStack.top();
            nfaStack.pop();

            // make new start state and end state
            nfaNode start(stateCounter++);
            nfaNode end(stateCounter++);
            start.acceptance = false;
            end.acceptance = true;

            // joint new start state to initial states of NFA's
            start.transition['e'].insert(NFA1.initialState);
            start.transition['e'].insert(NFA2.initialState);

            // join final states of NFA's to end
            for (nfaNode &node: NFA1.states)
            {
                if (node.acceptance)
                {
                    node.acceptance = false;
                    node.transition['e'].insert(end.state);
                }
            }

            for (nfaNode &node: NFA2.states)
            {
                if (node.acceptance)
                {
                    node.acceptance = false;
                    node.transition['e'].insert(end.state);
                }
            }


            // create nfa
            nfa NFA(NFA1.states.size() + NFA2.states.size(), NFA1.initialState);
            NFA.states[start.state] = start;
            NFA.states[end.state] = end;
            copy(NFA1.states.begin(), NFA1.states.end(), NFA.states.begin() + 1);
            copy(NFA2.states.begin(), NFA2.states.end(), NFA.states.begin() + 1 + NFA1.states.size());
            
            // push to stack
            nfaStack.push(NFA);
        }

        else if (c == '+')
        {
            if (nfaStack.empty())
                throw runtime_error("Invalid regular expression");

            nfa NFA = nfaStack.top();
            nfaStack.pop();

            // create two states
            nfaNode start(stateCounter++);
            start.acceptance = false;

            nfaNode end(stateCounter++);
            end.acceptance = true;

            // join new start state to initial state of NFA popped
            start.transition['e'].insert(NFA.initialState);

            // join final states of NFA to end state
            for (nfaNode &node : NFA.states)
            {
                if (node.acceptance)
                {
                    node.acceptance = false;
                    node.transition['e'].insert(end.state);
                    node.transition['e'].insert(NFA.initialState);
                }
            }

            // final NFA
            nfa newNFA(NFA.states.size() + 2, start.state);
            newNFA.states[start.state] = start;
            newNFA.states[end.state] = end;
            copy(NFA.states.begin(), NFA.states.end(), newNFA.states.begin() + 1);

            // push to stack
            nfaStack.push(newNFA);
        }

        else if (c == '*')
        {
            if (nfaStack.empty())
                throw runtime_error("Invalid regular expression");

            nfa NFA = nfaStack.top();
            nfaStack.pop();

            // create two states
            nfaNode start(stateCounter++);
            start.acceptance = false;

            nfaNode end(stateCounter++);
            end.acceptance = true;

            // join new start state to initial state of NFA popped
            start.transition['e'].insert(NFA.initialState);
            start.transition['e'].insert(end.state);

            // join final states of NFA to end state
            for (nfaNode &node : NFA.states)
            {
                if (node.acceptance)
                {
                    node.acceptance = false;
                    node.transition['e'].insert(end.state);
                    node.transition['e'].insert(NFA.initialState);
                }
            }

            // final NFA
            nfa newNFA(NFA.states.size() + 2, start.state);
            newNFA.states[start.state] = start;
            newNFA.states[end.state] = end;
            copy(NFA.states.begin(), NFA.states.end(), newNFA.states.begin() + 1);

            // push to stack
            nfaStack.push(newNFA);
        }

        else if (c == '?')
        {
            if (nfaStack.empty())
                throw runtime_error("Invalid regular expression");
            
            nfa NFA = nfaStack.top();
            nfaStack.pop();

            // create two states
            nfaNode start(stateCounter++);
            start.acceptance = false;

            nfaNode end(stateCounter++);
            end.acceptance = true;

            // join new start state to the initial state of NFA and the new end state
            start.transition['e'].insert(NFA.initialState);
            start.transition['e'].insert(end.state);

            // join final states of NFA to the new end state
            for (nfaNode &node : NFA.states) 
            {
                if (node.acceptance) {
                    node.acceptance = false;
                    node.transition['e'].insert(end.state);
                }
            }

            // The new NFA has all states of nfa1 and the new start and end states
            nfa newNFA(NFA.states.size() + 2, start.state);
            newNFA.states[start.state] = start;
            newNFA.states[end.state] = end;
            copy(NFA.states.begin(), NFA.states.end(), newNFA.states.begin() + 1);

            // push on stack
            nfaStack.push(newNFA);
        }
    }

    return nfaStack.top();
}

int main() 
{
    string regex = "ab|";
    nfa result = regexToNFA(regex);

    // Add code to print or inspect the resulting NFA here

    return 0;
}

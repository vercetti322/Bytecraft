#include<bits/stdc++.h>

using namespace std;

// alphabet set : ('a', 'b')

struct nfaNode
{
    int state;
    bool acceptance; // to check for final state
    map <char, set <int>> transition;

    // constructor to initialize nfaNode
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
    for (int i = 0; i < regex.size(); i++)
    {
        char c = regex[i];

        // if c is a symbol from alphabet
        if (c == 'a' || c == 'b')
        {
            // create two nfa states (x -> y)
            nfaNode start;
            start.state = stateCounter++;
            start.acceptance = false;

            nfaNode end;
            end.state = stateCounter++;
            end.acceptance = true;

            // make transition
            start.transition[c].insert(end.state);

            // fill in nfa
            nfa NFA(0, 0); 
            NFA.states.push_back(start);
            NFA.states.push_back(end);
            NFA.initialState = start.state;
            NFA.states[0].acceptance = false; 
            NFA.states[1].acceptance = true;

            // push to stack
            nfaStack.push(NFA);

        }

        else if (c == '.')
        {
            if (nfaStack.size() < 2)
                throw runtime_error("Invalid regular expression");

            nfa NFA2 = nfaStack.top();
            nfaStack.pop();

            nfa NFA1 = nfaStack.top();
            nfaStack.pop();

            // join final states of NFA1 to initial of NFA2
            for (nfaNode &node : NFA1.states)
            {
                if (node.acceptance)
                {
                    node.acceptance = false;
                    node.transition['e'].insert(NFA2.initialState);
                }
            }

            // create nfa
            nfa NFA(0, 0);
            NFA.states.reserve(NFA1.states.size() + NFA2.states.size());
            NFA.states.resize(NFA1.states.size() + NFA2.states.size());

            // Copy states from NFA1 to NFA, excluding start and end states
            for (int i = 0; i < NFA1.states.size(); i++)
            {
                NFA.states[i] = NFA1.states[i];
            }

            // Copy states from NFA2 to NFA, excluding start and end states
            for (int i = 0; i < NFA2.states.size(); i++)
            {
                NFA.states[i + NFA1.states.size()] = NFA2.states[i];
                if (NFA2.states[i].acceptance)
                        NFA.states[i + NFA1.states.size()].acceptance = true;
            }

            NFA.initialState = NFA1.initialState;

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
            nfaNode start;
            start.state = stateCounter++;

            nfaNode end;
            end.state = stateCounter++;
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
            nfa NFA(NFA1.states.size() + NFA2.states.size() + 2, start.state);
            NFA.states[start.state] = start;
            NFA.states[end.state] = end;

            // Copy states from NFA1 to NFA, excluding start and end states
            for (int i = 0; i < NFA1.states.size(); i++)
            {
                if (i != start.state && i != NFA1.states.size() && i != end.state)
                {
                    NFA.states[i] = NFA1.states[i];
                }
            }

            // Copy states from NFA2 to NFA, excluding start and end states
            for (int i = 0; i < NFA2.states.size(); i++)
            {
                if (i != start.state && i != NFA2.states.size() && i != end.state)
                {
                    NFA.states[i + NFA1.states.size()] = NFA2.states[i];
                }
            }

            NFA.initialState = start.state;
            
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
            nfaNode start;
            start.state = stateCounter++;
            start.acceptance = false;

            nfaNode end;
            end.state = stateCounter++;
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
            // copy(NFA.states.begin(), NFA.states.end(), newNFA.states.begin() + 1);

            for (int i = 0; i < NFA.states.size(); i++)
            {
                if (i != start.state && i != end.state)
                {
                    newNFA.states[i] = NFA.states[i];
                }
            }

            newNFA.initialState = start.state;

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
            nfaNode start;
            start.state = stateCounter++;
            start.acceptance = false;

            nfaNode end;
            end.state = stateCounter++;
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
            
            for (int i = 0; i < NFA.states.size(); i++)
            {
                if (i != start.state && i != end.state)
                {
                    newNFA.states[i] = NFA.states[i];
                }
            }

            newNFA.initialState = start.state;

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
            nfaNode start;
            start.state = stateCounter++;
            start.acceptance = false;

            nfaNode end;
            end.state = stateCounter++;
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
            
            for (int i = 0; i < NFA.states.size(); i++)
            {
                if (i != start.state && i != end.state)
                {
                    newNFA.states[i] = NFA.states[i];
                }
            }

            newNFA.initialState = start.state;

            // push on stack
            nfaStack.push(newNFA);
        }
    }

    return nfaStack.top();
}

int main() 
{
    string regex = "ab.";
    nfa result = regexToNFA(regex);

    for (nfaNode &node : result.states)  // Use references here
    {
        if (node.state == result.initialState)
            cout << "Start State: " << node.state << endl;
        else if (node.acceptance)
            cout << "Final State: " << node.state << endl;

        else
            cout << "State: " << node.state << endl;
        cout << "Acceptance: " << node.acceptance << endl;
        for (auto it : node.transition)
        {
            cout << "Transition: " << it.first << " -> ";
            for (int i : it.second)
            {
                cout << i << " ";
            }
            cout << endl;
        }
        cout << endl;
    }
    return 0;
}
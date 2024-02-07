/*
    Program which checks if an input string w => {0, 1}* is accepted by given DFA or not
*/

#include<unordered_map>
#include<iostream>
#include<algorithm>
#include<vector>
#include<string>

using namespace std;

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

// function to check acceptance of a string by dfa
bool dfaAcceptance(dfa DFA, string input)
{
    int currentState = DFA.initialState;

    // iterating over...
    for (char c : input)
    {
        if (DFA.states[currentState].transition.find(c) == DFA.states[currentState].transition.end())
        {
            cout << "Invalid input!" << endl;
            return false;
        }

        currentState = DFA.states[currentState].transition[c];
    }

    if (DFA.states[currentState].acceptance)
        return true;

    return false;
}

int main()
{
    cout << "enter a string: ";
    string s;
    cin >> s;

    cout << "enter number of states: ";
    int numStates;
    cin >> numStates;
    int initalState = 0;
    char alphabets[2] = {'0', '1'};

    cout << "enter number of final states: ";
    int finalStates;
    cin >> finalStates;
    vector <int> acceptingStates;

    cout << "enter " << finalStates << " final states" << endl;
    for (int i = 0; i < finalStates; i++)
    {
        int state;
        cin >> state;
        acceptingStates.push_back(state);
    }

    // define the dfa
    dfa DFA(numStates, initalState);

    // populate the dfa
    for (int i = 0; i < numStates; i++)
    {
        cout << "Enter " << i << "th transition" << endl;
        cout << i << ": {";

        DFA.states[i].state = i;

        // make the acceptance true if i lies in finalStates
        if (find(acceptingStates.begin(), acceptingStates.end(), i) != acceptingStates.end())
            DFA.states[i].acceptance = true;

        for (char c : alphabets)
        {
            if (c == '0')
                cout << c << ":";
            else
                cout << ", " << c << ":";
            int trastate;
            cin >> trastate;
            DFA.states[i].transition.insert({c, trastate});
        }
    }

    // check the acceptance
    bool result = dfaAcceptance(DFA, s);

    if (result)
        cout << "String accepted!" << endl;
    
    else
        cout << "String not accepted!" << endl;

}

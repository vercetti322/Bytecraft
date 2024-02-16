#include <iostream>
#include <stack>
#include <vector>

using namespace std;

// Structure to represent a state in the NFA
struct State {
    char symbol;
    State* out1;
    State* out2;
};

// Function to create a new state
State* newState(char symbol, State* out1 = nullptr, State* out2 = nullptr) {
    State* state = new State;
    state->symbol = symbol;
    state->out1 = out1;
    state->out2 = out2;
    return state;
}

// Function to convert postfix regular expression to NFA
State* postfixToNFA(const string& postfix) {
    stack<State*> nfaStack;

    for (char c : postfix) {
        if (c == '.') {
            State* second = nfaStack.top();
            nfaStack.pop();
            State* first = nfaStack.top();
            nfaStack.pop();

            first->out2 = second;
            nfaStack.push(first);
        } else if (c == '|') {
            State* second = nfaStack.top();
            nfaStack.pop();
            State* first = nfaStack.top();
            nfaStack.pop();

            State* start = newState('\0', first, second);
            State* end = newState('\0');
            first->out1 = end;
            second->out1 = end;

            nfaStack.push(start);
            nfaStack.push(end);
        } else if (c == '*') {
            State* top = nfaStack.top();
            nfaStack.pop();

            State* start = newState('\0', top, nullptr);
            State* end = newState('\0');
            top->out1 = start;
            top->out2 = end;

            nfaStack.push(start);
            nfaStack.push(end);
        } else {
            State* state = newState(c);
            nfaStack.push(state);
        }
    }

    State* nfa = nfaStack.top();
    nfaStack.pop();

    return nfa;
}

 void printNFA(State* state) {
        if (state == nullptr) {
            return;
        }

        cout << "State: " << state << ", Symbol: " << state->symbol << endl;
        cout << "Out1: " << state->out1 << ", Out2: " << state->out2 << endl;

        printNFA(state->out1);
        printNFA(state->out2);
    }

int main() {
    string postfix = "ab.c|*";
    State* nfa = postfixToNFA(postfix);

    // TODO: Perform operations on the NFA

    // Call the printNFA function
    printNFA(nfa);
    

    return 0;
}

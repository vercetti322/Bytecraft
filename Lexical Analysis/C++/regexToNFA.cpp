// function to convert regex string to eNFA
#include <bits/stdc++.h>

using namespace std;

// store nfa
struct nfa
{
    int initialState;
    mutable map <int, map <char, set <int>>> transition; // state --> symbol --> {set of next states}
    set <int> finalStates;
};

// store dfa
struct dfa
{
    set <int> initialStates;
    mutable map <int, map <char, int>> transition; // state --> symbol --> next state
    set <int> finalStates;
};

// store input.txt
struct input
{
    string str;
    vector <string> regex;
};

// Function to read input from a file
vector <input> readInputFromFile(const std::string& filename)
{
    vector <input> inputs;

    // Open the input file
    std::ifstream file(filename);
    if (!file.is_open()) 
    {
        // Failed to open file
        std::cerr << "Error: Unable to open file " << filename << std::endl;
        return inputs; // Return empty vector
    }

    // Read the first line containing the string
    string str;
    if (std::getline(file, str))
    {
        input inp;
        inp.str = str;

        // Read the remaining lines containing regexes
        std::string regex;
        while (std::getline(file, regex)) {
            inp.regex.push_back(regex);
        }

        // Add the input to the vector
        inputs.push_back(inp);
    }

    // Close the file
    file.close();
    return inputs;
}


// convert to postfix
string insertConcat(string s)
{
    string result;
    for (size_t i = 0; i < s.size(); ++i) 
    {
        result += s[i];
        if (i < s.size() - 1 && (isalnum(s[i]) || s[i] == ')' || s[i] == '*') && (isalnum(s[i + 1]) || s[i + 1] == '('))
            result += '.';
    }

    return result;
}

// a character is an operator or not
bool isOperator(char c) 
{
    return (c == '+' || c == '*' || c == '|' || c == '?' || c == '.');
}

// precedence of an operator
int opPrecedence(char op) 
{
    if (op == '+' || op == '?')
        return 1;

    else if (op == '*')
        return 2;

    else if (op == '|')
        return 3;

    else if (op == '.')
        return 4;

    return 0; 
}

// Function to convert infix expression to postfix
string infixToPostfix(string& infix) 
{
    string postfix;
    stack <char> stack;

    for (int i = 0; i < infix.size(); i++) 
    {
        char c = infix[i];

        if (isalnum(c)) 
        {
            postfix += c;
            // If the next character is also an operand or an opening parenthesis, append a concatenation operator to the output string.
            if (i + 1 < infix.size() && (isalnum(infix[i + 1]) || infix[i + 1] == '(') && infix[i + 1] != ')')
            {
                postfix += '.';
                stack.push('.');
            }
        }
        
        else if (c == '(')
            stack.push(c);

        else if (c == ')') 
        {
            while (!stack.empty() && stack.top() != '(') 
            {
                postfix += stack.top();
                stack.pop();
            }

            stack.pop(); // pop '('

            // If the next character is also an operand or an opening parenthesis, append a concatenation operator to the output string.
            if (i + 1 < infix.size() && (isalnum(infix[i + 1]) || infix[i + 1] == '(') && infix[i + 1] != ')')
            {
                postfix += '.';
                stack.push('.');
            }
        } 
        
        else if (isOperator(c)) 
        {
            while (!stack.empty() && opPrecedence(c) <= opPrecedence(stack.top())) 
            {
                postfix += stack.top();
                stack.pop();
            }

            stack.push(c);
        }
    }

    while (!stack.empty()) 
    {
        postfix += stack.top();
        stack.pop();
    }

    return postfix;
}

// convert postfix regex to nfa
nfa regexToNFA(string s)
{
    // convert to postfix
    s = insertConcat(s);
    string str = infixToPostfix(s);
    cout << str << endl;

    // stack to keep nfa's
    stack <nfa> nfaStack;

    // indexing the state
    int counter = 0;

    // iterate over string
    for (int i = 0; i < str.size(); i++)
    {
        char c = str[i];

        // if its a single character
        if (c == 'a' || c == 'b')
        {
            // create a new 2 states nfa and push it to the stack
            nfa NFA;
            NFA.initialState = counter++;
            NFA.finalStates = {counter++};
            NFA.transition[NFA.initialState].insert({c, NFA.finalStates});

            // push it to stack
            nfaStack.push(NFA);
        }

        // if its union '|'
        else if (c == '|')
        {
            // pop last two nfa's
            nfa second = nfaStack.top(); nfaStack.pop();
            nfa first = nfaStack.top(); nfaStack.pop();

            // make a new start and a new NFA
            nfa NFA;
            NFA.initialState = counter++;

            // make the new end state
            int end = counter++;
            NFA.finalStates = {end};

            // connect it to start states of second and first
            NFA.transition[NFA.initialState] = {{'e', {first.initialState, second.initialState}}};

            // connect end states of first and second to this new end
            for (int state : first.finalStates)
            {
                if (NFA.transition.find(state) == NFA.transition.end())
                    NFA.transition[state] = {{'e', {end}}};
                else
                    NFA.transition[state]['e'].insert(end);
            }
            
            for (int state : second.finalStates)
            {
                if (NFA.transition.find(state) == NFA.transition.end())
                    NFA.transition[state] = {{'e', {end}}};
                else
                    NFA.transition[state]['e'].insert(end);
            }

            // add remaining states of first and second to NFA, including epsilon transitions
            for (auto &it : first.transition)
            {

                for (auto &trans : it.second)
                {
                    NFA.transition[it.first][trans.first].insert(trans.second.begin(), trans.second.end());
                }
            }

            for (auto &it : second.transition)
            {
                for (auto &trans : it.second)
                {
                    NFA.transition[it.first][trans.first].insert(trans.second.begin(), trans.second.end());
                }
            }

            // push it to stack
            nfaStack.push(NFA);
        }

        else if (c == '.')
        {
            // pop last two nfa's
            nfa second = nfaStack.top(); nfaStack.pop();
            nfa first = nfaStack.top(); nfaStack.pop();

            // make a new start and a new NFA
            nfa NFA;

            // make the new start and end states
            NFA.initialState = first.initialState;
            NFA.finalStates = second.finalStates;

            // add remaining states of first and second to NFA, including epsilon transitions
            for (auto &it : first.transition)
            {
                for (auto &trans : it.second)
                {
                    NFA.transition[it.first][trans.first].insert(trans.second.begin(), trans.second.end());
                }
            }

            for (auto &it : second.transition)
            {
                for (auto &trans : it.second)
                {
                    NFA.transition[it.first][trans.first].insert(trans.second.begin(), trans.second.end());
                }
            }

            // connect last states of first to start state of second
            for (int state : first.finalStates)
            {
                NFA.transition[state]['e'].insert(second.initialState);
            }

            // push it to stack
            nfaStack.push(NFA);
        }

        else if (c == '*')
        {
            // pop the last state
            nfa last = nfaStack.top(); nfaStack.pop();

            // from end state to start state
            for (int x : last.finalStates)
            {
                last.transition[x]['e'].insert(last.initialState);
            }

            // make epsilon from new first state to end of last
            last.transition[last.initialState]['e'].insert(last.finalStates.begin(), last.finalStates.end());
            nfaStack.push(last);
        }

        else if (c == '+')
        {
            // pop the last state
            nfa last = nfaStack.top(); nfaStack.pop();

            // make epsilon from end of last to new first state
            for (int x : last.finalStates)
            {
                last.transition[x]['e'].insert(last.initialState);
            }
            
            // push it to stack
            nfaStack.push(last);
        }

        else if (c == '?')
        {
            // pop the last state
            nfa last = nfaStack.top(); nfaStack.pop();

            // make epsilon from new first state to end of last
            last.transition[last.initialState]['e'].insert(last.finalStates.begin(), last.finalStates.end());

            // push it to stack
            nfaStack.push(last);
        }
    }

    return nfaStack.top();
}

// convert enfa to dfa
set <int> eClosure(int state, map <int, map <char, set <int>>> transition)
{
    set <int> closure = {state};
    queue <int> q;
    q.push(state);

    while (!q.empty())
    {
        int x = q.front(); q.pop();
        for (int y : transition[x]['e'])
        {
            if (closure.find(y) == closure.end())
            {
                closure.insert(y);
                q.push(y);
            }
        }
    }

    return closure;
}

// Function to convert ENFA to NFA
nfa eNFAToNFA(const nfa& enfa)
{
    nfa nfa;
    nfa.initialState = enfa.initialState;

    // Iterate over each state in the ENFA
    for (int state = 0; state < enfa.transition.size(); ++state)
    {
        // Compute epsilon closure for the current state
        set <int> currentStateEpsilonClosure = eClosure(state, enfa.transition);

        // Iterate over input symbols to construct transitions
        for (char symbol : {'a', 'b'}) // assuming input alphabet is lowercase letters
        {
            set <int> nextState;
            for (int currentState : currentStateEpsilonClosure)
            {
                auto it = enfa.transition[currentState].find(symbol);
                if (it != enfa.transition[currentState].end())
                    nextState.insert(it->second.begin(), it->second.end());
            }

            // Compute epsilon closure of the next state
            set<int> nextStateEpsilonClosure;
            for (int state : nextState)
            {
                set<int> closure = eClosure(state, enfa.transition);
                nextStateEpsilonClosure.insert(closure.begin(), closure.end());
            }

            // Add transition from current state to the next state in the NFA
            if (!nextStateEpsilonClosure.empty())
                nfa.transition[state][symbol] = nextStateEpsilonClosure;
        }
    }

    for (int state = 0; state < enfa.transition.size(); ++state)
    {
        set <int> currentStateEpsilonClosure = eClosure(state, enfa.transition);
        for (int finalState : enfa.finalStates)
        {
            if (currentStateEpsilonClosure.find(finalState) != currentStateEpsilonClosure.end())
            {
                nfa.finalStates.insert(state);
                break;
            }
        }
    }

    return nfa;
}

// get final states for converted nfa from enfa
set <int> getfinalStates(nfa eNFA)
{
    for (int i = 0; i < eNFA.transition.size(); i++)
    {
        // get epsilon closure of each state
        set <int> closure = eClosure(i, eNFA.transition);

        // if it contains a final state, add it to the final states
        for (int x : closure)
        {
            if (eNFA.finalStates.find(x) != eNFA.finalStates.end())
            {
                eNFA.finalStates.insert(i);
                break;
            }
        }
    }

    return eNFA.finalStates;
}

// Function to remove unreachable states from an NFA
void removeUnreachableStates(nfa& nfa)
{
    // Set to store reachable states
    std::unordered_set<int> reachableStates;

    // Queue for BFS traversal
    std::queue<int> stateQueue;
    stateQueue.push(nfa.initialState);
    reachableStates.insert(nfa.initialState);

    // BFS traversal to find reachable states
    while (!stateQueue.empty())
    {
        int currentState = stateQueue.front();
        stateQueue.pop();

        // Iterate over transitions from the current state
        for (const auto& transition : nfa.transition[currentState])
        {
            for (int nextState : transition.second)
            {
                // Add the next state to the set of reachable states if it hasn't been visited yet
                if (reachableStates.find(nextState) == reachableStates.end())
                {
                    reachableStates.insert(nextState);
                    stateQueue.push(nextState);
                }
            }
        }
    }

    // Remove unreachable states from the NFA
    for (auto it = nfa.transition.begin(); it != nfa.transition.end();)
    {
        if (reachableStates.find(it->first) == reachableStates.end())
        {
            it = nfa.transition.erase(it);
        }
        else
        {
            ++it;
        }
    }

    // Remove unreachable states from the set of final states
    for (auto it = nfa.finalStates.begin(); it != nfa.finalStates.end();)
    {
        if (reachableStates.find(*it) == reachableStates.end())
        {
            it = nfa.finalStates.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

// convert nfa to dfa
dfa nfaToDFA(const nfa& NFA, const nfa& eNFA)
{
    dfa dfa;
    map <set <int>, int> stateMapping;

    // initialize work queue
    queue <set <int>> workQueue;
    set <int> initial = {NFA.initialState};
    workQueue.push(initial);
    stateMapping[initial] = 0;
    dfa.initialStates.insert(stateMapping[initial]);
    set <int> finalNFA = getfinalStates(eNFA);

    // process the states untill queue is empty
    while (!workQueue.empty())
    {
        set <int> current = workQueue.front(); workQueue.pop();

        // check if its a final state
        bool isfinal = false;
        for (int x : current)
            if (finalNFA.count(x) > 0)
            {
                isfinal = true;
                break;
            }

        // add current state to dfa
        int currentIndex = stateMapping[current];
        if (isfinal)
            dfa.finalStates.insert(currentIndex);

        if (current == initial)
            dfa.initialStates.insert(currentIndex);

        // generate transitions for each symbol
        for (char symbol : {'a', 'b'})
        {
            set <int> next;
            for (int state : current)
            {
                auto it = NFA.transition[state].find(symbol);
                if (it != NFA.transition[state].end())
                    next.insert(it->second.begin(), it->second.end());
            }

            // add above transition to DFA
            if (!next.empty())
            {
                // Check if next state already exists in DFA
                    if (stateMapping.find(next) == stateMapping.end()) {
                        int nextStateIndex = stateMapping.size();
                        stateMapping[next] = nextStateIndex;
                        workQueue.push(next);
                    }

                    dfa.transition[currentIndex][symbol] = stateMapping[next];
            }
        }
    }

    return dfa;
}

// get largest prefix accepted by dfa from input string starting from some index
string getLargestPrefix(const dfa& dfa, const string& input, int index)
{
    int currentState = 0;
    int lastAcceptingState = -1;
    int lastAcceptingIndex = -1;

    for (int i = index; i < input.size(); ++i)
    {
        char symbol = input[i];
        auto it = dfa.transition[currentState].find(symbol);
        if (it == dfa.transition[currentState].end())
        {
            break;
        }

        currentState = it->second;

        if (dfa.finalStates.find(currentState) != dfa.finalStates.end())
        {
            lastAcceptingState = currentState;
            lastAcceptingIndex = i;
        }
    }

    if (lastAcceptingState != -1)
    {
        return input.substr(index, lastAcceptingIndex - index + 1);
    }
    else
    {
        return "";
    }
}


int main() 
{
    std::vector<input> inputs = readInputFromFile("input.txt");
    if (inputs.empty()) {
        return 1;
    }

    // print input from file
    for (const input& inp : inputs)
    {
        cout << "String: " << inp.str << endl;
        for (const std::string& regex : inp.regex)
        {
            cout << regex << endl;
        }
    }

    /*
        iterate over the input string, start from first index
        and keep finding the maximum of largest prefix accepted by regexes given
        and then move to the next index, if the prefix is empty, move to the next index
        and if the prefix is not empty, move to the next index after the prefix and if
        multiple regexes accept the same length prefix, choose the one which comes first
        and if no regex accepts the prefix, move to the next index
    */
    vector <dfa> dfas;
    for (const std::string& regex : inputs[0].regex)
    {
        nfa eNFA = regexToNFA(regex);
        nfa NFA = eNFAToNFA(eNFA);
        removeUnreachableStates(NFA);
        dfa DFA = nfaToDFA(NFA, eNFA);
        dfas.push_back(DFA);
    }
        
    for (const input& inp : inputs)
    {
        // Iterate over the input string
        std::string input = inp.str;
        int index = 0;
        while (index < input.size() && index >= 0)
        {
            cout << "Index: " << index << endl;
            std::string largestPrefix;
            int regexIndex = -1;

            // Find the largest prefix accepted by the regexes
            for (int i = 0; i < dfas.size(); ++i)
            {
                std::string prefix = getLargestPrefix(dfas[i], input, index);
                if (prefix.size() > largestPrefix.size() || 
        (prefix.size() == largestPrefix.size() && i > regexIndex))
                {
                    largestPrefix = prefix;
                    regexIndex = i;
                }
            }

            // Update the index
            if (largestPrefix.empty())
                ++index;
            else
                index += largestPrefix.size();

            std::ofstream file("output.txt", std::ios::app);
            if (file.is_open())
            {
                if (largestPrefix.empty() && index <= input.size())
                {
                    // output the string at the index
                    file << "<" << input[index - 1] << "," << 0 << ">";
                }
                else
                {
                    // Output the largest prefix accepted by the regex
                    file << "<" << largestPrefix << "," << regexIndex + 1 << ">";
                }

                file.close();
            }

            else
            {
                std::cerr << "Error: Unable to open file output.txt" << std::endl;
                return 1;
            }
        }
    }

    return 0;
}
#include <bits/stdc++.h>

using namespace std;

string r,w;
int finalStateENFA;
set <int> finalStatesNFA;
FILE* outputfile;

// function to get eNFA from given regex
vector <vector <int>> generateNFA(string r)
{
    vector <vector <int>> nfaTrans;
    int startState = 0;
    int nextState = 1;
    map <int, int> stateIndex;

    // going over string
    for(int i = 0; i < r.length(); i++)
    {
        stateIndex[i] = startState;

        if(r[i] == 'a')
        {
            vector <int> aTrans;
            aTrans.push_back(startState);
            aTrans.push_back(2);
            aTrans.push_back(nextState);
            nfaTrans.push_back(aTrans);
            startState = nextState;
            nextState++;
            aTrans.clear();
            aTrans.push_back(startState);
            aTrans.push_back(1);
            aTrans.push_back(nextState);
            nfaTrans.push_back(aTrans);
            startState = nextState;
            nextState++;
            aTrans.clear();
            aTrans.push_back(startState);
            aTrans.push_back(2);
            aTrans.push_back(nextState);
            nfaTrans.push_back(aTrans);
            startState=nextState;
            nextState++;
        }

        else if(r[i] == 'b')
        {
            vector <int> bTrans;
            bTrans.push_back(startState);
            bTrans.push_back(2);
            bTrans.push_back(nextState);
            nfaTrans.push_back(bTrans);
            startState = nextState;
            nextState++;
            bTrans.clear();
            bTrans.push_back(startState);
            bTrans.push_back(0);
            bTrans.push_back(nextState);
            nfaTrans.push_back(bTrans);
            startState = nextState;
            nextState++;
            bTrans.clear();
            bTrans.push_back(startState);
            bTrans.push_back(2);
            bTrans.push_back(nextState);
            nfaTrans.push_back(bTrans);
            startState = nextState;
            nextState++;
        }

        else if(r[i] == '*')
        {
            vector <int> sTrans;
            int ss = startState;
            stack <char> currexp;
            currexp.push(')');
            int it = i-2;
            for (int j = nfaTrans.size()-1; j > -1 ; j--)
            {
                if (nfaTrans[j][2] == ss && nfaTrans[j][1] == 2 && nfaTrans[j][0] < ss)
                    ss=nfaTrans[j][0];
            }

            while(currexp.size()  != 0)
            {
                if(r[it] == 'a')
                {
                    for(int j = nfaTrans.size()-1; j > -1; j--)
                        if(nfaTrans[j][2] == ss && nfaTrans[j][1] == 1)
                            ss = nfaTrans[j][0];

                    for(int j = nfaTrans.size()-1; j > -1; j--)
                        if(nfaTrans[j][2] == ss && nfaTrans[j][1] == 2 && nfaTrans[j][0] < ss)
                            ss = nfaTrans[j][0];

                }

                else if(r[it]=='b')
                {
                    for(int j = nfaTrans.size()-1; j > -1; j--)
                        if (nfaTrans[j][2] == ss && nfaTrans[j][1] == 0)
                            ss = nfaTrans[j][0];
                    
                    for(int j = nfaTrans.size()-1; j >-1; j--)
                        if (nfaTrans[j][2] == ss && nfaTrans[j][1] == 2 && nfaTrans[j][0] < ss)
                            ss = nfaTrans[j][0];
                    
                }

                else if (r[it] == ')')
                    currexp.push(')');

                else if (r[it] == '(')
                    currexp.pop();

                --it;
            }

            int endState = startState;
            for(int j = nfaTrans.size()-1;j>-1;j--)
                if(nfaTrans[j][2] == endState && nfaTrans[j][1]==2 && nfaTrans[j][0]<ss)
                    endState=nfaTrans[j][0];

            for(int j = 0; j<nfaTrans.size(); j++)
                if(nfaTrans[j][0]==ss && nfaTrans[j][1]==2)
                    ss=nfaTrans[j][2];

            sTrans.push_back(endState - 1);
            sTrans.push_back(2);
            sTrans.push_back(ss);
            sTrans.push_back(-111);
            nfaTrans.push_back(sTrans);
            sTrans.clear();
            sTrans.push_back(ss);
            sTrans.push_back(2);
            sTrans.push_back(endState - 1);
            sTrans.push_back(-111);
            nfaTrans.push_back(sTrans);
        }

        else if (r[i] == '+')
        {
            vector <int> pTrans;
            int ss = startState;
            stack <char> currexp;
            currexp.push(')');
            int it = i-2;
            for (int j = nfaTrans.size()-1; j > -1 ; j--)
            {
                if (nfaTrans[j][2] == ss && nfaTrans[j][1] == 2 && nfaTrans[j][0] < ss)
                    ss=nfaTrans[j][0];
            }

            while(currexp.size() != 0)
            {
                if(r[it] == 'a')
                {
                    for(int j = nfaTrans.size()-1; j > -1; j--)
                        if(nfaTrans[j][2] == ss && nfaTrans[j][1] == 1)
                            ss = nfaTrans[j][0];

                    for(int j = nfaTrans.size()-1; j > -1; j--)
                        if(nfaTrans[j][2] == ss && nfaTrans[j][1] == 2 && nfaTrans[j][0] < ss)
                            ss = nfaTrans[j][0];

                }

                else if(r[it]=='b')
                {
                    for(int j = nfaTrans.size()-1; j > -1; j--)
                        if (nfaTrans[j][2] == ss && nfaTrans[j][1] == 0)
                            ss = nfaTrans[j][0];
                    
                    for(int j = nfaTrans.size()-1; j >-1; j--)
                        if (nfaTrans[j][2] == ss && nfaTrans[j][1] == 2 && nfaTrans[j][0] < ss)
                            ss = nfaTrans[j][0];
                    
                }

                else if (r[it] == ')')
                    currexp.push(')');

                else if (r[it] == '(')
                    currexp.pop();

                --it;
            }

            int endState = startState;
            for(int j = nfaTrans.size()-1;j>-1;j--)
                if(nfaTrans[j][2] == endState && nfaTrans[j][1]==2 && nfaTrans[j][0]<ss)
                    endState=nfaTrans[j][0];

            for(int j = 0; j<nfaTrans.size(); j++)
                if(nfaTrans[j][0]==ss && nfaTrans[j][1]==2)
                    ss=nfaTrans[j][2];
            
            pTrans.push_back(endState - 1);
            pTrans.push_back(2);
            pTrans.push_back(ss);
            pTrans.push_back(-999);
            nfaTrans.push_back(pTrans);
        }

        else if(r[i] == '?')
        {
            vector <int> sTrans;
            int ss = startState;
            stack <char> currexp;
            currexp.push(')');
            int it = i-2;
            for (int j = nfaTrans.size()-1; j > -1 ; j--)
            {
                if (nfaTrans[j][2] == ss && nfaTrans[j][1] == 2 && nfaTrans[j][0] < ss)
                    ss=nfaTrans[j][0];
            }

            while(currexp.size()  != 0)
            {
                if(r[it] == 'a')
                {
                    for(int j = nfaTrans.size()-1; j > -1; j--)
                        if(nfaTrans[j][2] == ss && nfaTrans[j][1] == 1)
                            ss = nfaTrans[j][0];

                    for(int j = nfaTrans.size()-1; j > -1; j--)
                        if(nfaTrans[j][2] == ss && nfaTrans[j][1] == 2 && nfaTrans[j][0] < ss)
                            ss = nfaTrans[j][0];

                }

                else if(r[it]=='b')
                {
                    for(int j = nfaTrans.size()-1; j > -1; j--)
                        if (nfaTrans[j][2] == ss && nfaTrans[j][1] == 0)
                            ss = nfaTrans[j][0];
                    
                    for(int j = nfaTrans.size()-1; j >-1; j--)
                        if (nfaTrans[j][2] == ss && nfaTrans[j][1] == 2 && nfaTrans[j][0] < ss)
                            ss = nfaTrans[j][0];
                    
                }

                else if (r[it] == ')')
                    currexp.push(')');

                else if (r[it] == '(')
                    currexp.pop();

                --it;
            }

            int endState = startState;
            for(int j = nfaTrans.size()-1;j>-1;j--)
                if(nfaTrans[j][2] == endState && nfaTrans[j][1]==2 && nfaTrans[j][0]<ss)
                    endState=nfaTrans[j][0];

            for(int j = 0; j<nfaTrans.size(); j++)
                if(nfaTrans[j][0]==ss && nfaTrans[j][1]==2)
                    ss=nfaTrans[j][2];

            sTrans.push_back(ss);
            sTrans.push_back(2);
            sTrans.push_back(endState);
            sTrans.push_back(-111);
            nfaTrans.push_back(sTrans);
        }
    }

    finalStateENFA = nextState - 1;
    stack <int> opBrac;
    vector <vector <int>> Bracs;
    for (int i = 0; i < r.length(); i++)
    {
        if (r[i] == '(')
            opBrac.push(i);

        else if(r[i]==')')
        {
            vector<int> pair;
            pair.push_back(opBrac.top());
            pair.push_back(i);
            Bracs.push_back(pair);
            opBrac.pop();
        }
    }

    for (int i = 0; i < r.length(); i++)
    {
        if(r[i]=='|')
        {
            int op1end = i-1;
            int op1beg = -1;
            int op2beg = i+1;
            int op2end = -1;
            for(int j=0; j < Bracs.size(); j++)
            {
                if(Bracs[j][1] == op1end)
                    op1beg = Bracs[j][0];

                else if(Bracs[j][0]==op2beg)
                    op2end=Bracs[j][1];
            }

            int sop1end=stateIndex[op1end];
            int sop1beg=stateIndex[op1beg];
            int sop2end=stateIndex[op2end];
            int sop2beg=stateIndex[op2beg];
            sop2beg++;

            if(sop1beg!=0)
                sop1beg++;
            vector<int> orTrans;
            orTrans.push_back(sop1beg);
            orTrans.push_back(2);
            orTrans.push_back(sop2beg);
            nfaTrans.push_back(orTrans);
            orTrans.clear();
            orTrans.push_back(sop1end);
            orTrans.push_back(2);
            orTrans.push_back(sop2end);
            nfaTrans.push_back(orTrans);
            for (int j = 0; j < nfaTrans.size(); j++)
            {
                if (nfaTrans[j][0] == sop1end && nfaTrans[j][1] == 2 && nfaTrans[j][2] == sop2beg)
                {
                    nfaTrans.erase(nfaTrans.begin()+j);
                    j--;
                }
            }
        }
    }

    return nfaTrans;
}

// get epsilon closure
set <int> epclosure(vector <vector <int>> nfa, int state)
{
    bool stateExists = false;
    for (int i = 0; i < nfa.size(); i++)
    {
        if (nfa[i][0] == state)
        {
            stateExists = true;
            break;
        }
    }
    if (!stateExists)
    {
        return {state};
    }
    set <int> closure;
    queue <int> st;
    st.push(state);
    closure.insert(state);

    while (!st.empty())
    {
        int top = st.front(); st.pop();

        for (int i = 0; i < nfa.size(); i++)
        {
            if (nfa[i][0] == top && nfa[i][1] == 2) // if the transition is epsilon
            {
                if (find(closure.begin(), closure.end(), nfa[i][2]) == closure.end())
                {
                    closure.insert(nfa[i][2]);
                    st.push(nfa[i][2]);
                }
            }
        }
    }

    return closure;
}

vector<vector<int>> enfaToNFA(vector<vector<int>> enfa)
{
    vector<vector<int>> nfa;
    map<set<int>, int> stateIndex;
    stack<set<int>> q;

    // Start with the epsilon closure of the start state
    set<int> startState = epclosure(enfa, 0);
    q.push(startState);
    stateIndex[startState] = 0;

    // Check if the start state should also be a final state
    if (startState.find(finalStateENFA) != startState.end())
    {
        finalStatesNFA.insert(0);
    }

    while (!q.empty())
    {
        set<int> currentState = q.top(); q.pop();
        for (int i = 0; i < 2; i++)
        {
            set<int> newState;
            for (int state : currentState)
            {
                // Check the transitions by the epsilon closure, the given input, and then the epsilon closure again
                for (int j = 0; j < enfa.size(); j++)
                {
                    if (enfa[j][0] == state && enfa[j][1] == i)
                    {
                        set<int> temp = epclosure(enfa, enfa[j][2]);
                        newState.insert(temp.begin(), temp.end());
                    }
                }
            }

            if (newState.empty())
                continue;

            if (stateIndex.find(newState) == stateIndex.end())
            {
                stateIndex[newState] = stateIndex.size();
                q.push(newState);
            }

            if (newState.find(finalStateENFA) != newState.end())
            {
                finalStatesNFA.insert(stateIndex[newState]);
            }

            vector<int> transition;
            transition.push_back(stateIndex[currentState]);
            transition.push_back(i);
            transition.push_back(stateIndex[newState]);
            nfa.push_back(transition);
        }
    }
    return nfa;
}
int main()
{
    string s;
    cin >> s;
    vector <vector <int>> nfa = generateNFA(s);
    vector <int> finalStatesEpsilonNFA = {finalStateENFA};
    
    // print the nfa
    std::cout << "eNFA" << endl;
    for (int i = 0; i < nfa.size(); i++)
    {
        char symbol;

        if (nfa[i][1] == 0)
            symbol = 'b';

        else if (nfa[i][1] == 1)
            symbol = 'a';

        else
            symbol = 'e';

        std::cout << "State " << nfa[i][0] << "-> " << symbol << " " << "State " << nfa[i][2] << endl;
    }

    // print final states
    std::cout << "Final States: ";
    cout << finalStateENFA << endl;

    vector <vector <int>> newNFA = enfaToNFA(nfa);
    std::cout << "NFA" << endl;
    for (int i = 0; i < newNFA.size(); i++)
    {
        char symbol;
        if (newNFA[i][1] == 0)
            symbol = 'b';

        else if (newNFA[i][1] == 1)
            symbol = 'a';

        else
            symbol = 'e';
        std::cout << "State " << newNFA[i][0] << "-> " << symbol << " " << "State " << newNFA[i][2] << endl;
    }

    // print final states
    std::cout << "Final States: ";
    for (int state : finalStatesNFA)
    {
        std::cout << state << " ";
    }

    // print epsilon closure of eNFA states
    std::cout << "Epsilon Closure of eNFA states" << endl;
    for (auto state : nfa)
    {
        std::cout << "State " << state[0] << ": ";
        set <int> closure = epclosure(nfa, state[0]);
        for (auto state : closure)
        {
            std::cout << state << " ";
        }
        std::cout << endl;
    }
}

// ((((b)|((a)(b)))+)|((((b)|((a)(b)))*)))
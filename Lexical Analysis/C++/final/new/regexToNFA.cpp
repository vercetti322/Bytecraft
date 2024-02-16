#include <bits/stdc++.h>

using namespace std;

string r,w;
int finalStateENFA;
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

            sTrans.push_back(endState);
            sTrans.push_back(2);
            sTrans.push_back(ss);
            sTrans.push_back(-111);
            nfaTrans.push_back(sTrans);
            sTrans.clear();
            sTrans.push_back(ss);
            sTrans.push_back(2);
            sTrans.push_back(endState);
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
            
            pTrans.push_back(endState);
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

int main()
{
    string s;
    cin >> s;
    vector <vector <int>> nfa = generateNFA(s);
    vector <int> finalStatesEpsilonNFA = {finalStateENFA};
    
    // print the nfa
    for (int i = 0; i < nfa.size(); i++)
    {
        char symbol;

        if (nfa[i][1] == 0)
            symbol = 'b';

        else if (nfa[i][1] == 1)
            symbol = 'a';

        else
            symbol = 'e';

        cout << "State " << nfa[i][0] << "-> " << symbol << " " << "State " << nfa[i][2] << endl;
    }

    cout << "Final state: " << finalStateENFA << endl;
        
}

// (((((a)*)(b))(((a)*)((b)((a)*))))*)

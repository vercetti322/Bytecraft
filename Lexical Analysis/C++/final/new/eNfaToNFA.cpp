#include <bits/stdc++.h>

using namespace std;
vector<int> finalStatesNFA; 

// to calculate epsilon closure
void epClosure(vector<vector<int>> nfa, vector<int> &currState){
    queue<int> q;
    for(int i=0;i<currState.size();i++){
        q.push(currState[i]);
    }

    while(!q.empty()){
        int state=q.front();
        q.pop();
        for(int i=0;i<nfa.size();i++){
            if(nfa[i][0]==state && nfa[i][1]==2){
                if(find(currState.begin(),currState.end(),nfa[i][2])==currState.end()){
                    currState.push_back(nfa[i][2]);
                    q.push(nfa[i][2]);
                }
            }
        }
    }

    sort(currState.begin(),currState.end());
}

// convert eNFA to NFA
vector<vector<int>> eNfaToNFA(vector<vector<int>> nfa, vector<int> finalStatesEpsilonNFA)
{
    vector<vector<int>> newNFA;
    for(int i=0;i<nfa.size();i++){
        if (nfa[i][1] == 2){
            vector<int> currState;
            currState.push_back(nfa[i][2]);
            epClosure(nfa,currState);
            for(int j=0;j<currState.size();j++){
                for(int symbol = 0; symbol <= 1; symbol++) {
                    vector<int> temp;
                    temp.push_back(nfa[i][0]);
                    temp.push_back(symbol);
                    temp.push_back(currState[j]);
                    newNFA.push_back(temp);
                }
                // If the current state is a final state in the epsilon-NFA, add it to the final states of the NFA
                if(find(finalStatesEpsilonNFA.begin(), finalStatesEpsilonNFA.end(), currState[j]) != finalStatesEpsilonNFA.end()){
                    finalStatesNFA.push_back(currState[j]);
                }
            }
        }
        else{
            newNFA.push_back(nfa[i]);
        }
    }
    // Remove duplicates from the final states
    sort(finalStatesNFA.begin(), finalStatesNFA.end());
    finalStatesNFA.erase(unique(finalStatesNFA.begin(), finalStatesNFA.end()), finalStatesNFA.end());

    return newNFA;
}

int main(){
    vector<vector<int>> nfa = {{0,2,1},{1,1,2},{2,2,3},{2,2,1}};
    vector<int> finalStatesEpsilonNFA = {3};
    vector<vector<int>> newNFA = eNfaToNFA(nfa, finalStatesEpsilonNFA);
    for(int i=0;i<newNFA.size();i++){
        for(int j=0;j<3;j++){
            cout<<newNFA[i][j]<<" ";
        }
        cout<<endl;
    }
    cout<<endl;

    cout << "Final states of NFA: ";
    for(int i=0; i<finalStatesNFA.size(); i++){
        cout << finalStatesNFA[i] << " ";
    }
    cout << endl;
    
    return 0;
}
#include<iostream>
#include<stack>
#include<string>
#include<vector>
#include<set>
#include<queue>

using namespace std;

class node{
public:
    char input;
    set<int> to;
    node *next;
};

int prec(char c){
    if(c=='*'){
        return 3;
    } else if(c=='.'){
        return 2;
    } else if(c=='+'){
        return 1;
    } else{
        return -1;
    }
}

string post(string s) {
    stack<char> st; 
    st.push('N'); 
    int l = s.length(); 
    string ns; 
    for(int i = 0; i < l; i++) {
        if((s[i] >= 'a' && s[i] <= 'z')||(s[i] >= 'A' && s[i] <= 'Z')){
            ns+=s[i]; 
        } else if(s[i] == '('){          
            st.push('('); 
        } else if(s[i] == ')') { 
            while(st.top() != 'N' && st.top() != '(') { 
                char c = st.top(); 
                st.pop(); 
                ns += c; 
            } 
            if(st.top() == '(') { 
                char c = st.top(); 
                st.pop(); 
            } 
        } else { 
            while(st.top() != 'N' && prec(s[i]) <= prec(st.top())) { 
                char c = st.top(); 
                st.pop(); 
                ns += c; 
            } 
            st.push(s[i]); 
        } 
    } 
    while(st.top() != 'N') { 
        char c = st.top(); 
        st.pop(); 
        ns += c; 
    } 
    return ns;
}

void printnode(vector<node*> v) {
    cout<<"___________________________________________"<<endl;
    cout<<"| from state\t| input\t| tostates"<<endl;
    for(int i=0;i<v.size();i++) {
        cout<<"| "<<i<<"          \t|";
        node* head = v[i];
        cout<<head->input;
        bool first = true;
        for(auto it = head->to.begin(); it != head->to.end(); ++it) {
            if (first) {
                cout<<"     \t|";
                first = false;
            } else {
                cout<<"     \t";
            }
            cout<<*it;
        }
        cout<<endl;
    }
    cout<<"___________________________________________"<<endl;
}

node* makenode(char in) {
    node* a = new node;
    a->input = in;
    a->next = NULL;
    return a;
}

node* copynode(node* a) {
    node* b = new node;
    b->input = a->input;
    b->to = a->to;
    b->next = NULL;
    return b;
}

void andd(vector<node*> &v, vector<vector<int> > &st) {
    int x,y;
    int first,last1;
    y = st[st.size()-1][0];
    x = st[st.size()-2][1];
    first = st[st.size()-2][0];
    last1 = st[st.size()-1][1];

    st.pop_back();
    st.pop_back();

    vector<int> ptemp;
    ptemp.push_back(first);
    ptemp.push_back(last1);
    st.push_back(ptemp);

    node* last = v[y];
    node * lnode= v[x];
    node* temp = copynode(last);

    while(lnode->next!=NULL){
        lnode = lnode->next;
    }
    lnode->next = temp;
}

void orr(vector<node*> &v, vector<vector<int> > &st) {
    int x,y,x1,y1;
    x = st[st.size()-2][0];
    y = st[st.size()-1][0];
    x1 = st[st.size()-2][1];
    y1 = st[st.size()-1][1];
    node* start = makenode('e');
    node* end = makenode('e');
    v.push_back(start);
    int firstnode = v.size() -1;
    v.push_back(end);
    int endnode = v.size() -1;

    st.pop_back();
    st.pop_back();

    vector<int> ptemp;
    ptemp.push_back(firstnode);
    ptemp.push_back(endnode);
    st.push_back(ptemp);

    for(int i=0;i<v.size()-2;i++){
        node* h=v[i];
        while(h->next!=NULL){
            if(h->to.find(x) != h->to.end() || h->to.find(y) != h->to.end()){
                h->to.insert(firstnode);
            }
            h = h->next;
        }
    }

    node* temp = copynode(v[x]);
    node* temp1 = copynode(v[y]);
    node* t = v[firstnode];
    while(t->next!=NULL){
        t = t->next;
    }
    t->to.insert(x);
    t->to.insert(y);
    t->next  = temp;
    t->next->to = temp1->to;
    t->next->next = temp1->next;

    node* adlink = v[x1];
    while(adlink->next!=NULL){
        adlink = adlink->next;
    }
    adlink->to.insert(endnode);
    adlink->next = end;

    node* adlink1 = v[y1];
    while(adlink1->next!=NULL){
        adlink1 = adlink1->next;
    }
    adlink1->to.insert(endnode);
    adlink1->next = end;
}

void closure(vector<node*> &v, vector<vector<int> > &st) {
    int x,x1;
    x = st[st.size()-1][0];
    x1 = st[st.size()-1][1];
    node* s = makenode('e');
    v.push_back(s);
    int firstnode = v.size() -1;

    st.pop_back();
    vector<int> ptemp;
    ptemp.push_back(x);
    ptemp.push_back(firstnode);
    st.push_back(ptemp);

    for(int i=0;i<v.size()-1;i++){
        node* h=v[i];
        while(h->next!=NULL){
            if(h->to.find(x) != h->to.end()){
                h->to.insert(firstnode);
            }
            h = h->next;
        }
    }

    node* t = v[x1];
    while(t->next!=NULL){
        t = t->next;
    }
    t->to.insert(x);
    t->to.insert(firstnode);
    t->next = copynode(t);
    t->next->to = s->to;
    t->next->next = s;
}

int main(){
    string in;
    cout<<"Enter a regular expression\n";
    cin>>in;
    string o;
    vector<node*> v;
    o = post(in);
    cout<<"\npostfix expression is "<< o<<endl;
    vector<vector<int>> st;
    int firstnode = 0;
    for(int l =0 ;l<o.length();l++){
        if(o[l] !='+' && o[l]!='*' && o[l]!='.'){
            node* temp = makenode(o[l]);
            v.push_back(temp);
            vector<int> ptemp;
            ptemp.push_back(v.size()-1);
            ptemp.push_back(v.size()-1);
            st.push_back(ptemp);
        }
        else if(o[l]=='.'){
            andd(v,st);
        }
        else if(o[l]=='+'){
            orr(v,st);
        }
        else if(o[l]=='*'){
            closure(v,st);
        }
    }
    cout<<"\nTransition table for given regular expression is - \n";
    printnode(v);
    cout<<endl;
    cout<<"Starting node is ";
    cout<<st[st.size()-1][0]<<endl;
    cout<<"Ending node is ";
    cout<<st[st.size()-1][1]<<endl;
    return 0;
}

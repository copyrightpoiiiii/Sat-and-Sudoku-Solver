#include"solve.h"

void VarOrder::newVar(){
    h.setBounds(ref_to_assigns.size());
    h.insert(ref_to_assigns.size()-1);
}

void VarOrder::update(int x){
    if(!h.inHeap(x))return;
    else h.increase(x);
}

void VarOrder::undo(int x){
    if(h.inHeap(x))return;
    else h.insert(x);
}

int VarOrder::select(){
    while(!h.empty()){
        int top=h.getmin();
        if(!ref_to_assigns[top])
            return top;
    }
    return -1;
}

inline Clause* Clause_new(myVector<Lit>& ps,bool learnt){
    void* mem=(char *)malloc(sizeof(Clause)-sizeof(Lit)+sizeof(unsigned long)*(ps.size())+(int)learnt);
}

inline GClause GClause_new(Lit p){
    return GClause((void*)(((int)p.x<<1)+1));
}

inline GClause GClause_new(Clause* c){
    return GClause((void*)c);
}


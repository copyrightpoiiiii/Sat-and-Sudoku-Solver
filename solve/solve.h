#ifndef SAT
#define  SAT
#include"../SAT.h"
#endif
#include"../data_structure/heap.hpp"

struct var_info{
    myVector<double>& active;
    bool operator () (int x,int y){
        return active[y] < active[x];
    }
    var_info(myVector<double>& x):active(x){}
};

class Lit{
    public:
        int x;
        Lit(){x=-2;}
        Lit(int var,bool sign){
            x=(var<<1)+(int)sign;
        }
        friend inline Lit operator ~(Lit a){
            a.x^=1;
            return a;
        }
        friend inline bool operator ==(Lit a,Lit b){
            return a.x==b.x;
        }
        friend inline bool operator <(Lit a,Lit b){
            return a.x<b.x;
        }
        friend inline bool sign(Lit a){
            return a.x&1;
        }
        friend inline int var(Lit a){
            return a.x>>1;
        }
        friend inline Lit unsign(Lit a){ 
            a.x = a.x & ~1; 
            return a; 
        }
        friend inline Lit id(Lit a, bool sign){ 
            a.x = a.x ^ (int)sign; 
            return a; 
        }
};

class VarOrder{
    private:
        myVector<double>& ref_to_activity;
        myVector<short>& ref_to_assigns;
        Heap<var_info> h;
    public:
        VarOrder(myVector<short>& value,myVector<double>& activity):
            ref_to_activity(activity),ref_to_assigns(value),h(var_info(activity)){}
        inline void newVar();
        inline void update(int x);
        //void updateAll();
        inline void undo(int x);
        inline int select();
};



class Clause{
    private: 
        Lit data[1];
        unsigned long learned_clause_size;
    public: 
        Clause(myVector<Lit>& ps,int learn){
            for(int i=0;i<ps.size();i++)
                data[i]=ps[i];
            learned_clause_size=learn|(ps.size()<<1);
            if(learn)
                data[(learned_clause_size>>1)].x=0;
        }
        friend Clause* Clause_new(myVector<Lit>& ps,bool learnt);
        int size();
        bool learnt();
        Lit operator [](int i)const{
            return data[i];
        }
        Lit& operator [](int i){
            return data[i];
        }
};

class GClause{
    private:     
        void* data;
        GClause(void* d): data(d){}
    public: 
        friend GClause GClause_new(Lit p);
        friend GClause GClause_new(Clause* c);
        Clause* clause()const{
            return (Clause*)data;
        }
};

class Solver{

};
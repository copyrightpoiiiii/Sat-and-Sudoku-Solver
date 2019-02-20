#include"vector.hpp"

struct info{
    bool exist;
    int size,l1,l2;
};

class Cnf{
    private:
        typedef myVector<int> clause;
    public:
        myVector<clause> cnf_set;
        myVector<int> lit_set;
        myVector<info> clause_info;
        unsigned long literal_size;
        inline Cnf(){
            cnf_set.clear();
            lit_set.clear();
            clause_info.clear();
            literal_size=0;
        }
        inline void init(){
            lit_set.capacity(literal_size);
            clause_info.capacity(cnf_set.size());
            for(int i=0;i<literal_size;i++)
                lit_set[i]=UNDEFINE;
            for(int i=0;i<cnf_set.size();i++){
                clause_info[i].exist=true;
                clause_info[i].size=clause_info[i].l2=cnf_set[i].size();
                clause_info[i].l1=0;
            }
        }
        inline void clear(){
            cnf_set.clear();
            lit_set.clear();
            clause_info.clear();
            literal_size=0;
        }
};
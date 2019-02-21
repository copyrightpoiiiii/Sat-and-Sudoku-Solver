#include"vector.hpp"

struct info{
    bool exist;
    int size,l1,l2;
};

class Cnf{
    private:
        typedef myVector<int> clause;
        typedef myVector<int> rec_lit_in_clause;
    public:
        myVector<clause> cnf_set;
        myVector<int> lit_set;
        myVector<info> clause_info;
        myVector<rec_lit_in_clause> rec_pos_lit;
        myVector<rec_lit_in_clause> rec_neg_lit;
        unsigned long literal_size;
        unsigned long clause_size;
        inline Cnf(){
            cnf_set.clear();
            lit_set.clear();
            clause_info.clear();
            rec_pos_lit.clear();
            rec_neg_lit.clear();
            literal_size=0;
            clause_size=0;
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
            rec_pos_lit.clear();
            rec_neg_lit.clear();
            clause_info.clear();
            literal_size=0;
            clause_size=0;
        }
        bool empty(){
            return clause_size==0;
        }
};
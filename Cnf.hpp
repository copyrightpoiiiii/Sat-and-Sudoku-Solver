#include"vector.hpp"
class Cnf{
    private:
        typedef myVector<int> clause;
    public:
        myVector<clause> cnf_set;
        myVector<int> lit_set;
        myVector<bool> clause_exist;
        unsigned long literal_size;
        inline Cnf(){
            literal_size=0;
        }
        inline void init(){
            lit_set.capacity(literal_size);
            clause_exist.capacity(cnf_set.size());
            for(int i=0;i<literal_size;i++)
                lit_set[i]=UNDEFINE;
        }
};

#include"vector.h"

class Cnf{
    private:
        typedef myVector<int> clause;
    public:
        myVector<clause> cnf_set;
        unsigned long literal_size;
        inline Cnf(){
            literal_size=0;
        }
};
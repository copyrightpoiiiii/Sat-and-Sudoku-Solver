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

struct sear_stat{
    double var_decay,clause_decay,rand_feq;
    sear_stat(double v=1,double c=1,double r=0){
        var_decay=v;
        clause_decay=c;
        rand_feq=r;
    }
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
const Lit lit_Undef(-1, false);
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
    public: 
        Lit data[1];
        unsigned long learned_clause_size;
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

inline Clause* Clause_new(myVector<Lit>& ps,bool learnt);

class GClause{
    public: 
        void* data;
        GClause(void* d): data(d){}
        friend GClause GClause_new(Lit p);
        friend GClause GClause_new(Clause* c);
        Clause* clause()const{
            return (Clause*)data;
        }
};


inline GClause GClause_new(Lit p);

inline GClause GClause_new(Clause* c);

class Solver{
    private:
        bool ok;
        myVector<Clause*> clauses;
        myVector<Clause*> learnts;
        int n_bin_clauses;
        double cla_inc;
        double cla_decay;
        myVector<double> activity;
        double var_inc;
        double var_decay;
        VarOrder order;
        myVector<myVector<GClause> > watches;
        myVector<short> assigns;
        myVector<Lit> trail;
        myVector<int> trail_lim;
        myVector<GClause> reason;
        myVector<int> level;
        int root_level,qhead;
        int simpDB_props,simpDB_assigns;
        myVector<short> analyze_seen;
        myVector<Lit> analyze_stack;
        myVector<Lit> analyze_toclear;
        Clause* propagate_tmpbin;
        Clause* analyze_tmpbin;
        Clause* solve_tmpunit;
        myVector<Lit> addBinary_tmp;
        myVector<Lit> addTernary_tmp;

        bool assume(Lit p);
        void cancelUntil(int level);
        void record(myVector<Lit>& clause);
        void analyze(Clause* confl,myVector<Lit>& out_learnt,int& out_btlevel);
        bool analyze_removable(Lit p,unsigned long min_level);
        void analyzeFinal(Clause* confl,bool skip_first);
        bool enqueue(Lit fact, GClause from);
        Clause* propagate();
        void        reduceDB         ();
        Lit         pickBranchLit    (const sear_stat& params);
        int       search           (int nof_conflicts, int nof_learnts, const sear_stat& params);
        double      progressEstimate ();

        void varBumpActivity(Lit p) {
            if (var_decay < 0) return;
            if ( (activity[var(p)] += var_inc) > 1e100 ) varRescaleActivity();
            order.update(var(p));
        }
        void     varDecayActivity  () { if (var_decay >= 0) var_inc *= var_decay; }
        void     varRescaleActivity();
        void     claDecayActivity  () { cla_inc *= cla_decay; }
        void     claRescaleActivity();

        // Operations on clauses:
        //
        void     newClause(const myVector<Lit>& ps, bool learnt = false);
        void     claBumpActivity (Clause* c) {
            int tmp=c->size();
            c->data[tmp].x += cla_inc;
            if (c->data[tmp].x  > 1e20 ) 
                claRescaleActivity(); 
        }
        void     remove(Clause* c, bool just_dealloc = false);
        bool     locked(const Clause* c){
            int tmp=(((*c)[0]).x>>1);
            if(((*(int *)reason[tmp].data)&1)==1)
                return false;
            else if(reason[tmp].clause()==c)
                return true;
            else return false;
        }
        bool     simplify        (Clause* c) const;

        int      decisionLevel(){ return trail_lim.size(); }

    public:
        Solver() : ok               (true)
                , n_bin_clauses    (0)
                , cla_inc          (1)
                , cla_decay        (1)
                , var_inc          (1)
                , var_decay        (1)
                , order            (assigns, activity)
                , qhead            (0)
                , simpDB_assigns   (0)
                , simpDB_props     (0)
                , default_params   (sear_stat(0.95, 0.999, 0.02))
                , expensive_ccmin  (true)
                , verbosity        (0)
                , progress_estimate(0)
                {
                    myVector<Lit> dummy(2,lit_Undef);
                    propagate_tmpbin = Clause_new(dummy,false);
                    analyze_tmpbin   = Clause_new(dummy,false);
                    dummy.pop_back();
                    solve_tmpunit    = Clause_new(dummy,false);
                    addBinary_tmp.capacity(2);
                    addTernary_tmp.capacity(3);
                }

    ~Solver() {
        for (int i = 0; i < learnts.size(); i++) remove(learnts[i], true);
        for (int i = 0; i < clauses.size(); i++) if (clauses[i] != NULL) remove(clauses[i], true); }
        int value(int x){
            return 2*assigns[x]-1; 
        }
        int value(Lit p){ return sign(p) ? ~(2*assigns[var(p)]-1) :(2*assigns[var(p)]-1); }

        int     nAssigns() { return trail.size(); }
        int     nClauses() { return clauses.size() + n_bin_clauses; }   // (minor difference from MiniSat without the GClause trick: learnt binary clauses will be counted as original clauses)
        int     nLearnts() { return learnts.size(); }

        // Statistics: (read-only member variable)
        //
        int    stats;

        // Mode of operation:
        //
        sear_stat default_params;     // Restart frequency etc.
        bool            expensive_ccmin;    // Controls conflict clause minimization. TRUE by default.
        int             verbosity;          // Verbosity level. 0=silent, 1=some progress report, 2=everything

        // Problem specification:
        //
        int     newVar();
        int     nVars(){
                    return assigns.size(); 
                }
        void    addUnit(Lit p){
                    if (ok) ok = enqueue(p,GClause_new((Clause*)NULL));
                }
        void    addBinary (Lit p, Lit q)        { addBinary_tmp [0] = p; addBinary_tmp [1] = q; addClause(addBinary_tmp); }
        void    addTernary(Lit p, Lit q, Lit r) { addTernary_tmp[0] = p; addTernary_tmp[1] = q; addTernary_tmp[2] = r; addClause(addTernary_tmp); }
        void    addClause (myVector<Lit>& ps)  { newClause(ps); }  // (used to be a difference between internal and external method...)

        // Solving:
        //
        bool    okay() { return ok; }       // FALSE means solver is in an conflicting state (must never be used again!)
        void    simplifyDB();
        bool    solve(myVector<Lit>& assumps);
        bool    solve() {myVector<Lit> tmp; return solve(tmp); }

        double      progress_estimate;  // Set by 'search()'.
        myVector<bool>  model;              // If problem is satisfiable, this vector contains the model (if any).
        myVector<Lit>    conflict;
};
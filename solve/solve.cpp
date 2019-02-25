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

bool removeWatch(myVector<GClause>& ws,GClause elem){
    if(ws.size()==0)return false;
    else{
        int i=0;
        while(ws[i]!=elem){
            i++;
            if(i==ws.size())return false;
        }
        while(i<ws.size()-1){
            ws[i]=ws[i+1];
        }
        ws.pop_back();
        return true;
    }
}

void Solver::newClause(myVector<Lit>& ps, bool learnt){
    myVector<Lit> qs;
    myVector<Lit>& tmp=ps;
    if(!ok)return ;
    if(!learnt){
        for(int i=0;i<ps.size();i++)
            qs.push_back(ps[i]);
        qs.qsort(0,qs.size()-1);
        qs.unique();
        for(int i=0;i<qs.size()-1;i++)
            if(qs[i]==~qs[i+1])
                return ;
        for(int i=0;i<qs.size();i++)
            if(qs[i].x==1)
                return ;
        int now=0;
        for(int i=0;i<qs.size();i++)
            if(qs[i].x!=-3)
                qs[now++]=qs[i];
        for(int i=qs.size()-now;i>0;i--)
            qs.pop_back();
        tmp=qs;
    }
    int size=tmp.size();
    if(!size)
        ok=false;
    else if(size==1){
        if(!enqueue(tmp[0],GClause_new((Clause*)NULL)))
            ok=false;
    }
    else if(size==2){
        watches[(~tmp[0]).x].push_back(GClause_new(tmp[1]));
        watches[(~tmp[1]).x].push_back(GClause_new(tmp[0]));
        stats.learnts_literals+=tmp.size();
        n_bin_clauses++;
    }
    else{
        Clause* c=Clause_new(tmp,learnt);
        if(learnt){
            int mx=level[var(tmp[1])],mx_id=1;
            for(int i=2;i<tmp.size();i++)
                if(level[var(tmp[i])]>mx){
                    mx=level[var(tmp[i])];
                    mx_id=i;
                }
            swap((*c)[1],(*c)[mx_id]);
            claBumpActivity(c);
            stats.learnts_literals+=c->size();
        }
        else{
            clauses.push_back(c);
            stats.clauses_literals+=c->size();
        }
        watches[(~(*c)[0]).x].push_back(GClause_new(c));
        watches[(~(*c)[1]).x].push_back(GClause_new(c));
    }
}

void Solver::remove(Clause* c, bool just_dealloc){
    if(!just_dealloc){
        if(c->size()==2){
            removeWatch(watches[(~(*c)[0]).x],GClause_new((*c)[1]));
            removeWatch(watches[(~(*c)[1]).x],GClause_new((*c)[0]));
        }
        else{
            removeWatch(watches[(~(*c)[0]).x],GClause_new(c));
            removeWatch(watches[(~(*c)[1]).x],GClause_new(c));
        }
    }
    if(c->learnt())stats.learnts_literals-=c->size();
    else stats.clauses_literals-=c->size();
    if(c!=NULL)free(c);
}

bool Solver::simplify(Clause* c){
    int num=c->size();
    for(int i=0;i<num;i++)
        if(value((*c)[i])==1)
            return true;
    return false;
}

Clause* Solver::propagate(){

}

int Solver::newVar(){
    int ind=nVars();
    reason.push_back(GClause_new((Clause*)NULL));
    assigns.push_back(-3);
    level.push_back(-1);
    order.newVar();
    analyze_seen.push_back(0);
    activity.push_back(0);
    watches.push_back();
    watches.push_back();
    return ind;
}

bool Solver::assume(Lit p){
    trail_lim.push_back(trail.size());
    return enqueue(p,GClause_new((Clause*)NULL));
}

void Solver::cancelUntil(int level){
    if(decisionLevel()>level){
        for(int i=trail.size()-1;i>=trail_lim[level];i--){
            int re_level=var(trail[i]);
            reason[re_level]=GClause_new((Clause*)NULL);
            assigns=-3;
            order.undo(re_level);
        }
        for(int i=trail.size()-trail_lim[level];i>0;i--)
            trail.pop_back();
        for(int i=trail_lim.size()-level;i>0;i--)
            trail_lim.pop_back();
        qhead=trail.size();
    }
}

void Solver::analyze(Clause* confl,myVector<Lit>& out_learnt,int& out_btlevel){
    GClause rec_con=GClause_new(confl);
    int pa=0;
    Lit p=lit_Undef;
    myVector<short>& se=analyze_seen;
    out_learnt.push_back();
    out_btlevel=0;
    int ind=trail.size()-1;
    int flag=1;
    while(flag){
        int tmp=*(int*)rec_con.data;
        Lit tt;
        tt.x=tmp>>1;
        Clause& c=((tmp&1)==1)?((*analyze_tmpbin)[1]=(tt),*analyze_tmpbin):*rec_con.clause();
        if(c.learnt())claBumpActivity(&c);
    }
}



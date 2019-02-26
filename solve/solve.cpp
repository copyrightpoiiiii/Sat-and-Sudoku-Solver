#include"solve.h"
#include<math.h>

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

inline Clause* Clause_new(const myVector<Lit>& ps,bool learnt){
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

void Solver::newClause(const myVector<Lit>& ps, bool learnt){
    myVector<Lit> qs;
    qs.clear();
    if(!ok)return ;
    if(!learnt){
        ps.copyTo(qs);
        qs.qsort(0,qs.size()-1);
        qs.unique();
        for(int i=0;i<qs.size()-1;i++)
            if(qs[i]==~qs[i+1])
                return ;
        for(int i=0;i<qs.size();i++)
            if(value(qs[i])==1)
                return ;
        int now=0;
        for(int i=0;i<qs.size();i++)
            if(value(qs[i])!=-3)
                qs[now++]=qs[i];
        for(int i=qs.size()-now;i>0;i--)
            qs.pop_back();
    }
    const myVector<Lit>& tmp=learnt?ps:qs;
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
    if((c->learned_clause_size)&1)stats.learnts_literals-=c->size();
    else stats.clauses_literals-=c->size();
    if(c!=NULL)free(c);
}

bool Solver::simplify(Clause* c)const{
    //int num=c->size();
    for(int i=0;i<c->size();i++)
        if(value((*c)[i])==1)
            return true;
    return false;
}

Clause* Solver::propagate(){
    Clause* confl=NULL;
    while(qhead<trail.size()){
        stats.propagations++;
        simpDB_props--;
        Lit p=trail[qhead++];
        myVector<GClause>& watcher=watches[p.x];
        GClause* l,*r;
        l=r=watcher.begin();
        while(l!=watcher.end()){
            int x=*(unsigned long*)l->data;
            if((x&1)==1){
                Lit tmp;
                tmp.x=x>>1;
                if(!enqueue(tmp,GClause_new(p))){
                    (*confl)[0]=tmp;
                    (*confl)[1]=~p;
                    confl=propagate_tmpbin;
                    qhead=trail.size();
                    if(decisionLevel()==0)
                        ok=false;
                    while(l<watcher.end()){
                        *r++ = *l++;
                    }
                }
                else *r++ = *l++;
            }
            else{
                Clause& c=*l->clause();l++;
                if(~p==c[0]){
                    c[0]=c[1];
                    c[1]=~p;
                }
                int v=value(c[0]);
                int flag=0;
                if(v!=1){
                    for(int i=2;i<c.size();i++)
                        if(value(c[i])!=-3){
                            c[1]=c[i];c[i]=~p;
                            watches[(~c[1]).x].push_back(GClause_new(&c));
                            flag=1;
                            break;
                        }
                    if(!flag){
                        *r=GClause_new(&c);
                        r++;
                        if(!enqueue(c[0],GClause_new(&c))){
                            qhead=trail.size();
                            confl=&c;
                            while(l!=watcher.end())
                                *r++=*l++;
                            if(decisionLevel()==0)
                                ok=false;
                        }
                    }
                }
                else{
                    *r=GClause_new(&c);
                    r++;
                }
            }
        }
        while(l!=r){
            trail.pop_back();
            r++;
        }
    }
    return confl;
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
    Lit p;
    p.x=lit_Undef.x;
    myVector<short>& se=analyze_seen;
    out_learnt.push_back();
    out_btlevel=0;
    int ind=trail.size()-1;
    while(1){
        int tmp=*(int*)rec_con.data;
        Lit tt;
        tt.x=tmp>>1;
        Clause& c=((tmp&1)==1)?((*analyze_tmpbin)[1]=(tt),*analyze_tmpbin):*rec_con.clause();
        if((c.learned_clause_size)&1)claBumpActivity(&c);
        int i;
        if(p.x==lit_Undef.x)
            i=0;
        else i=1;
        for(;i<c.size();i++)
            if(level[var(c[i])]>0&&!se[var(c[i])]){
                varBumpActivity(c[i]);
                se[var(c[i])]=1;
                if(level[var(c[i])]==decisionLevel())
                    pa++;
                else{
                    out_btlevel=max(out_btlevel,level[var(c[i])]);
                    out_learnt.push_back(c[i]);
                }
            }
        while(se[var(trail[ind])]==0)
            ind--;
        p=trail[ind+1];
        rec_con=reason[var(p)];
        se[var(p)]=0;
        pa--;
        if(pa<=0)break;
    }
    out_learnt[0]=~p;
    if(!expensive_ccmin){
        analyze_toclear.clear();
        for(int i=0;i<out_learnt.size();i++)
            analyze_toclear.push_back(out_learnt[i]);
        int now=1,flag=1;
        for(int i=1;i<out_learnt.size();i++){
            int tmp=*(unsigned int*)reason[var(out_learnt[i])].data;
            if(reason[var(out_learnt[i])]==GClause_new((Clause*)NULL))
                out_learnt[now++]=out_learnt[i];
            else if((tmp&1)==1){
                Lit tmp_q;
                tmp_q.x=tmp>>1;
                if(level[var(tmp_q)]!=0&&se[var(tmp_q)]==0)
                    out_learnt[now]=out_learnt[i],now++;
            }
            else{
                int size=(*reason[var(out_learnt[i])].clause()).size();
                Clause& tmp_cla=*reason[var(out_learnt[i])].clause();
                for(int j=1;j<size;j++)
                    if(level[var(tmp_cla[j])]!=0&&se[var(tmp_cla[j])]==0){
                        out_learnt[now]=out_learnt[i];
                        now++;
                        for(int j=i-now;j>0;j--)
                            out_learnt.pop_back();
                        flag=0;
                        break;
                    }
            }
        }
        if(flag){
            for(int i=out_learnt.size()-now;i>0;i--)
                out_learnt.pop_back();
        }
    }
    else{
        int mi=0;
        for(int i=1;i<out_learnt.size();i++)
            mi|=1<<(level[var(out_learnt[i])]&31);
        analyze_toclear.clear();
        for(int i=0;i<out_learnt.size();i++)
            analyze_toclear.push_back(out_learnt[i]);
        int now=1;
        for(int i=1;i<out_learnt.size();i++)
            if(reason[var(out_learnt[i])]==GClause_new((Clause*)NULL))
                out_learnt[now++]=out_learnt[i];
            else if(!analyze_removable(out_learnt[i],mi))
                out_learnt[now++]=out_learnt[i];
        for(int i=out_learnt.size()-now;i>0;i--)
            out_learnt.pop_back();
    }
    stats.tot_literals+=out_learnt.size();
    stats.max_literals+=out_learnt.size();
    for(int i=0;i<analyze_toclear.size();i++)
        se[var(analyze_toclear[i])]=0;
}

bool Solver::analyze_removable(Lit p,unsigned long min_level){
    analyze_stack.clear();analyze_stack.push_back(p);
    int to=analyze_toclear.size();
    while(!analyze_stack.empty()){
        GClause rec=reason[var(analyze_stack[analyze_stack.size()-1])];
        analyze_stack.pop_back();
        int tmp=*(unsigned int*)rec.data;
        Lit pp;
        pp.x=tmp>>1;
        if((tmp&1)==1)
            (*analyze_tmpbin)[1]=pp;
        Clause& cla=((tmp&1)==1)?(*analyze_tmpbin):(*rec.clause());
        for(int i=1;i<cla.size();i++)
            if(level[var(cla[i])]!=0&&analyze_seen[var(cla[i])]==0){
                int hash=min_level&(1<<(level[var(cla[i])]));
                if(!hash||reason[var(cla[i])]==GClause_new((Clause*)NULL)){
                    for(int j=to;j<analyze_toclear.size();j++)
                        analyze_seen[var(analyze_toclear[j])]=0;
                    for(int i=analyze_toclear.size()-to;i>0;i--)
                        analyze_toclear.pop_back();
                    return false;
                }
                else{
                    analyze_seen[var(cla[i])]=1;
                    analyze_stack.push_back(cla[i]);
                    analyze_toclear.push_back(cla[i]);
                }
            }
    }
    return true;
}

void Solver::analyzeFinal(Clause* confl,bool skip_first){
    conflict.clear();
    if(!root_level)return ;
    myVector<short>& se=analyze_seen;
    int i;
    if(skip_first)i=1;
    else i=0;
    for(;i<confl->size();i++){
        int x=var((*confl)[i]);
        if(level[x]>0)
            se[x]=1;
    }
    if(root_level<trail_lim.size())i=trail_lim[root_level];
    else i=trail.size()-1;
    for(;i>=trail_lim[0];i--)
        if(se[var(trail[i])]){
            if(reason[var(trail[i])]!=GClause_new((Clause*)NULL)){
                int tt=*(unsigned long*)reason[var(trail[i])].data;
                Lit tmp;
                tmp.x=tt>>1;
                if((tt&1)!=1){
                    Clause& cla=*reason[var(trail[i])].clause();
                    for(int j=1;j<cla.size();j++)
                        if(level[var(cla[j])]>0)
                            se[var(cla[j])]=1;
                }
                else{
                    if(level[var(tmp)]>0)
                        se[var(tmp)]=1;
                }
            }
            else conflict.push_back(~trail[i]);
            se[var(trail[i])]=0;
        }
}

bool Solver::enqueue(Lit p,GClause from){
    int x=var(p);
    if(value(p)==-1){
        level[x]=decisionLevel();
        reason[x]=from;
        assigns[x]=2*(!(p.x&1))-1;
        trail.push_back(p);
        return true;
    }
    else return value(p)!=-3;
}

struct cmp { 
    bool operator () (Clause* x, Clause* y){
        return x->size() > 2 && (y->size() == 2 || (*(double *)&x->data[(x->size())>>1]) < (*(double *)&y->data[(y->size())>>1]));
    } 
};
void Solver::reduceDB()
{
    int     i, j;
    double  extra_lim = cla_inc / learnts.size();    
    learnts.qsort(0,learnts.size()-1,cmp());
    for (i = j = 0; i < learnts.size() / 2; i++){
        if (learnts[i]->size() > 2 && !locked(learnts[i]))
            remove(learnts[i]);
        else
            learnts[j++] = learnts[i];
    }
    for (; i < learnts.size(); i++){
        if (learnts[i]->size() > 2 && !locked(learnts[i]) && (*(double *)&learnts[i]->data[(learnts[i]->size())>>1]) < extra_lim)
            remove(learnts[i]);
        else
            learnts[j++] = learnts[i];
    }
    for(int k=i-j;k>0;k--)
        learnts.push_back();
}

void Solver::simplifyDB(){
    if(!ok)return ;
    if(propagate()!=false){
        ok=false;
        return ;
    }
    if(trail.size()==simpDB_assigns||simpDB_props>0)
        return;
    for(int i=simpDB_assigns;i<trail.size();i++){
        myVector<GClause>& watcher=watches[(~trail[i]).x];
        for(int j=0;j<watches[(~trail[i]).x].size();j++){
            int tmp=*(unsigned long*)watches[(~trail[j]).x][j].data;
            if((tmp&1)==1){
                Lit t;
                t.x=tmp>>1;
                if(removeWatch(watches[(~t).x],GClause_new(trail[i])))
                    n_bin_clauses--;
            }
        }
        watches[trail[i].x].clear();
        watches[(~trail[i]).x].clear();
    }
    myVector<Clause*>& cla=clauses;
    int now=0;
    for(int i=0;i<cla.size();i++)
        if(simplify(cla[i])&&!locked(cla[i]))
            remove(cla[i]);
        else cla[now]=cla[i],now++;
    for(int i=cla.size()-now;i>0;i--)
        cla.pop_back();
    cla=learnts;
    now=0;
    for(int i=0;i<cla.size();i++)
        if(simplify(cla[i])&&!locked(cla[i]))
            remove(cla[i]);
        else cla[now]=cla[i],now++;
    for(int i=cla.size()-now;i>0;i--)
        cla.pop_back();
    simpDB_props=stats.learnts_literals;
    simpDB_assigns=trail.size();
    simpDB_props+=stats.clauses_literals;
}

int Solver::search(int nof_conflicts, int nof_learnts,const sear_stat& params){
    if(!ok)return -3;
    stats.starts++;
    int conflictC=0;
    var_decay=1/params.var_decay;
    cla_decay=1/params.clause_decay;
    model.clear();
    while(1){
        Clause* confl=propagate();
        if(confl!=NULL){
            conflictC++;
            stats.conflicts++;
            if(decisionLevel()==root_level){
                analyzeFinal(confl,false);
                return -3;
            }
            myVector<Lit> learnt_clause;
            learnt_clause.clear();
            int backtr_level=0;
            analyze(confl,learnt_clause,backtr_level);
            cancelUntil(max(backtr_level,root_level));
            newClause(learnt_clause,true);
            if(learnt_clause.size()==1)
                level[var(learnt_clause[0])]=0;
            claDecayActivity();
            varDecayActivity();
        }
        else{
            if(conflictC>=nof_conflicts&&nof_conflicts>=0){
                progress_estimate=progressEstimate();
                cancelUntil(root_level);
                return -1;
            }
            if(decisionLevel()==0)
                simplifyDB();
            if(nof_learnts>=0&&learnts.size()-trail.size()>=nof_learnts)
                reduceDB();
            stats.decisions++;
            int next=order.select();
            if(next==-1){
                model.capacity(assigns.size());
                for(int i=0;i<assigns.size();i++)
                    model[i]=(value(i));
                cancelUntil(root_level);
                return 1;
            }
            Lit p;
            p.x=next;
            assume(~p);
        }
    }
}

double Solver::progressEstimate(){
    double F=1.0/assigns.size(),progress=0;
    for(int i=0;i<assigns.size();i++)
        if(value(i)!=-1)
            progress+=pow(F,level[i]);
    return progress/assigns.size();
}

void Solver::varRescaleActivity(){
    for(int i=0;i<activity.size();i++)
        activity[i]*=1e-100;
    var_inc*=1e-100;
}

void Solver::claRescaleActivity(){
    for(int i=0;i<learnts.size();i++){
        int num=learnts[i]->learned_clause_size;
        *(double*)&learnts[i]->data[num>>1]*=1e-20;
    }
    cla_inc*=1e-20;
}

bool Solver::solve(myVector<Lit>& assumps){
    simplifyDB();
    if(!ok)return false;
    sear_stat params(default_params);
    double nof_learnts=nClauses()/3, nof_conflicts=100.0;
    int status=-1;
    root_level=assumps.size();
    for(int i=0;i<assumps.size();i++){
        if(!assume(assumps[i])){
            GClause r=reason[var(assumps[i])];
            if(r!=GClause_new((Clause*)NULL)){
                Clause* confl;
                int tmp=*(unsigned long*)r.data;
                if((tmp&1)==1){
                    confl=propagate_tmpbin;
                    Lit tt;
                    tt.x=tmp>>1;
                    (*confl)[0]=tt;
                    (*confl)[1]=~assumps[i];
                }
                else confl=r.clause();
                analyzeFinal(confl,true);
                conflict.push_back(~assumps[i]);
            }
            else{
                conflict.clear();
                conflict.push_back(~assumps[i]);
            }
            cancelUntil(0);
            return false;
        }
        Clause* confl=propagate();
        if(confl!=NULL){
            analyzeFinal(confl,false);
            cancelUntil(0);
            return false;
        }
    }
    root_level=decisionLevel();
    while(status==-3){
        status=search((int)nof_conflicts,(int)nof_learnts,params);
        nof_conflicts*=1.5;
        nof_learnts*=1.1;
    }
    cancelUntil(0);
    return status=1;
}






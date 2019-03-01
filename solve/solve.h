#pragma once
#ifndef GENERAL
#define GENERAL

#include"../General.hpp"

#endif

struct var_info {
	myVector<double> &active;

	bool operator() (int x, int y) {
		return active[x] > active[y];
	}

	var_info (myVector<double> &x) : active (x) {}
};

struct solve_state {
	long long starts, decisions, propagations, conflicts;
	long long clauses_literals, learnts_literals, max_literals, tot_literals;

	solve_state () : starts (0), decisions (0), propagations (0), conflicts (0), clauses_literals (0),
	                 learnts_literals (0), max_literals (0), tot_literals (0) {}
};

struct sear_stat {
	double var_decay, clause_decay, rand_feq;

	sear_stat (double v = 1, double c = 1, double r = 0) :
			var_decay (v),
			clause_decay (c),
			rand_feq (r) {}
};

class lit_sta {
	int value;

	explicit lit_sta (int v) : value (v) {}

public:
	lit_sta () : value (0) {}

	lit_sta (bool x) : value ((int) x * 2 - 1) {}

	int toInt () const { return value; }

	bool operator== (const lit_sta &other) const { return value == other.value; }

	bool operator!= (const lit_sta &other) const { return value != other.value; }

	lit_sta operator~ () const { return lit_sta (-value); }

	friend int toInt (lit_sta l);

	friend lit_sta toLbool (int v);
};

inline int toInt (lit_sta l) { return l.toInt (); }

inline lit_sta toLbool (int v) { return lit_sta (v); }

const lit_sta l_True = toLbool (1);
const lit_sta l_False = toLbool (-1);
const lit_sta l_Undef = toLbool (0);

class Lit {
	int x;
public:
	Lit () : x (-2) {}   // (lit_Undef)
	explicit Lit (int var, bool sgn = false) : x ((var + var) + (int) sgn) {}

	friend Lit operator~ (Lit p);

	friend bool sign (Lit p);

	friend int var (Lit p);

	friend int index (Lit p);

	friend Lit toLit (int i);

	friend Lit unsign (Lit p);

	friend Lit id (Lit p, bool sgn);

	friend bool operator== (Lit p, Lit q);

	friend bool operator< (Lit p, Lit q);
};

inline Lit operator~ (Lit p) {
	Lit q;
	q.x = p.x ^ 1;
	return q;
}

inline bool sign (Lit p) { return p.x & 1; }

inline int var (Lit p) { return p.x >> 1; }

inline int
index (Lit p) { return p.x; }                // A "toInt" method that guarantees small, positive integers suitable for array indexing.
inline Lit toLit (int i) {
	Lit p;
	p.x = i;
	return p;
}  // Inverse of 'index()'.
inline Lit unsign (Lit p) {
	Lit q;
	q.x = p.x & ~1;
	return q;
}

inline Lit id (Lit p, bool sgn) {
	Lit q;
	q.x = p.x ^ (int) sgn;
	return q;
}

inline bool operator== (Lit p, Lit q) { return index (p) == index (q); }

inline bool operator< (Lit p, Lit q) {
	return index (p) < index (q);
}  // '<' guarantees that p, ~p are adjacent in the ordering.

const Lit lit_Undef (-1, false);  // }- Useful special constants.
const Lit lit_Error (-1, true);  // }


class VarOrder {
private:
	myVector<double> &activity;
	myVector<char> &assigns;
	Heap<var_info> h;
public:
	VarOrder (myVector<char> &val, myVector<double> &act) :
			activity (act), assigns (val), h (var_info (act)) {}

	void newVar ();

	void update (int x);

	//void updateAll();
	void undo (int x);

	int select ();
};

typedef __PTRDIFF_TYPE__ intp;
typedef unsigned __PTRDIFF_TYPE__ uintp;

class Clause {
	unsigned int learned_clause_size;
	Lit data[1];
public:
	Clause (const myVector<Lit> &ps, bool learn) {
		learned_clause_size = (ps.size () << 1) | (int) learn;
		for (int i = 0; i < ps.size (); i++)
			data[i] = ps[i];
		if (learn)
			activity () = 0;
	}

	friend Clause *Clause_new (myVector<Lit> &ps, bool learnt);

	int size () const { return learned_clause_size >> 1; }

	bool learnt () const { return learned_clause_size & 1; }

	Lit operator[] (int i) const {
		return data[i];
	}

	Lit &operator[] (int i) {
		return data[i];
	}

	float &activity () const { return *((float *) &data[size ()]); }
};

inline Clause *Clause_new (const myVector<Lit> &ps, bool learnt) {
	void *mem = (char *) malloc (
			(sizeof (Clause) - sizeof (Lit) + sizeof (unsigned int) * (ps.size () + (int) learnt)) * sizeof (char));
	return new (mem) Clause (ps, learnt);
}

class GClause {
public:
	void *data;

	GClause (void *d) : data (d) {}

	friend GClause GClause_new (Lit p);

	friend GClause GClause_new (Clause *c);

	Clause *clause () const {
		return (Clause *) data;
	}

	bool operator== (GClause c) {
		return data == c.data;
	}

	bool operator!= (GClause c) {
		return data != c.data;
	}

	bool isLit () const { return ((uintp) data & 1) == 1; }

	Lit lit () const { return toLit (((intp) data) >> 1); }
};

inline GClause GClause_new (Lit p) {
	return GClause ((void *) (((intp) index (p) << 1) + 1));
}

inline GClause GClause_new (Clause *c) {
	return GClause ((void *) c);
}

class Solver {
	bool ok;
	myVector<Clause *> clauses;
	myVector<Clause *> learnts;
	int n_bin_clauses;
	double cla_inc;
	double cla_decay;
	myVector<double> activity;
	double var_inc;
	double var_decay;
	VarOrder order;
	myVector<myVector<GClause> > watches;
	myVector<char> assigns;
	myVector<Lit> trail;
	myVector<int> trail_lim;
	myVector<GClause> reason;
	myVector<int> level;
	int root_level, qhead;
	long long simpDB_props;
	int simpDB_assigns;
	myVector<char> analyze_seen;
	myVector<Lit> analyze_stack;
	myVector<Lit> analyze_toclear;
	Clause *propagate_tmpbin;
	Clause *analyze_tmpbin;
	Clause *solve_tmpunit;
	myVector<Lit> addBinary_tmp;
	myVector<Lit> addTernary_tmp;

	bool assume (Lit p);

	void cancelUntil (int level);

	//void record(const myVector<Lit>& clause);
	void analyze (Clause *confl, myVector<Lit> &out_learnt, int &out_btlevel);

	bool analyze_removable (Lit p, unsigned long min_level);

	void analyzeFinal (Clause *confl, bool skip_first = false);

	bool enqueue (Lit fact, GClause from);

	Clause *propagate ();

	void reduceDB ();

	//Lit         pickBranchLit    (const sear_stat& params);
	lit_sta search (int nof_conflicts, int nof_learnts, const sear_stat &params);

	double progressEstimate ();

	void varBumpActivity (Lit p) {
		if (var_decay < 0) return;
		if ((activity[var (p)] += var_inc) > 1e100) varRescaleActivity ();
		order.update (var (p));
	}

	void varDecayActivity () { if (var_decay >= 0) var_inc *= var_decay; }

	void varRescaleActivity ();

	void claDecayActivity () { cla_inc *= cla_decay; }

	void claRescaleActivity ();

	void newClause (const myVector<Lit> &ps, bool learnt = false);

	void claBumpActivity (Clause *c) { if ((c->activity () += cla_inc) > 1e20) claRescaleActivity (); }

	void remove (Clause *c, bool just_dealloc = false);

	bool locked (const Clause *c) const {
		GClause r = reason[var ((*c)[0])];
		return !r.isLit () && r.clause () == c;
	}

	bool simplify (Clause *c) const;

	int decisionLevel () const { return trail_lim.size (); }

public:
	Solver () : ok (true), n_bin_clauses (0), cla_inc (1), cla_decay (1), var_inc (1), var_decay (1),
	            order (assigns, activity), qhead (0), simpDB_assigns (0), simpDB_props (0),
	            default_params (sear_stat (0.95, 0.999, 0.02)), expensive_ccmin (true), verbosity (0),
	            progress_estimate (0) {
		myVector<Lit> dummy (2, lit_Undef);
		//dummy.push_back(lit_Undef);
		//dummy.push_back(lit_Undef);
		propagate_tmpbin = Clause_new (dummy, false);
		analyze_tmpbin = Clause_new (dummy, false);
		dummy.pop_back ();
		solve_tmpunit = Clause_new (dummy, false);
		addBinary_tmp.capacity (2);
		addTernary_tmp.capacity (3);
	}

	~Solver () {
		for (int i = 0; i < learnts.size (); i++) remove (learnts[i], true);
		for (int i = 0; i < clauses.size (); i++) if (clauses[i] != NULL) remove (clauses[i], true);
	}

	lit_sta value (int x) const {
		return toLbool (assigns[x]);
	}

	lit_sta value (Lit p) const { return sign (p) ? ~toLbool (assigns[var (p)]) : toLbool (assigns[var (p)]); }

	int nAssigns () { return trail.size (); }

	int nClauses () { return clauses.size () + n_bin_clauses; }

	int nLearnts () { return learnts.size (); }

	solve_state stats;
	sear_stat default_params;
	bool expensive_ccmin;
	int verbosity;

	int newVar ();

	int nVars () {
		return assigns.size ();
	}

	void addUnit (Lit p) {
		if (ok) ok = enqueue (p, GClause_new ((Clause *) NULL));
	}

	void addBinary (Lit p, Lit q) {
		addBinary_tmp[0] = p;
		addBinary_tmp[1] = q;
		addClause (addBinary_tmp);
	}

	void addTernary (Lit p, Lit q, Lit r) {
		addTernary_tmp[0] = p;
		addTernary_tmp[1] = q;
		addTernary_tmp[2] = r;
		addClause (addTernary_tmp);
	}

	void addClause (const myVector<Lit> &ps) { newClause (ps); }

	bool okay () { return ok; }

	void simplifyDB ();

	bool solve (const myVector<Lit> &assumps);

	bool solve () {
		myVector<Lit> tmp;
		return solve (tmp);
	}

	double progress_estimate;
	myVector<lit_sta> model;
	myVector<Lit> conflict;
};
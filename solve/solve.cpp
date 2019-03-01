#include"solve.h"
#include<math.h>

int max (int x, int y) {
	return (x > y) ? x : y;
}

void VarOrder::newVar () {
	h.setBounds (assigns.size ());
	h.insert (assigns.size () - 1);
}

void VarOrder::update (int x) {
	if (!h.inHeap (x))return;
	else h.increase (x);
}

void VarOrder::undo (int x) {
	if (h.inHeap (x))return;
	else h.insert (x);
}

int VarOrder::select () {
	while (!h.empty ()) {
		int top = h.getmin ();
		if (toLbool (assigns[top]) == l_Undef)
			return top;
	}
	return -1;
}

bool removeWatch (myVector<GClause> &ws, GClause elem) {
	if (ws.size () == 0)return false;
	else {
		int i = 0;
		while (ws[i] != elem) {
			i++;
			if (i == ws.size ())return false;
		}
		while (i < ws.size () - 1) {
			ws[i++] = ws[i + 1];
		}
		ws.pop_back ();
		return true;
	}
}

void Solver::newClause (const myVector<Lit> &ps, bool learnt) {
	myVector<Lit> qs;
	qs.clear ();
	if (!ok)return;
	if (!learnt) {
		ps.copyTo (qs);
		qs.qsort (0, qs.size () - 1);
		qs.unique ();
		for (int i = 0; i < qs.size () - 1; i++)
			if (qs[i] == ~qs[i + 1])
				return;
		for (int i = 0; i < qs.size (); i++)
			if (value (qs[i]) == l_True)
				return;
		int now = 0, i;
		for (i = 0; i < qs.size (); i++)
			if (value (qs[i]) != l_False)
				qs[now++] = qs[i];
		qs.dele (i - now);
	}
	const myVector<Lit> &tmp = learnt ? ps : qs;
	int size = tmp.size ();
	if (!size)
		ok = false;
	else if (size == 1) {
		if (!enqueue (tmp[0], GClause_new ((Clause *) NULL)))
			ok = false;
	} else if (size == 2) {
		watches[index (~tmp[0])].push_back (GClause_new (tmp[1]));
		watches[index (~tmp[1])].push_back (GClause_new (tmp[0]));
		if (learnt) {
			enqueue (tmp[0], GClause_new (~tmp[1]));
			stats.learnts_literals += tmp.size ();
		} else stats.clauses_literals += tmp.size ();
		n_bin_clauses++;
	} else {
		Clause *c = Clause_new (tmp, learnt);
		if (learnt) {
			int mx = level[var (tmp[1])], mx_id = 1;
			for (int i = 2; i < tmp.size (); i++)
				if (level[var (tmp[i])] > mx) {
					mx = level[var (tmp[i])];
					mx_id = i;
				}
			(*c)[1] = tmp[mx_id];
			(*c)[mx_id] = tmp[1];
			claBumpActivity (c);
			enqueue ((*c)[0], GClause_new (c));
			learnts.push_back (c);
			stats.learnts_literals += c->size ();
		} else {
			clauses.push_back (c);
			stats.clauses_literals += c->size ();
		}
		watches[index (~(*c)[0])].push_back (GClause_new (c));
		watches[index (~(*c)[1])].push_back (GClause_new (c));
	}
}

void Solver::remove (Clause *c, bool just_dealloc) {
	if (!just_dealloc) {
		if (c->size () == 2) {
			removeWatch (watches[index (~(*c)[0])], GClause_new ((*c)[1]));
			removeWatch (watches[index (~(*c)[1])], GClause_new ((*c)[0]));
		} else {
			removeWatch (watches[index (~(*c)[0])], GClause_new (c));
			removeWatch (watches[index (~(*c)[1])], GClause_new (c));
		}
	}
	if (c->learnt ())stats.learnts_literals -= c->size ();
	else stats.clauses_literals -= c->size ();
	if (c != NULL)free (c);
}

bool Solver::simplify (Clause *c) const {
	//int num=c->size();
	for (int i = 0; i < c->size (); i++)
		if (value ((*c)[i]) == l_True)
			return true;
	return false;
}

Clause *Solver::propagate () {
	Clause *confl = NULL;
	while (qhead < trail.size ()) {
		stats.propagations++;
		simpDB_props--;
		Lit p = trail[qhead++];
		myVector<GClause> &watcher = watches[index (p)];
		GClause *l, *r, *end;
		l = r = (GClause *) watcher;
		end = l + watcher.size ();
		while (l != end) {
			if (l->isLit ()) {
				if (!enqueue (l->lit (), GClause_new (p))) {
					if (decisionLevel () == 0)
						ok = false;
					confl = propagate_tmpbin;
					(*confl)[0] = l->lit ();
					(*confl)[1] = ~p;
					qhead = trail.size ();
					while (l < end) {
						*r++ = *l++;
					}
				} else *r++ = *l++;
			} else {
				Clause &c = *l->clause ();
				l++;
				if (~p == c[0]) {
					c[0] = c[1];
					c[1] = ~p;
				}
				lit_sta v = value (c[0]);
				int flag = 0;
				if (v != l_True) {
					for (int i = 2; i < c.size (); i++)
						if (value (c[i]) != l_False) {
							c[1] = c[i];
							c[i] = ~p;
							watches[index (~c[1])].push_back (GClause_new (&c));
							flag = 1;
							break;
						}
					if (!flag) {
						*r++ = GClause_new (&c);
						if (!enqueue (c[0], GClause_new (&c))) {
							if (decisionLevel () == 0)
								ok = false;
							qhead = trail.size ();
							confl = &c;
							while (l < end)
								*r++ = *l++;
						}
					}
				} else {
					*r++ = GClause_new (&c);
				}
			}
		}
		watcher.dele ((int) (l - r));
	}
	return confl;
}

int Solver::newVar () {
	int ind = nVars ();
	reason.push_back (GClause_new ((Clause *) NULL));
	assigns.push_back (toInt (l_Undef));
	level.push_back (-1);
	order.newVar ();
	analyze_seen.push_back (0);
	activity.push_back (0);
	watches.push_back ();
	watches.push_back ();
	return ind;
}

bool Solver::assume (Lit p) {
	trail_lim.push_back (trail.size ());
	return enqueue (p, GClause_new ((Clause *) NULL));
}

void Solver::cancelUntil (int level) {
	if (decisionLevel () > level) {
		for (int i = trail.size () - 1; i >= trail_lim[level]; i--) {
			int re_level = var (trail[i]);
			reason[re_level] = GClause_new ((Clause *) NULL);
			assigns[re_level] = toInt (l_Undef);
			order.undo (re_level);
		}
		trail.dele (trail.size () - trail_lim[level]);
		trail_lim.dele (trail_lim.size () - level);
		qhead = trail.size ();
	}
}

void Solver::analyze (Clause *confl, myVector<Lit> &out_learnt, int &out_btlevel) {
	GClause rec_con = GClause_new (confl);
	int pa = 0;
	Lit p = lit_Undef;
	myVector<char> &se = analyze_seen;
	out_learnt.push_back ();
	out_btlevel = 0;
	int ind = trail.size () - 1;
	while (1) {
		Clause &c = (rec_con.isLit ()) ? ((*analyze_tmpbin)[1] = rec_con.lit (), *analyze_tmpbin) : *rec_con.clause ();
		if (c.learnt ())claBumpActivity (&c);
		int i;
		if (p == lit_Undef)
			i = 0;
		else i = 1;
		for (; i < c.size (); i++) {
			Lit tmp = c[i];
			if (level[var (tmp)] > 0 && !se[var (tmp)]) {
				varBumpActivity (tmp);
				se[var (tmp)] = 1;
				if (level[var (tmp)] == decisionLevel ())
					pa++;
				else {
					out_btlevel = max (out_btlevel, level[var (tmp)]);
					out_learnt.push_back (tmp);
				}
			}
		}
		while (se[var (trail[ind--])] == 0);
		p = trail[ind + 1];
		rec_con = reason[var (p)];
		se[var (p)] = 0;
		pa--;
		if (pa <= 0)break;
	}
	out_learnt[0] = ~p;
	int i, now;
	if (!expensive_ccmin) {
		out_learnt.copyTo (analyze_toclear);
		now = 1;
		for (i = 1; i < out_learnt.size (); i++) {
			GClause tmp = reason[var (out_learnt[i])];
			if (tmp == GClause_new ((Clause *) NULL))
				out_learnt[now++] = out_learnt[i];
			else if (tmp.isLit ()) {
				Lit tmp_q = tmp.lit ();
				if (level[var (tmp_q)] != 0 && se[var (tmp_q)] == 0)
					out_learnt[now++] = out_learnt[i];
			} else {
				Clause &tmp_cla = *tmp.clause ();
				for (int j = 1; j < tmp_cla.size (); j++)
					if (level[var (tmp_cla[j])] != 0 && se[var (tmp_cla[j])] == 0) {
						out_learnt[now++] = out_learnt[i];
						break;
					}
			}
		}
	} else {
		unsigned int mi = 0;
		for (i = 1; i < out_learnt.size (); i++)
			mi |= 1 << (level[var (out_learnt[i])] & 31);
		out_learnt.copyTo (analyze_toclear);
		now = 1;
		for (i = 1; i < out_learnt.size (); i++)
			if (reason[var (out_learnt[i])] == GClause_new ((Clause *) NULL))
				out_learnt[now++] = out_learnt[i];
			else if (!analyze_removable (out_learnt[i], mi))
				out_learnt[now++] = out_learnt[i];
	}
	stats.max_literals += out_learnt.size ();
	out_learnt.dele (i - now);
	stats.tot_literals += out_learnt.size ();
	for (i = 0; i < analyze_toclear.size (); i++)
		se[var (analyze_toclear[i])] = 0;
}

bool Solver::analyze_removable (Lit p, unsigned long min_level) {
	analyze_stack.clear ();
	analyze_stack.push_back (p);
	int to = analyze_toclear.size ();
	while (!analyze_stack.empty ()) {
		GClause rec = reason[var (analyze_stack[analyze_stack.size () - 1])];
		analyze_stack.pop_back ();
		Clause &cla = (rec.isLit ()) ? (((*analyze_tmpbin)[1] = rec.lit ()), *analyze_tmpbin) : (*rec.clause ());
		for (int i = 1; i < cla.size (); i++) {
			Lit tmp = cla[i];
			if (level[var (tmp)] != 0 && analyze_seen[var (tmp)] == 0) {
				unsigned long hash = (1 << (level[var (p)])) & (unsigned long) min_level;
				if (hash != 0 && reason[var (tmp)] != GClause_new ((Clause *) NULL)) {
					analyze_seen[var (tmp)] = 1;
					analyze_stack.push_back (tmp);
					analyze_toclear.push_back (tmp);
				} else {
					for (int j = to; j < analyze_toclear.size (); j++)
						analyze_seen[var (analyze_toclear[j])] = 0;
					analyze_toclear.dele (analyze_toclear.size () - to);
					return false;
				}
			}
		}
	}
	return true;
}

void Solver::analyzeFinal (Clause *confl, bool skip_first) {
	conflict.clear ();
	if (!root_level)return;
	myVector<char> &se = analyze_seen;
	int i;
	if (skip_first)i = 1;
	else i = 0;
	for (; i < confl->size (); i++) {
		int x = var ((*confl)[i]);
		if (level[x] > 0)
			se[x] = 1;
	}
	if (root_level < trail_lim.size ())i = trail_lim[root_level];
	else i = trail.size () - 1;
	for (; i >= trail_lim[0]; i--) {
		int tmp = var (trail[i]);
		if (se[tmp]) {
			if (reason[tmp] != GClause_new ((Clause *) NULL)) {
				if (!reason[tmp].isLit ()) {
					Clause &cla = *reason[tmp].clause ();
					for (int j = 1; j < cla.size (); j++)
						if (level[var (cla[j])] > 0)
							se[var (cla[j])] = 1;
				} else {
					Lit tt = reason[tmp].lit ();
					if (level[var (tt)] > 0)
						se[var (tt)] = 1;
				}
			} else conflict.push_back (~trail[i]);
			se[tmp] = 0;
		}
	}
}

bool Solver::enqueue (Lit p, GClause from) {
	if (value (p) == l_Undef) {
		assigns[var (p)] = toInt (lit_sta (!sign (p)));
		level[var (p)] = decisionLevel ();
		reason[var (p)] = from;
		trail.push_back (p);
		return true;
	} else return value (p) != l_False;
}

struct ccmp {
	bool operator() (Clause *x, Clause *y) {
		return x->size () > 2 && (y->size () == 2 || x->activity () < y->activity ());
	}
};

void Solver::reduceDB () {
	int i, j;
	double extra_lim = cla_inc / learnts.size ();
	learnts.qsort (0, learnts.size () - 1, ccmp ());
	for (i = j = 0; i < learnts.size () / 2; i++) {
		if (learnts[i]->size () > 2 && !locked (learnts[i]))
			remove (learnts[i]);
		else
			learnts[j++] = learnts[i];
	}
	for (; i < learnts.size (); i++) {
		if (learnts[i]->size () > 2 && !locked (learnts[i]) && learnts[i]->activity () < extra_lim)
			remove (learnts[i]);
		else
			learnts[j++] = learnts[i];
	}
	learnts.dele (i - j);
}

void Solver::simplifyDB () {
	if (!ok)return;
	if (propagate () != NULL) {
		ok = false;
		return;
	}
	if (trail.size () == simpDB_assigns || simpDB_props > 0)
		return;
	for (int i = simpDB_assigns; i < trail.size (); i++) {
		Lit p = trail[i];
		myVector<GClause> &watcher = watches[index (~p)];
		for (int j = 0; j < watcher.size (); j++) {
			if (watcher[j].isLit ()) {
				if (removeWatch (watches[index (~watcher[j].lit ())], GClause_new (p)))
					n_bin_clauses--;
			}
		}
		watches[index (p)].clear (true);
		watches[index (~p)].clear (true);
	}
	for (int type = 0; type < 2; type++) {
		myVector<Clause *> &cla = type ? learnts : clauses;
		int j = 0;
		for (int i = 0; i < cla.size (); i++) {
			if (!locked (cla[i]) && simplify (cla[i]))
				remove (cla[i]);
			else
				cla[j++] = cla[i];
		}
		cla.dele (cla.size () - j);
	}
	simpDB_assigns = trail.size ();
	simpDB_props = stats.learnts_literals;
	simpDB_props += stats.clauses_literals;
}

lit_sta Solver::search (int nof_conflicts, int nof_learnts, const sear_stat &params) {
	if (!ok)return l_False;
	stats.starts++;
	int conflictC = 0;
	var_decay = 1 / params.var_decay;
	cla_decay = 1 / params.clause_decay;
	model.clear ();
	while (1) {
		Clause *confl = propagate ();
		if (confl != NULL) {
			conflictC++;
			stats.conflicts++;
			myVector<Lit> learnt_clause;
			//learnt_clause.clear();
			int backtr_level;
			if (decisionLevel () == root_level) {
				analyzeFinal (confl, false);
				return l_False;
			}
			analyze (confl, learnt_clause, backtr_level);
			cancelUntil (max (backtr_level, root_level));
			newClause (learnt_clause, true);
			if (learnt_clause.size () == 1)
				level[var (learnt_clause[0])] = 0;
			varDecayActivity ();
			claDecayActivity ();
		} else {
			if (conflictC >= nof_conflicts && nof_conflicts >= 0) {
				progress_estimate = progressEstimate ();
				cancelUntil (root_level);
				return l_Undef;
			}
			if (decisionLevel () == 0)
				simplifyDB ();
			if (nof_learnts >= 0 && learnts.size () - trail.size () >= nof_learnts)
				reduceDB ();
			stats.decisions++;
			int next = order.select ();
			if (next == -1) {
				model.capacity (assigns.size ());
				for (int i = 0; i < assigns.size (); i++)
					model[i] = value (i);
				cancelUntil (root_level);
				return l_True;
			}
			assume (~Lit (next));
		}
	}
}

double Solver::progressEstimate () {
	double F = 1.0 / assigns.size (), progress = 0;
	for (int i = 0; i < assigns.size (); i++)
		if (value (i) != l_Undef)
			progress += pow (F, level[i]);
	return progress / assigns.size ();
}

void Solver::varRescaleActivity () {
	for (int i = 0; i < assigns.size (); i++)
		activity[i] *= 1e-100;
	var_inc *= 1e-100;
}

void Solver::claRescaleActivity () {
	for (int i = 0; i < learnts.size (); i++) {
		learnts[i]->activity () *= 1e-20;
	}
	cla_inc *= 1e-20;
}

bool Solver::solve (const myVector<Lit> &assumps) {
	simplifyDB ();
	fflush (stdout);
	if (!ok)return false;
	sear_stat params (default_params);
	double nof_learnts = nClauses () / 3.0, nof_conflicts = 100.0;
	lit_sta status = l_Undef;
	root_level = assumps.size ();
	for (int i = 0; i < assumps.size (); i++) {
		Lit p = assumps[i];
		if (!assume (p)) {
			GClause r = reason[var (p)];
			if (r != GClause_new ((Clause *) NULL)) {
				Clause *confl;
				if (r.isLit ()) {
					confl = propagate_tmpbin;
					(*confl)[0] = r.lit ();
					(*confl)[1] = ~p;
				} else confl = r.clause ();
				analyzeFinal (confl, true);
				conflict.push_back (~p);
			} else {
				conflict.clear ();
				conflict.push_back (~p);
			}
			cancelUntil (0);
			return false;
		}
		Clause *confl = propagate ();
		if (confl != NULL) {
			analyzeFinal (confl);
			cancelUntil (0);
			return false;
		}
	}
	while (status == l_Undef) {
		status = search ((int) nof_conflicts, (int) nof_learnts, params);
		nof_conflicts *= 1.5;
		nof_learnts *= 1.1;
	}
	cancelUntil (0);
	return status == l_True;
}






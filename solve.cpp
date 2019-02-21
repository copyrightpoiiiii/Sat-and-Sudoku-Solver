#include"solve.h"

bool solve () {
	for (int i = 0; i < clauses.cnf_set.size (); i++)
		q.push (make_pair ((int) clauses.cnf_set[i].size (), i));
	while (!q.empty ()) {
		for (pair<int, int> i = q.top (); clauses.clause_info[i.first].size != i.second; q.pop ());
		pair<int, int> cho_cla = q.top ();
		q.pop ();
	}
}
#include "startup_code.cpp"

#ifndef EM_CPP
	#define EM_CPP

void expectation(network& N, double epsilon, vector<vector<double> >& CPT, vector<vector<double> >& missing);
void maximization(network& N, double epsilon, vector<vector<double> >& CPT, vector<vector<double> >& missing);
bool is_converge(vector<vector<double> >& prev_CPT, vector<vector<double> > CPT, double epsilon);
void randomize_CPT(vector<vector<double> >& CPT);

vector<vector<double> > exp_max(network& N , double epsilon)
{
	vector<vector<double> > CPT(N.netSize());
	vector<vector<double> > missing(N.data_set.size());
	
	for (int i = 0 ; i<N.netSize() ; i++)
		CPT[i] = N.get_nth_node(i).get_CPT();
	
	for (int i = 0 ; i<data_set.size() ; i++)
		missing[i] = vector<double>(N.get_nth_node(N.missing_value[i]).get_nvalues() , -1.0);
		
	randomize_CPT(CPT);
		
	while (true)
	{
		vector<vector<double> > prev_CPT = CPT;
		
		expectation(N,epsilon,CPT,missing);
		maximization(N,epsilon,CPT,missing);
		
		if (is_converge(prev_CPT,CPT,epsilon)) return CPT;		// Dharmaraja reference
	}
}

void expectation(network& N, double epsilon, vector<vector<double> >& CPT, vector<vector<double> >& missing)
{
	
}

void maximization(network& N, double epsilon, vector<vector<double> >& CPT, vector<vector<double> >& missing)
{
	
}

void randomize_CPT(vector<vector<double> >& CPT)
{
	
}

bool is_converge(vector<vector<double> >& prev_CPT, vector<vector<double> > CPT, double epsilon)
{
	return true;
}

#endif
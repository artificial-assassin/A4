#include "startup_code.cpp"

#ifndef EM_CPP
	#define EM_CPP

void expectation(network& N, const vector<vector<double> >& CPT, vector<vector<double> >& missing);
void maximization(network& N, vector<vector<double> >& CPT, const vector<vector<double> >& missing);

// helpers
bool it_is_converge(vector<vector<double> >& prev_CPT, vector<vector<double> > CPT, double epsilon);
void randomize_CPT(vector<vector<double> >& CPT);
vector<vector<int> > markov_blanket(network& N, int n);						// returns indices of elements in Markov Blanket of node n, refer function details
vector<double> fill_missing(int i, network& N, const vector<vector<double> >& CPT);

/*--------------------------------------------------------------------------------------------------------------------------------*/

vector<vector<double> > exp_max(network& N , double epsilon)	// assumes initial basic counting CPT has been called
{
	vector<vector<double> > CPT(N.netSize());					// CPT for all nodes
	vector<vector<double> > missing(N.data_set.size());			// one element for each data point corresponding to missing value
																// {p("?"==nvalue[0]),p("?"==nvalue[1]),...} 
	for (int i = 0 ; i<N.netSize() ; i++)
		CPT[i] = N.get_nth_node(i).get_CPT();
	
	for (int i = 0 ; i<N.data_set.size() ; i++)
		missing[i] = vector<double>(N.get_nth_node(N.missing_value[i]).get_nvalues() , -1.0);
		
	randomize_CPT(CPT);		// add some noise, change exact 0s and 1s
		
	while (true)
	{
		vector<vector<double> > prev_CPT = CPT;
		
		expectation(N,CPT,missing);
		maximization(N,CPT,missing);
		
		if (it_is_converge(prev_CPT,CPT,epsilon)) return CPT;		// Dharmaraja reference
	}
}

void expectation(network& N, const vector<vector<double> >& CPT, vector<vector<double> >& missing)
{
	for (int i = 0 ; i<N.data_set.size() ; i++)
		missing[i] = fill_missing(i,N,CPT);	
}

void maximization(network& N, vector<vector<double> >& CPT, const vector<vector<double> >& missing)
{
	
}

bool it_is_converge(vector<vector<double> >& prev_CPT, vector<vector<double> > CPT, double epsilon)
{
	return true;
}

vector<double> fill_missing(int i, network& N, const vector<vector<double> >& CPT)
{
	int n = N.missing_value[i];
	vector<vector<int> > markov_blanket = markov_blanket(N,n);
	
	
}

vector<vector<int> > markov_blanket(network& N, int n)	// 3 element vector, {parents,children,parents of children}
{
	Graph_Node cur = N.get_nth_node(n);
	vector<vector<int> > mb;
	vector<bool> in_markov_blanket(N.netSize(), false);
	in_markov_blanket[n] = true;							// so that parent of children don't return current node
	
	// inserting parents
	mb.push_back(vector<int>());
	vector<int> temp = cur.parents_int;
	for (vector<int>::iterator it = temp.begin() ; it != temp.end() ; it++)
	{
		mb[0].push_back(*it);
		in_markov_blanket[*it] = true;
	}
	
	// inserting children
	mb.push_back(vector<int>());
	temp = cur.get_children();
	for (vector<int>::iterator it = temp.begin() ; it != temp.end() ; it++)
	{
		mb[1].push_back(*it);
		in_markov_blanket[*it] = true;
	}
	
	// inserting parents of children
	mb.push_back(vector<int>());
	for (vector<int>::iterator it = temp.begin() ; it != temp.end() ; it++)
	{
		vector<int> current = N.get_nth_node(*it).parents_int;
		for (vector<int>::iterator itr = current.begin() ; itr != current.end() ; itr++)
			if (!in_markov_blanket[*itr])
			{
				mb[2].push_back(*itr);
				in_markov_blanket[*itr] = true;
			}
	}

	// for (int i = 0 ; i<3 ; i++)
	// 	for (int j = 0 ; j<mb[i].size() ; j++)
	// 		cout << N.get_nth_node(mb[i][j]).get_name() << " ";
	// cout << endl;
	
	return mb;
}

void randomize_CPT(vector<vector<double> >& CPT)
{
	
}


#endif
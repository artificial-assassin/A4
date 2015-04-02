#include "startup_code.cpp"

#ifndef EM_CPP
	#define EM_CPP

/*--------------------------------------------------------------------------------------------------------------------------------*/

void expectation(network& N, const vector<vector<double> >& CPT, vector<vector<double> >& missing);
void maximization(network& N, vector<vector<double> >& CPT, const vector<vector<double> >& missing);

// helpers
bool it_is_a_converge(vector<vector<double> >& prev_CPT, vector<vector<double> > CPT, double epsilon);
void randomize_CPT(network& N, vector<vector<double> >& CPT, double factor);
vector<double> fill_missing(int i, network& N, const vector<vector<double> >& CPT);
double prob_x_given_mb(int n, int i, int j, network& N, const vector<vector<double> >& CPT);
double fetch_from_CPT(int n, int j, network& N, const vector<vector<double> >& CPT, const vector<int> par_values);
//vector<vector<int> > markov_blanket(network& N, int n);						// returns indices of elements in Markov Blanket of node n, refer function details

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
		
	randomize_CPT(N,CPT,20.0);		// add some noise, change exact 0.0s and 1.0s	

	while (true)
	{
		vector<vector<double> > prev_CPT = CPT;

		expectation(N,CPT,missing);
		// maximization(N,CPT,missing);
		
		for (int i = 0 ; i<missing.size() ; i++)
		{
			for (int j = 0 ; j<missing[i].size() ; j++)
				cout << missing[i][j] << " ";
			cout << "\n";
		}
		
		if (it_is_a_converge(prev_CPT,CPT,epsilon)) return CPT;		// Dharamaraja reference
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

/*-----------------------------------------------------      HELPERS       --------------------------------------------------------*/

bool it_is_a_converge(vector<vector<double> >& prev_CPT, vector<vector<double> > CPT, double epsilon)
{
	return true;
}

vector<double> fill_missing(int i, network& N, const vector<vector<double> >& CPT)
{
	int n = N.missing_value[i];
	Graph_Node cur = N.get_nth_node(n);
	
	vector<double> missing(cur.get_nvalues(),0.0);
	
	// when applying Bayes, prob of parents and parents of children will cancel in the numerator and denominator
	// prob_j_given_mb returns product of remaining probabilities
	
	for (int x = 0 ; x<missing.size() ; x++)
		missing[x] = prob_x_given_mb(n,i,x,N,CPT);		// nth node, ith data point, xth nvalue
	
	double total = 0.0;
	
	for (int x = 0 ; x<missing.size() ; x++)
		total += missing[x];
	
	for (int x = 0 ; x<missing.size() ; x++)
		missing[x] /= total;

	return missing;
}

int value_index_from_data(network& N, int i, int node_number)
{
	return N.get_nth_node(node_number).get_index_value(N.data_set[i][node_number]);
}

double prob_x_given_mb(int n, int i, int j, network& N, const vector<vector<double> >& CPT)		// nth node, ith data point, jth nvalue
{
	N.data_set[i][n] = N.get_nth_node(n).get_values()[j];		// for its children, fix value of "?" parent
	
	vector<int> par_values;
	for (int x = 0 ; x<N.get_nth_node(n).parents_int.size() ; x++)
	{
		int cur = N.get_nth_node(n).parents_int[x];
		par_values.push_back(value_index_from_data(N,i,cur));
	}
	
	double prob = fetch_from_CPT(n,j,N,CPT,par_values);
	
//	cout << "P(node) : " << prob << "\n";
	
	for (int x = 0 ; x<N.get_nth_node(n).get_children().size() ; x++)
	{
		int cur = N.get_nth_node(n).get_children()[x];
		vector<int> par_val;
		
		for (int y = 0 ; y<N.get_nth_node(cur).parents_int.size() ; y++)
		{
			int cur_par = N.get_nth_node(cur).parents_int[y];
			par_values.push_back(value_index_from_data(N,i,cur_par));
		}
		
		prob *= fetch_from_CPT(cur,value_index_from_data(N,i,cur),N,CPT,par_val);
//		cout << "P(child) : " << prob << "\n";
	}
	
	N.data_set[i][n] = "\"?\"";
	
//	cout << "prob x given m : " << prob << "\n";
	return prob;
}


double fetch_from_CPT(int n, int j, network& N, const vector<vector<double> >& CPT, const vector<int> par_values)
{
	// size of par_values = no. of parents
	Graph_Node cur = N.get_nth_node(n);
	int multiplier = 1;
	int count = 0;
	
	for (int j = par_values.size()-1 ; j>=0 ; j--)
	{
		count 	   += par_values[j]*multiplier;
		multiplier *= N.get_nth_node(cur.parents_int[j]).get_nvalues();
	}
	
	//cout << "herehehrehre : " << CPT[n][(j*(CPT[n].size()/cur.get_nvalues()))+count] << "\n";
	return CPT[n][(j*(CPT[n].size()/cur.get_nvalues()))+count];	
}

void normalize_CPT(network& N, vector<vector<double> >& CPT)
{
	for (int i = 0 ; i<CPT.size() ; i++)
	{
		int block_size = (CPT[i].size()/N.get_nth_node(i).get_nvalues());
		
		for (int j = 0 ; j<block_size ; j++)
		{
			double total = 0.0;
			
			for (int k = j ; k<CPT[i].size() ; k += block_size)
				total += CPT[i][k];
			
			for (int k = j ; k<CPT[i].size() ; k += block_size)
				CPT[i][k] /= total;
		}
	}
}

void randomize_CPT(network& N, vector<vector<double> >& CPT, double factor)
{
	// cout << "--------BEFORE ADDING NOISE-----------\n";
	// for (int i = 0 ; i<CPT.size() ; i++)
	// {
	// 	for (int j = 0 ; j<CPT[i].size() ; j++)
	// 		cout << CPT[i][j] << " ";
	// 	cout << "\n";
	// }
	// cout << "\n";
	
	for (int i = 0 ; i<CPT.size() ; i++)
		for (int j = 0 ; j<CPT[i].size() ; j++)
		{
			if (CPT[i][j]== -1.0)
			{
				CPT[i][j] = ((double) rand() / (RAND_MAX));
			}
			
			else 
			{
				if (rand()%2 == 1) CPT[i][j] = CPT[i][j]+(((double) rand() / (RAND_MAX))/factor);
				else CPT[i][j] = fabs(CPT[i][j]-(((double) rand() / (RAND_MAX))/factor));
			}
		}
		
	normalize_CPT(N,CPT);
	
	// cout << "--------AFTER ADDING NOISE-----------\n";
	// for (int i = 0 ; i<CPT.size() ; i++)
	// {
	// 	for (int j = 0 ; j<CPT[i].size() ; j++)
	// 		cout << CPT[i][j] << " ";
	// 	cout << "\n";
	// }
	// cout << "\n";
}


/*
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
*/


#endif
#include "startup_code.cpp"

#ifndef SOLVE_NET_CPP
	#define SOLVE_NET_CPP

// COUNTING METHODS (w/o "?" cases)

vector<double> count_prob(network& N, int node_number, vector<int> v, const vector<vector<double> >& missing)
	{
		// if node has p parents, v has p elements which have values of the parents
		// return {P(node=values[0] | p_1 = v[1], p_2 = v[2], ...), P(node=values[1] | p_1 = v[1], p_2 = v[2], ...) , ...}
		// counting employed, takes care of "?" or missing expectation
		// to initialise (first time before EM), pass 'missing' of appropriate dimensions with all elements 0.0
		// works even if no parents
	
		Graph_Node gn = N.get_nth_node(node_number);
		vector<double> prob(gn.get_nvalues(),0.0);
		double total = 0.0;
	
		for (int i = 0 ; i<N.data_set.size() ; i++)
		{
			bool flag = true;
			bool missing_given = false;
			int missing_given_nvalue = -1;
		
			for (int j = 0 ; j<gn.parents_int.size() ; j++)
				if (N.data_set[i][gn.parents_int[j]] != N.get_nth_node(gn.parents_int[j]).get_values()[v[j]])
				{
					if (N.data_set[i][gn.parents_int[j]] == "\"?\"")
					{
						missing_given = true;
						missing_given_nvalue = v[j];
					}
					
					else 
					{
						flag = false;
						break;
					}
				}
		
			if (flag)	// givens satisfied
			{				
				if (N.data_set[i][node_number] == "\"?\"")	// => need to weigh only 		| 	givens satisfied, distribution of node
				{
					total++;
					bool initial = true;				// if initial call (very first call), then all missing[i][j] will be 0.0
					
					for (int j = 0 ; j<gn.get_nvalues() ; j++)
					{
						prob[j] += missing[i][j];

						if (missing[i][j] != 0.0) initial = false;
					}
					
					if (initial) total--;				// don't count this case because we don't know distribution of "?", since initial call
				}
			
				else		// givens may or may not have "?"						
				{
					if (!missing_given)				// normal counting
					{
						total++;
						for (int j = 0 ; j<gn.get_nvalues() ; j++)
							if (gn.get_values()[j] == N.data_set[i][node_number]) 
							{
								prob[j]++;
								break;
							}		
					}
					
					else					// need to weight num and denom				| takes care of initial call since missing[i][missing_given_nvalue] == 0.0
					{
						total += missing[i][missing_given_nvalue];
						for (int j = 0 ; j<gn.get_nvalues() ; j++)
							if (gn.get_values()[j] == N.data_set[i][node_number]) 
							{
								prob[j] += missing[i][missing_given_nvalue];
								break;
							}						
					}						
				}
			}		
		}
	
	//	cout << "Total such cases : " << total << "\n";
	
		//if (total == 0) prob = vector<double>(gn.get_nvalues(),-1.0);
		
		total += 0.01*gn.get_nvalues();				// for Laplace smoothing
		
		//else 
			for (int j = 0 ; j<gn.get_nvalues() ; j++)
				prob[j] = (prob[j]+0.01)/total;
	
		return prob;
	}

vector<int> next_given(network& N, vector<int> given, vector<int> parent_nvalues)		// helper for count_CPT
{
	vector<int> ng = given;
	
	for (int i = given.size()-1 ; i>=0 ; i--)
	{
		ng[i]++;
		
		if (ng[i] == parent_nvalues[i]) ng[i] = 0;
		else return ng;
	}
	
	return ng;	// Andrew
}

void count_CPT(network& N, vector<vector<double> >& CPT_in, const vector<vector<double> >& missing)		// generates CPT by counting
{
	for (int i = 0 ; i<N.netSize() ; i++)
	{
		Graph_Node gn = N.get_nth_node(i);
		
		vector<int> given(gn.parents_int.size(),0);
		vector<int> parent_nvalues(gn.parents_int.size());
		
		int CPT_size = gn.get_nvalues();
		for (int j = 0 ; j<gn.parents_int.size() ; j++)
		{
			CPT_size *= N.get_nth_node(gn.parents_int[j]).get_nvalues();
			parent_nvalues[j] = N.get_nth_node(gn.parents_int[j]).get_nvalues();
		}

		vector<double> CPT(CPT_size,-1.0);
		
		int temp = (CPT_size/gn.get_nvalues());
		for (int j = 0 ; j<temp ; j++)
		{
			vector<double> cur = count_prob(N,i,given,missing);
			
			for (int k = 0 ; k<cur.size() ; k++)
				CPT[k*temp+j] = cur[k];
			
			given = next_given(N,given,parent_nvalues);
		}
		
		CPT_in[i] = CPT;
		// for (vector<double>::iterator it = CPT.begin() ; it!=CPT.end() ; it++)
		// 	cout << *it << " ";
		// cout << "\n";
	}
}

#endif





#include "startup_code.cpp"
#include "em.cpp"

#ifndef SOLVE_NET_CPP
	#define SOLVE_NET_CPP

// COUNTING METHODS (w/o "?" cases)

vector<double> count_prob(network& N, int node_number, vector<int> v)
{
	// if node has p parents, v has p elements which have values of the parents
	// return {P(node=values[0] | p_1 = v[1], p_2 = v[2], ...), P(node=values[1] | p_1 = v[1], p_2 = v[2], ...) , ...}
	// basic counting employed
	// works even if no parents
	
	Graph_Node gn = N.get_nth_node(node_number);
	vector<double> prob(gn.get_nvalues(),0.0);
	double total = 0.0;
	
	for (int i = 0 ; i<N.data_set.size() ; i++)
	{
		bool flag = true;
		
		for (int j = 0 ; j<gn.parents_int.size() ; j++)
			if (N.data_set[i][gn.parents_int[j]] != N.get_nth_node(gn.parents_int[j]).get_values()[v[j]])
			{
				flag = false;
				break;
			}
		
		if (flag and (N.data_set[i][node_number] != "\"?\""))	// given condition satisfied
		{
			total++;
			
			for (int j = 0 ; j<gn.get_nvalues() ; j++)
				if (gn.get_values()[j] == N.data_set[i][node_number]) 
				{
					prob[j]++;
					break;
				}		
		}		
	}
	
	// cout << "Total such cases : " << total << "\n";
	
	if (total == 0) prob = vector<double>(gn.get_nvalues(),-1.0);
	
	else 
		for (int j = 0 ; j<gn.get_nvalues() ; j++)
			prob[j] /= total;
	
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
	
	return ng;
}

void count_CPT(network& N)		// generates CPT by basic counting
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
			vector<double> cur = count_prob(N,i,given);
			
			for (int k = 0 ; k<cur.size() ; k++)
				CPT[k*temp+j] = cur[k];
			
			given = next_given(N,given,parent_nvalues);
		}
		
		N.get_nth_node(i).set_CPT(CPT);
		// for (vector<double>::iterator it = CPT.begin() ; it!=CPT.end() ; it++)
		// 	cout << *it << " ";
		// cout << "\n";
	}
}

void solve_network(network& N)
{
	count_CPT(N);
	exp_max(N,0.00001);
}

#endif





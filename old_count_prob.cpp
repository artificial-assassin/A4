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
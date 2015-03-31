#include "startup_code.cpp"

#ifndef SOLVE_NET_CPP
	#define SOLVE_NET_CPP

void solve_network(network& N)
{
	float total = 0.0;
	float low = 0.0;
	float med = 0.0;
	
	for (int i = 0 ; i<N.data_set.size() ; i++)
	{
		if ((N.data_set[i][0] != "\"True\"") and (N.data_set[i][2] != "\"True\"") and (N.data_set[i][3] != "?"))
		{
			total++;
			if (N.data_set[i][3] == "\"Low\""){low++;}
			if (N.data_set[i][3] == "\"Normal\""){med++;}
		}
	}
	
	cout << "P() = " << low/total << " " << med/total << endl;
}

#endif
#include "startup_code.cpp"

#ifndef PRINTS_CPP
	#define PRINTS_CPP

void print_missing(vector<vector<double> >& missing)
{
	cout << "----------MISSING VALUE DISTRIBUTION------------\n";
	for (int i = 0 ; i<missing.size() ; i++)
	{
		for (int j = 0 ; j<missing[i].size() ; j++)
			cout << missing[i][j] << " ";
		cout << "\n";
	}
	
	cout << "\n";
}

void print_CPT(vector<vector<double> >& CPT)
{
	cout << "----------------------CPT------------------------\n";
	for (int i = 0 ; i<CPT.size() ; i++)
	{
		for (int j = 0 ; j<CPT[i].size() ; j++)
			cout << CPT[i][j] << " ";
		cout << "\n";
	}
	cout << "\n\n";
}

#endif
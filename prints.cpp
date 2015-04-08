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

void print_network(string inputFile, string outputFile, network& solvedAlarm)
{
	network Alarm;
	string line;
	int find=0;
  	ifstream myfile(inputFile);
	ofstream fout(outputFile);
  	string temp;
  	string name;
	int node_number = 0;
  	int pnode_number = 0;
    if (myfile.is_open())
    {
    	while (! myfile.eof() )
    	{
    		stringstream ss;
      		getline (myfile,line);
      		
      		
      		ss.str(line);
			if(myfile.eof())
				fout<< line;
			else
				fout << line << "\n";
     		ss>>temp;
     		
     		if(temp.compare("variable")==0)
     		{
                    
     				ss>>name;
     				getline (myfile,line);
                   	fout << line << "\n";
     				stringstream ss2;
     				ss2.str(line);
					
     				for(int i=0;i<4;i++)
     				{
     					ss2>>temp;	
     				}

     				while(temp.compare("};")!=0)
     				{
     					ss2>>temp;
    				}
					
					node_number++;
     		}
     		else if(temp.compare("probability")==0)
     		{
					vector<double> CPT = solvedAlarm.get_nth_node(pnode_number).get_CPT();
					fout << "\ttable ";
					copy(CPT.begin(), CPT.end(), std::ostream_iterator<double>(fout, " "));
					fout << ";\n";
    				getline (myfile,line);
					pnode_number++;
     		}
    	}
  	}  	
	myfile.close();
	fout.close();
}

#endif
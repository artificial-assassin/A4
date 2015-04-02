#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <cmath>
#include <cstdlib>

#ifndef STARTUP_CODE_CPP
	#define STARTUP_CODE_CPP

class network;

// Format checker just assumes you have Alarm.bif and Solved_Alarm.bif (your file) in current directory
using namespace std;

// Our graph consists of a vector of nodes where each node is represented as follows:
class Graph_Node{

private:
	string Node_Name;  // Variable name 
	vector<int> Children; // Children of a particular node - these are index of nodes in graph.
	vector<string> Parents; // Parents of a particular node- note these are names of parents
	int nvalues;  // Number of categories a variable represented by this node can take
	vector<string> values; // Categories of possible values
	vector<double> CPT; // conditional probability table as a 1-d array . Look for BIF format to understand its meaning

public:
	// Constructor- a node is initialised with its name and its categories
	
	vector<int> parents_int;		// SNair
	int node_number;				// index in network
	
    Graph_Node(string name,int n,vector<string> vals, int ind)
	{
		Node_Name=name;
	
		nvalues=n;
		values=vals;
		
		node_number = ind;

	}
	string get_name()
	{
		return Node_Name;
	}
	vector<int> get_children()
	{
		return Children;
	}
	vector<string> get_Parents()
	{
		return Parents;
	}
	vector<double> get_CPT()
	{
		return CPT;
	}
	int get_nvalues()
	{
		return nvalues;
	}
	vector<string> get_values()
	{
		return values;
	}
	int get_index_value(string s)
	{
		for (int i = 0 ; i<values.size() ; i++)
			if (values[i] == s) return i;
		
		cout << s << " : no such value in " << Node_Name << "\n";
	}
	void set_CPT(vector<double> new_CPT)
	{
		CPT.clear();
		CPT=new_CPT;
	}
    void set_Parents(vector<string> Parent_Nodes)
    {
        Parents.clear();
        Parents=Parent_Nodes;
    }
    // add another node in a graph as a child of this node
    int add_child(int new_child_index )
    {
        for(int i=0;i<Children.size();i++)
        {
            if(Children[i]==new_child_index)
                return 0;
        }
        Children.push_back(new_child_index);
        return 1;
    }
	
	void print_node(network& N);

};


 // The whole network represted as a vector of nodes
class network{

	vector<Graph_Node> Pres_Graph;

public:
	
	vector<vector<string> > data_set;
	vector<int> missing_value;				// index of missing value in each data point
	void init();
	
	int addNode(Graph_Node node)
	{
		Pres_Graph.push_back(node);
		return 0;
	}
    
    
	int netSize()
	{
		return Pres_Graph.size();
	}
    // get the index of node with a given name
    int get_index(string val_name)
    {
        vector<Graph_Node>::iterator it;
        int count=0;
        for(it=Pres_Graph.begin();it!=Pres_Graph.end();it++)
        {
            if(it->get_name().compare(val_name)==0)
                return count;
            count++;
        }
        return -1;
    }
// get the node at nth index
    vector<Graph_Node>::iterator get_nth_node_iter(int n)
    {
       vector<Graph_Node>::iterator it;
        int count=0;
        for(it=Pres_Graph.begin();it!=Pres_Graph.end();it++)
        {
            if(count==n) return it;
			
            count++;
        }
		
        return it; 
    }
	
	Graph_Node& get_nth_node(int n) 			 // SNair
		{ return Pres_Graph[n];	}
	
    //get the iterator of a node with a given name
    vector<Graph_Node>::iterator search_node(string val_name)
    {
        vector<Graph_Node>::iterator it;
        for(it=Pres_Graph.begin();it!=Pres_Graph.end();it++)
        {
            if(it->get_name().compare(val_name)==0)
                return it;
        }
    
            cout<<"node not found\n";
        return it;
    }
	

};

network read_network(string fname)
{
	network Alarm;
	string line;
	int find=0;
  	ifstream myfile(fname); 
  	string temp;
  	string name;
  	vector<string> values;
	int node_number = 0;
  	
    if (myfile.is_open())
    {
    	while (! myfile.eof() )
    	{
    		stringstream ss;
      		getline (myfile,line);
      		
      		
      		ss.str(line);
     		ss>>temp;
     		
     		
     		if(temp.compare("variable")==0)
     		{
                    
     				ss>>name;
     				getline (myfile,line);
                   
     				stringstream ss2;
     				ss2.str(line);
     				for(int i=0;i<4;i++)
     				{
     					
     					ss2>>temp;
     					
     					
     				}
     				values.clear();
     				while(temp.compare("};")!=0)
     				{
     					values.push_back(temp);
     					
     					ss2>>temp;
    				}
     				Graph_Node new_node(name,values.size(),values,node_number);
					node_number++;
     				int pos=Alarm.addNode(new_node);

     				
     		}
     		else if(temp.compare("probability")==0)
     		{
                    
     				ss>>temp;
     				ss>>temp;
     				
                    vector<Graph_Node>::iterator it;
                    vector<Graph_Node>::iterator it1;
     				it=Alarm.search_node(temp);
                    int index=Alarm.get_index(temp);
                    ss>>temp;
                    values.clear();
     				while(temp.compare(")")!=0)
     				{
                        it1=Alarm.search_node(temp);
                        it1->add_child(index);
     					values.push_back(temp);
     					
     					ss>>temp;

    				}
                    it->set_Parents(values);
    				getline (myfile,line);
     				stringstream ss2;
                    
     				ss2.str(line);
     				ss2>> temp;
                    
     				ss2>> temp;
                    
     				vector<double> curr_CPT;
                    string::size_type sz;
     				while(temp.compare(";")!=0)
     				{
                        
     					curr_CPT.push_back(atof(temp.c_str()));
     					
     					ss2>>temp;
                       
                        

    				}
                    
                    it->set_CPT(curr_CPT);


     		}
            else
            {
                
            }
     		
     		

    		
    		
    	}
    	
    	if(find==1)
    	myfile.close();
  	}
  	
  	return Alarm;
}

void network::init()			// SNair
{
	// initialise parents_int
	
	for (int i = 0 ; i<Pres_Graph.size() ; i++)
	{
		vector<string> cur_par = get_nth_node(i).get_Parents();
		
		//cout <<  i << ": ";
		for (int j = 0 ; j<cur_par.size(); j++)
		{
			get_nth_node(i).parents_int.push_back((search_node(cur_par[j]))->node_number);
		//	cout << cur_node.parents_int[j] << " ";
		}
		//cout << "\n";	
	}
	
	for (int i = 0 ; i<data_set.size() ; i++)
	{
		bool flag = false;
		
		for (int j = 0 ; j<Pres_Graph.size() ; j++)
		{
			if (data_set[i][j] == "\"?\"")
			{
				missing_value.push_back(j);
				flag = true;
				break;
			}
		}
		
		if (!flag) missing_value.push_back(-1);
	}
	
	// for (int i = 0 ; i<data_set.size() ; i++)
	// 	cout << missing_value[i] << " ";
	// cout << "\n";
}

void get_data(network& N, string fname)						// SNair
{
	string line;
	ifstream myfile(fname); 
	int i = 0;
 
	while(getline(myfile,line))
	{		
	    N.data_set.push_back(vector<string>(N.netSize()));
		
		istringstream iss(line);
		string val;

		for (int j = 0 ; j < N.netSize() ; j++)
		{
			iss >> val;
			N.data_set[i][j] = val;
		}
		
		i++;
	}
	
	// cout << "data_set size : " << N.data_set.size() << "\n";
	//
	// for (int i = 0 ; i < N.netSize() ; i++)
	// 	cout << N.data_set[0][i] << " ";
	// cout << endl;
	
}

void Graph_Node::print_node(network& N)							// SNair
{
	cout << "----------------------------------------\n";
	cout << "[] -- no. of categories   |    () -- index in network \n";
	cout << "Node No   : " << node_number << "\n";
	cout << "Node Name : " << Node_Name << "\n";
	cout << "nvalues   : " << nvalues << "\n";
	cout << "parents   : ";

	for (int i = 0 ; i<parents_int.size() ; i++)
		cout << N.get_nth_node(parents_int[i]).get_name() << "[" << N.get_nth_node(parents_int[i]).get_nvalues() << "](" << parents_int[i] << ") ";
	cout << "\n";
	
	cout << "values    : ";
	for (vector<string>::iterator it = values.begin() ; it != values.end() ; it++)
		cout << *it << " ";
	cout << "\n";
	
	cout << "CPT       : ";
	for (vector<double>::iterator it = CPT.begin() ; it != CPT.end() ; it++)
		cout << *it << " ";
	cout << "\n";
	
	cout << "children  : ";	
	for (int i = 0 ; i < Children.size() ; i++)
		cout << N.get_nth_node(Children[i]).get_name() << "[" << N.get_nth_node(Children[i]).get_nvalues() << "](" << Children[i] << ") ";
	cout << "\n";
	
	cout << "----------------------------------------\n";	
	
}

double learn_error(network& lhs, network& rhs)			// SNair
{
	// assumes same network and ordering
	double error = 0.0;
	
	for (int i = 0 ; i<lhs.netSize() ; i++)
	{
		vector<double> l = lhs.get_nth_node(i).get_CPT();
		vector<double> r = rhs.get_nth_node(i).get_CPT();
		
		for (int j = 0 ; j<l.size() ; j++)
			error += fabs(r[j]-l[j]);
	}
	
	return 10.0*error;
}

#endif






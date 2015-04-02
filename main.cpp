#include "startup_code.cpp"
#include "solve_net.cpp"
#include "em.cpp"

int main(int argc, char** argv)
{
	srand(time(NULL));
	
	if (!(argc == 3 or argc == 4))
	{
		cout << "./startup_code alarm.bif data.dat {gold.bif}\n";
		exit(0);
	}

	network Alarm;
	Alarm = read_network(argv[1]); 
	get_data(Alarm,argv[2]); Alarm.init();
	
//	----------- ^ initialized ^ -----------------

// Example: to do something
	cout<<"Perfect! Hurrah! \n";

	// Graph_Node temp = *Alarm.search_node("\"HRBP\"");
	// temp.print_node(Alarm);

	Graph_Node temp = *Alarm.search_node("\"Press\"");
	temp.print_node(Alarm);
	
	temp = Alarm.get_nth_node(0);
	temp.print_node(Alarm);

	temp = Alarm.get_nth_node(1);
	temp.print_node(Alarm);
		
	solve_network(Alarm);
	
	// for error metric
	if (argc == 4)
	{
		network Gold;
		Gold = read_network(argv[3]);
		
		cout << "Learning error : " << learn_error(Alarm,Gold) << "\n";		
	}
}
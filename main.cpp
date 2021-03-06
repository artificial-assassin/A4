#include "startup_code.cpp"
#include "em.cpp"
#include "solve_net.cpp"

void solve_network(network& N, network& Gold)
{
	exp_max(N,0.0001,Gold);
}

int main(int argc, char** argv)
{
	srand(time(NULL));
	
	if (!(argc == 4 or argc == 5))
	{
		cout << "./startup_code alarm.bif data.dat solved_alarm.bif {gold.bif}\n";
		exit(0);
	}

	network Alarm;
	Alarm = read_network(argv[1]); 
	get_data(Alarm,argv[2]); Alarm.init();
	
//	----------- ^ initialized ^ -----------------

// Example: to do something
	cout<<"Perfect! Hurrah! \n";

	//Graph_Node temp = *Alarm.search_node("\"Alarm\"");
	//temp.print_node(Alarm);

	// Graph_Node temp = *Alarm.search_node("\"Press\"");
	// temp.print_node(Alarm);
	//
	// temp = Alarm.get_nth_node(0);
	// temp.print_node(Alarm);
	//
	// temp = Alarm.get_nth_node(1);
	// temp.print_node(Alarm);
	
	// for error metric
	if (argc == 5)
	{
		network Gold;
		Gold = read_network(argv[4]);
		
		solve_network(Alarm,Gold);
		print_network(argv[1], argv[3], Alarm);
		cout << "Learning error : " << learn_error(Alarm,Gold) << "\n";		
	}
	

}
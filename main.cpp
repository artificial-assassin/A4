#include "startup_code.cpp"
#include "solve_net.cpp"

int main(int argc, char** argv)
{
	if (argc != 3)
	{
		cout << "./startup_code alarm.bif data.dat \n";
		exit(0);
	}

	network Alarm;
	Alarm = read_network(argv[1]);
	get_data(Alarm,argv[2]);
	
//	----------- ^ initialized ^ -----------------

// Example: to do something
	cout<<"Perfect! Hurrah! \n";

	Graph_Node temp = *Alarm.search_node("\"HRBP\"");
	temp.print_node(Alarm);

	temp = Alarm.get_nth_node(3);
	temp.print_node(Alarm);
	
	solve_network(Alarm);
}
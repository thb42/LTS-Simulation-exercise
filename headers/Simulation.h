#ifndef SIMULATION_H
#define SIMULATION_H

#include <random>
#include <typeinfo>
#include "Server.h"

enum type {
	finished = 1,
	araival = 2
};

template<typename ST>
struct event {
	ST time;
	type event_type;
	event(ST t, type et) : time(t), event_type(et) {}
};

//template<typename Distribution>
class Simulation
{
private:
	unsigned int time;
	std::mt19937 gen;
	std::geometric_distribution<int> distAraival;
	std::geometric_distribution<int> distFinished;
	std::list<event<unsigned int>> event_list;
	Server server;

	double throughput;
	double mean_jobs;


public:
	Simulation();
	Simulation(double a_Araival, double a_Finished, Server& s);

	void addServer(Server s);
	void addEvent(event<unsigned int> e);
	void step();

	double getThroughput() { return throughput; }
	double getMeanJobs() { return mean_jobs; }
	~Simulation();
};
#endif
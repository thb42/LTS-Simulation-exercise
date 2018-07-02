
#include <iostream>
#include <random>
#include "Simulation.h"
#include "Server.h"


int main()
{
	int sim_steps = 0;
	double throuput_old = 1.0;
	double mean_jobs_old = 1.0;
	Server myServer = Server();
	Simulation<std::exponential_distribution<double>, double> sim(0.2, 0.5, myServer);
	while (((throuput_old - sim.getThroughput())>0.0000001) || ((mean_jobs_old - sim.getMeanJobs()) > 0.0001)) {
		throuput_old = sim.getThroughput();
		mean_jobs_old = sim.getMeanJobs();
		for (int i = 0; i<100; i++) {
			sim.step();
			sim_steps++;
		}

	}

	std::cout << "Simulation<std::exponential_distribution<double>, double>" << std::endl;
	std::cout << "Durchsatz: " << sim.getThroughput() / sim_steps << std::endl;
	std::cout << "Mittlere Auftragsanzahl: " << sim.getMeanJobs() / sim_steps << std::endl;

	return 0;
}


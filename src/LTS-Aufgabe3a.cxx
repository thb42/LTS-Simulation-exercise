﻿
#include <iostream>
#include <random>
#include "Simulation.h"
#include "Server.h"


int main()
{
	int sim_steps = 0;
	double throuput_old = 1.0;
	Server myServer = Server();
	Simulation<std::geometric_distribution<int>, unsigned int> sim(0.2, 0.5, myServer);
	while ((throuput_old - sim.getThroughput())>0.0000001) {
		throuput_old = sim.getThroughput();
		for (int i = 0; i<100; i++) {
			sim.step();
			sim_steps++;
		}

	}

	std::cout << "Simulation<std::geometric_distribution<int>, unsigned int>" << std::endl;
	std::cout<< "Durchsatz: " << sim.getThroughput()/sim_steps <<std::endl;
	std::cout << "Mittlere Auftragsanzahl: " << sim.getMeanJobs()/sim_steps << std::endl;

    return 0;
}


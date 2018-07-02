#include "Simulation.h"

template class Simulation<std::geometric_distribution<int>, unsigned int>;
template class Simulation<std::exponential_distribution<double>, double>;

template<typename Distribution, typename SimulationTime> 
Simulation<Distribution, SimulationTime>::Simulation()
{
}

template<typename Distribution, typename SimulationTime> 
Simulation<Distribution, SimulationTime>::Simulation(double a_Araival, double a_Finished, Server& s) :
time(0), gen(),
distAraival(Distribution(a_Araival)), 
distFinished(Distribution(a_Finished)),
event_list(std::list<event<SimulationTime>>()), 
server(Server(s)), throughput(0.0),  mean_jobs(0.0)
{
	std::random_device rd;
	gen.seed(rd());
}

template<typename Distribution, typename SimulationTime>
Simulation<Distribution, SimulationTime>::~Simulation()
{
}

template<typename Distribution, typename SimulationTime>
void Simulation<Distribution, SimulationTime>::step() {
	// if we have simulation events we'll proceed them
	if (!event_list.empty()) {
		std::list<event<SimulationTime>> local_list;
		auto event_time = event_list.front().time;
		auto position = event_list.cbegin();

		// select all events with same simulationtime like the first element in list
		for (position;  position != event_list.cend() && event_time == position->time; position++) {
			// save next events for further usage
			local_list.push_front(*position);
		}
		// remove first elements with same simulationtime
		event_list.erase(event_list.cbegin(), position);

		// sort so finish events will be first in list
		local_list.sort([](const event<SimulationTime> E1, const event<SimulationTime> E2) { return E1.event_type < E2.event_type; });
		// and we will proceed them first
		for(auto iter = local_list.cbegin(); iter != local_list.cend(); ++iter) {
			switch (iter->event_type) {
			case araival:
				mean_jobs += server.getWait() * (event_time - time);
				server.add();
				time = event_time;
				{ bool has_finish = false;
				for (auto iter = event_list.cbegin(); iter != event_list.cend(); iter++) {
					has_finish = has_finish || (iter->event_type == finished);
				}
				addEvent(event<SimulationTime>(time + 1 + distAraival(gen), araival));
				if (!has_finish) {
					addEvent(event<SimulationTime>(time + 1 + distFinished(gen), finished));
				}}
				break;

			case finished:
				mean_jobs += server.getWait() * (event_time - time);
				throughput += 1;
				server.finish();
				time = event_time;
				if (server.getWait() > 0) {
					addEvent(event<SimulationTime>(time + 1 + distFinished(gen), finished));
				}
				break;

			default:
				break;
			}
		}
	}
	// else we have no events in event list we'll generate a araival events
	else {
		addEvent(event<SimulationTime>(time + 1 + distAraival(gen), araival));
		// and if we have jobs waiting we'll generate a finish event
		if (server.getWait() > 0) {
			addEvent(event<SimulationTime>(time + 1 + distFinished(gen), finished));
		}
	}
	event_list.sort([](const event<SimulationTime> E1, const event<SimulationTime> E2) { return E1.time < E2.time; });
}

template<typename Distribution, typename SimulationTime>
void Simulation<Distribution, SimulationTime>::addServer(Server s) { 
	server = s; 
}

template<typename  Distribution, typename SimulationTime>
void Simulation<Distribution, SimulationTime>::addEvent(event<SimulationTime> e) { 
	event_list.push_back(e); 
}

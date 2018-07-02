#include "Simulation.h"

/*template<typename unsigned int, class Distribution> 
Simulation<unsigned int, Distribution>::Simulation()
{
}*/

//template<typename Distribution> 
Simulation::Simulation(double a_Araival, double a_Finished, Server& s)/* :
time(),
gen(),
distAraival(Distribution(a_Araival)), 
distFinished(Distribution(a_Finished)),
event_list(std::list<event<unsigned int>>()), 
server(s), 
throughput(0.0),  
mean_jobs(0.0)*/
{
	time = 0;
	distAraival = std::geometric_distribution<int>(a_Araival);
	distFinished = std::geometric_distribution<int>(a_Finished);
	event_list = std::list<event<unsigned int>>();
	server = Server(s);
	throughput = 0.0;
	mean_jobs = 0.0;
	std::random_device rd;
	gen.seed(rd());
}

/*template<typename unsigned int, class Distribution>
Simulation<unsigned int, Distribution>::~Simulation()
{
}*/

//template<typename Distribution>
void Simulation::step() {
	if (!event_list.empty()) {
		std::list<event<unsigned int>> local_list;
		auto event_time = event_list.front().time;
		auto position = event_list.cbegin();

		// remove first elements with same simulationtime
		for (position;  position != event_list.cend() && event_time == position->time; position++) {
			//auto e = event_list.front();
			local_list.push_front(*position);
		}
		event_list.erase(event_list.cbegin(), position);
		//event_list.splice(--position, local_list);

		//todo local_eventlist nach typ sortieren, finish zuerst
		local_list.sort([](const event<unsigned int> E1, const event<unsigned int> E2) { return E1.event_type < E2.event_type; });
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
				addEvent(event<unsigned int>(time + 1 + distAraival(gen), araival));
				if (!has_finish) {
					addEvent(event<unsigned int>(time + 1 + distFinished(gen), finished));
				}}
				break;

			case finished:
				mean_jobs += server.getWait() * (event_time - time);
				throughput += 1;
				server.finish();
				time = event_time;
				if (server.getWait() > 0) {
					addEvent(event<unsigned int>(time + 1 + distFinished(gen), finished));
				}
				break;

			default:
				break;
			}
		}
	}
	else {
		addEvent(event<unsigned int>(time + 1 + distAraival(gen), araival));
		if (server.getWait() > 0) {
			addEvent(event<unsigned int>(time + 1 + distFinished(gen), finished));
		}
	}
	event_list.sort([](const event<unsigned int> E1, const event<unsigned int> E2) { return E1.time < E2.time; });
}

//template<typename Distribution>
void Simulation::addServer(Server s) { 
	server = s; 
}
//template<typename  Distribution>
void Simulation::addEvent(event<unsigned int> e) { 
	event_list.push_back(e); 
}

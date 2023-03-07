#include <algorithm>
#include <cmath>

#include <fstream>
#include <iostream>

#include <vector>

#include "deps/nlohmann/json.hpp"

#include "src/Entity.hpp"
#include "src/simulate.hpp"

int main()
{
	std::ifstream json_file("example.json");

	if (!json_file)
	{
		std::cout << "Couldn't open example file!\n";
		return -1;
	}
	auto json_data = nlohmann::json::parse(json_file);

	std::vector<Entity> initial_conditions;

	for (auto &something : json_data)
	{
		// std::cout << something << '\n';
		initial_conditions.reserve(something.size());
		for (auto &object : something)
		{
			initial_conditions.push_back(object.get<Entity>());
		}
	}

	std::cout << "Entities parsed from json: " << initial_conditions.size() << '\n';

	// // each vec in simulation is 1 frame
	std::vector<std::vector<Entity>> simulation = simulate(initial_conditions, 2 * 60);

	std::cout << "Num frames in simulation: " << simulation.size() << '\n';
	std::cout << "Total objects in first frame: " << simulation[0].size() << '\n';
	std::cout << "Total objects in last frame: " << simulation[simulation.size() - 1].size() << '\n';

	std::ofstream file("sim_frames.json");

	file << nlohmann::json(simulation).dump();
}
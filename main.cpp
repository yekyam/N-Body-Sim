#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string_view>
#include <vector>

#include <SDL2/SDL.h>

#include "deps/nlohmann/json.hpp"

#include "src/Entity.hpp"
#include "src/Renderer.hpp"
#include "src/simulate.hpp"

enum class SIMULATION_RESULT { SUCCESS, IN_FAILURE, OUT_FAILURE };

SIMULATION_RESULT
export_simulation_to_json(std::string_view infile, std::string_view outfile, size_t total_frames, bool verbose = false)
{
	std::ifstream json_file(infile);

	if (!json_file)
	{
		std::cout << "Couldn't open example file!\n";
		return SIMULATION_RESULT::IN_FAILURE;
	}
	auto json_data = nlohmann::json::parse(json_file);

	std::vector<Entity> initial_conditions;

	if (verbose)
	{
		std::cout << "Inital entites:\n";
	}
	for (auto &something : json_data)
	{

		initial_conditions.reserve(something.size());
		for (auto &object : something)
		{
			auto e = object.get<Entity>();
			initial_conditions.push_back(e);
			if (verbose)
			{
				std::cout << e << '\n';
			}
		}
	}

	auto start_time = std::chrono::steady_clock::now();

	// // each vec in simulation is 1 frame
	std::vector<std::vector<Entity>> simulation = simulate(initial_conditions, total_frames);

	auto end_time = std::chrono::steady_clock::now();

	if (verbose)
	{
		std::cout << "Entities parsed from json: " << initial_conditions.size() << "\n\n";
		std::cout << "Num frames in simulation: " << simulation.size() << '\n';
		std::cout << "Total objects in first frame: " << simulation[0].size() << '\n';
		std::cout << "Total objects in last frame: " << simulation[simulation.size() - 1].size() << '\n';

		std::cout << "\n";

		auto total_time = end_time - start_time;

		std::cout << "Duration in milliseconds: ";
		std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(total_time).count() << '\n';
	}

	std::ofstream file(outfile);

	file << nlohmann::json(simulation).dump();
	return SIMULATION_RESULT::SUCCESS;
}

int main()
{
	// auto res = export_simulation_to_json("example.json", "sim_frames.json", 30 * 60, true);

	// if (res != SIMULATION_RESULT::SUCCESS)
	// {
	// 	std::cout << "Couldn't create frames, error\n";
	// }
	// std::cout << "Done creating frames\n";
	std::vector<std::vector<Entity>> frames;

	Renderer r(frames);

	bool running = true;
	while (running)
	{
		SDL_Event e;
		while (SDL_PollEvent(&e) > 0)
		{
			SDL_UpdateWindowSurface(r.m_window);
		}
	}
}
#pragma once

#include <fstream>
#include <iostream>
#include <string_view>
#include <vector>

#include "../deps/nlohmann/json.hpp"

#include "Entity.hpp"
#include "simulate.hpp"

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

	for (auto &something : json_data)
	{

		initial_conditions.reserve(something.size());
		for (auto &object : something)
		{
			auto e = object.get<Entity>();
			initial_conditions.push_back(e);
			auto current_entity_index = initial_conditions.size() - 1;
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

std::vector<std::vector<Entity>> load_frames_from_file(std::string_view infile)
{
	std::vector<std::vector<Entity>> frames;
	std::fstream file(infile);
	if (!file)
	{
		std::cout << "Error opening file " << infile << '\n';
		return frames;
	}
	auto json_data = nlohmann::json::parse(file);

	frames.reserve(json_data.size());

	for (const auto &frame_json : json_data)
	{
		std::vector<Entity> frame;
		frame.reserve(frame_json.size());

		for (const auto &entity_json : frame_json)
		{
			auto e = entity_json.get<Entity>();

			frame.push_back(e);
		}
		frames.push_back(frame);
	}
	return frames;
}
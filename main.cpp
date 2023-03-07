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

void render_frames(const std::vector<std::vector<Entity>> &frames, int fps = 60)
{
	Renderer r(frames);

	SDL_RenderSetVSync(r.m_renderer, 0);

	float time_per_frame = (1.0 / fps) * 1000;

	bool running = true;
	while (running)
	{
		r.clear();

		SDL_Event e;
		while (SDL_PollEvent(&e) > 0)
		{
			switch (e.type)
			{
			case SDL_QUIT:
				running = false;
			}
		}
		auto start = SDL_GetTicks();

		r.draw_frame();

		SDL_RenderPresent(r.m_renderer);

		auto end = SDL_GetTicks();

		auto total_time = end - start;

		if (total_time < time_per_frame)
		{

			SDL_Delay(time_per_frame - total_time);
		}

		if (r.done_rendering)
			return;
	}
}

int main()
{
	// auto res = export_simulation_to_json("example.json", "sim_frames.json", 2 * 60);

	// if (res != SIMULATION_RESULT::SUCCESS)
	// {
	// 	std::cout << "Couldn't create frames, error\n";
	// }

	std::vector<std::vector<Entity>> frames = load_frames_from_file("sim_frames.json");

	render_frames(frames, 60);
}
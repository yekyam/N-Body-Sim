#include <algorithm>
#include <chrono>
#include <cmath>
#include <fstream>
#include <iostream>
#include <string_view>
#include <vector>

#include <SDL2/SDL.h>

#include "deps/nlohmann/json.hpp"
#include "deps/yekyam/Args.hpp"

#include "src/Entity.hpp"
#include "src/HandleJSON.hpp"
#include "src/Renderer.hpp"

int main(int argc, char **argv)
{
	YAGP::Args args(argc, argv);

	if (argc == 1)
	{
		std::cout << "usage:\n";
		std::cout << "./main -in inital_conditions.json -out simulation_frames.json -fps 60 -time 2\n\n";
		std::cout << "./main -r simulation_frames.json -fps 60\n\n";
		return 0;
	}
	auto infile = args.get<std::string_view>("-in");
	auto outfile = args.get<std::string_view>("-out");
	auto simulation_file = args.get<std::string_view>("-r");
	auto time = args.get<float>("-time");
	auto fps = args.get<int>("-fps");

	if (infile.has_value() && outfile.has_value())
	{
		auto res = export_simulation_to_json(infile.value(), outfile.value(), time.value() * fps.value());

		if (res != SIMULATION_RESULT::SUCCESS)
		{
			std::cout << "Couldn't create frames, error\n";
		}
		return 0;
	}

	if (!simulation_file.has_value() || !fps.has_value())
	{
		std::cout << "Error; refer to documentation\n";
		return 0;
	}

	std::vector<std::vector<Entity>> frames = load_frames_from_file(simulation_file.value());

	render_frames(frames, fps.value());
}
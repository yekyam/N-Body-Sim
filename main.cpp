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
#include "src/HandleJSON.hpp"
#include "src/Renderer.hpp"

int main(int argc, char **argv)
{
	// auto res = export_simulation_to_json("example.json", "sim_frames.json", 2 * 60);

	// if (res != SIMULATION_RESULT::SUCCESS)
	// {
	// 	std::cout << "Couldn't create frames, error\n";
	// }

	std::vector<std::vector<Entity>> frames = load_frames_from_file("sim_frames.json");

	render_frames(frames, 60);
}
#include <cmath>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <stddef.h>

#include "deps/nlohmann/json.hpp"

struct Vec2d
{
	float x;
	float y;

	friend Vec2d operator+(const Vec2d &lhs, const Vec2d &rhs)
	{ //
		return Vec2d{lhs.x + rhs.x, lhs.y + rhs.y};
	}

	friend Vec2d operator-(const Vec2d &lhs, const Vec2d &rhs)
	{ //
		return Vec2d{lhs.x - rhs.x, lhs.y - rhs.y};
	}

	friend Vec2d operator*(const Vec2d &lhs, const Vec2d &rhs)
	{ //
		return Vec2d{lhs.x * rhs.x, lhs.y * rhs.y};
	}

	friend Vec2d operator/(const Vec2d &lhs, const Vec2d &rhs)
	{ //
		return Vec2d{lhs.x / rhs.x, lhs.y / rhs.y};
	}
};

struct RGB
{
	int r, g, b;
};

struct Entity
{
	Vec2d center;
	Vec2d old_center;
	Vec2d velocity;
	float radius;
	float mass;
	RGB color;
};

float calculate_distance(const Vec2d &lhs, const Vec2d &rhs)
{
	return sqrt(pow(rhs.x - lhs.x, 2) + pow(rhs.y - lhs.y, 2));
}

std::vector<std::vector<Entity>> simulate(const std::vector<Entity> initial_conditions, size_t total_frames)
{
	std::vector<std::vector<Entity>> frames;
	frames.reserve(total_frames);

	frames.push_back(initial_conditions);

	for (size_t i = 1; i < total_frames; i++)
	{
		const auto &previous_frame = frames[i - 1];

		std::vector<Entity> current_frame;

		current_frame.reserve(previous_frame.size());

		for (const auto &obj1 : previous_frame)
		{
			Entity new_entity;
			Vec2d total_acceleration{0.0, 0.0};

			for (const auto &obj2 : previous_frame)
			{
				if (&obj1 == &obj2)
				{
					continue;
				}
				float distance = calculate_distance(obj1.center, obj2.center);

				float gravitation_force = (obj1.mass * obj2.mass) / pow(distance, 2);
				gravitation_force = -gravitation_force;

				float delta_x = obj1.center.x - obj2.center.x;
				float delta_y = obj1.center.y - obj2.center.y;

				float angle = atan2(delta_y, delta_x);

				float acceleration = gravitation_force / obj1.mass;

				Vec2d acceleration_vec{acceleration * cos(angle), acceleration * sin(angle)};

				std::cout << "Angle: " << angle << '\n';
				std::cout << "Cos of angle: " << cos(angle) << '\n';
				std::cout << "Sin of angle: " << sin(angle) << '\n';

				total_acceleration = acceleration_vec + total_acceleration;
			}

			Vec2d current_velocity = total_acceleration + obj1.velocity;

			// maybe add delta here, but if every frame is 1, I don't think its necessary
			Vec2d new_positon = obj1.center + current_velocity;

			new_entity.old_center = obj1.center;
			new_entity.center = new_positon;
			new_entity.velocity = current_velocity;
			current_frame.push_back(new_entity);
		}
		frames.push_back(current_frame);
	}
	return frames;
}

int main()
{
	// std::ifstream json_file("example.json");

	// if (!json_file)
	// {
	// 	std::cout << "Couldn't open example file!\n";
	// 	return -1;
	// }
	// auto json_data = nlohmann::json::parse(json_file);

	std::vector<Entity> initial_conditions;

	initial_conditions.reserve(2);

	initial_conditions.push_back(
	    Entity{Vec2d{650, 400}, Vec2d{650, 400}, Vec2d{0, 0}, 50.0f, 250.0f, RGB{255, 0, 0}});

	initial_conditions.push_back(
	    Entity{Vec2d{850, 400}, Vec2d{850, 400}, Vec2d{0, 0}, 50.0f, 250.0f, RGB{0, 255, 0}});

	// each vec in simulation is 1 frame
	std::vector<std::vector<Entity>> simulation = simulate(initial_conditions, 1 * 24);

	std::cout << "Num frames in simulation: " << simulation.size();

	for (const auto &frame : simulation)
	{
		std::cout << "Objects in frame: " << frame.size() << '\n';
	}
}
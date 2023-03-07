#include <algorithm>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stddef.h>
#include <vector>

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

	friend std::ostream &operator<<(std::ostream &os, const Entity &e)
	{
		os << "Entity: " << &e << '\n';
		os << "\tCenter: "
		   << "Vec(" << e.center.x << ", " << e.center.y << ")\n";
		os << "\tRadius: " << e.radius << '\n';
		os << "\tMass: " << e.mass << '\n';
		os << "\tVelocity: Vec2d(" << e.velocity.x << ", " << e.velocity.y << ")\n";
		os << "\tColor: " << e.color.r << ' ' << e.color.g << ' ' << e.color.b << '\n';
		return os;
	}

	friend void to_json(nlohmann::json &j, const Entity &e)
	{
		j = nlohmann::json{{"position", {{"x", e.center.x}, {"y", e.center.y}}},
				   {"radius", e.radius},
				   {"mass", e.mass},
				   {"color", {{"r", e.color.r}, {"g", e.color.g}, {"b", e.color.b}}},
				   {"velocity",
				    {
					{"x", e.velocity.x},
					{"y", e.velocity.y},
				    }}};
	}

	friend void from_json(const nlohmann::json &j, Entity &e)
	{ //
		j.at("position").at("x").get_to(e.center.x);
		j.at("position").at("y").get_to(e.center.y);

		j.at("radius").get_to(e.radius);

		if (!j.contains("mass"))
		{
			e.mass = pow(e.radius, 3);
		} else
		{
			j.at("mass").get_to(e.mass);
		}
		if (!j.contains("color"))
		{
			e.color.r = 255;
			e.color.g = 255;
			e.color.b = 255;

		} else
		{
			j.at("color").at("r").get_to(e.color.r);
			j.at("color").at("g").get_to(e.color.g);
			j.at("color").at("b").get_to(e.color.b);
		}

		if (!j.contains("velocity"))
		{
			e.velocity.x = 0;
			e.velocity.y = 0;
		} else
		{
			j.at("velocity").at("x").get_to(e.velocity.x);
			j.at("velocity").at("y").get_to(e.velocity.y);
		}
	}
};

float calculate_distance(const Vec2d &lhs, const Vec2d &rhs)
{
	return sqrt(pow(rhs.x - lhs.x, 2) + pow(rhs.y - lhs.y, 2));
}

std::vector<Entity> calculate_new_positions_from_previous_frame(const std::vector<Entity> previous_frame)
{
	std::vector<Entity> current_frame;
	current_frame.reserve(previous_frame.size());

	for (const auto &obj1 : previous_frame)
	{
		Entity new_entity(obj1);
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
	return current_frame;
}

std::vector<Entity> do_constraints(const std::vector<Entity> frame)
{
	std::vector<Entity> frame_after_contraint;
	frame_after_contraint.reserve(frame.size());

	std::vector<const Entity *> deleted_objects;
	deleted_objects.reserve(frame.size());

	for (const auto &obj1 : frame)
	{
		for (const auto &obj2 : frame)
		{
			if (&obj1 == &obj2)
			{
				continue;
			}
			auto obj1_marked_deleted =
			    std::find(deleted_objects.begin(), deleted_objects.end(), &obj1) != deleted_objects.end();

			auto obj2_marked_deleted =
			    std::find(deleted_objects.begin(), deleted_objects.end(), &obj2) != deleted_objects.end();

			if (obj1_marked_deleted && obj2_marked_deleted)
			{
				continue;
			}

			float distance = calculate_distance(obj1.center, obj2.center);

			if (distance <= (obj1.radius + obj2.radius))
			{
				Entity new_entity(obj1); // we just want a "base" object

				new_entity.radius = obj1.radius + obj2.radius;
				new_entity.mass = obj1.mass + obj2.mass;

				if (obj1.mass < obj2.mass)
				{
					new_entity.center = obj2.center;
					new_entity.color = obj2.color;
				}
				frame_after_contraint.push_back(new_entity);
				deleted_objects.push_back(&obj1);
				deleted_objects.push_back(&obj2);
			}
		}
	}
	for (const auto &obj : frame)
	{
		bool found_obj_in_deleted =
		    std::find(deleted_objects.begin(), deleted_objects.end(), &obj) != deleted_objects.end();
		if (!found_obj_in_deleted)
		{
			frame_after_contraint.push_back(obj);
		}
	}
	return frame_after_contraint;
}

std::vector<std::vector<Entity>> simulate(const std::vector<Entity> initial_conditions, size_t total_frames)
{
	std::vector<std::vector<Entity>> frames;
	frames.reserve(total_frames);

	frames.push_back(initial_conditions);

	for (size_t i = 1; i < total_frames; i++)
	{
		const auto &previous_frame = frames[i - 1];

		auto current_frame = calculate_new_positions_from_previous_frame(previous_frame);

		auto constrained_frame = do_constraints(current_frame);

		frames.push_back(constrained_frame);
	}
	return frames;
}

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

	for (auto &e : initial_conditions)
	{
		std::cout << e << '\n';
	}

	// initial_conditions.reserve(2);

	// initial_conditions.push_back(
	//     Entity{Vec2d{650, 400}, Vec2d{650, 400}, Vec2d{0, 0}, 50.0f, 125000.0f, RGB{255, 0, 0}});

	// initial_conditions.push_back(Entity{Vec2d{850, 400}, Vec2d{850, 400}, Vec2d{0, 0}, 2.0f, 8.0f, RGB{0,
	// 255, 0}});

	// // each vec in simulation is 1 frame
	// std::vector<std::vector<Entity>> simulation = simulate(initial_conditions, 2 * 60);

	// std::cout << "Num frames in simulation: " << simulation.size() << '\n';
	// std::cout << "Total objects in first frame: " << simulation[0].size() << '\n';
	// std::cout << "Total objects in last frame: " << simulation[simulation.size() - 1].size() << '\n';
}
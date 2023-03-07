#include <cstddef>
#include <fstream>
#include <iostream>
#include <stddef.h>

#include "deps/nlohmann/json.hpp"

template <typename T>
struct Vec2d
{
	T x;
	T y;

	friend Vec2d<T> operator+(const Vec2d<T> &lhs, const Vec2d<T> &rhs)
	{
		return Vec2d<T>{lhs.x + rhs.x, lhs.y + rhs.y};
	}

	friend Vec2d<T> operator-(const Vec2d<T> &lhs, const Vec2d<T> &rhs)
	{
		return Vec2d<T>{lhs.x - rhs.x, lhs.y - rhs.y};
	}

	friend Vec2d<T> operator*(const Vec2d<T> &lhs, const Vec2d<T> &rhs)
	{
		return Vec2d<T>{lhs.x * rhs.x, lhs.y * rhs.y};
	}

	friend Vec2d<T> operator/(const Vec2d<T> &lhs, const Vec2d<T> &rhs)
	{
		return Vec2d<T>{lhs.x / rhs.x, lhs.y / rhs.y};
	}
};

struct RGB
{
	int r, g, b;
};

template <typename T>
struct Entity
{
	Vec2d<T> center;
	Vec2d<T> old_center;
	Vec2d<T> velocity;
	T radius;
	T mass;
	RGB color;
};

using DefaultEntity = Entity<float>;

template <typename T>
std::vector<std::vector<Entity<T>>> simulate(const std::vector<Entity<T>> initial_conditions, size_t total_frames)
{
	for (size_t i = 0; i < total_frames; i++)
	{
	}
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

	std::vector<DefaultEntity> initial_conditions;

	initial_conditions.reserve(2);

	initial_conditions.push_back(DefaultEntity{
	    Vec2d<float>{650, 400}, Vec2d<float>{650, 400}, Vec2d<float>{0, 0}, 50.0f, 250.0f, RGB{255, 0, 0}});

	initial_conditions.push_back(DefaultEntity{
	    Vec2d<float>{850, 400}, Vec2d<float>{850, 400}, Vec2d<float>{0, 0}, 50.0f, 250.0f, RGB{0, 255, 0}});

	std::vector<std::vector<DefaultEntity>> simulation = simulate(initial_conditions,
								      10 * 60); // each vec in simulation is 1 frame
}
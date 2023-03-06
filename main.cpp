#include <fstream>
#include <iostream>

#include "deps/nlohmann/json.hpp"

template <typename T>
struct Vec2d
{
	T x;
	T y;

	friend Vec2d<T> operator+(const Vec2d<T> &lhs, const Vec2d<T> &rhs)
	{
		return Vec2d<T>(lhs.x + rhs.x, lhs.y + rhs.y);
	}
};

int main()
{
	std::ifstream json_file("example.json");

	if (!json_file)
	{
		std::cout << "Couldn't open example file!\n";
		return -1;
	}
	auto json_data = nlohmann::json::parse(json_file);
}
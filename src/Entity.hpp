#pragma once

#include "../deps/nlohmann/json.hpp"

#include "RGB.hpp"
#include "Vec2d.hpp"

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
		try
		{
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
		} catch (nlohmann::json::type_error &exception)
		{
			e.center.x = 0;
			e.center.y = 0;
			e.mass = 0;
			e.radius = 0;
			e.color = RGB{0, 0, 0};
			e.velocity.x = 0;
			e.velocity.y = 0;
		}
	}
};
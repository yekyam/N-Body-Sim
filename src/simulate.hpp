#pragma once

#include "Entity.hpp"
#include "Vec2d.hpp"
#include <cmath>

#include <vector>

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
			double distance = calculate_distance(obj1.center, obj2.center);

			if (distance == 0)
			{
				distance = 0.0000001;
			}

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

			if (obj1_marked_deleted || obj2_marked_deleted)
			{
				continue;
			}

			float distance = calculate_distance(obj1.center, obj2.center);

			if (distance <= (obj1.radius + obj2.radius))
			{
				Entity new_entity(obj1); // we just want a "base" object

				new_entity.radius = obj1.radius + obj2.radius;
				new_entity.mass = obj1.mass + obj2.mass;
				new_entity.velocity = Vec2d{0.0, 0.0};

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

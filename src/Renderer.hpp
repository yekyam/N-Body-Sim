#pragma once

#include <cmath>
#include <cstddef>
#include <exception>
#include <iostream>
#include <stdexcept>
#include <vector>

#include <SDL2/SDL.h>

#include "Entity.hpp"
#include "RGB.hpp"
#include "Vec2d.hpp"

struct Renderer
{
	std::vector<std::vector<Entity>> frames;

	SDL_Window *m_window;
	SDL_Renderer *m_renderer;
	size_t current_frame;

	bool done_rendering;

	Renderer(std::vector<std::vector<Entity>> frames) : frames(frames), current_frame(0), done_rendering(false)
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			throw std::runtime_error("Couldn't init SDL2?");
		}

		m_window = SDL_CreateWindow(
		    "N-Body Gravity Sim", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1500, 800, 0);
		if (!m_window)
		{
			throw std::runtime_error("Couldn't create SDL2 window?");
		}
		m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
		std::cout << "Total frames: " << frames.size() << '\n';
	}

	void clear()
	{
		SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
		SDL_RenderClear(m_renderer);
	}

	void draw_frame()
	{

		if (done_rendering)
			return;
		if (current_frame >= frames.size())
		{
			done_rendering = true;
			return;
		}
		const auto &frame = frames[current_frame];

		for (const auto &e : frame)
		{
			draw_circle(e);
		}

		current_frame++;
	}

	// https://stackoverflow.com/questions/65723827/sdl2-function-to-draw-a-filled-circle
	void draw_circle(const Entity &e)
	{

		SDL_SetRenderDrawColor(m_renderer, e.color.r, e.color.g, e.color.b, 255);

		int x = 0;
		int y = e.radius;
		int m = 5 - 4 * e.radius;

		int center_x = std::round(e.center.x);
		int center_y = std::round(e.center.y);

		// previous_positions.push_back(e.center)

		while (x <= y)
		{
			SDL_RenderDrawLine(m_renderer, center_x - x, center_y - y, center_x + x, center_y - y);
			SDL_RenderDrawLine(m_renderer, center_x - y, center_y - x, center_x + y, center_y - x);
			SDL_RenderDrawLine(m_renderer, center_x - y, center_y + x, center_x + y, center_y + x);
			SDL_RenderDrawLine(m_renderer, center_x - x, center_y + y, center_x + x, center_y + y);

			if (m > 0)
			{
				y -= 1;
				m -= 8 * y;
			}
			x += 1;
			m += 8 * x + 4;
		}
	}
};

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
		std::cout << "Time per frame: " << total_time << '\n';
		std::cout << "Objects in frame: " << frames[r.current_frame].size() << '\n';

		if (total_time < time_per_frame)
		{

			SDL_Delay(time_per_frame - total_time);
		}

		if (r.done_rendering)
			return;
	}
}
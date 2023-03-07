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

		int x = std::round(e.center.x);
		int y = std::round(e.center.y);

		for (int w = 0; w < e.radius * 2; w++)
		{
			for (int h = 0; h < e.radius * 2; h++)
			{
				int dx = e.radius - w; // horizontal offset
				int dy = e.radius - h; // vertical offset
				if ((dx * dx + dy * dy) <= (e.radius * e.radius))
				{
					SDL_RenderDrawPoint(m_renderer, x + dx, y + dy);
				}
			}
		}
	}
};
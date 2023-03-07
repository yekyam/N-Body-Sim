#pragma once

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
	SDL_Surface *m_window_surface;

	Renderer(std::vector<std::vector<Entity>> frames) : frames(frames)
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
		m_window_surface = SDL_GetWindowSurface(m_window);
	}
};
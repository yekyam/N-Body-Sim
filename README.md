# N-Body Sim

An N-Body gravity simulator written in C++ with the added feature of splitting simulation and rendering into two discrete steps.

## Rationale

Typically, N-Body simulations are very slow, and by splitting the simulation into two steps, the render runs much faster.
Plus, I thought it'd be cool to have a way to save simulations and play them back.
I decided on the JSON file format because the files are easy to create by both machines and humans. For example, Python has an amazing JSON library to generate initial_condition.json files 

## Steps

#### 1) Calculation 

This step calculates every frame of the simulation given the initial conditions, the FPS, and the total runtime.

The initial conditions are in a .json file, which contains an array of objects and their information.

The output is a .json file which contains every frame and the position of objects at each frame.

#### 2) Rendering

This step renders every frame in the outputted .json file at the specified FPS.
Collisions are present in the simulation. In this case, if two objects collide they create combine to create a new object at the collision site with the mass and radius of the two.



## Usage

inital_conditions.json:
The file describes the objects in the first frame of the simulation.
The only required attributes for an object are `position` and `radius`.
Defaults are:
- Mass = radius ^ 3
- Velocity = {0, 0}
- Color = {R: 255, G: 255, B: 255}
```
[
	{
		"position": {
			"x": 650,
			"y": 400
		},
		"radius": 50,
		"mass": 250,
		"color": {
			"r": 255,
			"g": 0,
			"b": 0
		},
		"velocity": {
			"x": 5,
			"y": -5
		}
	},
	{
		"position": {
			"x": 850,
			"y": 400
		},
		"radius": 50
	}
]
```

#### 1) Calculation

./main -in (initial_conditions.json) -out (full_simulation.json) -fps (60) -time (5) [-v]
- -in The name of the .json file containing the initial_conditions
- -out The name of the .json output file contating the frames of the simulation
- -fps The intended fps to run the simulation at
- -time The total time (in seconds)
- -v Optional: Outputs the number of objects in the first frame, the number of objects in the last frame, and the total ms to calculate

#### 2) Render

./main -r (full_simulation.json) -fps 60
- -r The name of the .json file containing the frames of the simulation
- -fps The number of frames to try and render at

## Dependencies:
- [nlohmann's JSON](https://github.com/nlohmann/json) (included in deps/)
- [YAGP](https://github.com/yekyam/YAGP) (included in deps/)
- SDL2
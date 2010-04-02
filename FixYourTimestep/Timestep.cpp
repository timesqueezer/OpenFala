// Timestep Example Source Code
// Copyright (c) Glenn Fiedler 2004
// http://www.gaffer.org/articles

bool openDisplay(int width, int height);
void updateDisplay();
void closeDisplay();
void onQuit();
float time();

#include "Apple.h"
#include "Windows.h"

bool quit = false;

void onQuit()
{
	quit = true;
}

struct State
{
	float x;
	float v;
};

struct Derivative
{
	float dx;
	float dv;
};

State interpolate(const State &previous, const State &current, float alpha)
{
	State state;
	state.x = current.x*alpha + previous.x*(1-alpha);
	state.v = current.v*alpha + previous.v*(1-alpha);
	return state;
}

float acceleration(const State &state, float t)
{
	const float k = 10;
	const float b = 1;
	return - k*state.x - b*state.v;
}

Derivative evaluate(const State &initial, float t)
{
	Derivative output;
	output.dx = initial.v;
	output.dv = acceleration(initial, t);
	return output;
}

Derivative evaluate(const State &initial, float t, float dt, const Derivative &d)
{
	State state;
	state.x = initial.x + d.dx*dt;
	state.v = initial.v + d.dv*dt;
	Derivative output;
	output.dx = state.v;
	output.dv = acceleration(state, t+dt);
	return output;
}

void integrate(State &state, float t, float dt)
{
	Derivative a = evaluate(state, t);
	Derivative b = evaluate(state, t, dt*0.5f, a);
	Derivative c = evaluate(state, t, dt*0.5f, b);
	Derivative d = evaluate(state, t, dt, c);
	
	const float dxdt = 1.0f/6.0f * (a.dx + 2.0f*(b.dx + c.dx) + d.dx);
	const float dvdt = 1.0f/6.0f * (a.dv + 2.0f*(b.dv + c.dv) + d.dv);
	
	state.x = state.x + dxdt*dt;
	state.v = state.v + dvdt*dt;
}

int main()
{
	const int width = 640;
	const int height = 480;
	
	if (!openDisplay("Timestep", width, height))
        return 1;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 4.0/3.0, 1, 1000);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0,0,150, 0,0,0, 0,1,0);
	
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(4);	
	
	glClearColor(0.3f, 0.3f, 0.3f, 1);

	State current;
	current.x = 100;
	current.v = 0;
	
	State previous = current;

	float t = 0.0f;
	float dt = 0.1f;
	
	float currentTime = 0.0f;
	float accumulator = 0.0f;
	
	while (!quit) 
	{			
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		const float newTime = time();
		float deltaTime = newTime - currentTime;
		currentTime = newTime;
		
		if (deltaTime>0.25f)
			deltaTime = 0.25f;
		
		accumulator += deltaTime;
		
		while (accumulator>=dt)
		{
			accumulator -= dt;
			previous = current;
			integrate(current, t, dt);
			t += dt;
		}
		
		State state = interpolate(previous, current, accumulator/dt);
		
		glBegin(GL_POINTS);
		glColor3f(1,1,1);
		glVertex3f(state.x, 0, 0);
		glEnd();
		
		updateDisplay();
	}
	
	closeDisplay();
	
	return 0;
}

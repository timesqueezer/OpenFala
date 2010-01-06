#include <cstdio>

#include "SFML/Graphics.hpp"
#include "ParticleSystem.h"
 
//Some convenient defines
const float PI = 3.141592f;
 
//Particle systems are built up with emitters. so let's make a basic point emitter
class CMyEmitter : public part::IParticleEmitter
{
public:
	virtual ~CMyEmitter() {}
 
protected:
	virtual void InitParticle(ParticlePtr particle)
	{
		//Calculate world position of the particle
		sf::Vector2f ws_pos = mPos + mPositionOffset;
		//Provide an initial direction
		sf::Vector2f dir(0.f, 1.f);
 
		//Get a random angle between the given extremes
		float angle_variattion = PI/5;
		float angle = sf::Randomizer::Random(-angle_variattion, angle_variattion);
		//Get a direction vector from the angle and initial direction
		float cos_val = cosf(angle);
		float sin_val = sinf(angle);
		dir = sf::Vector2f(dir.x * cos_val - dir.y * sin_val, dir.y * cos_val + dir.x * sin_val);
		//Since we have a normalized vector to begin with we do not need to normalize it after theese calculations.
 
		//Let's give a little variation to our speed value
		float speed_var = 40.f;
		float speed = 150.f;
		speed = sf::Randomizer::Random(speed-speed_var, speed+speed_var);
 
		//Use the provided base function to get a random color
		sf::Color clr = this->RandomiseColor();
 
		//Since we will use a scale in affector we set the starting scale to 0
		float scale = 0.f;
 
		//Finally we initialize our particle :)
		particle->Init(ws_pos, dir, speed, sf::Randomizer::Random(this->mLiveTime-mLiveTimeVariation, this->mLiveTime+mLiveTimeVariation), clr, 0.f, scale);
	}
};
 
//Just an emitter i kind of boring so let's make something that can affect the particles when they are in flight
//A scale in affector is a simple thing to make that makes the system look a lot better.
class CScaleInAffector : public part::IParticleAffector
{
public:
	CScaleInAffector(float fadeTime, float beginScale, float endScale) :
		mFadeTime(fadeTime), mSpeed(0), mMaxScale(endScale)
	{ 
		float scaleDif = mMaxScale - beginScale;
		mSpeed = scaleDif/mFadeTime;
	}
 
	void Update(part::CParticle *particle, float dt)
	{
		//Only scale during the timespan we set in the beginning
		if(particle->GetStartLiveTime()-mFadeTime < particle->GetLiveTime())
		{
			//Scale acording to our speed
			particle->SetScale(particle->GetScale() + mSpeed*dt);
			//Just a safety check so we don't go over the wanted scale.
			if(particle->GetScale() > mMaxScale) 
				particle->SetScale(mMaxScale);
		}
	}
 
private:
	float mFadeTime;
	float mSpeed;
	float mMaxScale;
};
 
 
 
int main(int argc, char *argv[])
{
	//Create the SFML app (this should be no surprise ;))
	sf::RenderWindow rw(sf::VideoMode(800, 600), "SFML particle");
 
	sf::Font arial;
	arial.LoadFromFile("data/arial.ttf");
 
	sf::String text("This is a simple sample showing how you can use sfml-particle", arial, 16);
	text.SetPosition(400 - (text.GetRect().GetWidth() / 2), 10);
 
	sf::String fps("FPS: -1\nNumActive: -1", arial, 14);
	fps.SetPosition(5, 34);
 
	sf::Randomizer::SetSeed((unsigned int)time(NULL));
 
	//Initialize a particle system
	//Load up a particle image
	sf::Image img;
	img.LoadFromFile("data/particle.png");
	//So we whant to have a nice scale in effect on our particle system
	//Lets create a scale in affector
	CScaleInAffector scale_in(0.1f, 0.f, 1.f);
	//Create a instance of the emitter we just made
	CMyEmitter emit;
	emit.SetColorRandom();
	emit.SetMaterial(&img);
	emit.SetEmissionRate(0.02f, 10);
	emit.SetMaxParticles(500);
	emit.SetLiveTime(0.8f, 0.4f);
	emit.SetInitialDimensions(12.f, 12.f);
	emit.SetRotationSpeed(PI*100);
	emit.AddAffector(&scale_in);
	emit.Init();
 
	//Let's copy our newly created emitter
	CMyEmitter emit2(emit);
	emit2.SetOffset(sf::Vector2f(80.f, 0.f));
	//when we copy the new system will share the same particles as the one it copied from, so to give it it's own pool of particles call Init() again
	emit2.Init();
 
	//Finally we create a particle system with our emitters :)
	part::CParticleSystem partSys;
	partSys.AddEmitter(&emit);
	partSys.AddEmitter(&emit2);
	partSys.SetPosition(sf::Vector2f(400, 300));
 
 
 
	// Start the game loop
	while (rw.IsOpened())
	{
		rw.Clear();
		// Process events
		sf::Event Event;
		while (rw.GetEvent(Event))
		{
			// Close window : exit
			if (Event.Type == sf::Event::Closed)
				rw.Close();
			if(Event.Type == sf::Event::KeyPressed)
			{
				switch(Event.Key.Code)
				{
				case sf::Key::Escape:
					rw.Close();
					break;
 
				case sf::Key::Num1:
					partSys.Stop();
					break;
				case sf::Key::Num2:
					partSys.Play();
					break;
				case sf::Key::Num3:
					emit.Stop();
					break;
				case sf::Key::Num4:
					emit.Play();
					break;
				}
			}
		}
 
		float dt = rw.GetFrameTime();
 
		partSys.Rotate(30*dt);
 
		//Print some statistics to the screen
		{
			int fps_num = static_cast<int>(1.f/dt);
 
			char buf[64];
            printf(buf, "FPS: %d\nNumActive: %d", fps_num, partSys.GetNumActiveParticles());
			fps.SetText(buf);
		}
		//To animate the particle system we need to update it.
		partSys.Update(rw.GetFrameTime());
 
		//now lets draw this 
		rw.Draw(partSys);
		rw.Draw(text);
		rw.Draw(fps);
 
		rw.Display();
	}
 
	return 0;
}


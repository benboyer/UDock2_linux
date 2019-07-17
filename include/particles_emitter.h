#ifndef __PARTICLES_EMITTER__
#define __PARTICLES_EMITTER__

#include "render_utils.h"

#define NB_MAX_PARTICLES_PER_EMITTER 300

class Particle
{
	public:
		glm::vec3 pos;
		glm::vec3 direction;
		float speed;
		float size;
		bool alive;
		float lifetime;
		float birthtime;
		glm::vec4 color;

		Particle()
		{
			alive = false;
			size = 1.0f;
			color =glm::vec4(0.0f,0.0f,0.0f,0.0f);
		}
};

class ParticlesEmitter
{
	public:
		// cubeNormalRenderer cuberenderer;
		cubeFlatColorRenderer cuberenderer;
		Particle particles[NB_MAX_PARTICLES_PER_EMITTER];
		bool emit;
		float frequency;
		float lifetime;
		glm::vec3 dir;
		glm::vec3 pos;
		float rndDir;
		float speed;
		float speedLossPerSec;
		// float sizeLossPerSec;
		float startSize;
		float endSize;
		float spreadRadius;
		glm::mat4 view;
		glm::mat4 proj;

		glm::vec4 startColor;
		glm::vec4 endColor;
		bool randomColor;

	private:
		float elapsedEmit;

	public:

		ParticlesEmitter()
		{

			// cuberenderer = cubeNormalRenderer();
			cuberenderer = cubeFlatColorRenderer();
			emit = false;
			
			spreadRadius = 0.25f; 
			lifetime = 0.4f;
			frequency = 55.0f;
			dir = glm::vec3(0.0f,1.0f,0.0f);
			pos = glm::vec3(0.0f,0.0f,0.0f);
			rndDir = 1.0f;
			speed = 0.0f;
			elapsedEmit = 0.0f;
			randomColor = false;
			speedLossPerSec = 0.0f;
			// sizeLossPerSec = 1.0f;
			startSize = 0.15f;
			endSize = 0.05f;
			startColor = glm::vec4 (1.0f,1.0f,1.0f,1.0f);
			endColor = glm::vec4 (1.0f,1.0f,1.0f,1.0f);
		}



		void update(float elapsed)
		{
			if(emit)
			{
				elapsedEmit += elapsed;
				if (elapsedEmit >= 1.0f/frequency)//if it's time to emit a new particle
				{
					elapsedEmit -= 1.0f/frequency;
					
					for(int i=0;i<NB_MAX_PARTICLES_PER_EMITTER;i++)
					{
						if(!particles[i].alive)//the first particle not alive
						{
							particles[i].alive = true;
							particles[i].lifetime = lifetime;
							particles[i].direction = dir;
							particles[i].speed = speed;
							//random spread factor
                			float transX =  ((float)(std::rand())/((float)(RAND_MAX/spreadRadius))) - spreadRadius/2.0f;
                			float transY =  ((float)(std::rand())/((float)(RAND_MAX/spreadRadius))) - spreadRadius/2.0f;
                			float transZ =  ((float)(std::rand())/((float)(RAND_MAX/spreadRadius))) - spreadRadius/2.0f;
							particles[i].pos = pos + glm::vec3(transX,transY,transZ);
							particles[i].size = 1.0f;
							particles[i].birthtime = elapsedEmit;
							break;
						}
					}
				}
			}

			float ratio;
			//On update les particles
			for(int i=0;i<NB_MAX_PARTICLES_PER_EMITTER;i++)
			{
				if(particles[i].alive)
				{
					//if lifetime over then kill the particle
					particles[i].birthtime += elapsed;
					if( particles[i].birthtime  >= particles[i].lifetime){
						particles[i].alive = false;
						continue;
					}

					// particles[i].speed -= speedLossPerSec * elapsed;
					if(particles[i].speed < 0){
						particles[i].speed = 0;
					}
					particles[i].pos += particles[i].direction * particles[i].speed * elapsed;
					ratio = particles[i].birthtime/lifetime;
					particles[i].size = glm::mix(startSize,endSize,ratio);
					particles[i].color = glm::mix(startColor,endColor,ratio);

				}
			}
		}


		void isEmitting(bool isEmitting)
		{
			if(!emit && isEmitting){
				elapsedEmit = 1/frequency;
			}
			emit = isEmitting;

		}

		// void explode(int nbParticules)
		// {
		// 	int nbExplode = 0;
		// 	for(int i=0;i<NB_MAX_PARTICLES_PER_EMITTER;i++)
		// 	{
		// 		if(_Particles[i]._Alive == false)
		// 		{
		// 			_Particles[i]._Alive = true;
		// 			_Particles[i]._Direction = NYVert3Df((float)randf()-0.5f,(float)randf()-0.5f,(float)randf()-0.5f).normalize();
		// 			_Particles[i]._Speed = _Speed*0.7f+_Speed*(float)randf()*0.3f;
		// 			_Particles[i]._Pos = this->_Transform.getWorldPos(NYVert3Df(0,0,0));
		// 			_Particles[i]._Size = _StartSize;
		// 			nbExplode++;
		// 		}

		// 		if(nbExplode >= nbParticules)
		// 			break;
		// 	}
		// }

		void render(void)
		{

			cuberenderer.setView(view);
        	cuberenderer.setProjection(proj);
        	cuberenderer.render();


			// glPushMatrix();
			// glLoadIdentity();
			// NYRenderer::getInstance()->viewFromCam();
			// glDisable(GL_LIGHTING);
			// glEnable(GL_COLOR_MATERIAL);

			// if(!_RandomColor)
			// 	glColor4f(_Color.R,_Color.V,_Color.B,_Color.A);

			for(int i=0;i<NB_MAX_PARTICLES_PER_EMITTER;i++)
			{

				if(particles[i].alive)
				{
					// if(_RandomColor)
					// 	glColor4f(_Particles[i]._Color.R,_Particles[i]._Color.V,_Particles[i]._Color.B,_Particles[i]._Color.A);

					// glPushMatrix();
					// //_cprintf("%f %f %f\n",_Particles[i]._Pos.X,_Particles[i]._Pos.Y,_Particles[i]._Pos.Z);
					// glTranslatef(_Particles[i]._Pos.X,_Particles[i]._Pos.Y,_Particles[i]._Pos.Z);
					// NYRenderer::getInstance()->drawBillBoard(_Particles[i]._Size);
					// glPopMatrix();
					// //_cprintf("render %d %0.2f\n",i,_Particles[i]._Size);

					cuberenderer.setModel(glm::translate (particles[i].pos) * glm::scale(glm::vec3(particles[i].size)));
					cuberenderer.setColor(particles[i].color);
        			cuberenderer.render();
				}

			}
			// glPopMatrix();
		}
};

#endif

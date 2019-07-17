#ifndef __NY_TIMER__
#define __NY_TIMER__

#include "SDL2/SDL_timer.h"

class NYTimer
{
	public :
		Uint64 lastUpdateTime;
		Uint64 freq;

		NYTimer()
		{
			lastUpdateTime = SDL_GetPerformanceCounter();
			freq = SDL_GetPerformanceFrequency();
		}

		void start(void)
		{
			lastUpdateTime = SDL_GetPerformanceCounter();
		}

		float getElapsedSeconds(bool restart = false)
		{
			Uint64 timeNow;
			timeNow = SDL_GetPerformanceCounter();
			Uint64 elapsedLong = timeNow-lastUpdateTime;

			float elapsed = (float) ((float)elapsedLong/(float)freq);

			if (restart)
				lastUpdateTime = timeNow;

			return elapsed;
		}

		unsigned long getElapsedMs(bool restart = false)
		{
			Uint64 timeNow;
			timeNow = SDL_GetPerformanceCounter();
			Uint64 elapsedLong = timeNow-lastUpdateTime;

			unsigned long elapsed = (float) ((float)elapsedLong/(float)freq*1000.0f);

			if (restart)
				lastUpdateTime = timeNow;

			return elapsed;
		}

		unsigned long getElapsedUs(bool restart = false)
		{
			Uint64 timeNow;
			timeNow = SDL_GetPerformanceCounter();
			Uint64 elapsedLong = timeNow-lastUpdateTime;

			Uint64 nbTicksPerUs = freq * 1000000;

			Uint64 elapsed = elapsedLong/nbTicksPerUs;

			if (restart)
				lastUpdateTime = timeNow;

			return elapsed;
		}

		unsigned long getElapsedNs(bool restart = false)
		{
			Uint64 timeNow;
			timeNow = SDL_GetPerformanceCounter();
			Uint64 elapsedLong = timeNow-lastUpdateTime;

			Uint64 nbTicksPerNs = freq * 1000000000;

			Uint64 elapsed = elapsedLong/nbTicksPerNs;

			if (restart)
				lastUpdateTime = timeNow;

			return elapsed;
		}
};

#endif

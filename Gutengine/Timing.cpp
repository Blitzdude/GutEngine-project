#include "Timing.h"

#include <SDL/SDL.h>

namespace Gutengine 
{

FpsLimiter::FpsLimiter() 
{
}

void
FpsLimiter::init(float maxFPS) 
{
    setMaxFPS(maxFPS);
}

void 
FpsLimiter::setMaxFPS(float maxFPS) 
{
    m_maxFPS = maxFPS;
}

void 
FpsLimiter::begin() 
{
    m_startTicks = SDL_GetTicks();
}

float
FpsLimiter::end() 
{
    calculateFPS();

    float frameTicks = (float)(SDL_GetTicks() - m_startTicks);
    //Limit the FPS to the max FPS
    if (1000.0f / m_maxFPS > frameTicks) 
	{
        SDL_Delay((Uint32)(1000.0f / m_maxFPS - frameTicks));
    }

    return m_fps;
}

void
FpsLimiter::calculateFPS() 
{
    //The number of frames to average
    static const int NUM_SAMPLES = 10;
    //Stores all the frametimes for each frame that we will average
    static float frameTimes[NUM_SAMPLES];
    //The current frame we are on
    static int currentFrame = 0;
    //the ticks of the previous frame
    static Uint32 prevTicks = SDL_GetTicks();

    //Ticks for the current frame
    Uint32 currentTicks = SDL_GetTicks();

    //Calculate the number of ticks (ms) for this frame
    m_frameTime = (float)(currentTicks - prevTicks);
    frameTimes[currentFrame % NUM_SAMPLES] = m_frameTime;

    //current ticks is now previous ticks
    prevTicks = currentTicks;

    //The number of frames to average
    int count;

    currentFrame++;
    if (currentFrame < NUM_SAMPLES) 
	{
        count = currentFrame;
    } 
	else 
	{
        count = NUM_SAMPLES;
    }

    //Average all the frame times
    float frameTimeAverage = 0;
    for (int i = 0; i < count; i++) 
	{
        frameTimeAverage += frameTimes[i];
    }
    frameTimeAverage /= count;
	
    //Calculate FPS
    if (frameTimeAverage > 0) 
	{
        m_fps = 1000.0f / frameTimeAverage;
    } 
	else 
	{
        m_fps = 60.0f; // only happens once.
    }
}

} // namespace end
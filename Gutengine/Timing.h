#pragma once

namespace Gutengine 
{

///Calculates FPS and also limits FPS
class FpsLimiter 
{
public:
    FpsLimiter();

    // Initializes the FPS limiter. For now, this is
    // analogous to setMaxFPS
    void init(float maxFPS);

    // Sets the desired max FPS
    void setMaxFPS(float maxFPS);

    void begin();

    // end() will return the current FPS as a float
    float end();
	float getFrameTicks() { return m_frameTicks; };
private:
    // Calculates the current FPS
    void calculateFPS();

    // Variables
    float m_fps;
    float m_maxFPS;
    float m_frameTime;
	float m_frameTicks;
    unsigned int m_startTicks;
};

}
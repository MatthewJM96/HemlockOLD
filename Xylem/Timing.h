#pragma once

namespace Xylem
{
    class FpsLimiter
    {
    public:
        FpsLimiter();

        void init(float maxFps);

        void setMaxFps(float maxFps);

        void begin();

        float end();
    private:
        float _maxFPS;
        float _frameTime;
        float _fps;
        unsigned int _startTicks;

        void calculateFps();
    };
}
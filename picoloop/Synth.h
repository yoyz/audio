#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>
#include <queue>
#include <cmath>

#ifndef __SYNTH____
#define __SYNTH____


const int AMPLITUDE = 28000;
const int FREQUENCY = 44100;

struct BeepObject
{
    double freq;
    int samplesLeft;
};

class Synth
{
private:
    double v;
    std::queue<BeepObject> beeps;
public:
    Synth();
    ~Synth();
    void initialize(double freq, int duration);
    void generateSamples(Uint8 *stream, int length);
    void wait();
};

#endif

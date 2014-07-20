#ifndef __VCO____
#define __VCO____

#include "Oscillator.h"
#include "SineOscillator.h"
#include "SawOscillator.h"
#include "FuzzyPulseOscillator.h"
#include "PulseOscillator.h"
#include "TriangleOscillator.h"
#include "WaveTableSineOscillator.h"
#include "NoiseOscillator.h"

class VCO : public Oscillator
{
public:
  VCO();
  ~VCO();

  int checkSevenBitBoundarie(int val);
  void setSynthFreq(float sfreq);
  void setOscillator(int oscillator_number,int oscillator_type);
  /*
  void setSineOsc();
  void setSawOsc();
  void setFuzzyPulseOsc();
  */

  

  void                  reset();
  Sint16                tick();
  void                  init();
  void                  setVCOMix(int mix);
  void                  setLfoDepth(int val);
  void                  setLfoSpeed(int val);
  Oscillator          * getOscillatorOne();

  SineOscillator          sineOsc1;
  SineOscillator          sineOsc2;

  SawOscillator           sawOsc1;
  SawOscillator           sawOsc2;

  PulseOscillator         pulseOsc1;
  PulseOscillator         pulseOsc2;

  TriangleOscillator      triangleOsc1;
  TriangleOscillator      triangleOsc2;

  WaveTableSineOscillator waveTableSineOsc1;
  WaveTableSineOscillator waveTableSineOsc2;

  NoiseOscillator         noiseOsc1;
  NoiseOscillator         noiseOsc2;

  SineOscillator          sineLfoOsc1;
  SawOscillator           sawLfoOsc1;

  //  FuzzyPulseOscillator    fuzzyPulseOsc1;
  //  FuzzyPulseOscillator    fuzzyPulseOsc2;


  //NoiseOscillator       noiseosc;
  Oscillator            * s1;
  Oscillator            * s2;

  Oscillator            * lfo1;

  float freq;

  int freqOsc1;
  int freqOsc2;


  int current;
  int vcomix;

  int lfo_depth;
  int lfo_depth_shift;

  int lfo_speed;

  int lfo_refresh;
  int lfo_counter;
    
  int tmp_i;
};

#endif



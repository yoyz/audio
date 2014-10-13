using namespace std;



#include <string>
#include <vector>
#include "Master.h"
#ifndef __SONGSEQUENCER__
#define __SONGSEQUENCER__

class SongSequencer
{

public:
  SongSequencer();
  ~SongSequencer();
  int  incStep();
  int  getStep();
  int  setStep(int step);
  int  setLoopPoint(int loopStep);
  int  getPatternNumber(int cp,int trackNumber);
  int  getPatternNumberAtCursorPosition(int trackNumber);
  void setPatternNumber(int cursorPosition,int trackNumber,int patternNumber);
  
private:
  int                  step;
  int                  loopStepA;
  int                  loopStepB;
  vector <vector <int> > songVector;
};

#endif

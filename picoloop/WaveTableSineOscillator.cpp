#include "WaveTableSineOscillator.h"

using namespace std;


WaveTableSineOscillator::WaveTableSineOscillator()
{
  printf("WaveTableSineOscillator::WaveTableSineOscillator()");
  table_size=1024;
  table=NULL;
  index=0;
}

WaveTableSineOscillator::~WaveTableSineOscillator()
{
  printf("WaveTableSineOscillator::~WaveTableSineOscillator()\n");
  if (table!=NULL)
    free(table);
}

void WaveTableSineOscillator::init()
{
  int i;
  float f;
  Sint16 s;
  if (table==NULL)
    {
      table=(Sint16*)malloc(sizeof(Sint16)*table_size);
      //  table=NULL;
      for (i=0;i<table_size;i++)
	{
	  f=sin((2*3.14159*i*1)/table_size)*(1<<13);
	  s=sin((2*3.14159*i*1)/table_size)*(1<<13);
	  table[i]=s;

	  //printf("fvalue = %f ",f);
	  printf("table[%d]=%d\n",i,table[i]);
	}
    }
  
}

Sint16 WaveTableSineOscillator::tick()
{ 
  index=index+(this->getFreq()*1024)/44100;
  if (index>table_size)
    index=index-table_size;
  //printf("freq=%d index=%d table[index]=%d\n",this->getFreq(),index,table[index]);
  return table[index];
}


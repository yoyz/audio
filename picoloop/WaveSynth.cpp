using namespace std;

#include <SDL/SDL.h>
#include <SDL/SDL_audio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "AudioEngine.h"
#include "MonoMixer.h" 
#include "InputManager.h"
#include "SDL_GUI.h"

SDL_GUI        SG;          // used to  initialize video
AudioEngine    AE;          // used to  init alsa/rtaudio
InputManager   IE;          // used to  fetch key

vector <Machine   *>        M(TRACK_MAX);
vector <MonoMixer *>        MM(TRACK_MAX);


#define SCREEN_DEPTH	16
#define NCOEF	(32)

int y_m=SCREEN_HEIGHT/2;
int divide=128;
bool redraw=true;
int loop=0;
int offset=0;
float zoom=1.0;
int quit;

int attack_amp=0;
int release_amp=63;

int attack_fltr=0;
int release_fltr=63;


int vcomix=63;

int cutoff=120;
int resonance=1;

char * filename;

int len=0;
Sint16 * file_buffer=0;
Sint16 * vector_buffer=0;
int filesize_octet=0;

int octave=25;

SDL_Surface *screen=NULL;

bool left_key=false;
bool right_key=false;

int    noteon=0;
int    note=0;

PatternElement PE;

void openaudio()
{
  int t;
  AE.setNbTickBeforeStepChange(1000);
  AE.openAudio();
  for (t=0;t<TRACK_MAX;t++)
    {
      MM[t]=AE.getAudioMixer().getTrack(t).getMonoMixer();
      M[t]=MM[t]->getInput();
      M[t]->init();
      M[t]->reset();
      M[t]->getADSRAmp().init();
      M[t]->getADSRFltr().init();
      M[t]->getVCO().init();
      M[t]->getVCO().setSynthFreq(0);      
    }


  printf("openAudio start streaming\n");
  AE.startAudio();

  int i=0;
  for (i=0;i<10;i++)
    printf("[%d]\n",M[0]->tick());

}




void init_video()
{
  SG.initVideo();
}



void handle_key()
{

  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;
  int t;

  IE.handleKey();

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();

  if (IE.shouldExit())
    quit=1;

  /*
  if(lastKey == SDLK_UP && lastEvent ==  SDL_KEYUP)
    {
    if (divide<=32) divide=32;
    octave=octave+12;
    redraw=true;
    printf("key up octave:%d\n",octave);
    }
  */


  // OCTAVE
  if (keyRepeat[SDLK_w]%64    && 
      keyRepeat[SDLK_DOWN]%64 
      //&& 
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      octave=octave-12;
      if (octave <=1) octave=1;
      redraw=true;
      printf("[w]+[down] => octave:%d\n",octave);
    }


  if (keyRepeat[SDLK_w]%64    && 
      keyRepeat[SDLK_UP]%64   
      //&& 
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      octave=octave+12;
      if (octave >=73) octave=73;
      redraw=true;
      printf("[w]+[up] => octave:%d\n",octave);
    }




  // AMP ENV ATTACK_AMP
  if (keyRepeat[SDLK_x]%4   && 
      keyRepeat[SDLK_DOWN]%4 
      //&& 
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      attack_amp=attack_amp-1;
      if (attack_amp <=1) attack_amp=0;
      redraw=true;
      printf("[x]+[down] => attack_amp:%d\n",attack_amp);
    }


  if (keyRepeat[SDLK_x]%64   && 
      keyRepeat[SDLK_UP]%64   
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      attack_amp=attack_amp+1;
      if (attack_amp >=127) attack_amp=127;
      redraw=true;
      printf("[x]+[up] => attack_amp:%d\n",attack_amp);
    }

  // AMP ENV RELEASE_AMP
  if (keyRepeat[SDLK_x]%64    && 
      keyRepeat[SDLK_RIGHT]%64 
      )
      //&& 
      //lastEvent ==  SDL_KEYDOWN)
    {
      release_amp=release_amp+1;
      if (release_amp >=127) release_amp=127;
      redraw=true;
      printf("[x]+[right] => release_amp:%d\n",release_amp);
    }


  if (keyRepeat[SDLK_x]%64    && 
      keyRepeat[SDLK_LEFT]%64   //&& 
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      release_amp=release_amp-1;
      if (release_amp <=1) release_amp=1;
      redraw=true;
      printf("[x]+[left] => release_amp:%d\n",release_amp);
    }







  // AMP ENV ATTACK_FLTR
  if (keyRepeat[SDLK_b]%4   && 
      keyRepeat[SDLK_DOWN]%4 
      //&& 
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      attack_fltr=attack_fltr-1;
      if (attack_fltr <=1) attack_fltr=0;
      redraw=true;
      printf("[x]+[down] => attack_fltr:%d\n",attack_fltr);
    }


  if (keyRepeat[SDLK_b]%64   && 
      keyRepeat[SDLK_UP]%64   
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      attack_fltr=attack_fltr+1;
      if (attack_fltr >=127) attack_fltr=127;
      redraw=true;
      printf("[x]+[up] => attack_fltr:%d\n",attack_fltr);
    }

  // AMP ENV RELEASE
  if (keyRepeat[SDLK_b]%64    && 
      keyRepeat[SDLK_RIGHT]%64 
      )
      //&& 
      //lastEvent ==  SDL_KEYDOWN)
    {
      release_fltr=release_fltr+1;
      if (release_fltr >=127) release_fltr=127;
      redraw=true;
      printf("[x]+[right] => release_fltr:%d\n",release_fltr);
    }

  
  if (keyRepeat[SDLK_b]%64    && 
      keyRepeat[SDLK_LEFT]%64   //&& 
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      release_fltr=release_fltr-1;
      if (release_fltr <=1) release_fltr=1;
      redraw=true;
      printf("[x]+[left] => release_fltr:%d\n",release_fltr);
    }








  // FILTER Cutoff/Resonance

  if (keyRepeat[SDLK_c]%64    && 
      keyRepeat[SDLK_DOWN]%64 
      //&& 
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      cutoff=cutoff-12;
      if (cutoff <=1) cutoff=1;
      redraw=true;
      printf("[c]+[down] => cutoff:%d\n",cutoff);
    }


  if (keyRepeat[SDLK_c]%64    && 
      keyRepeat[SDLK_UP]%64   
      //&& 
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      cutoff=cutoff+1;
      if (cutoff >=126) cutoff=126;
      redraw=true;
      printf("[c]+[up] => cutoff:%d\n",cutoff);
    }


  if (keyRepeat[SDLK_c]%64    && 
      keyRepeat[SDLK_RIGHT]%64 
      //&& 
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      resonance=resonance+1;
      if (resonance >=127) resonance=127;
      redraw=true;
      printf("[c]+[right] => resonance:%d\n",resonance);
    }


  if (keyRepeat[SDLK_c]%64    && 
      keyRepeat[SDLK_LEFT]%64   
      //&& 
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      resonance=resonance-1;
      if (resonance <=1) resonance=1;
      redraw=true;
      printf("[c]+[left] => resonance:%d\n",resonance);
    }




  // VCO MIX
  if (keyRepeat[SDLK_v]%64    && 
      keyRepeat[SDLK_DOWN]%64 
      //&& 
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      vcomix=vcomix-1;
      if (vcomix <=1) vcomix=1;
      redraw=true;
      printf("[v]+[down] => vcomix:%d\n",vcomix);
    }


  if (keyRepeat[SDLK_v]%64    && 
      keyRepeat[SDLK_UP]%64   
      //&& 
      //lastEvent ==  SDL_KEYDOWN)
      )
    {
      vcomix=vcomix+1;
      if (vcomix >=127) vcomix=127;
      redraw=true;
      printf("[v]+[up] => vcomix:%d\n",vcomix);
    }



  if(lastKey==SDLK_PAGEUP)
    {
      zoom=zoom*1.1;
      if (zoom>1024.0) zoom=1024;
      redraw=true;
      printf("key pgup %f\n",zoom);      
    }

  if(lastKey==SDLK_PAGEDOWN)
    {
      zoom=zoom/1.1;
      if (zoom<0.01) zoom=0.01;
      redraw=true;
      printf("key pgdown %f\n",zoom);
    }

  //  if (lastKey==SDLK_w) { attack--; printf("attack %d\n",attack); }
  //  if (lastKey==SDLK_x) { attack++; printf("attack %d\n",attack); }

  //  if (lastKey==SDLK_c) { release--; printf("release %d\n",release); }
  //  if (lastKey==SDLK_v) { release++; printf("release %d\n",release); }

  //  if (lastKey==SDLK_b) { vcomix--; printf("vco %d\n",vcomix); if (vcomix<=0  ) vcomix=0;   }
  //  if (lastKey==SDLK_n) { vcomix++; printf("vco %d\n",vcomix); if (vcomix>=127) vcomix=127; }


  if (lastEvent ==  SDL_KEYDOWN)
    {
      if (lastKey==SDLK_q) { printf("key q\n"); PE.setNote(0+octave);  note=0+octave;  noteon=1; }
      if (lastKey==SDLK_z) { printf("key z\n"); PE.setNote(1+octave);  note=1+octave;  noteon=1; }
      if (lastKey==SDLK_s) { printf("key s\n"); PE.setNote(2+octave);  note=2+octave;  noteon=1; }
      if (lastKey==SDLK_e) { printf("key e\n"); PE.setNote(3+octave);  note=3+octave;  noteon=1; }
      if (lastKey==SDLK_d) { printf("key d\n"); PE.setNote(4+octave);  note=4+octave;  noteon=1; }
      if (lastKey==SDLK_f) { printf("key f\n"); PE.setNote(5+octave);  note=5+octave;  noteon=1; }
      if (lastKey==SDLK_t) { printf("key t\n"); PE.setNote(6+octave);  note=6+octave;  noteon=1; }
      if (lastKey==SDLK_g) { printf("key g\n"); PE.setNote(7+octave);  note=7+octave;  noteon=1; }
      if (lastKey==SDLK_y) { printf("key y\n"); PE.setNote(8+octave);  note=8+octave;  noteon=1; }
      if (lastKey==SDLK_h) { printf("key h\n"); PE.setNote(9+octave);  note=9+octave;  noteon=1; }
      if (lastKey==SDLK_u) { printf("key u\n"); PE.setNote(10+octave); note=10+octave; noteon=1; }
      if (lastKey==SDLK_j) { printf("key j\n"); PE.setNote(11+octave); note=11+octave; noteon=1; }
      
      if (lastKey==SDLK_k) { printf("key k\n"); PE.setNote(12+octave); note=12+octave; noteon=1; }
      if (lastKey==SDLK_o) { printf("key o\n"); PE.setNote(13+octave); note=13+octave; noteon=1; }
      if (lastKey==SDLK_l) { printf("key l\n"); PE.setNote(14+octave); note=14+octave; noteon=1; }
      if (lastKey==SDLK_p) { printf("key p\n"); PE.setNote(15+octave); note=15+octave; noteon=1; }
      if (lastKey==SDLK_m) { printf("key m\n"); PE.setNote(16+octave); note=16+octave; noteon=1; }
    }

  if (lastEvent ==  SDL_KEYUP)
    {
      if (lastKey==SDLK_q) { printf("key q\n"); PE.setNote(0+octave);  note=0+octave;  noteon=0; }
      if (lastKey==SDLK_z) { printf("key z\n"); PE.setNote(1+octave);  note=1+octave;  noteon=0; }
      if (lastKey==SDLK_s) { printf("key s\n"); PE.setNote(2+octave);  note=2+octave;  noteon=0; }
      if (lastKey==SDLK_e) { printf("key e\n"); PE.setNote(3+octave);  note=3+octave;  noteon=0; }
      if (lastKey==SDLK_d) { printf("key d\n"); PE.setNote(4+octave);  note=4+octave;  noteon=0; }
      if (lastKey==SDLK_f) { printf("key f\n"); PE.setNote(5+octave);  note=5+octave;  noteon=0; }
      if (lastKey==SDLK_t) { printf("key t\n"); PE.setNote(6+octave);  note=6+octave;  noteon=0; }
      if (lastKey==SDLK_g) { printf("key g\n"); PE.setNote(7+octave);  note=7+octave;  noteon=0; }
      if (lastKey==SDLK_y) { printf("key y\n"); PE.setNote(8+octave);  note=8+octave;  noteon=0; }
      if (lastKey==SDLK_h) { printf("key h\n"); PE.setNote(9+octave);  note=9+octave;  noteon=0; }
      if (lastKey==SDLK_u) { printf("key u\n"); PE.setNote(10+octave); note=10+octave; noteon=0; }
      if (lastKey==SDLK_j) { printf("key j\n"); PE.setNote(11+octave); note=11+octave; noteon=0; }
     
      if (lastKey==SDLK_k) { printf("key k\n"); PE.setNote(12+octave); note=12+octave; noteon=0; }
      if (lastKey==SDLK_o) { printf("key o\n"); PE.setNote(13+octave); note=13+octave; noteon=0; }
      if (lastKey==SDLK_l) { printf("key l\n"); PE.setNote(14+octave); note=14+octave; noteon=0; }
      if (lastKey==SDLK_p) { printf("key p\n"); PE.setNote(15+octave); note=15+octave; noteon=0; }
      if (lastKey==SDLK_m) { printf("key m\n"); PE.setNote(16+octave); note=16+octave; noteon=0; }
    }
  IE.clearLastKeyEvent(); 

  //  printf("%%%%%%%%%%%%%%%%%%%%%%%% noteon:%d\n",noteon);
  
  //noteon=0;

  if (noteon &&
      M[t]->getADSRAmp().getNoteOn()==0)
    {

      for (t=0;t<1;t++)
	{
	  float f=PE.getNoteFreq();
	  int   i=f;
	  float   f_c;
	  float   f_r;

	  f_c=cutoff;
	  f_r=resonance;


	  f_c=f_c/256;
	  f_r=f_r/8;
	  
	  printf("[Freq:%d]\n",i);
	  //M[t]->getVCO().getOscillatorOne();
	  //M[t]->getVCO().reset();

	  M[t]->getBiquad().reset();

	  M[t]->getVCO().setSynthFreq(i);
	  M[t]->getVCO().setOscillator(0,1);
	  M[t]->getVCO().setOscillator(1,1);
	  M[t]->getVCO().setVCOMix(vcomix);		  

	  M[t]->getADSRAmp().setRelease(release_amp);
	  M[t]->getADSRAmp().setAttack(attack_amp);

	  M[t]->getADSRFltr().setAttack(attack_fltr);
	  M[t]->getADSRFltr().setRelease(release_fltr);





	  M[t]->getBiquad().setBiquad(0, f_c+0.005, (f_r+0.005), 0.0);
	  M[t]->getBiquad().calcBiquad();

	  M[t]->getADSRAmp().reset();	  
	  M[t]->getADSRFltr().reset();

	  M[t]->getVCO().reset();

	  M[t]->getADSRAmp().setNoteOn();
	  M[t]->getADSRFltr().setNoteOn();

	  //M[t]->getADSR().setRelease(P[t].getPatternElement(step).getRelease());		  
	  //	  M[t]->getADSR().setAttack(P[t].getPatternElement(step).getAttack());		  
	  //M[t]->getVCO().setVCOMix(P[t].getPatternElement(step).getVCOMix());	
	}
    }

  if (noteon==0 
      //&& 
      //M[t]->getADSR().getNoteOn()==1)
      )
    {
      //exit(0);
      for (t=0;t<1;t++)
	{
	  M[t]->getADSRAmp().setNoteOff();
	  //M[t]->getADSRAmp().setADSRRelease();

	  M[t]->getADSRFltr().setNoteOff();
	  //M[t]->getADSRFltr().setADSRRelease();


	  M[t]->reset();
	}
    }

  
  //printf("key pressed sleeping 10ms\n");
  redraw=true;
  SDL_Delay(10);
}

void prepare_vector_buffer()
{
  Sint16 y_value;
  Sint32 j;
  Sint32 last_j;
  Sint32 idx=0;
  Sint64 average=0;
  Sint64 counter=0;

  file_buffer=AE.getBufferOut();

  for (int i = 0; i < SCREEN_WIDTH-1 ; i++)
    {
      last_j=j;
      j=offset+i*zoom;
      //      if (j<=filesize_octet*2)
      if (j<=DEFAULTSAMPLES)
	{
	    y_value=file_buffer[j]/divide;
	    /*
	  else
	    {
	      average=0;
	      counter=0;
	      for (idx=j;idx<offset+(i+1)*zoom;idx++)
		{
		  average=average+file_buffer[idx];
		  counter++;
		  idx++;

		  }
	      printf("%d %d\n",counter,average);
	      vector_buffer[i]=(average/counter)/divide;
	      continue;
	    }
	    */
 	}
      else
	{
	  y_value=0;
	}
      vector_buffer[i]=y_value;
    }
}

void draw_screen()
{
  SDL_Rect rect;
  short color;

  // rect.x rect.y rect.w rect.h
  // int x the x location of the rectangle's upper left corner
  // int y the y location of the rectangle's upper left corner
  // int w the width of the rectangle
  // int h the height of the rectangle

  //Blank screen
  //SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));               
  if (screen==NULL)
    screen=SDL_GetVideoSurface();

  SDL_FillRect(screen,NULL, 0x000000);

  for (int i = 0; i < SCREEN_WIDTH-1 ; i++)
    {
      rect.x = i;
      rect.w = 1;

      if (vector_buffer[i]>0)
	{
	  rect.y=((SCREEN_HEIGHT/2)-vector_buffer[i]);
	  rect.h=vector_buffer[i];
	}
      else
	{
	  rect.y=((SCREEN_HEIGHT/2));
	  rect.h=vector_buffer[i]*-1;	  
	}

      // Create a vertical colored line
      color=vector_buffer[i];
      SDL_FillRect(screen, &rect, SDL_MapRGB(screen->format, color, 0, 128));      
    }
  
  SDL_Flip(screen);
  SDL_Delay(1);
  redraw=false;  
}



int main(int argc,char ** argv)
{	  
  //if (argc!=2) { printf("arg please\n"); exit(1); }
  file_buffer=(Sint16*)malloc(sizeof(short)*DEFAULTSAMPLES);
  vector_buffer=(Sint16*)malloc(sizeof(Sint16)*SCREEN_WIDTH);

  PE.setNote(32);

  struct stat fileStat;
  //  if(stat(argv[1],&fileStat) < 0)    
  //return 1;

  //filename=argv[1];
  //load_raw(argv[1]);

  openaudio();
  init_video();

  
  /* Main loop */
  quit = 0;
  while(!quit)
    {
      prepare_vector_buffer();
      draw_screen();
      handle_key();
    }

  return 0;
}


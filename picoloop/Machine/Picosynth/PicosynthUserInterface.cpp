#include "PicosynthUserInterface.h"




PicosynthUserInterface::PicosynthUserInterface()
{
}

PicosynthUserInterface::~PicosynthUserInterface()
{
}

void display_board_two_param(int machineParam1,int machineParam2);
void sub_handle_invert_trig();
int handle_tweakable_knob_key_two_button(int buttonPressed,int buttonKeyRepeat,int repeatInterval,int machineParam,int paramValue,int all);
void display_board_one_param_text(int machineParam1);
void display_board_two_param_text(int machineParam1,int machineParam2);
void display_board_trig();


void PicosynthUserInterface::handle_key(int menu)
{
  //DPRINTF("PicosynthUserInterface::handle_key(%d,%d)\n",menu);
  if (menu==GLOBALMENU_AD) 
    this->handle_key_amp_env();
  if (menu==GLOBALMENU_NOTE) 
    this->handle_key_note();
  if (menu==GLOBALMENU_OSC) 
    this->handle_key_osc();
  if (menu==GLOBALMENU_VCO) 
    this->handle_key_vco();
  if (menu==GLOBALMENU_LFO) 
    this->handle_key_lfo();
  if (menu==GLOBALMENU_FLTR) 
    this->handle_key_fltr();
}

void PicosynthUserInterface::handle_key_amp_env()
{
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;
  
  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();
  
  
  // GLOBALMENU_AD AMP
  // Move Attack Release 
  // Insert/Remove Trig
  if (menu          == MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad       == MENU_AD_AMP_ATTACK_RELEASE)
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_RELEASE, -1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_RELEASE,  1, 0);

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_ATTACK,   1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_ATTACK,  -1, 0);

    }  


  // M_DS AMP
  // Move Decay Sustain
  // Insert/Remove Trig
  if (menu          == MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_AMP_DECAY_SUSTAIN)
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_DECAY,    -1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_DECAY,     1, 0);

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_SUSTAIN,   1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_SUSTAIN,  -1, 0);
    }  


  // GLOBALMENU_AD AMP
  // Move Attack Release 
  // Insert/Remove Trig
  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_AMP_ATTACK_RELEASE)
    {
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_RELEASE, -1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_RELEASE,  1, 1);

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_ATTACK,   1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_ATTACK,  -1, 1);
    }  

  // GLOBALMENU_AD AMP
  // Move Decay Sustain
  // Insert/Remove Trig
  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_AMP_DECAY_SUSTAIN)
    {
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_DECAY,    -1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_DECAY,     1, 1);

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_SUSTAIN,   1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, ADSR_AMP_SUSTAIN,  -1, 1);
    }  



  if (menu          == MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_TRIGTIME_AMP)
    {
      if (lastKey   == BUTTON_A && 
	  lastEvent == KEYPRESSED)
	{
	  sub_handle_invert_trig();
	  // TK.invert_trig=1;
	  // DPRINTF("key lalt\n");      
	  // dirty_graphic=1;
	  // IE.clearLastKeyEvent();
	}
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, AMP            ,     -1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, AMP            ,      1, 0);

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, TRIG_TIME_DURATION,   1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, TRIG_TIME_DURATION,  -1, 0);

    }  


  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_AD     &&
      menu_ad      == MENU_AD_TRIGTIME_AMP)
    {
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, AMP            ,     -1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, AMP            ,      1, 1);
      
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, TRIG_TIME_DURATION,   1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, TRIG_TIME_DURATION,  -1, 1);
    }  



  // change GLOBALMENU_AD SUBMENU
  if (keyState[BUTTON_START]       &&
      keyRepeat[BUTTON_UP]%KEY_REPEAT_INTERVAL_LONGEST==127   &&
      menu_cursor ==  GLOBALMENU_AD)
    {
      menu_ad--;
      if (menu_ad<=-1)
	menu_ad=MENU_PAGE0_SUB2;
      dirty_graphic=1;
      IE.clearLastKeyEvent();
      menu_ad_dirty_keyboard=1;
      DPRINTF("[sub menu env : %d]\n",menu_ad);
    }

  // change GLOBALMENU_AD SUBMENU
  if (keyState[BUTTON_START] &&
      keyRepeat[BUTTON_DOWN]%KEY_REPEAT_INTERVAL_LONGEST==127  &&
      menu_cursor ==  GLOBALMENU_AD)
    {
      menu_ad++;
      if (menu_ad>=MENU_PAGE0_SUB2)
	menu_ad=0;
      dirty_graphic=1;
      IE.clearLastKeyEvent();
      menu_ad_dirty_keyboard=1;
      DPRINTF("[sub menu env : %d]\n",menu_ad);
    }

  // change GLOBALMENU_AD SUBMENU
  if (lastKey     ==  BUTTON_START  && 
      lastEvent   ==  KEYRELEASED     && 
      menu_cursor ==  GLOBALMENU_AD)
    {
      if (menu_ad_dirty_keyboard==0)
	{
	  if      (menu_ad==MENU_AD_AMP_ATTACK_RELEASE)       { menu_ad=MENU_AD_AMP_DECAY_SUSTAIN;        }
	  else if (menu_ad==MENU_AD_AMP_DECAY_SUSTAIN)        { menu_ad=MENU_AD_TRIGTIME_AMP;             }   
	  else if (menu_ad==MENU_AD_TRIGTIME_AMP)             { menu_ad=MENU_AD_AMP_ATTACK_RELEASE;       }   
	  else                                                { menu_ad=MENU_AD_AMP_ATTACK_RELEASE;       }
	  dirty_graphic=1;
	}
      menu_ad_dirty_keyboard=0;
      IE.clearLastKeyEvent();
      DPRINTF("[sub menu env : %d]\n",menu_ad);
    }




}

void PicosynthUserInterface::handle_key_note()
{

  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();

    // GLOBALMENU_NOTE
  // change note
  // copy/paste
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_NOTE)
    {
      // copy/paste/insert/delete trig 
      sub_handle_invert_trig();

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , NOTE1,             -1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , NOTE1,              1, 0);

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , NOTE1           ,  12, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , NOTE1           , -12, 0);

    }  

    // GLOBALMENU_NOTE
  // change note
  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_NOTE)
    {

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , NOTE1,             -1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , NOTE1,              1, 1);

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , NOTE1           ,  12, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , NOTE1           , -12, 1);

    }  


  // change note from box to value e.g C3 D4...
  if (lastKey     ==  BUTTON_START  && 
      lastEvent   ==  KEYRELEASED     && 
      menu_cursor ==  GLOBALMENU_NOTE)
    {
      if      (menu_note==0)        { menu_note=1;  }
      else if (menu_note==1)        { menu_note=0;  }   
      dirty_graphic=1;
      IE.clearLastKeyEvent();
      DPRINTF("[sub menu note : %d]\n",menu_note);
    }


}

void PicosynthUserInterface::handle_key_osc()
{
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();

  // GLOBALMENU_OSC
  // change oscilltor one and two type
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_OSC )
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , OSC1_TYPE,        -1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , OSC1_TYPE,         1, 0);

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , OSC2_TYPE      ,   1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , OSC2_TYPE      ,  -1, 0);
    }

  // GLOBALMENU_OSC
  // change oscilltor one and two type
  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_OSC )
    {

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , OSC1_TYPE,        -1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , OSC1_TYPE,         1, 1);

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , OSC2_TYPE      ,   1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , OSC2_TYPE      ,  -1, 1);

    }
  // change GLOBALMENU_OSC SUBMENU
  if (lastKey     ==  BUTTON_START  && 
      lastEvent   ==  KEYRELEASED     && 
      menu_cursor ==  GLOBALMENU_OSC)
    {
      if (menu_ad_dirty_keyboard==0)
	{
	  // if      (menu_osc==MENU_OSC_OSC1OSC2)            { menu_osc=MENU_OSC_LFO1LFO2;            }
	  // else if (menu_osc==MENU_OSC_LFO1LFO2)            { menu_osc=MENU_OSC_OSC1OSC2;            }
	  // else                                             { menu_osc=MENU_OSC_OSC1OSC2;            }
	  menu_osc=MENU_OSC_OSC1OSC2;
	  dirty_graphic=1;
	}
      menu_ad_dirty_keyboard=0;
      IE.clearLastKeyEvent();
      DPRINTF("[sub menu env : %d]\n",menu_ad);
    }
}


void PicosynthUserInterface::handle_key_vco()
{
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();




  // GLOBALMENU_VCO
  // VCO Menu
  // Change Value
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO    &&
      menu_vco    == MENU_VCO_OSCMIX_PHASE
      )
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, VCO_MIX        ,     -1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, VCO_MIX        ,      1, 0);

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, OSC1_DETUNE        ,   1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, OSC1_DETUNE        ,  -1, 0);
    }

  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_VCO   &&
      menu_vco    == MENU_VCO_OSCMIX_PHASE
      )
    {
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, VCO_MIX        ,     -1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, VCO_MIX        ,      1, 1);

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, OSC1_DETUNE     ,      1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, OSC1_DETUNE     ,     -1, 1);
    }





  // change GLOBALMENU_VCO SUBMENU
  if (lastKey     ==  BUTTON_START  && 
      lastEvent   ==  KEYRELEASED     && 
      menu_cursor ==  GLOBALMENU_VCO)
    {
      if (menu_ad_dirty_keyboard==0)
	{
	  if      (menu_vco==MENU_VCO_OSCMIX_PHASE)              { menu_vco=MENU_VCO_OSCMIX_PHASE;    }
	  else                                                   { menu_vco=MENU_VCO_OSCMIX_PHASE;    }
	  dirty_graphic=1;
	}
      menu_ad_dirty_keyboard=0;
      IE.clearLastKeyEvent();
      DPRINTF("[sub menu env : %d]\n",menu_ad);
    }



}


void PicosynthUserInterface::handle_key_lfo()
{
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();

  // GLOBALMENU_LFO
  // LFO Menu
  // Change Value
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_LFOPITCH
      )
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, LFO1_DEPTH     ,     -1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, LFO1_DEPTH     ,      1, 0);

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, LFO1_FREQ      ,      1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, LFO1_FREQ      ,     -1, 0);
    }


  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_LFOPITCH
      )
    {

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, LFO1_DEPTH     ,     -1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, LFO1_DEPTH     ,      1, 1);

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, LFO1_FREQ      ,      1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, LFO1_FREQ      ,     -1, 1);
    }

  // GLOBALMENU_LFO
  // LFO Menu
  // Change Value
  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_PITCHBEND
      )
    {
      // Insert/Remove Trig
      sub_handle_invert_trig();

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, PITCHBEND_DEPTH,     -1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, PITCHBEND_DEPTH,      1, 0);

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, PITCHBEND_SPEED,      1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, PITCHBEND_SPEED,     -1, 0);
    }


  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_PITCHBEND
      )
    {
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, PITCHBEND_DEPTH,     -1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, PITCHBEND_DEPTH,      1, 1);

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, PITCHBEND_SPEED,      1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, PITCHBEND_SPEED,     -1, 1);
    }


  if (menu        == MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_TYPE
      )
    {
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG,     LFO_TYPE      ,      1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG,     LFO_TYPE      ,     -1, 0);
    }



  if (menu        != MENU_OFF && 
      menu_cursor == GLOBALMENU_LFO &&
      menu_lfo    == MENU_LFO_TYPE
      )
    {
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG,     LFO_TYPE      ,      1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG,     LFO_TYPE      ,     -1, 1);
    }



  // change GLOBALMENU_VCO SUBMENU
  if (lastKey     ==  BUTTON_START  && 
      lastEvent   ==  KEYRELEASED     && 
      menu_cursor ==  GLOBALMENU_LFO)
    {
      if (menu_ad_dirty_keyboard==0)
	{
	  if      (menu_lfo==MENU_LFO_LFOPITCH)               { menu_lfo=MENU_LFO_PITCHBEND;       }
	  else if (menu_lfo==MENU_LFO_PITCHBEND)              { menu_lfo=MENU_LFO_TYPE;            } 
	  else if (menu_lfo==MENU_LFO_TYPE)                   { menu_lfo=MENU_LFO_LFOPITCH;        }  
	  else                                                { menu_lfo=MENU_LFO_LFOPITCH;        }
	  dirty_graphic=1;
	}
      menu_ad_dirty_keyboard=0;
      IE.clearLastKeyEvent();
      DPRINTF("[sub menu lfo : %d]\n",menu_lfo);
    }



}



void PicosynthUserInterface::handle_key_fltr()
{
  bool * keyState;
  int  * keyRepeat;
  int    lastEvent;
  int    lastKey;

  keyState=IE.keyState();
  keyRepeat=IE.keyRepeat();
  lastEvent=IE.lastEvent();
  lastKey=IE.lastKey();


  // GLOBALMENU_FLTR
  // Move Cutoff Resonance
  // Insert/Remove Trig
  if (menu          == MENU_OFF && 
      menu_cursor   == GLOBALMENU_FLTR   &&
      menu_fltr     == MENU_FLTR_CUTOFF_RESONANCE)
    {
      sub_handle_invert_trig();

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, FILTER1_RESONANCE,    -1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, FILTER1_RESONANCE,     1, 0);

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, FILTER1_CUTOFF,        1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, FILTER1_CUTOFF,       -1, 0);
    }  

  // GLOBALMENU_FLTR
  // Move Cutoff Resonance
  // Insert/Remove Trig
  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_FLTR   &&
      menu_fltr     == MENU_FLTR_CUTOFF_RESONANCE)
    {
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_SMALLEST, FILTER1_RESONANCE,    -1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_SMALLEST, FILTER1_RESONANCE,     1, 1);

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_SMALLEST, FILTER1_CUTOFF,        1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_SMALLEST, FILTER1_CUTOFF,       -1, 1);
    }  

  // GLOBALMENU_FLTR
  // Move filterAlgo filterType
  // Insert/Remove Trig
  if (menu          == MENU_OFF && 
      menu_cursor   == GLOBALMENU_FLTR   &&
      menu_fltr     == MENU_FLTR_ALGO_TYPE)
    {
      sub_handle_invert_trig();

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , FILTER1_ALGO,             -1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , FILTER1_ALGO,              1, 0);

      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , FILTER1_TYPE           ,   1, 0);
      handle_tweakable_knob_key_two_button( BUTTON_B, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , FILTER1_TYPE           ,  -1, 0);
    }  


  // GLOBALMENU_FLTR
  // Move filterAlgo filterType
  // Insert/Remove Trig
  if (menu          != MENU_OFF && 
      menu_cursor   == GLOBALMENU_FLTR   &&
      menu_fltr     == MENU_FLTR_ALGO_TYPE)
    {
      //sub_handle_invert_trig();

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_LEFT,    KEY_REPEAT_INTERVAL_LONG    , FILTER1_ALGO,             -1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_RIGHT,   KEY_REPEAT_INTERVAL_LONG    , FILTER1_ALGO,              1, 1);

      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_UP,      KEY_REPEAT_INTERVAL_LONG    , FILTER1_TYPE           ,   1, 1);
      handle_tweakable_knob_key_two_button( BUTTON_A, BUTTON_DOWN,    KEY_REPEAT_INTERVAL_LONG    , FILTER1_TYPE           ,  -1, 1);
    }  


  // change GLOBALMENU_FLTR SUBMENU
  if (lastKey     ==  BUTTON_START  && 
      lastEvent   ==  KEYRELEASED     && 
      menu_cursor ==  GLOBALMENU_FLTR)
    {
      if (menu_ad_dirty_keyboard==0)
	{
	  if      (menu_fltr==MENU_FLTR_CUTOFF_RESONANCE)   { menu_fltr=MENU_FLTR_ALGO_TYPE;            }
	  else if (menu_fltr==MENU_FLTR_ALGO_TYPE)          { menu_fltr=MENU_FLTR_CUTOFF_RESONANCE;     }   
	  else                                              { menu_fltr=MENU_FLTR_CUTOFF_RESONANCE;     }
	  dirty_graphic=1;
	}
      menu_ad_dirty_keyboard=0;
      IE.clearLastKeyEvent();
      DPRINTF("[sub menu_fltr : %d]\n",menu_fltr);
    }


}




void PicosynthUserInterface::display_board_text()
{
  int  i=0;
  char str_line1[64]="";
  char str_line2[64]="";
  char str_line3[64]="";
  char str_line4[64]="";
  char str_line5[64]="";
  char str_menu[64]="";

  int  right_x_display_offset=      200*SCREEN_MULT;
  int  right_y_display_offset_line1=20*SCREEN_MULT;
  int  right_y_display_offset_line2=40*SCREEN_MULT;
  int  right_y_display_offset_line3=60*SCREEN_MULT;
  int  right_y_display_offset_line4=80*SCREEN_MULT;

  int  menu_x_display_offset=       10*SCREEN_MULT;
  int  menu_y_display_offset=      200*SCREEN_MULT;

  int  cty=SEQ.getCurrentTrackY();
  int  stepdiv=SEQ.getPatternSequencer(cty).getBPMDivider();
  
  if (menu_ad==MENU_AD_AMP_ATTACK_RELEASE   && menu_cursor==GLOBALMENU_AD)
    sprintf(str_line2,"AMP  A/R");

  if (menu_ad==MENU_AD_AMP_DECAY_SUSTAIN    && menu_cursor==GLOBALMENU_AD)
    sprintf(str_line2,"AMP  S/D");

  if (menu_ad==MENU_AD_TRIGTIME_AMP         && menu_cursor ==GLOBALMENU_AD)
    sprintf(str_line2,"T/N AMP");

  if (menu_fltr==MENU_FLTR_CUTOFF_RESONANCE && menu_cursor == GLOBALMENU_FLTR)
    sprintf(str_line2,"CUTOFF/RES");
    
  if (menu_fltr==MENU_FLTR_ALGO_TYPE        && menu_cursor == GLOBALMENU_FLTR)
    sprintf(str_line2,"ALGO/TYPE");

  if (menu_vco==MENU_VCO_OSCMIX_PHASE       && menu_cursor == GLOBALMENU_VCO)
    sprintf(str_line2,"DETUNE/VCOMIX");

  if (menu_lfo==MENU_LFO_LFOPITCH           && menu_cursor == GLOBALMENU_LFO)
    sprintf(str_line2,"LFOPitch Depth/Speed");

  if (menu_lfo==MENU_LFO_PITCHBEND          && menu_cursor == GLOBALMENU_LFO)
    sprintf(str_line2,"PitchBend Depth/Speed");

  if (menu_lfo==MENU_LFO_TYPE               && menu_cursor == GLOBALMENU_LFO)
    sprintf(str_line2,"LFOType");

  if (menu_osc    == MENU_OSC_OSC1OSC2      && menu_cursor == GLOBALMENU_OSC)
    sprintf(str_line2,"OSC WAVEFORM");

  if (menu_osc    == MENU_OSC_LFO1LFO2      && menu_cursor == GLOBALMENU_OSC)
    sprintf(str_line2,"LFO WAVEFORM  ");
  


  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_AD)    sprintf(str_menu,"[ENV] Note  OSC   VCO   LFO   FLTR   ");
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_NOTE)  sprintf(str_menu," ENV [Note] OSC   VCO   LFO   FLTR   ");
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_OSC)   sprintf(str_menu," ENV  Note [OSC]  VCO   LFO   FLTR   ");
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_VCO)   sprintf(str_menu," ENV  Note  OSC  [VCO]  LFO   FLTR   ");
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_LFO)   sprintf(str_menu," ENV  Note  OSC   VCO  [LFO]  FLTR   ");
  if (menu==MENU_ON_PAGE1 && menu_cursor==GLOBALMENU_FLTR)  sprintf(str_menu," ENV  Note  OSC   VCO   LFO  [FLTR]  ");


  if (menu==0)                                              sprintf(str_menu,"                                     ");

  if (menu_cursor==GLOBALMENU_AD)               sprintf(str_line4,"ENV     ");
  if (menu_cursor==GLOBALMENU_NOTE)             sprintf(str_line4,"Note    ");
  if (menu_cursor==GLOBALMENU_OSC)              sprintf(str_line4,"OSC     ");
  if (menu_cursor==GLOBALMENU_VCO)              sprintf(str_line4,"VCO     ");
  if (menu_cursor==GLOBALMENU_LFO)              sprintf(str_line4,"LFO     ");
  if (menu_cursor==GLOBALMENU_FLTR)             sprintf(str_line4,"FLTR    ");


  SG.guiTTFText(right_x_display_offset,
		right_y_display_offset_line2,str_line2);


  if (menu==MENU_ON_PAGE1)
    SG.guiTTFText(right_x_display_offset,
		  right_y_display_offset_line4,str_line4);

  if (menu==MENU_ON_PAGE1)
    SG.guiTTFText(menu_x_display_offset,
		  menu_y_display_offset,str_menu);


}





void PicosynthUserInterface::display_board(int menu)
{
  DPRINTF("PicosynthUserInterface::display_board(%d,)\n",menu);
  if (menu==GLOBALMENU_AD) 
    this->display_board_amp_env();
  if (menu==GLOBALMENU_NOTE) 
    this->display_board_note();
  if (menu==GLOBALMENU_VCO) 
    this->display_board_vco();
  if (menu==GLOBALMENU_OSC) 
    this->display_board_osc();
  if (menu==GLOBALMENU_LFO) 
    this->display_board_lfo();
  if (menu==GLOBALMENU_FLTR) 
    this->display_board_fltr();

}


void PicosynthUserInterface::display_board_amp_env()
{
  DPRINTF("PicosynthUserInterface::display_board_amp_env()\n");
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();
  // Attack/Release
  if (menu_cursor==GLOBALMENU_AD)
    {
      // Fix to set the "menu_ad" on a valid value for picosynth
      if (menu_ad!=MENU_AD_AMP_ATTACK_RELEASE &&
	  menu_ad!=MENU_AD_AMP_DECAY_SUSTAIN  &&
	  menu_ad!=MENU_AD_TRIGTIME_AMP)
	menu_ad=MENU_AD_AMP_ATTACK_RELEASE;


      if (menu_ad==MENU_AD_AMP_ATTACK_RELEASE)
	display_board_two_param(ADSR_AMP_RELEASE,ADSR_AMP_ATTACK);

      if (menu_ad==MENU_AD_AMP_DECAY_SUSTAIN)
	display_board_two_param(ADSR_AMP_DECAY,ADSR_AMP_SUSTAIN);

      if (menu_ad==MENU_AD_TRIGTIME_AMP)
	display_board_two_param(AMP,TRIG_TIME_DURATION);	
    }  
}




void PicosynthUserInterface::display_board_note()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  NoteFreq & NF = NoteFreq::getInstance();
  // Note
  if (menu_cursor==GLOBALMENU_NOTE)
    {
      display_board_trig();
      // Note C3 
      if (menu_note==ENABLE)
	{	  
	  for (i=0;i<16;i++)
	    {
	      if (P[cty].getPatternElement(i).get(NOTE_ON))
		SG.drawTTFTextNumberFirstLine(i,NF.getNoteCharStar(P[cty].getPatternElement(i).get(NOTE1)));
	    }
	}
      // Note Cursor
      if (menu_note==DISABLE)      
	{
	  for (i=0;i<16;i++)
	    {
	      if (P[cty].getPatternElement(i).get(NOTE_ON))
		SG.smallBoxNumber(i,
				  (P[cty].getPatternElement(i).get(NOTE1)%12-1)*10,
				  (128-(P[cty].getPatternElement(i).get(NOTE1)/12)*10),
				  SMALLBOX_COLOR);
	    }
	}
    }
  
}


void PicosynthUserInterface::display_board_vco()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  if (menu_cursor == GLOBALMENU_VCO)
    menu_vco = MENU_VCO_OSCMIX_PHASE; // enforce this menu_vco to avoid blank screen

  // VCO
  if (menu_cursor == GLOBALMENU_VCO  && 
      menu_vco    == MENU_VCO_OSCMIX_PHASE)
    {
      display_board_two_param(VCO_MIX,OSC1_DETUNE);
    }

}

void PicosynthUserInterface::display_board_osc()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  if (menu_cursor==GLOBALMENU_OSC)
    {
      display_board_two_param_text(OSC1_TYPE,OSC2_TYPE);
    }
}


void PicosynthUserInterface::display_board_lfo()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  // LFOPITCH

  if (menu_cursor==GLOBALMENU_LFO &&
      menu_lfo   ==MENU_LFO_LFOPITCH)
    {

      display_board_two_param(LFO1_DEPTH,LFO1_FREQ);
    }

  // PITCHBEND

  if (menu_cursor==GLOBALMENU_LFO &&
      menu_lfo   ==MENU_LFO_PITCHBEND)
    {
      display_board_two_param(PITCHBEND_DEPTH,PITCHBEND_SPEED);
    }

  if (menu_lfo==MENU_LFO_TYPE)
    {
      display_board_one_param_text(LFO_TYPE);
    }
}


void PicosynthUserInterface::display_board_fltr()
{
  int  i;
  int  cty=SEQ.getCurrentTrackY();
  int  step=SEQ.getPatternSequencer(cty).getStep();

  if (menu_cursor==GLOBALMENU_FLTR)
    {

      if (menu_fltr==MENU_FLTR_CUTOFF_RESONANCE)
	{
	  display_board_two_param(FILTER1_RESONANCE,FILTER1_CUTOFF);
	}
      if (menu_fltr==MENU_FLTR_ALGO_TYPE)
	{
	  display_board_two_param_text(FILTER1_ALGO,FILTER1_TYPE);
	}
    }
}


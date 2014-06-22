#include "SDL_GUI.h"

SDL_GUI::SDL_GUI()
{
  printf("construction SDL_GUI::SDL_GUI()\n");
  screen=NULL;
  bmp_font=NULL;
  ttf_font=NULL;
  boxSize=31;
  boxOffset=20;

  message=NULL;
}

SDL_GUI::~SDL_GUI()
{
  printf("SDL_GUI::~SDL_GUI()\n");
  
  if (ttf_font!=NULL)
    TTF_CloseFont(ttf_font);
  if (message!=NULL)
    SDL_FreeSurface( message );
  //Quit SDL_ttf
  TTF_Quit();
}



int SDL_GUI::initVideo()
{
  if ( SDL_Init(SDL_INIT_VIDEO) < 0 )
    {
      fprintf(stderr, "Couldn't initialize SDL: %s\n", SDL_GetError());
      return 0;
    }
  
  printf("SDL initialized.\n");
  #ifdef OPENDINGUX
  screen = SDL_SetVideoMode(SCREEN_WIDTH, 
			    SCREEN_HEIGHT, 
			    SCREEN_DEPTH,			
			    SDL_HWSURFACE|SDL_DOUBLEBUF|SDL_FULLSCREEN);
  #endif
  #ifdef LINUX_DESKTOP
  screen = SDL_SetVideoMode(SCREEN_WIDTH, 
			    SCREEN_HEIGHT, 
			    SCREEN_DEPTH,			
			    SDL_HWSURFACE|SDL_DOUBLEBUF);
  #endif
  if (screen == NULL)
    {
      fprintf(stderr, "Couldn't set video mode: %s\n", SDL_GetError());
      SDL_Quit();
      return 0;
    }  
  return 1;
}

void SDL_GUI::refresh()
{
  SDL_Flip(screen);
}


void SDL_GUI::clearScreen()
{
  SDL_FillRect(screen,NULL, 0x000000);
}


int SDL_GUI::closeVideo()
{
  SDL_Quit();
}




void SDL_GUI::middleBox(int x,int y,Uint32 c)
{
  this->fullBox(x,y,boxSize/3,boxSize/3,c);
}

void SDL_GUI::middleBoxNumber(int x,int y,Uint32 c)
{
  this->middleBox(35+15*x,60+15*y,c);
}


void SDL_GUI::loadSaveBoxNumber(int x,int y,Uint32 c)
{
  this->middleBoxNumber(x,y,c);
}

void SDL_GUI::drawTTFTextLoadSaveBoxNumer(int x,int y,const char *txt)
{
  this->guiTTFText(35+15*x,60+15*y,txt);
} 


void SDL_GUI::smallBox(int x,int y,Uint32 c)
{
  this->fullBox(x,y,boxSize/4,boxSize/4,c);
}



void SDL_GUI::emptyBox(int x, int y, int w, int h, Uint32 c)
{
  SDL_Rect r;

  r.x = x;
  r.y = y;
  r.w = w;
  r.h = 1;
  SDL_FillRect(screen, &r, c);
  
  r.x = x;
  r.y = y + h - 1;
  r.w = w;
  r.h = 1;
  SDL_FillRect(screen, &r, c);
  
  r.x = x;
  r.y = y + 1;
  r.w = 1;
  r.h = h - 2;
  SDL_FillRect(screen, &r, c);
	
  r.x = x + w - 1;
  r.y = y + 1;
  r.w = 1;
  r.h = h - 2;
  SDL_FillRect(screen, &r, c);
}

void SDL_GUI::fullBox(int x, int y, int w, int h, Uint32 c)
{
  SDL_Rect r;
  /*
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = 1;
    SDL_FillRect(screen, &r, c);
    
    r.x = x;
    r.y = y + h - 1;
    r.w = w;
    r.h = 1;
    SDL_FillRect(screen, &r, c);
    
    r.x = x;
    r.y = y + 1;
    r.w = 1;
    r.h = h - 2;
    SDL_FillRect(screen, &r, c);
    
    r.x = x + w - 1;
    r.y = y + 1;
    r.w = 1;
    r.h = h - 2;
    SDL_FillRect(screen, &r, c);
  */
  r.x = x;
  r.y = y;
  r.w = w;
  r.h = h;
  SDL_FillRect(screen, &r, c);  
}




/*
void SDL_GUI::drawText(int x, int y, string txt)
{
  printf("SDL_GUI::drawText\n");
}
*/

/*
void SDL_GUI::drawBox(int x, int y, int color)
{
    printf("SDL_GUI::drawBox\n");
}
*/


int SDL_GUI::openTTFFont()
{
  if( TTF_Init() == -1 )
    {
      return 0;
    }

  //Open the font
  ttf_font = TTF_OpenFont("font.ttf", 8 );
  
  //If there was an error in loading the font
  //  printf("%d\n",ttf_font);
  // exit(0);
  if( ttf_font == NULL || ttf_font==0 )
    {      
      printf("font not found or is not a font file ttf_font:%d\n",ttf_font);
      return false;      
    }
  return true;  
}


void SDL_GUI::apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip
)
{
    //Holds offsets
    SDL_Rect offset;

    //Get offsets
    offset.x = x;
    offset.y = y;

    //Blit
    SDL_BlitSurface( source, clip, destination, &offset );
}


int SDL_GUI::guiTTFText(int x,int y,const char *txt)
{
  SDL_Color textColor = { 54, 25, 255 };
  SDL_Rect * clip = NULL;

  if (message!=NULL)
    SDL_FreeSurface(message);
  
  message = TTF_RenderText_Solid( ttf_font, txt, textColor );
  //apply_surface( 0, 0, background, screen );
  this->apply_surface( x, y, message, screen,clip);
}



SDL_Surface * SDL_GUI::loadBMP(const char *fn)
{
  SDL_Surface *cvt;
  SDL_Surface *img = SDL_LoadBMP(fn);
  if(!img)
    return NULL;
  cvt = SDL_DisplayFormat(img);
  SDL_FreeSurface(img);
  return cvt;
}


void SDL_GUI::smallBoxNumber(int n,int x,int y,Uint32 c)
{
  switch(n)
    {
      //case 0: this->box(boxOffset+(0*boxSize)+((x*((boxSize*100)/(x+1)))/100),boxOffset,boxSize/4,boxSize/4,c); break;
      //case 0: this->box(boxOffset+(0*boxSize)+((4*x)/boxSize),boxOffset,boxSize/4,boxSize/4,c); break;
    case 0:  this->fullBox(boxOffset+(0*boxSize)+0+ ((6*x)/boxSize),boxOffset+(0*boxSize+0)+((6*y)/boxSize),boxSize/4,boxSize/4,c); break;
    case 1:  this->fullBox(boxOffset+(1*boxSize)+10+((6*x)/boxSize),boxOffset+(0*boxSize+0)+((6*y)/boxSize),boxSize/4,boxSize/4,c); break;
    case 2:  this->fullBox(boxOffset+(2*boxSize)+20+((6*x)/boxSize),boxOffset+(0*boxSize+0)+((6*y)/boxSize),boxSize/4,boxSize/4,c); break;
    case 3:  this->fullBox(boxOffset+(3*boxSize)+30+((6*x)/boxSize),boxOffset+(0*boxSize+0)+((6*y)/boxSize),boxSize/4,boxSize/4,c); break;

    case 4:  this->fullBox(boxOffset+(0*boxSize)+0+ ((6*x)/boxSize),boxOffset+(1*boxSize+10)+((6*y)/boxSize),boxSize/4,boxSize/4,c); break;
    case 5:  this->fullBox(boxOffset+(1*boxSize)+10+((6*x)/boxSize),boxOffset+(1*boxSize+10)+((6*y)/boxSize),boxSize/4,boxSize/4,c); break;
    case 6:  this->fullBox(boxOffset+(2*boxSize)+20+((6*x)/boxSize),boxOffset+(1*boxSize+10)+((6*y)/boxSize),boxSize/4,boxSize/4,c); break;
    case 7:  this->fullBox(boxOffset+(3*boxSize)+30+((6*x)/boxSize),boxOffset+(1*boxSize+10)+((6*y)/boxSize),boxSize/4,boxSize/4,c); break;

    case 8:  this->fullBox(boxOffset+(0*boxSize)+0+ ((6*x)/boxSize),boxOffset+(2*boxSize+20)+((6*y)/boxSize),boxSize/4,boxSize/4,c); break;
    case 9:  this->fullBox(boxOffset+(1*boxSize)+10+((6*x)/boxSize),boxOffset+(2*boxSize+20)+((6*y)/boxSize),boxSize/4,boxSize/4,c); break;
    case 10: this->fullBox(boxOffset+(2*boxSize)+20+((6*x)/boxSize),boxOffset+(2*boxSize+20)+((6*y)/boxSize),boxSize/4,boxSize/4,c); break;
    case 11: this->fullBox(boxOffset+(3*boxSize)+30+((6*x)/boxSize),boxOffset+(2*boxSize+20)+((6*y)/boxSize),boxSize/4,boxSize/4,c); break;

    case 12: this->fullBox(boxOffset+(0*boxSize)+0+ ((6*x)/boxSize),boxOffset+(3*boxSize+30)+((6*y)/boxSize),boxSize/4,boxSize/4,c); break;
    case 13: this->fullBox(boxOffset+(1*boxSize)+10+((6*x)/boxSize),boxOffset+(3*boxSize+30)+((6*y)/boxSize),boxSize/4,boxSize/4,c); break;
    case 14: this->fullBox(boxOffset+(2*boxSize)+20+((6*x)/boxSize),boxOffset+(3*boxSize+30)+((6*y)/boxSize),boxSize/4,boxSize/4,c); break;
    case 15: this->fullBox(boxOffset+(3*boxSize)+30+((6*x)/boxSize),boxOffset+(3*boxSize+30)+((6*y)/boxSize),boxSize/4,boxSize/4,c); break;

    }  
}



void SDL_GUI::drawBoxNumber(int n, Uint32 c)
{
  //sg.box(boxOffset+(0*boxSize),   boxOffset,boxSize,boxSize,0xAECD15);

  switch (n)
    {
    case 0:  this->fullBox(boxOffset+(0*boxSize)+0, boxOffset+(0*boxSize)+0,boxSize,boxSize,c); break;
    case 1:  this->fullBox(boxOffset+(1*boxSize)+10,boxOffset+(0*boxSize)+0,boxSize,boxSize,c); break;
    case 2:  this->fullBox(boxOffset+(2*boxSize)+20,boxOffset+(0*boxSize)+0,boxSize,boxSize,c); break;
    case 3:  this->fullBox(boxOffset+(3*boxSize)+30,boxOffset+(0*boxSize)+0,boxSize,boxSize,c); break;

    case 4:  this->fullBox(boxOffset+(0*boxSize)+0, boxOffset+(1*boxSize+10),boxSize,boxSize,c); break;
    case 5:  this->fullBox(boxOffset+(1*boxSize)+10,boxOffset+(1*boxSize+10),boxSize,boxSize,c); break;
    case 6:  this->fullBox(boxOffset+(2*boxSize)+20,boxOffset+(1*boxSize+10),boxSize,boxSize,c); break;
    case 7:  this->fullBox(boxOffset+(3*boxSize)+30,boxOffset+(1*boxSize+10),boxSize,boxSize,c); break;

    case 8:  this->fullBox(boxOffset+(0*boxSize)+0, boxOffset+(2*boxSize+20),boxSize,boxSize,c); break;
    case 9:  this->fullBox(boxOffset+(1*boxSize)+10,boxOffset+(2*boxSize+20),boxSize,boxSize,c); break;
    case 10: this->fullBox(boxOffset+(2*boxSize)+20,boxOffset+(2*boxSize+20),boxSize,boxSize,c); break;
    case 11: this->fullBox(boxOffset+(3*boxSize)+30,boxOffset+(2*boxSize+20),boxSize,boxSize,c); break;
      
    case 12: this->fullBox(boxOffset+(0*boxSize)+0, boxOffset+(3*boxSize+30),boxSize,boxSize,c); break;
    case 13: this->fullBox(boxOffset+(1*boxSize)+10,boxOffset+(3*boxSize+30),boxSize,boxSize,c); break;
    case 14: this->fullBox(boxOffset+(2*boxSize)+20,boxOffset+(3*boxSize+30),boxSize,boxSize,c); break;
    case 15: this->fullBox(boxOffset+(3*boxSize)+30,boxOffset+(3*boxSize+30),boxSize,boxSize,c); break;
    }               
}

/*
void SDL_GUI::drawTTFTextNumber(int n,char * str)
{
  switch(n)
    {
    case 0:  this->guiTTFText(boxOffset+(0*boxSize)+0 ,boxOffset,str); break;
    case 1:  this->guiTTFText(boxOffset+(1*boxSize)+10,boxOffset,str); break;
    case 2:  this->guiTTFText(boxOffset+(2*boxSize)+20,boxOffset,str); break;
    case 3:  this->guiTTFText(boxOffset+(3*boxSize)+30,boxOffset,str); break;

    case 4:  this->guiTTFText(boxOffset+(0*boxSize)+0 ,boxOffset+(1*boxSize+10),str); break;
    case 5:  this->guiTTFText(boxOffset+(1*boxSize)+10,boxOffset+(1*boxSize+10),str); break;
    case 6:  this->guiTTFText(boxOffset+(2*boxSize)+20,boxOffset+(1*boxSize+10),str); break;
    case 7:  this->guiTTFText(boxOffset+(3*boxSize)+30,boxOffset+(1*boxSize+10),str); break;

    case 8:  this->guiTTFText(boxOffset+(0*boxSize)+0 ,boxOffset+(2*boxSize+20),str); break;
    case 9:  this->guiTTFText(boxOffset+(1*boxSize)+10,boxOffset+(2*boxSize+20),str); break;
    case 10: this->guiTTFText(boxOffset+(2*boxSize)+20,boxOffset+(2*boxSize+20),str); break;
    case 11: this->guiTTFText(boxOffset+(3*boxSize)+30,boxOffset+(2*boxSize+20),str); break;
      
    case 12: this->guiTTFText(boxOffset+(0*boxSize)+0 ,boxOffset+(3*boxSize+30),str); break;
    case 13: this->guiTTFText(boxOffset+(1*boxSize)+10,boxOffset+(3*boxSize+30),str); break;
    case 14: this->guiTTFText(boxOffset+(2*boxSize)+20,boxOffset+(3*boxSize+30),str); break;
    case 15: this->guiTTFText(boxOffset+(3*boxSize)+30,boxOffset+(3*boxSize+30),str); break;

    }
}
*/

void SDL_GUI::drawTTFTextNumberFirstLine(int n,const char * str)
{
  switch(n)
    {
    case 0:  this->guiTTFText(boxOffset+(0*boxSize)+0 ,boxOffset,str); break;
    case 1:  this->guiTTFText(boxOffset+(1*boxSize)+10,boxOffset,str); break;
    case 2:  this->guiTTFText(boxOffset+(2*boxSize)+20,boxOffset,str); break;
    case 3:  this->guiTTFText(boxOffset+(3*boxSize)+30,boxOffset,str); break;

    case 4:  this->guiTTFText(boxOffset+(0*boxSize)+0 ,boxOffset+(1*boxSize+10),str); break;
    case 5:  this->guiTTFText(boxOffset+(1*boxSize)+10,boxOffset+(1*boxSize+10),str); break;
    case 6:  this->guiTTFText(boxOffset+(2*boxSize)+20,boxOffset+(1*boxSize+10),str); break;
    case 7:  this->guiTTFText(boxOffset+(3*boxSize)+30,boxOffset+(1*boxSize+10),str); break;

    case 8:  this->guiTTFText(boxOffset+(0*boxSize)+0 ,boxOffset+(2*boxSize+20),str); break;
    case 9:  this->guiTTFText(boxOffset+(1*boxSize)+10,boxOffset+(2*boxSize+20),str); break;
    case 10: this->guiTTFText(boxOffset+(2*boxSize)+20,boxOffset+(2*boxSize+20),str); break;
    case 11: this->guiTTFText(boxOffset+(3*boxSize)+30,boxOffset+(2*boxSize+20),str); break;
      
    case 12: this->guiTTFText(boxOffset+(0*boxSize)+0 ,boxOffset+(3*boxSize+30),str); break;
    case 13: this->guiTTFText(boxOffset+(1*boxSize)+10,boxOffset+(3*boxSize+30),str); break;
    case 14: this->guiTTFText(boxOffset+(2*boxSize)+20,boxOffset+(3*boxSize+30),str); break;
    case 15: this->guiTTFText(boxOffset+(3*boxSize)+30,boxOffset+(3*boxSize+30),str); break;

    }
}

void SDL_GUI::drawTTFTextNumberSecondLine(int n,const char * str)
{
  switch(n)
    {
    case 0:  this->guiTTFText(boxOffset+(0*boxSize)+0 ,boxOffset+10            ,str); break;
    case 1:  this->guiTTFText(boxOffset+(1*boxSize)+10,boxOffset+10            ,str); break;
    case 2:  this->guiTTFText(boxOffset+(2*boxSize)+20,boxOffset+10            ,str); break;
    case 3:  this->guiTTFText(boxOffset+(3*boxSize)+30,boxOffset+10            ,str); break;

    case 4:  this->guiTTFText(boxOffset+(0*boxSize)+0 ,boxOffset+(1*boxSize+10)+10,str); break;
    case 5:  this->guiTTFText(boxOffset+(1*boxSize)+10,boxOffset+(1*boxSize+10)+10,str); break;
    case 6:  this->guiTTFText(boxOffset+(2*boxSize)+20,boxOffset+(1*boxSize+10)+10,str); break;
    case 7:  this->guiTTFText(boxOffset+(3*boxSize)+30,boxOffset+(1*boxSize+10)+10,str); break;

    case 8:  this->guiTTFText(boxOffset+(0*boxSize)+0 ,boxOffset+(2*boxSize+20)+10,str); break;
    case 9:  this->guiTTFText(boxOffset+(1*boxSize)+10,boxOffset+(2*boxSize+20)+10,str); break;
    case 10: this->guiTTFText(boxOffset+(2*boxSize)+20,boxOffset+(2*boxSize+20)+10,str); break;
    case 11: this->guiTTFText(boxOffset+(3*boxSize)+30,boxOffset+(2*boxSize+20)+10,str); break;
      
    case 12: this->guiTTFText(boxOffset+(0*boxSize)+0 ,boxOffset+(3*boxSize+30)+10,str); break;
    case 13: this->guiTTFText(boxOffset+(1*boxSize)+10,boxOffset+(3*boxSize+30)+10,str); break;
    case 14: this->guiTTFText(boxOffset+(2*boxSize)+20,boxOffset+(3*boxSize+30)+10,str); break;
    case 15: this->guiTTFText(boxOffset+(3*boxSize)+30,boxOffset+(3*boxSize+30)+10,str); break;

    }
}




void SDL_GUI::drawBMPTextNumber(int n,char * str)
{
  switch(n)
    {
    case 0:  this->guiBMPText(boxOffset+(0*boxSize)+0, boxOffset,str); break;
    case 1:  this->guiBMPText(boxOffset+(1*boxSize)+10,boxOffset,str); break;
    case 2:  this->guiBMPText(boxOffset+(2*boxSize)+20,boxOffset,str); break;
    case 3:  this->guiBMPText(boxOffset+(3*boxSize)+30,boxOffset,str); break;

    case 4:  this->guiBMPText(boxOffset+(0*boxSize)+0, boxOffset+(1*boxSize+10),str); break;
    case 5:  this->guiBMPText(boxOffset+(1*boxSize)+10,boxOffset+(1*boxSize+10),str); break;
    case 6:  this->guiBMPText(boxOffset+(2*boxSize)+20,boxOffset+(1*boxSize+10),str); break;
    case 7:  this->guiBMPText(boxOffset+(3*boxSize)+30,boxOffset+(1*boxSize+10),str); break;

    case 8:  this->guiBMPText(boxOffset+(0*boxSize)+0, boxOffset+(2*boxSize+20),str); break;
    case 9:  this->guiBMPText(boxOffset+(1*boxSize)+10,boxOffset+(2*boxSize+20),str); break;
    case 10: this->guiBMPText(boxOffset+(2*boxSize)+20,boxOffset+(2*boxSize+20),str); break;
    case 11: this->guiBMPText(boxOffset+(3*boxSize)+30,boxOffset+(2*boxSize+20),str); break;
      
    case 12: this->guiBMPText(boxOffset+(0*boxSize)+0, boxOffset+(3*boxSize+30),str); break;
    case 13: this->guiBMPText(boxOffset+(1*boxSize)+10,boxOffset+(3*boxSize+30),str); break;
    case 14: this->guiBMPText(boxOffset+(2*boxSize)+20,boxOffset+(3*boxSize+30),str); break;
    case 15: this->guiBMPText(boxOffset+(3*boxSize)+30,boxOffset+(3*boxSize+30),str); break;

    }
}




int SDL_GUI::openBMPFont()
{
  bmp_font = this->loadBMP("font.bmp");
  if(!bmp_font)
    {
      fprintf(stderr, "Couldn't load font!\n");
      return -1;
    } 
  return 0;
}


void SDL_GUI::guiBMPText(int x, int y, const char *txt)
{
  SDL_Surface *dst=this->screen;
  int sx = x;
  int sy = y;
  const char *stxt = txt;
  int highlights = 0;
  SDL_Rect sr;
  sr.w = FONT_CW;
  sr.h = FONT_CH;
  while(*txt)
    {
      int c = *txt++;
      switch(c)
	{
	case 0:	/* terminator */
	  break;
	case '\n':	/* newline */
	  x = sx;
	  y += FONT_CH;
	  break;
	case '\t':	/* tab */
	  x -= sx;
	  x += 8 * FONT_CW;
	  x %= 8 * FONT_CW;
	  x += sx;
	  break;
	case '\001':	/* red highlight */
	case '\002':	/* green highlight */
	case '\003':	/* yellow highlight */
	case '\004':	/* blue highlight */
	case '\005':	/* purple highlight */
	case '\006':	/* cyan highlight */
	case '\007':	/* white highlight */
	  highlights = 1;
	  if(*txt == '\001')
	    txt += 2;
	  break;
	case '\021':	/* red bullet */
	case '\022':	/* green bullet */
	case '\023':	/* yellow bullet */
	case '\024':	/* blue bullet */
	case '\025':	/* purple bullet */
	case '\026':	/* cyan bullet */
	case '\027':	/* white bullet */
	  {
	    SDL_Rect r;
	    int hlr = c & 1 ? 255 : 0;
	    int hlg = c & 2 ? 255 : 0;
	    int hlb = c & 4 ? 255 : 0;
	    Uint32 hlc = SDL_MapRGB(dst->format, hlr, hlg, hlb);
	    r.x = x;
	    r.y = y;
	    r.w = FONT_CW;
	    r.h = FONT_CH;
	    SDL_FillRect(dst, &r,
			 SDL_MapRGB(dst->format, 0, 0, 0));
	    //gui_dirty(&r);
	    r.x = x + 2;
	    r.y = y + 2;
	    r.w = FONT_CW - 6;
	    r.h = FONT_CH - 6;
	    SDL_FillRect(dst, &r, hlc);
	    x += FONT_CW;
	    break;
	  }
	default:	/* printables */
	  {
	    SDL_Rect dr;
	    if(c < ' ' || c > 127)
	      c = 127;
	    c -= 32;
	    sr.x = (c % (bmp_font->w / FONT_CW)) * FONT_CW;
	    sr.y = (c / (bmp_font->w / FONT_CW)) * FONT_CH;
	    dr.x = x;
	    dr.y = y;
	    SDL_BlitSurface(bmp_font, &sr, dst, &dr);
	    //gui_dirty(&dr);
	    x += FONT_CW;
	    break;
	  }
	}
    }
  if(!highlights)
    return;
  x = sx;
  y = sy;
  txt = stxt;
  while(*txt)
    {
      int c = *txt++;
      switch(c)
	{
	case 0:	/* terminator */
	  break;
	case '\n':	/* newline */
	  x = sx;
	  y += FONT_CH;
	  break;
	case '\t':	/* tab */
	  x -= sx;
	  x += 8 * FONT_CW;
	  x %= 8 * FONT_CW;
	  x += sx;
	  break;
	case '\001':	/* red highlight */
	case '\002':	/* green highlight */
	case '\003':	/* yellow highlight */
	case '\004':	/* blue highlight */
	case '\005':	/* purple highlight */
	case '\006':	/* cyan highlight */
	case '\007':	/* white highlight */
	  {
	    int hlr = c & 1 ? 255 : 0;
	    int hlg = c & 2 ? 255 : 0;
	    int hlb = c & 4 ? 255 : 0;
	    Uint32 hlc = SDL_MapRGB(screen->format, hlr, hlg, hlb);
	    int hlw = 1;
	    if(*txt == '\001')
	      {
		hlw = txt[1];
		txt += 2;
	      }
	    /*
	    gui_box(x - 2, y - 2,
		    FONT_CW * hlw + 2, FONT_CH + 2,
		    hlc, dst);
	    */
	    break;
	  }
	default:	/* printables */
	  x += FONT_CW;
	  break;
	}
    }
}




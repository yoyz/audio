#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "Pattern.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "Master.h"
#include "PatternReader.h"

using namespace std;

PatternReader::PatternReader() : twoDPVector(16,vector <Pattern>(TRACK_MAX)), 
				 loadedData(16,vector      <int>(TRACK_MAX))

				 //, 
				 //				 savedData(16,vector       <int>(TRACK_MAX))
{
  printf("PatternReader::PatternReader()\n");
}

PatternReader::~PatternReader()
{
  printf("PatternReader::~PatternReader()\n");
}

void PatternReader::init()
{
  int x;
  int y;
  
  for (x=0;x<16;x++)
    for (y=0;y<TRACK_MAX-1;y++)
      {
	loadedData[x][y]=DATA_HAS_NOT_BEEN_CHECK;
	//	savedData[x][y]=0;
	twoDPVector[x][y].init();
      }
}

bool PatternReader::PatternRemove(int PatternNumber,int TrackNumber)
{
  char filename[1024];
  sprintf(filename,"dataP%dT%d.pic",PatternNumber,TrackNumber);
  if (unlink(filename)==0)
    {
      loadedData[PatternNumber][TrackNumber]=DATA_DOES_NOT_EXIST_ON_STORAGE;
      return true;
    }
  else
    return false;
}


bool PatternReader::PatternDataExist(int PatternNumber,int TrackNumber)
{
  int match=0;
  //char * line;
  int PatNum,TrackNum,PatSize;
  bool retcode=true;
  bool found=false;
  char line[1024];
  char filename[1024];

  if (loadedData[PatternNumber][TrackNumber]==DATA_EXIST_ON_STORAGE)
    return true;
  if (loadedData[PatternNumber][TrackNumber]==DATA_DOES_NOT_EXIST_ON_STORAGE)
    return false;

  //line=(char*)malloc(1024);
  //fd=fopen(fn.c_str(),"r+");
  sprintf(filename,"dataP%dT%d.pic",PatternNumber,TrackNumber);
  fd=fopen(filename,"r+");
  if (fd==0)
    {
      return false;
    }

  while (match==0 && retcode==true)
    {
      char * catcheof;
      catcheof=fgets(line,512,fd);
      //printf("[%s]\n",st);
      //sleep(1);

      //match('Pattern 1 Track 1 Param PatternSize 16')
      sscanf(line,"Pattern %d Track %d Param PatternSize %d",
	     &PatNum,&TrackNum,&PatSize);
      if (PatNum    ==PatternNumber &&
	  TrackNum  ==TrackNumber)
	match=1;

      if ( catcheof==NULL)
	retcode=false;
    }

  if (match)
    found=true;

  if (found==true)
    loadedData[PatternNumber][TrackNumber]=DATA_EXIST_ON_STORAGE;
  else
    loadedData[PatternNumber][TrackNumber]=DATA_DOES_NOT_EXIST_ON_STORAGE;


  //free(line);
  fclose(fd);
  return found;
}

bool PatternReader::readPatternData(int PatternNumber,int TrackNumber, Pattern & P)
{
  PatternElement Pe;
  FILE *fd;
  int PatNum;
  int TrackNum;
  int PatSize;
  int PatBPM;
  int PatBPMDivider;
  int n[16];
  int t[16];
  int i;
  bool retcode=true;
  int match=0;
  //char * line;
  //line=(char*)malloc(1024);
  char line[1024];
  char filename[1024];

  sprintf(filename,"dataP%dT%d.pic",PatternNumber,TrackNumber);


  //check if file name can be open
  //fd=fopen(fn.c_str(),"r+");
  fd=fopen(filename,"r+");
  if (fd==0)
    {
      printf("[data file %s not found]\n",fn.c_str());
      return false;
      //exit(213);
    }
  
  while (match==0 && retcode==true)
    {
      char * catcheof;
      catcheof=fgets(line,512,fd);
      //printf("[%s]\n",st);
      //sleep(1);
      
      //match('Pattern 1 Track 1 Param PatternSize 16')
      sscanf(line,"Pattern %d Track %d Param PatternSize %d",
	     &PatNum,&TrackNum,&PatSize);
      //printf("%d %d %d",PatNum,TrackNum,PatSize);
      if (PatNum    ==PatternNumber &&
	  TrackNum  ==TrackNumber)
	match=1;
      if ( catcheof==NULL)
	retcode=false;
    }

  if (retcode==false)
    {
      P.setPatternSize(16);
      return false;
    }
  
  P.setPatternSize(PatSize);
  
  if (fgets(line,512,fd))
    {
      //match('Pattern 1 Track 1 Param PatternBPM 80')
      sscanf(line,"Pattern %d Track %d Param PatternBPM %d",
	     &PatNum,&TrackNum,&PatBPM);
      if (PatNum    ==PatternNumber &&
	  TrackNum  ==TrackNumber)
	{
	  P.setBPM(PatBPM);
	}
    }

  if (fgets(line,512,fd))
    {
      //match('Pattern 1 Track 1 Param PatternBPMDivider 4')
      sscanf(line,"Pattern %d Track %d Param PatternBPMDivider %d",
	     &PatNum,&TrackNum,&PatBPMDivider);
      if (PatNum    ==PatternNumber &&
	  TrackNum  ==TrackNumber)
	{
	  P.setBPMDivider(PatBPMDivider);
	}
    }


  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Note 41 0 52 0 22 0 12 0 21 11 14 12 43 12 54')
  sscanf(line,
	 "Pattern %d Track %d Param PitchLfoDepth %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &n[0], &n[1], &n[2], &n[3],
	 &n[4], &n[5], &n[6], &n[7],
	 &n[8], &n[9], &n[10],&n[11],
	 &n[12],&n[13],&n[14],&n[15]);

      if (PatNum    ==PatternNumber &&
	  TrackNum  ==TrackNumber)
	for (i=0;i<PatSize;i++)
	  {
	    Pe=P.getPatternElement(i);      
	    Pe.setLfoDepth(n[i]);
	    P.setPatternElement(i,Pe);      
	  }
      else
	retcode=false;


  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Note 41 0 52 0 22 0 12 0 21 11 14 12 43 12 54')
  sscanf(line,
	 "Pattern %d Track %d Param PitchLfoSpeed %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &n[0], &n[1], &n[2], &n[3],
	 &n[4], &n[5], &n[6], &n[7],
	 &n[8], &n[9], &n[10],&n[11],
	 &n[12],&n[13],&n[14],&n[15]);

      if (PatNum    ==PatternNumber &&
	  TrackNum  ==TrackNumber)
	for (i=0;i<PatSize;i++)
	  {
	    Pe=P.getPatternElement(i);      
	    Pe.setLfoSpeed(n[i]);
	    P.setPatternElement(i,Pe);      
	  }
      else
	retcode=false;



  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Note 41 0 52 0 22 0 12 0 21 11 14 12 43 12 54')
  sscanf(line,
	 "Pattern %d Track %d Param Amp %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &n[0], &n[1], &n[2], &n[3],
	 &n[4], &n[5], &n[6], &n[7],
	 &n[8], &n[9], &n[10],&n[11],
	 &n[12],&n[13],&n[14],&n[15]);

      if (PatNum    ==PatternNumber &&
	  TrackNum  ==TrackNumber)
	for (i=0;i<PatSize;i++)
	  {
	    Pe=P.getPatternElement(i);      
	    Pe.setAmp(n[i]);
	    P.setPatternElement(i,Pe);      
	  }
      else
	retcode=false;

  
  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Note 41 0 52 0 22 0 12 0 21 11 14 12 43 12 54')
  sscanf(line,
	 "Pattern %d Track %d Param Note %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &n[0], &n[1], &n[2], &n[3],
	 &n[4], &n[5], &n[6], &n[7],
	 &n[8], &n[9], &n[10],&n[11],
	 &n[12],&n[13],&n[14],&n[15]);

      if (PatNum    ==PatternNumber &&
	  TrackNum  ==TrackNumber)
	for (i=0;i<PatSize;i++)
	  {
	    Pe=P.getPatternElement(i);      
	    Pe.setNote(n[i]);
	    P.setPatternElement(i,Pe);      
	  }
      else
	retcode=false;



  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Trig 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param Trig %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.setTrig(t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;


  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Decay 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param VCOMix %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.setVCOMix(t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;


  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Attack 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param Attack %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.setAttack(t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;





  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Decay 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param Decay %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.setDecay(t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;




  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Sustain 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param Sustain %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.setSustain(t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;


  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Release 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param Release %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);      
	Pe.setRelease(t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;


  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Release 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param OscOneType %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);
	Pe.setOscillatorOneType(t[i]);
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;

  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Release 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param OscTwoType %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);    
	Pe.setOscillatorTwoType(t[i]);  
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;

  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Release 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param Cutoff %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);    
	Pe.setCutoff(t[i]);  
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;

  fgets(line,512,fd);
  //match('Pattern 1 Track 1 Param Release 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0')
  sscanf(line,
	 "Pattern %d Track %d Param Resonance %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d %d",
	 &PatNum,&TrackNum,
	 &t[0], &t[1], &t[2], &t[3],
	 &t[4], &t[5], &t[6], &t[7],
	 &t[8], &t[9], &t[10],&t[11],
	 &t[12],&t[13],&t[14],&t[15]);

  if (PatNum    ==PatternNumber &&
      TrackNum  ==TrackNumber)
    for (i=0;i<PatSize;i++)
      {
	Pe=P.getPatternElement(i);    
	Pe.setResonance(t[i]);  
	P.setPatternElement(i,Pe);      
      }
  else
    retcode=false;




  if (retcode==true)
    loadedData[PatternNumber][TrackNumber]=DATA_EXIST_ON_STORAGE;
  
  //free(line);
  fclose(fd);
  return retcode;
}


bool PatternReader::writePattern(int PatternNumber, int TrackNumber, Pattern & P)
{  
  FILE         * fd=NULL;
  int            PatNum=0;
  int            TrackNum=0;
  int            PatSize=0;  
  int            match=0;
  vector<string> data;
  bool           retcode=true;
  bool           found=false;
  //char         * line=NULL;
  //char         * line2=NULL;
  char         * catcheof=NULL;
  //line=(char*)malloc(1024);
  //line2=(char*)malloc(1024);
  char line[1024];
  char line2[1024];
  char filename[1024];

  sprintf(filename,"dataP%dT%d.pic",PatternNumber,TrackNumber);

  //  printf("SIZE:%d\n",P.getSize());
  //  exit(1);

  //  loadedData[PatternNumber][TrackNumber]==0;
  //savedData[PatternNumber][TrackNumber]==0;

  
  //fd=fopen(fn.c_str(),"r+");
  fd=fopen(filename,"r+");
  if (fd==0)
    {
      printf("[data file %s not found]\n",fn.c_str());
      retcode=true;
      //exit(213);
    }
  else
    {

      while (retcode==true)
	{
	  catcheof=fgets(line,512,fd);
	  sscanf(line,"Pattern %d Track %d ",
		 &PatNum,&TrackNum);
	  if (PatNum    ==  PatternNumber &&
	      TrackNum  ==  TrackNumber)
	    found=true;
	  else
	    {
	      data.insert(data.end(),line);
	    }
	  if ( catcheof==NULL)
	    retcode=false;
	}
      fclose(fd);
    }
  //  data.insert(data.end(),"\n");

  //insert 'Pattern 2 Track 2 Param PatternSize 16'
  sprintf(line,"Pattern %d Track %d Param PatternSize %d\n",
	  PatternNumber,
	  TrackNumber, 
	  P.getSize());
  data.insert(data.end(),line);


  sprintf(line,"Pattern %d Track %d Param PatternBPM %d\n",
	  PatternNumber,
	  TrackNumber, 
	  P.getBPM());
  data.insert(data.end(),line);

  sprintf(line,"Pattern %d Track %d Param PatternBPMDivider %d\n",
	  PatternNumber,
	  TrackNumber, 
	  P.getBPMDivider());
  data.insert(data.end(),line);


  //line=""; //Weird ?


  //insert 'Pattern 2 Track 2 Param Note 84 0 52 0 22 0 12 0 21 11 14 12 43 12 54 13'
  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param PitchLfoDepth ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).getLfoDepth());
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);


  //insert 'Pattern 2 Track 2 Param Note 84 0 52 0 22 0 12 0 21 11 14 12 43 12 54 13'
  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param PitchLfoSpeed ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).getLfoSpeed());
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);


  //insert 'Pattern 2 Track 2 Param Note 84 0 52 0 22 0 12 0 21 11 14 12 43 12 54 13'
  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Amp ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).getAmp());
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);


  //insert 'Pattern 2 Track 2 Param Note 84 0 52 0 22 0 12 0 21 11 14 12 43 12 54 13'
  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Note ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).getNote());
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  //insert 'Pattern 2 Track 2 Param Trig 0  0 1  0 0  0 1  0 0  1  0  0  1  0  0 0'
  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Trig ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).getTrig());
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param VCOMix ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).getVCOMix());
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Attack ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).getAttack());
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Decay ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).getDecay());
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Sustain ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).getSustain());
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Release ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).getRelease());
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param OscOneType ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).getOscillatorOneType());
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param OscTwoType ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).getOscillatorTwoType());    
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);


  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Cutoff ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).getCutoff());
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);

  for (int i=0; i< P.getSize();i++)
    {
      if (i==0)
	sprintf(line,"Pattern %d Track %d Param Resonance ",PatternNumber,TrackNumber);
      sprintf(line+strlen(line),"%d ",P.getPatternElement(i).getResonance());
    }
  sprintf(line+strlen(line),"\n");
  data.insert(data.end(),line);



  sprintf(line,"\n");
  data.insert(data.end(),line);

  sprintf(line,"\n");
  data.insert(data.end(),line);

  //fd=fopen(fn.c_str(),"w");
  fd=fopen(filename,"w");
  for (int i=0; i< data.size();i++)
    {
      fprintf(fd,"%s",data[i].c_str());
    }
  fclose(fd);

  loadedData[PatternNumber][TrackNumber]=DATA_EXIST_ON_STORAGE;
}



void PatternReader::setFileName(string filename)
{
  fn=filename;
}

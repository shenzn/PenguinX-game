//*****************************************************************************
//*************************** Game: PenguinX **********************************
//************************ By: Shendelyar Evgeniy *****************************
//*****************************************************************************
/******************************************************************************
'This program is free software; you can redistribute it and/or modify it under
'the terms of the GNU General Public License as published by the Free Software
'Foundation; either version 2 of the License, or (at your option) any later 
'version.
'
'This program is distributed in the hope that it will be useful, but WITHOUT
'ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
'FOR A PARTICULAR PURPOSE. See the GNU General Public License for more details.
'
'You should have received a copy of the GNU General Public License along with
'this program; if not, write to the Free Software Foundation, Inc., 59 Temple
'Place, Suite 330, Boston, MA 02111-1307 USA.
******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "SDL/SDL.h"
#include "sprite.h"
#include "DrawLIB.h"
#include "TTF_Text.h"
#include <SDL/SDL_framerate.h>
#include <SDL/SDL_mixer.h>

SDL_Surface *screen;  //*****
SDL_Surface *about;
bool draw_house=false;
Sprite obj_img;
Sprite bon_img;
Sprite img_bg;
Sprite img_effect;
Sprite img_finish;
Sprite img_plot;
Sprite tux;
Uint8* keys;
//******************
short int dy_bg=1,a_bg=0;
short int x_t=200,y_t=400,dy_t=2,dx_t=2;

short int fr_t=0;

const short int size_bon=11,size_obj=21;

short int tm_obj=1000;
short int tm_level=100;

float zoom_tux=1;

short int type_bg=-4;
short int score_fish=0,score_almaz=0,score_min=100;
short int global_score=0;

Uint32 time_fr,ttime,timer_level,time_jump=SDL_GetTicks(),time_zoom,time_obj,time_aspeed;

bool JumpTux=false;
bool quit=true;
bool effect=false;

FPSmanager  man;

//*************

struct POINT_OBJ
{
  short int x,y;
  int img;
  bool live;
};

POINT_OBJ bg[22][27];
POINT_OBJ bon[size_bon];
POINT_OBJ obj[21];

TTF_Font *fnt,*txt;

Mix_Music *music;
Mix_Chunk *collide[5],*jump,*ajump;
//**********************************
void InitBG();
void DrawBG();
void TestBG();
bool TestKey();
void DrawTux();
void TestTime();
void TestTux();
void NewObj();
void DrawObj();
void TestObj();
void InitObj();
void NewBon();
void DrawBon();
void FreeSprite();
void DrawText();
void GameOver();
void GameLoop();
void InitNextLevel();
void DrawEffect();
void InitPause();
void DrawAbout();
void EndGame();
void StartGame();
void SoundFree();
//**********************************

//|||||||||||||||||||||||||||||||||||||||||||||||||
int main(int argc, char *argv[])
{
  srand(time(NULL));

  SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO);
  screen=SDL_SetVideoMode(640,480,32,SDL_HWSURFACE|SDL_ANYFORMAT);
  TTF_Init();
  Mix_OpenAudio(44100,AUDIO_S16SYS,2,2048);


  SDL_initFramerate(&man);
  SDL_setFramerate(&man,  200);
  ttime=SDL_GetTicks();
  time_fr=SDL_GetTicks();

  music=Mix_LoadMUS("sound/music.mod");
  collide[0]=Mix_LoadWAV("sound/col1.wav");
  collide[1]=Mix_LoadWAV("sound/col2.wav");
  collide[2]=Mix_LoadWAV("sound/col3.wav");
  collide[3]=Mix_LoadWAV("sound/col4.wav");
  collide[4]=Mix_LoadWAV("sound/col5.wav");
  jump=Mix_LoadWAV("sound/jump.wav");
  ajump=Mix_LoadWAV("sound/ajump.wav");

  about=LoadImage("img/about.png");

  img_bg.SLoad("img/image_bg.png",12,4,3);
  img_bg.SSetMaskRGB(255,0,255);
  img_bg.SSet_xy(10,10);
  img_bg.SSetFrame(0);

  tux.SLoad("img/image.png",5);
  tux.SSetMaskRGB(255,0,255);
  tux.SSetFrame(0);


  obj_img.SLoad("img/image2.png",8,4,2);
  obj_img.SSetMaskRGB(255,0,255);

  bon_img.SLoad("img/image3.png",2);
  bon_img.SSetMaskRGB(255,0,255);

  img_effect.SLoad("img/effect.png",6);
  img_effect.SSetMaskRGB(255,0,255);

  img_finish.SLoad("img/finish.png",7);
  img_finish.SSetMaskRGB(255,0,255);
  img_finish.SSetAlphaALL(128);
  img_finish.SSet_xy(270,200);

  img_plot.SLoad("img/img_plot.png",3);
  img_plot.SSet_xy(150,250);

  fnt=TTF_OpenFont("courier.ttf", 14);
  txt=TTF_OpenFont("courier.ttf", 20);

  //************************************
  StartGame();
  InitNextLevel();
  Mix_PlayMusic(music,-1);
  GameLoop();

  InitNextLevel();
  GameLoop();

  InitNextLevel();
  GameLoop();

  EndGame();

  FreeSprite();
  SoundFree();
  TTF_Quit();
  SDL_Quit();
}
//|||||||||||||||||||||||||||||||||||||||||||||||||||*

void GameLoop()
{
  while(quit)
  {
    quit=TestKey();
    TestTime();
    TestBG();
    TestObj();
    TestTux();
    DrawBG();
    DrawObj();
    DrawBon();
    DrawTux();
    DrawText();
    GameOver();
    SDL_Flip(screen);
    SDL_framerateDelay(&man);
  }
}

void InitNextLevel()
{
  Mix_PauseMusic();
  x_t=320;y_t=240;
  tm_obj-=100;
  tm_level=100;
  score_min+=25;
  global_score+=(2*score_fish)+(3*score_almaz);
  score_fish=0;
  score_almaz=0;
  type_bg+=4;
  quit=true;
  InitObj();
  InitBG();
  ClearScreen(screen);
  char lev[7]="Level:";
  char num[3]="";
  int lv=1;
  if (type_bg==4){lv=2;}
  if (type_bg==8){lv=3;}
  sprintf(num,"%i",lv);
  strcat(lev,num);
  print_ttf(txt,screen,lev,250,200,255,50,50);
  img_plot.SSetFrame(lv-1);
  img_plot.SDraw();
  SDL_Flip(screen);
  SDL_Delay(2000);
  Mix_ResumeMusic();
}

void InitBG()
{
  int i,j,e=-23;
  for(i=0;i<22;i++)
  {
    for(j=0;j<27;j++)
    {
      bg[i][j].x=j*24;
      bg[i][j].y=e;
      bg[i][j].img=(rand()%4)+type_bg;
      bg[i][j].live=true;
    }
    e=e+23;
  }
}

void DrawBG()
{
  int i,j;
  for(i=0;i<22;i++)
  {
    for(j=0;j<27;j++)
    {
      if (bg[i][j].live==true)
      {
        img_bg.SSetFrame(bg[i][j].img);
        img_bg.SSet_xy(bg[i][j].x,bg[i][j].y);
        img_bg.SDraw();
      }
    }
  }
}

void TestBG()
{
  int i,j;
  for(i=0;i<22;i++)
  {
    for(j=0;j<27;j++)
    {
      if (bg[i][j].y>=480)
      {
        bg[i][j].y=(-30);
        bg[i][j].img=(rand()%4)+type_bg;
      }
      bg[i][j].y+=dy_bg;
    }
  }
}

bool TestKey()
{
  SDL_Event event;

  while ( SDL_PollEvent(&event) )
  {
    if ( event.type == SDL_QUIT ){ exit(1); }
    if ( event.type == SDL_KEYDOWN )
    {
     // if ( event.key.keysym.sym == SDLK_ESCAPE ){ return false; }

    }
  }
  keys = SDL_GetKeyState(NULL);
  if (JumpTux==false)
  {
    if ( (keys[SDLK_UP]) and (y_t>0) ){y_t-=dy_t;}
    if ( (keys[SDLK_DOWN]) and (y_t<450) ){y_t+=dy_t;}
    if ( (keys[SDLK_RIGHT]) and (x_t<600) ){x_t+=dx_t;}
    if ( (keys[SDLK_LEFT]) and (x_t>0) ){x_t-=dx_t;}
    if ( (keys[SDLK_x])){dy_bg=1;}
    if ( (keys[SDLK_c])){dy_bg=2;}
    if ( (keys[SDLK_p])){
         Mix_PauseMusic();
         print_ttf(txt,screen,"PAUSE!",270,400,255,0,0);
         print_ttf(txt,screen,"Press any key!",220,440,255,50,0);
         SDL_Flip(screen);
         InitPause();
         Mix_ResumeMusic();
         }
    if ( (keys[SDLK_F1])){DrawAbout();}
    if ( (keys[SDLK_SPACE]) and (ttime-time_jump>1500) )
    {
      JumpTux=true;
      Mix_PlayChannel(0,jump,0);
      time_jump=SDL_GetTicks();
    }
  }
  return true;
}

void DrawTux()
{
  bool temp=true;
  tux.SSet_xy(x_t,y_t);
  if (ttime-time_fr>50) {fr_t+=1;time_fr=SDL_GetTicks();}
  if (fr_t>4) {fr_t=0;}
  tux.SSetFrame(fr_t);
  if (!JumpTux)
  {
    tux.SDraw();
  }
  else
  {
    SDL_Surface *temp;
    temp=ZoomIMG(tux.SGetFrameIMG(0),zoom_tux,1);
    DrawIMG(temp,screen,x_t,y_t);
    SDL_FreeSurface(temp);
    if (ttime-time_jump>500){temp=false;}
    if (ttime-time_jump>1000){JumpTux=false;zoom_tux=1;}
    if (ttime-time_zoom>10)
    {
      time_zoom=SDL_GetTicks();
      if (temp) zoom_tux+=.005;
      if (temp==false) zoom_tux-=.005;
    }

  }
  DrawEffect();
}

void TestTime()
{
  ttime=SDL_GetTicks();
  if (ttime-time_obj>tm_obj){time_obj=SDL_GetTicks();NewObj();NewBon();if (tm_obj>350){tm_obj-=1;}}
  if (ttime-timer_level>1000){timer_level=SDL_GetTicks();tm_level-=1;}
  if (ttime-time_aspeed>1000){dy_bg=1;}
}

void TestTux()
{
  if (JumpTux==false)
  {

    int i;
    for(i=0;i<size_obj;i++)
    {
      if (obj[i].live==true and obj[i].img<=3)
      {

        if ((tux.SPCollide(obj_img.SGetFrameIMG(obj[i].img-1),obj[i].x,obj[i].y)))
        {
          if (obj[i].img==0)
          {
            img_effect.SSetFrame(0);effect=true;
            img_effect.SSet_xy(x_t-30,y_t-20);
            obj[i].img=4;score_fish-=5;score_almaz-=5;
            Mix_PlayChannel(0,collide[0],0);
            Mix_PlayChannel(0,collide[1],0);
          }
          if (obj[i].img==1)
          {
            img_effect.SSetFrame(0);effect=true;
            img_effect.SSet_xy(x_t-30,y_t-20);
            obj[i].img=5;score_almaz-=5;
            Mix_PlayChannel(0,collide[0],0);
            Mix_PlayChannel(0,collide[3],0);
          }
          if (obj[i].img==2)
          {
            img_effect.SSetFrame(0);effect=true;
            img_effect.SSet_xy(x_t-30,y_t-20);
            obj[i].img=6;score_fish-=5;
            Mix_PlayChannel(0,collide[0],0);
            Mix_PlayChannel(0,collide[4],0);
          }
          if (obj[i].img==3)
          {
            JumpTux=true;
            Mix_PlayChannel(0,ajump,0);
            time_jump=SDL_GetTicks();
            time_aspeed=SDL_GetTicks();dy_bg=2;
          }

        }
      }
    }
    int v;
    for(i=0;i<size_bon;i++)
    {
      if (bon[i].live==true)
      {
        if (bon[i].img==1){v=4;}
        else{v=5;}
        if ((tux.SPCollide(bon_img.SGetFrameIMG(bon[i].img-1),bon[i].x,bon[i].y)))
        {
          bon[i].live=false;
          if (bon[i].img==0){score_fish+=1;}
          if (bon[i].img==1){score_almaz+=1;}
          Mix_PlayChannel(0,collide[2],0);
        }
      }
    }

  }
}

void InitObj()
{
  int i;
  for(i=0;i<size_obj;i++)
  {
    obj[i].live=false;
  }
  for(i=0;i<size_bon;i++)
  {
    bon[i].live=false;
  }
}

void DrawObj()
{
  int i;
  for(i=0;i<size_obj;i++)
  {
    if (obj[i].live==true)
    {
      obj_img.SSetFrame(obj[i].img);
      obj[i].y+=dy_bg;
      obj_img.SSet_xy(obj[i].x,obj[i].y);
      obj_img.SDraw();
    }
  }
}

void TestObj()
{
  int i;
  for(i=0;i<size_obj;i++)
  {

    if (obj[i].y>500)
    {
      obj[i].live=false;
    }

  }
  for(i=0;i<size_bon;i++)
  {

    if (bon[i].y>500)
    {
      bon[i].live=false;
    }

  }
}

void NewObj()
{
  int i,j;

  for(i=0;i<size_obj;i++)
  {

    if (obj[i].live==false)
    {
      obj[i].y=-42;
      obj[i].x=rand()%600;
      obj[i].img=rand()%4;
      for(j=0;j<size_obj;j++)
      {
        if (!SDL_CollideBoundingBox(obj[j].x,obj[j].y,obj_img.SGet_w(),obj_img.SGet_h(),obj[i].x,obj[i].y,
                                    obj_img.SGet_w(),obj_img.SGet_h()))
        {
          obj[i].live=true;
        }
      }
      break;
    }

  }

}

void NewBon()
{
  int i;

  for(i=0;i<size_bon;i++)
  {

    if (bon[i].live==false)
    {
      bon[i].y=-42;
      bon[i].x=rand()%600;
      bon[i].img=rand()%2;
      bon[i].live=true;
      break;
    }

  }

}

void DrawBon()
{
  int i;
  for(i=0;i<size_bon;i++)
  {
    if (bon[i].live==true)
    {
      bon_img.SSetFrame(bon[i].img);
      bon[i].y+=dy_bg;
      bon_img.SSet_xy(bon[i].x,bon[i].y);
      bon_img.SDraw();
    }
  }
}

void FreeSprite()
{
  img_bg.SUnload();
  obj_img.SUnload();
  bon_img.SUnload();
}

void DrawText()
{
  char score[50]="";
  char score2[50]="";
  sprintf(score,"%i       %i",score_fish,score_almaz);
  sprintf(score2,"%i",tm_level);
  if (((2*score_fish)+(3*score_almaz))<score_min)
  {
    print_ttf(fnt,screen,score,50,12,200,0,0);
  }
  else
  {
    print_ttf(fnt,screen,score,50,12,0,255,100);
  }
  print_ttf(txt,screen,score2,300,12,100,0,100);
  bon_img.SSet_xy(10,10);
  bon_img.SSetFrame(0);
  bon_img.SDraw();

  bon_img.SSet_xy(100,10);
  bon_img.SSetFrame(1);
  bon_img.SDraw();

}

void GameOver()
{
  if (tm_level<5){img_finish.SDraw(80);}
  if (score_fish<0)
  {
    score_fish=0;
  }
  if(score_almaz<0)
  {
    score_almaz=0;
  }

  if (tm_level<0)
  {
    if (((2*score_fish)+(3*score_almaz))>score_min)
    {
      quit=false;
    }
    else
    {
      ClearScreen(screen);
      print_ttf(txt,screen,"GAME OVER",250,200,255,50,50);
      SDL_Flip(screen);
      SDL_Delay(3000);
      exit(1);
    }
  }
}

void DrawEffect()
{
  if (img_effect.SGetFrame()>4){effect=false;}
  if (effect==true)
  {
    img_effect.SSet_xy(img_effect.SGet_x(),img_effect.SGet_y()+dy_bg);
    img_effect.SDraw(100);
  }
}

void InitPause()
{
  SDL_Event event;
  bool quit=true;
  while(quit)
  {
    while ( SDL_PollEvent(&event) )
    {

      if ( event.type == SDL_KEYDOWN )
      {
        quit=false;
      }
    }
  }
}

void DrawAbout()
{
  Mix_PauseMusic();
  DrawBG();
  DrawObj();
  DrawBon();
  DrawTux();
  DrawIMG(about,screen,200,140);
  print_ttf(fnt,screen,"Free Game",300,200,255,0,100); 
  print_ttf(fnt,screen,"GNU General Public License ",220,220,255,0,100); 
  SDL_Flip(screen);
  InitPause();
  Mix_ResumeMusic();
}

void EndGame()
{
  char score[6]="";
  char temp[7]="Score:";
  sprintf(score,"%i",global_score);
  strcat(temp,score);
  ClearScreen(screen);
  print_ttf(txt,screen,"You attained victory!!!",175,200,255,50,50);
  SDL_Flip(screen);
  print_ttf(txt,screen,temp,290,300,0,255,50);
  SDL_Flip(screen);
  SDL_Delay(1000);
  InitPause();
  FILE *out;
  out=fopen("score","at");
  fprintf(out,"***\n Score:%i\n***",global_score);
  fclose(out);
}

void StartGame()
{
  SDL_Surface *img,*temp;
  TTF_Font *font;
  font=TTF_OpenFont("courier.ttf", 16);
  img=LoadImage("img/logo.png");
  float rotate=0,zoom=0;
  while(rotate<360)
  {
    ClearScreen(screen);
    temp=RotateIMG(ZoomIMG(img,zoom,1),rotate,1);
    DrawIMG(temp,screen,150,100);
    rotate+=1;
    zoom+=0.0027777;
    SDL_Flip(screen);
  }
  print_ttf(font,screen,"Press any key...",225,300,255,100,255);
  print_ttf(txt,screen,"By: Shendelyar Evgeniy",150,420,100,0,255);
  SDL_Flip(screen);
  SDL_Delay(1000);
  InitPause();
  SDL_FreeSurface(img);
  SDL_FreeSurface(temp);
  TTF_CloseFont( font );
}

void SoundFree()
{
  Mix_FreeMusic(music);
  Mix_FreeChunk(jump);
  Mix_FreeChunk(ajump);
  Mix_FreeChunk(collide[0]);
  Mix_FreeChunk(collide[1]);
  Mix_FreeChunk(collide[2]);
  Mix_FreeChunk(collide[3]);
  Mix_FreeChunk(collide[4]);
}

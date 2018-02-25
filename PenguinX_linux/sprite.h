//***************************************
//******** Game: PenguinX ***************
//******** By: Shendelyar Evgeniy *******
//***************************************
//***************************************
//******** Sprite ENGINE ****************
//******** By: Shendelyar Evgeniy *******
//***************************************
#ifndef SPRITE_IMAGE_HPP
#define SPRITE__IMAGE_HPP
   #include "SDL/SDL_image.h"
#endif
#ifndef COLLIDE_HPP
#define COLLIDE_HPP
   #include "collide.h"
#endif

//***Поверхность экрана должна быть глобальной при использовании сложного спрайта ****

struct SSprite
{
  SDL_Surface *image;
};

void ClearDisplay(SDL_Surface *screen)
{
	int i;
	/* Set the screen to black */
	if ( SDL_LockSurface(screen) == 0 ) {
		Uint32 black;
		Uint8 *pixels;
		black = SDL_MapRGB(screen->format, 0, 0, 0);
		pixels = (Uint8 *)screen->pixels;
		for ( i=0; i<screen->h; ++i ) {
			memset(pixels, black,
				screen->w*screen->format->BytesPerPixel);
			pixels += screen->pitch;
		}
		SDL_UnlockSurface(screen);
	}
}

//Базовый класс ***Sprite***

class Sprite
{
public:
  Sprite();
  ~Sprite()
  {
    printf("Sprite UNLOAD!--OK\n");
    SUnload();
  }
  short int SLoad(char *name,unsigned short int frames=0, unsigned short int stolb=0,
            unsigned short int stroka=1);
  short int SDraw();
  short int SDraw(unsigned short int pause);
  void SUnload();
  void SSet_xy(short int xp=0, short int yp=0);
  short int SGet_x();
  short int SGet_y();
  short int SGet_w();
  short int SGet_h();
  void SSetMaskRGB(Uint8 r=255,Uint8 g=0, Uint8 b=255);
  void SSetAlpha(unsigned short int alpha);
  void SSetAlphaALL(unsigned short int alpha);
  void SSetFrame(unsigned short int f);
  short int SGetFrame();
  void SSetXFrame(unsigned short int xfrm=1);
  void SSetYFrame(unsigned short int Yfrm=1);
  bool SPCollide(SDL_Surface *img,short int x,short int y);
  bool SCollide(short int wtwo, short int htwo,short int xtwo,short int ytwo);
  SDL_Surface* SGetFrameIMG(int lf=-1);
private:
  SDL_Surface* SGetFrameIMG(SDL_Surface *bufer,short int lf=-1,bool BS=false);
  SSprite* img;
  unsigned short int gframe,xframe,yframe;
  unsigned short int frame;
  short int x,y;
  Uint8 red,gren,blue;
  Uint32 timer; 
  bool mask;
};

Sprite::Sprite()
{
  frame=0;
  gframe=1;
  xframe=1;yframe=1;
  x=0;
  y=0;
  red=0;gren=0;blue=0;mask=false;
}


short int Sprite::SLoad(char *name,unsigned short int frames,unsigned short int stolb,
                  unsigned short int stroka)
{

  ClearDisplay(SDL_GetVideoSurface()); //***********

  SDL_Surface *buf;
  buf=IMG_Load(name);
  if (mask==true)
  {
    SDL_SetColorKey(buf, SDL_RLEACCEL|SDL_SRCCOLORKEY, SDL_MapRGB(buf->format,red,gren,blue));
  }
  buf=SDL_DisplayFormat(buf);
  gframe=frames;
  if (stolb==0 and stroka==1){stolb=frames;}
  frame=0;
  xframe=stolb;yframe=stroka;
  if (buf==0)
  {
    printf("Error! Image not load!");
    return 0;
  }
  img=new SSprite [gframe];
  bool BigSprite;
  if (stroka==1) {BigSprite=false;}
  else {BigSprite=true;}
  int i;
  for(i=0;i<gframe;i++)
  {
    img[i].image=SDL_DisplayFormat(SGetFrameIMG(buf,i,BigSprite));
  }

}

short int Sprite::SDraw()
{
  SDL_Rect dest;
  dest.x = x;
  dest.y = y;

  SDL_BlitSurface(img[SGetFrame()].image, NULL, SDL_GetVideoSurface(), &dest);
}

 short int Sprite::SDraw(unsigned short int pause){
 
 if (SDL_GetTicks()-timer>pause){
     timer=SDL_GetTicks();
     SSetFrame(SGetFrame()+1);
    }
 if (SGetFrame()>gframe-1) {SSetFrame(0);}
 SDraw();
}

SDL_Surface* Sprite::SGetFrameIMG(SDL_Surface* bufer,short int lf,bool BS)
{
  SDL_Surface *temp;
  Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
  rmask = 0xff000000;
  gmask = 0x00ff0000;
  bmask = 0x0000ff00;
  amask = 0x000000ff;
#else
  rmask = 0x000000ff;
  gmask = 0x0000ff00;
  bmask = 0x00ff0000;
  amask = 0xff000000;
#endif
  temp = SDL_CreateRGBSurface(SDL_HWSURFACE|SDL_SRCCOLORKEY, bufer->w/xframe, bufer->h/yframe, 32,
                              rmask, gmask, bmask, amask);
  if(temp == NULL)
  {
    fprintf(stderr, "CreateRGBSurface failed: %s\n", SDL_GetError());
  }

  SDL_Rect dest;
  dest.x = 0;
  dest.y = 0;
  SDL_Rect img_dest;

  short int lframe=0;
  if (lf==-1){lframe=frame;}
  else{lframe=lf;}

  short int y_frame_kord=0;
  if (BS==true)
  {
    short int i,stroka=-1;
    for (i=0;i<lframe+1;i++)
    {
      stroka+=1;
      if (stroka>=xframe) { stroka=0; y_frame_kord+=bufer->h/yframe; }

    }
    if (lframe>gframe-1) { lframe=0;y_frame_kord=0; }
    img_dest.x=stroka*(bufer->w/xframe);
    img_dest.y=y_frame_kord;
    img_dest.w=bufer->w/xframe;
    img_dest.h=bufer->h/yframe;
    SDL_BlitSurface(bufer, &img_dest, temp, NULL);
  }
  if (BS==false)
  {
    if (lframe>gframe-1) { lframe=0;y_frame_kord=0; }
    img_dest.x=lframe*(bufer->w/gframe);
    img_dest.y=y_frame_kord;
    img_dest.w=bufer->w/gframe;
    img_dest.h=bufer->h;
    SDL_BlitSurface(bufer, &img_dest, temp, NULL);
  }
  return temp;
}

SDL_Surface* Sprite::SGetFrameIMG( int lf)
{

  if (lf==-1)
  {
    return img[SGetFrame()].image;
  }
  else
  {
    if (lf<gframe){return img[lf].image;}
  }

}

void Sprite::SSet_xy(short int xp,short int yp)
{
  x=xp;
  y=yp;
}

short int Sprite::SGet_x()
{
  return x;
}

short int Sprite::SGet_y()
{
  return y;
}

short int Sprite::SGet_w()
{
  return img[SGetFrame()].image->w;
}

short int Sprite::SGet_h()
{
  return img[SGetFrame()].image->w;
}

void Sprite::SUnload()
{
  int i;
  delete [] img;
  img=0;
}

void Sprite::SSetMaskRGB(Uint8 r,Uint8 g, Uint8 b)
{
  red=r;
  gren=g;
  blue=b;
  mask=true;

  int i;
  for(i=0;i<gframe;i++)
  {
    SDL_SetColorKey(img[i].image,SDL_SRCCOLORKEY,SDL_MapRGB(img[i].image->format,r,g,b));
  }
}

void Sprite::SSetAlpha(unsigned short int alpha)
{
  SDL_SetAlpha(img[SGetFrame()].image, SDL_SRCALPHA, alpha);
}

void Sprite::SSetAlphaALL(unsigned short int alpha)
{
int i;
  for(i=0;i<gframe;i++)
  {
  SDL_SetAlpha(img[i].image, SDL_SRCALPHA, alpha);
  }
}

void Sprite::SSetFrame(unsigned short int f)
{
  frame=f;
  if (frame>gframe-1){frame=0;}
}

short int Sprite::SGetFrame()
{
  return frame;
}

void Sprite::SSetYFrame(unsigned short int yfrm)
{
  yframe=yfrm;
}

void Sprite::SSetXFrame(unsigned short int xfrm)
{
  xframe=xfrm;
}

bool Sprite::SPCollide(SDL_Surface *imgtwo,short int xtwo,short int ytwo)
{

  if(xtwo + imgtwo->w < x)	return 0;
  if(xtwo > x + SGet_w())	return 0;

  if(ytwo + imgtwo->h < y)      return 0;
  if(ytwo > y + SGet_h())	return 0;

  if (SDL_CollidePixel(img[SGetFrame()].image,x,y,imgtwo,xtwo,ytwo))
  {
  return 1;
  }
  return 0;
}

bool Sprite::SCollide(short int wtwo, short int htwo,short int xtwo,short int ytwo)
{

  if(xtwo + wtwo < x)	        return 0;
  if(xtwo > x + SGet_w())	return 0;

  if(ytwo + htwo < y)           return 0;
  if(ytwo > y + SGet_h())	return 0;

  return 1;
}



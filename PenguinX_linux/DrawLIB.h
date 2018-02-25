//***************************************
//******** Game: PenguinX ***************
//******** By: Shendelyar Evgeniy *******
//***************************************
#ifndef SDL_ROTOZOOM_HPP
#define SDL_ROTOZOOM_HPP
   #include "SDL/SDL_rotozoom.h"
#endif
#ifndef SDL_IMAGE_HPP
#define SDL_IMAGE_HPP
   #include "SDL/SDL_image.h"
#endif

void DrawIMG(SDL_Surface *img,SDL_Surface *screen, int x, int y){

 SDL_Rect dest;
 dest.y = y;
 dest.x = x;

 SDL_BlitSurface(img, NULL, screen, &dest); 

}

void DrawIMG(SDL_Surface *img,SDL_Surface *screen, int x, int y, int w, int h, int sx, int sy){

 SDL_Rect dest;
 dest.x = x;
 dest.y = y;

 SDL_Rect src;
 src.x = sx;
 src.y = sy;
 src.w = w;
 src.h = h;
 
 SDL_BlitSurface(img, &src, screen, &dest);

}

SDL_Surface* LoadImage(const char* filename)
{
	SDL_Surface* loaded = IMG_Load(filename);
	SDL_Surface* optimal = NULL;
	if (loaded)
	{
		SDL_SetColorKey(loaded, SDL_RLEACCEL|SDL_SRCCOLORKEY,
			SDL_MapRGB(loaded->format, 255, 0, 255));
		optimal = SDL_DisplayFormat(loaded);
		SDL_FreeSurface(loaded);
	}
	return optimal;
        //return loaded;
}

void ClearScreen(SDL_Surface *screen)
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

SDL_Surface* RotateIMG(SDL_Surface *img,double angle, short int smooth=0){
 SDL_Surface *temp;
  temp=rotozoomSurface (img, angle,1, smooth);
  SDL_MapRGB(temp->format, 255, 0, 255);
  return temp;
}



SDL_Surface* ZoomIMG(SDL_Surface *img,double zoom, short int smooth=0){
  SDL_Surface *temp;
  temp=rotozoomSurface (img, 0,zoom, smooth);
  SDL_MapRGB(temp->format, 255, 0, 255);
  return temp; 
}









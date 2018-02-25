//***************************************
//******** Game: PenguinX ***************
//******** By: Shendelyar Evgeniy *******
//***************************************
#ifndef SDL_TTF_HPP
#define SDL_TTF_HPP
#include "SDL/SDL_ttf.h"
#endif
// Вначале инициализируйте SDL_TTF


void print_ttf(SDL_Surface *sDest, char* message, char* font="courier.ttf",short int x=0,short int y=0,
                unsigned short int size=14, Uint8 r=255, Uint8 g=255, Uint8 b=255){
 SDL_Color color = {r,g,b,0};
 SDL_Rect dest = {x, y,0,0}; 
 TTF_Font *fnt = TTF_OpenFont(font, size);
 SDL_Surface *sText = TTF_RenderText_Blended( fnt, message, color);
 SDL_BlitSurface( sText,NULL, sDest,&dest );
 SDL_FreeSurface( sText );
 TTF_CloseFont( fnt );

}




void print_ttf(TTF_Font *font,SDL_Surface *sDest, char* message,short int x=0,short int y=0,Uint8 r=255,                Uint8 g=255, Uint8 b=255)
{
 SDL_Color color = {r,g,b,0};
 SDL_Rect dest = {x,y,0,0}; 
 SDL_Surface *sText = TTF_RenderText_Blended( font, message, color);
 SDL_BlitSurface( sText,NULL, sDest,&dest );
 SDL_FreeSurface( sText );
}


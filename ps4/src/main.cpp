// ORION Store PS4 - interface nativa experimental (sem instalar PKGs).
// Direitos autorais: projeto original, licenca MIT. Requer SDL2-PS4 e OpenOrbis.
#include <SDL2/SDL.h>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <string>
#include <vector>

struct Item { const char* title; const char* category; const char* about; SDL_Color color; };
static const Item catalog[]={
 {"CORRIDA NEBULOSA","CORRIDA","ITEM DEMONSTRATIVO SEM PKG",{14,125,182,255}},
 {"MUNDO PIXEL","PLATAFORMA","ITEM DEMONSTRATIVO SEM PKG",{150,65,139,255}},
 {"HORIZONTE LUNAR","AVENTURA","ITEM DEMONSTRATIVO SEM PKG",{185,86,48,255}},
 {"CIRCUITO NEON","ARCADE","ITEM DEMONSTRATIVO SEM PKG",{32,100,201,255}},
 {"LABIRINTO AZUL","PUZZLE","ITEM DEMONSTRATIVO SEM PKG",{5,140,135,255}},
 {"LENDAS DO NORTE","RPG","ITEM DEMONSTRATIVO SEM PKG",{125,91,54,255}}
};
static const char* sections[]={"INICIO","TODOS OS JOGOS","CATEGORIAS","DOWNLOADS","BIBLIOTECA","CONFIGURACOES"};
static const char* labels[]={"INICIO","JOGOS","CATEGORIAS","DOWNLOADS","BIBLIOTECA","AJUSTES"};
static int selected_section=0,selected_card=0;static bool in_cards=false,details=false;
static SDL_Renderer* r=nullptr;static SDL_Texture* font_texture=nullptr;
static void rect(int x,int y,int w,int h,SDL_Color c){SDL_SetRenderDrawColor(r,c.r,c.g,c.b,c.a);SDL_Rect q={x,y,w,h};SDL_RenderFillRect(r,&q);}
static void border(int x,int y,int w,int h,SDL_Color c){SDL_SetRenderDrawColor(r,c.r,c.g,c.b,c.a);SDL_Rect q={x,y,w,h};SDL_RenderDrawRect(r,&q);}
static void text(int x,int y,const std::string& s,int scale=1,SDL_Color tint={234,242,255,255}){
 if(!font_texture)return;
 SDL_SetTextureColorMod(font_texture,tint.r,tint.g,tint.b);
 for(unsigned char ch:s){if(ch<' '||ch>126)ch='?';int index=ch-32;SDL_Rect src={(index%16)*20,(index/16)*32,20,32};SDL_Rect dst={x,y,13*scale,21*scale};SDL_RenderCopy(r,font_texture,&src,&dst);x+=12*scale;}
}
static void draw(){
 const SDL_Color base={8,19,32,255},panel={14,31,51,255},blue={54,161,234,255},muted={144,169,197,255},white={235,245,255,255};
 rect(0,0,1280,720,base);rect(0,0,240,720,panel);rect(238,0,2,720,{35,61,85,255});
 rect(24,28,48,48,{12,109,183,255});text(37,39,"*",2);text(83,32,"ORION",2);text(83,60,"STORE PS4",1,muted);
 text(32,126,"EXPLORAR",1,muted);
 for(int i=0;i<6;i++){
   int yy=159+i*58;bool active=i==selected_section;rect(14,yy,210,47,active?SDL_Color{25,70,108,255}:panel);
   if(active){rect(14,yy,4,47,blue);if(!in_cards)border(14,yy,210,47,{80,185,255,255});}
   text(29,yy+12,labels[i],1,active?white:muted);
 }
 rect(16,634,207,1,{41,62,89,255});text(28,651,"DEMO NATIVA 0.1",1,muted);text(28,677,"SEM DOWNLOADS",1,muted);
 text(279,33,"ORION / "+std::string(sections[selected_section]),1,muted);
 rect(1095,27,120,29,{20,51,77,255});text(1110,32,"HOMEBREW",1,{111,197,250,255});
 if(selected_section==0){
   rect(280,90,947,265,{13,55,95,255});rect(800,95,420,255,{9,42,80,255});
   for(int j=0;j<8;j++)border(850+j*18,113+j*9,200-j*20,200-j*20,{32,96,142,255});
   text(314,119,"SUA BIBLIOTECA.",3,white);text(314,191,"SEU UNIVERSO.",3,{110,199,253,255});
   text(319,279,"HOME BREW / JOGOS / UTILITARIOS",1,muted);
 }
 if(selected_section<=2){
  const int top=selected_section==0?390:140;
  text(279,top-37,selected_section==2?"EXPLORE AS CATEGORIAS":"EXPLORE O CATALOGO",2);
  for(int i=0;i<6;i++){
   int x=280+i*158,y=top;SDL_Color c=catalog[i].color;
   rect(x,y,145,180,{16,36,58,255});rect(x+4,y+4,137,124,c);
   for(int j=0;j<5;j++)border(x+19+j*9,y+18+j*8,88-j*10,88-j*10,{static_cast<Uint8>(std::min(255,int(c.r)+45)),static_cast<Uint8>(std::min(255,int(c.g)+45)),static_cast<Uint8>(std::min(255,int(c.b)+45)),255});
   if(in_cards&&selected_card==i)border(x-3,y-3,151,186,blue);
   const std::string name=catalog[i].title; text(x+9,y+139,name.substr(0,11),1);text(x+9,y+159,catalog[i].category,1,muted);
  }
  text(280,631,"X SELECIONAR   O VOLTAR   DIRECIONAL NAVEGAR",1,muted);
 } else {
   text(280,156,selected_section==3?"FILA DE DOWNLOADS":selected_section==4?"APLICATIVOS INSTALADOS":"CONFIGURACOES",2);
   rect(279,216,940,160,panel);
   text(307,259,"RECURSO DEPENDE DE INTEGRACAO NATIVA",1,muted);
   text(307,297,"NENHUMA INSTALACAO FOI EXECUTADA.",1,muted);
 }
 if(details){rect(250,80,990,545,{7,20,37,255});border(250,80,990,545,blue);rect(285,119,323,456,catalog[selected_card].color);text(644,160,catalog[selected_card].title,2);text(644,226,catalog[selected_card].category,1,muted);text(644,283,"CONTEUDO DE DEMONSTRACAO",1);text(644,329,"DOWNLOAD NAO DISPONIVEL",1,{250,190,110,255});text(644,518,"O VOLTAR",1,muted);}
 SDL_RenderPresent(r);
}
static void select(){if(details){details=false;return;}if(in_cards&&selected_section<=2){details=true;}else if(selected_section<=2){in_cards=true;}}
static void move(int dx,int dy){if(details)return;if(in_cards){if(dy!=0){in_cards=false;return;}selected_card=std::max(0,std::min(5,selected_card+dx));}else {if(dx>0&&selected_section<=2){in_cards=true;return;}selected_section=std::max(0,std::min(5,selected_section+dy));}}
int main(int,char**){
 if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_GAMECONTROLLER)!=0){std::fprintf(stderr,"SDL: %s\n",SDL_GetError());return 1;}
 SDL_Window* window=SDL_CreateWindow("Orion Store PS4",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1280,720,SDL_WINDOW_SHOWN);
 if(!window){SDL_Quit();return 2;}r=SDL_CreateRenderer(window,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
 if(!r)r=SDL_CreateRenderer(window,-1,SDL_RENDERER_SOFTWARE);
 if(!r){SDL_DestroyWindow(window);SDL_Quit();return 3;}
 SDL_Surface* f=SDL_LoadBMP("/app0/assets/font.bmp");if(!f)f=SDL_LoadBMP("assets/font.bmp");
 if(f){SDL_SetColorKey(f,SDL_TRUE,SDL_MapRGB(f->format,0,0,0));font_texture=SDL_CreateTextureFromSurface(r,f);SDL_FreeSurface(f);}
 for(int i=0;i<SDL_NumJoysticks();i++)if(SDL_IsGameController(i))SDL_GameControllerOpen(i);
 bool running=true;while(running){SDL_Event e;while(SDL_PollEvent(&e)){if(e.type==SDL_QUIT)running=false;
  if(e.type==SDL_KEYDOWN){switch(e.key.keysym.sym){case SDLK_ESCAPE:case SDLK_BACKSPACE:if(details)details=false;else in_cards=false;break;case SDLK_RETURN:case SDLK_SPACE:select();break;case SDLK_UP:move(0,-1);break;case SDLK_DOWN:move(0,1);break;case SDLK_LEFT:move(-1,0);break;case SDLK_RIGHT:move(1,0);break;}}
  if(e.type==SDL_CONTROLLERBUTTONDOWN){switch(e.cbutton.button){case SDL_CONTROLLER_BUTTON_A:select();break;case SDL_CONTROLLER_BUTTON_B:if(details)details=false;else in_cards=false;break;case SDL_CONTROLLER_BUTTON_DPAD_UP:move(0,-1);break;case SDL_CONTROLLER_BUTTON_DPAD_DOWN:move(0,1);break;case SDL_CONTROLLER_BUTTON_DPAD_LEFT:move(-1,0);break;case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:move(1,0);break;}}
 }draw();SDL_Delay(16);}
 if(font_texture)SDL_DestroyTexture(font_texture);SDL_DestroyRenderer(r);SDL_DestroyWindow(window);SDL_Quit();return 0;
}

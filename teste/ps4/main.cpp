// ORION STORE TESTE: native SDL2 UI prototype for homebrew PS4.
// The program DOES NOT download, install or provide third-party games.
#include <SDL2/SDL.h>
#include <cstdio>
#include "ui_model.hpp"

using namespace orion_teste;
static ViewModel model;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* fontTexture = NULL;
static SDL_Color background = {7,18,33,255};
static SDL_Color panel = {14,32,53,255};
static SDL_Color blue = {95,194,253,255};
static SDL_Color pale = {232,243,255,255};
static SDL_Color muted = {141,164,192,255};
static SDL_Color colors[3] = {{182,101,47,255},{20,126,197,255},{23,166,128,255}};

static void fill(int x,int y,int w,int h,SDL_Color color) {
    SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,color.a);
    SDL_Rect rectangle={x,y,w,h};SDL_RenderFillRect(renderer,&rectangle);
}
static void border(int x,int y,int w,int h,SDL_Color color){
    SDL_SetRenderDrawColor(renderer,color.r,color.g,color.b,color.a);
    SDL_Rect rectangle={x,y,w,h};SDL_RenderDrawRect(renderer,&rectangle);
}
static void label(int x,int y,const char* source,int scale=1,SDL_Color tint={232,243,255,255}) {
    if (!fontTexture) return;
    SDL_SetTextureColorMod(fontTexture,tint.r,tint.g,tint.b);
    for (int i=0;source[i] != '\0';++i) {
        unsigned char c=(unsigned char)source[i]; if(c<32 || c>126)c='?';
        int index=(int)c-32;
        SDL_Rect src={(index%16)*20,(index/16)*32,20,32};
        SDL_Rect dst={x+i*11*scale,y,12*scale,20*scale};
        SDL_RenderCopy(renderer,fontTexture,&src,&dst);
    }
}
static void drawCard(int n,int x,int y,int width) {
    const Game& g=games[n];
    fill(x,y,width,202,panel);fill(x+5,y+5,width-10,140,colors[n]);
    for(int i=0;i<5;i++) border(x+38+i*9,y+25+i*9,width-91-i*18,95-i*13,{178,222,247,255});
    label(x+13,y+151,g.title);
    label(x+13,y+174,g.category,1,muted);
    if (model.cardsFocused && model.selectedCard==n) border(x-4,y-4,width+8,210,blue);
}
static void drawDetails() {
    int n=model.selectedCard; if(n<0||n>=3)return;
    const Game& g=games[n];
    fill(253,87,977,553,{8,29,49,255});border(253,87,977,553,blue);
    fill(282,111,273,492,colors[n]);label(303,555,g.title);
    label(587,124,g.title,2);
    label(588,182,g.category,1,muted);
    label(588,219,g.description,1,muted);
    char langBuffer[100];
    std::snprintf(langBuffer,sizeof(langBuffer),"AUDIO PT-BR: %s",languageLabel(g.audioPtBr)); label(588,270,langBuffer);
    std::snprintf(langBuffer,sizeof(langBuffer),"LEGENDAS PT-BR: %s",languageLabel(g.subtitlesPtBr)); label(588,299,langBuffer);
    std::snprintf(langBuffer,sizeof(langBuffer),"INTERFACE PT-BR: %s",languageLabel(g.interfacePtBr)); label(588,328,langBuffer);
    label(588,378,"OPCOES DE DOWNLOAD",1,blue);
    for(int option=0;option<2;option++){
        int top=411+option*74;bool selected=model.selectedVariant==option;
        fill(581,top,622,61,selected?SDL_Color{28,74,109,255}:panel);
        if(selected)border(581,top,622,61,blue);
        label(591,top+9,g.variants[option].label,1,selected?pale:muted);
        label(591,top+33,g.variants[option].explanation,1,muted);
    }
    label(588,582,"SEM PKG DISPONIVEL - O PARA VOLTAR",1,muted);
}
static void draw() {
    fill(0,0,1280,720,background);fill(0,0,240,720,panel);
    fill(25,30,47,47,{30,126,212,255});label(39,39,"*",2);label(83,28,"ORION",2);
    label(83,60,"STORE TESTE",1,muted);label(23,129,"CATEGORIAS",1,muted);
    for(int i=0;i<6;i++) {
        int y=171+i*61;bool selected=(int)model.section==i;
        fill(12,y,214,49,selected?SDL_Color{25,71,108,255}:panel);
        if(selected){fill(12,y,4,49,blue);if(!model.cardsFocused && model.screen==NAVIGATION)border(12,y,214,49,blue);}
        label(27,y+13,sectionNames[i],1,selected?pale:muted);
    }
    label(24,663,"VERSAO TESTE",1,muted);
    char sectionBuffer[64]; std::snprintf(sectionBuffer,sizeof(sectionBuffer),"ORION / %s",sectionNames[(int)model.section]);
    label(280,43,sectionBuffer,1,muted);
    if(model.section==HOME){
        fill(281,91,935,245,{19,68,108,255});label(317,130,"ORION STORE",3);
        label(318,204,"EDICAO TESTE",2,blue);label(319,281,"3 TITULOS. ZERO ANUNCIOS.",1,muted);
    }
    if(model.section==DOWNLOADS){
        label(282,166,"DOWNLOADS",2);label(282,228,"NENHUM PKG PUBLICADO NESTA EDICAO.",1,muted);
    } else {
        int count=model.visibleCount();int top=model.section==HOME?403:169;
        label(282,top-44,count==1?"SELECIONE O JOGO":"SELECIONE UM DOS TRES JOGOS",count==1?2:1);
        int width=count==1?402:289;
        for(int k=0;k<count;k++){
            int game=model.visibleGameIndex(k);
            int x=281+k*(width+17);
            drawCard(game,x,top,width);
        }
        label(282,641,"X DETALHES / O VOLTAR",1,muted);
    }
    if(model.screen==DETAILS) drawDetails();
    SDL_RenderPresent(renderer);
}
static void handleKey(SDL_Keycode key) {
    switch (key) {
        case SDLK_ESCAPE:case SDLK_BACKSPACE:model.back();break;
        case SDLK_RETURN:case SDLK_SPACE:model.accept();break;
        case SDLK_UP:model.move(0,-1);break;
        case SDLK_DOWN:model.move(0,1);break;
        case SDLK_LEFT:model.move(-1,0);break;
        case SDLK_RIGHT:model.move(1,0);break;
        default:break;
    }
}
static void handleController(Uint8 button) {
    switch(button) {
        case SDL_CONTROLLER_BUTTON_A:model.accept();break;
        case SDL_CONTROLLER_BUTTON_B:model.back();break;
        case SDL_CONTROLLER_BUTTON_DPAD_UP:model.move(0,-1);break;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:model.move(0,1);break;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:model.move(-1,0);break;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:model.move(1,0);break;
        default:break;
    }
}
int main(int, char**) {
    if(SDL_Init(SDL_INIT_VIDEO|SDL_INIT_GAMECONTROLLER)!=0){std::fprintf(stderr,"SDL init failed: %s\n",SDL_GetError());return 1;}
    SDL_Window* win=SDL_CreateWindow("Orion Store Teste",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1280,720,SDL_WINDOW_SHOWN);
    if(!win){SDL_Quit();return 2;}
    renderer=SDL_CreateRenderer(win,-1,SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
    if(!renderer)renderer=SDL_CreateRenderer(win,-1,SDL_RENDERER_SOFTWARE);
    if(!renderer){SDL_DestroyWindow(win);SDL_Quit();return 3;}
    SDL_RenderSetLogicalSize(renderer,1280,720);
    SDL_Surface* bitmap=SDL_LoadBMP("/app0/assets/font.bmp");
    if(!bitmap)bitmap=SDL_LoadBMP("assets/font.bmp");
    if(bitmap){SDL_SetColorKey(bitmap,SDL_TRUE,SDL_MapRGB(bitmap->format,0,0,0));fontTexture=SDL_CreateTextureFromSurface(renderer,bitmap);SDL_FreeSurface(bitmap);}
    if (!fontTexture) std::fprintf(stderr,"Font bitmap not loaded: %s\n",SDL_GetError());
    for(int k=0;k<SDL_NumJoysticks();k++)if(SDL_IsGameController(k))SDL_GameControllerOpen(k);
    bool running=true;
    while(running){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type==SDL_QUIT)running=false;
            else if(event.type==SDL_KEYDOWN)handleKey(event.key.keysym.sym);
            else if(event.type==SDL_CONTROLLERBUTTONDOWN)handleController(event.cbutton.button);
        }
        draw();SDL_Delay(16);
    }
    if(fontTexture)SDL_DestroyTexture(fontTexture);
    SDL_DestroyRenderer(renderer);SDL_DestroyWindow(win);SDL_Quit();return 0;
}

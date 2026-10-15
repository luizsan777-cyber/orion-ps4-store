#ifndef ORION_SDL_TEST_STUB_H
#define ORION_SDL_TEST_STUB_H
// Compile-only stub; NOT a real SDL library, not included in the PS4 package.
typedef unsigned char Uint8;
typedef int SDL_Keycode;
typedef struct SDL_Window SDL_Window;
typedef struct SDL_Renderer SDL_Renderer;
typedef struct SDL_Texture SDL_Texture;
typedef struct SDL_PixelFormat SDL_PixelFormat;
typedef struct SDL_Surface { SDL_PixelFormat* format; } SDL_Surface;
typedef struct SDL_Rect {int x,y,w,h;} SDL_Rect;
typedef struct SDL_Color {Uint8 r,g,b,a;} SDL_Color;
typedef struct SDL_KeyboardEvent {struct {SDL_Keycode sym;} keysym;} SDL_KeyboardEvent;
typedef struct SDL_ControllerButtonEvent {Uint8 button;} SDL_ControllerButtonEvent;
typedef struct SDL_Event {int type;SDL_KeyboardEvent key;SDL_ControllerButtonEvent cbutton;} SDL_Event;
#define SDL_INIT_VIDEO 0x1
#define SDL_INIT_GAMECONTROLLER 0x2
#define SDL_WINDOWPOS_CENTERED 0
#define SDL_WINDOW_SHOWN 0x4
#define SDL_RENDERER_ACCELERATED 0x1
#define SDL_RENDERER_PRESENTVSYNC 0x2
#define SDL_RENDERER_SOFTWARE 0x4
#define SDL_TRUE 1
#define SDL_QUIT 0x100
#define SDL_KEYDOWN 0x300
#define SDL_CONTROLLERBUTTONDOWN 0x650
#define SDLK_ESCAPE 27
#define SDLK_BACKSPACE 8
#define SDLK_RETURN 13
#define SDLK_SPACE 32
#define SDLK_UP 1001
#define SDLK_DOWN 1002
#define SDLK_LEFT 1003
#define SDLK_RIGHT 1004
#define SDL_CONTROLLER_BUTTON_A 0
#define SDL_CONTROLLER_BUTTON_B 1
#define SDL_CONTROLLER_BUTTON_DPAD_UP 11
#define SDL_CONTROLLER_BUTTON_DPAD_DOWN 12
#define SDL_CONTROLLER_BUTTON_DPAD_LEFT 13
#define SDL_CONTROLLER_BUTTON_DPAD_RIGHT 14
int SDL_Init(unsigned int);
const char* SDL_GetError(void);
SDL_Window* SDL_CreateWindow(const char*, int,int,int,int,unsigned int);
SDL_Renderer* SDL_CreateRenderer(SDL_Window*,int,unsigned int);
int SDL_RenderSetLogicalSize(SDL_Renderer*,int,int);
int SDL_SetRenderDrawColor(SDL_Renderer*,Uint8,Uint8,Uint8,Uint8);
int SDL_RenderFillRect(SDL_Renderer*,const SDL_Rect*);
int SDL_RenderDrawRect(SDL_Renderer*,const SDL_Rect*);
int SDL_SetTextureColorMod(SDL_Texture*,Uint8,Uint8,Uint8);
int SDL_RenderCopy(SDL_Renderer*,SDL_Texture*,const SDL_Rect*,const SDL_Rect*);
void SDL_RenderPresent(SDL_Renderer*);
SDL_Surface* SDL_LoadBMP(const char*);
int SDL_SetColorKey(SDL_Surface*,int,unsigned int);
unsigned int SDL_MapRGB(SDL_PixelFormat*,Uint8,Uint8,Uint8);
SDL_Texture* SDL_CreateTextureFromSurface(SDL_Renderer*,SDL_Surface*);
void SDL_FreeSurface(SDL_Surface*);
int SDL_NumJoysticks(void);
int SDL_IsGameController(int);
void* SDL_GameControllerOpen(int);
void SDL_Delay(unsigned int);
int SDL_PollEvent(SDL_Event*);
void SDL_DestroyTexture(SDL_Texture*);
void SDL_DestroyRenderer(SDL_Renderer*);
void SDL_DestroyWindow(SDL_Window*);
void SDL_Quit(void);
#endif

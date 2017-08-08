#include <iostream>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
using namespace std;

bool flag1 = false;
bool flag2 = false;
bool flag3 = false;
SDL_Window* window = NULL;
SDL_Renderer* render = NULL;
SDL_Texture* background = NULL;
SDL_Texture* weapon = NULL;
SDL_Texture* target = NULL;
SDL_Texture* target2 = NULL;
SDL_Texture* enemy = NULL;
SDL_Texture* damage = NULL;
SDL_Texture* win = NULL;
SDL_Rect damageRect = { 0, 0, 41, 34 };
SDL_Rect dw_rect = { 500, 490, 250,150 };
SDL_Rect sw_rect1 = { 0, 0, 174, 97 };
SDL_Rect sw_rect2 = { 175, 0, 174, 97 };
SDL_Rect dt_rect = { 0, 0, 30, 30 };
SDL_Rect se_rect1 = { 0, 5, 46, 36 };		SDL_Rect de_rect1 = {10,20,46,36};
SDL_Rect se_rect2 = { 248, 1, 36, 42 };		SDL_Rect de_rect2 = {600,80,36,42};
SDL_Rect se_rect3 = { 347, 0, 62, 44 };		SDL_Rect de_rect3 = {1100,140,62,44};

Mix_Chunk* start;
Mix_Chunk* shoots;
Mix_Music* music;

void init();
void loadGraphics();
bool result();
void close();
void createEnemy();
SDL_Texture* loadTexture(string path);

void init(){
	SDL_Init(SDL_INIT_EVERYTHING);
	window = SDL_CreateWindow("SHOOT IT", 30, 30, 1200, 600, SDL_WINDOW_SHOWN);
	render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	SDL_SetRenderDrawColor(render, 0xFF, 0xFF, 0xFF, 0xFF);

	//Initialize PNG loading
	int imgFlags = IMG_INIT_PNG;
	IMG_Init(imgFlags) & imgFlags;

	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}

SDL_Texture* loadTexture(string path){
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load(path.c_str());
	
	//Create texture from surface pixels
	newTexture = SDL_CreateTextureFromSurface(render, loadedSurface);
	
	//Get rid of old loaded surface
	SDL_FreeSurface(loadedSurface);
	
	return newTexture;
}

void loadGraphics(){
	background = loadTexture("assets/bk2.png");
	weapon = loadTexture("assets/weapon.png");
	target = loadTexture("assets/target.png");
	target2 = loadTexture("assets/target2.png");
	enemy = loadTexture("assets/enemy.png");
	damage = loadTexture("assets/damage.png");
	shoots = Mix_LoadWAV("assets/shoot.wav");
	start = Mix_LoadWAV("assets/start.wav");
	music = Mix_LoadMUS("assets/music.mp3");
	win = loadTexture("assets/win.png");
}

void createEnemy(){
	int x1, y1, x2, y2, x3, y3;

	x1 = de_rect1.x;	y1 = de_rect1.y;
	x2 = de_rect2.x;	y2 = de_rect2.y;
	x3 = de_rect3.x;	y3 = de_rect3.y;

	if (x1 >= 1150)
		flag1 = true;
	if (x1 <= 0)
		flag1 = false;
	if (x2 >= 1150)
		flag2 = true;
	if (x2 <= 0)
		flag2 = false;
	if (x3 >= 1150)
		flag3 = true;
	if (x3 <= 0)
		flag3 = false;
	if (flag1)
		x1--;
	else
		x1++;
	if (flag2)
		x2 -= 2;
	else
		x2 += 2;
	if (flag3)
		x3--;
	else
		x3++;
	de_rect1.x = x1;	de_rect1.y = y1;
	de_rect2.x = x2;	de_rect2.y = y2;
	de_rect3.x = x3;	de_rect3.y = y3;

}

bool result(){
	if ((se_rect1.w == 0) && (se_rect2.w == 0) && (se_rect3.w == 0)){
		SDL_RenderCopy(render, win, NULL, NULL);
		return true;
	}
	return false;
}

void close(){
	SDL_DestroyWindow(window);
	window = NULL;
	SDL_DestroyRenderer(render);
	render = NULL;
	SDL_DestroyTexture(background);
	background = NULL;
	SDL_DestroyTexture(weapon);
	weapon = NULL;
	SDL_DestroyTexture(target);
	target = NULL;
	Mix_FreeChunk(shoots);
	Mix_FreeChunk(start);
	Mix_FreeMusic(music);
	music = NULL;

	Mix_Quit();
	IMG_Quit();
	SDL_Quit();
}
int main(int a, char* b[]){
	init();
	loadGraphics();
	SDL_Event eve;
	bool quit = false;
	bool targetspotted = false;
	bool shoot = false;
	int destroy = 0;
	Mix_PlayMusic(music, 1);
	while (!quit){
		createEnemy();
		//Clear screen
		SDL_RenderClear(render);
		SDL_SetTextureAlphaMod(background, 200);
		//Render texture to screen
		SDL_RenderCopy(render, background, NULL, NULL);
		damageRect = { 0, 0, 41, 34 };

		SDL_RenderCopyEx(render, enemy, &se_rect1, &de_rect1, 0, nullptr, SDL_FLIP_VERTICAL);
		SDL_RenderCopyEx(render, enemy, &se_rect2, &de_rect2, 0, nullptr, SDL_FLIP_VERTICAL);
		SDL_RenderCopyEx(render, enemy, &se_rect3, &de_rect3, 0, nullptr, SDL_FLIP_VERTICAL);
		
		while (SDL_PollEvent(&eve)){
			if (eve.type == SDL_QUIT)
				quit = true;
			if (eve.key.keysym.sym == SDLK_ESCAPE)
				quit = true;
			if (eve.type == SDL_MOUSEBUTTONDOWN){
				shoot = true;
				if (((dt_rect.x + 5) > de_rect1.x) && ((dt_rect.x - 5) < (de_rect1.x + de_rect1.w)) && ((dt_rect.y + 5)>de_rect1.y) && ((dt_rect.y - 5) < (de_rect1.y + de_rect1.h)))
					destroy = 1;
				if (((dt_rect.x + 5) > de_rect2.x) && ((dt_rect.x - 5) < (de_rect2.x + de_rect2.w)) && ((dt_rect.y + 5)>de_rect2.y) && ((dt_rect.y - 5) < (de_rect2.y + de_rect2.h)))
					destroy = 2;
				if (((dt_rect.x + 5) > de_rect3.x) && ((dt_rect.x - 5) < (de_rect3.x + de_rect3.w)) && ((dt_rect.y + 5)>de_rect3.y) && ((dt_rect.y - 5) < (de_rect3.y + de_rect3.h)))
					destroy = 3;
			}
			else
				shoot = false;
			if (eve.type == SDL_MOUSEMOTION){
				int x, y;
				SDL_GetMouseState(&x, &y);
				dt_rect.x = x - 14;
				dt_rect.y = y - 14;
				dw_rect.x = x - 124;
			}
			if ((((dt_rect.x + 5) > de_rect1.x) && ((dt_rect.x - 5) < (de_rect1.x + de_rect1.w)) && ((dt_rect.y + 5)>de_rect1.y) && ((dt_rect.y - 5) < (de_rect1.y + de_rect1.h))) || (((dt_rect.x + 5) > de_rect2.x) && ((dt_rect.x - 5) < (de_rect2.x + de_rect2.w)) && ((dt_rect.y + 5)>de_rect2.y) && ((dt_rect.y - 5) < (de_rect2.y + de_rect2.h))) || (((dt_rect.x + 5) > de_rect3.x) && ((dt_rect.x - 5) < (de_rect3.x + de_rect3.w)) && ((dt_rect.y + 5)>de_rect3.y) && ((dt_rect.y - 5) < (de_rect3.y + de_rect3.h))))
				targetspotted = true;
			else
				targetspotted = false;
		}
		if (!shoot){
			SDL_RenderCopy(render, weapon, &sw_rect1, &dw_rect);
		}
		else{
			SDL_RenderCopy(render, weapon, &sw_rect2, &dw_rect);
			Mix_PlayChannel(-1, shoots, 0);
		}
		if (destroy == 1){
			static int count1 = 0;
			SDL_RenderCopyEx(render, enemy, &se_rect1, &de_rect1, 0, nullptr, SDL_FLIP_VERTICAL);
			SDL_RenderCopy(render, damage, &damageRect, &de_rect1);
			count1++;
			if (count1 > 50){
				damageRect = {0,0,0,0};
				se_rect1 = { 0, 0, 0, 0 };
				count1 = 0;
				destroy = 0;
			}
		}
		if (destroy == 2){
			static int count2 = 0;
			SDL_RenderCopyEx(render, enemy, &se_rect2, &de_rect2, 0, nullptr, SDL_FLIP_VERTICAL);
			SDL_RenderCopy(render, damage, &damageRect, &de_rect2);
			count2++;
			
			if (count2 > 50){
				se_rect2 = { 0, 0, 0, 0 };
				damageRect = { 0, 0, 0, 0 };
				count2 = 0;
				destroy = 0;
			}
		}
		if (destroy == 3){
			static int count3 = 0;
			SDL_RenderCopyEx(render, enemy, &se_rect3, &de_rect3, 0, nullptr, SDL_FLIP_VERTICAL);
			SDL_RenderCopy(render, damage, &damageRect, &de_rect3);
			count3++;
			if (count3 > 50){
				se_rect3 = { 0, 0, 0, 0 };
				damageRect = { 0, 0, 0, 0 };
				count3 = 0;
				destroy = 0;
			}
		}
		int win = result();
		if (targetspotted)
			SDL_RenderCopy(render, target, NULL, &dt_rect);
		else
			SDL_RenderCopy(render, target2, NULL, &dt_rect);
		//Update screen
		SDL_RenderPresent(render);
		if (win){
			SDL_Delay(2500);
			break;
		}
	}
	close();
	return 0;
}
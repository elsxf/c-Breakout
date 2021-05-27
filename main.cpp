/*made with extensive help from https://lazyfoo.net/. 
thi was made as an exercise is btoh c++ and sdl.*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_events.h>
#include <stdio.h>
#include <math.h>
#include <string>
#include <vector>

#define PI 3.1415926

//Screen dimension constants
const int SCREEN_WIDTH = 1000;
const int SCREEN_HEIGHT = 1000;

SDL_Event event;

const Uint8* state = SDL_GetKeyboardState(NULL);

class Block{
	public:
		int width=75;
		int height=25;
		int x;
		int y;
	Block(int x, int y){
			this->x=x;
			this->y=y;
	}

	
};

std::vector<Block*> allBlocks;

void removeElement(Block* block){
	for(int i = 0; i<allBlocks.size(); i++){
		if(allBlocks[i]==block){
			delete block;
			allBlocks.erase(allBlocks.begin()+i);
			return;
		}
	}
}

int paddleX = 450;
int paddleWidth = 100;
int paddleHeight = 50;

class Ball{
    public:
        int x;
        int y;
        int rotation;
        int speed;
    void move(){
		if(rotation%90==15){//keeps rotation diagonal-ish
			//printf("absolute rotation");
				fflush(stdout);
				rotation+=10;
		}
		if(y+26>SCREEN_HEIGHT-paddleHeight){//if below paddleheihgt
			if(x<paddleX+paddleWidth&&x+26>paddleX){//if in between paddle bounds
				bounceY();
				//change agnle/speed code

				if(state[SDL_SCANCODE_LEFT]){
					rotation-=10;
				}
				if(state[SDL_SCANCODE_RIGHT]){
					rotation+=10;
				}
        		y=std::min(y+(int)sin(rotation*PI/180)*speed,SCREEN_HEIGHT-paddleHeight-26);//push ball out of panel		
			}
		}

        if(x<=0||x+26>=SCREEN_WIDTH){
            bounceX();
			x+(int)cos(rotation*PI/180)*speed;
        }
        if(y<=0||y+26>=SCREEN_HEIGHT){
            bounceY();
			y+(int)sin(rotation*PI/180)*speed;
        }
		for(Block* block: allBlocks){
			if(block->x<=x+26&&block->x+block->width>=x){
				if(block->y<=y+26&&block->y+block->height>=y){
					int x1 = abs((block->width+block->x)-x);
					int x2 = abs(block->x-(x+26));
					int y1 = abs((block->height+block->y)-y);
					int y2 = abs(block->y-(y+26));
					//printf("\nx1: %d, x2: %d, y1: %d, y2: %d\n", x1,x2,y1,y2);
					fflush(stdout);
					int trueX = std::min(x1,x2);
					int trueY = std::min(y1,y2);
					if(trueX<trueY){
						bounceX();
						//printf("bouncex");
					}
					else if(trueY<trueX){
						bounceY();
						//printf("bouncey");
					}
					else{
						bounceY();
						bounceX();
						//printf("bounce both");
					}
					removeElement(block);
				}
			}

		}
		x+=cos(rotation*PI/180)*speed;
        y+=sin(rotation*PI/180)*speed;
    }
    void bounceX(){
        rotation=(540-rotation)%360;
    }
    void bounceY(){
		rotation=(360-rotation);
    }
    
};


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;
	
//The surface contained by the window
SDL_Surface* gScreenSurface = NULL;

//The image we will load and show on the screen
SDL_Surface* gHelloWorld = NULL;

bool init()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
		success = false;
	}
	else
	{
		//Create window
		gWindow = SDL_CreateWindow( "SDL test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
			success = false;
		}
		else
		{
			//Get window surface
			gScreenSurface = SDL_GetWindowSurface( gWindow );
		}
	}

	return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load splash image
	gHelloWorld = SDL_LoadBMP( "ball.bmp" );
	if( gHelloWorld == NULL )
	{
		printf( "Unable to load image %s! SDL Error: %s\n", "02_getting_an_image_on_the_screen/hello_world.bmp", SDL_GetError() );
		success = false;
	}

	return success;
}

void close()
{
	//Deallocate surface
	SDL_FreeSurface( gHelloWorld );
	gHelloWorld = NULL;

	//Destroy window
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	for(int i = 0; i<10; i++){
		for(int j = 0; j<10; j++){
			allBlocks.push_back(new Block(i*100+10,j*50+110));
		}
	}
	fflush(stdout);


    Ball b; 
    b.x=500;
    b.y=800;
    b.speed=10;
    b.rotation=180+45;

	SDL_Rect coords;

	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{
			bool quit = false;
            while(!quit){


			    SDL_FillRect( gScreenSurface, NULL, SDL_MapRGB( gScreenSurface->format, 0x00, 0x00, 0x00) );

                //Apply the image
                coords.x=b.x;
                coords.y=b.y;
                b.move();

				SDL_BlitSurface(gHelloWorld, NULL, gScreenSurface, &coords);//draw ball
                

				for(Block* block : allBlocks){//draw blocks
					coords.x=block->x;
					coords.y=block->y;
					coords.h=block->height;
					coords.w=block->width;
					SDL_FillRect( gScreenSurface, &coords, SDL_MapRGB( gScreenSurface->format, 0x10, 0xf0, 0x10) );
				}

				coords.x=paddleX;
				coords.y=SCREEN_HEIGHT-paddleHeight;
				coords.h=paddleHeight;
				coords.w=paddleWidth;

				SDL_FillRect( gScreenSurface, &coords, SDL_MapRGB( gScreenSurface->format, 0xff, 0xff, 0xff) );

				while( SDL_PollEvent(&event)){
					if(event.type == SDL_QUIT){
						quit = true;
						break;
					}
				}

				if(state[SDL_SCANCODE_LEFT]){
					paddleX = std::max(0,paddleX-15);//move paddle left
				}
				if(state[SDL_SCANCODE_RIGHT]){
					paddleX = std::min(paddleX+15,SCREEN_WIDTH-paddleWidth);//move paddle right
				}
                
                //Update the surface
                SDL_UpdateWindowSurface( gWindow );

                //Wait two seconds
                SDL_Delay( 20 );
            }
			close();
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}
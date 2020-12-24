#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <stdbool.h>

#define window_width 800
#define window_height 600
#define FPS 120 //determine the fps that we want
#define target_time 1000/FPS //the time for every frame

struct objects
{
    float x ;
    float y ;
    float width ;
    float height ;
    float x_vlc ;
    float y_vlc
}paddle, ball;

SDL_Window *window = NULL ;
SDL_Renderer *renderer = NULL ;
int game_is_running = false ;
int last_frame_time = 0 ;


int window_initialize();
void destroy_window();
void setup();
void process_input();
void update();
void render_window();


int main(int argc , char* argv[])
{
    game_is_running = window_initialize();
    setup();

    while (game_is_running)
    {
        process_input();
        update();
        render_window();
    }

    destroy_window() ;
    return 0 ;

}

int window_initialize()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        fprintf(stderr , "Error in SDL initializing \n") ;
        return false ;
    }
    SDL_Init(SDL_INIT_EVERYTHING) ;

    window = SDL_CreateWindow("" , SDL_WINDOWPOS_CENTERED , SDL_WINDOWPOS_CENTERED , window_width , window_height , SDL_WINDOW_OPENGL) ;
    //(name , x pos , w pos , width , height , flags(may be borderless))
    if (!(window))
    {
        fprintf(stderr , "Error in making window\n") ;
        return false ;
    }
    renderer = SDL_CreateRenderer(window , -1 , 0) ;
    if(!(renderer))
    {
        fprintf(stderr , "Error in creating renderer\n");
        return false ;
    }
    return true ;
}

void setup()
{
    // Initialize the ball object moving down at a constant velocity
    ball.x = 10 ;
    ball.y = 10 ;
    ball.height = 15 ;
    ball.width = 15 ;
    ball.x_vlc = 300 ;
    ball.y_vlc = 300 ;
    // Initialize the paddle position at the bottom of the screen
    paddle.width = 100;
    paddle.height = 20;
    paddle.x = (window_width / 2) - (paddle.width / 2);
    paddle.y = window_height - 40;
    paddle.x_vlc = 0;
    paddle.y_vlc = 0;
}

void process_input()
{
    SDL_Event event ;
    SDL_PollEvent(&event);

    switch (event.type) {
        case SDL_QUIT:  //the game will quit if we pressed escape or from the X button
            game_is_running = false ;
            break;
        case SDL_KEYDOWN:  //moving the paddle when we press the arrows
            if (event.key.keysym.sym == SDLK_ESCAPE) {
                game_is_running = false ;
            }
            else if (event.key.keysym.sym == SDLK_RIGHT) {
                paddle.x_vlc = 300 ;
            }
            else if (event.key.keysym.sym == SDLK_LEFT) {
                paddle.x_vlc = -300 ;
            }
            if (event.key.keysym.sym == SDLK_UP) {
                paddle.y_vlc = -300 ;
            }
            else if (event.key.keysym.sym == SDLK_DOWN) {
                paddle.y_vlc = 300 ;
            }
            break;
        case SDL_KEYUP://stoping the paddle when we leaving the arrows
            if (event.key.keysym.sym == SDLK_RIGHT) {
                paddle.x_vlc = 0 ;
            }
            else if (event.key.keysym.sym == SDLK_LEFT) {
                paddle.x_vlc = 0 ;
            }
            if (event.key.keysym.sym == SDLK_UP) {
                paddle.y_vlc = 0 ;
            }
            else if (event.key.keysym.sym == SDLK_DOWN) {
                paddle.y_vlc = 0 ;
            }
            break;
        }
    }


void update()
{
    // Calculate how much we have to wait until next frame
    SDL_Delay(target_time) ;
    float delta_time = (SDL_GetTicks() - last_frame_time) / 1000.0f ; //calculate the time to use it for velocity
    last_frame_time = SDL_GetTicks() ;

    ball.x += ball.x_vlc * delta_time ;
    ball.y += ball.y_vlc * delta_time ;

    paddle.x += paddle.x_vlc * delta_time ;
    paddle.y += paddle.y_vlc * delta_time ;

    //Check for ball collision with the walls
    if(ball.x + ball.width >= window_width || ball.x <= 0.0)
    {
        ball.x_vlc = ball.x_vlc * -1 ;
    }
    if(ball.y  <= 0.0 )
    {
        ball.y_vlc = ball.y_vlc * -1 ;
    }

    //Check for ball collision with the paddle
    if(ball.x >= paddle.x && ball.x + ball.width<= paddle.x + paddle.width && ball.y + ball.height >= paddle.y && ball.y <= paddle.y + paddle.height)
    {
        ball.y_vlc = ball.y_vlc * -1 ;
    }

    //Prevent paddle from moving outside the boundaries of the window
    if (paddle.x + paddle.width >= window_width)
    {
        paddle.x = window_width - paddle.width ;
    }
    else if (paddle.x <= 0.0)
    {
        paddle.x = 0 ;
    }
    if (paddle.y + paddle.height >= window_height)
    {
        paddle.y = window_height - paddle.height ;
    }
    else if (paddle.y <= 0.0)
    {
        paddle.y = 0 ;
    }

    //Check for re-spawning the ball when it hits the bottom part of the screen
    if(ball.y + ball.height >= window_height)
    {
        ball.x = window_width/2;
        ball.y = 0 ;
    }
}

void render_window() //the renderer to draw the background , paddle and the ball
{
    SDL_SetRenderDrawColor(renderer , 255 , 255 , 255 , 255) ;
    SDL_RenderClear(renderer) ;

    SDL_Rect ball_shape = {ball.x , ball.y , ball.height , ball.width} ;
    SDL_SetRenderDrawColor(renderer, 0 , 0 , 0 , 255) ;
    SDL_RenderFillRect(renderer , &ball_shape) ;

    SDL_Rect paddle_shape = {paddle.x , paddle.y , paddle.width , paddle.height} ;
    SDL_SetRenderDrawColor(renderer, 0 , 0 , 0 , 255) ;
    SDL_RenderFillRect(renderer , &paddle_shape) ;

    SDL_RenderPresent(renderer) ;
}

void destroy_window() //destroy everything after finishing
{
    SDL_DestroyRenderer(renderer) ;
    SDL_DestroyWindow(window) ;
    SDL_Quit() ;
}

#include "common.h"
#include "cmath"

bool Init();
void CleanUp();
void Run();
void RenderRect(double x, double y, int sizeW, int sizeH, Uint32 pointColor);
void SpawnPoints();
void RenderPoints();
void Scan();
void Draw();

SDL_Window *window;
SDL_GLContext glContext;
SDL_Surface *gScreenSurface = nullptr;
SDL_Renderer *renderer = nullptr;

int screenWidth = 640;
int screenHeight = 480;
int squareWidth = 10;
int squareHeight = 10;
int numPoints = 100;
int resolution = 1;
unsigned int pointColor = 0xff0000;

vector<double> pX;
vector<double> pY;

bool Init()
{
    if (SDL_Init(SDL_INIT_NOPARACHUTE & SDL_INIT_EVERYTHING) != 0)
    {
        SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
        return false;
    }
    else
    {
        //Specify OpenGL Version (4.2)
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
        SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
        SDL_Log("SDL Initialised");
    }

    //Create Window Instance
    window = SDL_CreateWindow(
        "Game Engine",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        screenWidth,
        screenHeight,   
        SDL_WINDOW_OPENGL);

    //Check that the window was succesfully created
    if (window == NULL)
    {
        //Print error, if null
        printf("Could not create window: %s\n", SDL_GetError());
        return false;
    }
    else{
        gScreenSurface = SDL_GetWindowSurface(window);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        SDL_Log("Window Successful Generated");
    }
    //Map OpenGL Context to Window
    glContext = SDL_GL_CreateContext(window);

    return true;
}

int main()
{
    //Error Checking/Initialisation
    if (!Init())
    {
        printf("Failed to Initialize");
        return -1;
    }

    // Clear buffer with black background
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    //Swap Render Buffers
    SDL_GL_SwapWindow(window);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    Run();

    CleanUp();
    return 0;
}

void CleanUp()
{
    //Free up resources
    SDL_GL_DeleteContext(glContext);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void Run()
{
    bool gameLoop = true;

    SpawnPoints();
    while (gameLoop)
    {   
        Scan();
        SDL_RenderPresent(renderer);

        while(gameLoop){      
            SDL_Event event;
            while (SDL_PollEvent(&event))
            {
                if (event.type == SDL_QUIT)
                {
                    gameLoop = false;
                }
                if (event.type == SDL_KEYDOWN)
                {
                    switch (event.key.keysym.sym)
                    {
                    case SDLK_ESCAPE:
                        gameLoop = false;
                        break;
                    default:
                        break;
                    }
                }

                if (event.type == SDL_KEYUP)
                {
                    switch (event.key.keysym.sym)
                    {
                    default:
                        break;
                    }
                }
            }
        }
    }
}

void Scan(){
    double minDistance;
    double distance;
    vector<double> colors;

    for(int x = 0; x <= screenWidth; x++){
        for(int y = 0; y <= screenHeight; y++){
            minDistance = sqrt(((pX[0] - x) * (pX[0] - x)) + ((pY[0] - y) * (pY[0] - y)));
            for(int index = 0; index < numPoints; index++){
                distance = sqrt(((pX[index] - x) * (pX[index] - x)) + ((pY[index] - y) * (pY[index] - y)));
                if(distance < minDistance)
                    minDistance = distance;
            }
            double thresh = 50;
            double color;
            if(minDistance > thresh)
                color = 0;
            else
                color = (-1.0/thresh * minDistance + 1) * 255;

            SDL_Rect pos;
            pos.x = x;
            pos.y = y;
            pos.w = resolution;
            pos.h = resolution;
            SDL_SetRenderDrawColor(renderer, color, color, color, 255);
            SDL_RenderFillRect(renderer, &pos);
        }
    }
}

void SpawnPoints(){
    for(int i = 0; i < numPoints; i++){
        int x = rand() % (screenWidth - 1) + 1;
        int y = rand() % (screenHeight - 1) + 1;
        pX.push_back(static_cast<double>(x));
        pY.push_back(static_cast<double>(y));
    }
}
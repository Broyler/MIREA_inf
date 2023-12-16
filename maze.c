#include <stdio.h>
#include <stdbool.h>
#ifdef _WIN32
#define IS_LINUX 0
#define SDL_MAIN_HANDLED
#include <SDL.h>
#include <SDL_ttf.h>
#endif

/*

Compilation:
Requires SDL2, SDL2_ttf

Windows:
gcc -o maze.exe maze.c -IC:\w64devkit\include\SDL2 -LC:\w64devkit\lib -w -Wl,-subsystem,windows -lmingw32 -lSDL2_ttf -lSDL2 -O3 -march=native -mtune=native

Linux:
gcc -o maze.exe maze.c -w -lSDL2_ttf -lSDL2 -O3 -march=native -mtune=native

*/

#ifdef linux
#define IS_LINUX 1
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#endif

#define WINDOWS_DEBUG 0
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 800
#define CELLS 10
#define LINE_WIDTH (int)(SCREEN_WIDTH / 80)
#define LINE_HEIGHT (int)(SCREEN_HEIGHT / 80)
#define WPAD (int)(SCREEN_WIDTH / 10)
#define HPAD (int)(SCREEN_HEIGHT / 10)
#define UWIDTH (int)(SCREEN_WIDTH - 2 * WPAD)
#define UHEIGHT (int)(SCREEN_HEIGHT - 2 * HPAD)
#define CWIDTH (int)(UWIDTH / 10)
#define CHEIGHT (int)(UHEIGHT / CELLS)
#define PLAYER_WIDTH (int)(2 * LINE_WIDTH)
#define PLAYER_HEIGHT (int)(2 * LINE_HEIGHT)
#define TELEPORT_THRESHOLD 9.0

#define WALL(x, y, w, h, renderer) (SDL_RenderFillRect(renderer, &(SDL_Rect){x, y, w, h}))

double playerX = WPAD + 4 * CHEIGHT + 2 * LINE_WIDTH;
double playerY = HPAD + 2 * LINE_HEIGHT;
double velX = 0;
double velY = 0;
double inpX = 0;
double inpY = 0;
double acceleration = 10.0f;
double deceleration = 15.5f;
double maxVel = 4.0f;
uint64_t lastFrameTime = 0;
double deltaTime = 0;
bool isGameWon = false;

const int wallsCount = 58;
const walls[58][4] = {
    {WPAD, HPAD, LINE_WIDTH, UHEIGHT},
    {WPAD, HPAD, 4 * CWIDTH, LINE_HEIGHT},
    {WPAD + 5 * CWIDTH, HPAD, 5 * CWIDTH, LINE_HEIGHT},
    {WPAD + 10 * CWIDTH, HPAD, LINE_WIDTH, UHEIGHT},
    {WPAD, HPAD + 10 * CHEIGHT, 5 * CWIDTH, LINE_HEIGHT},
    {WPAD + 6 * CWIDTH, HPAD + 10 * CHEIGHT, 4 * CWIDTH + LINE_WIDTH, LINE_HEIGHT},
    {WPAD, HPAD + CHEIGHT, 3 * CWIDTH, LINE_HEIGHT},
    {WPAD + 3 * CWIDTH, HPAD + CHEIGHT, LINE_WIDTH, CHEIGHT},
    {WPAD + 4 * CWIDTH, HPAD, LINE_WIDTH, CHEIGHT},
    {WPAD + 6 * CWIDTH, HPAD, LINE_WIDTH, CHEIGHT},
    {WPAD + 6 * CWIDTH, HPAD + CHEIGHT, CWIDTH, LINE_HEIGHT},
    {WPAD + 5 * CWIDTH, HPAD + CHEIGHT, LINE_WIDTH, CHEIGHT},
    {WPAD + 4 * CWIDTH, HPAD + 2 * CHEIGHT, CWIDTH + LINE_WIDTH, LINE_HEIGHT},
    {WPAD + 4 * CWIDTH, HPAD + 2 * CHEIGHT, LINE_WIDTH, CHEIGHT},
    {WPAD + 2 * CWIDTH, HPAD + 3 * CHEIGHT, 2 * CWIDTH + LINE_WIDTH, LINE_HEIGHT},
    {WPAD + CWIDTH, HPAD + 2 * CHEIGHT, CWIDTH, LINE_HEIGHT},
    {WPAD + 2 * CWIDTH, HPAD + 2 * CHEIGHT, LINE_WIDTH, CHEIGHT},
    {WPAD + CWIDTH, HPAD + 2 * CHEIGHT, LINE_WIDTH, 3 * CHEIGHT},
    {WPAD + CWIDTH, HPAD + 4 * CHEIGHT, 4 * CWIDTH, LINE_HEIGHT},
    {WPAD + 5 * CWIDTH, HPAD + 3 * CHEIGHT, LINE_WIDTH, 2 * CHEIGHT},
    {WPAD + 4 * CWIDTH, HPAD + 4 * CHEIGHT, LINE_WIDTH, 2 * CHEIGHT},
    {WPAD + 4 * CWIDTH, HPAD + 6 * CHEIGHT, CWIDTH, LINE_HEIGHT},
    {WPAD + 5 * CWIDTH, HPAD + 6 * CHEIGHT, LINE_WIDTH, 2 * CHEIGHT},
    {WPAD + 5 * CWIDTH, HPAD + 9 * CHEIGHT, LINE_WIDTH, CHEIGHT + LINE_HEIGHT},
    {WPAD + 4 * CWIDTH, HPAD + 7 * CHEIGHT, LINE_WIDTH, 2 * CHEIGHT},
    {WPAD + 3 * CWIDTH, HPAD + 7 * CHEIGHT, CWIDTH, LINE_HEIGHT},
    {WPAD + 3 * CWIDTH, HPAD + 7 * CHEIGHT, LINE_WIDTH, CHEIGHT},
    {WPAD + 4 * CWIDTH, HPAD + 9 * CHEIGHT, CWIDTH, LINE_HEIGHT},
    {WPAD + 2 * CWIDTH, HPAD + 6 * CHEIGHT, LINE_WIDTH, 4 * CHEIGHT},
    {WPAD + CWIDTH, HPAD + 6 * CHEIGHT, CWIDTH * 2 + LINE_WIDTH, LINE_HEIGHT},
    {WPAD, HPAD + 9 * CHEIGHT, CWIDTH, LINE_HEIGHT},
    {WPAD + 2 * CWIDTH, HPAD + 9 * CHEIGHT, CWIDTH, LINE_HEIGHT},
    {WPAD + CWIDTH, HPAD + 8 * CHEIGHT, CWIDTH, LINE_HEIGHT},
    {WPAD + CWIDTH, HPAD + 6 * CHEIGHT, LINE_WIDTH, CHEIGHT},
    {WPAD + 3 * CWIDTH, HPAD + 5 * CHEIGHT, LINE_WIDTH, CHEIGHT},
    {WPAD + 2 * CWIDTH, HPAD + 5 * CHEIGHT, CWIDTH, LINE_HEIGHT},
    {WPAD + 6 * CWIDTH, HPAD + 2 * CHEIGHT, CWIDTH * 4, LINE_HEIGHT},
    {WPAD + 6 * CWIDTH, HPAD + 2 * CHEIGHT, LINE_WIDTH, CHEIGHT},
    {WPAD + 7 * CWIDTH, HPAD + 3 * CHEIGHT, CWIDTH, LINE_HEIGHT},
    {WPAD + 8 * CWIDTH, HPAD + CHEIGHT, CWIDTH, LINE_HEIGHT},
    {WPAD + 8 * CWIDTH, HPAD + CHEIGHT, LINE_WIDTH, CHEIGHT * 2 + LINE_HEIGHT},
    {WPAD + 9 * CWIDTH, HPAD + 3 * CHEIGHT, LINE_WIDTH, 2 * CHEIGHT},
    {WPAD + 6 * CWIDTH, HPAD + 4 * CHEIGHT, CWIDTH * 3, LINE_HEIGHT},
    {WPAD + 6 * CWIDTH, HPAD + 4 * CHEIGHT, LINE_WIDTH, CHEIGHT * 3},
    {WPAD + 6 * CWIDTH, HPAD + 7 * CHEIGHT, CWIDTH * 2, LINE_HEIGHT},
    {WPAD + 8 * CWIDTH, HPAD + 7 * CHEIGHT, LINE_WIDTH, CHEIGHT},
    {WPAD + 5 * CWIDTH, HPAD + 8 * CHEIGHT, 4 * CWIDTH, LINE_HEIGHT},
    {WPAD + 9 * CWIDTH, HPAD + 8 * CHEIGHT, LINE_WIDTH, CHEIGHT},
    {WPAD + 6 * CWIDTH, HPAD + 8 * CHEIGHT, LINE_WIDTH, CHEIGHT * 2},
    {WPAD + 7 * CWIDTH, HPAD + 9 * CHEIGHT, CWIDTH, LINE_HEIGHT},
    {WPAD + 8 * CWIDTH, HPAD + 9 * CHEIGHT, LINE_WIDTH, CHEIGHT},
    {WPAD + 7 * CWIDTH, HPAD + 5 * CHEIGHT, CWIDTH, LINE_HEIGHT},
    {WPAD + 7 * CWIDTH, HPAD + 5 * CHEIGHT, LINE_WIDTH, CHEIGHT},
    {WPAD + 8 * CWIDTH, HPAD + 5 * CHEIGHT, LINE_WIDTH, CHEIGHT},
    {WPAD + 8 * CWIDTH, HPAD + 6 * CHEIGHT, CWIDTH, LINE_HEIGHT},
    {WPAD + 9 * CWIDTH, HPAD + 6 * CHEIGHT, LINE_WIDTH, CHEIGHT},
    {WPAD + 9 * CWIDTH, HPAD + 7 * CHEIGHT, CWIDTH, LINE_HEIGHT},
    {WPAD + 4 * CWIDTH, HPAD, CWIDTH, LINE_HEIGHT}
};

void drawWalls(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0x0f, 0x5b, 0xd6, 0xFF);
    for (int i = 0; i < wallsCount; ++i) {
        WALL(walls[i][0], walls[i][1], walls[i][2], walls[i][3], renderer);
    }
}

void drawPlayer(SDL_Renderer* renderer) {
    if (isGameWon) {
        SDL_SetRenderDrawColor(renderer, 0xd1, 0x2b, 0x1f, 0xFF);
    }
    else {
        SDL_SetRenderDrawColor(renderer, 0x27, 0xd1, 0x0d, 0xFF);
    }
    WALL((int)(playerX), (int)(playerY), PLAYER_WIDTH, PLAYER_HEIGHT, renderer);
}

void handleInput(SDL_KeyboardEvent *key) {
    switch (key->keysym.scancode) {
        case 0x07:
            inpX = (int)(key->type == SDL_KEYDOWN);
            break;

        case 0x16:
            inpY = (int)(key->type == SDL_KEYDOWN);
            break;

        case 0x04:
            inpX = -(int)(key->type == SDL_KEYDOWN);
            break;

        case 0x1A:
            inpY = -(int)(key->type == SDL_KEYDOWN);
            break;

        default:
            break;
    }
}

double sgn(double a) {
    if (a == 0) {
        return 0;
    }
    return (double)(abs(a) / a);
}

void applyInput() {
    if (inpX == 0) {
        if (abs(inpX * maxVel - velX) < 0.01) {
            velX = 0;
        } else {
            if (inpX * maxVel - velX < 0) {
                velX -= deceleration * deltaTime;
            } else {
                velX += deceleration * deltaTime;
            }
        }
    } else {
        velX += sgn(inpX * maxVel - velX) * acceleration * deltaTime;
    }
    if (abs(velX) > maxVel) {
        velX = maxVel * sgn(velX);
    }

    if (inpY == 0) {
        if (abs(inpY * maxVel - velY) < 0.01) {
            velY = 0;
        } else {
            if (inpY * maxVel - velY < 0) {
                velY -= deceleration * deltaTime;
            } else {
                velY += deceleration * deltaTime;
            }
        }
    } else {
        velY += sgn(inpY * maxVel - velY) * acceleration * deltaTime;
    }
    if (abs(velY) > maxVel) {
        velY = maxVel * sgn(inpY);
    }

    playerX += velX;
    playerY += velY;
    double newX;
    double newY;

    if (velX != 0 || velY != 0) {
        for (int i = 0; i < wallsCount; ++i) {
            if (velX != 0) {
                if (playerX < walls[i][0] + walls[i][2] && playerX + PLAYER_WIDTH > walls[i][0]) {
                    if (playerY + PLAYER_HEIGHT > walls[i][1] && playerY < walls[i][1] + walls[i][3]) {
                        if (velX < 0) {
                            newX = walls[i][0] + walls[i][2];
                        } else {
                            newX = walls[i][0] - PLAYER_WIDTH;
                        }
                        if (abs(playerX - newX) < TELEPORT_THRESHOLD) {
                            playerX = newX;
                        }
                        else {
                            playerX -= velX;
                        }
                        velX = 0;
                    }
                }
            }

            if (velY != 0) {
                if (playerX < walls[i][0] + walls[i][2] && playerX + PLAYER_WIDTH > walls[i][0]) {
                    if (playerY < walls[i][1] + walls[i][3] && playerY + PLAYER_HEIGHT > walls[i][1]) {
                        if (velY < 0) {
                            newY = walls[i][1] + walls[i][3];
                        } else {
                            newY = walls[i][1] - PLAYER_HEIGHT;
                        }
                        if (abs(playerY - newY) < TELEPORT_THRESHOLD) {
                            playerY = newY;
                        }
                        else {
                            playerY -= velY;
                        }
                        velY = 0;
                    }
                }
            }
        }
    }

    if (playerX >= WPAD + 5 * CWIDTH && playerX <= WPAD + 6 * CWIDTH && playerY >= 9.5 * CHEIGHT + HPAD) {
        isGameWon = true;
    }
}

int main(int argc, char *argv[]) {
    if (IS_LINUX == 0 && WINDOWS_DEBUG == 1) {
        AllocConsole();
        freopen("CONOUT$", "w", stdout);
    }
    SDL_Window* window = NULL;
    lastFrameTime = SDL_GetPerformanceCounter();

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL Initialization Error! SDL_Error: %s\n", SDL_GetError());
        return -1;
    }

    if (TTF_Init() < 0) {
        printf("TTF Initialization Error! TTF_Error: %s\n", TTF_GetError());
        return -1;
    }

    window = SDL_CreateWindow("Maze Game", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (window == NULL) {
        printf("SDL Window can't be created! SDL_Error: %s\n", SDL_GetError());
        return -2;
    }
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_Texture* mapTex = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, SCREEN_WIDTH, SCREEN_HEIGHT);
    TTF_Font* winFont;
    SDL_Rect winMsgRect;

    if (IS_LINUX == 0) {
        winFont = TTF_OpenFont("C:\\Windows\\Fonts\\courer.fon", 24);
        winMsgRect = (SDL_Rect){SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 10};
    }
    else {
        winFont = TTF_OpenFont("/usr/share/fonts/truetype/noto/NotoSansMono-Regular.ttf", 24);
        winMsgRect = (SDL_Rect){SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, SCREEN_WIDTH / 2, SCREEN_HEIGHT / 10};   
    }
    SDL_Color whiteColor = {255, 255, 255};
    SDL_Surface* winSurface = TTF_RenderText_Solid(winFont, "You Win!!!", whiteColor);
    SDL_Texture* winTex = SDL_CreateTextureFromSurface(renderer, winSurface);

    SDL_SetRenderTarget(renderer, mapTex);
    SDL_SetRenderDrawColor(renderer, 0x3e, 0x3e, 0x3e, 0xFF);
    SDL_RenderClear(renderer);
    drawWalls(renderer);
    SDL_SetRenderTarget(renderer, NULL);

    uint32_t ticks = SDL_GetTicks();
    uint32_t delta;
    uint32_t lastTicks = ticks;

    SDL_Event e;
    bool run = true;
    while (run) {

        ticks = SDL_GetTicks();
        delta = ticks - lastTicks;

        if (delta > 1000 / 60.0) {
            lastTicks = ticks;
        }
        else {
            continue;
        }

        while (SDL_PollEvent(&e)) {
            switch (e.type) {
                case SDL_QUIT:
                    run = false;
                    break;

                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    handleInput(&e.key);
                    break;

                default:
                    break;
            }
        }

        deltaTime = (double)((SDL_GetPerformanceCounter() - lastFrameTime) / (double)(SDL_GetPerformanceFrequency()));
        lastFrameTime = SDL_GetPerformanceCounter();

        SDL_RenderCopy(renderer, mapTex, NULL, NULL);
        applyInput();

        if (isGameWon) {
            SDL_RenderCopy(renderer, winTex, NULL, &winMsgRect);
        }

        drawPlayer(renderer);
        SDL_RenderPresent(renderer);
    }

    SDL_DestroyWindow(window);
    SDL_Quit();

    if (IS_LINUX == 0 && WINDOWS_DEBUG == 1) {
        system("pause");
    }

    return 0;
}

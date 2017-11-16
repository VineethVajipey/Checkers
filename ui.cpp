#ifdef _WIN32
#include <SDL.h>
#include <SDL_image.h>
#else // _WIN32
#include <SDL2/SDL.h>
#ifdef __APPLE__
#include <SDL2_image/SDL_image.h>
#else // __APPLE__
#include <SDL2/SDL_image.h>
#endif // __APPLE_
#endif // _WIN32

SDL_Texture *loadTexture(const char *file, SDL_Renderer *renderer) {
    SDL_Surface *surface = IMG_Load(file);
    if (!surface) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "IMG_Load: %s", IMG_GetError());
        return nullptr;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateTextureFromSurface: %s", SDL_GetError());
        return nullptr;
    }
    return texture;
}

#include <string>

int board[8][8] = {
    {1, 0, 1, 0, 1, 0, 1, 0},
    {0, 1, 0, 1, 0, 1, 0, 1},
    {1, 0, 1, 0, 1, 0, 1, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0},
    {0, 2, 0, 2, 0, 2, 0, 2},
    {2, 0, 2, 0, 2, 0, 2, 0},
    {0, 2, 0, 2, 0, 2, 0, 2}
};

bool isPlayer2 = true;
int selectedX = -1,
    selectedY = -1;
int piecesRemaining[2] = {12, 12};

const int window_w = 500,
          window_h = 500;

SDL_Texture  *sprites[9];
SDL_Renderer *renderer;
SDL_Window   *window;

void draw(int x, int y, int w, int h, SDL_Texture* sprite) {
    SDL_Rect position;
    position.x = x;
    position.y = y;
    position.w = w;
    position.h = h;
    SDL_RenderCopy(renderer, sprite, nullptr, &position);
}

int fromx(int x) {
    return static_cast<int>(28 + 55.5 * (x + 0.5) + 0.5);
}
int fromy(int y) {
    return static_cast<int>(29 + 55.5 * (y + 0.5) + 0.5);
}
double toX(int x) {
    return (x - 28) / 55.5 - 0.5;
}
double toY(int y) {
    return (y - 29) / 55.5 - 0.5;
}

int getPiece(int clickX, int clickY) {
    double x = toX(clickX) + 0.5;
    double y = toY(clickY) + 0.5;

    if (x > 8 || x < 0 || y > 8 || y < 0)
        return -1;

    int xi = static_cast<int>(x);
    int yi = static_cast<int>(y);

    if (board[yi][xi] && board[yi][xi] % 2 == isPlayer2)
        return -1;

    double dx = x - xi - 0.5;
    double dy = y - yi - 0.5;

    if (board[yi][xi] && dx * dx + dy * dy > 24/55.5 * 24/55.5)
        return -1;
    
    return xi + 8 * yi;

}

bool newspotcheck(int board[8][8], int newy, int newx, int oldy, int oldx, bool isPlayer1, int &otherPlayersPiecesRemaining);

void loop() {
    bool done = false;

    SDL_Event event;
    while (SDL_PollEvent(&event))
        if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
            int piece = getPiece(event.button.x, event.button.y);
            if (piece >= 0) {
                int x = piece % 8,
                    y = piece / 8;

                if (board[y][x])
                    if (selectedX == x && selectedY == y)
                        selectedX = selectedY = -1;
                    else {
                        selectedX = x;
                        selectedY = y;
                    }
                else if (selectedX >= 0 && selectedY >= 0 && newspotcheck(board, y, x, selectedY, selectedX, !isPlayer2, piecesRemaining[!isPlayer2])) {
                    board[y][x] = board[selectedY][selectedX];
                    board[selectedY][selectedX] = 0;

                    isPlayer2 = !isPlayer2;
                    SDL_SetWindowTitle(window, ("Checkers: Player " + std::to_string(isPlayer2 + 1)).c_str());
                    if (y == 7 * isPlayer2 && board[y][x] < 3)
                        board[y][x] += 2;
                    if (!piecesRemaining[isPlayer2])
                        done = true;
                }
            }
        }

    SDL_RenderClear(renderer);
    draw(0, 0, 500, 500, sprites[0]);
    for (int x = 0; x < 8; ++x)
        for (int y = 0; y < 8; ++y)
            if (board[y][x])
                draw(fromx(x) - 24, fromy(y) - 24, 48, 48, sprites[board[y][x] + 4 * (x == selectedX && y == selectedY)]);
    SDL_RenderPresent(renderer);

    if (done) {
        std::string player = "Player " + std::to_string(2 - isPlayer2);
        int amount = piecesRemaining[!isPlayer2];
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, (player + " Won!").c_str(), (player + " won with " + std::to_string(amount) + (amount == 1 ? " piece." : " pieces remaining.")).c_str(), window);
        exit(0);
    }
}

int main(int argc, char **argv) {
    (void)argc; (void)argv;

    SDL_LogSetAllPriority(SDL_LOG_PRIORITY_INFO);

    if (SDL_CreateWindowAndRenderer(window_w, window_h, SDL_WINDOW_ALLOW_HIGHDPI, &window, &renderer)) {
        SDL_LogCritical(SDL_LOG_CATEGORY_APPLICATION, "SDL_CreateWindowAndRenderer: %s", SDL_GetError());
        return 1;
    }

    SDL_SetWindowTitle(window, "Checkers: Player 1");

    SDL_RenderSetLogicalSize(renderer, window_w, window_h);

    sprites[0] = loadTexture("Media/checker_board.png", renderer);
    sprites[1] = loadTexture("Media/white_guy.png"    , renderer);
    sprites[2] = loadTexture("Media/black_guy.png"    , renderer);
    sprites[3] = loadTexture("Media/white_king.png"   , renderer);
    sprites[4] = loadTexture("Media/black_king.png"   , renderer);
    sprites[5] = loadTexture("Media/white_sel.png"    , renderer);
    sprites[6] = loadTexture("Media/black_sel.png"    , renderer);
    sprites[7] = loadTexture("Media/white_ksel.png"   , renderer);
    sprites[8] = loadTexture("Media/black_ksel.png"   , renderer);
    
    if (!(sprites[0] && sprites[1] && sprites[2] && sprites[3] && sprites[4] && sprites[5] && sprites[6] && sprites[7] && sprites[8]))
        return 1;
        
    while (!SDL_QuitRequested())
        loop();

    return 0;
}

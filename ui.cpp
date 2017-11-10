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

#include <string>

int c_board[8][8] = { { 1, 0, 1, 0, 1, 0, 1, 0 },{ 0, 1, 0, 1, 0, 1, 0, 1 },{ 1, 0, 1, 0, 1, 0, 1, 0 },{ 0, 0, 0, 0, 0, 0, 0, 0 },{ 0, 0, 0, 0, 0, 0, 0, 0 },{ 0, 2, 0, 2, 0, 2, 0, 2 },{ 2, 0, 2, 0, 2, 0, 2, 0 },{ 0, 2, 0, 2, 0, 2, 0, 2 } };
int player2turn = 0;
int selX  = -1, selY = -1;
int pieces[2] = {12, 12};

static SDL_Texture *loadTexture(const char *file, SDL_Renderer *renderer) {

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

static const int window_w = 500, window_h = 500;

static SDL_Texture *board, *white_piece, *white_king, *black_piece, *black_king, *white_sel, *black_sel, *white_ksel, *black_ksel;

static SDL_Renderer *renderer;
static SDL_Window *window;

static void draw(int x, int y, int w, int h, SDL_Texture* sprite) {

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

/*	for (int x = 0; x < 8; ++x)
	{
		double dx = clickX - fromx(x);
		for (int y = 0; y < 8; ++y)
		{
			double dy = clickY - fromy(y);
			if (dx * dx + dy * dy < (48 * 48) / 4)
				return x + 8 * y;
		}
	}

	return -1;
	*/

	double x = toX(clickX) + 0.5;
	double y = toY(clickY) + 0.5;

	if (x > 8 || x < 0 || y > 8 || y < 0)
		return -1;

	int xi = static_cast<int>(x);
	int yi = static_cast<int>(y);

	if (c_board[yi][xi] && c_board[yi][xi] % 2 == player2turn)
		return -1;

	double dx = x - xi - 0.5;
	double dy = y - yi - 0.5;

	//SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "mouse button pressed at (%f, %f)", dx, dy);

	if (c_board[yi][xi] && dx * dx + dy * dy > 24/55.5 * 24/55.5) {
		return -1;
	}
	
	return xi + 8 * yi;

}


bool newspotcheck(int board[8][8], int nr, int nc, int cr, int cc, bool type, int &count);

static void loop() {

	bool done = false;

	SDL_Event event;

	while (SDL_PollEvent(&event))

		if (event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT) {
			int piece = getPiece(event.button.x, event.button.y);
			if (piece >= 0) 
				if (c_board[piece / 8][piece % 8] > 0) {
					if (selX == piece % 8 && selY == piece / 8)
						selX = selY = -1;
					else {
						selX = piece % 8;
						selY = piece / 8;
					}
				} else if(selX >= 0 && selY >= 0) {
					
					if (newspotcheck(c_board, piece / 8, piece % 8, selY, selX, !player2turn, pieces[!player2turn])) {
						SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "pieces (%d, %d)", pieces[0], pieces[1]);
						c_board[piece / 8][piece % 8] = c_board[selY][selX];
						c_board[selY][selX] = 0;
						player2turn = !player2turn;
						SDL_SetWindowTitle(window, ("Checkers: Player " + std::to_string(player2turn + 1)).c_str());
						if (piece / 8 == 7 * player2turn)
							c_board[piece / 8][piece % 8] += 2;
						if (pieces[player2turn] == 0)
						{
							done = true;
						}
					}
				}
					
			
		}
		//else if (event.type == SDL_KEYDOWN)

		//	SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "key %s pressed", SDL_GetKeyName(event.key.keysym.sym));

	SDL_RenderClear(renderer);

	draw(0, 0, 500, 500, board);

	for (int x = 0; x < 8; ++x)
		for (int y = 0; y < 8; ++y)
		{
			if (x == selX && y == selY)
				switch (c_board[y][x]) {
				
				case 1:
					draw(fromx(x) - 24, fromy(y) - 24, 48, 48, white_sel);
					break;
				case 2:
					draw(fromx(x) - 24, fromy(y) - 24, 48, 48, black_sel);
					break;
				case 3:
					draw(fromx(x) - 24, fromy(y) - 24, 48, 48, white_ksel);
					break;
				case 4:
					draw(fromx(x) - 24, fromy(y) - 24, 48, 48, black_ksel);
					break;
				}
			else
				switch (c_board[y][x]) {
				case 1:
					draw(fromx(x) - 24, fromy(y) - 24, 48, 48, white_piece);
					break;
				case 2:
					draw(fromx(x) - 24, fromy(y) - 24, 48, 48, black_piece);
					break;
				case 3:
					draw(fromx(x) - 24, fromy(y) - 24, 48, 48, white_king);
					break;
				case 4:
					draw(fromx(x) - 24, fromy(y) - 24, 48, 48, black_king);
					break;
				}
		}
	SDL_RenderPresent(renderer);

	if (done) {
		SDL_LogInfo(SDL_LOG_CATEGORY_APPLICATION, "mouse button %d pressed at (%d, %d)", event.button.button, event.button.x, event.button.y);
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_INFORMATION, ("Player " + std::to_string(2 - player2turn) + " Won!").c_str(), ("Player " + std::to_string(2 - player2turn) + " won with " + std::to_string(pieces[!player2turn]) + (pieces[!player2turn] == 1 ? " piece." : " pieces.")).c_str(), window);
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

	board = loadTexture("Media/checker_board.png", renderer);
	black_piece = loadTexture("Media/black_guy.png", renderer);
	black_king = loadTexture("Media/black_king.png", renderer);
	black_sel = loadTexture("Media/black_sel.png", renderer);
	black_ksel = loadTexture("Media/black_ksel.png", renderer);
	white_piece = loadTexture("Media/white_guy.png", renderer);
	white_king = loadTexture("Media/white_king.png", renderer);
	white_sel = loadTexture("Media/white_sel.png", renderer);
	white_ksel = loadTexture("Media/white_ksel.png", renderer);
	
	//system("pause");

	if (!(board && black_piece && black_king && black_sel && black_ksel && white_piece && white_king && white_sel && white_ksel)) {
		system("pause");
		return 1;
	}
		
	while (!SDL_QuitRequested())
		loop();

	return 0;
}
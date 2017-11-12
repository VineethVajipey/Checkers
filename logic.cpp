#include <iostream>

int blank = 0, red = 1, black = 2, rking = 3, bking = 4;

bool isitblank(int num) {
	if (num == 0)
		return true;
	else
		return false;
}

bool isitfitting(int num1, int num2) {
	return (num1 >= 0 && num1 <= 7 && num2 >= 0 && num2 <= 7);
}


bool eitherblack(int board[8][8], int row, int column) {
	if (board[row][column] == black || board[row][column] == bking) {
		return true;
	}
	else
		return false;
}
bool eitherred(int board[8][8], int row, int column) {
	if (board[row][column] == red || board[row][column] == rking) {
		return true;
	}
	else
		return false;
}

bool newspotcheck(int board[8][8], int nr, int nc, int cr, int cc, bool type, int &count) { //array, new row and column, type of piece
																						   //int blank = 0, red = 1, black = 2, rking = 3, bking = 4;
	if (!isitfitting(nr, nc))
		return false;

	if (board[nr][nc] == red || board[nr][nc] == black || board[nr][nc] == rking || board[nr][nc] == bking)
		return false;
	int smh = cr + 2, smh1 = cr - 2, smh3 = cc - 2, smh4 = cc + 2;
	//if red
	if (board[cr][cc] == 3) {

		if ((eitherblack(board, cr + 1, cc + 1)) && (nr != smh || nc != smh4) && board[smh][smh4] == blank && isitfitting(smh, smh4)) {
			return false;
		}
		if ((eitherblack(board, cr + 1, cc - 1)) && (nr != smh || nc != smh3) && board[smh][smh3] == blank&&isitfitting(smh, smh3)) {
			return false;
		}
		if ((eitherblack(board, cr - 1, cc + 1)) && (nr != smh1 || nc != smh4) && board[smh1][smh4] == blank&&isitfitting(smh1, smh4)) {
			return false;
		}
		if ((eitherblack(board, cr - 1, cc - 1)) && (nr != smh1 || nc != smh3) && board[smh1][smh3] == blank&&isitfitting(smh1, smh3)) {
			return false;
		}
		if ((nr == cr + 1 || nr == cr - 1) && (nc == cc + 1 || nc == cc - 1)) {
			return true;
		}
		/*else
		return false;*/
		if (nr == cr + 2 && (nc == cc + 2 || nc == cc - 2)) {
			if (eitherblack(board, cr + 1, cc + 1) || eitherblack(board, cr + 1, cc - 1)) {
				count--;
				if (nc == cc + 2)
					board[nr - 1][nc - 1] = blank;
				else if (nc == cc - 2)
					board[nr - 1][nc + 1] = blank;
				board[nr][nc] = rking;
				return true;
			}
		}
		if (nr == cr - 2 && (nc == cc + 2 || nc == cc - 2)) {
			if (eitherblack(board, cr - 1, cc + 1) || eitherblack(board, cr - 1, cc - 1)) {
				count--;
				if (nc == cc + 2)
					board[nr + 1][nc - 1] = blank;
				else if (nc == cc - 2)
					board[nr + 1][nc + 1] = blank;
				board[nr][nc] = rking;
				return true;
			}
		}

		return false;
	}


	//if black
	if (board[cr][cc] == 4) {

		if ((eitherred(board, cr + 1, cc + 1)) && (nr != smh || nc != smh4) && isitfitting(smh, smh4) && board[smh][smh4] == blank) {
			return false;
		}
		if ((eitherred(board, cr + 1, cc - 1)) && (nr != smh || nc != smh3) && isitfitting(smh, smh3) && board[smh][smh3] == blank) {
			return false;
		}
		if ((eitherred(board, cr - 1, cc + 1)) && (nr != smh1 || nc != smh4) && isitfitting(smh1, smh4) && board[smh1][smh4] == blank) {
			return false;
		}
		if ((eitherred(board, cr - 1, cc - 1)) && (nr != smh1 || nc != smh3) && isitfitting(smh1, smh3) && board[smh1][smh3] == blank) {
			return false;
		}
		if ((nr == cr + 1 || nr == cr - 1) && (nc == cc + 1 || nc == cc - 1)) {
			return true;
		}
		if (nr == cr + 2 && (nc == cc + 2 || nc == cc - 2)) {
			if (eitherred(board, cr + 1, cc + 1) || eitherred(board, cr + 1, cc - 1)) {
				count--;
				if (nc == cc + 2)
					board[nr - 1][nc - 1] = blank;
				else if (nc == cc - 2)
					board[nr - 1][nc + 1] = blank;
				board[nr][nc] = 4;
				return true;
			}
		}
		if (nr == cr - 2 && (nc == cc + 2 || nc == cc - 2)) {
			if (eitherred(board, cr - 1, cc + 1) || eitherred(board, cr - 1, cc - 1)) {
				count--;
				if (nc == cc + 2)
					board[nr + 1][nc - 1] = blank;
				else if (nc == cc - 2)
					board[nr + 1][nc + 1] = blank;
				board[nr][nc] = 4;
				return true;
			}
		}
		return false;
	}

	//int smh = cr + 2, smh1 = cr - 2, smh3 = cc - 2, smh4 = cc + 2;
	if (type == 1) { //red
		if (eitherblack(board, cr + 1, cc + 1)) {
			if ((nr != smh || nc != smh4) && isitfitting(smh, smh4) && board[smh][smh4] == blank) {
				return false;
			}
		}
		else if (eitherblack(board, cr + 1, cc - 1)) {
			if ((nr != smh || nc != smh3) && isitfitting(smh, smh3) && board[smh][smh3] == blank) {
				return false;
			}
		}

		if (nr == cr + 2 && (nc == cc + 2 || nc == cc - 2)) {
			if (eitherblack(board, cr + 1, cc + 1) || eitherblack(board, cr + 1, cc - 1)) {
				count--;
				if (nc == cc + 2)
					board[nr - 1][nc - 1] = blank;
				else if (nc == cc - 2)
					board[nr - 1][nc + 1] = blank;

				return true;
			}
		}

		if (nr == cr + 1 && (nc == cc + 1 || nc == cc - 1)) {
			return true;
		}
		else
			return false;
	}

	else if (type == 0) {
		if (eitherred(board, cr - 1, cc + 1)) {
			if ((nr != smh1 || nc != smh4) && isitfitting(smh1, smh4) && isitblank(board[smh1][smh4])) {
				return false;
			}
		}
		else if (eitherred(board, cr - 1, cc - 1)) {
			if ((nr != smh1 || nc != smh3) && isitfitting(smh1, smh3) && isitblank(board[smh1][smh3])) {
				return false;
			}
		}
		if (nr == cr - 2 && (nc == cc + 2 || nc == cc - 2)) {
			if (eitherred(board, cr - 1, cc + 1) || eitherred(board, cr - 1, cc - 1)) {
				count--;
				if (nc == cc + 2)
					board[nr + 1][nc - 1] = blank;
				else if (nc == cc - 2)
					board[nr + 1][nc + 1] = blank;
				return true;
			}
		}

		if (nr == cr - 1 && (nc == cc + 1 || nc == cc - 1)) {
			return true;
		}
		else
			return false;

	}
}

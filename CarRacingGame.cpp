#include <iostream>
#include <conio.h>
#include <windows.h>
#include <ctime>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int enemyY[3];
int enemyX[3];
int enemyFlag[3];
char car[4][4] = {
    ' ', '#', '#', ' ',
    '#', '#', '#', '#',
    ' ', '#', '#', ' ',
    '#', '#', '#', '#'
};

int carPos = WIN_WIDTH / 2;
int score = 0;

void gotoxy(int x, int y) {
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size) {
    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = size;
    SetConsoleCursorInfo(console, &lpCursor);
}

void drawBorder() {
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < 17; j++) {
            gotoxy(0 + j, i); cout << "#";
            gotoxy(WIN_WIDTH - j, i); cout << "#";
        }
    }
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        gotoxy(SCREEN_WIDTH, i); cout << "#";
    }
}

void genEnemy(int ind) {
    enemyX[ind] = 17 + rand() % 33;
}

void drawEnemy(int ind) {
    if (enemyFlag[ind]) {
        gotoxy(enemyX[ind], enemyY[ind]);   cout << "####";
        gotoxy(enemyX[ind], enemyY[ind] + 1); cout << " ## ";
        gotoxy(enemyX[ind], enemyY[ind] + 2); cout << "####";
        gotoxy(enemyX[ind], enemyY[ind] + 3); cout << " ## ";
    }
}

void eraseEnemy(int ind) {
    if (enemyFlag[ind]) {
        gotoxy(enemyX[ind], enemyY[ind]); cout << "    ";
        gotoxy(enemyX[ind], enemyY[ind] + 1); cout << "    ";
        gotoxy(enemyX[ind], enemyY[ind] + 2); cout << "    ";
        gotoxy(enemyX[ind], enemyY[ind] + 3); cout << "    ";
    }
}

void resetEnemy(int ind) {
    eraseEnemy(ind);
    enemyY[ind] = 1;
    genEnemy(ind);
}

void drawCar() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            gotoxy(j + carPos, i + 22); cout << car[i][j];
        }
    }
}

void eraseCar() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            gotoxy(j + carPos, i + 22); cout << " ";
        }
    }
}

int collision() {
    for (int i = 0; i < 3; i++) {
        if (enemyFlag[i] && enemyY[i] + 4 >= 23) {
            if (enemyX[i] + 4 - carPos >= 0 && enemyX[i] + 4 - carPos < 9) {
                return 1;
            }
        }
    }
    return 0;
}

void gameover() {
    system("cls");
    cout << endl;
    cout << "\t\t--------------------------" << endl;
    cout << "\t\t-------- Game Over -------" << endl;
    cout << "\t\t--------------------------" << endl << endl;
    cout << "\t\tPress any key to go back to menu.";
    getch();
}

void updateScore() {
    gotoxy(WIN_WIDTH + 7, 5); cout << "Score: " << score << endl;
}

void instructions() {
    system("cls");
    cout << "Instructions";
    cout << "\n----------------";
    cout << "\n Avoid Cars by moving left or right.";
    cout << "\n\n Press 'L' to move left";
    cout << "\n Press 'R' to move right";
    cout << "\n Press 'escape' to exit";
    cout << "\n\nPress any key to go back to menu";
    getch();
}

void play() {
    carPos = -1 + WIN_WIDTH / 2;
    score = 0;
    enemyFlag[0] = 1;
    enemyFlag[1] = 0;
    enemyY[0] = enemyY[1] = 1;

    system("cls");
    drawBorder();
    updateScore();
    genEnemy(0);
    genEnemy(1);

    gotoxy(WIN_WIDTH + 7, 2); cout << "Car Game";
    gotoxy(WIN_WIDTH + 6, 4); cout << "----------";
    gotoxy(WIN_WIDTH + 6, 6); cout << "----------";
    gotoxy(WIN_WIDTH + 7, 12); cout << "Control ";
    gotoxy(WIN_WIDTH + 7, 13); cout << "-------- ";
    gotoxy(WIN_WIDTH + 2, 14); cout << " L Key - Left";
    gotoxy(WIN_WIDTH + 2, 15); cout << " R Key - Right";

    gotoxy(18, 5); cout << "Press any key to start";
    getch();
    gotoxy(18, 5); cout << "                      ";

    while (1) {
        if (kbhit()) {
            char ch = getch();
            if (ch == 'l' || ch == 'L') {
                if (carPos > 18) carPos -= 4;
            }
            if (ch == 'r' || ch == 'R') {
                if (carPos < 50) carPos += 4;
            }
            if (ch == 27) {
                break;
            }
        }

        drawCar();
        for (int i = 0; i < 3; i++) {
            drawEnemy(i);
        }
        if (collision() == 1) {
            gameover();
            return;
        }
        Sleep(50);
        eraseCar();
        for (int i = 0; i < 3; i++) {
            eraseEnemy(i);
        }

        for (int i = 0; i < 3; i++) {
            if (enemyY[i] == 10 && enemyFlag[(i + 1) % 3] == 0) {
                enemyFlag[(i + 1) % 3] = 1;
            }
            if (enemyFlag[i] == 1) {
                enemyY[i] += 1;
            }
            if (enemyY[i] > SCREEN_HEIGHT - 4) {
                resetEnemy(i);
                score++;
                updateScore();
            }
        }
    }
}

int main() {
    setcursor(0, 0);
    srand((unsigned)time(NULL));

    do {
        system("cls");
        gotoxy(10, 5); cout << " -------------------------- ";
        gotoxy(10, 6); cout << " |        Car Game        | ";
        gotoxy(10, 7); cout << " --------------------------";
        gotoxy(10, 9); cout << "1. Start Game";
        gotoxy(10, 10); cout << "2. Instructions";
        gotoxy(10, 11); cout << "3. Quit";
        gotoxy(10, 13); cout << "Select option: ";
        char op = getche();

        if (op == '1') play();
        else if (op == '2') instructions();
        else if (op == '3') exit(0);

    } while (1);

    return 0;
}


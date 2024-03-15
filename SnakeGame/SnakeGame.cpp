//Задание. Создать игровую программу "Червяк". По экрану движется червяк, направление движения головы которого можно
//менять(стрелками).Червяк ест призы и растет.Задача - при движении головы не наткнуться
//на хвост и границы экрана.


#include <iostream>
#include <Windows.h>
#include "SnakeGame.h"
#include <ctime>

using namespace std;

// Глобальные переменные
// Карта 
const int rows = 15;
const int columns = 30;
const char mapSymbol = '#';
// Змейка
const char snakeHead = 'O';
const char snakeTail = '*';
const int maxSnakeLength = (columns - 3) * (rows - 2);
int snakeX[maxSnakeLength] = { 0 };
int snakeY[maxSnakeLength] = { 0 };
int snakeLength = 1;
// Еда
int foodX;
int foodY;
const char foodSymbol = '@';
// Статус игры
bool isGameRunning = true;

// Перечисление для выбора направления движения
enum Move
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

Move snakeDirection = RIGHT; // Начальное направление движения змейки вправо

// Методы
void PrintMap()
{
    for (int y = 0; y < rows; y++) { // Печатает карту
        for (int x = 0; x < columns; x++) {
            if (y == 0 || x == 0 || y == rows - 1 || x == columns - 1) {
                cout << mapSymbol;
            }
            else {
                cout << ' ';
            }        
        }
        cout << endl;
    }
}

void SetCursorPosition(int x, int y) { // Устанавливает курсор в позицию с заданными координатами
    COORD position = { x, y }; // Структура с координатами
    HANDLE output = GetStdHandle(STD_OUTPUT_HANDLE); // Дескриптор устройства стандартного вывода
    CONSOLE_CURSOR_INFO structCursorInfo; // Структура, содержащая сведения о буфере экрана консоли
    GetConsoleCursorInfo(output, &structCursorInfo); // Присваиваем дескриптору output структуру со сведениями о курсоре
    SetConsoleCursorPosition(output, position); // Установка курсора в координаты структуры position        
    structCursorInfo.bVisible = FALSE; // Делаем курсор невидимым
    SetConsoleCursorInfo(output, &structCursorInfo); // Передаем данные о видимости курсора
}

void GetKeyToMove()
{ // Выбираем направление движения в зависимости от нажатой клавиши
    if (GetKeyState('W') & 0x8000) {
        if (snakeDirection != DOWN) {
            snakeDirection = UP;
        }
    }
        
    if (GetKeyState('S') & 0x8000) {
        if (snakeDirection != UP) {
            snakeDirection = DOWN;
        }
    }
        
    if (GetKeyState('A') & 0x8000) {
        if (snakeDirection != RIGHT) {
            snakeDirection = LEFT;
        }
    }
        
    if (GetKeyState('D') & 0x8000) {
        if (snakeDirection != LEFT) {
            snakeDirection = RIGHT;
        }
    }       
    ChangeHeadPosition();
}

void ChangeHeadPosition()
{ // Меняем координаты головы змейки в зависимости от направления движения
    switch (snakeDirection) {
    case UP:
        snakeY[0]--;
        break;
    case DOWN:
        snakeY[0]++;
        break;
    case LEFT:
        snakeX[0]--;
        break;
    case RIGHT:
        snakeX[0]++;
        break;
    }
}

void PrintFood() { // Отрисовка еды
    SetCursorPosition(foodX, foodY);
    cout << foodSymbol;
    if (snakeX[0] == foodX && snakeY[0] == foodY) { // Если голова змеи съела еду, то увеличиваем длину змеи и рисуем новую еду
        snakeLength++;
        PrintNewFood();        
    }
}

void PrintNewFood() {
    bool foodSpawn = true; // Разрешение спавна еды
    foodX = 1 + (rand() % (columns - 3));
    foodY = 1 + (rand() % (rows - 2));
    CheckSpawnStatus(foodSpawn);
    while (foodSpawn == false) { // Пока запрещен спавн еды (совпадает со змеей), переопределяем координаты спавна
        foodSpawn = true;
        foodX = 1 + (rand() % (columns - 3));
        foodY = 1 + (rand() % (rows - 2));
        CheckSpawnStatus(foodSpawn);
    }
    SetCursorPosition(foodX, foodY);
    cout << foodSymbol;    
}

void CheckSpawnStatus(bool &foodSpawn) { // Если координаты появления еды совпадают со змеей, то запрещаем спавн
    for (int i = 0; i <= snakeLength; i++) {
        if (snakeX[i] == foodX && snakeY[i] == foodY) {
            foodSpawn = false;
            break;
        }
    }
}

void CheckGameStatus() { // Проверяет пересекает ли голова змеи границы карты или свой хвост, если да, то игра завершается   
    for (int i = 2; i <= snakeLength; i++) {
        if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
            isGameRunning = false;
            return;
        }
    }
    if (snakeX[0] == 0 || snakeY[0] == 0 || snakeX[0] == columns - 1 || snakeY[0] == rows - 1) {
        isGameRunning = false;
    }
}

void GetSnakeCoordinates() { // Заполнение массива координатами каждой части змеи
    for (int i = snakeLength; i > 0; i--) {
        snakeX[i] = snakeX[i - 1];
        snakeY[i] = snakeY[i - 1];
    }
}

void PrintSnake() { // Печатает змейку на экран
    GetSnakeCoordinates();
    for (int i = 0; i <= snakeLength; i++) {
        SetCursorPosition(snakeX[i], snakeY[i]);
        if (i <= 1)
            cout << snakeHead;
        else
            cout << snakeTail;
    }
}

void GameOver() { // Выводит информацию о результатах игры
    SetCursorPosition(2, rows / 2);
    cout << "GameOver! Your score is " << snakeLength - 1 << endl;
    SetCursorPosition(0, rows);
}

void SetInitialCoordinates() { // Устанавливает начальные координаты для змейки и еды
    snakeX[0] = columns / 2;
    snakeY[0] = rows / 2;
    foodX = 1 + (rand() % (columns - 3));
    foodY = 1 + (rand() % (rows - 2));
}

int main()
{   
    srand(time(0));
    SetInitialCoordinates();

    while (isGameRunning) {        
        SetCursorPosition(0, 0);        
        PrintMap();
        PrintFood();       
        PrintSnake();               
        GetKeyToMove();               
        CheckGameStatus();
        Sleep(150);
    }
    GameOver();
}


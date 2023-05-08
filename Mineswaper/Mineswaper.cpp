#include <iostream>
#include <ctime>
#include <windows.h>
#include <conio.h>

using namespace std;

// N - розмір поля за замовчуванням (воно квадратне); M - число мін на полі
//#define N 10
//#define M 10

const int N = 10;
const int M = 10;

// поле, і масив логічних значень, що позначає ті поля, що відкриті
int matrix[N][N];
bool open[N][N];

// перевіряє ячейку на міну, вихід за межі повертає false
bool mine(int i, int j) {
    if ((i >= 0) && (i < N)) {
        if ((j >= 0) && (j < N)) {
            if (matrix[i][j] == -1) return true;
        }
    }
    return false;
}

// перевіряє осередок на порожнечу(true), вихід за межі масиву повертає false
bool empty(int i, int j) {
    if ((i >= 0) && (i < N)) {
        if ((j >= 0) && (j < N)) {
            if (matrix[i][j] == 0) return true;
        }
    }
    return false;
}

// рекурсивна функція, яка відкриває поля в точці попадання
void clean(int i, int j) {
    // перевіримо на вихід за межі масиву
    if ((i >= 0) && (i < N)) {
        if ((j >= 0) && (j < N)) {
            // провірим, не було поле відкрите раніше
            if (!open[i][j]) {
                // відкриваемо
                open[i][j] = true;
                // перевіримо, чи не було відкрито поле раніше
                if (matrix[i][j] == 0) {
                    clean(i - 1, j - 1);
                    clean(i - 1, j);
                    clean(i - 1, j + 1);
                    clean(i, j - 1);
                    clean(i, j + 1);
                    clean(i + 1, j - 1);
                    clean(i + 1, j);
                    clean(i + 1, j + 1);
                }
                // якщо не порожнє (! = 0) відкриваємо тільки порожніх (= 0) сусідів
                else {
                    if (empty(i - 1, j - 1)) clean(i - 1, j - 1);
                    if (empty(i - 1, j)) clean(i - 1, j);
                    if (empty(i - 1, j + 1)) clean(i - 1, j + 1);
                    if (empty(i, j - 1)) clean(i, j - 1);
                    if (empty(i, j + 1)) clean(i, j + 1);
                    if (empty(i + 1, j - 1)) clean(i + 1, j - 1);
                    if (empty(i + 1, j)) clean(i + 1, j);
                    if (empty(i + 1, j + 1)) clean(i + 1, j + 1);
                }
            }
        }
    }
}

// малює "міну" - зірочку червоного кольору
void coutmine(HANDLE hConsole) {
    SetConsoleTextAttribute(hConsole, 12);  // red text
    cout << "* ";
    SetConsoleTextAttribute(hConsole, 7);   // white text
}

// малює мінне поле з урахуванням відкритих і закритих полів
// і допоміжні осі
void draw_matrix(HANDLE hConsole) {
    SetConsoleTextAttribute(hConsole, 6);  // dark yellow
    cout << "  A B C D E F G H I J\n";
    SetConsoleTextAttribute(hConsole, 7);  // white
    for (int x = 0; x < N; x++) {
        SetConsoleTextAttribute(hConsole, 6);  // dark yellow 
        cout << x << " ";
        SetConsoleTextAttribute(hConsole, 7);  // white
        for (int y = 0; y < N; y++) {
            if (open[x][y]) {
                SetConsoleTextAttribute(hConsole, 8);  // gray
                if (matrix[x][y] == -1) coutmine(hConsole);
                else if (matrix[x][y] == 0) cout << ". ";
                else cout << matrix[x][y] << " ";
                SetConsoleTextAttribute(hConsole, 7);  // white
            }
            else {
                cout << "# ";
            }
        }
        cout << "\n";
    }
}

// функція завершує гру, вивівши одну з двох написів "Loser!" або "Winner!"
void fin(HANDLE hConsole, bool loser) {
    COORD coord;
    coord.X = 33;
    coord.Y = 10;
    system("cls");
    draw_matrix(hConsole);
    SetConsoleCursorPosition(hConsole, coord);
    if (loser) {
        SetConsoleTextAttribute(hConsole, 12);  // red
        cout << "L O S E R ! ! !";
        SetConsoleTextAttribute(hConsole, 7);  // white
    }
    else {
        SetConsoleTextAttribute(hConsole, 10);  // red
        cout << "W I N N E R ! ! !";
        SetConsoleTextAttribute(hConsole, 7);  // white
    }
    cout << "\n\n\n\n";
    _getch();
}

// в разі програшу ця функція відкриє всі міни
void openmines() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (matrix[i][j] == -1) open[i][j] = true;
        }
    }
}

// перевіряє, чи всі поле відкрито, крім мін
bool checkwin() {
    for (int x = 0; x < N; x++) {
        for (int y = 0; y < N; y++) {
            if ((matrix[x][y] != -1) && (!open[x][y])) return false;
        }
    }
    return true;
}


int main(int argc, int argv[])
{
    int i, j, k = 0;
    char s[3];
    // хэндл экна необхідний для малювання кольорового тексту
    HANDLE  hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    // ініціалізація
    setlocale(0, "rus");
    srand(time(0));

    // все чистимо
    for (int c = 0; c < 100; c++) { matrix[c / 10][c % 10] = 0; open[c / 10][c % 10] = false; }
    // заповнюємо масив поля мінами
    for (int c = 0; c < M; c++) {
        do {
            i = rand() % N; j = rand() % N;
        } while (matrix[i][j] != 0);
        matrix[i][j] = -1;
    }
    // заповнюємо масив поля цифрами
    for (i = 0; i < N; i++) {
        for (j = 0; j < N; j++) {
            if (matrix[i][j] != -1) {
                k = 0;
                if (mine(i - 1, j - 1)) k++;
                if (mine(i - 1, j)) k++;
                if (mine(i - 1, j + 1)) k++;
                if (mine(i, j - 1)) k++;
                if (mine(i, j + 1)) k++;
                if (mine(i + 1, j - 1)) k++;
                if (mine(i + 1, j)) k++;
                if (mine(i + 1, j + 1)) k++;
                matrix[i][j] = k;
            }
        }
    }

    // головний ігровий цикл
    while (true) {
        // чистимо екран від старого малюнка
        system("cls");
        // малюємо поле
        draw_matrix(hConsole);
        cout << "\n";
        // запитуємо координати удару

        cout << "Введите координаты удара (D2): ";
        cin >> s;
        // переводимо координати в цифровий вигляд
        if ((s[0] >= 65) && (s[0] <= 74)) j = s[0] - 65;        // буква в проміжку від A до J
        else if ((s[0] >= 97) && (s[0] <= 106)) j = s[0] - 97;  // буква в проміжку від a до j
        else continue;                              // якщо введені невірні значення, назад до початку циклу
        if ((s[1] >= 48) && (s[1] <= 57)) i = s[1] - 48;
        else continue;
        // далі перевіряємо всі вісім навколишніх полів на порожні клітини
        // і якщо треба показуємо якийсь шматок поля (ну, багато порожніх клітин, наприклад)
        clean(i, j);

        if (mine(i, j)) { openmines(); fin(hConsole, true); break; }  // програма залишає цикл в разі програшу
        if (checkwin()) { fin(hConsole, false); break; }                 // або перемоги
    }

    return 0;
}
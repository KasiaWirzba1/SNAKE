#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace std;

const int szerokosc_planszy = 25;
const int wysokosc_planszy = 25;

// Inicjalizacja planszy
void init_tab(int plansza[szerokosc_planszy][wysokosc_planszy]) {
    for (int i = 0; i < szerokosc_planszy; i++) {  // Wypełnienie planszy zerami.
        for (int j = 0; j < wysokosc_planszy; j++) {
            plansza[i][j] = 0;
        }
    }
}

void wąż(vector<pair<int, int>>& ciało_węża, int& wążX, int& wążY) {
    wążX = 13;
    wążY = 13;
    ciało_węża.push_back({wążX, wążY});
}


int main() {
    int plansza[szerokosc_planszy][wysokosc_planszy];

    init_tab(plansza);  // Inicjalizujemy planszę

    vector<pair<int, int>> ciało_węża;
    int wążX, wążY;
    wąż(ciało_węża, wążX, wążY);

    // Wyświetlenie planszy
    for (int i = 0; i < szerokosc_planszy; i++) {
        for (int j = 0; j < wysokosc_planszy; j++) {
            cout << plansza[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}

//Funkcja z jabłka (generowania randomowo)
//Funkcja poruszania się (segment na końcu się nie usuwa jak napotyka się na jabłko).
//Funkcja kiedy kończy się gra
//while (true): na początku czas 1000 milisekund, na końcu to co się dzieje jeśli przegrałeś (break).
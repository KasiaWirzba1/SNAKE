#include <SFML/Graphics.hpp>
#include <iostream>
#include <deque>
#include <random>

//Rozmiary okienek
const int CELL_SIZE = 50;
const int WIDTH = 20;
const int HEIGHT = 20;
const float GAME_SPEED = 0.15f;

//Funkcja zmieniająca "znaczenie" komórki
struct Point {
    int x, y;
    Point(int x = 0, int y = 0) : x(x), y(y) {}
    bool operator==(const Point& other) const {
        return x == other.x && y == other.y;
    }
};
//Klasa gry Snake
class Game {
private:
    //inicjalizacja zmiennych
    std::vector<std::vector<int>> board;
    std::deque<Point> snake;
    Point food;
    char direction;
    bool gameOver;
    int score;
    sf::Clock clock;
    float timeSinceLastMove;

    sf::RenderWindow window;
    sf::RectangleShape cell;

    //inicalizacja tablicy i granic
    void initializeBoard() {
        board = std::vector<std::vector<int>>(HEIGHT, std::vector<int>(WIDTH, 0));
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (i == 0 || i == HEIGHT-1 || j == 0 || j == WIDTH-1) {
                    board[i][j] = 4;
                }
            }
        }
    }

    //spawn jedzenia
    void spawnFood() {
        std::random_device rd;  //"klucz do randomizacji"
        std::mt19937 gen(rd());  //generacja randomowego znaczenia
        std::uniform_int_distribution<> x_dist(1, WIDTH-2);  //zakres dystansu generacji jabłka
        std::uniform_int_distribution<> y_dist(1, HEIGHT-2);

        do {
            food = Point(x_dist(gen), y_dist(gen));
        } while (!isCellEmpty(food));

        board[food.y][food.x] = 3;
    }

    //sprawdzanie czy komórka jest pusta(wykorzysuje się dla pojawienia jabłek)
    bool isCellEmpty(const Point & p) const {
        return board[p.y][p.x] == 0;
    }

    //aktualizacja planszy
    void updateBoard() {
        for (int i = 1; i < HEIGHT-1; i++) {
            for (int j = 1; j < WIDTH-1; j++) {
                board[i][j] = 0;
            }
        }

        board[food.y][food.x] = 3;  //dla jedzenia

        for (size_t i = 1; i < snake.size(); i++) {
            board[snake[i].y][snake[i].x] = 2;  //dla ciała węża
        }

        board[snake.front().y][snake.front().x] = 1;  //dla głowy węża
    }

    //Rysowanie(Tablica, Wąż, Ciało, Jabłka, Ściany) z użyciem biblioteki SFML
    void drawBoard() {
        window.clear(sf::Color::Black);

        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                // Konwersja wektorów do float.
                cell.setPosition(sf::Vector2f(static_cast<float>(j * CELL_SIZE),
                                           static_cast<float>(i * CELL_SIZE)));

                // Tło szachownica
                if ((i + j) % 2 == 0) {
                    cell.setFillColor(sf::Color(50, 50, 50));
                } else {
                    cell.setFillColor(sf::Color(80, 80, 80));
                }
                window.draw(cell);

                // Nadanie kolorów innym komórkom
                switch (board[i][j]) {
                    case 1: // Głowa
                        cell.setFillColor(sf::Color::Green);
                        window.draw(cell);
                        break;
                    case 2: // Ciało
                        cell.setFillColor(sf::Color(144, 238, 144));
                        window.draw(cell);
                        break;
                    case 3: // Jabłka
                        cell.setFillColor(sf::Color::Red);
                        window.draw(cell);
                        break;
                    case 4: // Ściany
                        cell.setFillColor(sf::Color::Blue);
                        window.draw(cell);
                        break;
                }
            }
        }

        window.display();
    }
//konstruktor(inicjalizuje grę)
public:
    Game() :
        board(HEIGHT, std::vector<int>(WIDTH)),
        direction('d'),
        gameOver(false),
        score(0),
        timeSinceLastMove(0),
        window(sf::VideoMode(sf::Vector2u(WIDTH * CELL_SIZE, HEIGHT * CELL_SIZE)), "Snake Game")
    {
        cell.setSize(sf::Vector2f(static_cast<float>(CELL_SIZE), static_cast<float>(CELL_SIZE)));
        initializeBoard();
        snake.push_front(Point(WIDTH/2, HEIGHT/2));
        spawnFood();
        window.setFramerateLimit(60);
    }

    //sprawdza i odpowiada za poruszanie się węża
    void processInput() {
    for (auto event = window.pollEvent(); event.has_value(); event = window.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            window.close();
        }
        else if (event->is<sf::Event::KeyPressed>()) {  //w nie może być s...
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W) && direction != 's') direction = 'w';
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S) && direction != 'w') direction = 's';
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::A) && direction != 'd') direction = 'a';
            else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::D) && direction != 'a') direction = 'd';
        }
    }
}
    //odpowiada za czas w grze, dynamikę i koniec gry
    void update() {
        float deltaTime = clock.restart().asSeconds(); //odpowiada za ilość czasu, jaki upłynął
        timeSinceLastMove += deltaTime;

        if (timeSinceLastMove >= GAME_SPEED) {
            Point newHead = snake.front();

            switch (direction) {
                case 'w': newHead.y--; break;
                case 's': newHead.y++; break;
                case 'a': newHead.x--; break;
                case 'd': newHead.x++; break;
            }

            // zasady końca gry
            if (board[newHead.y][newHead.x] == 4 || board[newHead.y][newHead.x] == 2) {
                gameOver = true;
                return;
            }

            //poruszanie się głowy
            snake.push_front(newHead);

            //poruszanie się ciała
            if (newHead == food) {
                score++;
                spawnFood();
            } else {
                snake.pop_back();
            }

            updateBoard();
            timeSinceLastMove = 0;
        }
    }
    //funkcja która łączy wszystkie procesy w grę
    void run() {
        while (window.isOpen() && !gameOver) {
            processInput();
            update();
            drawBoard();
        }

        if (gameOver) {
            std::cout << "Game Over! Final Score: " << score << std::endl;
        }
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}
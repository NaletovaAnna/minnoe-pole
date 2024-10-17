#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

const int GRID_SIZE = 10;    // ������ ����� (10x10)
const int CELL_SIZE = 50;    // ������ ������
const int MINE_COUNT = 10;    // ���������� ���

enum CellState { EMPTY, MINE, REVEALED };

struct Cell {
    CellState state;
    bool isStart;
    bool isEnd;

    Cell() : state(EMPTY), isStart(false), isEnd(false) {}
};

class Minesweeper {
public:
    Minesweeper() {
        grid.resize(GRID_SIZE, std::vector<Cell>(GRID_SIZE));
        srand(static_cast<unsigned int>(time(0)));
        placeMines();
        start.x = 0;
        start.y = 0;
        end.x = GRID_SIZE - 1;
        end.y = GRID_SIZE - 1;
        grid[start.x][start.y].isStart = true;
        grid[end.x][end.y].isEnd = true;
    }

    void placeMines() {
        int placedMines = 0;
        while (placedMines < MINE_COUNT) {
            int x = rand() % GRID_SIZE;
            int y = rand() % GRID_SIZE;
            if (grid[x][y].state != MINE) {
                grid[x][y].state = MINE;
                placedMines++;
            }
        }
    }

    void revealGrid(sf::RenderWindow& window) {
        // ����������� ��� �� 2 �������
        for (int x = 0; x < GRID_SIZE; ++x) {
            for (int y = 0; y < GRID_SIZE; ++y) {
                if (grid[x][y].state == MINE) {
                    sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                    cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);
                    cell.setFillColor(sf::Color::Red);
                    window.draw(cell);
                }
            }
        }
        window.display();
        sf::sleep(sf::seconds(2));
        window.clear();
    }

    void draw(sf::RenderWindow& window) {
        for (int x = 0; x < GRID_SIZE; ++x) {
            for (int y = 0; y < GRID_SIZE; ++y) {
                sf::RectangleShape cell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
                cell.setPosition(x * CELL_SIZE, y * CELL_SIZE);

                // ��������� ����� ��� ������
                if (grid[x][y].state == REVEALED) {
                    if (grid[x][y].state == MINE) {
                        cell.setFillColor(sf::Color::Red); // ������� ��� ����
                    }
                    else {
                        cell.setFillColor(sf::Color::White); // ����� ��� ��������� ������
                    }
                }
                else {
                    cell.setFillColor(sf::Color::Black); // ������ ��� �������� ������
                }

                window.draw(cell);
            }
        }

        // ����������� ��������� � �������� �����
        sf::RectangleShape startCell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        startCell.setPosition(start.x * CELL_SIZE, start.y * CELL_SIZE);
        startCell.setFillColor(sf::Color::Green);
        window.draw(startCell);

        sf::RectangleShape endCell(sf::Vector2f(CELL_SIZE, CELL_SIZE));
        endCell.setPosition(end.x * CELL_SIZE, end.y * CELL_SIZE);
        endCell.setFillColor(sf::Color::Blue);
        window.draw(endCell);
    }

    void movePlayer(int dx, int dy) {
        int newX = start.x + dx;
        int newY = start.y + dy;

        // �������� ������
        if (newX >= 0 && newX < GRID_SIZE && newY >= 0 && newY < GRID_SIZE) {
            if (grid[newX][newY].state != MINE) {
                start.x = newX;
                start.y = newY;
                grid[start.x][start.y].state = REVEALED;

                if (start.x == end.x && start.y == end.y) {
                    std::cout << "You reached the end!" << std::endl;
                }
            }
            else {
                std::cout << "Game Over! You hit a mine." << std::endl;
            }
        }
    }

private:
    std::vector<std::vector<Cell>> grid;
    sf::Vector2i start, end;
};

int main() {
    sf::RenderWindow window(sf::VideoMode(GRID_SIZE * CELL_SIZE, GRID_SIZE * CELL_SIZE), "Minesweeper");
    Minesweeper game;

    game.revealGrid(window); // �������� ���� �� 2 �������

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    game.movePlayer(0, -1);  // �������� �����
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    game.movePlayer(0, 1);   // �������� ����
                }
                else if (event.key.code == sf::Keyboard::Left) {
                    game.movePlayer(-1, 0);  // �������� �����
                }
                else if (event.key.code == sf::Keyboard::Right) {
                    game.movePlayer(1, 0);   // �������� ������
                }
            }
        }

        window.clear();
        game.draw(window);
        window.display();
    }

    return 0;
}

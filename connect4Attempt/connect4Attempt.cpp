#include <SFML/Graphics.hpp>

#include <algorithm>

#include <iostream>

enum class Colors {
   None = 0,
      Red = 1,
      Yellow = 2
};

const size_t ROWS = 6,
   COLUMNS = 7;
const size_t WINDOWX = 800,
   WINDOWY = 800;

sf::RenderWindow window(sf::VideoMode(WINDOWX, WINDOWY), "SFML works!");

void drawConnect4(const Colors( & board)[ROWS][COLUMNS], size_t selectIdx) {
   constexpr float radius = WINDOWX / (COLUMNS + .6) / 2;
   const float padding = 1.1f;

   sf::Texture arrowTexture;
   if (!arrowTexture.loadFromFile("arrow.png")) return;
   arrowTexture.setSmooth(true);

   sf::Sprite arrowSprite(arrowTexture);
   arrowSprite.setScale(0.2, 0.2);
   arrowSprite.setPosition(selectIdx * radius * 2 * padding, 0);

   window.draw(arrowSprite);
   sf::CircleShape circle(radius);

   for (size_t i = 0; i < ROWS; i++) {
      for (size_t j = 0; j < COLUMNS; j++) {
         circle.setPosition(j * radius * 2 * padding, i * radius * 2 * padding + 110.f);
         sf::Color red(245, 66, 66);
         sf::Color yellow(245, 239, 66);
         switch (board[i][j]) {
         case Colors::None:
            circle.setFillColor(sf::Color::White);
            break;
         case Colors::Red:
            circle.setFillColor(red);
            break;
         case Colors::Yellow:
            circle.setFillColor(yellow);
            break;
         default:
            std::cout << "\nUnexpected color value";
            break;
         }
         window.draw(circle);
      }
   }
}

bool didWin(Colors( & board)[ROWS][COLUMNS], size_t xPos, size_t yPos) {
   Colors winningColor = board[yPos][xPos];
   //horizontal
   unsigned int count = 0;
   for (int i = xPos + 1; i < COLUMNS &&
       count < 3 &&
       board[yPos][i] == winningColor; i++) {
       count++;
   }
   for (int i = xPos - 1; i >= 0 &&
       count < 3 &&
       board[yPos][i] == winningColor; i--) {
       count++;
   }
   if (count == 3) return true;

   //vertical
   count = 0;
   for (int i = yPos + 1; i < ROWS &&
       count < 3 &&
       board[i][xPos] == winningColor; i++) {
       count++;
   }
   for (int i = yPos - 1; i >= 0 &&
       count < 3 &&
       board[i][xPos] == winningColor; i++) {
       count++;
   }
   if (count == 3) return true;

   //diagonal
   //f(x)
   count = 0;
   for (int x = xPos + 1, y = yPos - 1; x < COLUMNS &&
       y >= 0 &&
       count < 3 &&
       board[y][x] == winningColor;) {
       count++;
       y--;
       x++;
   }
   for (int x = xPos - 1, y = yPos + 1; x >= 0 &&
       y < ROWS &&
       count < 3 &&
       board[y][x] == winningColor;) {
       count++;
       y++;
       x--;
   }
   if (count == 3) return true;
   //f(-x)
   count = 0;
   for (int x = xPos - 1, y = yPos - 1; x >= 0 &&
       y >= 0 &&
       count < 3 &&
       board[y][x] == winningColor;) {
       count++;
       y--;
       x--;
   }
   for (int x = xPos + 1, y = yPos + 1; x < COLUMNS &&
       y < ROWS &&
       count < 3 &&
       board[y][x] == winningColor;) {
       count++;
       y++;
       x++;
   }
   if (count == 3) return true;
   return false;
}

sf::Vector2i insertToken(Colors( & board)[ROWS][COLUMNS], size_t selectIdx, Colors color) {
   for (int  i = ROWS - 1; i >= 0; i--) {
      if (board[i][selectIdx] == Colors::None) {
         board[i][selectIdx] = color;
         sf::Vector2i pos(selectIdx, i);
         return pos;
      }
   }
   sf::Vector2i noResult(-1, -1);
   return noResult;
}

int main() {
   sf::CircleShape shape(100.f);
   shape.setFillColor(sf::Color::Green);

   Colors connect4[ROWS][COLUMNS] = {};


   size_t indexSelect = 0;
   bool focused = true;
   Colors playerTurn = Colors::Red;
   bool gameEnded = false;
   std::string winString;

   while (window.isOpen()) {
      sf::Event event;
      while (window.pollEvent(event)) {
         if (event.type == sf::Event::Closed)
            window.close();
         if (event.type == sf::Event::LostFocus)
            focused = false;
         if (event.type == sf::Event::GainedFocus)
            focused = true;
         if (event.type == sf::Event::MouseMoved && focused) {
            indexSelect = (int)((float) event.mouseMove.x / WINDOWX * COLUMNS);
         }
         if (event.type == sf::Event::MouseButtonPressed && !gameEnded) {
            if (event.mouseButton.button == sf::Mouse::Left && focused) {
               sf::Vector2i position = insertToken(connect4, indexSelect, playerTurn);
               if (position.x != -1 && position.y != -1) {
                   if (didWin(connect4, position.x, position.y)) {
                       gameEnded = true;
                       std::string player = (playerTurn == Colors::Red ? "Red" : "Yellow");
                       winString = player + " won!";
                   }
                   if (playerTurn == Colors::Red) playerTurn = Colors::Yellow;
                   else playerTurn = Colors::Red;
               }
            }
         }
      }
      sf::Color board(66, 135, 245);
      window.clear(board);
      drawConnect4(connect4, indexSelect);
      if (gameEnded) {
          sf::Text winText;
          sf::Font font;
          font.loadFromFile("arial.ttf");
          winText.setFont(font);
          winText.setString(winString);
          winText.setCharacterSize(60);
          winText.setStyle(sf::Text::Bold);
          winText.setFillColor(sf::Color::White);
          sf::FloatRect rc = winText.getLocalBounds();
          winText.setOrigin(rc.width / 2, rc.height / 2);
          winText.setPosition((float)WINDOWX / 2, 20.f);
          sf::Text shadow = winText;
          shadow.setFillColor(sf::Color::Black);
          shadow.move(2.f, 2.f);
          window.draw(shadow);
          window.draw(winText);
      }
      window.display();
   }

   return 0;
}
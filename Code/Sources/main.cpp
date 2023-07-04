#include <iostream>
#include <fstream>
#include <math.h>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include "SFML/Audio.hpp"

#include "../Headers/Board.h"

//TODO clean up main, change variable names, split to functions (main loop), move setup elsewhere
//TODO verify strange OpenAL warnings on some computers, is it because of disabled sound device?
void setup(float *xMax, float *yMax, int *iMax, int *jMax, std::string *imageName);  //loads information about image size, name and number of parts it is to be divided into from setup file

int main()
{
    srand(time(NULL));

    sf::RenderWindow window( sf::VideoMode( ScreenX, ScreenY, 32 ), "PuzzleWithSFML", sf::Style::Fullscreen );
    sf::Music music, victorySound;
    music.openFromFile("Resources/Music/deep-ambient.ogg");
    victorySound.openFromFile("Resources/Music/bicycle-bell.ogg");
    music.setLoop(true);

    float xMax, yMax; //width and length of the image
    int iMax, jMax; //number of rows and columns the image will be divided into
    std::string imageName;   //name of the image and its extension


    setup(&xMax, &yMax, &iMax, &jMax, &imageName);
    Board board(iMax, jMax);
    board.setImageInfo(xMax, yMax, imageName);
    board.loadImage();
    board.setValuesOfAllPieces();
    board.shufflePieces();
    board.drawAll(window);

    music.play();

    bool isPressed=0;
    int mousePressPointX=0, mousePressPointY=0;
    int mouseCurrentPositionX=0, mouseCurrentPositionY=0;

    Piece* piece1;
    Piece* piece2;
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent( event ))
        {
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                window.close();
                break;
            }
            if (isPressed==0)
            {
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    isPressed=1;
                    mousePressPointX=sf::Mouse::getPosition(window).x;
                    mousePressPointY=sf::Mouse::getPosition(window).y;
                    piece1 = board.identifyPieceByPosition(mousePressPointX, mousePressPointY);
                }
            }
            else
            {
                if (event.type == sf::Event::MouseMoved)
                {
                    mouseCurrentPositionX=sf::Mouse::getPosition(window).x;
                    mouseCurrentPositionY=sf::Mouse::getPosition(window).y;
                    piece1->movePieceByMouse(mouseCurrentPositionX-mousePressPointX, mouseCurrentPositionY-mousePressPointY);
                    board.drawAll(window, *piece1);
                }
                else if (event.type == sf::Event::MouseButtonReleased)
                {
                    isPressed=0;
                    mousePressPointX=sf::Mouse::getPosition(window).x;
                    mousePressPointY=sf::Mouse::getPosition(window).y;
                    piece1->restorePiecePosition();
                    piece2 = board.identifyPieceByPosition(mousePressPointX, mousePressPointY);
                    Piece::swapPiecePositions(*piece1, *piece2);
                    board.drawAll(window);
                    if (board.checkForVictory())
                    {
                        music.setPlayingOffset(sf::seconds(0));
                        victorySound.play();
                    }
                }
            }
        }
    }
    return (0);
}

void setup(float *xMax, float *yMax, int *iMax, int *jMax, std::string *imageName)   //loads information about image size, name and number of parts it is to be divided into from setup file
{
    char s;
    std::ifstream setupFile("Resources/setup.txt");
    while (setupFile>>s) if (s=='#') setupFile>>*xMax>>*yMax>>*iMax>>*jMax>>*imageName;
    setupFile.close();
}

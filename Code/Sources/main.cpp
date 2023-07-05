#include <iostream>
#include <fstream>
#include <math.h>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include "SFML/Audio.hpp"

#include "../Headers/Board.h"

//TODO split main to functions (main loop)?, move setup elsewhere
//TODO verify strange OpenAL warnings on some computers, is it because of disabled sound device?

void setup(int *boardColumns, int *boardRows, std::string *imageName);  //loads information about image size, name and number of parts it is to be divided into from setup file

int main()
{
    //randomizing the rand function seed
    srand(time(NULL));

    //creating main window
    sf::RenderWindow window( sf::VideoMode( ScreenX, ScreenY, 32 ), "PuzzleWithSFML", sf::Style::Fullscreen );

    //variables for the image related parameters
    float imageSizeX, imageSizeY; //width and length of the image
    int boardColumns, boardRows; //number of rows and columns the image will be divided into
    std::string imageFilename; //name of the image and its extension

    //setting up the playing board, its pieces, shuffling them and drawing for the first time
    setup(&boardColumns, &boardRows, &imageFilename);
    Board board(boardColumns, boardRows);
    //board.setImageInfo(imageSizeX, imageSizeY, imageName);
    board.loadImage(imageFilename);
    board.setValuesOfAllPieces();
    board.shufflePieces();
    board.drawAll(window);

    //variables for the main loop that regard the mouse
    bool isLeftMouseButtonHeld=false;
    float mousePressPositionX=0, mousePressPositionY=0;//mouse position in SFML is technically an integer, but we will have divisions later and require floats//TODO verify game functionality with all positions as integers
    float mouseCurrentPositionX=0, mouseCurrentPositionY=0;//these update only when left mouse button is pressed!

    //two pointers that will hold up to two pieces when the mouse is pressed (needs to remember on which piece) and released (needs to know on which piece to swap them)
    Piece* piece1;
    Piece* piece2;

    //creating music variables and playing the main one
    sf::Music music, victorySound;
    music.openFromFile("Resources/Music/deep-ambient.ogg");
    victorySound.openFromFile("Resources/Music/bicycle-bell.ogg");
    music.setLoop(true);
    music.play();

    //main loop, it will continuously poll events, read them and terminate only if the window closes
    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent( event ))
        {
            //if Escape key is pressed the window should close, ending the game and terminating the program
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape)
            {
                window.close();
                break;
            }
            //for any other key the first thing to check is whether the left mouse button is currently held, if yes then process mouse movement and button release events, otherwise button press events
            if (isLeftMouseButtonHeld)
            {
                //if the mouse moved (while left mouse button is held) the game should move the held piece with the mouse and redraw the board
                if (event.type == sf::Event::MouseMoved)
                {
                    mouseCurrentPositionX=sf::Mouse::getPosition(window).x;
                    mouseCurrentPositionY=sf::Mouse::getPosition(window).y;
                    piece1->movePieceByMouse(mouseCurrentPositionX-mousePressPositionX, mouseCurrentPositionY-mousePressPositionY);
                    board.drawAll(window, *piece1);
                }
                //if the left mouse button is released the game should put the currently held piece back to its original position, then swap it with the one that it was dropped on, redraw board and check victory condition
                else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
                {
                    isLeftMouseButtonHeld=false;
                    mousePressPositionX=sf::Mouse::getPosition(window).x;
                    mousePressPositionY=sf::Mouse::getPosition(window).y;
                    piece1->restorePiecePosition();
                    piece2 = board.identifyPieceByPosition(mousePressPositionX, mousePressPositionY);
                    Piece::swapPiecePositions(*piece1, *piece2);
                    board.drawAll(window);
                    if (board.checkForVictory())
                    {
                        music.setPlayingOffset(sf::seconds(0));
                        victorySound.play();
                    }
                }
            }
            else
            {
                //meanwhile if the left mouse button isn't held but was just pressed the game should "pick up" the piece under it
                if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
                {
                    isLeftMouseButtonHeld=true;
                    mousePressPositionX=sf::Mouse::getPosition(window).x;
                    mousePressPositionY=sf::Mouse::getPosition(window).y;
                    piece1 = board.identifyPieceByPosition(mousePressPositionX, mousePressPositionY);
                }
            }
        }
    }
    return 0;
}

void setup(int *boardColumns, int *boardRows, std::string *imageName)   //loads information about image size, name and number of parts it is to be divided into from setup file
{
    char signBuffer;
    std::ifstream setupFile("Resources/setup.txt");
    while (setupFile>>signBuffer)
    {
        if (signBuffer=='#')
        {
            setupFile>>*boardColumns>>*boardRows>>*imageName;
        }
    }
    setupFile.close();
}

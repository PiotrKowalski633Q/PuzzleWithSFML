#ifndef PUZZLEWITHSFML_BOARD_H
#define PUZZLEWITHSFML_BOARD_H

#include <SFML/Graphics.hpp>
#include <wtypes.h> //purely for screen resolution

#include "Piece.h"

//TODO maybe change names and places later
#define ScreenX GetSystemMetrics(SM_CXSCREEN)
#define ScreenY GetSystemMetrics(SM_CYSCREEN)

class Board
{
private:
    Piece *mPuzzles;
    int mRows, mColumns;

    float mImageSizeX, mImageSizeY;
    sf::Texture mImage;
    //std::string mImageFilename;

    float mOffsetXBetweenPieces, mOffsetYBetweenPieces;

public:
    Board(int rows, int columns);
    ~Board();

    void loadImage(std::string imageFilename);
    void setValuesOfAllPieces();
    void shufflePieces();
    Piece* identifyPieceByPosition(int positionX, int positionY);
    void drawAll(sf::RenderWindow &window);
    void drawAll(sf::RenderWindow &window, Piece &movingPiece);
    bool checkForVictory();//TODO hasWon?

};

#endif //PUZZLEWITHSFML_BOARD_H

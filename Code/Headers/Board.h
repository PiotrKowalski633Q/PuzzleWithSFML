#ifndef PUZZLEWITHSFML_BOARD_H
#define PUZZLEWITHSFML_BOARD_H

#include <fstream>
#include <wtypes.h>

#include <SFML/Graphics.hpp>

#include "Piece.h"

//TODO maybe change names and places later
#define ScreenX GetSystemMetrics(SM_CXSCREEN)
#define ScreenY GetSystemMetrics(SM_CYSCREEN)

class Board
{
private:
    Piece *mPuzzles;//TODO rename to mPieces for consistency
    int mRows, mColumns;

    float mImageSizeX, mImageSizeY;
    sf::Texture mImage;
    std::string mImageFilename;

    float mOffsetXBetweenPieces, mOffsetYBetweenPieces;//TODO mRowHeight and mColumnWidth?

public:
    Board(const std::string &setupFilepath);//TODO explicit?
    ~Board();

    void setValuesOfAllPieces();//TODO this too can be integrated into constructor
    void shufflePieces();
    void drawAll(sf::RenderWindow &window);
    void drawAll(sf::RenderWindow &window, Piece &movingPiece);
    Piece* identifyPieceByPosition(int positionX, int positionY);
    bool checkForVictory();//TODO hasWon?

private:
    void setupBoardDataFromFile(const std::string &filepath);
    void loadImage();
};

#endif //PUZZLEWITHSFML_BOARD_H

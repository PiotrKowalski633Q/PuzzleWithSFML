#ifndef PUZZLEWITHSFML_BOARD_H
#define PUZZLEWITHSFML_BOARD_H

#include <fstream>

#include <SFML/Graphics.hpp>

#include "Piece.h"

class Board
{
private:
    float mBoardSizeX, mBoardSizeY;
    int mRows, mColumns;
    float mPieceSizeX, mPieceSizeY;//common across all pieces

    float mImageSizeX, mImageSizeY;
    sf::Texture mImage;
    std::string mImageFilename;

    Piece *mPieces;

public:
    Board(float boardSizeX, float BoardSizeY, const std::string &setupFilepath);//TODO explicit?
    ~Board();

    void setValuesOfAllPieces();//TODO this too can be integrated into constructor
    void shufflePieces();
    void drawAll(sf::RenderWindow &window);
    void drawAll(sf::RenderWindow &window, Piece &movingPiece);
    Piece* identifyPieceByPosition(int positionX, int positionY);
    bool arePiecesInCorrectOrder();

private:
    void setupBoardDataFromFile(const std::string &filepath);
    void loadImage();
};

#endif //PUZZLEWITHSFML_BOARD_H

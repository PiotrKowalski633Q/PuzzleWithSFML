#ifndef PUZZLEWITHSFML_PIECE_H
#define PUZZLEWITHSFML_PIECE_H

#include <SFML/Graphics.hpp>

class Piece
{
private:
    float mPositionX, mPositionY;
    int mIndexX, mIndexY;
    float mMovedDistanceX, mMovedDistanceY;

    sf::Sprite mSprite;

public:
    int getIndexX();
    int getIndexY();

    void setValues(float positionX, float positionY, int indexX, int indexY, float scaleX, float scaleY, sf::Texture &image, sf::IntRect spriteRectangle);
    static void swapPiecePositions(Piece& piece1, Piece& piece2);
    bool isPositionWithinPiece(float positionX, float positionY, float offsetXBetweenPieces, float offsetYBetweenPieces);
    void draw(sf::RenderWindow &window);
    void movePieceByMouse(float displacementX, float displacementY);
    void restorePiecePosition();
};

#endif //PUZZLEWITHSFML_PIECE_H

#include "../Headers/Piece.h"

int Piece::getIndexX()
{
    return mIndexX;
}

int Piece::getIndexY()
{
    return mIndexY;
}

void Piece::setValues(float positionX, float positionY, int indexX, int indexY, float scaleX, float scaleY, sf::Texture &image, sf::IntRect spriteRectangle)
{
    mMovedDistanceX = 0;
    mMovedDistanceY = 0;
    mPositionX = positionX;
    mPositionY = positionY;
    mIndexX = indexX;
    mIndexY = indexY;
    mSprite.setTexture(image);
    mSprite.setScale(scaleX, scaleY);
    mSprite.setOrigin(0, 0);
    mSprite.setTextureRect(spriteRectangle);
}

void Piece::swapPiecePositions(Piece& piece1, Piece& piece2)
{
    float bufferF;
    int bufferI;

    bufferF=piece1.mPositionX;
    piece1.mPositionX=piece2.mPositionX;
    piece2.mPositionX=bufferF;

    bufferF=piece1.mPositionY;
    piece1.mPositionY=piece2.mPositionY;
    piece2.mPositionY=bufferF;

    bufferI=piece1.mIndexX;
    piece1.mIndexX=piece2.mIndexX;
    piece2.mIndexX=bufferI;

    bufferI=piece1.mIndexY;
    piece1.mIndexY=piece2.mIndexY;
    piece2.mIndexY=bufferI;
}

bool Piece::isPositionWithinPiece(float positionX, float positionY, float offsetXBetweenPieces, float offsetYBetweenPieces)
{
    if (mPositionX<positionX && mPositionY<positionY && mPositionX+offsetXBetweenPieces>positionX && mPositionY+offsetYBetweenPieces>positionY)
    {
        return true;
    }
    return false;
}

void Piece::draw(sf::RenderWindow &window)
{
    mSprite.setPosition(mPositionX, mPositionY);
    window.draw(mSprite);
}

void Piece::movePieceByMouse(float displacementX, float displacementY)
{
    mPositionX-=mMovedDistanceX;
    mPositionY-=mMovedDistanceY;
    mMovedDistanceX=displacementX;
    mMovedDistanceY=displacementY;
    mPositionX+=mMovedDistanceX;
    mPositionY+=mMovedDistanceY;
}

void Piece::restorePiecePosition()
{
    mPositionX-=mMovedDistanceX;
    mPositionY-=mMovedDistanceY;
    mMovedDistanceX=0;
    mMovedDistanceY=0;
}

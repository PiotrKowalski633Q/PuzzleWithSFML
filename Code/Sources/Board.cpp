#include "../Headers/Board.h"

Board::Board(int rows, int columns)
{
    mRows = rows;
    mColumns = columns;
    mOffsetXBetweenPieces = ((float)ScreenX / mColumns);
    mOffsetYBetweenPieces = ((float)ScreenY / mRows);
    mPuzzles = new Piece[rows*columns];
}

Board::~Board()
{
    delete[] mPuzzles;
}

void Board::setImageInfo(float imageSizeX, float imageSizeY, std::string imageFilename)
{
    mImageSizeX = imageSizeX;
    mImageSizeY = imageSizeY;
    mImageFilename = imageFilename;
}

void Board::loadImage()
{
    mImage.loadFromFile("Resources/Images/" + mImageFilename);    //image set should have resolution at least equal to the resolution of used screen, not lower, else it might be displayed incorrectly
    //TODO retrieve image size here (mImage.imageSize()), then setImageInfo can be deleted
}

void Board::setValuesOfAllPieces()
{
    for (int rowNumber=0; rowNumber<mRows; rowNumber++)
    {
        for (int columnNumber=0; columnNumber<mColumns; columnNumber++)
        {
            //TODO verify float conversion, verify if rectangle would need scale
            float positionX = mOffsetXBetweenPieces * columnNumber;
            float positionY = mOffsetYBetweenPieces * rowNumber;
            int indexX = columnNumber;
            int indexY = rowNumber;
            float scaleX = ScreenX / mImageSizeX;
            float scaleY = ScreenY / mImageSizeY;
            sf::IntRect spriteRectangle(positionX*mImageSizeX/ScreenX, positionY*mImageSizeY/ScreenY, mImageSizeX/mColumns, mImageSizeY/mRows);

            (mPuzzles[rowNumber*mColumns+columnNumber]).setValues(positionX, positionY, indexX, indexY, scaleX, scaleY, mImage, spriteRectangle);
        }
    }
}

void Board::shufflePieces()
{
    for (int rowNumber=0; rowNumber<mRows; rowNumber++)
    {
        for (int columnNumber=0; columnNumber<mColumns; columnNumber++)
        {
            //TODO verify random function
            Piece::swapPiecePositions( mPuzzles[rowNumber*mColumns+columnNumber], mPuzzles[ (rand() % mColumns)*mRows + (rand() % mRows) ] );
        }
    }
}

Piece* Board::identifyPieceByPosition(int positionX, int positionY)
{
    for (int rowNumber=0; rowNumber<mRows; rowNumber++)
    {
        for (int columnNumber=0; columnNumber<mColumns; columnNumber++)
        {
            if (mPuzzles[rowNumber*mColumns+columnNumber].isPositionWithinPiece(positionX, positionY, mOffsetXBetweenPieces, mOffsetYBetweenPieces))
            {
                return &(mPuzzles[rowNumber*mColumns+columnNumber]);
            }
        }
    }
    return nullptr; //TODO add cases for when this nullptr is returned (even though it never should, not in a properly setup board)
}

void Board::drawAll(sf::RenderWindow &window)
{
    window.clear();
    for (int rowNumber=0; rowNumber<mRows; rowNumber++)
    {
        for (int columnNumber=0; columnNumber<mColumns; columnNumber++)
        {
            mPuzzles[rowNumber*mColumns+columnNumber].draw(window);
        }
    }
    window.display();
}

void Board::drawAll(sf::RenderWindow &window, Piece &movingPiece)
{
    window.clear();
    for (int rowNumber=0; rowNumber<mRows; rowNumber++)
    {
        for (int columnNumber=0; columnNumber<mColumns; columnNumber++)
        {
            mPuzzles[rowNumber*mColumns+columnNumber].draw(window);
        }
    }
    movingPiece.draw(window);  //added for purposes of drawing the moving piece on the top
    window.display();
}

bool Board::checkForVictory()
{
    for (int rowNumber=0; rowNumber<mRows; rowNumber++)
    {
        for (int columnNumber=0; columnNumber<mColumns; columnNumber++)
        {
            if ( mPuzzles[rowNumber*mColumns+columnNumber].getIndexX()!=columnNumber || mPuzzles[rowNumber*mColumns+columnNumber].getIndexY()!=rowNumber)
            {
                return false;
            }
        }
    }
    return true;
}


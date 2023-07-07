#include "../Headers/Board.h"

Board::Board(float boardSizeX, float boardSizeY, const std::string &setupFilepath)
{
    mBoardSizeX = boardSizeX;
    mBoardSizeY = boardSizeY;

    setupBoardDataFromFile(setupFilepath);
    loadImage();

    mPieces = new Piece[mRows*mColumns];
}

Board::~Board()
{
    delete[] mPieces;
}

void Board::setupBoardDataFromFile(const std::string &filepath)
{
    char signBuffer;
    std::string imageFilename;

    std::ifstream setupFile(filepath);
    while (setupFile>>signBuffer)
    {
        if (signBuffer=='#')
        {
            setupFile>>mColumns>>mRows>>mImageFilename;
        }
    }
    setupFile.close();

    mPieceSizeX = ((float)mBoardSizeX / mColumns);
    mPieceSizeY = ((float)mBoardSizeY / mRows);
}

void Board::loadImage()
{
    mImage.loadFromFile("Resources/Images/" + mImageFilename);    //image set should have resolution at least equal to the resolution of used screen, not lower, else it might be displayed incorrectly
    mImageSizeX = mImage.getSize().x;
    mImageSizeY = mImage.getSize().y;
}

void Board::setValuesOfAllPieces()
{
    for (int rowNumber=0; rowNumber<mRows; rowNumber++)
    {
        for (int columnNumber=0; columnNumber<mColumns; columnNumber++)
        {
            //TODO verify float conversion, verify if rectangle would need scale
            float positionX = mPieceSizeX * columnNumber;
            float positionY = mPieceSizeY * rowNumber;
            int indexX = columnNumber;
            int indexY = rowNumber;
            float scaleX = mBoardSizeX / mImageSizeX;
            float scaleY = mBoardSizeY / mImageSizeY;
            sf::IntRect spriteRectangle(positionX*mImageSizeX/mBoardSizeX, positionY*mImageSizeY/mBoardSizeY, mImageSizeX/mColumns, mImageSizeY/mRows);

            (mPieces[rowNumber*mColumns+columnNumber]).setValues(positionX, positionY, indexX, indexY, scaleX, scaleY, mImage, spriteRectangle);
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
            Piece::swapPiecePositions( mPieces[rowNumber*mColumns+columnNumber], mPieces[ (rand() % mColumns)*mRows + (rand() % mRows) ] );
        }
    }
}

void Board::drawAll(sf::RenderWindow &window)
{
    window.clear();
    for (int rowNumber=0; rowNumber<mRows; rowNumber++)
    {
        for (int columnNumber=0; columnNumber<mColumns; columnNumber++)
        {
            mPieces[rowNumber*mColumns+columnNumber].draw(window);
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
            mPieces[rowNumber*mColumns+columnNumber].draw(window);
        }
    }
    movingPiece.draw(window);  //added for purposes of drawing the moving piece on the top
    window.display();
}

Piece* Board::identifyPieceByPosition(int positionX, int positionY)
{
    for (int rowNumber=0; rowNumber<mRows; rowNumber++)
    {
        for (int columnNumber=0; columnNumber<mColumns; columnNumber++)
        {
            if (mPieces[rowNumber*mColumns+columnNumber].isPositionWithinPiece(positionX, positionY, mPieceSizeX, mPieceSizeY))
            {
                return &(mPieces[rowNumber*mColumns+columnNumber]);
            }
        }
    }
    return nullptr; //TODO add cases for when this nullptr is returned (even though it never should, not in a properly setup board)
}

bool Board::arePiecesInCorrectOrder()
{
    for (int rowNumber=0; rowNumber<mRows; rowNumber++)
    {
        for (int columnNumber=0; columnNumber<mColumns; columnNumber++)
        {
            if ( mPieces[rowNumber*mColumns+columnNumber].getIndexX()!=columnNumber || mPieces[rowNumber*mColumns+columnNumber].getIndexY()!=rowNumber)
            {
                return false;
            }
        }
    }
    return true;
}


#include <iostream>
#include <fstream>
#include <math.h>
#include "SFML/Graphics.hpp"
#include "SFML/Window.hpp"
#include "SFML/System.hpp"
#include "SFML/Audio.hpp"
#include <wtypes.h> //purely for screen resolution

#define ScreenX GetSystemMetrics(SM_CXSCREEN)
#define ScreenY GetSystemMetrics(SM_CYSCREEN)

using namespace std;

sf::RenderWindow okno( sf::VideoMode( ScreenX, ScreenY, 32 ), "PuzzleWithSFML", sf::Style::Fullscreen );

class Parts
{
private:
    float x, y; //position of the part object
    int i, j;   //vertical and horizontal number of the part
    static float xMax, yMax;  //vertical and horizontal size of the image used
    static int iMax, jMax;  //number of rows and columns that parts will be created in
    int addTempX, addTempY;    //values added to x and y to restore them after moving animation
    static sf::Texture image;   //image used
    sf::Sprite sprite;  //sprite holding the texture of part object
    static string imageName;    //name and extension of the image to be used in program
public:
    static void setImageInfo(float xMaxG, float yMaxG, int iMaxG, int jMaxG, string imageNameG)    //setting width and length of the image as well as number of rows and columns it will be divided into
    {
        imageName=imageNameG;
        xMax=xMaxG;
        yMax=yMaxG;
        iMax=iMaxG;
        jMax=jMaxG;
    }
    static void loadImage() //loading image used
    {
        image.loadFromFile("Resources/Images/" + imageName);    //image set should have resolution at least equal to the resolution of used screen, not lower, else it will be displayed incorrectly
    }
    void setValues(int iG, int jG)  //setting position of the part object meaning its row and column number
    {
        this->addTempX=0;
        this->addTempY=0;
        this->x=(ScreenX/(iMax))*iG;
        this->y=(ScreenY/(jMax))*jG;
        this->i=iG;
        this->j=jG;
        this->sprite.setTexture(image);
        this->sprite.setScale(ScreenX/xMax, ScreenY/yMax);
        this->sprite.setOrigin(0,0);
        this->sprite.setTextureRect( sf::IntRect( x*xMax/ScreenX, y*yMax/ScreenY, (xMax/iMax), (yMax/jMax) ) ); //rectangle from starting point defined by first two arguments with width and length defined by the other two
    }
    static void setValuesAll(Parts *partsTab)   //sets values of i, j for all parts
    {
        for (int k=0; k<iMax; k++)
        {
            for (int l=0; l<jMax; l++)
            {
                (partsTab[k*jMax+l]).setValues(k, l);
            }
        }
    }
    void draw() //providing necessary information to part objects sprite and drawing it (but not displaying)
    {
        this->sprite.setPosition(x,y);
        okno.draw(this->sprite);
    }
    static void drawAll(Parts *partsTab, int lastI, int lastJ)    //draws and displays all parts and additionally part i,j at the end
    {
        okno.clear();
        for (int k=0; k<iMax; k++)
        {
            for (int l=0; l<jMax; l++)
            {
                (partsTab[k*jMax+l]).draw();
            }
        }
        partsTab[lastI*jMax+lastJ].draw();  //added for purposes of drawing the moving part on the top
        okno.display();
    }
    static void swapPartsPositions(Parts& part1, Parts& part2)    //swaps positions between two parts (both their x,y positions as well as i,j)
    {
        float buforF;
        int buforI;

        buforF=part1.x;
        part1.x=part2.x;
        part2.x=buforF;

        buforF=part1.y;
        part1.y=part2.y;
        part2.y=buforF;

        buforI=part1.i;
        part1.i=part2.i;
        part2.i=buforI;

        buforI=part1.j;
        part1.j=part2.j;
        part2.j=buforI;
    }
    static void shuffleParts(Parts *partsTab)   //shuffles all parts, had to consider their array as one-dimensional
    {
        for (int k=0; k<iMax; k++)
        {
            for (int l=0; l<jMax; l++)
            {
                Parts::swapPartsPositions( partsTab[k*jMax+l], partsTab[ (rand() % iMax)*jMax + (rand() % jMax) ] );    //swaps a certain k,l part with a random other
            }
        }
    }
    static void identifyClickedPart(Parts *partsTab, int *iValue, int *jValue, int mousePressPointX, int mousePressPointY)  //identifies which partsTab[i][j] the mouse is currently upon by given position of the mouse and returns i,j values
    {
        for (int k=0; k<iMax; k++)
        {
            for (int l=0; l<jMax; l++)
            {
                if ((partsTab[k*jMax+l]).x<mousePressPointX && (partsTab[k*jMax+l]).y<mousePressPointY && (partsTab[k*jMax+l]).x+(ScreenX/(iMax))>mousePressPointX && (partsTab[k*jMax+l]).y+(ScreenY/(jMax))>mousePressPointY)
                {
                    *iValue=k;
                    *jValue=l;
                }
            }
        }
    }
    void movePartByMouse(int mousePressPointX, int mousePressPointY, int mouseCurrentPositionX, int mouseCurrentPositionY)  //changes position of the given part by mouse current and mouse press positions as well as saves the total move in x and y allowing the part to be restored later to its normal position
    {
        this->x-=this->addTempX;
        this->y-=this->addTempY;
        this->addTempX=(mouseCurrentPositionX-mousePressPointX);
        this->addTempY=(mouseCurrentPositionY-mousePressPointY);
        this->x+=this->addTempX;
        this->y+=this->addTempY;
    }
    void restorePartPosition()  //restores part to its position before the move
    {
        this->x-=this->addTempX;
        this->y-=this->addTempY;
        addTempX=0;
        addTempY=0;
    }
    static int checkForVictory(Parts *partsTab) //checks whether parts are in right order and returns 1 if yes or 0 if no
    {
        for (int k=0; k<iMax; k++)
        {
            for (int l=0; l<jMax; l++)
            {
                if ( (partsTab[k*jMax+l]).i!=k || (partsTab[k*jMax+l]).j!=l)
                {
                    return (0);
                }
            }
        }
        return (1);
    }
};

float Parts::xMax=0, Parts::yMax=0;  //necessary definitions of in-class static members
int Parts::iMax=0, Parts::jMax=0; //necessary definitions of in-class static members
sf::Texture Parts::image;   //necessary definitions of in-class static members
string Parts::imageName;    //necessary definitions of in-class static members

void setup(float *xMax, float *yMax, int *iMax, int *jMax, string *imageName);  //loads information about image size, name and number of parts it is to be divided into from setup file

int main()
{
    srand(time(NULL));

    sf::Music music, victorySound;
    music.openFromFile("Resources/Music/deep-ambient.ogg");
    victorySound.openFromFile("Resources/Music/bicycle-bell.ogg");
    music.setLoop(true);

    float xMax, yMax; //width and length of the image
    int iMax, jMax; //number of rows and columns the image will be divided into
    string imageName;   //name of the image and its extension

    setup(&xMax, &yMax, &iMax, &jMax, &imageName);
    Parts partsTab[iMax][jMax];
    Parts::setImageInfo(xMax, yMax, iMax, jMax, imageName);
    Parts::loadImage();
    Parts::setValuesAll(&(partsTab[0][0]));
    Parts::shuffleParts(&(partsTab[0][0]));
    Parts::drawAll(&(partsTab[0][0]), 0, 0);
    music.play();

    bool isPressed=0;
    int mousePressPointX=0, mousePressPointY=0;
    int mouseCurrentPositionX=0, mouseCurrentPositionY=0;
    int iValue=-1, jValue=-1;
    int iValue2=-1, jValue2=-1;
    while (okno.isOpen())
    {
        sf::Event zdarzenie;
        while (okno.pollEvent( zdarzenie ))
        {
            if (zdarzenie.type == sf::Event::KeyPressed && zdarzenie.key.code == sf::Keyboard::Escape) okno.close();
            if (isPressed==0)
            {
                if (zdarzenie.type == sf::Event::MouseButtonPressed && zdarzenie.mouseButton.button == sf::Mouse::Left)
                {
                    isPressed=1;
                    mousePressPointX=sf::Mouse::getPosition(okno).x;
                    mousePressPointY=sf::Mouse::getPosition(okno).y;
                    Parts::identifyClickedPart(&(partsTab[0][0]), &iValue, &jValue, mousePressPointX, mousePressPointY);
                }
            }
            else
            {
                if (zdarzenie.type == sf::Event::MouseMoved)
                {
                    mouseCurrentPositionX=sf::Mouse::getPosition(okno).x;
                    mouseCurrentPositionY=sf::Mouse::getPosition(okno).y;
                    partsTab[iValue][jValue].movePartByMouse(mousePressPointX, mousePressPointY, mouseCurrentPositionX, mouseCurrentPositionY);
                    Parts::drawAll(&(partsTab[0][0]), iValue, jValue);
                }
                else if (zdarzenie.type == sf::Event::MouseButtonReleased)
                {
                    isPressed=0;
                    mousePressPointX=sf::Mouse::getPosition(okno).x;
                    mousePressPointY=sf::Mouse::getPosition(okno).y;
                    partsTab[iValue][jValue].restorePartPosition();
                    Parts::identifyClickedPart(&(partsTab[0][0]), &iValue2, &jValue2, mousePressPointX, mousePressPointY);
                    Parts::swapPartsPositions(partsTab[iValue][jValue], partsTab[iValue2][jValue2]);
                    Parts::drawAll(&(partsTab[0][0]), 0, 0);
                    if (Parts::checkForVictory(&(partsTab[0][0])))
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

void setup(float *xMax, float *yMax, int *iMax, int *jMax, string *imageName)   //loads information about image size, name and number of parts it is to be divided into from setup file
{
    char s;
    ifstream setupFile("Resources/setup.txt");
    while (setupFile>>s) if (s=='#') setupFile>>*xMax>>*yMax>>*iMax>>*jMax>>*imageName;
    setupFile.close();
}

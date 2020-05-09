#ifndef HEXBOARD_H
#define HEXBOARD_H

#include "Hex.h"
#include <QList>

class HexBoard
{
public:
    //constructors
    HexBoard();

    //getters
    QList<Hex*> getHexes();

    //setters
    void append(Hex* hexToAppend);
    void removeAll(Hex* hexToRemove);

     //functions
    void placeHexes(int x,int y, int cols, int rows);

private:
    QList<Hex*> hexes;
    //functions
    void createHexColumn(int x,int y, int numOfRows);
};

#endif // HEXBOARD_H

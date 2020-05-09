#include "HexBoard.h"
#include "Game.h"

extern Game* game;


HexBoard::HexBoard(){

}

QList<Hex *> HexBoard::getHexes(){
    return hexes;
}

void HexBoard::append(Hex *hexToAppend){
    hexes.append(hexToAppend) ;
}

void HexBoard::removeAll(Hex *hexToRemove){
    hexes.removeAll(hexToRemove);
}

void HexBoard::placeHexes(int x, int y, int cols, int rows){
    int Xshift = 82;
    int Yshift = 41;
    for(size_t i =0 ,n = cols; i<n ; i++){
        if(i%2 != 0)
            createHexColumn(x+Xshift*i,y+Yshift,rows);

        else
            createHexColumn(x+Xshift*i,y,rows);
    }
}

void HexBoard::createHexColumn(int x, int y, int numOfRows){
    //creating a column of at the specified location with specified location with specified
    // number of rows
    for(size_t i =0,n = numOfRows; i<n ;i++){
        Hex* hex = new Hex();
        hex->setIsPlaced(true);
        hex->setPos(x,y+82*i);
        hexes.append(hex);
        game->scene->addItem(hex);

        //set owner to no one
        hex->setOwner(QString("NOONE"));
    }
}




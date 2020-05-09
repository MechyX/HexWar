#ifndef HEX_H
#define HEX_H

#include<QGraphicsPolygonItem>
#include <QGraphicsSceneMouseEvent>

class Hex: public QGraphicsPolygonItem{
public:
    //constructors
    Hex(QGraphicsItem* parent=NULL);

    //getters
    int getAttackOf(int side);
    bool CheckifPlaced();
    QString getOwner();

    //event
    void mousePressEvent(QGraphicsSceneMouseEvent *event);

    //setters
    void setAttackOf(int side,int attackValue);
    void setOwner(QString player);
    void setIsPlaced(bool truth);

    void displaySideAttacks();
    void createLines();
    void findNeighbours();
    void switchOwner();
    void captureNeigbour();

private:
    //attributes
    QList<QGraphicsLineItem*> lines;
    QList<Hex*> neighbours;
    bool isPlaced;
    QString owner;
    int *SideAttackValues;
    QList<QGraphicsTextItem*> attackTexts;
};

#endif // HEX_H

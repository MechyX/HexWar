#include "Hex.h"
#include <QBrush>
#include "Game.h"
#include <QDebug>

extern Game* game;

Hex::Hex(QGraphicsItem* parent){
    //draw the polygon
    //Hex Points -> 1,0   2,0  3,1  2,2  1,2  0,1

    QVector<QPointF> hexPoints;
    hexPoints << QPointF(1,0) << QPointF(2,0) << QPointF(3,1)
              << QPointF(2,2) << QPointF(1,2) << QPointF(0,1);

    int scaleBy = 40;
    for(size_t i =0,n = hexPoints.size() ; i<n ; i++){
        hexPoints[i] *= scaleBy;
    }

     //creating polygon with those points above
    QPolygonF hexagon(hexPoints);

    //drawing the polygon
    setPolygon(hexagon);

    //intialize
    isPlaced = false;

    //create lines to detect neighbours
    createLines();

    SideAttackValues = NULL;

    //try catch statements
    try {
          SideAttackValues = new int[6];
          if(SideAttackValues == NULL)
               throw 0;
    } catch (int e) {
        //exit if not able to allocate memory
        qDebug()<<"Cannot Allocate memory";
        exit(0);
    }

    //intialize side attacks to zero
    for(size_t i=0;i<6;i++){
            SideAttackValues[i] = 0;
    }

    //create QGraphicsTextItem visually each side's attack
    QGraphicsTextItem* text0 = new QGraphicsTextItem(QString::number(0),this);
    QGraphicsTextItem* text1 = new QGraphicsTextItem(QString::number(0),this);
    QGraphicsTextItem* text2 = new QGraphicsTextItem(QString::number(0),this);
    QGraphicsTextItem* text3 = new QGraphicsTextItem(QString::number(0),this);
    QGraphicsTextItem* text4 = new QGraphicsTextItem(QString::number(0),this);
    QGraphicsTextItem* text5 = new QGraphicsTextItem(QString::number(0),this);

    attackTexts.append(text0);
    attackTexts.append(text1);
    attackTexts.append(text2);
    attackTexts.append(text3);
    attackTexts.append(text4);
    attackTexts.append(text5);

    //set the correct position of the attack texts
    text0->setPos(50,0);
    text1->setPos(20,15);
    text2->setPos(20,40);
    text3->setPos(50,55);
    text4->setPos(80,40);
    text5->setPos(80,15);

    // make all the attack value invisible
    for(size_t i=0 ; i<6 ;i++){
        attackTexts[i]->setVisible(false);
    }
}

int Hex::getAttackOf(int side){
    return SideAttackValues[side];
}

bool Hex::CheckifPlaced(){
    return isPlaced;
}

QString Hex::getOwner(){
    return owner;
}

void Hex::mousePressEvent(QGraphicsSceneMouseEvent *event){
    // if hex is not placed(then pick it up)not in the hex board


    if(CheckifPlaced() == false){
        game->pickUpCard(this);
    }

    //if hex is placed(already in the hex board)
    else{
        if(game->cardToPlace != NULL){
            if(getOwner() == QString("NOONE"))
                   game->placeCard(this);
          }
    }
}

void Hex::setAttackOf(int side, int attackValue){
    //set attack value to each side
    SideAttackValues[side] = attackValue;
    attackTexts[side]->setPlainText(QString::number(attackValue));
}

void Hex::setOwner(QString player){

    //set the owner change color according to player
    owner = player;

    //Grey if no owner
    if(owner == QString("NOONE")){
        QBrush brush;
        brush.setColor(Qt::lightGray);
        brush.setStyle(Qt::SolidPattern);
        setBrush(brush);
    }

    //player 1 to blue
    if(owner == QString("PLAYER1")){
        QBrush brush;
        brush.setColor(Qt::blue);
        brush.setStyle(Qt::SolidPattern);
        setBrush(brush);
    }

    //player 2 to red
    if(owner == QString("PLAYER2")){
        QBrush brush;
        brush.setColor(Qt::red);
        brush.setStyle(Qt::SolidPattern);
        setBrush(brush);
    }
}

void Hex::setIsPlaced(bool truth){
    isPlaced = truth;
}

void Hex::displaySideAttacks(){

    //display side attacks on the scene
   for(size_t i=0 ; i<6 ;i++){
    attackTexts[i]->setVisible(true);
   }
}

void Hex::createLines() {
    QPointF hexCenter(x()+60,y()+40);
    QPointF finalPoint(hexCenter.x(),hexCenter.y()-65);
    QLineF line(hexCenter,finalPoint);

    for(size_t i=0; i<6 ;i++){
        QLineF lineCopy(line);
        lineCopy.setAngle(90+60*i);
        QGraphicsLineItem* lineItem = new QGraphicsLineItem(lineCopy,this);
        lines.append(lineItem);
        //make the line invisible
        lineItem->setVisible(false);
    }
}

void Hex::findNeighbours(){
     for(size_t i=0,n = lines.size(); i< n; i++){
            //If the line collides with an item of type HEX add it to neighbours list
        QList<QGraphicsItem*> cItems = lines[i] -> collidingItems();

        if(cItems.size()-2 == 0){
            Hex *ImaginaryHex = new Hex();
            ImaginaryHex->setOwner(QString("NOONE"));
            ImaginaryHex->setIsPlaced(true);
            neighbours.append(ImaginaryHex);
        }


            for(size_t j=0, n = cItems.size(); j < n ; j++){
                Hex* item = dynamic_cast<Hex*>(cItems[j]);
                        if(cItems[j] != this && item){
                            neighbours.append(item);
                        }
            }

        }

     }

void Hex::switchOwner(){
    qDebug()<<"Inside Switch Owner************";
    //if the owner is player 1 make it player 2 or vice-versa
        if(getOwner() == QString("PLAYER1"))
        setOwner(QString("PLAYER2"));

        else{
            setOwner(QString("PLAYER1"));
        }

}

void Hex::captureNeigbour(){

    //traverses through neighbours captures weaker neighbours
    for(size_t i=0,n = neighbours.size(); i<n ;i++){
        bool isEnemy = false;
        bool isNotNeutral = false;
        if(this->getOwner() != neighbours[i] -> getOwner()){
            isEnemy = true;
        }
        if(neighbours[i]->owner != QString("NOONE")){
            isNotNeutral = true;
        }

        // it is an enemy,not neutral
        if(isEnemy && isNotNeutral){
            //find attack value of adjacent hex
            int thisAttack = 0;
            int neigboursAttack = 0;
            thisAttack = this->getAttackOf(i);
            neigboursAttack = neighbours[i]->getAttackOf((i+3)%6);

           //if this has greater attack value,switch neighbour's owner
           if(thisAttack > neigboursAttack){
               neighbours[i]->switchOwner();
           }
        }
    }
}

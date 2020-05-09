#ifndef GAME_H
#define GAME_H

//QGraphicsView is a widget

#include<QGraphicsView>
#include<QGraphicsScene>
#include "HexBoard.h"
#include <QMouseEvent>

class Game : public QGraphicsView{
    Q_OBJECT
public:
    //since there is no parent and this is the main widget
    Game(QWidget * parent = NULL);

    //public functions
    void displayMainMenu();
    QString getWhoseTurn();
    void setWhoseTurn(QString player);
    void pickUpCard(Hex* card);
    void placeCard(Hex* hexToReplace);
    void gameOver();



     //For Dragging a hex and placing it on the hexboard
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);

    //attributes
    //Only these class attributes are public since there are lot of uses
    QGraphicsScene * scene;
    HexBoard* hexBoard;
    Hex* cardToPlace;
    QPointF originalPos;


public slots:
    void start();
    void close();
    void restartGame();

private:

    //functions
    void drawPanel(int x,int y,int width,int height,QColor color,double opacity);
    void drawGUI();
    void createNewCard(QString player);
    void createInitialCards();
    void drawCards();
    void nextPlayerTurn();
    void removeFromDeck(Hex* card,QString player);
    void displayGameOverWindow(QString message);

    //attributes
    QString whosTurn_;
    QGraphicsTextItem* whosTurnText;
    QList<Hex*> Player1Cards;
    QList<Hex*> Player2Cards;
    int numOfCardsPlaced;
};

#endif // GAME_H

#include "Game.h"
#include "Hex.h"
#include "Button.h"
#include <QGraphicsTextItem>
#include <QBrush>
#include <QGraphicsTextItem>
#include <stdlib.h>
#include <time.h>
#include <QDebug>



Game::Game(QWidget *parent){

    //setting up the screen
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setFixedSize(1024,768);

    //setting up the scene
    scene = new QGraphicsScene();
    scene->setSceneRect(0,0,1024,768);
    setScene(scene);

    cardToPlace = NULL;
    this->setWindowTitle(QString("HexWar"));
}


void Game::start(){
    cardToPlace = NULL;
    numOfCardsPlaced = 0;
    //clear the menu
    scene->clear();
    drawGUI();
    hexBoard = new HexBoard();
    hexBoard->placeHexes(200,80,7,7);
    scene->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));

    createInitialCards();
}


void Game::close(){
    exit(0);
}

void Game::restartGame(){
    //clear some stuff then call start()
    Player1Cards.clear();
    Player2Cards.clear();
    hexBoard->getHexes().clear();
    scene->clear();
    start();
}


void Game::drawPanel(int x, int y, int width, int height, QColor color, double opacity){
    //draws a panel at specified location with specified criteria

    QGraphicsRectItem* panel = new QGraphicsRectItem(x,y,width,height);
    QBrush brush;
    brush.setStyle(Qt::SolidPattern);
    brush.setColor(color);
    panel->setBrush(brush);
    panel->setOpacity(opacity);
    scene->addItem(panel);
}


void Game::drawGUI(){
    //draw left panel
    drawPanel(0,0,150,768,Qt::darkBlue,0.5);

    //draw right panel
     drawPanel(1024-150,0,150,768,Qt::darkRed,0.5);

     //Text Styling
     QFont font("Arial",20);


     //place player 1 text
     QGraphicsTextItem* p1 = new QGraphicsTextItem("P1");
     p1->setDefaultTextColor(Qt::blue);
     p1->setFont(font);
     p1->setPos(10,0);
     scene->addItem(p1);

     //place player 2 text
     QGraphicsTextItem* p2 = new QGraphicsTextItem("P2");
     p2->setDefaultTextColor(Qt::red);
     p2->setFont(font);
     p2->setPos(1024-150+25-25,0);
     scene->addItem(p2);

      //place Turn Text
      whosTurnText = new QGraphicsTextItem();
      whosTurnText->setDefaultTextColor(Qt::white);
      whosTurnText->setFont(font);
      setWhoseTurn(QString("PLAYER1"));

      //(490,0) is top middle of the screen
      whosTurnText->setPos(400,0);
      scene->addItem(whosTurnText);
}


void Game::createNewCard(QString player){
        Hex* card = new Hex();
        card->setOwner(player);
        card->setIsPlaced(false);

        //set random attack values to each side
        for(size_t i=0; i<6 ;i++){
            //generate a random number
            int randNum = rand() % 6 + 1; // generates any value btw 1 and 6
            card->setAttackOf(i,randNum);
        }

        //make side attacks visible
        card->displaySideAttacks();

        if(player == QString("PLAYER1"))
            Player1Cards.append(card);

        else
            Player2Cards.append(card);

        drawCards();
}


void Game::createInitialCards(){
//create player 1's cards
    for(size_t i=0,n = 5 ;i< n ;i++){
            createNewCard(QString("PLAYER1"));
            createNewCard(QString("PLAYER2"));
    }
        drawCards();
}

void Game::drawCards(){
    //traverse through list and draw cards on the screen
    //drawCards() is to be called often,so delete the items in the scene and reprint them

    //player 1
    for(size_t i=0, n=Player1Cards.size() ; i<n ; i++){
        //check if item is in scene or not
        if(Player1Cards[i]->scene() != nullptr)
        scene->removeItem(Player1Cards[i]);
    }

    //player 2
    for(size_t i=0, n=Player2Cards.size() ; i<n ; i++){
        //check if item is in scene or not
        if(Player2Cards[i]->scene() != nullptr)
        scene->removeItem(Player2Cards[i]);
    }

    //draw player 1 cards on the left
    for(size_t i=0, n=Player1Cards.size() ; i<n ; i++){
        Hex * card = Player1Cards[i];
        card->setPos(13,70+85*i);
        scene->addItem(card);
    }

    //draw player 2 cards on the right
    for(size_t i=0, n=Player2Cards.size() ; i<n ; i++){
        Hex * card = Player2Cards[i];
        card->setPos(1024-150+13,70+85*i);
        scene->addItem(card);
    }

}

void Game::displayMainMenu(){
     scene->setBackgroundBrush(QBrush(Qt::black, Qt::SolidPattern));
    //Creating the game title
    QGraphicsTextItem* titleText = new QGraphicsTextItem(QString("HexWar"));
    QFont titleFont("verdana",50);
    titleText->setDefaultTextColor(Qt::red);
    titleText->setFont(titleFont);
    int textXPos = this->width()/2 - titleText->boundingRect().width()/2;
    int textYPos = this->height()/2 - titleText->boundingRect().height()/2  - 100;
    titleText->setPos(textXPos,textYPos);
    scene->addItem(titleText);

    //creating the play button
    Button *playButton = new Button(QString("Play"));
    int butXPos = this->width()/2 - playButton->boundingRect().width()/2;
    int butYPos = 500;
    playButton->setPos(butXPos , butYPos);
    connect(playButton, SIGNAL(clicked()),this,SLOT(start()));
    scene->addItem(playButton);

    //creating the quit Button
    Button *quitButton = new Button(QString("Quit"));
    butXPos = this->width()/2 - quitButton->boundingRect().width()/2;
    butYPos = 600;
    quitButton->setPos(butXPos , butYPos);
    connect(quitButton, SIGNAL(clicked()),this,SLOT(close()));
    scene->addItem(quitButton);
}

QString Game::getWhoseTurn(){
    //return whose turn it is
    return whosTurn_;
}

void Game::setWhoseTurn(QString Player){
   //change turn
    whosTurn_ =  Player;
    qDebug() << whosTurn_ + " Turn";
    //change TextOnScreen
    whosTurnText->setPlainText(Player + QString(" TURN"));
    if(whosTurn_ == QString("PLAYER1"))
        whosTurnText->setDefaultTextColor(Qt::blue);

    else
        whosTurnText->setDefaultTextColor(Qt::red);

}

void Game::pickUpCard(Hex *card){
    //picks up the specify Card

    if(card->getOwner() == getWhoseTurn() && cardToPlace == NULL){
    cardToPlace = card;
    originalPos = card->pos();
    return;
}
}

void Game::mouseMoveEvent(QMouseEvent *event){
    //make the card follow the mouse

    if (cardToPlace != NULL){
            cardToPlace->setPos(event->pos());
        }

    QGraphicsView::mouseMoveEvent(event);
}

void Game::mousePressEvent(QMouseEvent *event){
    if(event->button() == Qt::RightButton){
        if(cardToPlace != NULL){
                cardToPlace->setPos(originalPos);
                cardToPlace = NULL;
        }
    }
    QGraphicsView::mousePressEvent(event);
}

void Game::placeCard(Hex *hexToReplace){
    //if there is a card to place, place it on the board
        cardToPlace->setPos(hexToReplace->pos());
        hexBoard->append(cardToPlace);
        hexBoard->removeAll(hexToReplace);
        scene->removeItem(hexToReplace);
        cardToPlace->setIsPlaced(true);
        removeFromDeck(cardToPlace,getWhoseTurn());

        //find neighours
        cardToPlace->findNeighbours();
        cardToPlace->captureNeigbour();

        //make cardToPlace NULL
        cardToPlace = NULL;

        // replace the card
        createNewCard(getWhoseTurn());

        //switch turns
        nextPlayerTurn();

        numOfCardsPlaced++;

        if(numOfCardsPlaced >= hexBoard->getHexes().size()){
            gameOver();
        }
}

void Game::gameOver(){
    //count hexes, determine who has more
    int p1Hexes = 0;
    int p2Hexes = 0;
    for(size_t i=0,n=hexBoard->getHexes().size(); i<n; i++){
            if(hexBoard->getHexes()[i]->getOwner() == QString("PLAYER1")){
                p1Hexes++;
}

            else if(hexBoard->getHexes()[i]->getOwner() == QString("PLAYER2")){
                p2Hexes++;
    }
            }

    QString message;
    if(p1Hexes > p2Hexes){
        message = QString("Player 1 has won");
    }

    else if(p1Hexes < p2Hexes){
        message = QString("Player 1 has won");
    }

    else{
        message = QString("Draw");
    }

    displayGameOverWindow(message);
}

void Game::nextPlayerTurn(){
    //Change Player Turns
    if(getWhoseTurn() == QString("PLAYER1")){
        setWhoseTurn(QString("PLAYER2"));
    }

    else if(getWhoseTurn() == QString("PLAYER2")){
        setWhoseTurn(QString("PLAYER1"));
    }
}

void Game::removeFromDeck(Hex *card, QString player){

    if(player == QString("PLAYER1")){
        //remove all instances of that card
        Player1Cards.removeAll(card);
    }

    if(player == QString("PLAYER2")){
        //remove all instances of that card
        Player2Cards.removeAll(card);
    }
}

void Game::displayGameOverWindow(QString message){
    //disable all scene items
    for(size_t i=0, n = scene->items().size();i<n;i++){
        scene->items()[i]->setEnabled(false);
   }

    //pop up semi transparent scene
    drawPanel(0,0,1024,768,Qt::black,0.65);

    //draw panel
    drawPanel(312,184,400,400,Qt::lightGray,0.75);

    //create PlayAgainButton
    Button *playAgain = new Button(QString("Play Again"));
    playAgain->setPos(410,300);
    scene->addItem(playAgain);
    connect(playAgain,SIGNAL(clicked()),this,SLOT(restartGame()));

    //create quit button
    Button *quit = new Button(QString("Quit"));
    quit->setPos(410,350);
    scene->addItem(quit);
    connect(quit,SIGNAL(clicked()),this,SLOT(close()));

    QGraphicsTextItem* overText = new QGraphicsTextItem(message);
    overText->setPos(460,225);
    scene->addItem(overText);
}

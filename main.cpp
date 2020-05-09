#include <QApplication>
#include "Game.h"

Game* game;

int main(int argc, char *argv[])
{
    QApplication a(argc,argv);

    //start a new game
    game = new Game();

    //view the game
    game->show();

    //display menu
    game->displayMainMenu();


    return a.exec();
}

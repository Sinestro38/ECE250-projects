/**
 * @file driver.cpp
 * @brief This file contains the main function that drives the game.
 */

#include <iostream>
#include <string>
#include "game.h"

// are inline definitions okay in header files?
int main(){
    Game* game_ptr = new Game();
    auto game = *game_ptr;

    while (true) {
        std::string command;
        std::cin >> command;

        if (command == "SPAWN") {
            double x;
            double y;
            std::cin >> x;
            std::cin >> y;

            game.spawn(x, y);
        } else if (command == "TIME") {
            double t;
            std::cin >> t;        
            game.time(t);

        } else if (command == "LUNCH") {
            game.lunch();

        } else if (command == "NUM") {
            game.num_playing();

        } else if (command == "PRT") {
            double d;
            std::cin >> d;
            game.prt(d);

        } else if (command == "OVER") {
            game.determine_winner();

            delete game_ptr;
            game_ptr = nullptr;
            break;
            
        } else {
            std::cout << "Invalid command, try again." << std::endl;
        }

    }

    return 0;
}
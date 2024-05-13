/**
 * @file game.cpp
 * @brief Implementation file for the Game class and related classes.
 *
 * This file contains the implementation of the Game class and its related classes: Position and Player.
 * The Game class represents a game with multiple players, where each player has a position in a 2D space.
 * The Position class represents a position in the 2D space, defined by its x and y coordinates.
 * The Player class represents a player in the game, with a position and a reference to the next player in the game.
 *
 * The Game class provides various methods for manipulating and interacting with the players in the game,
 * such as spawning new players, updating player positions over time, filtering out cheaters, and determining the winner.
 *
 * The implementation of the Game class is divided into three sections: Position class implementation, Player class implementation,
 * and Game class implementation.
 */
#include "game.h"
#include <cmath>

// POSITION CLASS IMPLEMENTATION

/**
 * @brief Constructs a Position object with the given x and y coordinates.
 *
 * @param x_pos The x-coordinate of the position.
 * @param y_pos The y-coordinate of the position.
 * @throws std::invalid_argument if x_pos or y_pos is not positive.
 */
Position::Position(double x_pos, double y_pos)
{
    if (is_valid(x_pos, y_pos))
    {
        this->x = x_pos;
        this->y = y_pos;
    }
    else
    {
        throw std::invalid_argument("x and y coordinates must be positive.");
    }
}

// PLAYER CLASS IMPLEMENTATION

std::ostream &operator<<(std::ostream &os, const Player &p)
{
    os << "P@(" << p._pos.x << ", " << p._pos.y << ")";
    return os;
}

void Player::print_full() const
{
    std::cout << "P@(" << this->_pos.x << ", " << this->_pos.y << ") -> ";
    if (this->_next != nullptr)
    {
        std::cout << "P@(" << this->_next->_pos.x << ", " << this->_next->_pos.y << ")";
    }
    else
    {
        std::cout << "nullptr";
    }
    std::cout << std::endl;
}

/**
 * Calculates the distance of the player from the origin (0, 0).
 *
 * @return The distance of the player from the origin.
 */
double Player::distance() const
{
    auto x = this->get_x();
    auto y = this->get_y();

    return sqrt((x * x) + (y * y));
}

// GAME CLASS IMPLEMENTATION

/**
 * Constructor for the Game class.
 * Initializes the game with the given array of players and the number of players.
 * Sets the head of the player list to the first player in the array.
 * Connects each player to the next player in the array, forming a circular linked list.
 *
 * @param first_player An array of Player objects representing the players in the game.
 * @param num_players The number of players in the game.
 */
Game::Game(Player first_player[], int num_players)
{
    this->_head = first_player;

    Player *curr = this->_head;
    for (int i = 0; i < num_players; i++)
    {
        bool last_player = (i == (num_players - 1));

        if (!last_player)
        {
            curr->set_next(&first_player[i + 1]);
        }
        else
        {
            curr->set_next(nullptr);
        }

        curr = curr->get_next();
    }
}

/**
 * @brief Destructor for the Game class.
 *
 * This destructor is responsible for deallocating the memory used by the linked list
 * of nodes in the Game class. It iterates through the linked list and deletes each node
 * until the list is empty.
 */
Game::~Game()
{
    while (this->_head != nullptr)
    {
        auto tmp = this->_head;
        this->set_head(this->_head->get_next());
        delete tmp;
    }
}

/**
 * Prints all the players in the game.
 *
 * @param debug If true, prints detailed information about each player.
 *      If false, prints only basic information.
 */

void Game::_prepend(Player *player)
{
    Player *curr = this->get_head();
    this->_num_players++; // increment number of players

    // set this player as new head
    // and point it to the current head
    this->set_head(player);
    player->set_next(curr);
}

/**
 * Spawns a new player at the specified position.
 *
 * @param x The x-coordinate of the position.
 * @param y The y-coordinate of the position.
 */
void Game::spawn(double x, double y)
{
    try
    {
        Position pos = Position(x, y);
        Player *player = new Player(pos, nullptr);
        this->_prepend(player);
        std::cout << "success" << std::endl;
    }
    catch (const std::invalid_argument &e)
    {
        std::cout << "failure" << std::endl;
    }
}

/**
 * Updates the position of all players in the game based on the given time.
 * The position of each player is updated using the formula:
 * new_x = x - (t * cos(atan2(y,x)))
 * new_y = y - (t * sin(atan2(y,x)))
 *
 * @param t The time value used to update the positions of the players.
 */
void Game::time(double t)
{
    Player *curr = this->get_head();

    while (curr != nullptr)
    {
        auto x = curr->get_x();
        auto y = curr->get_y();

        auto new_x = x - (t * cos(atan2(y, x)));
        auto new_y = y - (t * sin(atan2(y, x)));

        curr->set_x(new_x);
        curr->set_y(new_y);

        curr = curr->get_next();
    }

    this->_purge_cheaters();

    this->num_playing();
}

/**
 * @brief Purges cheaters from the game.
 *
 * This function filters the list of players and removes any cheaters from the game.
 * A player is considered a cheater if they are not valid.
 */
void Game::_purge_cheaters()
{
    this->_filter_list([](Player *player)
                       { return player->is_valid(); });
}

/**
 * Filters out all players within a distance of less than 1 from the wolf.
 * Updates the player list accordingly.
 *
 * @param player The player to be filtered.
 */
void Game::lunch()
{
    // filter out all players within a distance
    // of <1 from the wolf
    this->_filter_list([](Player *player)
                       { return player->distance() >= 1; });

    this->num_playing();
}

/**
 * Prints the number of kids playing the game.
 */
void Game::num_playing() const
{
    // print the number of kids playing
    std::cout << "num of players: " << this->_num_players << std::endl;
}

/**
 * Prints the coordinates of all active players whose distance from the origin is less than the given distance.
 * If no players are found within the given distance, it prints "no players found".
 *
 * @param dist The distance threshold for filtering active players.
 */
void Game::prt(double dist) const
{
    // print all active players
    bool found_players = false;

    auto curr = this->get_head();
    while (curr != nullptr)
    {
        if (curr->distance() < dist)
        {
            found_players = true;
            std::cout << curr->get_x() << " ";
            std::cout << curr->get_y() << " ";
        }
        curr = curr->get_next();
    }

    if (!found_players)
        std::cout << "no players found";

    std::cout << std::endl;
}

/**
 * Determines the winner of the game.
 * Prints "wolf wins" if the _head member variable is nullptr,
 * otherwise prints "players win".
 */
void Game::determine_winner() const
{
    if (this->_head == nullptr)
        std::cout << "wolf wins" << std::endl;
    else
        std::cout << "players win" << std::endl;
}

/**
 * Filters the list of players based on the provided filter function.
 * Removes all nodes that do not pass the filter function.
 *
 * @param filter_fn A pointer to a boolean function that takes a Player pointer as a parameter and returns true or false.
 *                  The function is used to determine whether a player should be removed from the list or not.
 */
void Game::_filter_list(bool (*filter_fn)(Player *))
{
    // Remove all nodes that don't pass filter_fn(&node)
    while (this->_head != nullptr && !filter_fn(this->_head))
    {
        // remove head node
        auto tmp_head = this->get_head();
        this->set_head(tmp_head->get_next());
        this->_num_players--; // decrement number of elements in list
        delete tmp_head;
    }

    if (this->get_head() == nullptr)
    {
        return;
    }

    auto curr = this->get_head();
    Player *tmp = nullptr;
    while (curr->get_next() != nullptr)
    {
        // if next node is invalid
        if (!filter_fn(curr->get_next()))
        {
            // remove next node
            tmp = curr->get_next();
            curr->set_next(tmp->get_next());
            this->_num_players--; // decrement number of elements in list
            delete tmp;
        }

        // if next node is valid
        if (curr->get_next() != nullptr && filter_fn(curr->get_next()))
        {
            // move cursor to next node
            curr = curr->get_next();
        }
    }
}
/**
 * @file game.h
 * @brief This file contains the declarations of the Position, Player, and Game classes.
 *
 * The Position class represents a position with x and y coordinates and provides methods
 *  to check if the position is valid.
 *
 * The Player class represents a player in the game and is implemented as a linked list
 *  node. It contains methods to manipulate the player's position and interact with other players.
 *
 * The Game class manages the game state using a linked list of players. It provides
 *  methods to spawn players, update the game state, remove players, and determine the winner of the game.
 */
#pragma once

#include <iostream>
#include <string>

/*
    Position struct to store x and y coordinates with
    methods to check if the position is valid
*/
class Position
{
    /*
        public members
        - x (double): x coordinate
        - y (double): y coordinate
        - is_valid(): method to check if the position is in 1st quadrant
    */
public:
    double x;
    double y;
    Position() = default;
    Position(double x_pos, double y_pos);
    inline bool is_valid() const
    {
        return this->x > 0 && this->y > 0;
    };

    inline bool is_valid(double x, double y) const
    {
        return x > 0 && y > 0;
    }
};

/*
    Player class as a linked list node
*/
class Player
{
    /*
        private members
        - _pos (Position): Position object representing the player's position
        - _next (Player*): pointer to the next player in the game
    */
private:
    Position _pos;
    Player *_next;

    /*
        public members
        - default constructor
        - destructor
        - constructor with x and y coordinates
        - constructor with Position object and next player
        - method to set the next player (next pointer)
        - method to get the next player
        - method to get the x coordinate
        - method to get the y coordinate
        - method to check if the position is valid
        - method to check if the position is invalid
        - method to calculate the distance from the origin
        - method to set the x coordinate
        - method to set the y coordinate
        - friend function to overload the << operator
        - method to print the player's position and the next player's position
    */
public:
    Player() = default;
    ~Player() = default;
    Player(double x, double y)
    {
        this->_pos = Position(x, y);
        this->_next = nullptr;
    }
    Player(Position &pos, Player *next = nullptr)
    {
        this->_next = next;
        this->_pos = pos;
    };

    void set_next(Player *next_node) { this->_next = next_node; }
    inline Player *get_next() const { return this->_next; }
    inline double get_x() const { return this->_pos.x; }
    inline double get_y() const { return this->_pos.y; }
    inline bool is_valid() const { return this->_pos.is_valid(); }
    inline bool is_invalid() const { return !this->is_valid(); }
    double distance() const;

    void set_x(double x) { this->_pos.x = x; }
    void set_y(double y) { this->_pos.y = y; }

    friend std::ostream &operator<<(std::ostream &os, const Player &p);
    void print_full() const;
};

/*
    Game class to manage the game state using a linked list of players
*/
class Game
{
    /*
        private members
        - _head (Player*): pointer to the first player node in the linked list
        - _num_players (int): number of players in the game (linked list)
        - _prepend(): method to add a player to the game (front of the list)
        - _purge_cheaters(): method to remove players with invalid positions
        - _filter_list(): method to remove players based on a filter condition
    */
private:
    Player *_head;
    int _num_players = 0;
    void _prepend(Player *player);
    void _purge_cheaters();
    void _filter_list(bool (*filter_fn)(Player *));

    /*
        public members
        - default constructor
        - constructor with an array of players and the number of players
        - destructor
        - (void) method to spawn a player with x and y coordinates (prepend to the list)
        - (void) method to update the state of the Player nodes dependent on time
        - (void) method to remove all players within a certain distance from the wolf
        - (void) method to print the number of players in the game
        - (void) method to print the players within a certain distance from the origin
        - (void) method to determine the winner of the game
        - (Player*) method to get the first player in the game
        - (int) method to get the number of players in the game
        - (void) method to set the first player in the game
        - (void) method to print all players in the game
    */
public:
    Game() : _head{nullptr} {};
    Game(Player first_player[], int num_players = 1);
    ~Game();

    // following functions modify game state
    void spawn(double x, double y);
    void time(double t);
    void lunch();

    void num_playing() const;
    void prt(double distance) const;
    void determine_winner() const;

    // utility functions
    inline Player *get_head() const { return this->_head; }
    inline int get_num_players() const { return this->_num_players; }
    void set_head(Player *new_head) { this->_head = new_head; }
    void print_all(bool debug = true) const;
};
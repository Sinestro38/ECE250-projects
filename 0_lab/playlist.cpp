/**
 * @file playlist.cpp
 * @brief Implementation file for the Playlist class and related functions.
 * DOXYGEN DOCUMENTATION STYLE
 */

#include "playlist.h"
#include <iostream>

/**
 * @brief Constructor for the Song class.
 * @param title The title of the song.
 * @param artist The artist of the song.
 */
Song::Song(std::string title, std::string artist) : _title(title), _artist(artist) {}

/**
 * @brief Get the title of the song.
 * @return The title of the song.
 */
std::string Song::get_title() const {
    return _title;
}

/**
 * @brief Get the artist of the song.
 * @return The artist of the song.
 */
std::string Song::get_artist() const {
    return _artist;
}

/**
 * @brief Check if two songs are equal.
 * @param rhs The song to compare with.
 * @return True if the songs are equal, false otherwise.
 */
bool Song::operator==(const Song& rhs) const {
    return this->_title == rhs._title && this->_artist == rhs._artist;
}

/**
 * @brief Check if two songs are not equal.
 * @param rhs The song to compare with.
 * @return True if the songs are not equal, false otherwise.
 */
bool Song::operator!=(const Song& rhs) const {
    return !(*this == rhs);
}

/**
 * @brief Overloaded stream insertion operator for the Song class.
 * @param os The output stream.
 * @param song The song to be inserted into the stream.
 * @return The output stream.
 */
std::ostream& operator<<(std::ostream& os, const Song& song) {
    return os << song._title << ";" << song._artist;
}

/**
 * @brief Check if the input song is valid to be added to the playlist.
 * @param song The song to be checked.
 * @return True if the song is valid, false otherwise.
 */
bool Playlist::_is_valid_input(const Song& song) const {
    Song blacklisted_song{"Baby", "Justin Bieber"}; // hardcoded
    std::string blacklisted_title{"My Heart Will Go On"};

    bool not_in_list = !(this->ismember(song));
    bool not_blacklisted = (song != blacklisted_song) && (song.get_title() != blacklisted_title);
    bool not_full = this->_size < this->_capacity;
    
    bool is_valid_input = not_in_list && not_blacklisted && not_full;
    return is_valid_input;
}

/**
 * @brief Check if the song is a member of the playlist.
 * @param song The song to be checked.
 * @return True if the song is a member of the playlist, false otherwise.
 */
bool Playlist::ismember(const Song& song) const {
    bool ismember = false;
    for (std::size_t i{0}; i < this->_size; i++) {
        if ( this->_song_arr[i] == song) {
            ismember = true;
        }
    }
    return ismember;
}

/**
 * @brief Append a song to the end of the playlist's song array.
 * @param title The title of the song.
 * @param artist The artist of the song.
 */
void Playlist::append(const std::string title, const std::string artist) {
    Song song_to_add = Song(title, artist);

    if (_is_valid_input(song_to_add)) {
        std::size_t curr_size = this->_size;
        this->_song_arr[curr_size] = song_to_add;
        this->_size++;
        
        std::cout << "success" << std::endl;
    } else {
        std::cout << "can not insert " << song_to_add << std::endl;
    }
}

/**
 * @brief Play the song at the specified position in the playlist.
 * @param n The position of the song to be played.
 */
void Playlist::play_song(int n) const {
    if (n < this->_size) {
        std::cout << "played " << n << " " << this->_song_arr[n] << std::endl; 
    } else {
        std::cout << "can not play " << n << std::endl;
    }
}

/**
 * @brief Erase the song at the specified position in the playlist.
 * @param n The position of the song to be erased.
 */
void Playlist::erase(int erase_i) {
    // Erases song at position n
    // Moves any songs after position n up by 1

    // Validate input
    if (erase_i < 0 || erase_i >= this->_size) {
        std::cout << "can not erase " << erase_i << std::endl;
    } else {
        Song* new_arr = new Song[this->_capacity];
        std::size_t new_size = this->_size - 1;

        for (int i{0}; i < erase_i; i++) {
            new_arr[i] = Song(this->_song_arr[i]);
        }
        for (int i{erase_i+1}; i < this->_size; i++) {
            new_arr[i-1] = Song(this->_song_arr[i]);
        }

        delete[] this->_song_arr;
        this->_song_arr = new_arr;
        this->_size = new_size;

        std::cout << "success" << std::endl;
    }
}

/**
 * @brief Overloaded stream insertion operator for the Playlist class.
 * @param os The output stream.
 * @param playlist The playlist to be inserted into the stream.
 * @return The output stream.
 */
std::ostream& operator<<(std::ostream& os, const Playlist& playlist) {
    for (std::size_t i{0}; i < playlist._size; i++) {
        os << playlist._song_arr[i] << std::endl;
    }
    return os;
}

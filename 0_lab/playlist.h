/**
 * @file playlist.h
 * Provides the definition of the Playlist class and its associated Song class.
 * This header file includes the necessary classes for managing a playlist of songs.
 */

#pragma once

#include <string>

/**
 * Represents a song with a title and an artist.
 */
class Song {
    
    private:    
        std::string _title;   // Title of the song.
        std::string _artist;  // Artist of the song.

    public:
        // Default constructors and destructors.
        Song() = default;
        Song(std::string title, std::string artist);
        ~Song() = default;
        Song(const Song& rhs) = default;
        
        /**
         * Gets the title of the song.
         *
         * @return The song's title.
         */
        std::string get_title() const;
        
        /**
         * Gets the artist of the song.
         *
         * @return The song's artist.
         */
        std::string get_artist() const;
        
        /**
         * Sets the title of the song.
         *
         * @param title The new title for the song.
         */
        void set_title(std::string title);
        
        /**
         * Sets the artist of the song.
         *
         * @param artist The new artist for the song.
         */
        void set_artist(std::string artist);
        
        /**
         * Compares this song with another for equality.
         *
         * @param rhs The song to compare with.
         * @return True if the songs are equal, false otherwise.
         */
        bool operator==(const Song& rhs) const;
        
        /**
         * Compares this song with another for inequality.
         *
         * @param rhs The song to compare with.
         * @return True if the songs are not equal, false otherwise.
         */
        bool operator!=(const Song& rhs) const;

        /**
         * Overloads the stream insertion operator for song objects.
         *
         * @param os The output stream.
         * @param song The song to be output.
         * @return The output stream.
         */
        friend std::ostream& operator<<(std::ostream& os, const Song& song);
};


/**
 * Manages a collection of songs as a playlist.
 */
class Playlist {
    // Manages an array of songs.

    private:
        std::size_t _capacity;  // Maximum number of songs in the playlist.
        std::size_t _size;      // Current number of songs in the playlist.
        Song* _song_arr;        // Array storing the songs.

        /**
         * Checks if a song can be added to the playlist.
         *
         * @param song The song to check.
         * @return True if the song can be added, false otherwise.
         */
        bool _is_valid_input(const Song& song) const;

    public:
        // Default constructors and destructors.
        Playlist() = default;
        Playlist(int N) : _capacity(N), _size(0), _song_arr(new Song[N]) {}
        ~Playlist() = default;

        /**
         * Adds a new song to the end of the playlist.
         *
         * @param title The title of the song to add.
         * @param artist The artist of the song to add.
         */
        void append(const std::string title, const std::string artist);
        
        /**
         * Removes a song from the playlist at a given index.
         *
         * @param n The index of the song to remove.
         */
        void erase(int n);
        
        /**
         * Plays a song from the playlist at a given index.
         *
         * @param n The index of the song to play.
         */
        void play_song(int n) const;
        
        /**
         * Checks if a song is already in the playlist.
         *
         * @param song The song to check.
         * @return True if the song is in the playlist, false otherwise.
         */
        bool ismember(const Song& song) const;

        /**
         * Overloads the stream insertion operator for playlist objects.
         *
         * @param os The output stream.
         * @param playlist The playlist to be output.
         * @return The output stream.
         */
        friend std::ostream& operator<<(std::ostream& os, const Playlist& playlist);
};

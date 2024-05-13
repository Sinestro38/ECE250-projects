// include libraries here (STL not allowed)
#include <iostream>
#include <string>
#include "playlist.h"


int main(int argc, char *argv[]){
    while (true) {
        std::string command;
        std::cin >> command;

        Playlist user_playlist;

        if (command == "m") {
            int N;
            std::cin >> N;
            user_playlist = Playlist(N);

            std::cout << "success" << std::endl;
        } else if (command == "i") {
            std::string input_str;
            std::string delimiter = ";";
            std::getline(std::cin, input_str);
            input_str.erase(0, 1);

            // parse input
            std::size_t pos = input_str.find(delimiter);
            std::string title = input_str.substr(0, pos);
            std::string artist = input_str.substr(pos + delimiter.length(), input_str.length() - 1);

            user_playlist.append(title, artist);
        } else if (command == "p") {
            int n;
            std::cin >> n;
            user_playlist.play_song(n);

        } else if (command == "e") {
            int n;
            std::cin >> n;
            user_playlist.erase(n);

        } else if (command == "done") {
            break;
        } else {
            std::cout << "Invalid command, try again." << std::endl;
        }

    }

    
    return 0;
}
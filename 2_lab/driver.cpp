// include libraries here (STL not allowed)
#include <iostream>
#include <fstream>
#include <string>
#include "trie.h"

#define TEST_MODE 0


int main(){

    Trie trie = Trie();

    auto load = [&trie]() {
        // load all words in corpus.txt into the trie
        std::ifstream file("corpus.txt");
        std::string word;
        while (file >> word) {
            trie.insert(word);
        }
        std::cout << "success" << std::endl;
    };

    auto insert = [&trie](const std::string& word) {
        if(trie.insert(word)) {
            std::cout << "success" << std::endl;
        } else {
            std::cout << "failure" << std::endl;
        }
    };

    auto numWords = [&trie](const std::string& word) {
        try {
            auto count = trie.numWords(word);
            std::cout << "count is " << count << std::endl;
        } catch (std::invalid_argument& e) {
            std::cout << "not found" << std::endl;
        }
    };

    auto size = [&trie]() {
        std::cout << "number of words is " << trie.numWords() << std::endl;
    };

    auto erase = [&trie](std::string word) {
        bool deleted = trie.erase(word);
        std::cout << (deleted ? "success" : "failure") << std::endl;
    };

    auto empty = [&trie]() {
        std::cout << "empty " << (trie.empty() ? "1" : "0") << std::endl;
    };

    #ifdef TEST_MODE
    while (true) {
        std::string command;
        std::cin >> command;

        if (command == "load") {
            load();
        } else if (command == "i") {
            std::string word;
            std::cin >> word;
            insert(word);
        } else if (command == "c") {
            std::string prefix;
            std::cin >> prefix;
            numWords(prefix);

        } else if (command == "e") {
            std::string word;
            std::cin >> word;
            erase(word);

        } else if (command == "p") {
            trie.printTrie();
        } else if (command == "spellcheck") {
            std::string word;
            std::cin >> word;
            trie.spellcheck(word);
        } else if (command == "empty") {
            empty();
        } else if (command == "clear") {
            trie.clear();
        } else if (command == "size")  {
            size();
        } else if (command == "exit")  {
            break;
        }
        else {
            std::cout << "Invalid command, try again." << std::endl;
        }

    }
    #endif

    #ifndef TEST_MODE
    insert("A");
    insert("AB");
    insert("ABC");
    insert("AN");
    insert("BECAUSE");
    insert("BEC");
    insert("BANK");
    trie.printTrie();

    trie.spellcheck("ANIMAL");

    
    // load();
    // size();
    // trie.clear();
    // size();


    // /* Deletion */

    // erase("CAR");
    // erase("CAT");
    // erase("CAN");
    // erase("DO");
    // trie.printTrie();
    // erase("DOG");
    // erase("DOT");

    // numWords("DZJKD");
    // numWords("DO");
    #endif

    return 0;
}
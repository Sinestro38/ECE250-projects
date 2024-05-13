// driver.cpp
#include <iostream>
#include <string>
#include "tokenize.h"
#include <cassert>
#include <fstream>
#include <sstream>

// # define TEST_MODE 0

void test_hash_table();

int main(){

    #ifndef TEST_MODE
    std::string command;
    std::cin >> command;
    while (command != "create") {
        std::cout << "First command must be create" << std::endl;
        std::cin >> command;
        return 0;
    }
    unsigned int size;
    std::cin >> size;

    Dictionary dict{size};
    std::cout << "success" << std::endl;

    while (true) {
        std::string command;
        std::cin >> command;

        if (command == "insert") {
            std::string word;
            std::cin >> word;
            bool success = dict.insert(word);
            if (success) {
                std::cout << "success" << std::endl;
            } else {
                std::cout << "failure" << std::endl;
            }
        } else if (command == "load") {
            std::string filename;
            std::cin >> filename;

            bool success = false;
            std::ifstream file(filename.c_str());
            std::string line;
            while (std::getline(file, line)) {
                std::istringstream iss(line);
                std::string word;
                while (iss >> word) {
                    success |= dict.insert(word);
                }
            }
            file.close();
            std::cout << (success ? "success" : "failure") << std::endl;

        } else if (command == "tok") {
            std::string word;
            std::cin >> word;
            int token = dict.token(word);
            std::cout << token << std::endl;
        } else if (command == "ret") {
            int token;
            std::cin >> token;
            std::string word = dict.ret(token);
            std::cout << word << std::endl;
        } else if (command == "tok_all") {
            // Tokenize all words, which are separated by
            // white spaces only. For each non-existing word
            // print -1
            std::string line{};
            std::getline(std::cin, line);
            std::istringstream iss(line);
            std::string word;
            while (iss >> word) {
                int token = dict.token(word);
                std::cout << token << " ";
            }
            std::cout << std::endl;
        } else if (command == "ret_all") { // TODO
            // Retrieve the words of all tokens, which
            // are separated by white spaces. For
            // each non-existing token print N/A
            std::string line{};

            std::getline(std::cin, line); 
            std::istringstream ss(line);
            std::string something;

            // Parse the line using a string stream
            while (ss >> something) {
                int index = std::stoi(something);
                std::string word = dict.ret(index);
                std::cout << word << " ";
            }
        
            std::cout << std::endl;
        } else if (command == "print") {
            int k;
            std::cin >> k;
            dict.print(k);
        } else if (command == "exit")  {
            break;
        }
        else {
            std::cout << "Invalid command, try again." << std::endl;
        }

    }
    #endif

    
    #ifdef TEST_MODE
    Dictionary dict(4);

    dict.insert("bang");
    dict.insert("jang");
    dict.insert("gang");
    dict.insert("aang");
    dict.insert("tang");
    dict.insert("kang");

    test_hash_table();
    #endif

    return 0;
}

void test_hash_table() {
    // Test the HashTable with string keys and integer values
    HashTable<std::string, int> hashTable{10};

    // Test insert method
    hashTable.insert(std::make_pair("keyA", 1));
    hashTable.insert(std::make_pair("keyB", 2));
    hashTable.insert(std::make_pair("keyC", 3));
    hashTable.insert(std::make_pair("keyD", 4));
    hashTable.insert(std::make_pair("keyE", 5));
    hashTable.insert(std::make_pair("keyF", 6));
    hashTable.insert(std::make_pair("keyG", 7));
    hashTable.insert(std::make_pair("keyH", 8));
    hashTable.insert(std::make_pair("keyI", 9));
    hashTable.insert(std::make_pair("keyJ", 10));

    // Test get method
    std::cout << "BANG BANG" << std::endl;
    assert(hashTable.get("keyA") == 1);
    assert(hashTable.get("keyB") == 2);

    // Test remove method
    // Add test cases for remove method

    // Test get_size method
    assert(hashTable.get_size() == 10);
    assert(hashTable.get_max_size() == 20);

    std::cout << "All Hash table tests passed successfully!" << std::endl;
}

/*
WHAT I LEARNED FROM SAD 9hr DEBUGGING SESSION:
Why changing the object declaration from `Dictionary dict; dict = Dictionary{4};` to `Dictionary dict{4};` resolved the problem:

In the original code:
```cpp
Dictionary dict;
dict = Dictionary{4};
```

Here's what happens:
1. The first line `Dictionary dict;` creates a `Dictionary` object named `dict` using the default constructor. 
 This default constructor does not allocate memory for the `HashTable` member variable `hash_table`.

2. The second line `dict = Dictionary{4};` creates a temporary `Dictionary` object using the constructor that 
 takes an `unsigned int` parameter (size). This temporary object's `hash_table` member variable is properly initialized 
 with the specified size.

3. The assignment operator `=` is then used to assign the temporary `Dictionary` object to the `dict` object. 
 However, since the `Dictionary` class does not have a custom assignment operator defined, the default assignment 
 operator is used.

4. The default assignment operator performs a memberwise assignment, which means it assigns each member variable of
 the temporary object to the corresponding member variable of `dict`. In this case, it assigns the `hash_table` member
 variable of the temporary object to the `hash_table` member variable of `dict`.

5. After the assignment, the temporary `Dictionary` object goes out of scope and its destructor is called. This
 destructor frees the memory allocated for the `hash_table` member variable.

6. Now, the `dict` object's `hash_table` member variable is pointing to the memory that was just freed by the temporary
 object's destructor. This leads to the use-after-free error when `dict.insert("bang")` is called, as it tries to access
 the freed memory.

On the other hand, when you changed the code to:
```cpp
Dictionary dict{4};
```

This line creates a `Dictionary` object named `dict` using the constructor that takes an `unsigned int` parameter (size). 
 The `hash_table` member variable of `dict` is properly initialized with the specified size, and no temporary objects are
 involved.

The `dict` object now has a valid `hash_table` member variable with memory properly allocated, and the use-after-free
 error is avoided.

In summary, the issue was caused by the unintended destruction of the temporary `Dictionary` object, which freed the memory
 that was later accessed by the `dict` object. By directly initializing the `dict` object with the appropriate constructor,
  the memory is properly managed, and the issue is resolved.

This highlights the importance of properly managing object lifetimes and being cautious when using assignment operators,
 especially when dealing with objects that manage dynamic memory allocation.
*/
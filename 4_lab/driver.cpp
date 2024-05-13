// include libraries here
#include <iostream>
#include <string>
#include "graph.h"
#include <fstream>
#include <sstream> // Added to use istringstream

using namespace std;

int main(){
    Graph myg{};

    // myg.insert(1, 2, 2.4, 4.2);
    // myg.load("./files/4-load.txt");

    while (true) {
        string line;
        getline(cin, line);
        istringstream iss{line};
        string command;
        iss >> command;

        if (command == "insert") {
            int a, b;
            double d, s;
            iss >> a >> b >> d >> s;
            myg.insert(a, b, d, s);
        } else if (command == "load") {
            string filename;
            iss >> filename;
            myg.load(filename);
        } else if (command == "traffic") {
            int a, b;
            double aprime;
            iss >> a >> b >> aprime;
            myg.traffic(a, b, aprime);
        } else if (command == "update") {
            string filename;
            iss >> filename;
            myg.update(filename);
        } else if (command == "print") {
            int a;
            iss >> a;
            myg.print(a);
        } else if (command == "delete") {
            int a;
            iss >> a;
            myg.delete_n(a);
        } else if (command == "path") {
            int a, b;
            iss >> a >> b;
            myg.path(a, b);
        } else if (command == "lowest") {
            int a, b;
            iss >> a >> b;
            myg.lowest(a, b);
        } else if (command == "exit") {
            break;
        }
    }
    
}
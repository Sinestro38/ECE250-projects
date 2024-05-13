#pragma once
#include <iostream>
#include <unordered_map>

using namespace std;

struct Edge{
    double d; // distance
    double s; // speed limit > 0
    double a = 1.0; // traffic > 0

    Edge() = default;

    Edge(double d, double s){
        this->d = d;
        this->s = s;
    }

    friend ostream& operator<<(ostream& os, const Edge& e){
        os << "Distance: " << e.d << " Speed Limit: " << e.s << " Traffic: " << e.a;
        return os;
    }
};

class Graph{
private:
    std::unordered_map<int, std::unordered_map<int, Edge>> g;
    bool edge_exists(int a, int b);
    bool node_exists(int a);
    void dijkstra(int source, int dest, unordered_map<int, double>& dist, unordered_map<int, int>& prev);


public:
    void insert(int a, int b, double d, double s, bool silence = false);
    void load(const string& filename);
    void traffic(int a, int b, double aprime, bool silence = false); // update the adjustment factor 
    void update(const string& filename);
    void print(int a);
    void delete_n(int a);
    void path(int a, int b);
    void lowest(int a, int b);
};
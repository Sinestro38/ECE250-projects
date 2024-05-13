// tokenize.h
#pragma once
#include <iostream>
#include <vector>
#include <cctype>
#include <forward_list>
#include <exception>

using kv_pair = std::pair<std::string, unsigned int>;

class KeyError : public std::exception {
public:
    KeyError(const std::string& key);
    const char* what() const noexcept override;

private:
    std::string key_;
    std::string error_msg;
};

/**
 * @brief A general hash table template for key-value pairs.
 * 
 * @tparam key_t The type of the key.
 * @tparam value_t The type of the value.
 */
template<typename key_t, typename value_t>
class HashTable;

/**
 * @brief A hash table template specialization for key-value pairs with string keys.
 * 
 * @tparam value_t The type of the value.
 */
template<typename value_t>
class HashTable<std::string, value_t> {
    using ht_pair = std::pair<std::string, value_t>;
private:
    std::forward_list<ht_pair>* array;
    unsigned int curr_size;
    unsigned int capacity;
    double threshold = 0.8;

    unsigned int hash(const std::string &str) const;
    void double_size();
    inline double load_factor() const;
public:
    HashTable() = default;
    HashTable(const unsigned int size);
    HashTable(const HashTable& other) = delete;
    ~HashTable();
    bool insert(const ht_pair& pair);
    value_t get(const std::string& word) const;
    value_t operator[](const std::string& word) const;
    HashTable& operator=(const HashTable& other) = delete;
    unsigned int get_size() const;
    unsigned int get_max_size() const;
    bool remove(std::string word);
    void print(unsigned int k) const;
    void debug_print(std::string prefix = "") const;
};


class Dictionary {
private:
    std::vector<std::string> words;
    HashTable<std::string, int> hash_table;

public:
    Dictionary(unsigned int size);
    Dictionary() = default;
    Dictionary(const Dictionary& other) = delete;
    bool insert(const std::string & str);
    int token(const std::string& word);
    std::string ret(const int& token);
    void print(unsigned int k);
};

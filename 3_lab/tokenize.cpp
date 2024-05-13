#include "tokenize.h"

/**
 * @brief Constructor for KeyError exception class.
 * @param key The key that caused the error.
 */
KeyError::KeyError(const std::string& key) : key_(key) {
    error_msg = "KeyError: " + key_;
}

/**
 * @brief Returns the error message as a C-style string.
 * @return The error message.
 */
const char* KeyError::what() const noexcept {
    return error_msg.c_str();
}

/**
 * @brief Hash function to calculate the index for a given string key.
 * @param str The string key to hash.
 * @return The hashed index.
 */
template <typename value_t>
unsigned int HashTable<std::string, value_t>::hash(const std::string &str) const {
    unsigned int ans = 0;
    for (auto it = str.begin(); it != str.end(); ++it)
        ans = ans * 29 + *it;
    return ans % this->capacity;
}

/**
 * @brief Doubles the size of the hash table and rehashes all elements.
 */
template <typename value_t>
void HashTable<std::string, value_t>::double_size() {
    this->capacity *= 2;
    auto new_array = new std::forward_list<ht_pair>[this->capacity];
    for (unsigned int i = 0; i < this->capacity / 2; i++) {
        for (auto item : this->array[i]) {
            auto new_hash_int = hash(item.first);
            new_array[new_hash_int].push_front(item);
        }
    }
    delete[] this->array;
    this->array = new_array;
}

/**
 * @brief Calculates the load factor of the hash table.
 * @return The load factor.
 */
template <typename value_t>
inline double HashTable<std::string, value_t>::load_factor() const {
    return static_cast<double>(this->curr_size) / this->capacity;
}

/**
 * @brief Constructor for HashTable class.
 * @param size The initial size of the hash table.
 */
template <typename value_t>
HashTable<std::string, value_t>::HashTable(const unsigned int size) : curr_size{0}, capacity{size} {
    array = new std::forward_list<ht_pair>[size];
}

/**
 * @brief Destructor for HashTable class.
 */
template <typename value_t>
HashTable<std::string, value_t>::~HashTable() {
    delete[] array;
}

/**
 * @brief Inserts a key-value pair into the hash table.
 * @param pair The key-value pair to insert.
 * @return True if insertion is successful, false if the key already exists or is invalid.
 */
template <typename value_t>
bool HashTable<std::string, value_t>::insert(const ht_pair& pair) {
    for (auto c : pair.first) {
        if (!std::isalpha(c)) {
            return false;
        }
    }
    auto hash_int = hash(pair.first);
    auto& this_list = this->array[hash_int];
    if (this_list.empty()) {
        this_list.push_front(pair);
    } else {
        for (auto item : this_list) {
            if (item.first == pair.first) {
                return false;
            }
        }
        this_list.push_front(pair);
    }
    this->curr_size++;

    if (this->load_factor() >= threshold) {
        double_size();
    }
    
    return true;
}

/**
 * @brief Retrieves the value associated with a given string key.
 * @param word The string key to search for.
 * @return The value associated with the key.
 * @throws KeyError if the key is not found in the hash table.
 */
template <typename value_t>
value_t HashTable<std::string, value_t>::get(const std::string& word) const {
    auto hash_int = hash(word);
    if (!array[hash_int].empty()) {
        for (auto item : array[hash_int]) {
            if (item.first == word) {
                return item.second;
            }
        }
    }
    throw KeyError(word);
}

/**
 * @brief Retrieves the value associated with a given string key using the [] operator.
 * @param word The string key to search for.
 * @return The value associated with the key.
 */
template <typename value_t>
value_t HashTable<std::string, value_t>::operator[](const std::string& word) const {
    return get(word);
}

/**
 * @brief Returns the current size of the hash table.
 * @return The current size.
 */
template <typename value_t>
unsigned int HashTable<std::string, value_t>::get_size() const {
    return this->curr_size;
}

/**
 * @brief Returns the maximum size (capacity) of the hash table.
 * @return The maximum size.
 */
template <typename value_t>
unsigned int HashTable<std::string, value_t>::get_max_size() const {
    return this->capacity;
}

/**
 * @brief Prints the keys in the chain at a specific hash table position.
 * @param k The position in the hash table.
 */
template <typename value_t>
void HashTable<std::string, value_t>::print(unsigned int k) const {
    if (k < 0 || k >= this->capacity) {
        return;
    }
    if (!this->array[k].empty()) {
        for (auto item : array[k]) {
            std::cout << item.first << " ";
        }
        std::cout << std::endl;
    }
}

// Instantiate the template
template class HashTable<std::string, int>;

/**
 * @brief Constructor for Dictionary class.
 * @param size The initial size of the dictionary.
 */
Dictionary::Dictionary(unsigned int size) : hash_table(size) {
    words.reserve(size);
    words.push_back("");
}

/**
 * @brief Inserts a word into the dictionary.
 * @param str The word to insert.
 * @return True if insertion is successful, false otherwise.
 */
bool Dictionary::insert(const std::string & str) {
    unsigned int token = this->words.size();
    bool _inserted = this->hash_table.insert({str, token});
    if (!_inserted)
        return false;
    this->words.push_back(str);
    return true;
}

/**
 * @brief Retrieves the numeric token associated with a word.
 * @param word The word to search for.
 * @return The numeric token if the word exists, -1 otherwise.
 */
int Dictionary::token(const std::string& word) {
    try {
        return this->hash_table.get(word);
    } catch (const KeyError& e) {
        return -1;
    }    
}

/**
 * @brief Retrieves the word associated with a numeric token.
 * @param token The numeric token to search for.
 * @return The word if the token exists, "N/A" otherwise.
 */
std::string Dictionary::ret(const int& token) {
    if (token < 1 || token >= this->words.size()) {
        return std::string("N/A");
    }
    return this->words[token];
}

/**
 * @brief Prints the keys in the chain at a specific hash table position.
 * @param k The position in the hash table.
 */
void Dictionary::print(unsigned int k) {
    this->hash_table.print(k);
}

// template <typename value_t>
// HashTable<std::string, value_t>::HashTable(const HashTable& other) : curr_size(other.curr_size), capacity(other.capacity) {
//     // Allocate a new array
//     array = new std::forward_list<ht_pair>[capacity];

//     // Copy the elements from the other hash table
//     for (unsigned int i = 0; i < capacity; ++i) {
//         array[i] = other.array[i];
//     }
// }

// template <typename value_t>
// HashTable<std::string, value_t>& HashTable<std::string, value_t>::operator=(const HashTable<std::string, value_t>& other) {
//     if (this != &other) {
//         // Create a temporary copy of the other hash table
//         HashTable<std::string, value_t> temp(other);
//         // Swap the contents of the temporary copy with the current object
//         std::swap(capacity, temp.capacity);
//         std::swap(curr_size, temp.curr_size);
//         std::swap(array, temp.array);
//     }
//     return *this;
// }


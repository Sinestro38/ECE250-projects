#pragma once
#include <iostream>

class TrieNode {
private:
    TrieNode* _children[26];
    bool _is_last_char;
public:
    /**
     * @brief Constructor for TrieNode object
     * Initializes _is_last_char and the _children array to nullptrs
     * @param last_char (bool) - whether the node is the last character of a word
    */
    TrieNode(bool last_char = false);

    /**
     * @brief Destructor for TrieNode object
     * Recursively deletes all children of the node
    */
    ~TrieNode();

    TrieNode*& getChild(char uppercaseLetter);
    TrieNode*& operator[](char uppercaseLetter); // alias for getChild

    /**
     * @brief Sets a child for the current node
     * 
     * This function creates a new TrieNode with the specified last_char value
     * and assigns it to the appropriate index in the _children array based on
     * the given uppercaseLetter. The index is calculated by subtracting 'A' from
     * the uppercaseLetter to get a value between 0 and 25, corresponding to each
     * letter of the English alphabet.
     * 
     * @param uppercaseLetter (char) - The uppercase letter for which to set the child node.
     * @param last_char (bool) - Indicates whether the new child node is the last character of a word.
     */
    TrieNode* setChild(char uppercaseLetter, bool last_char = false);

    void setChildNull(char uppercaseLetter);

    bool isLastChar();

    void setLastChar(bool last_char);

    bool has_children();
};

class Trie {
private:
    TrieNode* _root;
    unsigned int num_words;
    TrieNode* _findNode(const std::string& word);

public:
    Trie() : _root{new TrieNode()}, num_words{0} {};
    ~Trie() {
        delete this->_root;
    }

    TrieNode* getRoot() { return this->_root; }

    /**
    * @brief Inserts a word into the Trie
    * 
    * This function iterates through each character of the input word, converting it to an uppercase letter if necessary,
    * and traverses the Trie from the root, creating new nodes as needed. Each node represents a character in the word.
    * The last character's node is marked to indicate the end of the word.
    * 
    * @param word (std::string) - The word to be inserted into the Trie.
    * @return (bool) - Returns true if the word was successfully inserted, false if the word already exists in the Trie.
    */
    bool insert(const std::string& word);

    bool erase(std::string& word);

    bool eraseRecursive(std::string& word, TrieNode*& curr_node, int i = 0);

    void printTrie();

    void printTrieRecursive(TrieNode* root, std::string& word, bool& printed);

    /**
     * @brief Counts the total number of words stored in the Trie.
     * 
     * This function initiates the recursive counting of words stored in the Trie by calling
     * the numWordsRecursive function starting from the root node.
     * 
     * @return (unsigned int) - The total number of words in the Trie.
     */
    unsigned int numWords();

    /**
     * @brief Counts the number of words in the Trie that start with a given prefix.
     * 
     * This function first finds the node corresponding to the last character of the prefix
     * using the _findNode function. If such a node does not exist (indicating that no word
     * in the Trie starts with the given prefix), it throws an std::invalid_argument exception.
     * If the node is found, it then calls the numWordsRecursive function starting from this node
     * to count all words that extend from this prefix.
     * 
     * @param prefix (std::string) - The prefix for which to count the number of words.
     * @return (unsigned int) - The total number of words in the Trie that start with the given prefix.
     * @throws std::invalid_argument if the prefix is not found in the Trie.
     */
    unsigned int numWords(std::string prefix);

    void spellcheck(const std::string& word);

    /**
     * @brief Recursively counts the number of words in the Trie.
     * 
     * This function traverses the Trie from a given node (root) downwards, counting the number
     * of words. A word is identified by its last character node being marked as such. The function
     * iterates through all possible child nodes (corresponding to letters 'A' to 'Z') of the current
     * node. If a child node exists, it checks if it is marked as the last character of a word, in which
     * case it increments the word count. It then recursively calls itself for the child node to continue
     * the traversal. The count of words is accumulated and returned up the call stack.
     * 
     * @param root (TrieNode*) - The current node from which to count words.
     * @param numWords (unsigned int) - The current count of words. Defaults to 0 for the initial call.
     * @return (unsigned int) - The total number of words found from the current node downwards.
     */
    unsigned int numWordsRecursive(TrieNode* root, unsigned int numWords = 0);

    void clear();

    bool empty();

};
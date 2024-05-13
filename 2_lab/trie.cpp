#include "trie.h"

/*
    * @brief Constructor for TrieNode object
    * Initializes _is_last_char and the _children array to nullptrs
    * @param last_char (bool) - whether the node is the last character of a word
*/
TrieNode::TrieNode(bool last_char) {
    _is_last_char = last_char;
    for (int i = 0; i < 26; i++)
        _children[i] = nullptr;
}

/*
    * @brief Destructor for TrieNode object
    * Recursively deletes all children of the node
*/
TrieNode::~TrieNode() {
    for (int i = 0; i < 26; i++) {
        if (_children[i] != nullptr) {
            delete _children[i];
        }
    }
}

TrieNode*& TrieNode::getChild(char uppercaseLetter) {
    /*
    Args:
        uppercaseLetter (char) - the character path node to return
    Returns:
        node (Trienode*) - the address of the node down the character's path
    return nullptr; // Placeholder to satisfy return type. Actual implementation is context-specific and not provided.
    */
    if (uppercaseLetter < 'A' || uppercaseLetter > 'Z')
        throw std::invalid_argument("Character must be an uppercase ASCII letter.");

    unsigned int index = uppercaseLetter - 0x41;
    return this->_children[index];
}

TrieNode*& TrieNode::operator[](char uppercaseLetter) {
    // Alias for getChild
    return this->getChild(uppercaseLetter);
}

/**
 * @brief Sets a child for the current node
 * @param uppercaseLetter (char) - The uppercase letter for which to set the child node.
 * @param last_char (bool) - Indicates whether the new child node is the last character of a word.
 */
TrieNode* TrieNode::setChild(char uppercaseLetter, bool last_char) {
    if (uppercaseLetter < 'A' || uppercaseLetter > 'Z')
        throw std::invalid_argument("Character must be an uppercase ASCII letter.");

    unsigned int index = uppercaseLetter - 0x41;
    this->_children[index] = new TrieNode(last_char);

    return this->_children[index];
}


void TrieNode::setChildNull(char uppercaseLetter) {
    unsigned int index = uppercaseLetter - 0x41;
    this->_children[index] = nullptr;
}

bool TrieNode::isLastChar() {
    return this->_is_last_char;
}

void TrieNode::setLastChar(bool last_char) {
    this->_is_last_char = last_char;
}

bool TrieNode::has_children() { 
    for (int i = 0; i < 26; i++) {
        if (_children[i] != nullptr) {
            return true;
        }
    }
    return false;
}

/*
TRIE CLASS IMPLEMENTATION
*/

TrieNode* Trie::_findNode(const std::string& word) {
    TrieNode* curr = this->getRoot();

    for (int i{0}; i < word.length(); i++) {
        curr = curr->getChild(word[i]);

        if (curr == nullptr) {
            return nullptr;
        }
    }
    return curr;
}


/**
* @brief Inserts a word into the Trie
* @param word (std::string) - The word to be inserted into the Trie.
* @return (bool) - Returns true if the word was successfully inserted, false if the word already exists in the Trie.
*/
bool Trie::insert(const std::string& word) {
    auto str_len = word.length();

    TrieNode* curr = this->_root;

    for (int i = 0; i < str_len; i++) {
        bool is_last_char = (i == (str_len-1));

        if (curr->getChild(word[i]) == nullptr) {
            curr = curr->setChild(word[i], is_last_char);
        } else {
            curr = curr->getChild(word[i]);
            // If this is the last character of this word and this node corresponds 
            // to the last character of another word, then these two words must actually
            // be equivalent since it was the same path travelled
            if (is_last_char && curr->isLastChar())
                return false; // unsucessful insert due to duplicate word
            else if (is_last_char && !curr->isLastChar())
                curr->setLastChar(true);
        }
    }

    this->num_words++;
    return true;
}

bool Trie::erase(std::string& word) {
    TrieNode*& first_char_pointer = this->_root->getChild(word[0]);
    bool deleted = eraseRecursive(word, first_char_pointer);
    if (deleted)
        this->num_words--;
    return deleted;
}

bool Trie::eraseRecursive(std::string& word, TrieNode*& curr_node, int i) {
    /* Case 1: Node to be deleted is a leaf node with no children
        -> Mark the node to be deleted as not an end of word anymore
        -> Delete the node
        -> If parent, now has no children delete the parent
    Case 2: Node to be deleted is not a leaf node
        -> Mark the node to be deleted as not an end of word anymore
    */
    // get parent of node to be deleted
    if (curr_node == nullptr)
        return false;

    if ( i == (word.length() - 1) ) {
        curr_node->setLastChar(false);
        if (!curr_node->has_children()) {
            delete curr_node;
            curr_node = nullptr;
        }
        return true;
    } else {
        TrieNode*& child_of_next_char = curr_node->getChild(word[i+1]);
        bool deleted = eraseRecursive(word, child_of_next_char, i+1);

        if (!curr_node->has_children() && !curr_node->isLastChar()) {
            delete curr_node;
            curr_node = nullptr;
        }   
        return deleted;
    }
}

void Trie::printTrie() {
    // print all words in the Trie in alphabetical order on a single line. 
    // No output if the Trie is empty.
    std::string word = "";
    bool printed = false;
    printTrieRecursive(this->_root, word, printed);
    if (printed)
        std::cout << std::endl;
}

void Trie::printTrieRecursive(TrieNode* root, std::string& word, bool& printed) {
    for (unsigned char i = 'A'; i <= 'Z'; i++) {
        const auto child_ptr = root->getChild(i);
        if (child_ptr != nullptr) {
            word.push_back(i);
            if (child_ptr->isLastChar()) {
                std::cout << word << " ";
                printed = true;
            }
            printTrieRecursive(child_ptr, word, printed);
            word.pop_back();
        }
    }
}
/**
 * @brief Counts the total number of words stored in the Trie.
 * @return (unsigned int) - The total number of words in the Trie.
 */
unsigned int Trie::numWords() {
    return this->num_words;
}

/**
 * @brief Counts the number of words in the Trie that start with a given prefix.
 * @param prefix (std::string) - The prefix for which to count the number of words.
 * @return (unsigned int) - The total number of words in the Trie that start with the given prefix.
 * @throws std::invalid_argument if the prefix is not found in the Trie.
 */
unsigned int Trie::numWords(std::string prefix) {
    auto prefix_ptr = _findNode(prefix);

    if (prefix_ptr == nullptr) {
        throw std::invalid_argument("not found");
    }

    return numWordsRecursive(prefix_ptr, prefix_ptr->isLastChar());
}

void Trie::spellcheck(const std::string& word) {
    // If work is in the Trie, print correct,
    // If not, suggest words that have the maximum common prefix
    // Else, no output if no suggestion found
    TrieNode* node = this->getRoot();
    if (node->getChild(word[0]) == nullptr) { 
        // Case 1: The word has no match in the Trie for the first letter
        std::cout << std::endl;
        return;
    }

    for (int i{0}; i < word.length(); i++) {
        TrieNode*& child_ptr = node->getChild(word[i]);

        if (child_ptr == nullptr || (i == (word.length()-1) && !child_ptr->isLastChar()) ){
            std::string common_prefix = child_ptr ? word : word.substr(0, i); // word[:i]
            bool printed = false;
            if (node->isLastChar())
                std::cout << common_prefix << " ";
            this->printTrieRecursive(child_ptr ? child_ptr : node, common_prefix, printed);
            break;
        }
        else if ( (i == (word.length()-1) ) && child_ptr->isLastChar() ) {
            // if last character in search word AND if this character marks an end of word in Trie
            // then search word exists in Trie
            std::cout << "correct";
            break;
        } else {
            node = child_ptr;
        }
    }

    std::cout << std::endl;
}

/**
 * @brief Recursively counts the number of words in the Trie.
 * @param root (TrieNode*) - The current node from which to count words.
 * @param numWords (unsigned int) - The current count of words. Defaults to 0 for the initial call.
 * @return (unsigned int) - The total number of words found from the current node downwards.
 */
unsigned int Trie::numWordsRecursive(TrieNode* root, unsigned int numWords) {
    // We want to add the number of words in each branch
    for (unsigned char i = 'A'; i <= 'Z'; i++) {
        const auto child_ptr = root->getChild(i);
        if (child_ptr != nullptr) {
            numWords += child_ptr->isLastChar(); // Increment if this node marks the end of a word
            numWords = numWordsRecursive(child_ptr, numWords); // Recurse into the child node
        }
    }
    return numWords;
}

void Trie::clear() {
    // keep root node, delete all children recursively
    for (unsigned char i = 'A'; i <= 'Z'; i++) {
        TrieNode*& child_ptr = this->_root->getChild(i);
        if (child_ptr != nullptr) {
            delete child_ptr;
            child_ptr = nullptr;
        }
    }
    this->num_words = 0;
    std::cout << "success" << std::endl;
}

bool Trie::empty() {
    return !this->_root->has_children();
}
#ifndef SKIP_LIST_HPP
#define SKIP_LIST_HPP

#include <iostream>
#include <vector>
#include <random>
#include <ctime>

template<typename T>
class SkipList {
private:
    struct Node {
        T value;
        std::vector<Node*> forward;
        Node(const T& val, int level) : value(val), forward(level, nullptr) {}
    };

    std::vector<Node*> head;
    int maxLevel;
    int currentLevel;
    float probability;
    std::mt19937 rng;
    std::uniform_real_distribution<float> dist;

    int randomLevel() {
        int level = 1;
        while (dist(rng) < probability && level < maxLevel) {
            level++;
        }
        return level;
    }

public:
    SkipList() : maxLevel(32), currentLevel(1), probability(0.5), rng(std::random_device{}()), dist(0.0, 1.0) {
        head.assign(maxLevel, nullptr);
    }

    ~SkipList() {
        Node* current = head[0];
        while (current != nullptr) {
            Node* next = current->forward[0];
            delete current;
            current = next;
        }
    }

    // Insert a value into the skip list. If the value already exists, do nothing.
    void insert(const T & item) {
        std::vector<Node*> update(maxLevel, nullptr);
        Node* current = nullptr;

        for (int i = currentLevel - 1; i >= 0; i--) {
            Node* nextNode = (current == nullptr) ? head[i] : current->forward[i];
            while (nextNode != nullptr && nextNode->value < item) {
                current = nextNode;
                nextNode = current->forward[i];
            }
            update[i] = current;
        }

        Node* nextNode = (current == nullptr) ? head[0] : current->forward[0];

        if (nextNode != nullptr && !(nextNode->value < item) && !(item < nextNode->value)) {
            // Item already exists
            return;
        }

        int newLevel = randomLevel();

        if (newLevel > currentLevel) {
            for (int i = currentLevel; i < newLevel; i++) {
                update[i] = nullptr;
            }
            currentLevel = newLevel;
        }

        Node* newNode = new Node(item, newLevel);

        for (int i = 0; i < newLevel; i++) {
            if (update[i] == nullptr) {
                newNode->forward[i] = head[i];
                head[i] = newNode;
            } else {
                newNode->forward[i] = update[i]->forward[i];
                update[i]->forward[i] = newNode;
            }
        }
    }

    // Search for a value in the skip list
    bool search(const T & item) {
        Node* current = nullptr;

        for (int i = currentLevel - 1; i >= 0; i--) {
            Node* nextNode = (current == nullptr) ? head[i] : current->forward[i];
            while (nextNode != nullptr && nextNode->value < item) {
                current = nextNode;
                nextNode = current->forward[i];
            }
        }

        Node* nextNode = (current == nullptr) ? head[0] : current->forward[0];

        if (nextNode != nullptr && !(nextNode->value < item) && !(item < nextNode->value)) {
            return true;
        }

        return false;
    }

    // Delete a value from the skip list. If the value does not exist in the skip list, do nothing.
    void deleteItem(const T & item) {
        std::vector<Node*> update(maxLevel, nullptr);
        Node* current = nullptr;

        for (int i = currentLevel - 1; i >= 0; i--) {
            Node* nextNode = (current == nullptr) ? head[i] : current->forward[i];
            while (nextNode != nullptr && nextNode->value < item) {
                current = nextNode;
                nextNode = current->forward[i];
            }
            update[i] = current;
        }

        Node* nextNode = (current == nullptr) ? head[0] : current->forward[0];

        if (nextNode != nullptr && !(nextNode->value < item) && !(item < nextNode->value)) {
            for (int i = 0; i < currentLevel; i++) {
                Node* expectedNext = (update[i] == nullptr) ? head[i] : update[i]->forward[i];
                if (expectedNext != nextNode) {
                    break;
                }
                if (update[i] == nullptr) {
                    head[i] = nextNode->forward[i];
                } else {
                    update[i]->forward[i] = nextNode->forward[i];
                }
            }

            delete nextNode;

            while (currentLevel > 1 && head[currentLevel - 1] == nullptr) {
                currentLevel--;
            }
        }
    }
};

#endif

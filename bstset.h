#ifndef BSTSET_H_
#define BSTSET_H_

#include <iostream>
#include <stack>

template <typename T>
class BSTSet {
private:
    struct Node {
        T value;
        Node* left;
        Node* right;

        Node(const T& val) : value(val), left(nullptr), right(nullptr) {}
    };

    Node* root;

    // Helper function to insert a value into the tree
    Node* insertHelper(Node* node, const T& value) {
        if (node == nullptr) {
            return new Node(value);
        }

        if (value < node->value) {
            node->left = insertHelper(node->left, value);
        }
        else if (value > node->value) {
            node->right = insertHelper(node->right, value);
        }
        else {
            // Replace existing value
            node->value = value;
        }

        return node;
    }

    // Helper function to find a value in the tree
    Node* findHelper(Node* node, const T& value) const {
        if (node == nullptr || node->value == value) {
            return node;
        }

        if (value < node->value) {
            return findHelper(node->left, value);
        }
        else {
            return findHelper(node->right, value);
        }
    }

    // Helper function to find the first element not smaller than a given value
    Node* findFirstNotSmallerHelper(Node* node, const T& value) const {
        Node* result = nullptr;
        while (node != nullptr) {
            if (node->value >= value) {
                result = node;
                node = node->left;
            }
            else {
                node = node->right;
            }
        }
        return result;
    }

    // Helper function to delete the tree
    void deleteTree(Node* node) {
        if (node != nullptr) {
            deleteTree(node->left);
            deleteTree(node->right);
            delete node;
        }
    }

public:
    BSTSet() : root(nullptr) {}

    ~BSTSet() {
        deleteTree(root);
    }

    void insert(const T& value) {
        root = insertHelper(root, value);
    }

    class SetIterator {
    private:
        std::stack<Node*> stack;
        Node* root;

    public:
        SetIterator(Node* node, Node* root) : root(root) {
            pushLeft(node);
        }

        const T* get_and_advance() {
            if (stack.empty()) {
                return nullptr;
            }

            Node* current = stack.top();
            stack.pop();

            pushLeft(current->right);

            return &current->value;
        }

        void pushLeft(Node* node) {
            while (node != nullptr) {
                stack.push(node);
                node = node->left;
            }
        }
    };

    SetIterator find(const T& value) const {
        Node* node = findHelper(root, value);
        return SetIterator(node, root);
    }

    SetIterator find_first_not_smaller(const T& value) const {
        Node* node = findFirstNotSmallerHelper(root, value);
        return SetIterator(node, root);
    }
};

#endif

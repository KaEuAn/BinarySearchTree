#include <iostream>
#include <vector>
#include <random>
#include <stdio.h>
#include <bits/shared_ptr.h>


class Node {
    //left > right
    Node* left;
    Node* right;
    long long key;
    long long priority;
    long long count_behind;
    bool isReversed;
    long long make_more_by;

public:
    Node(long long k) : left(nullptr), right(nullptr), key(k), priority(std::rand()), count_behind(1),
                        isReversed(false), make_more_by(0) {}
    ~Node() {
        if (left)
            left->~Node();
        if (right)
            right->~Node();
        delete this;
    }
    long long count_behind_function() {
        return this ? count_behind : 0;
    }
    void update_count() {
        count_behind = left->count_behind_function() + right->count_behind_function() + 1;
    }
    friend void split(Node* my_vertex, double input_i, Node*& l, Node*& r);
    void merge(Node* l, Node* r) {
        if (!l || !r) {
            this = l ? l : r;
        }
        else if (l->priority < r->priority) {
            merge(r->left, l, r->left);
            this = r;
            update_count();
        } else{
            merge(l->right, l->right, r);
            this = l;
            update_count();
        }
    }

    void insert(Node* a) {
        if (this == nullptr)
            this = a;
        else if (a->priority > this->priority) {
            split(this, a->key, a->left, a->right);
            this = a;
            update_count();
        } else {
            (count_behind_function() < a->count_behind_function() ? left : right)->insert(a);
            update_count();
        }
    }
    void erase(long long a) {
        if (this->key == a) {
            Node* x = this;
            merge(this, this->left, this->right);
            delete x;
            x = nullptr;
        }
        else {
            (this->key > a ? this->right : this->left)->erase(a);
            this->update_count();
        }
    }
    Node* find(long long index) {
        long long count_left = left->count_behind_function() + 1;
        if (index == count_left)
            return this;
        if (index < count_left)
            return left->find(index);
        return right->find(index - count_left);
    }
};

struct BinarySearchTree{
    std::shared_ptr<Node> root;
    BinarySearchTree(int size, int* array) {
        auto root = new Node(array[0]);
        for (int i = 1; i < size; ++i) {
            Node* ver = new Node(i);
            root->insert(ver);
        }
    }
    void insert(long long key, long long index) {
        Node* new_node = new Node(key);

    }
};


void split(Node* my_vertex, double input_i, Node*& l, Node*& r) {
    if (my_vertex == nullptr) {
        l = nullptr;
        r = nullptr;
        return;
    } else if (my_vertex->left->count_behind_function() < input_i ) {
        split(my_vertex->right, input_i - my_vertex->left->count_behind_function() - 1, my_vertex->right, r);
        l = my_vertex;
        l->update_count();
    } else {
        split(my_vertex->left, input_i, l, my_vertex->left);
        r = my_vertex;
        r->update_count();
    }
    my_vertex->update_count();
}

//merge l and r to Node with root in my
void merge(Node*& my, Node* l, Node* r)



int main() {
    int n, m;
    std::cin >> n >> m;

    return 0;
}
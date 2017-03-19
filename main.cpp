#include <iostream>
#include <vector>
#include <random>
#include <stdio.h>
#include <bits/shared_ptr.h>

enum how {
    no,
    more,
    equal
};


class Node {
    //left > right
    Node* left;
    Node* right;
    long long key;
    long long priority;
    long long count_behind;
    long long sum_behind;
    bool is_reversed;
    long long make_more_by;
    long long make_equal_to;
    how must_be_chanded;

    long long count_behind_function() {
        return this ? count_behind : 0;
    }
    void update_count() {
        count_behind = left->count_behind_function() + right->count_behind_function() + 1;
    }
    void push() {
        if (is_reversed) {
            std::swap(left, right);
            is_reversed = false;
        }
        if (must_be_chanded == more) {
            if (left) {
                left->make_more_by = make_more_by;
                left->must_be_chanded = more;
            }
            if (right) {
                right->make_more_by = make_more_by;
                right->must_be_chanded = more;
            }
            key += make_more_by;
            make_more_by = 0;
        }
        if (must_be_chanded == equal) {
            if (left) {
                left->make_equal_to = make_equal_to;
                left->must_be_chanded = equal;
            }
            if (right) {
                right->make_equal_to = make_equal_to;
                right->must_be_chanded = equal;
            }
            key = make_equal_to;
            make_equal_to = key;
        }
        must_be_chanded = no;
    }
    void merge(Node* l, Node* r) {
        if (!l || !r) {
            this = l ? l : r;
            return;
        }
        push();
        if (l->priority < r->priority) {
            r->left->merge(l, r->left);
            this = r;
            update_count();
        } else{
            l->right->merge(l->right, r);
            this = l;
            update_count();
        }
    }

public:
    Node(long long k) : left(nullptr), right(nullptr), key(k), priority(std::rand()), count_behind(1),
                        sum_behind(1), is_reversed(false), make_more_by(0) {}
    ~Node() {
        if (left)
            left->~Node();
        if (right)
            right->~Node();
        delete this;
    }
    friend void split(Node* my_vertex, double input_i, Node*& l, Node*& r);

    void insert_by_index(Node* a, double index) {
        if (this == nullptr)
            this = a;
        push();
        if (a->priority > this->priority) {
            split(this, index - 1, a->left, a->right);
            this = a;
            update_count();
        } else {
            if (index <= left->count_behind_function() + 1)
                left->insert_by_index(a, index);
            else
                right->insert_by_index(a, index - left->count_behind_function() - 1);
            update_count();
        }
    }
    Node* find(long long index) {
        push();
        long long count_left = left->count_behind_function() + 1;
        if (index == count_left)
            return this;
        if (index < count_left)
            return left->find(index);
        return right->find(index - count_left - 1);
    }
    void erase() {
        Node* x = this;
        merge(left, right);
        delete x;
        x = nullptr;
    }
    void erase(long long index) {
        Node* deleting_node = find(index);
        deleting_node->erase();
    }
};

class BinarySearchTree{
    std::shared_ptr<Node> root;
public:
    BinarySearchTree(int size, int* array) {
        auto root = new Node(array[0]);
        for (int i = 1; i < size; ++i) {
            Node* ver = new Node(array[i]);
            root->insert_by_index(ver, i + 1);
        }
    }
    void insert(long long key, long long index) {
        Node* new_node = new Node(key);
        root->insert_by_index(new_node, index + 1);
    }
};


void split(Node* my_vertex, double input_i, Node*& l, Node*& r) {
    if (my_vertex == nullptr) {
        l = nullptr;
        r = nullptr;
        return;
    }
    my_vertex->push();
    if (my_vertex->left->count_behind_function() < input_i ) {
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

int main() {
    int array[5] = {1,2,3,4,5};
    BinarySearchTree my_tree(5, std::move(array));

}
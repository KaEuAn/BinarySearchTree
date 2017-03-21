#include <iostream>
#include <vector>
#include <random>
#include <stdio.h>
#include <memory>

enum how {
    no,
    more,
    equal
};


class Node {
    //left < right
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
    long long sum_behind_function() {
        return this ? sum_behind : 0;
    }
    void update_count() {
        count_behind = left->count_behind_function() + right->count_behind_function() + 1;
    }
    void update_sum() {
        sum_behind = left->sum_behind_function() + right->sum_behind_function() + key;
    }
    void update_all() {
        update_count();
        update_sum();
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


public:
    Node(long long k) : left(nullptr), right(nullptr), key(k), priority(std::rand()), count_behind(1),
                        sum_behind(1), is_reversed(false), make_more_by(0), make_equal_to(key), must_be_chanded(no) {}
    ~Node() {
        if (left)
            delete left;
        if (right)
            delete right;
        //delete this;
    }
    friend void split(Node* my_vertex, double input_i, Node*& l, Node*& r);
    friend Node* merge(Node* l, Node* r);

    Node* insert_by_index(Node* a, double index) {
        if (! this) {
            return a;
        }
        push();
        if (a->priority > this->priority) {
            split(this, index - 1, a->left, a->right);
            a->update_all();
            return a;
        } else {
            if (index <= left->count_behind_function() + 1)
                left->insert_by_index(a, index);
            else
                right->insert_by_index(a, index - left->count_behind_function() - 1);
            update_all();
            return this;
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
    Node* erase() {
        Node* x = this;
        Node* answer = merge(left, right);
        delete x;
        x = nullptr;
        return answer;
    }
    Node* erase(long long index) {
        Node* deleting_node = find(index);
        return deleting_node->erase();
    }

    Node* reverse(long long l, long long r) {
        Node* left = nullptr; Node* center = nullptr; Node* right = nullptr;
        split(this, l - 0.5, left, center);
        split(center, r - l + 0.5, center, right);
        center->is_reversed ^= true;
        left = merge(left, center);
        return merge(left, right);
    }
    long long sum(long long l, long long r) {
        Node* left = nullptr; Node* center = nullptr; Node* right = nullptr;
        split(this, l - 0.5, left, center);
        split(center, r - l + 0.5, center, right);
        return center->sum_behind_function();
    }
    Node* make_equal(long long l, long long r, long long new_key) {
        Node* left = nullptr; Node* center = nullptr; Node* right = nullptr;
        split(this, l - 0.5, left, center);
        split(center, r - l + 0.5, center, right);
        center->must_be_chanded = equal;
        center->make_equal_to = new_key;
        center->sum_behind = center->count_behind * new_key;
        left = merge(left, center);
        return merge(left, right);
    }
    Node* make_more(long long l, long long r, long long new_key) {
        Node* left = nullptr; Node* center = nullptr; Node* right = nullptr;
        split(this, l - 0.5, left, center);
        split(center, r - l + 0.5, center, right);
        center->must_be_chanded = more;
        center->make_more_by = new_key;
        center->sum_behind += center->count_behind * new_key;
        left = merge(left, center);
        return merge(left, right);
    }
};

class BinarySearchTree{
    Node* root;
public:
    BinarySearchTree(int size, int* array) {
        root = new Node(array[0]);
        for (int i = 1; i < size; ++i) {
            Node* ver = new Node(array[i]);
            root = root->insert_by_index(ver, i + 1);
        }
    }
    ~BinarySearchTree() {
        if (root)
            delete root;
    }
    void insert(long long key, long long index) {
        Node* new_node = new Node(key);
        root = root->insert_by_index(new_node, index + 1);
    }

};

Node* merge(Node* l, Node* r) {
    Node* my;
    if (!l || !r) {
        my = (l ? l : r);
        return my;
    }
    r->push();
    l->push();
    if (l->priority < r->priority) {
        r->left = merge(l, r->left);
        my = r;
    } else{
        l->right = merge(l->right, r);
        my = l;
    }
    my->update_all();
    return my;
}
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
        l->update_all();
    } else {
        split(my_vertex->left, input_i, l, my_vertex->left);
        r = my_vertex;
        r->update_all();
    }
    my_vertex->update_all();
}

int main() {
    int array[5] = {1,2,3,4,5};
    BinarySearchTree my_tree(5, std::move(array));

}
#include <iostream>
#include <vector>
#include <random>
#include <cstdio>
#include <memory>
#include <algorithm>

enum operation{
    no,
    more,
    equal,
    next_permutation,
    prev_permutation,
    rev
};


class Node {
    //left < right
    mutable Node* left;
    mutable Node* right;
    mutable int key;
    int priority;
    mutable int count_behind;
    mutable int sum_behind;
    mutable int leftmost;
    mutable int rightmost;
    mutable int ascending_suffix_len;
    mutable int descending_suffix_len;
    mutable bool is_reversed;
    mutable int make_more_by;
    mutable int make_equal_to;
    mutable operation must_be_chanded;

    int count_behind_func() const {
        return this ? count_behind : 0;
    }
    int sum_behind_func() const {
        return this ? sum_behind : 0;
    }
    int leftmost_func() const {
        return this ?  leftmost : 0;
    }
    int rightmost_func() const {
        return this ? rightmost : 0;
    }
    int ascending_func() const {
        return this ? ascending_suffix_len: 0;
    }
    int descending_func() const {
        return this ? descending_suffix_len : 0;
    }

    void update_count() const {
        count_behind = left->count_behind_func() + right->count_behind_func() + 1;
    }
    void update_sum() const {
        sum_behind = left->sum_behind_func() + right->sum_behind_func() + key;
    }
    void update_leftmost() const {
        if (left)
            leftmost = left->leftmost_func();
        else
            leftmost = key;
    }
    void update_rightmost() const {
        if (right)
            rightmost = right->rightmost_func();
        else
            rightmost = key;
    }
    void update_ascending() const {
        if (!right)
            ascending_suffix_len = 1;
        else {
            ascending_suffix_len = right->ascending_func();
            if (right->ascending_func() != right->count_behind_func())
                return;
            if (key < right->leftmost_func())
                ascending_suffix_len += 1;
        }
        if (left && key > left->rightmost_func())
            ascending_suffix_len += left->ascending_func();
    }
    void update_descending() const {
        if (!right)
            descending_suffix_len = 1;
        else {
            descending_suffix_len = right->descending_func();
            if (right->descending_func() != right->count_behind_func())
                return;
            if (key > right->leftmost_func())
                descending_suffix_len += 1;
        }
        if (left && key < left->rightmost_func())
            descending_suffix_len += left->descending_func();
    }
    void update_all() const {
        update_count();
        update_sum();
        update_leftmost();
        update_rightmost();
        update_ascending();
        update_descending();
    }
    void push() const {
        if (is_reversed) {
            std::swap(left, right);
            is_reversed = false;
            if (left)
                left->reverse();
            if (right)
                right->reverse();
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
            sum_behind += make_more_by * count_behind;
            leftmost += make_more_by;
            rightmost += make_more_by;
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
            rightmost = make_equal_to;
            leftmost = make_equal_to;
            descending_suffix_len = count_behind_func();
            ascending_suffix_len = count_behind_func();
            sum_behind = make_equal_to * count_behind;
            make_equal_to = key;
        }
        must_be_chanded = no;
    }

    int find_in_desc(int find_key) {
        if (!this)
            return 0;
        if (key > find_key) {
            return right->find_in_desc(find_key) + left->count_behind_func() + 1;
        }
        return left->find_in_desc(find_key);
    }
    int find_in_asc(int find_key) {
        if (!this)
            return 0;
        if (key < find_key) {
            return right->find_in_asc(find_key) + left->count_behind_func() + 1;
        }
        return left->find_in_asc(find_key);
    }

public:
    Node(int k) : left(nullptr), right(nullptr), key(k), priority(std::rand()), count_behind(1),
                  sum_behind(k), leftmost(k), rightmost(k), ascending_suffix_len(1), descending_suffix_len(1),
                  is_reversed(false), make_more_by(0), make_equal_to(key), must_be_chanded(no) {}
    ~Node() {
        if (left)
            delete left;
        if (right)
            delete right;
    }
    friend void split(const Node* my_vertex, double input_i, Node*& l, Node*& r);
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
            if (index <= left->count_behind_func() + 1)
                left = left->insert_by_index(a, index);
            else
                right = right->insert_by_index(a, index - left->count_behind_func() - 1);
            update_all();
            return this;
        }
    }
    Node* erase(int index) {
        push();
        int count_left = left->count_behind_func() + 1;
        if (index == count_left) {
            Node* x = this;
            Node* answer = merge(left, right);
            x->left = nullptr;
            x->right = nullptr;
            delete x;
            x = nullptr;
            return answer;
        }
        if (index < count_left) {
            left = left->erase(index);
            return this;
        }
        right = right->erase(index - left->count_behind_func());
        return this;
    }
    Node* reverse() {
        is_reversed ^= true;
    }

    int sum(int l, int r) const {
        Node* left_part = nullptr;
        Node* center = nullptr;
        Node* right_part = nullptr;
        split(this, l - 0.5, left_part, center);
        split(center, r - l + 0.5, center, right_part);
        int answer = center->sum_behind_func();
        left_part = merge(left_part, center);
        merge(left_part, right_part);
        return answer;
    }
    void make_equal(int new_key) {
        must_be_chanded = equal;
        make_equal_to = new_key;
    }
    void make_more(int new_key) {
        must_be_chanded = more;
        make_more_by = new_key;
    }

    Node* make_next_permutation() {
        if (!this)
            return this;
        if (descending_func() == count_behind_func()) {
            reverse();
            return this;
        }
        Node* left_part = nullptr;
        Node* center = nullptr;
        Node* right_part = nullptr;
        split(this, count_behind_func() - descending_suffix_len - 0.5, left_part, right_part);
        int first_node_key = left_part->rightmost;
        int center_count = right_part->find_in_desc(first_node_key);
        Node* first_node;
        split(left_part, left_part->count_behind_func() - 1, left_part, first_node);
        Node* second_node;
        split(right_part, center_count - 1.5, center, right_part);
        split(right_part, 0.5, second_node, right_part);
        right_part = merge(first_node, right_part);
        right_part = merge(center, right_part);
        right_part->reverse();
        right_part = merge(second_node, right_part);
        return merge(left_part, right_part);
    }
    Node* make_prev_permutation() {
        if (!this)
            return this;
        if (ascending_func() == count_behind_func()) {
            reverse();
            return this;
        }
        Node* left_part = nullptr;
        Node* center = nullptr;
        Node* right_part = nullptr;
        split(this, count_behind_func() - ascending_suffix_len - 0.5, left_part, right_part);
        int first_node_key = left_part->rightmost;
        int center_count = right_part->find_in_asc(first_node_key);
        Node* first_node;
        split(left_part, left_part->count_behind_func() - 1, left_part, first_node);
        Node* second_node;
        split(right_part, center_count - 1.5, center, right_part);
        split(right_part, 0.5, second_node, right_part);
        right_part = merge(first_node, right_part);
        right_part = merge(center, right_part);
        right_part->reverse();
        right_part = merge(second_node, right_part);
        return merge(left_part, right_part);
    }

    Node* split_and_do(int l, int r, operation x, int new_key = 0) {
        Node* left_part = nullptr;
        Node* center = nullptr;
        Node* right_part = nullptr;
        split(this, l - 0.5, left_part, center);
        split(center, r - l + 0.5, center, right_part);
        switch (x) {
            case more:
                center->make_more(new_key);
                break;
            case equal:
                center->make_equal(new_key);
                break;
            case next_permutation:
                center = center->make_next_permutation();
                break;
            case prev_permutation:
                center = center->make_prev_permutation();
                break;
            case rev:
                center = center->reverse();
                break;
        }
        left_part = merge(left_part, center);
        return merge(left_part, right_part);

    }
    void print() {
        push();
        if (left)
            left->print();
        std::cout << key << ' ';
        if (right)
            right->print();
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

    int sum(int l, int r) const {
        return root->sum(l, r);
    }
    void insert(int key, int index) {
        Node* new_node = new Node(key);
        root = root->insert_by_index(new_node, index + 1);
    }
    void erase(int index) {
        root = root->erase(index);
    }
    void make_equal(int l, int r, int new_key) {
        root = root->split_and_do(l, r, equal, new_key);
    }
    void make_more(int l, int r, int new_key) {
        root = root->split_and_do(l, r, more, new_key);
    }
    void make_next_permutation(int l, int r) {
        root = root->split_and_do(l, r, next_permutation);
    }
    void make_prev_permutation(int l, int r) {
        root = root->split_and_do(l, r, prev_permutation);
    }
    void print() {
        root->print();
        std::cout << '\n';
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
void split(const Node* my_vertex, double input_i, Node*& l, Node*& r) {
    if (!my_vertex) {
        l = nullptr;
        r = nullptr;
        return;
    }
    my_vertex->push();
    if (my_vertex->left->count_behind_func() < input_i ) {
        split(my_vertex->right, input_i - my_vertex->left->count_behind_func() - 1, my_vertex->right, r);
        l = const_cast<Node*>(my_vertex);
        l->update_all();
    } else {
        split(my_vertex->left, input_i, l, my_vertex->left);
        r = const_cast<Node*>(my_vertex);
        r->update_all();
    }
    my_vertex->update_all();
}

int main() {
    int size;
    std::cin >> size;
    int array[size];
    for (int i = 0; i < size; ++i) {
        std::cin >> array[i];
    }
    BinarySearchTree MyTree(size, array);
    int operations_amount;
    std::cin >> operations_amount;
    for (int i = 0; i < operations_amount; ++i) {
        int number;
        int l, r, x;
        std::cin >> number;
        switch (number){
            case 1:
                std::cin >> l >> r;
                std::cout << MyTree.sum(l, r);
                break;
            case 2:
                std::cin >> l >> r;
                MyTree.insert(l, r);
                break;
            case 3:
                std::cin >> x;
                MyTree.erase(x);
                break;
            case 4:
                std::cin >> x >> l >> r;
                MyTree.make_equal(l, r, x);
                break;
            case 5:
                std::cin >> x >> l >> r;
                MyTree.make_more(l, r, x);
                break;
            case 6:
                std::cin >> l >> r;
                MyTree.make_next_permutation(l, r);
                break;
            case 7:
                std::cin >>l >> r;
                MyTree.make_prev_permutation(l, r);
                break;
        }
        MyTree.print();
    }
    MyTree.print();
}
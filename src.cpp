#include <stdio.h>
#include <chrono>
#include <time.h>
#include <iostream>

struct Node
{
    int* keys;
    Node** childs;
    int count;
    bool leaf;
};

struct B_Tree
{
    Node* root;
    int _degree;

    void init_tree(int degree) {
        _degree = degree;
        root = create_node();
        root->leaf = true;
        root->count = 0;
    }

    Node* create_node() {
        Node* new_node = new Node;
        new_node->keys = new int[2 * _degree - 1];
        new_node->childs = new Node * [2 * _degree];
        return new_node;
    }

    void insert(int key) {
        if (root->count == 2 * _degree - 1)
            insert_step1(key); /* Step 1: root가 full인 경우 */
        else
            insert_step2(root, key); /* Step 2: 빈 공간이 있는 경우 */
    }

    void insert_step1(int key) {
        Node* old_root = root;
        Node* new_root = create_node();
        root = new_root;
        new_root->leaf = false;
        new_root->count = 0;
        new_root->childs[0] = old_root;
        /* new root의 child가 full인 상태이므로 split한다. */
        split(new_root, 0);
        insert_step2(new_root, key);
    }

    void insert_step2(Node* s, int k) {

        /*
        s는 subtree의 root node.
        s가 leaf node인 경우 그냥 insert
        */
        if (s->leaf)
        {
            int i = s->count - 1;
            for (; i >= 0 && k < s->keys[i]; i--)
                s->keys[i + 1] = s->keys[i];
            s->keys[i + 1] = k;
            s->count++;
        }
        else
        {
            int i = s->count;
            for (; i >= 1 && k < s->keys[i - 1]; i--) {}
            /*
            s가 full인 경우 split하고
            다음 subtree의 root node를 찾은 후 step2 반복
            */
            if ((s->childs[i])->count == 2 * _degree - 1)
            {
                split(s, i);
                if (k > s->keys[i]) i++;
            }
            insert_step2(s->childs[i], k);
        }
    }

    /* 1부터 n까지 n개의 키를 insert */
    void insert_many(int n) {
        for (int i = 1; i <= n; i++)
            insert(i);
    }

    /* x 노드의 i번 child 노드를 split한다. */
    void split(Node* p, int i) {
        Node* right = create_node();
        Node* left = p->childs[i];
        right->leaf = left->leaf;
        right->count = _degree - 1;
        left->count = _degree - 1;
        p->childs[i + 1] = right;

        /* right에 윈래 키들의 오른쪽 절반정도 복사 */
        for (int j = 0; j < _degree - 1; j++)
            right->keys[j] = left->keys[j + _degree];

        /* child가 있는 경우 child도 복사 */
        if (left->leaf == false)
            for (int j = 0; j < _degree; j++)
                right->childs[j] = left->childs[j + _degree];

        for (int j = p->count; j > i; j--)
            p->childs[j + 1] = p->childs[j];

        for (int j = p->count - 1; j > i - 1; j--)
            p->keys[j + 1] = p->keys[j];

        /* 중간값 parent로 올려보냄 */
        p->keys[i] = left->keys[_degree - 1];
        p->count = p->count + 1;
    }
};

void test(int degree, int size) {
    clock_t time_start = clock();
    for (int i = 0; i < 10000; i++)
    {
        B_Tree* tree = new B_Tree;
        tree->init_tree(degree);
        tree->insert_many(size);
    }
    clock_t time_end = clock();
    std::cout << "degree :" << degree << ", ";
    std::cout << "time :" << (time_end - time_start) << std::endl;
}

int main()
{
    test(5, 1000);
    test(7, 1000);
    test(9, 1000);
}
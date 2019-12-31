#pragma once
struct BinaryTree;
struct node;
typedef struct BinaryTree* BT;
typedef struct node* NodePtr;

void init_node(NodePtr *n, int data);
void set_data(NodePtr n, int data);
int get_data(NodePtr n);
void set_left(NodePtr p, NodePtr c);
NodePtr get_left(NodePtr n);
void set_right(NodePtr p, NodePtr c);
NodePtr get_right(NodePtr n);
int get_depth(BT t);
int nodes_count(BT t);
int leaves_count(BT t);
int nodes_in_level(BT t, int level);
NodePtr search_value(BT t, int value);
void insert_value(BT t, int value);
void delete_value(BT t, int value);
void preorder_traverse(BT t);
void inorder_traverse(BT t);
void postorder_traverse(BT t);
void init_tree(BT* tree);
void destory_tree(NodePtr root);
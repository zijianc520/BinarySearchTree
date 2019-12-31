#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "main.h"

struct BinaryTree {
	NodePtr root;
};

struct node {
	int data;
	NodePtr left;
	NodePtr right;
};

void init_node(NodePtr *n, int data)
{
	(*n) = malloc(sizeof(struct node));
	(*n)->data = data;
	(*n)->left = NULL;
	(*n)->right = NULL;
}

void set_data(NodePtr n, int data)
{
	n->data = data;
}

int get_data(NodePtr n)
{
	return n->data;
}

void set_left(NodePtr p, NodePtr c)
{
	p->left = c;
}

NodePtr get_left(NodePtr n)
{
	return n->left;
}

void set_right(NodePtr p, NodePtr c)
{
	p->right = c;
}

NodePtr get_right(NodePtr n)
{
	return n->right;
}

int get_depth(BT t)
{
	if (!t->root) {
		return 0;
	}
	BT sub_L, sub_R;
	init_tree(&sub_L), init_tree(&sub_R);
	sub_L->root = get_left(t->root);
	sub_R->root = get_right(t->root);
	int left = get_depth(sub_L);  // left sub tree
	int right = get_depth(sub_R); // right sub tree
	if (left > right) {
		return (left + 1);
	}
	else {
		return (right + 1);
	}
}

int nodes_count(BT t)
{
	if (!t->root) {
		return 0;
	}
	else {
		BT sub_L, sub_R;
		init_tree(&sub_L), init_tree(&sub_R);
		sub_L->root = get_left(t->root);
		sub_R->root = get_right(t->root);
		return (nodes_count(sub_L) + nodes_count(sub_R) + 1);
	}
}

int leaves_count(BT t)
{
	// if tree is empty
	if (!t->root) return 0;

	// if it does not have any child
	if (!get_left(t->root) && !get_right(t->root)) return 1;

	// if it has child/children
	BT sub_L, sub_R;
	init_tree(&sub_L), init_tree(&sub_R);
	sub_L->root = get_left(t->root);
	sub_R->root = get_right(t->root);
	return (leaves_count(sub_L) + leaves_count(sub_R));
}

int nodes_in_level(BT t, int level)
{
	if (!t->root || !level) return 0;
	if (level == 1) return 1;

	BT sub_L, sub_R;
	init_tree(&sub_L),init_tree(&sub_R);
	sub_L->root = get_left(t->root), sub_R->root = get_right(t->root);
	return (nodes_in_level(sub_L, level - 1) + nodes_in_level(sub_R, level - 1));
}

NodePtr search_value(BT t, int value)
{
	if (!t->root) return NULL;
	if (get_data(t->root) == value) return t->root;

	BT sub_tree;
	init_tree(&sub_tree);
	if (value > get_data(t->root)) {
		sub_tree->root = get_right(t->root);
	}
	else {
		BT sub_tree;
		init_tree(&sub_tree);
		sub_tree->root = get_left(t->root);
	}
	return search_value(sub_tree, value);
}

void insert_value(BT t, int value)
{
	//if (search_value(t, value)) return; //value checking needs to be done before insertion
	NodePtr new_node;
	init_node(&new_node, value);
	if (!t->root) {
		t->root = new_node;
	}
	else if (get_data(t->root) > value && !get_left(t->root)) {
		set_left(t->root, new_node);
	}
	else if (get_data(t->root) < value && !get_right(t->root)) {
		set_right(t->root, new_node);
	}
	else {
		/**/
		BT sub_tree;
		init_tree(&sub_tree);
		if (get_data(t->root) > value) {
			sub_tree->root = get_left(t->root);
		}
		else {
			sub_tree->root = get_right(t->root);
		}
		insert_value(sub_tree, value);
		/**/
	}
}

void delete_value(BT t, int value)
{
	//if (!search_value(t, value)) return; //value checking needs to be done before deleting
	if (get_data(t->root) == value) {
		//test
		//printf("TEST :: Target found \n");

		// if it does not hava any child, remove node directly
		if (!get_left(t->root) && !get_right(t->root)) {
			free(t->root);
			t->root = NULL;
			return;
		}

		/* if it only has left child
		 * link its left child and remove node
		 */
		if (get_left(t->root) && !get_right(t->root)) {
			NodePtr tmp = t->root;
			t->root = get_left(t->root);
			free(tmp);
			tmp = NULL;
			return;
		}

		/* if it only has left child
		 * link its right child and remove node
		 */
		if (!get_left(t->root) && get_right(t->root)) {
			NodePtr tmp = t->root;
			t->root = get_right(t->root);
			free(tmp);
			tmp = NULL;
			return;
		}

		/* if it has left & right child
		 * find the smallest node from its right sub tree (a node without any child)
		 * replace deleted node by found node
		 */
		NodePtr tmp = t->root, parent = t->root, child = get_right(t->root), left_c = get_left(t->root), right_c = get_right(t->root);
		while (get_left(child) || get_right(child)) {
			//test
			//printf("TEST :: Finding Replacement Node -> Parent is : %d, Child is %d \n", get_data(parent), get_data(child));

			if (get_left(child)) {
				parent = child;
				child = get_left(child);
			}
			else {
				parent = child;
				child = get_right(child);
			}
		}
		if (get_left(parent) == child) {
			set_left(parent, NULL);
		}
		else {
			set_right(parent, NULL);
		}
		//test
		//printf("TEST :: Replacement Node is %d, its parent is %d, Deleted Node is:%d \n", get_data(child), get_data(parent), get_data(t->root));

		t->root = child;
		set_left(t->root, left_c), set_right(t->root, right_c);
		free(tmp);
	}
	else if (get_data(t->root) > value) {
		//test
		//printf("TEST :: Current Value is bigger than target \n");

		BT sub_L;
		init_tree(&sub_L);
		sub_L->root = get_left(t->root);
		delete_value(sub_L, value);
		set_left(t->root, sub_L->root);
	}
	else {
		//test
		//printf("TEST :: Current Value is smaller than target \n");

		BT sub_R;
		init_tree(&sub_R);
		sub_R->root = get_right(t->root);
		delete_value(sub_R, value);
		set_right(t->root, sub_R->root);
	}
}

void preorder_traverse(BT t)
{
	if (!t->root) return;
	BT sub_L, sub_R;
	init_tree(&sub_L), init_tree(&sub_R);
	sub_L->root = get_left(t->root);
	sub_R->root = get_right(t->root);
	
	printf("%d, ", get_data(t->root));
	preorder_traverse(sub_L);
	preorder_traverse(sub_R);
}

void inorder_traverse(BT t)
{
	if (!t->root) return;
	BT sub_L, sub_R;
	init_tree(&sub_L), init_tree(&sub_R);
	sub_L->root = get_left(t->root);
	sub_R->root = get_right(t->root);
	
	inorder_traverse(sub_L);
	printf("%d, ", get_data(t->root));
	inorder_traverse(sub_R);
}

void postorder_traverse(BT t)
{
	if (!t->root) return;
	BT sub_L, sub_R;
	init_tree(&sub_L), init_tree(&sub_R);
	sub_L->root = get_left(t->root);
	sub_R->root = get_right(t->root);
	
	postorder_traverse(sub_L);
	postorder_traverse(sub_R);
	printf("%d, ", get_data(t->root));
}

void init_tree(BT *tree)
{
	(*tree) = malloc(sizeof(struct BinaryTree));
	(*tree)->root = NULL;
}

void destory_tree(NodePtr root)
{
	if (root) {
		destory_tree(get_left(root));
		destory_tree(get_right(root));
		free(root);
		root = NULL;
	}
}

void main()
{
	int source[] = {5,4,7,1,10,3,9,12,16,2,13,8};
	BT tree;
	init_tree(&tree);
	for (int i = 0; i < sizeof(source) / 4; i++) {
		// check existence before insertion
		if (!search_value(tree, source[i])) {
			insert_value(tree, source[i]);
		}
	}

	preorder_traverse(tree);
	printf("\n");
	printf("Depth of tree is: %d, Number of nodes is: %d, Number of leaves is: %d \n", get_depth(tree), nodes_count(tree), leaves_count(tree));
	printf("Num of nodes in level::%d is %d", 5, nodes_in_level(tree, 5));
	printf("\n\n");

	if (search_value(tree, 10)) delete_value(tree, 10); // check existence before deleting
	inorder_traverse(tree);
	printf("\n");
	printf("Depth of tree is: %d, Number of nodes is: %d, Number of leaves is: %d \n", get_depth(tree), nodes_count(tree), leaves_count(tree));
	printf("Num of nodes in level::%d is %d", 6, nodes_in_level(tree, 6));
	printf("\n\n");

	//test
	//int tar = 10;
	//printf("%p -> data == %d \n ", search_value(tree, tar), tar);

	if (!search_value(tree, 10)) insert_value(tree, 10); // check existence before insertion
	postorder_traverse(tree);
	printf("\n");
	printf("Depth of tree is: %d, Number of nodes is: %d, Number of leaves is: %d \n", get_depth(tree), nodes_count(tree), leaves_count(tree));
	printf("Num of nodes in level::%d is %d", 5, nodes_in_level(tree, 5));
	printf("\n\n");

	destory_tree(tree->root);
}
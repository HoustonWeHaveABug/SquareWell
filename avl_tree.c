#include <stdio.h>
#include <stdlib.h>
#define AVL_TREE
#include "avl_tree.h"

node_t *new_node(void *);
int get_node_balance(node_t *);
node_t *right_rotate(node_t *);
node_t *left_rotate(node_t *);
void set_node_height(node_t *);

node_t *insert_node(node_t *node, void *key) {
int balance, compare;
	if (!node) {
		return new_node(key);
	}
	compare = compare_keys(key, node->key);
	if (compare < 0) {
		node->left = insert_node(node->left, key);
	}
	else if (compare > 0) {
		node->right = insert_node(node->right, key);
	}
	else {
		return node;
	}
	set_node_height(node);
	balance = get_node_balance(node);
	if (balance > 1) {
		compare = compare_keys(key, node->left->key);
		if (compare < 0) {
			return right_rotate(node);
		}
		else if (compare > 0) {
			node->left = left_rotate(node->left);
			return right_rotate(node);
		}
		else {
			return node;
		}
	}
	else if (balance < -1) {
		compare = compare_keys(key, node->right->key);
		if (compare > 0) {
			return left_rotate(node);
		}
		else if (compare < 0) {
			node->right = right_rotate(node->right);
			return left_rotate(node);
		}
		else {
			return node;
		}
	}
	else {
		return node;
	}
}

node_t *delete_node(node_t *node, void *key) {
int compare, balance, balance_child;
node_t *tmp;
	if (!node) {
		return node;
	}
	compare = compare_keys(key, node->key);
	if (compare < 0) {
		node->left = delete_node(node->left, key);
	}
	else if (compare > 0) {
		node->right = delete_node(node->right, key);
	}
	else {
		if (node->left) {
			if (node->right) {
				tmp = get_node_min(node->right);
				node->key = tmp->key;
				node->right = delete_node(node->right, tmp->key);
			}
			else {
				tmp = node->left;
				*node = *tmp;
				free(tmp);
			}
		}
		else {
			if (node->right) {
				tmp = node->right;
				*node = *tmp;
			}
			else {
				tmp = node;
				node = NULL;
			}
			free(tmp);
		}
	}
	if (!node) {
		return node;
	}
	set_node_height(node);
	balance = get_node_balance(node);
	if (balance > 1) {
		balance_child = get_node_balance(node->left);
		if (balance_child >= 0) {
			return right_rotate(node);
		}
		else {
			node->left = left_rotate(node->left);
			return right_rotate(node);
		}
	}
	else if (balance < -1) {
		balance_child = get_node_balance(node->right);
		if (balance_child <= 0) {
			return left_rotate(node);
		}
		else {
			node->right = right_rotate(node->right);
			return left_rotate(node);
		}
	}
	else {
		return node;
	}
}

node_t *get_node(node_t *node, void *key) {
int compare;
	if (!node) {
		return node;
	}
	compare = compare_keys(key, node->key);
	if (compare < 0) {
		return get_node(node->left, key);
	}
	else if (compare > 0) {
		return get_node(node->right, key);
	}
	else {
		return node;
	}
}

node_t *get_node_min(node_t *node) {
	while (node->left) {
		node = node->left;
	}
	return node;
}

node_t *get_node_max(node_t *node) {
	while (node->right) {
		node = node->right;
	}
	return node;
}

void free_node(node_t *node) {
	if (node) {
		free_node(node->left);
		free_node(node->right);
		free(node);
	}
}

node_t *new_node(void *key) {
node_t *node = malloc(sizeof(node_t));
	if (node) {
		node->key = key;
		node->height = 1;
		node->left = NULL;
		node->right = NULL;
	}
	else {
		fprintf(stderr, "Could not allocate memory for new node\n");
		fflush(stderr);
	}
	return node;
}

int get_node_balance(node_t *node) {
	if (node) {
		if (node->left) {
			if (node->right) {
				return node->left->height-node->right->height;
			}
			else {
				return node->left->height;
			}
		}
		else {
			if (node->right) {
				return -node->right->height;
			}
			else {
				return 0;
			}
		}
	}
	else {
		return 0;
	}
}

node_t *right_rotate(node_t *node) {
node_t *left = node->left, *left_right = left->right;
	left->right = node;
	node->left = left_right;
	set_node_height(node);
	set_node_height(left);
	return left;
}

node_t *left_rotate(node_t *node) {
node_t *right = node->right, *right_left = right->left;
	right->left = node;
	node->right = right_left;
	set_node_height(node);
	set_node_height(right);
	return right;
}

void set_node_height(node_t *node) {
	if (node->left) {
		if (node->right) {
			if (node->left->height < node->right->height) {
				node->height = node->right->height+1;
			}
			else {
				node->height = node->left->height+1;
			}
		}
		else {
			node->height = node->left->height+1;
		}
	}
	else {
		if (node->right) {
			node->height = node->right->height+1;
		}
		else {
			node->height = 1;
		}
	}
}

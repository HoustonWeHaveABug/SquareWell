#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

typedef struct {
	int height;
	int row;
	int column;
	int inserted;
}
cell_t;

typedef struct node_s node_t;

struct node_s {
	cell_t *cell;
	int height;
	node_t *left;
	node_t *right;
};

int read_cell(cell_t *, int, int);
int insert_cell(cell_t *);
node_t *insert_node(node_t *, cell_t *);
node_t *new_node(cell_t *);
node_t *delete_node(node_t *, cell_t *);
node_t *get_node_min(node_t *);
int get_node_balance(node_t *);
node_t *right_rotate(node_t *);
node_t *left_rotate(node_t *);
void set_node_height(node_t *);
void free_node(node_t *);

int cells_n, *used;
cell_t *source_cell;
node_t *nodes;

int main(void) {
	int rows_n, columns_n, row, column, target_height, cells_idx, source_height, source_size;
	int64_t cube_meters;
	cell_t *cells, *target_cell, *min_cell;
	if (scanf("%d%d", &rows_n, &columns_n) != 2 || rows_n < 1 || columns_n < 1) {
		fprintf(stderr, "Invalid square well size\n");
		fflush(stderr);
		return EXIT_FAILURE;
	}
	cells_n = rows_n*columns_n;
	cells = malloc(sizeof(cell_t)*(size_t)cells_n);
	if (!cells) {
		fprintf(stderr, "Could not allocate memory for cells\n");
		fflush(stderr);
		return EXIT_FAILURE;
	}
	used = calloc((size_t)cells_n, sizeof(int));
	if (!used) {
		fprintf(stderr, "Could not allocate memory for used\n");
		fflush(stderr);
		free(cells);
		return EXIT_FAILURE;
	}
	for (row = 0; row < rows_n; row++) {
		for (column = 0; column < columns_n; column++) {
			if (!read_cell(cells+row*columns_n+column, row, column)) {
				free(used);
				free(cells);
				return EXIT_FAILURE;
			}
		}
	}
	if (scanf("%d", &target_height) != 1 || target_height < 1 || target_height > cells_n) {
		fprintf(stderr, "Invalid target height\n");
		fflush(stderr);
		free(used);
		free(cells);
		return EXIT_FAILURE;
	}
	for (cells_idx = 0; cells_idx < cells_n && cells[cells_idx].height != target_height; cells_idx++);
	target_cell = cells+cells_idx;
	nodes = NULL;
	if (!insert_cell(source_cell)) {
		free(used);
		free(cells);
		return EXIT_FAILURE;
	}
	cube_meters = 0;
	source_height = 1;
	source_size = 0;
	do {
		min_cell = get_node_min(nodes)->cell;
		if (min_cell->height < source_height) {
			cube_meters += source_height-min_cell->height;
		}
		else {
			cube_meters += (min_cell->height-source_height)*source_size;
			source_height = min_cell->height;
		}
		source_size++;
		if (min_cell->column > 0 && !insert_cell(min_cell-1)) {
			free_node(nodes);
			free(used);
			free(cells);
			return EXIT_FAILURE;
		}
		if (min_cell->row > 0 && min_cell->column > 0 && !insert_cell(min_cell-columns_n-1)) {
			free_node(nodes);
			free(used);
			free(cells);
			return EXIT_FAILURE;
		}
		if (min_cell->row > 0 && !insert_cell(min_cell-columns_n)) {
			free_node(nodes);
			free(used);
			free(cells);
			return EXIT_FAILURE;
		}
		if (min_cell->row > 0 && min_cell->column < columns_n-1 && !insert_cell(min_cell-columns_n+1)) {
			free_node(nodes);
			free(used);
			free(cells);
			return EXIT_FAILURE;
		}
		if (min_cell->column < columns_n-1 && !insert_cell(min_cell+1)) {
			free_node(nodes);
			free(used);
			free(cells);
			return EXIT_FAILURE;
		}
		if (min_cell->row < rows_n-1 && min_cell->column < columns_n-1 && !insert_cell(min_cell+columns_n+1)) {
			free_node(nodes);
			free(used);
			free(cells);
			return EXIT_FAILURE;
		}
		if (min_cell->row < rows_n-1 && !insert_cell(min_cell+columns_n)) {
			free_node(nodes);
			free(used);
			free(cells);
			return EXIT_FAILURE;
		}
		if (min_cell->row < rows_n-1 && min_cell->column > 0 && !insert_cell(min_cell+columns_n-1)) {
			free_node(nodes);
			free(used);
			free(cells);
			return EXIT_FAILURE;
		}
		nodes = delete_node(nodes, min_cell);
	}
	while (min_cell != target_cell);
	if (target_cell->height < source_height) {
		cube_meters -= source_height-target_cell->height-1;
	}
	else {
		cube_meters += source_size;
	}
	printf("%" PRIi64 "\n", cube_meters);
	fflush(stdout);
	free_node(nodes);
	free(used);
	free(cells);
	return EXIT_SUCCESS;
}

int read_cell(cell_t *cell, int row, int column) {
	if (scanf("%d", &cell->height) != 1 || cell->height < 1 || cell->height > cells_n || used[cell->height-1]) {
		fprintf(stderr, "Invalid cell height\n");
		fflush(stderr);
		return 0;
	}
	cell->row = row;
	cell->column = column;
	cell->inserted = 0;
	if (cell->height == 1) {
		source_cell = cell;
	}
	used[cell->height-1] = 1;
	return 1;
}

int insert_cell(cell_t *cell) {
	if (!cell->inserted) {
		node_t *nodes_tmp = insert_node(nodes, cell);
		if (!nodes_tmp) {
			return 0;
		}
		nodes = nodes_tmp;
		cell->inserted = 1;
	}
	return 1;
}

node_t *insert_node(node_t *node, cell_t *cell) {
int balance;
	if (!node) {
		return new_node(cell);
	}
	if (cell->height < node->cell->height) {
		node->left = insert_node(node->left, cell);
	}
	else {
		node->right = insert_node(node->right, cell);
	}
	set_node_height(node);
	balance = get_node_balance(node);
	if (balance > 1) {
		if (cell->height < node->left->cell->height) {
			return right_rotate(node);
		}
		else if (cell->height > node->left->cell->height) {
			node->left = left_rotate(node->left);
			return right_rotate(node);
		}
		else {
			return node;
		}
	}
	else if (balance < -1) {
		if (cell->height > node->right->cell->height) {
			return left_rotate(node);
		}
		else if (cell->height < node->right->cell->height) {
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

node_t *new_node(cell_t *cell) {
node_t *node = malloc(sizeof(node_t));
	if (node) {
		node->cell = cell;
		node->height = 1;
		node->left = NULL;
		node->right = NULL;
	}
	else {
		fprintf(stderr, "Could not allocate memory for new node\n");
	}
	return node;
}

node_t *delete_node(node_t *node, cell_t *cell) {
int balance, balance_child;
node_t *tmp;
	if (!node) {
		return node;
	}
	if (cell->height < node->cell->height) {
		node->left = delete_node(node->left, cell);
	}
	else if (cell->height > node->cell->height) {
		node->right = delete_node(node->right, cell);
	}
	else {
		if (node->left) {
			if (node->right) {
				tmp = get_node_min(node->right);
				node->cell = tmp->cell;
				node->right = delete_node(node->right, tmp->cell);
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

node_t *get_node_min(node_t *node) {
	while (node->left) {
		node = node->left;
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

void free_node(node_t *node) {
	if (node) {
		free_node(node->left);
		free_node(node->right);
		free(node);
	}
}

#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include "avl_tree.h"

typedef struct {
	int height;
	int row;
	int column;
	int inserted;
}
cell_t;

int read_cell(cell_t *, int, int);
int compare_cells(void *, void *);
int insert_cell(cell_t *);

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
	compare_keys = compare_cells;
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
		min_cell = (cell_t *)get_node_min(nodes)->key;
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
		if (min_cell->row > 0 && !insert_cell(min_cell-columns_n)) {
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
		if (min_cell->row < rows_n-1 && !insert_cell(min_cell+columns_n)) {
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

int compare_cells(void *a, void *b) {
	cell_t *cell_a = (cell_t *)a, *cell_b = (cell_t *)b;
	return cell_a->height-cell_b->height;
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

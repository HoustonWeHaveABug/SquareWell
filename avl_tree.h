typedef struct node_s node_t;

struct node_s {
	void *key;
	int height;
	node_t *left;
	node_t *right;
};

node_t *insert_node(node_t *, void *);
node_t *delete_node(node_t *, void *);
node_t *get_node(node_t *, void *);
node_t *get_node_min(node_t *);
node_t *get_node_max(node_t *);
void free_node(node_t *);

#ifdef AVL_TREE
	int (*compare_keys)(void *, void *);
#else
	extern int (*compare_keys)(void *, void *);
#endif

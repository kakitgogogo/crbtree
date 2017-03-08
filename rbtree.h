#ifndef RBTREE_H
#define	RBTREE_H

#define offsetof(type, member) ((unsigned int) &((type *)0)->member)

#define container_of(ptr, type, member) ({ \
        const typeof( ((type *)0)->member ) *__mptr = (ptr); \
        (type *)( (char *)__mptr - offsetof(type, member) );})

#define RED   0
#define BLACK 1
#define HEADER_KEY 	0
#define key_type 	unsigned int
#define compare(x, y) (x < y)

struct node
{
	int color;
	key_type key;
	struct node* left;
	struct node* right;
	struct node* parent;
};

struct rbtree
{
	int count;
	struct node header;
	struct node *root;
	struct node *leftmost;
	struct node *rightmost;
};

typedef void (*rbtree_handler)(struct node*);

void rbtree_init(struct rbtree* tree);
void rbtree_release(struct rbtree* tree);
void rbtree_insert(struct rbtree* tree, struct node *z);
void rbtree_inorder(struct rbtree* tree, rbtree_handler func);
struct node* rbtree_find(struct rbtree* tree, key_type key);
void rbtree_erase(struct rbtree* tree, struct node* z);
void rbtree_erase_by_key(struct rbtree* tree, key_type key);

#endif
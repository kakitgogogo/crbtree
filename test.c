#include <stdio.h>
#include "rbtree.h"

struct item
{
	int val;
	struct node node;
};

struct item2
{
	char val;
	struct node node;
};

void handler(struct node* n)
{
	printf("%d ", container_of(n, struct item, node)->node.key);
}

int main()
{
	int i;
	struct rbtree tree;
	rbtree_init(&tree);

	struct item items[5];
	struct item2 items2[5];
	int a[10] = {4, 6, 9, 13, 2, 7, 1, 8, 23, 12};
	for(i = 0; i < 5; ++i)
	{
		items[i].val = i;
		items[i].node.key = a[i];
		rbtree_insert(&tree, &items[i].node);
	}
	for(i = 0; i < 5; ++i)
	{
		items2[i].val = i;
		items2[i].node.key = a[i+5];
		rbtree_insert(&tree, &items2[i].node);
	}

	rbtree_inorder(&tree, handler);
	printf("\n========================\n");

	for(i = 0; i < 5; ++i)
	{
		rbtree_erase(&tree, &items[i].node);
		rbtree_inorder(&tree, handler);
		printf("\n========================\n");
	}
}

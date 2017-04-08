#include <stdio.h>
#include "rbtree.h"

void rbtree_init(struct rbtree* tree)
{
	tree->count=0;
	
	tree->header.color = RED;
	tree->header.key = HEADER_KEY;
	tree->header.left = NULL;
	tree->header.right = NULL;
	tree->header.parent = NULL;
	
	tree->root = NULL;
	
	tree->leftmost = &tree->header;
	tree->rightmost = &tree->header;
}

static void _rbtree_release(struct node* n)
{
	if(n == NULL) return;
	_rbtree_release(n->left);
	_rbtree_release(n->right);

	n->left = NULL;
	n->right = NULL;
	n->parent = NULL;
}

void rbtree_release(struct rbtree* tree)
{
	_rbtree_release(tree->root);
	tree->root = NULL;
	tree->leftmost = NULL;
	tree->rightmost = NULL;
}

static void rbtree_rotate_left(struct rbtree* tree, struct node* x)
{
	struct node *y = x->right;
	x->right = y->left;
	if(y->left != NULL) y->left->parent = x;
	y->parent = x->parent;
	
	if(x == tree->root)
		tree->root = y;
	else if(x == x->parent->left)
		x->parent->left = y;
	else
		x->parent->right = y;
	
	y->left = x;
	x->parent = y;
}

static void rbtree_rotate_right(struct rbtree* tree, struct node* x)
{
	struct node *y = x->left;
	x->left = y->right;
	if(y->right != NULL) y->right->parent = x;
	y->parent = x->parent;
	
	if(x == tree->root)
		tree->root = y;
	else if(x == x->parent->right)
		x->parent->right = y;
	else
		x->parent->left = y;
	
	y->right = x;
	x->parent = y;
}

static void rbtree_balance(struct rbtree* tree, struct node* x)
{
	x->color = RED;
	
	while(x != tree->root && x->parent->color == RED)
	{
		if(x->parent == x->parent->parent->left)
		{
			struct node *y = x->parent->parent->right;
			if(y && y->color == RED)
			{
				x->parent->color = BLACK;
				y->color = BLACK;
				x->parent->parent->color = RED;
				x = x->parent->parent;
			}
			else 
			{
				if(x == x->parent->right)
				{
					x = x->parent;
					rbtree_rotate_left(tree, x);
				}
				x->parent->color = BLACK;
				x->parent->parent->color = RED;
				rbtree_rotate_right(tree, x->parent->parent);
			}
		}
		else
		{
			struct node *y = x->parent->parent->left;
			if(y && y->color == RED)
			{
				x->parent->color = BLACK;
				y->color = BLACK;
				x->parent->parent->color = RED;
				x = x->parent->parent;
			}
			else 
			{
				if(x == x->parent->left)
				{
					x = x->parent;
					rbtree_rotate_right(tree, x);
				}
						
				x->parent->color = BLACK;
				x->parent->parent->color = RED;
				rbtree_rotate_left(tree, x->parent->parent);
			}
		}			
	}
	tree->root->color = BLACK;
}

void rbtree_insert(struct rbtree* tree, struct node *z)
{
	struct node *y = &tree->header;
	struct node *x = tree->root;
	while(x != NULL)
	{
		y = x;
		x = compare(z->key, x->key) ? x->left : x->right;
	}
	z->color = RED;
	z->left = NULL;
	z->right = NULL;
	if(y == &tree->header)
	{
		tree->root = z;
		tree->leftmost = z;
		tree->rightmost = z;
		y->parent = z;
		y->left = z;
		y->right = z;
	}
	else if(compare(z->key, y->key))
	{
		y->left = z;
		if(y == tree->leftmost) tree->leftmost = z;
	}
	else
	{
		y->right = z;
		if(y == tree->rightmost) tree->rightmost = z;
	}
	z->parent = y;
	
	rbtree_balance(tree, z);
	++tree->count;
}


static void _rbtree_inorder(struct node* n, rbtree_handler func)
{
	if(n == NULL) return;
	_rbtree_inorder(n->left, func);
	func(n);
	_rbtree_inorder(n->right, func);
}

void rbtree_inorder(struct rbtree* tree, rbtree_handler func)
{
	_rbtree_inorder(tree->root, func);
}

struct node* rbtree_find(struct rbtree* tree, key_type key)
{
	struct node *x = tree->root;
	while(x != NULL)
	{
		if(x->key == key) return x;
		else if(compare(key,x->key)) x = x->left;
		else x = x->right;
	}
	return NULL;
}

void rbtree_modify(struct rbtree* tree, key_type key, rbtree_handler func)
{
	struct node *z = rbtree_find(tree, key);
	if(z==NULL) return;
	func(z);
}

static void swap_color(int* a, int* b)
{
	int t = *a;
	*a = *b;
	*b = t;
}

struct node* rbtree_minimum(struct node* x)
{
	while(x->left != NULL) x = x->left;
	return x;
}

struct node* rbtree_maximum(struct node* x)
{
	while(x->right!=NULL) x=x->left;
	return x;
}

void rbtree_erase(struct rbtree* tree, struct node* z)
{
	if(z == NULL) return;
	struct node *y = z;
	struct node *x = NULL;
	struct node *x_parent = NULL;

	--tree->count;

	if(y->left == NULL) x = y->right;
	else if(y->right == NULL) x = y->left;
	else
	{
		y = y->right;
		while(y->left != NULL) y = y->left;
		x = y->right;
	}

	if(y != z)
	{
		z->left->parent = y;
		y->left = z->left;
		if(y != z->right)
		{
			x_parent = y->parent;
			if(x != NULL) x->parent = y->parent;
			y->parent->left = x;
			y->right = z->right;
			y->right = z->right;
			z->right->parent = y;
		}
		else x_parent = y;
		if(z == tree->root) tree->root = y;
		else if(z->parent->left == z) z->parent->left = y;
		else z->parent->right = y;
		y->parent = z->parent;
		swap_color(&y->color, &z->color);
		y = z;
	}
	else
	{
		x_parent = y->parent;
		if(x != NULL) x->parent = y->parent;
		if(z == tree->root) tree->root = x;
		else if(z->parent->left == z) z->parent->left = x;
		else z->parent->right = x;
		if(tree->leftmost == z)
		{
			if(z->right == NULL) tree->leftmost = z->parent;
			else tree->leftmost = rbtree_minimum(x);
		}
		else if(tree->rightmost == z)
		{
			if(z->left == NULL) tree->rightmost = z->parent;
			else tree->rightmost = rbtree_maximum(x);
		}
	}

	if(y->color != RED)
	{
		while(x != tree->root && (x == NULL || x->color == BLACK))
		{
			if(x == x_parent->left)
			{
				struct node *w = x_parent->right;
				if(w->color == RED)
				{
					w->color = BLACK;
					x_parent->color = RED;
					rbtree_rotate_left(tree, x_parent);
					w = x_parent->right;
				}

				if((w->left == NULL || w->left->color == BLACK) &&
					(w->right == NULL || w->right->color == BLACK))
				{
					w->color = RED;
					x = x_parent;
					x_parent = x_parent->parent;
				}
				else
				{
					if(w->right == 0 || w->right->color == BLACK)
					{
						if(w->left != NULL) w->left->color = BLACK;
						w->color = RED;
						rbtree_rotate_right(tree, w);
						w = x_parent->right;
					}

					w->color = x_parent->color;
					x_parent->color = BLACK;
					if(w->right) w->right->color = BLACK;
					rbtree_rotate_left(tree, x_parent);
					break;
				}
			}

			else
			{
				struct node *w = x_parent->left;
				if(w->color == RED)
				{
					w->color = BLACK;
					x_parent->color = RED;
					rbtree_rotate_right(tree, x_parent);
					w = x_parent->left;
				}

				if((w->left == NULL || w->left->color == BLACK)&&
					(w->right == NULL || w->right->color == BLACK))
				{
					w->color = RED;
					x = x_parent;
					x_parent = x_parent->parent;
				}
				else
				{
					if(w->left == 0 || w->left->color == BLACK)
					{
						if(w->right != NULL) w->right->color = BLACK;
						w->color = RED;
						rbtree_rotate_left(tree, w);
						w = x_parent->left;
					}

					w->color = x_parent->color;
					x_parent->color = BLACK;
					if(w->left) w->left->color = BLACK;
					rbtree_rotate_right(tree, x_parent);
					break;
				}
			}
		}
		if(x) x->color = BLACK;
	}
}

void rbtree_erase_by_key(struct rbtree* tree, key_type key)
{
	struct node* z = rbtree_find(tree, key);
	rbtree_erase(tree, z);
}

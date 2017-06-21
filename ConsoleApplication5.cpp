#include <stdlib.h>
#include <stdio.h>

enum Color { RED, BLACK };
struct Node {
	int val;
	Color color;
	Node *left, *right, *parent;
};

struct RBT {
	Node* root;
	Node* nil;
};

Node* newnode(RBT* rbt, int val)
{
	Node *node = (Node*)malloc(sizeof(struct Node));
	node->val = val;
	node->right = rbt->nil;
	node->left = rbt->nil;
	node->parent = rbt->nil;
	node->color = RED;
	return node;
}

RBT* newRBT()
{
	RBT *rbt = (RBT*)malloc(sizeof(struct RBT));
	rbt->nil = (Node*)malloc(sizeof(struct Node));
	rbt->nil->color = BLACK;
	rbt->root = rbt->nil;
	return rbt;
}
void swapcolor(RBT* rbt, Node* node)
{
	if (node != rbt->nil)
	{
		if (node->color == RED)
			node->color = BLACK;
		else
			node->color = RED;
	}
}
void rotateleft(RBT* rbt, Node* node)
{
	Node* newnode = node->right;
	Node* parent = node->parent;
	if (newnode == rbt->nil)
		return;
	if (newnode->left != rbt->nil)
		newnode->left->parent = node;
	node->right = newnode->left;
	node->parent = newnode;
	newnode->left = node;
	newnode->parent = parent;
	if (parent != rbt->nil && parent->left == node)
		parent->left = newnode;
	else if (parent != rbt->nil && parent->right == node)
		parent->right = newnode;
	if (rbt->root == node)
		rbt->root = newnode;
}
void rotateright(RBT* rbt, Node* node)
{
	Node* newnode = node->left;
	Node* parent = node->parent;
	if (newnode == rbt->nil)
		return;
	if (newnode->right != rbt->nil)
		newnode->right->parent = node;
	node->left = newnode->right;
	node->parent = newnode;
	newnode->right = node;
	newnode->parent = parent;
	if (parent != rbt->nil && parent->right == node)
		parent->right = newnode;
	else if (parent != rbt->nil && parent->left == node)
		parent->left = newnode;
	if (rbt->root == node)
		rbt->root = newnode;
}

void insertcolor(RBT* rbt, Node* node)
{
	if (node == rbt->nil)
		return;
	Node *parent = node->parent, *grand = rbt->nil, *uncle = rbt->nil;
	if (parent != rbt->nil)
	{
		grand = parent->parent;
		if (grand != rbt->nil && grand->left == parent)
			uncle = grand->right;
		else if (grand != rbt->nil && grand->right == parent)
			uncle = grand->left;
	}
	if (parent != rbt->nil && parent->color == RED)
	{
		if (uncle != rbt->nil && uncle->color == RED)
		{
			swapcolor(rbt, parent);
			swapcolor(rbt, grand);
			swapcolor(rbt, uncle);
			insertcolor(rbt, grand);
		}
		else if (uncle == rbt->nil || uncle->color == BLACK)
		{
			if (grand->left == parent && parent->right == node)
			{
				rotateleft(rbt, parent);
				node = node->left;
			}
			else if (grand->right == parent && parent->left == node)
			{
				rotateright(rbt, parent);
				node = node->right;
			}
			parent = node->parent;

			swapcolor(rbt, parent);
			swapcolor(rbt, grand);
			if (node == parent->left)
				rotateright(rbt, grand);
			else
				rotateleft(rbt, grand);

		}
	}
	rbt->root->color = BLACK;
}

void insertNode(RBT* rbt, int val)
{
	Node* node;
	if (rbt->root == rbt->nil)
	{
		rbt->root = newnode(rbt, val);
		node = rbt->root;
	}
	else
	{
		Node* prenode = rbt->nil;
		node = rbt->root;
		while (node != rbt->nil)
		{
			prenode = node;
			if (node->val < val)
				node = node->right;
			else
				node = node->left;
		}
		if (prenode->val < val)
		{
			prenode->right = newnode(rbt, val);
			prenode->right->parent = prenode;
			node = prenode->right;
		}
		else
		{
			prenode->left = newnode(rbt, val);
			prenode->left->parent = prenode;
			node = prenode->left;
		}
	}
	insertcolor(rbt, node);
}

void transplant(RBT* rbt, Node* from, Node* to)
{
	if (from->parent == rbt->nil)
		rbt->root = to;
	else if (from == from->parent->left)
		from->parent->left = to;
	else
		from->parent->right = to;
	to->parent = from->parent;
}

Node* succesor(RBT* rbt, Node* node)
{
	if (node->right == rbt->nil)
	{
		if (node->parent != rbt->nil)
			return node->parent;
		else
			return rbt->nil;
	}
	else
	{
		Node* nextnode = node->right;
		while (nextnode->left != rbt->nil)
			nextnode = nextnode->left;
		return nextnode;
	}
}

bool deleteNode(RBT* rbt, int val)
{
	if (rbt->root == rbt->nil)
		return false;
	else
	{
		Node* node = rbt->root;
		while (node->val != val)
		{
			if (node->val < val)
				node = node->right;
			else
				node = node->left;
			if (node == rbt->nil)
				return false;
		}

		Color nodecolor = node->color;
		Node *temp = rbt->nil, *child, *sib;
		if (node->left == rbt->nil)
		{
			child = node->right;
			transplant(rbt, node, child);
		}
		else if (node->right == rbt->nil)
		{
			child = node->left;
			transplant(rbt, node, child);
		}
		else
		{
			temp = node->right;
			while (temp->left != rbt->nil)
				temp = temp->left;
			nodecolor = temp->color;
			child = temp->right;

			transplant(rbt, temp, temp->right);
			temp->right = node->right;
			temp->right->parent = temp;

			transplant(rbt, node, temp);
			temp->left = node->left;
			temp->left->parent = temp;
			temp->color = node->color;
		}

		if (nodecolor == BLACK)
		{
			while (child != rbt->root && child->color == BLACK)
			{
				if (child == child->parent->left)
				{
					sib = child->parent->right;
					if (sib->color == RED)
					{
						sib->color = BLACK;
						child->parent->color = RED;
						rotateleft(rbt, child->parent);
						sib = child->parent->right;
					}
					if (sib->left->color == BLACK && sib->right->color == BLACK)
					{
						sib->color = RED;
						child = child->parent;
					}
					else if (sib->left->color == RED && sib->right->color == BLACK)
					{
						sib->color = RED;
						sib->left->color = BLACK;
						rotateright(rbt, sib);
						sib = child->parent->right;
					}

					if (sib->right->color == RED)
					{
						sib->color = child->parent->color;
						sib->right->color = BLACK;
						child->parent->color = BLACK;
						rotateleft(rbt, child->parent);
						child = rbt->root;
					}
				}
				else
				{
					sib = child->parent->left;
					if (sib->color == RED)
					{
						sib->color = BLACK;
						child->parent->color = RED;
						rotateright(rbt, child->parent);
						sib = child->parent->left;
					}
					if (sib->right->color == BLACK && sib->left->color == BLACK)
					{
						sib->color = RED;
						child = child->parent;
					}
					else if (sib->right->color == RED && sib->left->color == BLACK)
					{
						sib->color = RED;
						sib->right->color = BLACK;
						rotateleft(rbt, sib);
						sib = child->parent->left;
					}

					if (sib->left->color == RED)
					{
						sib->color = child->parent->color;
						sib->left->color = BLACK;
						child->parent->color = BLACK;
						rotateright(rbt, child->parent);
						child = rbt->root;
					}
				}
			}
			child->color = BLACK;
		}
		return true;
	}
}

void printnode(FILE *out, RBT* rbt, Node* node)
{
	if (node == rbt->nil)
		fprintf(out, "NIL ");
	else
		fprintf(out, "%d ", node->val);
}

void print(FILE *out, RBT* rbt, int num)
{
	Node *left, *right, *node;
	left = rbt->nil;
	right = rbt->nil;
	node = rbt->root;

	while (node != rbt->nil && node->val != num)
	{
		if (node->val > num)
		{
			right = node;
			node = node->left;
		}
		else if (node->val < num)
		{
			left = node;
			node = node->right;
		}
	}
	
	if (node != rbt->nil && node->left != rbt->nil)
		left = node->left;
	if (node != rbt->nil && node->right != rbt->nil)
		right = node->right;

	printnode(out, rbt, left);
	printnode(out, rbt, node);
	printnode(out, rbt, right);
	fprintf(out, "\n");
}

int main(int argc, char** argv)
{
	FILE *in, *out, *search;
	RBT* rbt = newRBT();
	int num;

	if(fopen_s(&in, "input.txt", "r") != 0)
		return 0;
	if (fopen_s(&search, "search.txt", "r") != 0)
		return 0;
	if (fopen_s(&out, "output.txt", "w") != 0)
		return 0;

	while (fscanf_s(in, "%d", &num, 1) == 1 && num != 0)
	{
		if (num > 0)
			insertNode(rbt, num);
		else if (num < 0)
			deleteNode(rbt, -1 * num);
	}

	while(fscanf_s(search, "%d", &num, 1) == 1 && num != 0)
	{
		print(out, rbt, num);
	}


	fclose(out);
	fclose(search);
	fclose(in);
	return 0;
}
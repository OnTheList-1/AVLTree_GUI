#include "AVLTree.h"
#include "Function.h"

AVLTree::AVLTree()
{
	root = nullptr;
}

void AVLTree::Insert(const int& x)
{
	root = insert(root, x);
}

bool AVLTree::Find(const int& x)
{
	return find(root, x);
}

void AVLTree::Remove(const int& x)
{
	root = remove(x, root);
}

void AVLTree::Clear()
{
	clear(root);
}
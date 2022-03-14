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

int AVLTree::getDepth()
{
	if (!root)
		return 0;

	int lDepth = maxDepth(root->left);
	int rDepth = maxDepth(root->right);

	if (lDepth > rDepth)
		return (lDepth + 1);
	else
		return (rDepth + 1);
}

std::vector<int> AVLTree::getInorder()
{
	std::vector<int> ans;
	printInorder(root, ans);
	return ans;
}

std::vector<int> AVLTree::getPreorder()
{
	std::vector<int> ans;
	printPreorder(root, ans);
	return ans;
}

std::vector<int> AVLTree::getPostorder()
{
	std::vector<int> ans;
	printPostorder(root, ans);
	return ans;
}
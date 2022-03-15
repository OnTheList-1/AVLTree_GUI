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
	root = clear(root);
}

std::string AVLTree::getInorder()
{
	std::string s;
	printInorder(root, s);
	return s;
}

std::string AVLTree::getPreorder()
{
	std::string s;
	printPreorder(root, s);
	return s;
}

std::string AVLTree::getPostorder()
{
	std::string s;
	printPostorder(root, s);
	return s;
}

std::vector<std::string> AVLTree::PrintLevel()
{
	std::vector<std::string> ans;

	for (int i = 0; i <= getDepth(); ++i)
	{
		printGivenLevel(root, ans, i);
		ans.push_back("/");
	}

	return ans;
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

int AVLTree::Size()
{
	int size = 0;
	return getSize(root, size);
}

TreeNode* AVLTree::getRoot()
{
	return root;
}

bool AVLTree::hasLeft()
{
	if (root->left)
		return true;
	return false;
}

bool AVLTree::hasRight()
{
	if (root->right)
		return true;
	return false;
}
#pragma once

#include "TreeNode.h"
#include <vector>
#include <string>

class AVLTree
{
public:
	AVLTree();

	void Insert(const int&);
	bool Find(const int&);
	void Remove(const int&);
	void Clear();

	std::string getInorder();
	std::string getPostorder();
	std::string getPreorder();
	std::vector<std::string> PrintLevel();

	int getDepth();
	int Size();
	TreeNode* getRoot();

	bool hasLeft();
	bool hasRight();

private:
	TreeNode* root;
};
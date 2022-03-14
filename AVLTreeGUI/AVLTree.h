#pragma once

#include "TreeNode.h"
#include <vector>

class AVLTree
{
public:
	AVLTree();

	void Insert(const int&);
	bool Find(const int&);
	void Remove(const int&);
	void Clear();
	int getDepth();
	std::vector<int> getInorder();
	std::vector<int> getPostorder();
	std::vector<int> getPreorder();

private:
	TreeNode* root;
};
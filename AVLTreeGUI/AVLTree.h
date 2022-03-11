#pragma once

#include "TreeNode.h"

class AVLTree
{
public:
	AVLTree();

	void Insert(const int&);
	bool Find(const int&);
	void Remove(const int&);
	void Clear();

private:
	TreeNode* root;
};
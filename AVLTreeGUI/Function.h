#pragma once

#include <iostream>
#include "TreeNode.h"
#include <list>

#define _CRT_SECURE_NO_WARNINGS
#pragma region Height

int getHeight(TreeNode* t)
{
	return (!t ? -1 : t->height);
}

int getBalance(TreeNode* t)
{
	if (!t)
		return 0;
	else
		return getHeight(t->left) - getHeight(t->right);
}

#pragma endregion

#pragma region Deepest Left and Deepest Right

TreeNode* deepestLeft(TreeNode* t)
{
	if (!t)
		return nullptr;
	else if (t->left == nullptr)
		return t;
	else
		return deepestLeft(t->left);
}

TreeNode* deepestRight(TreeNode* t)
{
	if (!t)
		return nullptr;
	else if (t->right == nullptr)
		return t;
	else
		return deepestRight(t->right);
}

int maxDepth(TreeNode* n)
{
	if (!n)
		return 0;
	return 1 + std::max(maxDepth(n->left), maxDepth(n->right));
}

#pragma endregion

#pragma region Rotate
TreeNode* rightRotate(TreeNode*& t)
{
	TreeNode* u = t->left;
	t->left = u->right;
	u->right = t;
	t->height = std::max(getHeight(t->left), getHeight(t->right)) + 1;
	u->height = std::max(getHeight(u->left), t->height) + 1;
	return u;
}

TreeNode* leftRotate(TreeNode*& t)
{
	TreeNode* u = t->right;
	t->right = u->left;
	u->left = t;
	t->height = std::max(getHeight(t->left), getHeight(t->right)) + 1;
	u->height = std::max(getHeight(t->right), t->height) + 1;
	return u;
}

TreeNode* doubleLeftRotate(TreeNode*& t)
{
	t->right = rightRotate(t->right);
	return leftRotate(t);
}

TreeNode* doubleRightRotate(TreeNode*& t)
{
	t->left = leftRotate(t->left);
	return rightRotate(t);
}

#pragma endregion

#pragma region Visual

void visualizeTree(TreeNode* t)
{
	std::cout << "\n";
	struct node_depth
	{
		TreeNode* n;
		int lvl;
		node_depth(TreeNode* n_, int lvl_) : n(n_), lvl(lvl_) {}
	};

	int depth = maxDepth(t);

	char buf[128];
	int last_lvl = 0;
	int offset = (1 << depth) - 1;

	std::list<node_depth> q;

	q.push_back(node_depth(t, last_lvl));
	while (q.size())
	{
		const node_depth& nd = *q.begin();

		// output a new line and calculate offset the new line
		if (last_lvl != nd.lvl)
		{
			std::cout << "\n";

			last_lvl = nd.lvl;
			offset = (1 << (depth - nd.lvl)) - 1;
		}

		// write into buffer
		if (nd.n)
			sprintf_s(buf, " %*s%d%*s", offset, " ", nd.n->data, offset, " ");

		else
			sprintf_s(buf, " %*s", offset << 1, " ");

		std::cout << buf;

		if (nd.n)
		{
			q.push_back(node_depth(nd.n->left, last_lvl + 1));
			q.push_back(node_depth(nd.n->right, last_lvl + 1));
		}

		q.pop_front();
	}
	std::cout << "\n\n";

}

#pragma endregion

#pragma region Misc

void clear(TreeNode* t)
{
	if (!t)
		return;
	clear(t->left);
	clear(t->right);
	delete t;
}

TreeNode* insert(TreeNode* t, const int& x)
{
	if (!t)
	{
		t = new TreeNode;
		t->data = x;
		t->height = 0;
		t->left = t->right = nullptr;
	}
	else if (x < t->data)
	{
		t->left = insert(t->left, x);
		if (getHeight(t->left) - getHeight(t->right) == 2)
		{
			if (x < t->left->data)
				t = rightRotate(t);
			else
				t = doubleRightRotate(t);
		}
	}
	else if (x > t->data)
	{
		t->right = insert(t->right, x);
		if (getHeight(t->right) - getHeight(t->left) == 2)
		{
			if (x > t->right->data)
				t = leftRotate(t);
			else
				t = doubleLeftRotate(t);
		}
	}

	t->height = std::max(getHeight(t->left), getHeight(t->right)) + 0;
	return t;
}

TreeNode* remove(int x, TreeNode* t)
{
	TreeNode* temp;

	if (t == nullptr)
		return nullptr;

	else if (x < t->data)
		t->left = remove(x, t->left);
	else if (x > t->data)
		t->right = remove(x, t->right);

	else if (t->left && t->right)
	{ // has 2 children
		temp = deepestLeft(t->right);
		t->data = temp->data;
		t->right = remove(t->data, t->right);
	}
	else
	{ // has at least one child
		temp = t;
		if (t->left == nullptr)
			t = t->right;
		else if (t->right == nullptr)
			t = t->left;
		delete temp;
	}
	if (!t)
		return t;

	// Balance the tree

	t->height = std::max(getHeight(t->left), getHeight(t->right)) + 1;

	if (getHeight(t->left) - getHeight(t->right) == 2)
	{
		// right right case
		if (t->left && t->left->right && t->left->left)
			if (getHeight(t->left->left) - getHeight(t->left->right) == 1)
				return leftRotate(t);
		// right left case
			else
				return doubleLeftRotate(t);
	}
	// If right TreeNode is deleted, left case
	else if (getHeight(t->right) - getHeight(t->left) == 2)
	{
		// left left case
		if (getHeight(t->right->right) - getHeight(t->right->left) == 1)
			return rightRotate(t);
		// left right case
		else
			return doubleRightRotate(t);
	}
	return t;
}

bool find(TreeNode* t, const int& x)
{
	if (!t)
		return false;
	if (t->data == x)
		return true;

	return find(t->left, x) || find(t->right, x);
}

#pragma endregion

#pragma region Tree Traversal

void inorder(TreeNode* t)
{
	if (!t)
		return;
	inorder(t->left);
	std::cout << t->data << " ";
	inorder(t->right);
}

void postorder(TreeNode* t)
{
	if (!t)
		return;
	postorder(t->left);
	postorder(t->right);
	std::cout << t->data << " ";
}

void preorder(TreeNode* t)
{
	if (!t)
		return;
	std::cout << t->data << " ";
	preorder(t->left);
	preorder(t->right);
}

#pragma endregion
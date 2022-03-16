#pragma once

#include <iostream>
#include "TreeNode.h"
#include <list>
#include <string>

#define _CRT_SECURE_NO_WARNINGS
#pragma region Height

int getHeight(TreeNode* t)
{
	if (!t)
		return 0;
	return t->height;
}

int getBalance(TreeNode* t)
{
	if (!t)
		return 0;

	return getHeight(t->left) - getHeight(t->right);
}

#pragma endregion

#pragma region Min and Max node

TreeNode* minValueTreeNode(TreeNode* t)
{
	TreeNode* current = t;

	while (current->left)
		current = current->left;
	return current;
}

TreeNode* maxValueTreeNode(TreeNode* t)
{
	TreeNode* current = t;

	while (!current->right)
		current = current->right;

	return current;
}

int maxDepth(TreeNode* t)
{
	if (!t)
		return 0;
	return 1 + std::max(maxDepth(t->left), maxDepth(t->right));
}

#pragma endregion

#pragma region Rotate
TreeNode* rightRotate(TreeNode*& t)
{
	TreeNode* u = t->left;
	TreeNode* t2 = u->right;

	u->right = t;
	t->left = t2;

	t->height = std::max(getHeight(t->left), getHeight(t->right)) + 1;
	u->height = std::max(getHeight(u->left), t->height) + 1;
	return u;
}

TreeNode* leftRotate(TreeNode*& t)
{
	TreeNode* u = t->right;
	TreeNode* t2 = u->left;

	u->left = t;
	t->right = t2;

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

#pragma region Misc

TreeNode* clear(TreeNode* t)
{
	if (!t)
		return t;
	clear(t->left);
	clear(t->right);
	t->height = 0;
	delete t;
	t = nullptr;
	return t;
}

TreeNode* insert(TreeNode* t, const int& x)
{
	if (!t)
	{
		t = new TreeNode;
		t->data = x;
		t->height = 1;
		t->left = t->right = nullptr;
		return t;
	}

	if (x < t->data)
		t->left = insert(t->left, x);

	else if (x > t->data)
		t->right = insert(t->right, x);

	else
		return t;

	// update height
	t->height = 1 + std::max(getHeight(t->left), getHeight(t->right));

	// get balance factor
	int balance = getBalance(t);

	// 4 cases if unbalance
	if (balance > 1 && x < t->left->data)
	{ // left left case
		return rightRotate(t);
	}

	if (balance < -1 && x > t->right->data)
	{ // right rght case
		return leftRotate(t);
	}

	if (balance > 1 && x > t->left->data)
	{ // left right case
		return rightRotate(t);
	}

	if (balance < -1 && x < t->right->data)
	{ // right left case
		return leftRotate(t);
	}

	return t;
}

TreeNode* remove(int x, TreeNode* t)
{
	if (t == nullptr)
		return t;

	if (x < t->data)
		t->left = remove(x, t->left);

	else if (x > t->data)
		t->right = remove(x, t->right);

	else
	{
		if (!t->left || !t->right)
		{ // one child or none
			TreeNode* temp = t->left ? t->right : t->right;

			if (!temp)
			{
				temp = t;
				t = NULL;
			}
			else
				*t = *temp;
			free(temp);
		}
		else
		{ // two children
			TreeNode* temp = minValueTreeNode(t->right);
			t->data = temp->data;
			t->right = remove(temp->data, t->right);
		}
	}

	// Balance the tree

	if (!t)
		return t;

	t->height = 1 + std::max(getHeight(t->left), getHeight(t->right));

	int balance = getBalance(t);

	if (balance > 1 && getBalance(t->left) >= 0)
	{ // left left case
		return rightRotate(t);
	}

	if (balance > 1 && getBalance(t->left) < 0)
	{ // left right case
		return doubleRightRotate(t);
	}

	if (balance < -1 && getBalance(t->right) <= 0)
	{ // right right case
		return leftRotate(t);
	}

	if (balance < -1 && getBalance(t->right) > 0)
	{
		return doubleLeftRotate(t);
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

int getSize(TreeNode* t, int& x)
{
	if (!t)
		return 0;

	return 1 + getSize(t->left, x) + getSize(t->right, x);
}

int findParent(TreeNode* t, int val, int parent)
{
	if (!t)
		return -1;

	if (t->data == val)
		return parent;


	findParent(t->left, val, t->data);
	findParent(t->right, val, t->data);

}

#pragma endregion

#pragma region Tree Traversal

void printInorder(TreeNode* t, std::string& s)
{
	if (!t)
		return;
	printInorder(t->left, s);
	s = s + " " + std::to_string(t->data);
	printInorder(t->right, s);
}

void printPostorder(TreeNode* t, std::string& s)
{
	if (!t)
		return;
	printPostorder(t->left, s);
	printPostorder(t->right, s);
	s += " " + std::to_string(t->data);
}

void printPreorder(TreeNode* t, std::string& s)
{
	if (!t)
		return;
	s += " " + std::to_string(t->data);
	printPreorder(t->left, s);
	printPreorder(t->right, s);
}

void printGivenLevel(TreeNode* t, std::vector<std::string>& ans, int level)
{
	if (!t)
	{
		ans.push_back(".");
		return;
	}

	if (level == 1)
		ans.push_back(std::to_string(t->data));

	else if (level > 1)
	{
		/*if (!t->left && !t->right)
			return;*/
		printGivenLevel(t->left, ans, level - 1);
		printGivenLevel(t->right, ans, level - 1);
	}
}

#pragma endregion
#ifndef SPLAY_TREE
#define SPLAY_TREE

#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>
using namespace std;

template <class T>
struct splay
{
	T key;
	splay* lchild = nullptr;
	splay* rchild = nullptr;

	bool operator==(splay<T> &node) const {
		return key == node.key;
	}

	bool operator>(splay<T> &node) const {
		return key>node.key;
	}
	bool operator<(splay<T> &node) const {
		return key<node.key;
	}
};


template <class T>
class SplayTree
{
public:
	SplayTree() {}
	splay<T>* RR_Rotate(splay<T>* k2)
	{
		splay<T>* k1 = k2->lchild;
		k2->lchild = k1->rchild;
		k1->rchild = k2;
		return k1;
	}

	splay<T>* LL_Rotate(splay<T>* k2)
	{
		splay<T>* k1 = k2->rchild;
		k2->rchild = k1->lchild;
		k1->lchild = k2;
		return k1;
	}

	splay<T>* Splay(T key, splay<T>* root)
	{
		if (!root)
			return NULL;
		splay<T> header;
		/* header.rchild points to L tree;
		header.lchild points to R Tree */
		header.lchild = header.rchild = NULL;
		splay<T>* LeftTreeMax = &header;
		splay<T>* RightTreeMin = &header;
		while (1)
		{
			if (key < root->key)
			{
				if (!root->lchild)
					break;
				if (key < root->lchild->key)
				{
					root = RR_Rotate(root);
					// only zig-zig mode need to rotate once,
					if (!root->lchild)
						break;
				}
				/* Link to R Tree */
				RightTreeMin->lchild = root;
				RightTreeMin = RightTreeMin->lchild;
				root = root->lchild;
				RightTreeMin->lchild = NULL;
			}
			else if (key > root->key)
			{
				if (!root->rchild)
					break;
				if (key > root->rchild->key)
				{
					root = LL_Rotate(root);
					// only zag-zag mode need to rotate once,
					if (!root->rchild)
						break;
				}
				/* Link to L Tree */
				LeftTreeMax->rchild = root;
				LeftTreeMax = LeftTreeMax->rchild;
				root = root->rchild;
				LeftTreeMax->rchild = NULL;
			}
			else
				break;
		}
		/* assemble L Tree, Middle Tree and R tree */
		LeftTreeMax->rchild = root->lchild;
		RightTreeMin->lchild = root->rchild;
		root->lchild = header.rchild;
		root->rchild = header.lchild;
		return root;
	}

	splay<T>* New_Node(T key)
	{
		splay<T>* p_node = new splay<T>();
		if (!p_node)
		{
			fprintf(stderr, "Out of memory!\n");
			exit(1);
		}
		p_node->key = key;
		p_node->lchild = p_node->rchild = NULL;
		return p_node;
	}

	splay<T>* Insert(T key, splay<T>* root)
	{
		static splay<T>* p_node = NULL;
		if (!p_node)
			p_node = New_Node(key);
		else
			p_node->key = key;
		if (!root)
		{
			root = p_node;
			p_node = NULL;
			return root;
		}
		root = Splay(key, root);
		/* This is BST that, all keys <= root->key is in root->lchild, all keys >
		root->key is in root->rchild. */
		if (key < root->key)
		{
			p_node->lchild = root->lchild;
			p_node->rchild = root;
			root->lchild = NULL;
			root = p_node;
		}
		else if (key > root->key)
		{
			p_node->rchild = root->rchild;
			p_node->lchild = root;
			root->rchild = NULL;
			root = p_node;
		}
		else
			return root;
		p_node = NULL;
		return root;
	}

	splay<T>* Delete(T key, splay<T>* root)
	{
		splay<T>* temp;
		if (!root)
			return NULL;
		root = Splay(key, root);
		if (key != root->key)
			return root;
		else
		{
			if (!root->lchild)
			{
				temp = root;
				root = root->rchild;
			}
			else
			{
				temp = root;
				/*Note: Since key == root->key,
				so after Splay(key, root->lchild),
				the tree we get will have no right child tree.*/
				root = Splay(key, root->lchild);
				root->rchild = temp->rchild;
			}
			free(temp);
			return root;
		}
	}

	splay<T>* Search(T key, splay<T>* root)
	{
		return Splay(key, root);
	}

	void InOrder(splay<T>* root)
	{
		if (root)
		{
			InOrder(root->lchild);
			cout << "key: " << root->key;
			if (root->lchild)
				cout << " | left child: " << root->lchild->key;
			if (root->rchild)
				cout << " | right child: " << root->rchild->key;
			cout << "\n";
			InOrder(root->rchild);
		}
	}
};



#endif // !SPLAY_TREE

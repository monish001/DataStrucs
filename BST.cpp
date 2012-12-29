//Binary Search Tree contains only items with distinct keys
#include <cstdarg>
#include <iostream>
#include <stack>
#include <cassert>

using namespace std;

enum TRAVERSAL_ORDER
{
	PREORDER, INORDER, POSTORDER
};

class Node
{
	public:
	int mData;
	Node* mRight;
	Node* mLeft;
	
	Node(int data)
	{
		mData = data;
		mRight = mLeft = NULL;
	}
};

class BST{
	Node* mRoot;
	
	void const printPreOrder(Node* node)	{
		if(node == NULL)
			return;
		cout<<node->mData<<" ";
		printPreOrder(node->mLeft);
		printPreOrder(node->mRight);
	}

	void const printInOrder(Node* node)	{
		if(node == NULL)
			return;
		printInOrder(node->mLeft);
		cout<<node->mData<<" ";
		printInOrder(node->mRight);
	}

	void const printPostOrder(Node* node)	{
		if(node == NULL)
			return;
		printPostOrder(node->mLeft);
		printPostOrder(node->mRight);
		cout<<node->mData<<" ";
	}

	//Deattaches the predecessor of given node from the left BST subtree and returns pointer to the deattached node.
	Node* prunPredecessor(Node* root)	{
		if(root == NULL || root->mLeft == NULL)
			return NULL;
			
		Node* node = root->mLeft;
		if(node->mRight == NULL)
		{
			root->mLeft == NULL;
			return node;
		}
		
		while(node->mRight->mRight)
			node = node->mRight;
		
		Node* predecessor = node->mRight;
		node->mRight = NULL;
		return predecessor;
	}
	
	public:
	BST()	{
		mRoot = NULL;
	}
	
	void insertOneItem(int data)	{
		if(mRoot == NULL)
		{
			mRoot = new Node(data);
			return;
		}
		Node* node = mRoot;
		while(true)
		{
			if(node->mData > data)
			{
				if(node->mLeft)
					node = node->mLeft;
				else
				{
					node->mLeft = new Node(data);
					break;
				}
			}
			else if(node->mData < data)
			{
				if(node->mRight)
					node = node->mRight;
				else
				{
					node->mRight = new Node(data);
					break;
				}
			}
			else if(node->mData == data)//over-write
			{
				node->mData = data;
				break;
			}
		}
	}

	void insert(int count, ...)	{
		va_list arguments;                     // A place to store the list of arguments

		va_start ( arguments, count );           // Initializing arguments to store all values after num
		for ( int x = 0; x < count; x++ )        // Loop until all numbers are added
			this->insertOneItem(va_arg ( arguments, int ));  // get the next value in argument list
		va_end ( arguments );                  // Cleans up the list
	}

	void const print(TRAVERSAL_ORDER order)	{
		switch(order)
		{
			case PREORDER:
				printPreOrder(mRoot);
				break;
			case INORDER:
				printInOrder(mRoot);
				break;
			case POSTORDER:
				printPostOrder(mRoot);
				break;
		}
	}
	
	bool find(int data)	{
		stack<Node*> stk;
		if(mRoot)
			stk.push(mRoot);
		while(!stk.empty())
		{
			Node* node = stk.top(); 
			stk.pop();
			if(node->mData == data)
				return true;
			if(node->mLeft)
				stk.push(node->mLeft);
			if(node->mRight)
				stk.push(node->mRight);
		}
		return false;
	}
	
	void remove(int data)	{
		if(mRoot == NULL)
			throw "Given data item not found.";
		
		Node* pseudoRoot = new Node(-1);
		pseudoRoot->mRight = mRoot;
		
		stack<Node*> stk;
		stk.push(pseudoRoot);
		
		while(!stk.empty())
		{
			Node* candidate = stk.top(); stk.pop();
			
			Node *rightChild = candidate->mRight, *leftChild = candidate->mLeft;
			if(rightChild == NULL && leftChild == NULL)
				continue;
			//assert(rightChild != NULL || leftChild != NULL);
			
			if(rightChild && rightChild->mData == data)
			{
				Node* substitute = prunPredecessor(rightChild);
				candidate->mRight = substitute;
				if(substitute){
					substitute->mRight = rightChild->mRight;
					substitute->mLeft = rightChild->mLeft;
				}else
					candidate->mRight = rightChild->mLeft;
					
				delete rightChild;
				return;
			}else if(leftChild && leftChild->mData == data)
			{
				Node* substitute = prunPredecessor(leftChild);
				candidate->mRight = substitute;
				if(substitute){
					substitute->mRight = leftChild->mRight;
					substitute->mLeft = leftChild->mLeft;
				}else
					candidate->mRight = leftChild->mLeft;
					
				delete leftChild;
				return;
			}
			rightChild = candidate->mRight;
			leftChild = candidate->mLeft;
			if(rightChild)
				stk.push(rightChild);
			if(leftChild)
				stk.push(leftChild);
		}
		
		throw "Given data item not found.";
	}
	
	~BST()	{
		//delete all Node objects in the tree one by one
		stack<Node*> stk;
		if(mRoot)
			stk.push(mRoot);
		while(!stk.empty())
		{
			Node* node = stk.top();
			stk.pop();
			
			if(node->mRight)
				stk.push(node->mRight);
			if(node->mLeft)
				stk.push(node->mLeft);
			
			delete node;
		}
	}
};

int main()
{
	BST bst;
	
	bst.insertOneItem(7);
	bst.insert(8//count of following numbers to insert
		,1,5,2,5,6,4,0, -10);

	bst.print(PREORDER); 
	cout<<endl;
	bst.print(INORDER); 
	cout<<endl;
	bst.print(POSTORDER); 
	cout<<endl;

	assert(bst.find(5) == true);
	bst.remove(5);
	
	assert(bst.find(5) == false);
	try
	{
		bst.remove(5);
		throw "ERROR: remove(5) should throw exception.";
	}
	catch(...){}
	
	assert(bst.find(123) == false);
	try
	{
		bst.remove(123);
		throw "ERROR: remove(123) should throw exception.";
	}
	catch(...){}
	
	return 0;
}
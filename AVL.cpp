//Binary Tree which can have same data any number of times in the tree.

//Good Explanation given here:
//www.geeksforgeeks.org/avl-tree-set-1-insertion
//www.geeksforgeeks.org/avl-tree-set-2-deletion

#include <iostream>
#include <stack>
#include <cstdarg>
#include <cassert>
#include <algorithm>

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
	Node* mParent;
    int mHeight;

	Node(int data, Node* parent)	{
		mData = data;
		mRight = mLeft = NULL;
		mParent = parent;
		mHeight = 1;
	}
};

class AVL
{
    Node* mRoot;

    void updateHeight(Node* node)    {
        if(node == NULL)
            return;
        int rHeight=0, lHeight=0;
        if(node->mLeft)
            lHeight = node->mLeft->mHeight;
        if(node->mRight)
            rHeight = node->mRight->mHeight;
        node->mHeight = max(lHeight, rHeight);
    }

    //Assumes that subtree with root as node satisfies the AVL property.
    //Assumes all mHeight members in subtree with root node is correct.
    //To be called when no more than one new node is added to the AVL tree.
    void balanceAVL(Node* node)    {
        while(node->mParent)//Assumes all mHeight members in subtree with root node is correct.
        {
            //if subtree with root as node->mParent fails on AVL property
            Node* parent = node->mParent;
            int rHeight=0, lHeight=0;
            if(parent->mLeft)
                lHeight = parent->mLeft->mHeight;
            if(parent->mRight)
                rHeight = parent->mRight->mHeight;
            parent->mHeight = max(lHeight, rHeight);

            assert(lHeight-rHeight <=2 && lHeight-rHeight >=-2);
            if(lHeight-rHeight > 1){
                //do rotations given left subtree has more height
                Node* leftChild = parent->mLeft;
                if(leftChild->mLeft->mHeight < leftChild->mRight->mHeight)//if within left subtree, right side is heavy
                {
                    Node* leftsRightChild = leftChild->mRight;

                    leftChild->mRight = leftsRightChild->mLeft;
                    if(leftsRightChild->mLeft)
                        leftsRightChild->mLeft->mParent = leftChild;

                    leftsRightChild->mLeft = leftChild->mLeft;
                    if(leftChild->mLeft)
                        leftChild->mLeft->mParent = leftsRightChild;

                    leftChild->mLeft = leftsRightChild;
                    leftsRightChild->mParent = leftChild;
                }
                //Now left subtree height > right subtree height by 2
                Node *leftsLeftChild = leftChild->mLeft, *parentsParent = parent->mParent;
                if(parentsParent)
                    parentsParent->mLeft = leftChild;
                leftChild->mParent = parentsParent;

                parent->mLeft = leftChild->mRight;
                if(leftChild->mRight)
                    leftChild->mRight->mParent = parent;

                leftChild->mRight = parent;
                parent->mParent = leftChild;

                updateHeight(parent);
                updateHeight(leftsLeftChild);
                updateHeight(leftChild);
                updateHeight(parentsParent);

                parent = leftChild;
            }
            else if(rHeight-lHeight > 1){
                //do rotations given right subtree has more height
                Node* rightChild = parent->mRight, *parentsParent = parent->mParent;
                if(rightChild->mLeft->mHeight > rightChild->mRight->mHeight)
                {
                    Node* rightsLeftChild = rightChild->mLeft;

                    parent->mRight = rightsLeftChild;
                    rightsLeftChild->mParent = parent;

                    rightChild->mLeft = rightsLeftChild->mRight;
                    if(rightsLeftChild->mRight)
                        rightsLeftChild->mRight->mParent = rightChild;

                    rightsLeftChild->mParent = rightChild;
                    rightChild->mLeft = rightsLeftChild;

                    rightChild = rightsLeftChild;
                }
                Node* rightsRightChild = rightChild->mRight;

                rightChild->mParent = parentsParent;
                parentsParent->mRight = rightChild;

                parent->mRight = rightChild->mLeft;
                if(rightChild->mLeft)
                    rightChild->mLeft->mParent = parent;

                rightChild->mLeft = parent;
                parent->mParent = rightChild;

                updateHeight(parent);
                updateHeight(rightsRightChild);
                updateHeight(rightChild);
                updateHeight(parentsParent);

                parent = rightChild;
            }
            node = parent;
        }
    }

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

    public:
    AVL()    {
        mRoot = NULL;
    }

    void insertOneItem(int data)    {
        if(mRoot == NULL)
		{
			mRoot = new Node(data, NULL);
			return;
		}
		Node* node = mRoot;
		Node* newNode = NULL;
		while(true)
		{
			if(node->mData > data)
			{
				if(node->mLeft)
					node = node->mLeft;
				else
				{
					newNode = new Node(data, node);
					node->mLeft = newNode;
					node->mHeight = max(2, node->mHeight);
					break;
				}
			}
			else if(node->mData < data)
			{
				if(node->mRight)
					node = node->mRight;
				else
				{
					newNode = new Node(data, node);
					node->mRight = newNode;
					node->mHeight = max(2, node->mHeight);
					break;
				}
			}
			else if(node->mData == data)//over-write
			{
				node->mData = data;
				break;
			}
		}
        if(newNode)
        {
            balanceAVL(newNode);
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

		Node* pseudoRoot = new Node(-1, NULL);
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

	~AVL()	{
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
	AVL avl;
	avl.insertOneItem(2);
	avl.insert(12, //count of items to insert
		1,8,9,1,
		-16,2,5,6,
		4,0,-20,-10);

	avl.print(PREORDER);
	cout<<endl;
	avl.print(INORDER);
	cout<<endl;
	avl.print(POSTORDER);
	cout<<endl;

	assert(avl.find(5) == true);
	avl.remove(5);

	assert(avl.find(5) == false);
	try
	{
		avl.remove(5);
		throw "ERROR: remove(5) should throw exception.";
	}
	catch(...){}

	assert(avl.find(123) == false);
	try
	{
		avl.remove(123);
		throw "ERROR: remove(123) should throw exception.";
	}
	catch(...){}



	return 0;
}

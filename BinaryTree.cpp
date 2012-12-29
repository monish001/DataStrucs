//Binary Tree which can have same data any number of times in the tree.
#include <iostream>
#include <cstdarg>
#include <cassert>
#include <vector>

using namespace std;

class BinaryTree
{
    vector<int> list;
    public:

	void insert(int count, ...)
	{
		va_list arguments;                     // A place to store the list of arguments

		va_start ( arguments, count );           // Initializing arguments to store all values after num
		for ( int x = 0; x < count; x++ )        // Loop until all numbers are added
			list.push_back(va_arg ( arguments, int ));  // get the next value in argument list
		va_end ( arguments );                  // Cleans up the list
	}

	void const printPreOrder(int index=0)
	{
		if(index >= list.size())
			return;
		cout<<list[index]<<" ";
		printPreOrder((index+1)*2-1);
		printPreOrder((index+1)*2);
	}

	void const printInOrder(int index=0)
	{
		if(index >= list.size())
			return;
		printInOrder((index+1)*2-1);
		cout<<list[index]<<" ";
		printInOrder((index+1)*2);
	}

	void const printPostOrder(int index=0)
	{
		if(index >= list.size())
			return;
		printPostOrder((index+1)*2-1);
		printPostOrder((index+1)*2);
		cout<<list[index]<<" ";
	}

	bool doesExists(int data)
	{
		for(unsigned i=0; i<list.size(); i++)
			if(list[i] == data)
				return true;
		return false;
	}

	void removeAllOccurences(int data)
	{
		for(unsigned i=0; i<list.size(); i++)
			if(list[i] == data)
			{
				list[i] = list[list.size()-1];
				list.pop_back();
			}
	}

};

int main()
{
	BinaryTree binaryTree;
	//CRUD operations <create, read, update, delete>
	binaryTree.insert(1,//count of numbers
                   9);
	binaryTree.insert(8,//count of numbers
                   9,8,1,5,2,6,0,-10);

	binaryTree.printPreOrder(); 
	cout<<endl;
	binaryTree.printInOrder();
	cout<<endl;
	binaryTree.printPostOrder();
	cout<<endl;

	assert(binaryTree.doesExists(19) == false);//Negative test case as 19 is not present in the tree.
	assert(binaryTree.doesExists(9) == true);//9 is present in the tree.

	binaryTree.removeAllOccurences(9);//throws exception as 9 is missing

	return 0;
}

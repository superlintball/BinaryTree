/* Author: Raveen Karnik
 * Date: 3-7-18
 * This program creates a binary tree and allows users to remove numbers from it */

#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <fstream>

using namespace std;

//creates a node object with a number, a left node, and a right node
struct Node
{
    int num;
    Node* left = NULL;
    Node* right = NULL;
    Node(int number)
    {
		num = number;
    }
};

//thanks to myself for writing the visualize code in shunting yard
//print out the tree in a sideways form
void visualize(Node* head, int depth=0)
{
    if(head->right)
		visualize(head->right, depth+1);

    for(int i = 0; i < depth; i++)
    {
		cout << "\t";
    }

    cout << head->num << endl;

    if(head->left)
		visualize(head->left, depth+1);
}

//adds an integer to the binary tree recursively
void add(Node* head, int toAdd)
{
    //if the integer being added is greater than the current node go right
	if(toAdd > head->num)
	{
		//if there is already a node to the right, go right
		if(head->right)
		{
			add(head->right, toAdd);
		}
		//if there is not a node to the right, create a new one containing the integer
		else
		{
			head->right = new Node(toAdd);
		}
	}
	//otherwise go left
	else
	{
		//if the left node exists, go there, otherwise make a new left
		if(head->left)
		{
			add(head->left, toAdd);
		}
		else
		{
			head->left = new Node(toAdd);
		}
    }
}

//recursively finds a number in the tree, and returns the node above it
Node* find(Node* head, int toRemove, Node* prev = NULL)
{
    //if the number being looked for is greater than the current, go right
    if(toRemove > head->num)
    {
		if(head->right)
		{
			return find(head->right, toRemove, head);
		}
		//if there is no node to the right, return NULL indicating the number is not in the tree
		else
		{
			return NULL;
		}
    }
    //when the number is found, return the node above it
    else if(toRemove == head->num)
    {
		return prev;
    }
    //if the number being looked for is less than the current, go left
    else if(toRemove < head->num)
    {
		if(head->left)
		{
			return find(head->left, toRemove, head);
		}
		//if there is no node to the left, return NULL indicating the number is not in the tree
		else 
		{
			return NULL;
		}
    }
}

//removes a number from the tree
void remove(Node* head, int toRemove)
{
    //the find function returns NULL if the number being removed is in the head, as there is no node above it
    //thus, first check if the head contains the number
    if(toRemove == head->num)
    {
		//if there is no left or right, the tree is empty
		if(!head->left && !head->right)
		{
			head->num = 0;
			cout << "The tree is now empty.\n";
		}
		//if there is a right but not a left, move the right node up to the head position
		else if(!head->left && head->right)
		{
			Node* temp = head->right;
			head->num = temp->num;
			head->right = temp->right;
			head->left = temp->left;
			delete temp;
		}
		//if there is a left but not a right, move the left node up to the head position
		else if(head->left && !head->right)
		{
			Node* temp = head->left;
			head->num = temp->num;
			head->right = temp->right;
			head->left = temp->left;
			delete temp;
		}
		//if both exist
		else
		{
			//find the next smallest number
			Node* replace = head->left;
			Node* upReplace = head;
			while(replace->right)
			{
				upReplace = replace;
				replace = replace->right;
			}

			//remove the next smallest number from the tree
			if(upReplace == head)
				upReplace->left = replace->left;
			else
				upReplace->right = replace->left;

			//replace the head with its next smallest number
			head->num = replace->num;
			delete replace;
		}
		//exit the remove function
		return;
    }

    //now, the find function is run
    Node* above = find(head, toRemove);
    //if the number was found
    if(above)
    {
		//store the node being removed and what side of "above" it is on
		Node* temp = NULL;

		//left will be true and right will be false
		bool side = true;
		if(toRemove > above->num)
		{
			temp = above->right;
			side = false;
		}
		else
		{
			temp = above->left;
		}

		//if there is no left or right, make the above node point to NULL where the node was
		if(!temp->right && !temp->left)
		{
			if(side)
				above->left = NULL;
			else
				above->right = NULL;

			//delete the node
			delete temp;
		}
		//if the node has a left but not a right, move the left-side branch up to the node's spot
		else if(!temp->right && temp->left)
		{
			if(side)
				above->left = temp->left;
			else
				above->right = temp->left;

			//delete the node
			delete temp;
		}
		//if the node has a right but not a left, move the right-side branch up to the node's spot
		else if(temp->right && !temp->left)
		{
			if(side)
				above->left = temp->right;
			else
				above->right = temp->right;

			//delete the node
			delete temp;
		}
		//if the node has both a right and a left
		else
		{
			//find the next smallest number
			Node* replace = temp->left;
			Node* upReplace = temp;
			while(replace->right)
			{
				upReplace = replace;
				replace = replace->right;
			}

			//remove the next smallest number from its position in the tree
			if(upReplace == temp)
				upReplace->left = replace->left;
			else
				upReplace->right = replace->left;

			//replace the node with the next smallest number
			temp->num = replace->num;
			delete replace;
		}
    }
    //if the number wasn't found, complain
    else
    {
		cout << "That number is not in the tree.\n";
    }
}

int main()
{
    Node* tree = NULL;
    bool fileFound = true;
    bool running = true;

    while(running)
    {
		cout << "Would you like to INPUT numbers, REMOVE numbers, or QUIT?\n";
		//thanks to myself for writing the input code in heap
		char* input = new char[8];
		cin >> input;
		
		if(!strcmp(input, "INPUT") || !strcmp(input, "input"))
		{
			//prompt the user for how numbers will be entered
			cout << "Would you like to enter numbers via MANUAL input or from FILE? \n";
			cin >> input;
			//if the user wants to enter numbers manually
			if(!strcmp(input, "MANUAL") || !strcmp(input, "manual"))
			{
				//add numbers one by one until the user says they are done
				cout << "Enter integers from 1-1000 one at a time.\nEnter DONE when you are done adding numbers \n";
				cin >> input;
				if(isdigit(input[0]) && !tree)
				{
					tree = new Node(atoi(input));
					cin >> input;
				}
				while(strcmp(input, "DONE") && strcmp(input, "done"))
				{
					int toAdd = atoi(input);
					add(tree, toAdd);
					cin >> input;
				}
			}
			//if the user wants to enter numbers from a file
			else if(!strcmp(input, "FILE") || !strcmp(input, "file"))
			{
				char* file = new char[32];
				char* list = new char[512];

				//prompt the user for the name of the file
				cout << "\nEnter the name of the file.\n";
				cin.get();
				cin.get(file, 32);
				cin.get();

				//open the file
				ifstream stream(file);
				if(stream.is_open())
				{
					//extract the text from the file
					stream.getline(list, 512);
					stream.close();
					cout << endl << list << endl;
					//extract the numbers from the text
					for(int i = 0; i < strlen(list); i++)
					{
						if(isdigit(list[i]))
						{
							//isolate the number into a separate char pointer
							int start = i;
							char* number = new char[5];
							do number[i-start] = list[i];
								while(list[++i] != ' ' && i < strlen(list));
							number[i-start] = '\0';

							//convert the isolated char pointer into an integer and add it to the tree
							if(!tree)
							{
								tree = new Node(atoi(number));
							}
							else
							{
								int toAdd = atoi(number);
								add(tree, toAdd);
							}
						}
					}
				}
				else
				{
					cout << "File not found\n";
					fileFound = false;
				}
			}
			//if the user entered neither option, throw an error.
			else
			{
				cout << "Sorry I couldn't understand that.\n";
				fileFound = false;
			}
		}
		else if(!strcmp(input, "REMOVE") || !strcmp(input, "remove"))
		{
			//if the tree exists
			if(fileFound)
			{
				//print it out
				visualize(tree);

				//if the user wants to delete, delete
				while(strcmp(input, "DONE") && strcmp(input, "done"))
				{
					//ask the user for the numbers to delete
					cout << "Enter numbers to remove one at a time.\nEnter DONE when you are done removing numbers.\n";
					cin >> input;
					int toRemove = atoi(input);
					cout << endl;

					//remove the number from the tree
					remove(tree, toRemove);
					
					//if the tree is empty, end
					if(!tree || tree->num == 0)
					{
						tree = NULL;
						break;
					}
					else
						visualize(tree);
				}
			}
		}
		else if(!strcmp(input, "QUIT") || !strcmp(input, "quit"))
		{
			running = false;
		}
    }
    return 0;
}

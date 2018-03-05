#include <iostream>
#include <cstring>
#include <stdlib.h>
#include <fstream>

using namespace std;

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
//print out the tree in a simple form
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

void add(Node* head, int toAdd)
{
	if(toAdd > head->num)
	{
		if(head->right)
		{
			add(head->right, toAdd);
		}
		else
		{
			head->right = new Node(toAdd);
		}
	}
	else
	{
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

Node* find(Node* head, int toRemove, Node* prev = NULL)
{
	if(toRemove > head->num)
	{
		if(head->right)
		{
			return find(head->right, toRemove, head);
		}
		else
		{
			return NULL;
		}
	}
	else if(toRemove == head->num)
	{
		return prev;
	}
	else if(toRemove < head->num)
	{
		if(head->left)
		{
			return find(head->left, toRemove, head);
		}
		else 
		{
			return NULL;
		}
	}
}

void remove(Node* head, int toRemove)
{
	if(toRemove == head->num)
	{
		if(!head->left && !head->right)
		{
			delete head;
			cout << "The tree is now empty.\n";
		}
		else if(!head->left && head->right)
		{
			Node* temp = head->right;
			head->num = temp->num;
			head->right = temp->right;
			head->left = temp->left;
			delete temp;
		}
		else if(head->left && !head->right)
		{
			Node* temp = head->right;
			head->num = temp->num;
			head->right = temp->right;
			head->left = temp->left;
			delete temp;
		}
		else
		{
			Node* replace = head->left;
			Node* upReplace = head;
			while(replace->right)
			{
				upReplace = replace;
				replace = replace->right;
			}
			
			upReplace->right = replace->left;
			head->num = replace->num;
			delete replace;
		}
		
		return;
	}
	
	Node* above = find(head, toRemove);
	if(above)
	{
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
		
		if(!temp->right && !temp->left)
		{
			if(side)
			{
				above->left = NULL;
			}
			else
			{
				above->right = NULL;
			}
			delete temp;
		}
		else if(!temp->right && temp->left)
		{
			if(side)
			{
				above->left = temp->left;
			}
			else
			{
				above->right = temp->left;
			}
			delete temp;
		}
		else if(temp->right && !temp->left)
		{
			if(side)
			{
				above->left = temp->right;
			}
			else
			{
				above->right = temp->right;
			}
			delete temp;
		}
		else
		{
			Node* replace = temp->left;
			Node* upReplace = temp;
			while(replace->right)
			{
				upReplace = replace;
				replace = replace->right;
			}
			
			cout << "upreplace: " << upReplace->num << "\nreplace: " << replace->num << endl;
			
			if(upReplace == temp)
			{
				cout << "hello I am here\n";
				upReplace->left = replace->left;
			}
			else
			{
				upReplace->right = replace->left;
			}
			temp->num = replace->num;
			delete replace;
		}
	}
	else
	{
		cout << "That number is not in the tree.\n";
	}
}

int main()
{
	Node* tree = NULL;
	bool fileFound = true;
	
	//thanks to myself for writing the input code in heap
	//prompt the user for how numbers will be entered
	cout << "Would you like to enter numbers via MANUAL input or from FILE? \n";
	char* input = new char[8];
	cin >> input;
	
	//if the user wants to enter numbers manually
	if(!strcmp(input, "MANUAL") || !strcmp(input, "manual"))
	{
		//add numbers one by one until the user says they are done
		cout << "Enter integers from 1-1000 one at a time. Enter DONE when you are done adding numbers \n";
		cin >> input;
		if(isdigit(input[0]))
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
	
	if(fileFound)
	{
		visualize(tree);
		bool deleting = true;
		while(deleting)
		{
			cout << "Would you like to remove a number from the tree? (y/n)\n";
			cin >> input;
			if(!strcmp(input, "y") || !strcmp(input, "Y")
				|| !strcmp(input, "Yes") || !strcmp(input, "yes") || !strcmp(input, "YES"))
			{
				cout << "What number would you like to remove?\n";
				int toRemove = -1;
				cin >> toRemove;
				
				remove(tree, toRemove);
				
				if(!tree)
					deleting = false;
				
				visualize(tree);
			}
			else if(!strcmp(input, "n") || !strcmp(input, "N")
				|| !strcmp(input, "No") || !strcmp(input, "no") || !strcmp(input, "NO"))
			{
				deleting = false;
			}
		}
	}
    return 0;
}

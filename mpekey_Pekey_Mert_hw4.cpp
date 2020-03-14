#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "dynintstack.h"
using namespace std;

int main()
{
	int lineamount=0, spacecounter, currentlvl = 0, temp;
	string filename, a, line, word;
	bool keyfound=false, iserror=false;
	ifstream pyfile;
	DynIntStack myStack;

	cout<<"Please enter the file name that is going to be analyzed."<<endl;
	cin>>filename;

	pyfile.open(filename);	//open the file
	while (pyfile.fail())	// checking whether it opens or not
	{
		cout<<"Unable to open file please enter a different file name."<<endl;
		cin>>filename;
		pyfile.clear(); 
		pyfile.open(filename);
	}

	cout<<"Starting file analysis..."<<endl;
	myStack.push(0);
	cout<<"Initial indentation level is pushed to the stack as 0."<<endl;

	while (!pyfile.eof())	//getting amount of line in the file
	{
		getline(pyfile,a);
		lineamount++;
	}

	pyfile.clear();
	pyfile.seekg(0);	//to go back to the beginning of the line

	int i=0, j;
	while (i<lineamount)
	{
		j=0;
		cout<<endl<<"Line number: "<<i+1<<endl;
		getline(pyfile,line);
		cout<<"Line: "<<line<<endl;

		spacecounter = 0;
		while (line[j]==' ') //getting the number of indentation level with getting number of space at beginning of the line
		{
			spacecounter++;
			j++;
		}

		cout<<spacecounter<<" number of spaces observed before the start of the line."<<endl;
		if(keyfound==true)
			cout<<"This line proceeds a keyword containing line."<<endl;	//If we found one of the keywords such as while and if at the previous line, this line runs
		cout<<"Current Level = "<<currentlvl<<" This Line = "<<spacecounter<<endl;

		if (keyfound==true)	//if we found the keywords we check for their conditions to decide if it obeys the rules of indentation
		{
			if (currentlvl >= spacecounter)
			{
				cout<<"Incorrect file structure."<<endl;
				cout<<"Current line must be greater than the Current indentation level."<<endl;
				cout<<"Stopping file analysis..."<<endl<<endl;
				iserror=true;
				break;
			}
			else
			{
				myStack.push(spacecounter);	//push new number of space to the stack to have the new indentation level
				cout<<"Line correct. Depth "<<spacecounter<<" added to the stack."<<endl;
				currentlvl = spacecounter;
			}
		}
		else
		{
			if (currentlvl == spacecounter)
			{
				cout<<"Line belongs to current block."<<endl;
			}
			else if (currentlvl > spacecounter)
			{
				cout<<"Current line is smaller than Current indentation level; checking if line belongs to outer indentation."<<endl;
				myStack.pop(currentlvl);	// to delete the top of the stack
				while (!myStack.isEmpty())
				{				
					myStack.pop(currentlvl);	//to get the 2nd top of the stack with deleting it and assign it to currentlvl variable then we push it back when we are done with its value if the level is equal to new space amount

					if (currentlvl == spacecounter)
					{
						cout<<"Current Level = "<<currentlvl<<" This Line = "<<spacecounter<<endl;
						cout<<"Line belongs to outer block."<<endl;
						myStack.push(currentlvl);
						break;
					}
					else if (currentlvl < spacecounter)	//it breaks the rule of indentation
					{
						cout<<"Current Level = "<<currentlvl<<" This Line = "<<spacecounter<<endl;
						cout<<"Incorrect file structure."<<endl;
						cout<<"An indentation level same as the Current line is not found in outer blocks."<<endl;
						cout<<"Stopping file analysis..."<<endl<<endl;
						iserror=true;
						break;
					}
					else
					{
						cout<<"Current Level = "<<currentlvl<<" This Line = "<<spacecounter<<endl;
						cout<<"Current line is smaller than Current indentation level; checking if line belongs to outer indentation."<<endl;
					}
				}
			}
			else
			{
				cout<<"Incorrect file structure."<<endl;
				cout<<"Current line cannot be greater than the Current indentation level."<<endl;
				cout<<"Stopping file analysis..."<<endl<<endl;
				iserror=true;
				break;
			}
		}

		if (iserror == true)	//If there is an error about the indentation, we finish the while loop and end the program
			break;

		keyfound=false;
		istringstream iss;
		iss.str(line);

		while(iss>>word)		//Checking if we have such keywords in the line with getting the words one by one with iss and if we found, make keyfound true to decide other conditions for the next line of the program.
		{
			if (word == "while")
			{
				cout<<"Keyword while found on this line"<<endl;
				keyfound=true;
			}
			else if (word == "elif")
			{
				cout<<"Keyword elif found on this line"<<endl;
				keyfound=true;
			}
			else if (word == "else")
			{
				cout<<"Keyword else found on this line"<<endl;
				keyfound=true;
			}
			else if (word == "if")
			{
				cout<<"Keyword if found on this line"<<endl;
				keyfound=true;
			}
			else if (word == "for")
			{
				cout<<"Keyword for found on this line"<<endl;
				keyfound=true;
			}
		}

		i++;
	}

	if(iserror==false)
		cout<<endl<<"Finished file analysis. File structure is correct!"<<endl;

	while (!myStack.isEmpty())	//Clearing the stack
	{
		myStack.pop(temp);
	}
	cout<<"Stack emptied and program ending."<<endl;
	

	return 0;
}
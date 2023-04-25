#include <stdio.h>
#include <string.h>

//in the main method it allows for arguments to be passed from the command line
int main(int argc, char *argv[])
{
	int error = 0;
	
	//checks if the number of words does not exceed 6
	if (argc-1 > 6)
	{
		error = 1;
	}
	
	//checks if each word is no more than 10 characters long
	for(int i=1; i<argc; i++)
	{
		if (strlen(argv[i]) > 10)
		{
			error = 1;
		}
	}
	
	//if there are no errors reverse sentence
	if (error == 0)
	{
		for(int j=(argc-1); j>1; j--)
		{
			strcat(argv[argc-1], " ");
			strcat(argv[argc-1], argv[j-1]);
		}
		printf("%s\n", argv[argc-1]);
		return 0;
	}
	//outputs error if there has been an error
	else
	{
		printf("Error\n");
	}
}
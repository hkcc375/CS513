/* Question : Create the following types of a files using (i) shell command (ii) system call 
		a. soft link (symlink system call)
		b. hard link (link system call)
		c. FIFO (mkfifo Library Function or mknod system call)
*/

// The command to create a soft link using shell command is ln -s <path to file/folder to be linked> <path of the link to be created>

#include<unistd.h>
#include<fcntl.h>
#include<stdio.h>

#define RD_WR 0666

int main(int argc, char* argv[])
{
	char* fileName;
	int fileDescriptor;
	if(argc != 2)
		printf("Pass the filename as a command line argument. \n");
	else
	{
		fileName = argv[1];
		fileDescriptor = creat(fileName, RD_WR);
		if(fileDescriptor == -1)
			perror("Error while creating the file ! \n");
		else
		{
			printf("The file was created successfully. \n");
			if(symlink(fileName, "symbolic_link_file") < 0)
				perror("There was an error while creating a symbolic link. \n");
			else
				printf("The symbolic link was created successfully. \n");
		}
	}
}


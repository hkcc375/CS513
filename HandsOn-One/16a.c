/* Question : Write a program to perform mandatory locking. 
	a. Implement write lock
	b. Implement read lock
*/

#include<fcntl.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>

#define RD_WR 0666

int main(int argc, char* argv[])
{
	char* fileName;
	int fileDescriptor;
	if(argc != 2)
		printf("Pass the filename as a command line argument. \n");
	else
	{
		char* fileName = argv[1];
		int fileDescriptor;
		struct flock lock, lockStat;

		lock.l_type = F_WRLCK;
		lock.l_whence = SEEK_SET;
		lock.l_start = 0;
		lock.l_len = 0;
		lock.l_pid = getpid();
			
		printf("I am trying to open the file. \n");
		fileDescriptor = open(fileName, O_RDWR);
		if(fileDescriptor == -1)
			perror("Error while opening a file. \n");
		else
		{
			
		}					
	}
}

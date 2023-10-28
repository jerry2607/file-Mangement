#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include<string.h>
#include <stdlib.h>  // exit()


#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define YELLOW "\033[1;33m"
#define RESET "\033[0m"
#define BOLD "\x1B[1m"

void printHelp(){
	// fnction to print help file
    int fd = open("help.txt",O_RDONLY);
    char buffer[1];
    while(read(fd,buffer,1)>0){
        printf("%s",buffer);
    }
    close(fd);
}
int converter(char * n)
{
	// function to convert string to corrosponding octal representation in int
	int ind=0,arrInd=0;
	int arr[]={0,0,0,0,0,0,0,0,0}; //  bcz permission can have atmost 9 bits
	while(n[ind]!='\0')
	{
			if(ind>=3)
			{
				printf("invalid permissions\n");  // validating condition
				exit(0);
			}
		int val=n[ind]-'0';
		if((val<0)||(val>7))   // validating condition
		{
				printf("invalid permissions\n");
				exit(0);
		}
		for(int i=2;i>=0;i--)
		{
			if(val&(1<<i))
			{
				arr[arrInd]=1;    // converting a number to 3 bit representation
			}
			arrInd++;
		}
		ind++;

	}

	int ans=0;
	for(int i=0;i<9;i++)
	{
		if(arr[i])               // converting array to int
		ans+=((1<<(8-i)));
	}
	return ans;
}


void createFile(char *path, int perm)
{
	int fd=access(path,F_OK);  // checking if file already exist
	if(fd==0)
	{
		printf("error :/ file with specified name already exist do you want to truncate it\nenter y for yes ,anyother key for no\n");
		char decide;
		scanf("%c",&decide);
		if(decide!='y')
			exit(0);
		  
	}
	
	
	
	fd=creat(path,perm);  // creating using given permission
	
	
	if(fd<0)
	{
		printf("Error :/ permission denied\n"); // error case
		return;
	}

	printf("file created successfully........\n");
	close(fd);	 // closing file
}

void readFile(char *path,int skip,int amount)
{
	
		if((skip<0)||(amount))
		{
		             printf("\n %s********* Invalid Integer *********%s \n\n",RED,RESET);
            exit(0);
	    }
	int fd=access(path,F_OK);  // checking if file already exist
	if(fd!=0)
	{ 
		// if file does'nt exist
		printf("ERROR:/file does not exist....\naborting operation.....\n");
		exit(0);
	}
	
     fd=open(path,O_RDONLY); // opening in readinly mode 

	
	if(fd<0)
	{
	   printf("ERROR :/ reading Permission denied \n");
	   exit(0);
	}
	
	char c;

	
	lseek(fd,skip,SEEK_SET);             // seeking the given skip bytes
	
	int temp=amount;
	while((amount)&&(read(fd,&c,1)==1))
	{
		amount--;       
		printf("%c",c);  // printing char by char the given amount
	}
	if(amount>0)
	{
		if(temp==amount)
		printf("file exhausted before reading\n");  // if file exhausted before reading
		else	
		printf("\n%d bytes read successfully but file doesn't have data to read anymore:/\n",temp-amount);  // if file exhausted in between
	}
	
	
	close(fd);

}




void writeFile(char *path,int skip)
{
	if(skip<0){
		             printf("\n %s********* Invalid Integer *********%s \n\n",RED,RESET);
                     exit(0);

	        }
		int fd=access(path,F_OK);  // checking if file already exist
	if(fd==0)
	{
		   // if file already exist
			   printf("This may overwite the file. Do you want to contine?(press y for yes \n any other key for no )\n");
			   
              char decide;
                fflush(stdin);
              scanf("%c",&decide);
                     
                     
              if(decide!='y')
              {
              	printf("aborting......");
              	return;
              }
	}


	 fd=open(path,O_CREAT|O_RDWR,0776);   // opening in read-write mode

	
	if(fd<0)
	{
	   printf("ERROR :/ while opening the given file\n");
	   return;
	}
	

	
	//lseek(fd,skip,0); // start from here
	char temp;
while((skip)&&(read(fd,&temp,1)==1))
{
	skip--;  // skipping the given skip bits
}

if(skip>0)  
{
	 // if end of file reached
	        printf("\n ********* %sWARNING%s Cursor go beyond end of file ********* \n\n",RED,RESET);
           printf("\n %sDo you want to enter data at the end : [y for yes/any other key for no]%s\t",BOLD,RESET);
                while(getchar()!='\n');  
          char decide;
           //fflush(stdin);             
            scanf("%c",&decide);
            if(decide!='y'){
               exit(0);
            }
}
	char c[1024];
    printf("\n %sENTER YOUR DATA BELOW : %s\n",YELLOW,RESET);
	int succ=read(0,c,1024);
	

	int done=write(fd,c,succ-1);  // leaving one last char (i.e. \n)
	
	if(done==-1){
		printf("ERROR occured while writing\n");
		return;
	}
	
	printf("writing done successfully\n");
	
	close(fd);
}




void copyFile(char *source, char *target)
{
	if(access(source,F_OK)!=0) // checking if source file exist or not
	{
	      printf("\n %s********* File %s %s %s Doesn't Exists *********%s \n\n",RED,RESET,source,RED,RESET);
		exit(0);
	}
	
	
	if(access(target,F_OK)==0)  // checking if target file exist or not
	{
		printf("%swarning %s: file already exist by copying  previous data will get lost(%s)[y/n]...\n",RED,RESET,target);
		char c;
		scanf("%c",&c);
		if(c!='y')
		exit(0);
	}
	else
	{
		printf("\n %s********* File %s %s %s Doesn't Exists *********%s \n\n",RED,RESET,target,RED,RESET);
        // file not exists , file created or not
        printf("\n Do you want to create new file and copy the data : %s[y for yes /any other key for no]%s\t",BOLD,RESET);
        char decide;
        scanf("%c",&decide);
        if(decide!='y')
        	exit(0);
	}
	int rfd=open(source,O_RDONLY),wfd;   // opening source in read mode
	if(rfd<0)
	{
	   printf("ERROR :/ reading Permission denied \n");
	   exit(0);
	}
	
	wfd=open(target,O_CREAT|O_TRUNC|O_WRONLY,0776);  // opening file to copy data
	
		if(wfd<0)
	{
	   printf("ERROR :/ Permission denied \n");
	   exit(0);
	}
	
	
	char c; 
	while(read(rfd,&c,1)==1)  // reading from source char by char
	{
		if(write(wfd,&c,1)<0)  // writing at target char by char
		{
			printf("error occured while writing to %s file",target);
			exit(0);
		}
	}
	
	close(rfd);
	close(wfd);
  printf("\n\n %s********* FILE COPIED successfully *********%s \n\n",GREEN,RESET);
	
}


void appendFile(char* fileName){
    // This function appending the file 
   if(access(fileName,F_OK)!=0){
        printf("\n %s********* File %s %s %s Doesn't Exists *********%s \n\n",RED,RESET,fileName,RED,RESET);
        exit(0);
    }
    int fd = open(fileName,O_APPEND|O_RDWR);
    char buffer[512];
    // reading the input from terminal
    printf("\n %sENTER YOUR DATA BELOW : %s\n",YELLOW,RESET);
    int amountRead = read(0,buffer,512);
    // appending to the file
    write(fd,buffer,amountRead);
    close(fd);
}


void showStat(char* fileName){
    // this function show the statistical information of file (fileName)
if(access(fileName,F_OK)!=0)
{
	printf("file doesn't exist\n");
	exit(0);
}
    struct stat file_stat;
    int result = stat(fileName, &file_stat);

    // permission issue
    if (result != 0) {
        printf("Error: Failed to retrieve file information\n");
        exit(0);
    }

    printf("%sFile Information:%s\n",BOLD,RESET);
    printf("%sInode number \t\t:\t %s %ld\n",YELLOW,RESET, file_stat.st_ino);
    printf("File type and mode \t:\t %o\n", file_stat.st_mode);
    printf("%sFile type \t\t:\t %s",YELLOW,RESET);
    printf( (S_ISDIR(file_stat.st_mode)) ? "directory\n" : ((S_ISREG(file_stat.st_mode)) ? "Regular file\n" : "other\n"));
    printf("%sFile Permissions \t:\t%s ",YELLOW,RESET);
    printf( (file_stat.st_mode & S_IRUSR) ? "r" : "-");
    printf( (file_stat.st_mode & S_IWUSR) ? "w" : "-");
    printf( (file_stat.st_mode & S_IXUSR) ? "x" : "-");
    printf( (file_stat.st_mode & S_IRGRP) ? "r" : "-");
    printf( (file_stat.st_mode & S_IWGRP) ? "w" : "-");
    printf( (file_stat.st_mode & S_IXGRP) ? "x" : "-");
    printf( (file_stat.st_mode & S_IROTH) ? "r" : "-");
    printf( (file_stat.st_mode & S_IWOTH) ? "w" : "-");
    printf( (file_stat.st_mode & S_IXOTH) ? "x" : "-");
    printf("\nNumber of hard links \t:\t %ld\n", file_stat.st_nlink);
    printf("%sUser ID of owner \t:\t %s %d\n",YELLOW,RESET, file_stat.st_uid);
    printf("Group ID of owner \t:\t %d\n", file_stat.st_gid);
    printf("Total size, in bytes \t:\t %ld\n", file_stat.st_size);
    printf("Last access time \t:\t %ld\n", file_stat.st_atime);
    printf("Last modification time \t:\t %ld\n", file_stat.st_mtime);
    printf("Last status change time :\t %ld\n", file_stat.st_ctime);

}



int main(int argc,char *argv[])
{
    if(argc<2){
        printf("\n Enter -h for help \n\n");
    }
    else if(argc==2){
        if(strcmp(argv[1],"-h")==0){
            printHelp();
        }
        else{
            printf("\n Enter -h for help \n\n");
        }
    }
    else if(argc==3){
        char per[] = "776";
        if(strcmp(argv[1],"-c")==0){
            createFile(argv[2],converter(per));
        }
        else if(strcmp(argv[1],"-r")==0){
            readFile(argv[2],0,0);
        }
        else if(strcmp(argv[1],"-w")==0){
            writeFile(argv[2],0);
        }
        else if(strcmp(argv[1],"-a")==0){
            appendFile(argv[2]);
        }
        else if(strcmp(argv[1],"-s")==0){
            showStat(argv[2]);
        }
        else{
            printf("\n Enter -h for help \n\n");
        }
    }
    else if(argc==4){
        char off[] = "0";
        
        if(strcmp(argv[1],"-c")==0){
            createFile(argv[2],converter(argv[3]));
        }
        else if(strcmp(argv[1],"-r")==0){
            readFile(argv[2],0,atoi(argv[3]));
        }
        else if(strcmp(argv[1],"-w")==0){
            writeFile(argv[2],atoi(argv[3]));
        }
        else if(strcmp(argv[1],"-cp")==0){
            copyFile(argv[2],argv[3]);
        }
        else{
            printf("\n Enter -h for help \n\n");
        }
    }
    else if(argc==5){
        if(strcmp(argv[1],"-r")==0){
            readFile(argv[2],atoi(argv[3]),atoi(argv[4]));
        }
        else{
            printf("\n Enter -h for help \n\n");
        }
    }
    else{
        printf("\n Enter -h for help \n\n");
    }
    return 0;
}

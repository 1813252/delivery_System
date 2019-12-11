#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_MSG_SIZE			10
#define PASSWD_LEN				4
#define N_BUILDING				10
#define N_FLOOR					10
#define N_ROOM					10

#define STORAGE_FILEPATH 	"storage2.txt"

typedef struct {
	int building;
	int room;
	int cnt;
	char passwd[PASSWD_LEN+1];
	
	char *context;
} storage_t;

static storage_t** deliverySystem; 	
static int storedCnt =0;

void str_printStorageStatus(void) {
	
	int i, j;
	printf("----------------------------- Delivery Storage System Status (%i occupied out of %i )-----------------------------\n\n", storedCnt,24);
	
	printf("\t");
	for (j=0;j<6;j++)
	{
		printf(" %i\t\t",j);
	}
	printf("\n-----------------------------------------------------------------------------------------------------------------\n");
	
	for (i=0;i<4;i++)
	{
		printf("%i|\t",i);
		for (j=0;j<6;j++)
		{
			if (deliverySystem[i][j].cnt > 0)
			{
				printf("%i,%i\t|\t", deliverySystem[i][j].building, deliverySystem[i][j].room);
			}
			else
			{
				printf(" -  \t|\t");
			}
		}
		printf("\n");
	}
	printf("--------------------------------------- Delivery Storage System Status --------------------------------------------\n\n");
}

int str_createSystem(char *filepath){
	
	FILE *fp;
	
	int i,j=0;
 	
	deliverySystem = (storage_t**)malloc(sizeof(storage_t*)*4); 

		for(i=0;i<4;i++)
		{
			
			deliverySystem[i] = (storage_t*)malloc(sizeof(storage_t)*6); 
		
		}
			
		fp= fopen(filepath,"r");		
		
		int line=0;
		char c;
		
		while ((c=fgetc(fp)) !=EOF)
		{
			if (c=='\n')
			
				line++;
		}
		
		
		fclose(fp);
		
		fp= fopen(filepath,"r");		
	
		for(i=0;i<4;i++)
		{
			for(j=0;j<6;j++)
			deliverySystem[i][j].cnt=0;
		}
		
		int k;
			
		for(k=0;k<line;k++)
		{
			fscanf(fp,"%i %i",&i ,&j);
			deliverySystem[i][j].context= (char*)malloc(sizeof(char)*100);
			fscanf(fp," %i %i %s %s", &deliverySystem[i][j].building, &deliverySystem[i][j].room, deliverySystem[i][j].passwd, deliverySystem[i][j].context);
			deliverySystem[i][j].cnt++;
			storedCnt++;
		}
		
		
		fclose(fp);

		
	return 0;
	
}

//put a package (msg) to the cell
//input parameters
//int x, int y : coordinate of the cell to put the package
//int nBuilding, int nRoom : building and room numbers of the destination
//char msg[] : package context (message string)
//char passwd[] : password string (4 characters)
//return : 0 - successfully put the package, -1 - failed to put
int str_pushToStorage(int x, int y, int nBuilding, int nRoom, char msg[MAX_MSG_SIZE+1], char passwd[PASSWD_LEN+1]) {
	

	deliverySystem[x][y].context= (char*)malloc(sizeof(char)*strlen(msg));
	
	deliverySystem[x][y].building=nBuilding;
	deliverySystem[x][y].room = nRoom;
	strcpy(deliverySystem[x][y].passwd,passwd);
	strcpy(deliverySystem[x][y].context, msg);
	deliverySystem[x][y].cnt++;
	
	if(deliverySystem[x][y].cnt!=1)
	{
		return -1;
	}
	
	else 
	return 0;

}

//free the memory of the deliverySystem 
void str_freeSystem(void) {
	
	int i;
	
	for(i=0;i<4;i++)
	{

		free(deliverySystem[i]);
			
	}
		
		free(deliverySystem);
	
}


//check if the input cell (x,y) is valid and whether it is occupied or not
int str_checkStorage(int x, int y) {
	
	if (x < 0 || x >= 4)
	{
		return -1;
	}
	
	if (y < 0 || y >= 6)
	{
		return -1;
	}
	
	return deliverySystem[x][y].cnt;	
}


//get password input and check if it is correct for the cell (x,y)
//int x, int y : cell for password check
//return : 0 - password is matching, -1 - password is not matching
static int inputPasswd(int x, int y) {
	
	char input_passwd[PASSWD_LEN+1];
	printf("input password for (%i,%i) storage : \n",x,y);
	scanf("%s",input_passwd);
	
	if (strcmp(input_passwd,deliverySystem[x][y].passwd)==0) //password is matching 
	{
		return 0;
	}
	
	else //not matching 
	
	return -1;
	
}

//initialize the storage
//set all the member variable as an initial value
//int x, int y : cell coordinate to be initialized
static void initStorage(int x, int y) {
	
	
	deliverySystem[x][y].building=NULL;
	deliverySystem[x][y].room=NULL;
	deliverySystem[x][y].cnt = 0;
	deliverySystem[x][y].passwd[PASSWD_LEN+1]=NULL;
	
	free(deliverySystem[x][y].context); ///free memory	
	
}




//extract the package context with password checking
//after password checking, then put the msg string on the screen and re-initialize the storage
//int x, int y : coordinate of the cell to extract
//return : 0 - successfully extracted, -1 = failed to extract
int str_extractStorage(int x, int y) {
	
	
	int check_passwd=inputPasswd(x,y);
	
	if (check_passwd!=0)
	{
		printf("password is wrong!!\n");
		return -1;
	}
	
	else if (check_passwd==0)
	{
		
			
		printf("---------->extracting the storage (%i,%i)...",x,y);
		printf("\n------------------------------------------------\n");
		printf("<<<<<<<<<<<<<<< : %s>>>>>>>>>>>>>>>>>>>",deliverySystem[x][y].context);
		printf("\n------------------------------------------------\n");
		
		initStorage(x,y);
				
		return 0;
	}
	
	
}

//get an integer value from keyboard
int getIntegerInput(void)
{
	int input = -1;
	scanf("%d", &input);
	fflush(stdin);
	
	return input;
}



//check whether building/room number is valid or not
int buildingValidityCheck(int nBuilding, int nRoom)
{
	if (nBuilding <= 0 || nBuilding > N_BUILDING)
	{
		return -1;
	}
	
	if (nRoom/100 <= 0 || nRoom/100 > N_FLOOR)
	{
		return -1;
	}
	
	if (nRoom%100 <= 0 || nRoom%100 > N_ROOM)
	{
		return -1;
	}
	
	return 0;
}

int main(int argc, char *argv[]) {
	
	int cmd;
	int x, y, nBuilding, nRoom;
	char msg[MAX_MSG_SIZE+1];
	char passwd[PASSWD_LEN+1];
	
	//1. initialize the delivery system
	if (str_createSystem(STORAGE_FILEPATH) != 0)
	{
		printf("failed to open storage config file! (%s)\n", STORAGE_FILEPATH);
		return -1;
	}
	
	printf("------- Unmanned Delivery Storage System START -------\n");
	
	do
	{
        //2. menu printing
		printf("\n\n");
		printf("----------------------------------------------\n");
		printf("1. put a package\n");
		printf("2. get my package\n");
		printf("3. check the storage status\n");
		printf("4. find my package\n");
		printf("0. exit\n");
		printf("----------------------------------------------\n");
		printf("command : ");
		
        //3. getting user command
		cmd = getIntegerInput();	
		
		switch(cmd)
		{
            //4-1. command analysis : exit the program
			case 0:
				printf(" -----------> exiting the program....\n");
				break;
			
            //4-2. command analysis : put package
			case 1:
                //provide the current status
				str_printStorageStatus();
				
                //storage cell selection
				printf(" - storage row : ");
				x = getIntegerInput();
				printf(" - storage column : ");
				y = getIntegerInput();
				if (str_checkStorage(x,y) != 0)
				{
					printf(" -----------> Storage (%i,%i) is already occupied or invalid!\n", x, y);
					continue;
				}
				
                //destination input
				printf(" - building # : ");
				nBuilding = getIntegerInput();
				printf(" - room # : ");
				nRoom = getIntegerInput();
				if (buildingValidityCheck(nBuilding, nRoom) != 0)
				{
					printf(" -----------> Invalid building/room number (%i,%i)\n", nBuilding, nRoom);
					continue;
				}
				
                //package context input (message string)
				printf(" - message (max : 100 characters) : ");
				scanf("%100s", msg);
				fflush(stdin);
				
                //password input
				passwd[0] = 0;
				printf(" - password : ");
				scanf("%4s", passwd);
				fflush(stdin);
				
                //put the package in the cell
				printf(" -----------> processing to put the package...\n\n");
				if (str_pushToStorage(x, y, nBuilding, nRoom, msg, passwd) == 0)
				{
					printf(" -----------> Succeeded to put package for (%i, %i) user in (%i,%i), msg : %s passwd : %s\n", nBuilding, nRoom, x, y, msg, passwd);
				}
				else
				{
					printf(" -----------> Failed to put the package in the storage!\n");
				}
				break;
				
				//4-3. command analysis : extract my package
			case 2:
                //input the storage cell
				printf(" - storage row : ");
				x = getIntegerInput();
				printf(" - storage column : ");
				y = getIntegerInput();
				
                //check the input storage cell
				if (str_checkStorage(x,y) <= 0)
				{
					printf(" -----------> Storage (%i,%i) is empty or invalid!\n", x, y);
					continue;
				}
                //try to extract the cell
				if (str_extractStorage(x,y) != 0)
				{
					printf(" -----------> Failed to extract my package!\n");
				}
				break;
				
			  //4-4. command analysis : print the storage status
			case 3:
				str_printStorageStatus();
				break;}
				
		} while (cmd!=0);
		
return 0;
}

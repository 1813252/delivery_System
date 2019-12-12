#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_MSG_SIZE			100
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
static int systemSize[2] = {0, 0};  
static char masterPassword[PASSWD_LEN+1];

void str_printStorageStatus(void) {
	
	int i, j;
	printf("----------------------------- Delivery Storage System Status (%i occupied out of %i )-----------------------------\n\n", storedCnt,systemSize[0]*systemSize[1]);
	
	printf("\t");
	for (j=0;j<systemSize[1];j++)
	{
		printf(" %i\t\t",j);
	}
	printf("\n-----------------------------------------------------------------------------------------------------------------\n");
	
	for (i=0;i<systemSize[0];i++)
	{
		printf("%i|\t",i);
		for (j=0;j<systemSize[1];j++)
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
 	
 	fp= fopen(filepath,"r");	//���� ����  
			
		
		int line=0; //�ؽ�Ʈ ���� �������� ������  
		char c;      
		
		if (fp==NULL)
		{
			return -1; //������ �� ������ �ű⼭ ��  
			
		}
		
		while ((c=fgetc(fp)) !=EOF) // ����  ������ �б�  
		{
			if (c=='\n') //�� �ٲ� �� ����  
			
				line++; //line ++ 
		}
			
		fclose(fp); //�� �о����� �ݾ���  
		
		fp= fopen(filepath,"r");	 //�ٽ� �����  
		
		fscanf(fp, "%d %d %s", &systemSize[0], &systemSize[1], masterPassword); // ��,��,������ ��й�ȣ �ޱ�  
 	
 	
		deliverySystem = (storage_t**)malloc(sizeof(storage_t*)*systemSize[0]);  //���������� deliverySystem �� deliverySystem[i] �� ����Ű���� ��// 

		for(i=0;i<systemSize[0];i++)
		{
			
			deliverySystem[i] = (storage_t*)malloc(sizeof(storage_t)*systemSize[1]); //deliverySystem[i]�� deliverySystem[i][0]�� ����ŵ�ϴ�//
		
		}
			
	
		for(i=0;i<4;i++)
		{
			for(j=0;j<6;j++)
			deliverySystem[i][j].cnt=0; //deliverySystem[i][j].cnt=0 ���� �ʱ�ȭ�Ͽ� ȭ�鿡 trash ���� ����� �ʵ��� �մϴ�//
		}
		
		
		int k; 
			
		for(k=2;k<line;k++) //3��° �ٺ��� ������ �ٱ��� �н��ϴ�
		{
			fscanf(fp,"%i %i",&i ,&j); //�տ� �� ������ i, j ���� �����մϴ�
			deliverySystem[i][j].context= (char*)malloc(sizeof(char)*100); //�ϴ� 100���ڷ� �޸� �Ҵ��� ���ݴϴ�//
			fscanf(fp," %i %i %s %s", &deliverySystem[i][j].building, &deliverySystem[i][j].room, deliverySystem[i][j].passwd, deliverySystem[i][j].context);
			// passwd�� context�� ���� �迭, ������ �̹Ƿ� �ּҿ����� &�� ������� �ʽ��ϴ�//
			
			deliverySystem[i][j].cnt++; //cnt=1�� ������־� ���α׷� ȭ�鿡 ���ϴ�//
			storedCnt++;//����� �ù���� ++ �˴ϴ�.
		}
		
		
		fclose(fp); //������ �ݽ��ϴ�.

		
	return 0; //�ý����� �� ��������� 0�� ��ȯ�մϴ� 
	
}

//put a package (msg) to the cell
//input parameters
//int x, int y : coordinate of the cell to put the package
//int nBuilding, int nRoom : building and room numbers of the destination
//char msg[] : package context (message string)
//char passwd[] : password string (4 characters)
//return : 0 - successfully put the package, -1 - failed to put
int str_pushToStorage(int x, int y, int nBuilding, int nRoom, char msg[MAX_MSG_SIZE+1], char passwd[PASSWD_LEN+1]) {
	

	deliverySystem[x][y].context= (char*)malloc(sizeof(char)*strlen(msg)); //���� message��ŭ context �����Ϳ� �޸𸮸� �Ҵ����ݴϴ�// 
	
	deliverySystem[x][y].building=nBuilding; 
	deliverySystem[x][y].room = nRoom;
	strcpy(deliverySystem[x][y].passwd,passwd);
	strcpy(deliverySystem[x][y].context, msg);
	deliverySystem[x][y].cnt++;
	
	if(deliverySystem[x][y].cnt!=1)
	{
		return -1; //�ù谡 �����Կ� �ȵ����� -1�� return �մϴ�// 
	}
	
	else //�����ϸ�  
	storedCnt++; //�ù�� ++ 
	str_backupSystem(STORAGE_FILEPATH);//�ؽ�Ʈ���Ͽ��� ���ݴϴ�// 
	
	return 0; 

}



// ------- API function for main.c file ---------------

//backup the delivery system context to the file system
//char* filepath : filepath and name to write
//return : 0 - backup was successfully done, -1 - failed to backup
int str_backupSystem(char* filepath) { 
	
	
	FILE *fp;
	int i,j;
	fp=fopen(filepath,"w"); //������� �����ݴϴ�(���� ���ݴϴ�)// 
	
	fprintf(fp,"%d %d\n%s\n", systemSize[0], systemSize[1], masterPassword); 
		
	for (i=0;i<systemSize[0];i++)
	{
		for(j=0;j<systemSize[1];j++)
		if (deliverySystem[i][j].cnt==1) //deliverySystem[i][j].cnt==1�� ��츸 ���� ���ݴϴ�// 
		{	
			fprintf(fp,"%i %i %i %i %s %s\n",i,j,deliverySystem[i][j].building, deliverySystem[i][j].room, deliverySystem[i][j].passwd, deliverySystem[i][j].context);		
		}
	}

	fclose(fp); //�ݾ��ִ°� �������ϴ�// 
	
	
	if(fp==NULL)
	{
		return -1;
	} 
	
	else 
	
	return 0;  
	
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




//initialize the storage
//set all the member variable as an initial value
//int x, int y : cell coordinate to be initialized

static void initStorage(int x, int y) { //extractStorage �Լ��� ���鶧 ����� ���Դϴ� 1// 
	
	
	deliverySystem[x][y].building=NULL; 
	deliverySystem[x][y].room=NULL;
	deliverySystem[x][y].cnt = 0;
	deliverySystem[x][y].passwd[PASSWD_LEN+1]=NULL;
	
	free(deliverySystem[x][y].context); ///free memory	
	
}
//get password input and check if it is correct for the cell (x,y)
//int x, int y : cell for password check
//return : 0 - password is matching, -1 - password is not matching

static int inputPasswd(int x, int y) { //extractStorage �Լ��� ���鶧 ����� ���Դϴ� 2// 
	
	char input_passwd[PASSWD_LEN+1]; //���ڿ��� �޽��ϴ�// 
	printf("input password for (%i,%i) storage : ",x,y);
	scanf("%s",input_passwd); 
	
	if (strcmp(input_passwd,deliverySystem[x][y].passwd)==0) //��й�ȣ�� ������// 
	{
		return 0; 
	}
	
	else if (strcmp(input_passwd,masterPassword)==0) //��й�ȣ�� �����ͺ�й�ȣ��// 
	{
		return 0;
	}
	
	else //Ʋ����//
	 
	return -1;
	
}


//extract the package context with password checking
//after password checking, then put the msg string on the screen and re-initialize the storage
//int x, int y : coordinate of the cell to extract
//return : 0 - successfully extracted, -1 = failed to extract
int str_extractStorage(int x, int y) {
	
	
	int check_passwd=inputPasswd(x,y); //�Ʊ�� ��й�ȣ Ȯ�����ִ� �Լ��� ����ϴ�// 
	
	if (check_passwd!=0) //Ʋ�� ��// 
	{
		printf("password is wrong!!\n");
		return -1;
	}
	
	else if (check_passwd==0) //�°ų� �����ͺ�й�ȣ �� ��// 
	{
		
			
		printf("---------->extracting the storage (%i,%i)...",x,y);
		printf("\n------------------------------------------------\n");
		printf("<<<<<<<<<<<<<<< : %s>>>>>>>>>>>>>>>>>>>",deliverySystem[x][y].context);
		printf("\n------------------------------------------------\n");
		
		initStorage(x,y); //�������� �����// 
		str_backupSystem(STORAGE_FILEPATH); //�ؽ�Ʈ���Ͽ��� ����// 
		storedCnt--; //������ �ù� -1// 
					
	}
	
	if (deliverySystem[x][y].cnt!=0) //������ �Ⱥ��������// 
	{
		return -1;
	}
	
	else
	return 0; 

}

//find my package from the storage
//print all the cells (x,y) which has my package
//int nBuilding, int nRoom : my building/room numbers
//return : number of packages that the storage system has
int str_findStorage(int nBuilding, int nRoom) {
	
	
	int cnt;
	int x,y;
	
		for (x=0;x<systemSize[0];x++)
		{
			for(y=0;y<systemSize[1];y++) //��� �������� �� Ȯ���մϴ�// 
			
			if(deliverySystem[x][y].building==nBuilding && deliverySystem[x][y].room==nRoom) //ã���ϴ�// 
			printf("Find a package in (%i,%i)\n",x,y); //�˷��ݴϴ�// 
			cnt++;
		}
		
	return cnt; //�� �ù谡  ������ cnt=0// 
}


//free the memory of the deliverySystem 
void str_freeSystem(void) {
	
	int i;
	
	for(i=0;i<systemSize[0];i++)
	{

		free(deliverySystem[i]);
			
	}
		
		free(deliverySystem); //�������� deliverySystem �� �Ҵ������ �޸𸮸� �������ݴϴ�// 
	
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
				break;
				
				//4-4. command analysis : find my package from the storage
			case 4:
                //input the destination (my address)
				printf(" - building # : ");
				nBuilding = getIntegerInput();
				printf(" - room # : ");
				nRoom = getIntegerInput();
                
				//check whether the address is valid or not
				if (buildingValidityCheck(nBuilding, nRoom) != 0)
				{
					printf(" -----------> Invalid building/room number (%i,%i)\n", nBuilding, nRoom);
					continue;
				}
				
                //try to find packages destinated to the address
				if (str_findStorage(nBuilding, nRoom) == 0)
				{
					printf(" -----------> Failed to find my package!\n");
				}
				
				break;
				
			default:
				printf(" -----------> Wrong cmd!! input again!..\n");
				break;
		}
		
				
		} while (cmd!=0);
		
		str_freeSystem();
	
		return 0;
}

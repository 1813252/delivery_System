#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "storage.h"

#define STORAGE_FILEPATH 	"storage2.txt"

/* 
  definition of storage cell structure ----
  members :
  int building : building number of the destination
  int room : room number of the destination
  int cnt : number of packages in the cell
  char passwd[] : password setting (4 characters)
  char *contents : package context (message string)
*/

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
 	
 	fp= fopen(filepath,"r");	//파일 오픈  
			
		
		int line=0; //텍스트 파일 몇줄인지 세보기  
		char c;      
		
		if (fp==NULL)
		{
			return -1; //파일이 안 열리면 거기서 끝  
			
		}
		
		while ((c=fgetc(fp)) !=EOF) // 파일  끝까지 읽기  
		{
			if (c=='\n') //줄 바뀔 때 마다  
			
				line++; //line ++ 
		}
			
		fclose(fp); //다 읽었으니 닫아줌  
		
		fp= fopen(filepath,"r");	 //다시 열어요  
		
		fscanf(fp, "%d %d %s", &systemSize[0], &systemSize[1], masterPassword); // 행,열,마스터 비밀번호 받기  
 	
 	
		deliverySystem = (storage_t**)malloc(sizeof(storage_t*)*systemSize[0]);  //이중포인터 deliverySystem 이 deliverySystem[i] 를 가르키도록 함// 

		for(i=0;i<systemSize[0];i++)
		{
			
			deliverySystem[i] = (storage_t*)malloc(sizeof(storage_t)*systemSize[1]); //deliverySystem[i]는 deliverySystem[i][0]을 가르킵니다//
		
		}
			
	
		for(i=0;i<systemSize[0];i++)
		{
			for(j=0;j<systemSize[1];j++)
			deliverySystem[i][j].cnt=0; //deliverySystem[i][j].cnt=0 으로 초기화하여 화면에 trash 값을 띄우지 않도록 합니다//
		}
		
		
		int k; 
			
		for(k=2;k<line;k++) //3번째 줄부터 마지막 줄까지 읽습니다
		{
			fscanf(fp,"%i %i",&i ,&j); //앞에 두 정수는 i, j 값에 저장합니다
			deliverySystem[i][j].context= (char*)malloc(sizeof(char)*100); //일단 100글자로 메모리 할당을 해줍니다//
			fscanf(fp," %i %i %s %s", &deliverySystem[i][j].building, &deliverySystem[i][j].room, deliverySystem[i][j].passwd, deliverySystem[i][j].context);
			// passwd와 context는 각각 배열, 포인터 이므로 주소연산자 &를 사용하지 않습니다//
			
			deliverySystem[i][j].cnt++; //cnt=1을 만들어주어 프로그램 화면에 띄웁니다//
			storedCnt++;//저장된 택배수가 ++ 됩니다.
		}
		
		
		fclose(fp); //파일을 닫습니다.

		
	return 0; //시스템을 잘 만들었으면 0을 반환합니다 
	
}

//put a package (msg) to the cell
//input parameters
//int x, int y : coordinate of the cell to put the package
//int nBuilding, int nRoom : building and room numbers of the destination
//char msg[] : package context (message string)
//char passwd[] : password string (4 characters)
//return : 0 - successfully put the package, -1 - failed to put
int str_pushToStorage(int x, int y, int nBuilding, int nRoom, char msg[MAX_MSG_SIZE+1], char passwd[PASSWD_LEN+1]) {
	

	deliverySystem[x][y].context= (char*)malloc(sizeof(char)*strlen(msg)); //받은 message만큼 context 포인터에 메모리를 할당해줍니다// 
	
	deliverySystem[x][y].building=nBuilding; 
	deliverySystem[x][y].room = nRoom;
	strcpy(deliverySystem[x][y].passwd,passwd);
	strcpy(deliverySystem[x][y].context, msg);
	deliverySystem[x][y].cnt++;
	
	if(deliverySystem[x][y].cnt!=1)
	{
		return -1; //택배가 보관함에 안들어갔으면 -1을 return 합니다// 
	}
	
	else //성공하면  
	storedCnt++; //택배수 ++ 
	str_backupSystem(STORAGE_FILEPATH);//텍스트파일에도 써줍니다// 
	
	return 0; 

}



// ------- API function for main.c file ---------------

//backup the delivery system context to the file system
//char* filepath : filepath and name to write
//return : 0 - backup was successfully done, -1 - failed to backup
int str_backupSystem(char* filepath) { 
	
	
	FILE *fp;
	int i,j;
	fp=fopen(filepath,"w"); //쓰기모드로 열어줍니다(새로 써줍니다)// 
	
	fprintf(fp,"%d %d\n%s\n", systemSize[0], systemSize[1], masterPassword); 
		
	for (i=0;i<systemSize[0];i++)
	{
		for(j=0;j<systemSize[1];j++)
		if (deliverySystem[i][j].cnt==1) //deliverySystem[i][j].cnt==1인 경우만 새로 써줍니다// 
		{	
			fprintf(fp,"%i %i %i %i %s %s\n",i,j,deliverySystem[i][j].building, deliverySystem[i][j].room, deliverySystem[i][j].passwd, deliverySystem[i][j].context);		
		}
	}

	fclose(fp); //닫아주는걸 잊지맙니다// 
	
	
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

static void initStorage(int x, int y) { //extractStorage 함수를 만들때 사용할 것입니다 1// 
	
	
	deliverySystem[x][y].building=NULL; 
	deliverySystem[x][y].room=NULL;
	deliverySystem[x][y].cnt = 0;
	deliverySystem[x][y].passwd[PASSWD_LEN+1]=NULL;
	
	free(deliverySystem[x][y].context); ///free memory	
	
}
//get password input and check if it is correct for the cell (x,y)
//int x, int y : cell for password check
//return : 0 - password is matching, -1 - password is not matching

static int inputPasswd(int x, int y) { //extractStorage 함수를 만들때 사용할 것입니다 2// 
	
	char input_passwd[PASSWD_LEN+1]; //문자열을 받습니다// 
	printf("input password for (%i,%i) storage : ",x,y);
	scanf("%s",input_passwd); 
	
	if (strcmp(input_passwd,deliverySystem[x][y].passwd)==0) //비밀번호가 맞으면// 
	{
		return 0; 
	}
	
	else if (strcmp(input_passwd,masterPassword)==0) //비밀번호가 마스터비밀번호면// 
	{
		return 0;
	}
	
	else //틀리면//
	 
	return -1;
	
}


//extract the package context with password checking
//after password checking, then put the msg string on the screen and re-initialize the storage
//int x, int y : coordinate of the cell to extract
//return : 0 - successfully extracted, -1 = failed to extract
int str_extractStorage(int x, int y) {
	
	
	int check_passwd=inputPasswd(x,y); //아까만든 비밀번호 확인해주는 함수를 만듭니다// 
	
	if (check_passwd!=0) //틀릴 때// 
	{
		printf("password is wrong!!\n");
		return -1;
	}
	
	else if (check_passwd==0) //맞거나 마스터비밀번호 일 때// 
	{
		
			
		printf("---------->extracting the storage (%i,%i)...",x,y);
		printf("\n------------------------------------------------\n");
		printf("<<<<<<<<<<<<<<< : %s>>>>>>>>>>>>>>>>>>>",deliverySystem[x][y].context);
		printf("\n------------------------------------------------\n");
		
		initStorage(x,y); //보관함을 비워줌// 
		str_backupSystem(STORAGE_FILEPATH); //텍스트파일에도 써줌// 
		storedCnt--; //보관된 택배 -1// 
					
	}
	
	if (deliverySystem[x][y].cnt!=0) //보관함 안비워졌으면// 
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
			for(y=0;y<systemSize[1];y++) //모든 보관함을 다 확인합니다// 
			
			if(deliverySystem[x][y].building==nBuilding && deliverySystem[x][y].room==nRoom) //찾습니다// 
			printf("Find a package in (%i,%i)\n",x,y); //알려줍니다// 
			cnt++;
		}
		
	return cnt; //내 택배가  없으면 cnt=0// 
}


//free the memory of the deliverySystem 
void str_freeSystem(void) {
	
	int i;
	
	for(i=0;i<systemSize[0];i++)
	{

		free(deliverySystem[i]);
			
	}
		
		free(deliverySystem); //마지막에 deliverySystem 에 할당해줬던 메모리를 해제해줍니다// 
	
}


#include "user.h"


/*
이름    : mycd 함수
작성자  : 오규빈
기능    : 작업 디렉토리의 경로를 변경한다
받는값  : 경로 문자열
리턴값  : X
*/
void mycd (char path[])
{
    if(path == NULL)
    {//path 인자가 없을시
        rear_dir_list_ptr = front_dir_list_ptr;//작업 디렉터리를 홈디렉터리로 설정
    }
    else if (path != NULL)
    {
        int first = 0;//첫 / 잘라내기
        DIR_LIST *tmp_ptr;
        char *nm_ptr = strtok(path, "/");
        printf("%s",nm_ptr);
        while (nm_ptr != NULL)//이름이 NULL값이 아니라면
        {
            if (first == 0)//시작디렉토리가 . , .. , /셋 중 하나  . 와 ..에서 시작할때의 경우 판단
            {
                if (nm_ptr == ".") //.에서 시작
                {
                    tmp_ptr = rear_dir_list_ptr;
                    printf("%s\n", nm_ptr);
                    nm_ptr = strtok(NULL, "/");
                    first++;
                    continue;
                }
                else if (nm_ptr == "..")//..에서 시작
                {
                    int i = 0;
                    tmp_ptr = front_dir_list_ptr;
                    while (i < cntfound()-1)
                    {
                        tmp_ptr = tmp_ptr -> next_ptr;
                    } // tmp_ptr이 현재 디렉터리 리스트에서 (마지막-1)번째 디렉토리를 가리키도록 함.
                    
                    tmp_ptr = rear_dir_list_ptr;
                    nm_ptr = strtok(NULL, "/");
                    first++;
                    continue;
                }
                else
                {
                    ;
                }
            }
            else if (first != 0)
            {
                DIR_LIST* new_dir = malloc(sizeof(DIR_LIST)); 
                new_dir-> name = nm_ptr;
                printf("%s",nm_ptr);
                char* s = strcat("/",nm_ptr);//s = /'다음 디렉토리'
                new_dir-> inode = path_to_inode(strcat(prtpwd(),s));
                tmp_ptr -> next_ptr = new_dir;
                nm_ptr = strtok(NULL, "/");
            }
        }
    }
    return;
}

/*
이름    : mycpto 함수
작성자  : 오규빈
기능    : MY 파일시스템에 있는 파일을 호스트 컴퓨터로 복사한다.
받는값  : 파일명 1,2
리턴값  : 리턴값
*/
void mycpto (const char* source_file, const char* dest_file  )
{
    FILE *ofp;
    FILE *myfs;
    int c,d;
    int i = 0;
    int inode = 1;
    int tmp_datablock;

    if(source_file ==NULL || dest_file == NULL)
    {
        printf("오류 : 인자가 부족합니다");
        return;
    }
    char *tmp_file_string_ptr = (char *)malloc(sizeof(char) * 8); //디렉토리의 datablock에서 추출한 파일명을 가리킬 포인터
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //디렉토리의 datablock에서 추출한 inode 번호를 가리킬 포인터
    INODE *inode_ptr = (INODE *)malloc(sizeof(INODE));// inode 포인터

    myfs = fopen("myfs", "rb");
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (rear_dir_list_ptr -> inode - 1)), SEEK_SET);
    fread(inode_ptr, sizeof(INODE), 1, myfs);

    tmp_datablock = (int)(inode_ptr->dir_1 + 1);
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+ INODE_LIST_SIZE +  (256 * (tmp_datablock - 1)),SEEK_SET);//현재 디렉토리의 데이터블록 앞으로 포인터 이동
    fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs); // 포인터로 파일명 확인

    while(!strcmp(tmp_file_string_ptr, source_file))
    {//찾는 파일과 확인한 파일의 이름이 같지 않다면
        if((c = getchar()) != EOF)
        {//myfs파일의 끝 확인
            fseek(myfs, -1, SEEK_CUR);
            fseek(myfs,sizeof(int),SEEK_CUR);//현재 포인터 위치로부터 int형 크기만큼 이동
            fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs);
        }
        else
        {
        printf("오류 : %s 파일이 없습니다.\n", source_file); //파일명을 현재 디렉토리에서 못찾으면 오류 띄우기
        return;
        }
    }

    fread(tmp_inode_ptr, sizeof(int), 1, myfs);
    inode =*tmp_inode_ptr;

    if ((ofp = fopen(dest_file, "wb")) == NULL)
    {
        printf("오류 : %s 파일을 열 수 없습니다. \n", dest_file);
        return;
    }

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (inode - 1)), SEEK_SET);
    fread(inode_ptr, sizeof(INODE), 1, myfs);

    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+INODE_LIST_SIZE +(DATA_BLOCK_SIZE*((inode_ptr->dir_1)- 1)),SEEK_SET);//새로운 파일에 복사
    while(i != DATA_BLOCK_SIZE)
    {
        d = getc(myfs);
        putc(d,ofp);
        i++;
    }
    
    free(tmp_file_string_ptr);
    free(tmp_inode_ptr);
    free(inode_ptr);
    fclose(ofp);
    fclose(myfs);

    return;
}

/*
이름    : mycpfrom 함수
작성자  : 오규빈
기능    : 호스트 컴퓨터에 있는 파일을 MY 파일시스템으로 복사한다.
받는값  : 파일명 1,2
리턴값  : 리턴값
*/

void mycpfrom (const char* source_file, const char* dest_file )
{
    FILE *ifp;
    FILE *myfs;
    int c,size_F;
    time_t Time;
    struct tm* TimeInfo;
 
    Time = time(NULL);                // 현재 시간을 받음
    TimeInfo = localtime(&Time); 

    myfs = fopen("myfs", "rb+");
    char *tmp_dir_string_ptr = (char *)malloc(sizeof(char) * 8); //디렉토리의 datablock에서 추출한 디렉토리명을 가리킬 포인터
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //디렉토리의 datablock에서 추출한 inode 번호를 가리킬 포인터
    INODE *inode_data_ptr = (INODE *)malloc(sizeof(INODE));
    
    int new_inode = acc_inode();
    int new_data = acc_data();
    ifp = fopen(source_file,"rb");
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*(new_data-1)),SEEK_SET);//새로운 파일에 복사
    while ((c = getc(ifp)) != EOF)
    {
      size_F++;
      putchar(c);
    }

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (new_inode - 1)),SEEK_SET);//INODELIST 채우기
    fread(inode_data_ptr, sizeof(INODE), 1, myfs);
    inode_data_ptr -> type = 0;
    inode_data_ptr -> year = TimeInfo ->tm_year+1900;
    inode_data_ptr-> size = size_F;
    inode_data_ptr -> month = TimeInfo ->tm_mon+1;
    inode_data_ptr -> date = TimeInfo ->tm_mday;
    inode_data_ptr -> hour = TimeInfo ->tm_hour;
    inode_data_ptr -> minute = TimeInfo ->tm_min;
    inode_data_ptr -> second = TimeInfo ->tm_sec;
    inode_data_ptr -> dir_1 = new_data;    
    inode_data_ptr -> dir_2 = 0;
    inode_data_ptr -> dir_3 = 0;
    inode_data_ptr -> dir_4 = 0;
    inode_data_ptr -> dir_5 = 0;
    inode_data_ptr -> dir_6 = 0;
    inode_data_ptr -> dir_7 = 0;
    inode_data_ptr -> dir_8 = 0;
    inode_data_ptr -> indir = 0; 
    fwrite(inode_data_ptr,sizeof(INODE),1,myfs);

    

    fclose(ifp);
    fclose(myfs);

    return;
}

/*
이름    : mycp 함수
작성자  : 오규빈
기능    : MY 파일시스템에 있는 파일을 복사하는 명령어
받는값  : 파일명 1,2
리턴값  : 리턴값
*/

void mycp(const char* source_file, const char* dest_file  )
{
    FILE *myfs;
    int c,size_F;;
    int i = 0;
    int inode = 1;
    char tmp;
    char* d;
    time_t Time;
    struct tm* TimeInfo;
 
    Time = time(NULL);                // 현재 시간을 받음
    TimeInfo = localtime(&Time); 

    char *tmp_file_string_ptr; //디렉토리의 datablock에서 추출한 파일명을 가리킬 포인터
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //디렉토리의 datablock에서 추출한 inode 번호를 가리킬 포인터
    INODE *inode_data_ptr = (INODE *)malloc(sizeof(INODE));// inode 포인터
    char *tmp_data_string; //디렉토리의 datablock에서 추출한 데이터를 가리킬 포인터

    myfs = fopen("myfs", "rb+");
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*(path_to_inode(prtpwd())-1)),SEEK_SET);//현재 디렉토리의 데이터블록 앞으로 포인터 이동
    fread(inode_data_ptr, sizeof(INODE), 1, myfs);
    int tmp_datablock = (int)(inode_data_ptr->dir_1 + 1);
    fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs); // 포인터로 파일명 확인

    while(!strcmp(tmp_file_string_ptr, source_file))
    {//찾는 파일과 확인한 파일의 이름이 같지 않다면
        if((c = getchar()) != EOF)
        {//myfs파일의 끝 확인
            fseek(myfs, -1, SEEK_CUR);
            fseek(myfs,sizeof(int),SEEK_CUR);//현재 포인터 위치로부터 int형 크기만큼 이동
            fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs);
        }
        else
        {
        fprintf(stderr,"오류 : %s 파일이 없습니다.\n", source_file); //파일명을 현재 디렉토리에서 못찾으면 오류 띄우기
        exit(1);
        }
    }
    fread(tmp_inode_ptr, sizeof(int), 1, myfs);
    inode =*tmp_inode_ptr;

    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*((inode_data_ptr->dir_1)- 1)),SEEK_SET);//문자열 포인터에 복사해놓기
    while(i != DATA_BLOCK_SIZE)
    {
        tmp = getchar();
        sprintf(d,"%d",tmp);
        strcat(tmp_data_string,d);
        i++;
    }

    fclose(myfs);

    int new_inode = acc_inode();
    int new_data = acc_data();
    
    rewind(myfs);
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*(new_data-1)),SEEK_SET);//새로운 파일에 복사
    int k = 0;
    int j = sizeof(tmp_data_string);
    while (k < j);
    {
      size_F++;
      putchar(c);
      k++;
    }

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (new_inode - 1)),SEEK_SET);//INODELIST 채우기
    fread(inode_data_ptr, sizeof(INODE), 1, myfs);
    inode_data_ptr -> type = 0;
    inode_data_ptr -> year = TimeInfo ->tm_year+1900;
    inode_data_ptr-> size = size_F;
    inode_data_ptr -> month = TimeInfo ->tm_mon+1;
    inode_data_ptr -> date = TimeInfo ->tm_mday;
    inode_data_ptr -> hour = TimeInfo ->tm_hour;
    inode_data_ptr -> minute = TimeInfo ->tm_min;
    inode_data_ptr -> second = TimeInfo ->tm_sec;
    inode_data_ptr -> dir_1 = new_data;    
    inode_data_ptr -> dir_2 = 0;
    inode_data_ptr -> dir_3 = 0;
    inode_data_ptr -> dir_4 = 0;
    inode_data_ptr -> dir_5 = 0;
    inode_data_ptr -> dir_6 = 0;
    inode_data_ptr -> dir_7 = 0;
    inode_data_ptr -> dir_8 = 0;
    inode_data_ptr -> indir = 0; 
    fwrite(inode_data_ptr,sizeof(INODE),1,myfs);

    
    free(tmp_file_string_ptr);
    free(tmp_inode_ptr);
    fclose(myfs);

    return;
}

/*
이름    : myrm 함수
작성자  : 오규빈
기능    : 파일을 삭제하는 명령어
받는값  : 파일명
리턴값  : 리턴값
*/

void myrm(const char* file)
{
    FILE *myfs;
    int c,i=0;
    int inode = 1;

    char *tmp_file_string_ptr = (char *)malloc(sizeof(char) * 8); //디렉토리의 datablock에서 추출한 파일명을 가리킬 포인터
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //디렉토리의 datablock에서 추출한 inode 번호를 가리킬 포인터
    INODE *inode_ptr = (INODE *)malloc(sizeof(INODE));// inode 포인터

    myfs = fopen("myfs", "rb+");
    if (myfs == NULL)
    {
        printf("myrm() 함수 : 파일 열기에 실패했습니다.\n");
        abort();
    }
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*((path_to_inode(prtpwd()))-1)),SEEK_SET);//현재 디렉토리의 데이터블록 앞으로 포인터 이동
    fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs); // 포인터로 파일명 확인

    while(!strcmp(tmp_file_string_ptr, file))
    {//찾는 파일과 확인한 파일의 이름이 같지 않다면
        if((c = getchar()) != EOF)
        {//myfs파일의 끝 확인
            fseek(myfs, -1, SEEK_CUR);
            fseek(myfs,sizeof(int),SEEK_CUR);//현재 포인터 위치로부터 int형 크기만큼 이동
            fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs);
        }
        else
        {
        fprintf(stderr,"오류 : %s 파일이 없습니다.\n", file); //파일명을 현재 디렉토리에서 못찾으면 오류 띄우기
        exit(1);
        }
    }
    fread(tmp_inode_ptr, sizeof(int), 1, myfs);
    inode =*tmp_inode_ptr;
    fclose(myfs);

    rewind(myfs);
    fseek(myfs, BOOT_BLOCK_SIZE + (inode- 1) ,SEEK_SET);//INODELIST 
    putchar(0);
    fseek(myfs, BOOT_BLOCK_SIZE + 128 + ((inode_ptr -> dir_1) - 1),SEEK_SET);//DATABLOCK
    putchar(0);
    fseek(myfs, BOOT_BLOCK_SIZE + (sizeof(INODE)*128)+(DATA_BLOCK_SIZE)*((inode_ptr -> dir_1) - 1) ,SEEK_SET);
    while(i != DATA_BLOCK_SIZE)
    {
        putchar(0);
        i++;
    }
}

/*
이름    : cntfound 함수
작성자  : 오규빈
기능    : dir_list에서 현재 이어진 디렉토리 수 리턴
받는값  : X
리턴값  : count값
*/

int cntfound() 
{
    int cnt = 0;
    DIR_LIST* tmp_dir = front_dir_list_ptr;
    while ((tmp_dir->next_ptr) != NULL)
    {
        {
            tmp_dir = tmp_dir->next_ptr;
            cnt++;
            printf("fff\n");
        }
    }
    return cnt;//탐색 실패시

}

/*
이름    : prtpwd 함수
작성자  : 오규빈
기능    : pwd를 문자열로 리턴
받는값  : X
리턴값  : pwd 문자열값
*/
char* prtpwd()
{
    char* s;
    char* pwd;
    DIR_LIST *tmp_ptr = front_dir_list_ptr;

    while (1)
    {
        s = tmp_ptr->name;
        pwd = strcat(pwd,s);
        if (tmp_ptr->next_ptr == NULL)
        {
            break;
        }
        else
        {
            pwd = strcat(pwd,"/");
            tmp_ptr = tmp_ptr->next_ptr;
        }
    }
    return pwd;
}

/*
이름    : acc_inode 함수
작성자  : 오규빈
기능    : 가용중이지 않은 inode 번호를 가져오고, 그 inode번호에 해당하는 슈퍼블록을 1로 채운다.
받는값  : X
리턴값  : 가용중이지 않은 inode 번호
*/

int acc_inode()
{
    FILE* myfs;
    int t = 1;

    myfs = fopen("myfs", "rb");
    if (myfs == NULL)
    {
        printf("acc_inode() 함수 : 파일 열기에 실패했습니다.\n");
        abort();
    }

    SUPERBLOCK *sb_ptr = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
    fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
    fread(sb_ptr, sizeof(SUPERBLOCK), 1, myfs);//사용중인 슈퍼블록 확인하여 가용중이지 않은 아이노드에 파일 넣기 위함.

    unsigned mask;
    while(1){ // 빈 inode가 있을때 까지 무한 반복
    if (t < 33) // inode_1에 빈 inode가 있을 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->inode_1 & mask) == 0) //빈 아이노드가 있었을 경우
        {
            sb_ptr->inode_1 = sb_ptr->inode_1 | 1;//빈 아이노드 채우기
            break;
        }
        t++;
    }
    else if (t < 65) // inode_2에 빈 inode가 있을 경우
    {
        mask = 1 << ((t- 32) - 1);
        if ((sb_ptr->inode_2 & mask) == 0)
        {
            sb_ptr->inode_2 = sb_ptr->inode_2 | 1;
            break;
        }
        t++;
    }
    else if (t < 97) // inode_3에 빈 inode가 있을 경우
    {
        mask = 1 << ((t - 64) - 1);
        if ((sb_ptr->inode_3 & mask) == 0)
        {
          sb_ptr->inode_3 = sb_ptr->inode_3 | 1;
          break;
        }
        t++;
    }
    else // inode_4에 빈 inode가 있을 경우
    {
        mask = 1 << ((t - 96) - 1);
        if ((sb_ptr->inode_4 & mask) == 0)
        {
            sb_ptr->inode_4 = sb_ptr->inode_4 | 1;
            break;
        }
        t++;
    }
    }
    return t;
}

/*
이름    : acc_data 함수
작성자  : 오규빈
기능    : 가용중이지 않은 datablock 번호를 가져오고, 그 inode번호에 해당하는 슈퍼블록을 1로 채운다.
받는값  : X
리턴값  : 가용중이지 않은 datablock 번호
*/
int acc_data()
{
    FILE* myfs;
    int t = 1;

    myfs = fopen("myfs", "rb");
    if (myfs == NULL)
    {
        printf("acc_data() 함수 : 파일 열기에 실패했습니다.\n");
        abort();
    }

    SUPERBLOCK *sb_ptr = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
    fseek(myfs, BOOT_BLOCK_SIZE+128, SEEK_SET);
    fread(sb_ptr, sizeof(SUPERBLOCK), 1, myfs);//사용중인 슈퍼블록 확인하여 가용중이지 않은 데이터블록에 파일 넣기 위함.

    unsigned mask;
    while(1)
    { // 빈 데이터블록이 있을때 까지 무한 반복
    if (t < 33) // data_block_1에 빈 data_block이 있을 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_1 & mask) == 0) //빈 슈퍼블록이 있었을 경우
        {
            sb_ptr->data_block_1 = sb_ptr->data_block_1 | 1;//빈 슈퍼블록 채우기
            break;
        }
        t++;
    }
    else if (t < 65) // data_block_2에 빈 data_block이 있을 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_2 & mask) == 0) //빈 슈퍼블록이 있었을 경우
        {
            sb_ptr->data_block_2 = sb_ptr->data_block_2 | 1;//빈 슈퍼블록 채우기
            break;
        }
        t++;
    }
    else if (t < 97) // data_block_3에 빈 data_block이 있을 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_3 & mask) == 0) //빈 슈퍼블록이 있었을 경우
        {
            sb_ptr->data_block_3 = sb_ptr->data_block_3 | 1;//빈 슈퍼블록 채우기
            break;
        }
        t++;
    }
    else if (t < 129) // data_block_4에 빈 data_block이 있을 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_4 & mask) == 0) //빈 슈퍼블록이 있었을 경우
        {
            sb_ptr->data_block_4 = sb_ptr->data_block_4 | 1;//빈 슈퍼블록 채우기
            break;
        }
        t++;
    }
    else if (t < 161) // data_block_5에 빈 data_block이 있을 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_5 & mask) == 0) //빈 슈퍼블록이 있었을 경우
        {
            sb_ptr->data_block_5 = sb_ptr->data_block_5 | 1;//빈 슈퍼블록 채우기
            break;
        }
        t++;
    }
    else if (t < 193) // data_block_6에 빈 data_block이 있을 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_6 & mask) == 0) //빈 슈퍼블록이 있었을 경우
        {
            sb_ptr->data_block_6 = sb_ptr->data_block_6 | 1;//빈 슈퍼블록 채우기
            break;
        }
        t++;
    }
    else if (t < 225) // data_block_7에 빈 data_block이 있을 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_7 & mask) == 0) //빈 슈퍼블록이 있었을 경우
        {
            sb_ptr->data_block_7 = sb_ptr->data_block_7 | 1;//빈 슈퍼블록 채우기
            break;
        }
        t++;
    }
    else // data_block_3에 빈 data_block이 있을 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_8 & mask) == 0) //빈 슈퍼블록이 있었을 경우
        {
            sb_ptr->data_block_8 = sb_ptr->data_block_8 | 1;//빈 슈퍼블록 채우기
            break;
        }
        t++;
    }
    }
    return t;
}

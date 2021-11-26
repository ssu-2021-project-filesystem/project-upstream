#include "user.h"
#include <time.h>
/*
이름    : mycd 함수
작성자  : 오규빈
기능    : 작업 디렉토리의 경로를 변경한다
받는값  : 경로 문자열
리턴값  : X
*/
void mycd (const char* path){
    if(path == NULL)
    {//path 인자가 없을시
        rear_dir_list_ptr == front_dir_list_ptr;//작업 디렉터리를 홈디렉터리로 설정
    }

    else if (path != NULL)
    {
        int i = 0;
        int first = 0;
        DIR_LIST *tmp_ptr;
        char *nm_ptr = strtok(path, "/");
        while (nm_ptr != NULL)
        {
         if (first == 0)
         {
          if (nm_ptr == ".")
          {
          tmp_ptr = rear_dir_list_ptr -> next_ptr;
          tmp_ptr -> next_ptr -> name = nm_ptr;
          nm_ptr = strtok(NULL, "/");
          first++;
          }
          else if (nm_ptr == "..")
          {
           tmp_ptr = front_dir_list_ptr -> next_ptr;
           front_dir_list_ptr -> next_ptr -> name = nm_ptr;
           nm_ptr = strtok(NULL, "/");
           first++;
          }
          else if (nm_ptr == "/")
          {
           tmp_ptr = front_dir_list_ptr -> next_ptr;
           front_dir_list_ptr -> next_ptr -> name = nm_ptr;
           nm_ptr = strtok(NULL, "/");
           first++;
          }
          else
          {
              printf("잘못된 입력 방식입니다.");
              break;
          }
         }
         else if (first != 0)
         {
          tmp_ptr -> next_ptr -> name = nm_ptr;
          nm_ptr = strtok(NULL, "/");
         }
         tmp_ptr->next_ptr == NULL;
         rear_dir_list_ptr == tmp_ptr;
        }
    }
}
    //미안합니다

/*
이름    : mycpto 함수
작성자  : 오규빈
기능    : MY 파일시스템에 있는 파일을 호스트 컴퓨터로 복사한다.
받는값  : 파일명 1,2
리턴값  : 리턴값
*/
void mycpto (const char* source_file, const char* dest_file  ){
    FILE *ofp;
    FILE *myfs;
    int c,d;
    int i = 0;

    char *tmp_dir_string_ptr = (char *)malloc(sizeof(char) * 8); //디렉토리의 datablock에서 추출한 디렉토리명을 가리킬 포인터
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //디렉토리의 datablock에서 추출한 inode 번호를 가리킬 포인터

    myfs = fopen(myfs, "rb");
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*(rear_dir_list_ptr->inode - 1)),SEEK_SET);//현재 디렉토리의 데이터블록 앞으로 포인터 이동
    const char* check = fread(tmp_dir_string_ptr, sizeof(char) * 8, 1, myfs); // 포인터로 파일명 확인

    while(!strcmp(check, source_file))
    {//찾는 파일과 확인한 파일의 이름이 같지 않다면
        if((c = getchar()) != EOF)
        {//myfs파일의 끝 확인
            fseek(myfs, -1, SEEK_CUR);
            fseek(myfs,sizeof(int),SEEK_CUR);//현재 포인터 위치로부터 int형 크기만큼 이동
            check = fread(tmp_dir_string_ptr, 8, 1, myfs);
        }
        else
        {
        fprintf(stderr,"오류 : %s 파일이 없습니다.\n", source_file); //파일명을 현재 디렉토리에서 못찾으면 오류 띄우기
        exit(1);
        }
    }

    int tmp_inode = atoi(fread(tmp_inode_ptr, sizeof(int), 1, myfs)); // 포인터로 inode 확인

    if ((ofp = fopen(dest_file, "wb")) == NULL)
    {
        fprintf(stderr, "오류 : %s 파일을 열 수 없습니다. \n", source_file);
        exit(1);
    }

    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*(tmp_inode-1)),SEEK_SET);//새로운 파일에 복사
    while(i != DATA_BLOCK_SIZE){
        d = getchar();
        putc(d,ofp);
        i++;
    }

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

void mycpfrom (const char* source_file, const char* dest_file  ){
    FILE *ifp;
    FILE *myfs;
    int c;
    time_t Time;
    struct tm* TimeInfo;
 
    Time = time(NULL);                // 현재 시간을 받음
    TimeInfo = localtime(&Time); 

    myfs = fopen(myfs, "wb");
    char *tmp_dir_string_ptr = (char *)malloc(sizeof(char) * 8); //디렉토리의 datablock에서 추출한 디렉토리명을 가리킬 포인터
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //디렉토리의 datablock에서 추출한 inode 번호를 가리킬 포인터

    fseek(myfs, BOOT_BLOCK_SIZE,SEEK_SET);
    int t = 0; // superblock을 통해 새로운 파일 만들 inode 번호 생성

    //mk랑 같은 걸로 해야되는데...
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

    INODE *inode_data_ptr = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (t - 1)),SEEK_SET);//INODELIST 채우기
    inode_data_ptr -> type = 0;
    inode_data_ptr -> year = TimeInfo ->tm_year+1900;
    inode_data_ptr->size = 0;
    inode_data_ptr -> month = TimeInfo ->tm_mon+1;
    inode_data_ptr -> date = TimeInfo ->tm_mday;
    inode_data_ptr -> hour = TimeInfo ->tm_hour;
    inode_data_ptr -> minute = TimeInfo ->tm_min;
    inode_data_ptr -> second = TimeInfo ->tm_sec;
    
     fwrite(inode_data_ptr,sizeof(INODE),1,myfs);

    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*(t-1)),SEEK_SET);//새로운 파일에 복사
    while ((c = getc(ifp)) != EOF)
    {
    putchar(c);
    }

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

void mycp(const char* source_file, const char* dest_file  ){
    FILE *myfs;
    int c,d;
    int i = 0;
    time_t Time;
    struct tm* TimeInfo;
 
    Time = time(NULL);                // 현재 시간을 받음
    TimeInfo = localtime(&Time); 

    char *tmp_dir_string_ptr = (char *)malloc(sizeof(char) * 8); //디렉토리의 datablock에서 추출한 디렉토리명을 가리킬 포인터
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //디렉토리의 datablock에서 추출한 inode 번호를 가리킬 포인터
    
    myfs = fopen(myfs, "rb");
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*(rear_dir_list_ptr->inode - 1)),SEEK_SET);//현재 디렉토리의 데이터블록 앞으로 포인터 이동
    const char* check = fread(tmp_dir_string_ptr, sizeof(char) * 8, 1, myfs); // 포인터로 파일명 확인

    while(!strcmp(check, source_file))
    {//찾는 파일과 확인한 파일의 이름이 같지 않다면
        if((c = getchar()) != EOF)
        {//myfs파일의 끝 확인
            fseek(myfs, -1, SEEK_CUR);
            fseek(myfs,sizeof(int),SEEK_CUR);//현재 포인터 위치로부터 int형 크기만큼 이동
            check = fread(tmp_dir_string_ptr, 8, 1, myfs);
        }
        else
        {
        fprintf(stderr,"오류 : %s 파일이 없습니다.\n", source_file); //파일명을 현재 디렉토리에서 못찾으면 오류 띄우기
        exit(1);
        }
    }

    int tmp_inode = atoi(fread(tmp_inode_ptr, sizeof(int), 1, myfs)); // 포인터로 inode 확인

    fclose(myfs);

    myfs = fopen(myfs, "wb");
    rewind(myfs);
    char *tmp_dir_string_ptr = (char *)malloc(sizeof(char) * 8); //디렉토리의 datablock에서 추출한 디렉토리명을 가리킬 포인터
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //디렉토리의 datablock에서 추출한 inode 번호를 가리킬 포인터

    fseek(myfs, BOOT_BLOCK_SIZE,SEEK_SET);
    int t = 0; // superblock을 통해 새로운 파일 만들 inode 번호 생성

    //mk랑 같은 걸로 해야되는데...
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
    INODE *inode_data_ptr = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (t - 1)),SEEK_SET);//INODELIST 채우기
    inode_data_ptr -> type = 0;
    inode_data_ptr -> year = TimeInfo ->tm_year+1900;
    inode_data_ptr->size = 0;
    inode_data_ptr -> month = TimeInfo ->tm_mon+1;
    inode_data_ptr -> date = TimeInfo ->tm_mday;
    inode_data_ptr -> hour = TimeInfo ->tm_hour;
    inode_data_ptr -> minute = TimeInfo ->tm_min;
    inode_data_ptr -> second = TimeInfo ->tm_sec;

    fwrite(inode_data_ptr,sizeof(INODE),1,myfs);

    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*(t-1)),SEEK_SET);//새로운 파일에 복사
    while(i != DATA_BLOCK_SIZE){
        d = getchar();
        putchar(d);
        i++;
    }
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

void myrm(const char* file){
    FILE *myfs;
    int c,i=0;

    char *tmp_dir_string_ptr = (char *)malloc(sizeof(char) * 8); //디렉토리의 datablock에서 추출한 디렉토리명을 가리킬 포인터
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //디렉토리의 datablock에서 추출한 inode 번호를 가리킬 포인터

    myfs = fopen(myfs, "rb");
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*(rear_dir_list_ptr->inode - 1)),SEEK_SET);//현재 디렉토리의 데이터블록 앞으로 포인터 이동
    const char* check = fread(tmp_dir_string_ptr, sizeof(char) * 8, 1, myfs); // 포인터로 파일명 확인

    while(!strcmp(check, file))
    {//찾는 파일과 확인한 파일의 이름이 같지 않다면
        if((c = getchar()) != EOF)
        {//myfs파일의 끝 확인
            fseek(myfs, -1, SEEK_CUR);
            fseek(myfs,sizeof(int),SEEK_CUR);//현재 포인터 위치로부터 int형 크기만큼 이동
            check = fread(tmp_dir_string_ptr, 8, 1, myfs);
        }
        else
        {
        fprintf(stderr,"오류 : %s 파일이 없습니다.\n", file); //파일명을 현재 디렉토리에서 못찾으면 오류 띄우기
        exit(1);
        }
    }

    int tmp_inode = atoi(fread(tmp_inode_ptr, sizeof(int), 1, myfs)); // 포인터로 inode 확인

    fclose(myfs);

    myfs = fopen(myfs, "wb");
    fseek(myfs, BOOT_BLOCK_SIZE + (tmp_inode - 1) ,SEEK_SET);//INODELIST 
    putchar(0);
    fseek(myfs, BOOT_BLOCK_SIZE + (sizeof(INODE)*128)+(DATA_BLOCK_SIZE)*(tmp_inode - 1) ,SEEK_SET);
    while(i != DATA_BLOCK_SIZE){
        putchar(0);
        i++;
    }
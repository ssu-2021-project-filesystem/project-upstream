//헤더파일
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



//매크로
//block 별 크기(바이트 단위)
#define     BOOT_BLOCK_SIZE             2       //boot block의 총 크기
#define     SUPER_BLOCK_SIZE            12      //super block의 총 크기
#define     INODE_LIST_SIZE             2176    //inode list의 총 크기(inode 하나의 크기 아님)

#define     DATA_BLOCK_SIZE             256     //data block의 block 하나의 크기

//명령어 문자열의 크기 지정
#define     COM_SEP_SIZE                30      //쉘에 입력하는 명령어에서 각 단어의 최대 크기
#define     COM_SEP_NUM                 4       //쉘에 입력하는 명령어가 가질 수 있는 단어의 최대 개수



//사용자 정의형
//super block 구조체
typedef struct
{
    //inode 부분
    unsigned inode_1 : 32;
    unsigned inode_2 : 32;
    unsigned inode_3 : 32;
    unsigned inode_4 : 32;
    
    //data block 부분
    unsigned data_block_1 : 32;
    unsigned data_block_2 : 32;
    unsigned data_block_3 : 32;
    unsigned data_block_4 : 32;
    unsigned data_block_5 : 32;
    unsigned data_block_6 : 32;
    unsigned data_block_7 : 32;
    unsigned data_block_8 : 32;
}SUPERBLOCK;

//inode 구조체
typedef struct
{
    unsigned type : 8;      //파일 종류
    
    unsigned year : 8;      //파일 생성 일자
    unsigned month : 8;     //파일 생성 일자
    unsigned date : 8;      //파일 생성 일자
    unsigned hour : 8;      //파일 생성 일자
    unsigned minute : 8;    //파일 생성 일자
    unsigned second : 8;    //파일 생성 일자

    unsigned size : 32;     //파일 크기

    unsigned dir_1 : 8;     //direct 블럭 번호
    unsigned dir_2 : 8;     //direct 블럭 번호
    unsigned dir_3 : 8;     //direct 블럭 번호
    unsigned dir_4 : 8;     //direct 블럭 번호
    unsigned dir_5 : 8;     //direct 블럭 번호
    unsigned dir_6 : 8;     //direct 블럭 번호
    unsigned dir_7 : 8;     //direct 블럭 번호
    unsigned dir_8 : 8;     //direct 블럭 번호

    unsigned indir : 8;     //single indirect 블럭 번호
}INODE;

//작업 디렉토리 경로를 저장할 node 구조체
typedef struct dir_list
{
    char *name;
    int inode;
    struct dir_list *next_ptr;
}DIR_LIST;



//전역변수 선언
extern DIR_LIST *front_dir_list_ptr;
extern DIR_LIST *rear_dir_list_ptr;



//함수 원형 선언
//mainfile
void shell(void);

//jhun
void mypwd(void);

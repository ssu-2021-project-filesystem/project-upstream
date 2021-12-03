//헤더파일
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>



//매크로
//block 별 크기(바이트 단위)
#define     BOOT_BLOCK_SIZE             2               //boot block의 총 크기
#define     SUPER_BLOCK_SIZE            48              //super block의 총 크기
#define     INODE_LIST_SIZE             (20 * 128)      //inode list의 총 크기(inode 하나의 크기 아님)

#define     DATA_BLOCK_SIZE             256             //data block의 block 하나의 크기

//명령어 문자열의 크기 지정
#define     COM_SEP_SIZE                50              //쉘에 입력하는 명령어에서 각 단어의 최대 크기(NULL 문자 포함)
#define     COM_SEP_NUM                 4               //쉘에 입력하는 명령어가 가질 수 있는 단어의 최대 개수



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
    unsigned size : 32;     //파일 크기

    unsigned type : 8;      //파일 종류
    
    unsigned year : 8;      //파일 생성 일자
    unsigned month : 8;     //파일 생성 일자
    unsigned date : 8;      //파일 생성 일자
    unsigned hour : 8;      //파일 생성 일자
    unsigned minute : 8;    //파일 생성 일자
    unsigned second : 8;    //파일 생성 일자

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

//작업 디렉토리의 절대 경로를 저장할 node 구조체
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
void myinode(const char *ptr);
void mydatablock(const char *ptr);
void command(void);
int int_pow(int a, int b);
void mytree(const char *path_ptr);
int path_to_inode(const char *path_ptr);
void dir_print(int inode, FILE *myfs);
char *current_dir_find(int inode, int high_inode, FILE *myfs);
void mymkfs(void);
void mymv(char *file_1, char *file_2);
void mv_to_dir(FILE *myfs, int file_1_inode, int file_2_inode, char *file_1_name, int count);

//insuk
int checkbit(int number, unsigned block);
void change_superblock(int saveinumber, int savedbnumber, SUPERBLOCK *sb_data);
void bit_print(unsigned a);
void mymkdir(char *dir_name);
void myrmdir(char *givenname);
void mystate(void);
void mytouch(char *givenname);

//jin
int stringtoint(char *byte);
void myls(const char *ptr);
void mycat(char *givename);
void myshowfile(char *startbyte, char *endbyte, char *givenname);

//oh
void mycd (char* path);
void mycpto (const char* source_file, const char* dest_file);
void mycpfrom (char* source_file, char* dest_file);
void mycp(char* source_file, char* dest_file);
void myrm(const char* file);
char* prtpwd();
int cntfound();
int acc_inode();
int acc_data();

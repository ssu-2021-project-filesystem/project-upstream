#include "user.h"


/*
이름    : mypwd 함수
작성자  : 이준혁
기능    : 작업 디렉토리의 절대경로를 출력한다
받는값  : X
리턴값  : X
*/
void mypwd(void)
{
    DIR_LIST *tmp_ptr = front_dir_list_ptr;

    int first = 0;

    while(1)
    {
        printf("%s", tmp_ptr->name);

        if(tmp_ptr->next_ptr == NULL)
        {
            break;
        }

        tmp_ptr = tmp_ptr->next_ptr;

        if(first == 0)
        {
            first++;
        }
        else
        {
            printf("/");
        }
    }

    return;
}


/*
이름    : myinode 함수
작성자  : 이준혁
기능    : 지정한 inode의 내용을 출력한다
받는값  : X
리턴값  : X
*/
void myinode(int inode)
{
    //파일 열기
    FILE *myfs;
    myfs = fopen("myfs.bin", "rb");

    //inode 정보 읽기
    INODE *inode_data_ptr = (INODE *)malloc(sizeof(INODE)); //inode 정보를 저장할 메모리 공간을 가리키는 포인터

    fseek(myfs, BOOT_BLOCK_SIZE + (sizeof(INODE) * (inode - 1)), SEEK_SET);
    fread(inode_data_ptr, sizeof(INODE), 1, myfs);

    //inode 정보 출력하기
    //종류
    printf("종류 : ");
    if((inode_data_ptr->type) == 0)
    {
        printf("디렉토리\n");
    }
    else
    {
        printf("파일\n");
    } 
    //생성일자
    printf("생성일자 : ");
    if(inode_data_ptr->year > 50)
    {
        printf("19%d/", inode_data_ptr->year);
    }
    else
    {
        printf("20%d/", inode_data_ptr->year);
    }
    printf("%02d/%02d ", inode_data_ptr->month, inode_data_ptr->date);
    printf("%02d:%02d:%02d\n", inode_data_ptr->hour, inode_data_ptr->minute, inode_data_ptr->second);

    //여기서부터 이어서 짜기-------------------------------------------
    //크기
    //test
    printf("크기 : %d\n", inode_data_ptr->size);
    //블럭
    printf("직접 블록 목록 : \n");




    //파일 닫기
    fclose(myfs);

    return;
}
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

    if (tmp_ptr == NULL)
    {
        printf("pwd를 위한 linked list가 존재하지 않습니다.\n");
        abort();
    }

    int first = 0;

    while (1)
    {
        printf("%s", tmp_ptr->name);

        if (tmp_ptr->next_ptr == NULL)
        {
            break;
        }
        else
        {
            tmp_ptr = tmp_ptr->next_ptr;
        }

        if (first == 0)
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
void myinode(char *ptr)
{
    // ptr이 가리키는 문자열을 정수로 전환, 올바른 값인지 검사
    int inode = 0;  // inode 번호
    int figure = 0; // ptr이 가리키고 있는 문자열의 자릿수
    int ptr_offset = 0;

    while (1)
    {
        if (*(ptr + ptr_offset) == 0)
        {
            break;
        }
        else
        {
            if ((*(ptr + ptr_offset) < 48) || (*(ptr + ptr_offset) > 57)) //정수 문자가 아닌 문자가 있는 경우
            {
                printf("inode 번호가 잘못되었습니다.\n");
                return;
            }
            else
            {
                ptr_offset++;
                figure++;
            }
        }
    }

    ptr_offset = 0;

    for (int i = (figure - 1); i >= 0; i--)
    {
        inode += (*(ptr + ptr_offset) - 48) * ((int)pow(10, i));

        ptr_offset++;
    }

    if ((inode > 128) || (inode < 1)) // 1~128인 정수가 아닌 경우
    {
        printf("inode 번호가 잘못되었습니다.\n");
        return;
    }

    //파일 열기
    FILE *myfs;
    myfs = fopen("myfs.bin", "rb");
    if (myfs == NULL)
    {
        printf("myinode() 함수 : 파일 열기에 실패했습니다.\n");
        abort();
    }

    // myfs 정보 읽기
    INODE *inode_data_ptr = (INODE *)malloc(sizeof(INODE)); // inode 정보를 저장할 메모리 공간을 가리키는 포인터
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (inode - 1)), SEEK_SET);
    fread(inode_data_ptr, sizeof(INODE), 1, myfs);

    SUPERBLOCK *sb_ptr = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
    fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
    fread(sb_ptr, sizeof(SUPERBLOCK), 1, myfs);

    // inode 사용 여부 확인하기
    unsigned mask;

    if (inode < 33) // inode_1에 정보가 들어있는 경우
    {
        mask = 1 << (inode - 1);
        if ((sb_ptr->inode_1 & mask) == 0)
        {
            printf("해당 inode는 사용 중이 아닙니다.\n");
            return;
        }
    }
    else if (inode < 65) // inode_2에 정보가 들어있는 경우
    {
        mask = 1 << ((inode - 32) - 1);
        if ((sb_ptr->inode_2 & mask) == 0)
        {
            printf("해당 inode는 사용 중이 아닙니다.\n");
            return;
        }
    }
    else if (inode < 97) // inode_3에 정보가 들어있는 경우
    {
        mask = 1 << ((inode - 64) - 1);
        if ((sb_ptr->inode_3 & mask) == 0)
        {
            printf("해당 inode는 사용 중이 아닙니다.\n");
            return;
        }
    }
    else // inode_4에 정보가 들어있는 경우
    {
        mask = 1 << ((inode - 96) - 1);
        if ((sb_ptr->inode_4 & mask) == 0)
        {
            printf("해당 inode는 사용 중이 아닙니다.\n");
            return;
        }
    }

    // inode 정보 출력하기
    //종류
    printf("종류 : ");
    if (inode_data_ptr->type == 0) //파일 종류가 0인 경우
    {
        printf("디렉토리\n");
    }
    else //파일 종류가 1인 경우
    {
        printf("파일\n");
    }
    //생성일자
    printf("생성일자 : ");
    if (inode_data_ptr->year > 50)
    {
        printf("19%d/", inode_data_ptr->year);
    }
    else
    {
        printf("20%d/", inode_data_ptr->year);
    }
    printf("%02d/%02d ", inode_data_ptr->month, inode_data_ptr->date);
    printf("%02d:%02d:%02d\n", inode_data_ptr->hour, inode_data_ptr->minute, inode_data_ptr->second);

    //크기
    printf("크기 : %d\n", inode_data_ptr->size);

    // direct
    printf("직접 블록 목록 : \n");

    if (inode_data_ptr->dir_1 != 0)
    {
        printf(" #0 직접 데이터 블록 : %d\n", inode_data_ptr->dir_1 + 1);
    }
    if (inode_data_ptr->dir_2 != 0)
    {
        printf(" #1 직접 데이터 블록 : %d\n", inode_data_ptr->dir_2 + 1);
    }
    if (inode_data_ptr->dir_3 != 0)
    {
        printf(" #2 직접 데이터 블록 : %d\n", inode_data_ptr->dir_3 + 1);
    }
    if (inode_data_ptr->dir_4 != 0)
    {
        printf(" #3 직접 데이터 블록 : %d\n", inode_data_ptr->dir_4 + 1);
    }
    if (inode_data_ptr->dir_5 != 0)
    {
        printf(" #4 직접 데이터 블록 : %d\n", inode_data_ptr->dir_5 + 1);
    }
    if (inode_data_ptr->dir_6 != 0)
    {
        printf(" #5 직접 데이터 블록 : %d\n", inode_data_ptr->dir_6 + 1);
    }
    if (inode_data_ptr->dir_7 != 0)
    {
        printf(" #6 직접 데이터 블록 : %d\n", inode_data_ptr->dir_7 + 1);
    }
    if (inode_data_ptr->dir_8 != 0)
    {
        printf(" #7 직접 데이터 블록 : %d\n", inode_data_ptr->dir_8 + 1);
    }

    // single indirect
    printf("간접 블록 번호 : %d\n", inode_data_ptr->indir);

    //동적 메모리 할당 공간 반납
    free(inode_data_ptr);
    free(sb_ptr);

    //파일 닫기
    fclose(myfs);

    return;
}
#include "user.h"


/*
이름    : myls 함수
작성자  : 이우진
기능    : 파일 목록 자세히 보기
받는값  : 파일 명
리턴값  : X
*/
void myls(const char* filename)
{
    // 파일 열기
    FILE *myfs;
    myfs = fopen("myfs", "rb");

    //현재 디렉토리에서 인자로 입력받은 디렉토리명의 inode 번호 찾기

    int saveinode, count, saveinumber;
    int *fileinode = (int *)malloc(sizeof(int));
    saveinode = rear_dir_list_ptr->inode;

    INODE *presenti_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinode - 1), SEEK_SET);
    fread(presenti_data, sizeof(INODE), 1, myfs);

    int n = presenti_data->size / (8 + sizeof(int));
    char *filename = (char *)malloc(sizeof(char) * 8);

    int none_tmp = 0;

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (presenti_data->dir_1)), SEEK_SET);
    for (int i = 0; i < n; i++)
    {
        fread(filename, 8, 1, myfs);
        fread(fileinode, sizeof(int), 1, myfs);

        if (strcmp(file, filename) == 0)
        {
            count = i;
            break;
        }
        else
        {
            none_tmp++;
        }
    }

    if (none_tmp == n) //해당 이름의 파일이 존재하지 않는 경우
    {
        printf("해당 이름의 파일이 존재하지 않습니다.\n");

        return;
    }
    else //해당 이름의 파일이 존재하는 경우
    {
        //해당 파일이 일반 파일인지 검사
        INODE *file_inode_tmp_ptr = (INODE *)malloc(sizeof(INODE));
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (*fileinode - 1), SEEK_SET);
        fread(file_inode_tmp_ptr, sizeof(INODE), 1, myfs);
        if (file_inode_tmp_ptr->type == 1) //해당 파일이 일반 파일인 경우
        {
            printf("해당 파일은 디렉토리가 아닙니다.\n");

            free(file_inode_tmp_ptr);
            free(fileinode);
            free(presenti_data);
            free(filename);

            return;
        }
        else //해당 파일이 디렉토리인 경우
        {
            free(file_inode_tmp_ptr);
        }
    }

    //해당 inode의 datablock 번호 찾기

    INODE *file_d = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (*fileinode - 1), SEEK_SET);
    fread(file_d, sizeof(INODE), 1, myfs);
    int data_num = (int)(file_d->dir_1 + 1);


    // datablock에서 각각의 파일 찾아내고 그 다음 모든 파일의 inode로 세부사항 접근

    n = file_d->size / (8 + sizeof(int));


    for(int i = count; i < n; i++)
    {
        int i = 0;
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + ((DATA_BLOCK_SIZE * (data_num)) + i), SEEK_SET);
        fread(filename, 8, 1, myfs);
        fread(fileinode, sizeof(int), 1, myfs);

        // 파일명
        printf("%s" ,filename);

        // inode list 정보 읽기
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (*fileinode - 1)), SEEK_SET);
        INODE *inode_ptr = (INODE *)malloc(sizeof(INODE));
        fread(inode_ptr, sizeof(INODE), 1, myfs);

        // 생성일자
        printf("    생성일자 : ");
        
        if (inode_ptr->year > 50)
        {
            printf(" 19%d/", inode_ptr->year);
        }
        else
        {
            printf(" 20%d/", inode_ptr->year);
        }
        printf("%d/%d ", inode_ptr->month, inode_ptr->date);
        printf("%d:%d:%d", inode_ptr->hour, inode_ptr->minute, inode_ptr->second);


        // 종류
        printf("종류 : ");

        if (inode_ptr->type == 0) // 파일 종류 0 = 디렉토리
        {
            printf(" 디렉토리");
        }
        else // 파일 종류 1 = 일반 파일
        {
            printf(" 파일");
        }

        // inode 번호
        printf("    inode번호 : %d", inode_ptr);


        // 크기
        printf("    크기 : %d\n", inode_ptr->size);

        fseek(myfs, -1, SEEK_CUR);
        fseek(myfs,sizeof(int),SEEK_CUR);

        i++;
    }


    fclose(myfs);

    return;
}
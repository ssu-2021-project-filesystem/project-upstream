#include "user.h"
/*
이름 : stringtoint함수
작성자 : 양인석
기능 : 문자열 정수로변환
받는값 : 문자열
출력값 : 정수
*/
int stringtoint(char *byte)
{
    int n;
    if(strlen(byte) == 1)
    {
        n = (*(byte) - 48);
    }
    else if(strlen(byte) == 2)
    {
        n = ((*(byte) - 48) * 10) + (*(byte + 1) - 48);
    }
    else if(strlen(byte) == 3)
    {
        n = ((*(byte) - 48) * 100) + ((*(byte + 1) - 48) * 10) + (*(byte + 2) - 48);
    }
    else if(strlen(byte) == 4)
    {
        n = ((*(byte) - 48) * 1000) + ((*(byte + 1) - 48) * 100) + ((*(byte + 2) - 48) * 10) + (*(byte + 3) - 48);
    }
    return n;
}


/*
이름    : myls 함수
작성자  : 양인석
기능    : 파일 목록 자세히 보기
받는값  : 파일 명
리턴값  : X
*/
void myls(const char* givenname)
{
    FILE *myfs;
    myfs = fopen("myfs", "rb");

    if(givenname == NULL)
    {
        //현재디렉터리의 파일목록출력
        //현재 디렉터리 아이노드 받기
        int saveinode = rear_dir_list_ptr->inode; //현재 디렉터리의 inode 번호 저장
        //현재 디렉터리의 inode 정보 불러오기
        INODE *presenti_data = (INODE *)malloc(sizeof(INODE));
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinode - 1), SEEK_SET);
        fread(presenti_data, sizeof(INODE), 1, myfs);
        //출력
        INODE *filei_data = (INODE *)malloc(sizeof(INODE));
        char *filename = (char *)malloc(sizeof(char) * 8);
        int *inodenumber = (int *)malloc(sizeof(int));

        int dir_file_num = presenti_data->size / (8 + sizeof(int));

        for (int i = 0; i < dir_file_num; i++)
        {
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * presenti_data->dir_1) + (i * (8+sizeof(int))), SEEK_SET);
            //현재디렉터리의 데이터블록에서 파일명과 아이노드번호 읽기
            fread(filename, 8, 1, myfs);
            fread(inodenumber, sizeof(int), 1, myfs);
            //읽은 아이노드번호로 아이노드 불러오기
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (*inodenumber - 1), SEEK_SET);
            fread(filei_data, sizeof(INODE), 1, myfs);
            //파일 업데이트 시간, 파일 유형, inode 번호, 파일 크기 출력
            //생성일자
            if (filei_data->year > 50)
            {
                printf("19%02d/", filei_data->year);
            }
            else
            {
                printf("20%02d/", filei_data->year);
            }
            printf("%02d/%02d ", filei_data->month, filei_data->date);
            printf("%02d:%02d:%02d\t", filei_data->hour, filei_data->minute, filei_data->second);
            //파일유형
            if(filei_data-> type == 0)
            {
                printf("directory\t");
            }
            else
            {
                printf("file     \t");
            }
            //inode 번호
            printf("%d\t", *inodenumber);
            //파일크기
            printf("%d byte\t", filei_data-> size);
            //파일명
            printf("%s\n", filename);
        }
    }
    else
    {
        //특정디렉터리의 파일목록출력
        //현재 디렉터리 아이노드 받기
        int saveinode = rear_dir_list_ptr->inode; //현재 디렉터리의 inode 번호 저장
        //현재 디렉터리의 inode 정보 불러오기
        INODE *presenti_data = (INODE *)malloc(sizeof(INODE));
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinode - 1), SEEK_SET);
        fread(presenti_data, sizeof(INODE), 1, myfs);

        INODE *filei_data = (INODE *)malloc(sizeof(INODE));
        char *filename = (char *)malloc(sizeof(char) * 8);
        int *inodenumber = (int *)malloc(sizeof(int));
        
        int dir_file_num = presenti_data->size / (8 + sizeof(int));
        //특정 디렉터리를 찾기
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * presenti_data->dir_1), SEEK_SET);
        for (int i = 0; i < dir_file_num; i++)
        {
            fread(filename, 8, 1, myfs);
            fread(inodenumber, sizeof(int), 1, myfs);

            if (strcmp(givenname, filename) == 0)
            {
                break;
            }
        }
        //특정 디렉터리의 아이노드 받기
        INODE *diri_data = (INODE *)malloc(sizeof(INODE));
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (*inodenumber - 1), SEEK_SET);
        fread(diri_data, sizeof(INODE), 1, myfs);
        //출력
        int dir_num = diri_data->size / (8 + sizeof(int));

        for (int i = 0; i < dir_num; i++)
        {
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * diri_data->dir_1) + (i * (8+sizeof(int))), SEEK_SET);
            //현재디렉터리의 데이터블록에서 파일명과 아이노드번호 읽기
            fread(filename, 8, 1, myfs);
            fread(inodenumber, sizeof(int), 1, myfs);
            //읽은 아이노드번호로 아이노드 불러오기
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (*inodenumber - 1), SEEK_SET);
            fread(filei_data, sizeof(INODE), 1, myfs);
            //파일 업데이트 시간, 파일 유형, inode 번호, 파일 크기 출력
            //생성일자
            if (filei_data->year > 50)
            {
                printf("19%02d/", filei_data->year);
            }
            else
            {
                printf("20%02d/", filei_data->year);
            }
            printf("%02d/%02d ", filei_data->month, filei_data->date);
            printf("%02d:%02d:%02d\t", filei_data->hour, filei_data->minute, filei_data->second);
            //파일유형
            if(filei_data-> type == 0)
            {
                printf("directory\t");
            }
            else
            {
                printf("file     \t");
            }
            //inode 번호
            printf("%d\t", *inodenumber);
            //파일크기
            printf("%d byte\t", filei_data-> size);
            //파일명
            printf("%s\n", filename);
        }
    }
}
/*
이름    : mycat 함수
작성자  : 양인석
기능    : 파일 내용 출력 명령어
받는값  : 파일이름
리턴값  : X
*/
void mycat(char *givenname)
{
    //예외처리
    if (givenname == NULL)
    {
        printf("파일명이 필요합니다.\n");
        return;
    }

    if (strlen(givenname) > 7)
    {
        printf("문자열이 너무 깁니다. (최대 7글자)\n");

        return;
    }

    FILE *myfs;
    myfs = fopen("myfs", "rb+");
    
    //현재 디렉터리 아이노드 받기
    int saveinode = rear_dir_list_ptr->inode; //현재 디렉토리의 inode 번호 저장
    //현재 디렉토리의 inode 정보 불러오기
    INODE *presenti_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinode - 1), SEEK_SET);
    fread(presenti_data, sizeof(INODE), 1, myfs);
    
    //파일명이 같은 파일이 있는지 검사
    char *filename = (char *)malloc(sizeof(char) * 8);
    int *inodenumber = (int *)malloc(sizeof(int));
    int saveinumber;
    unsigned count = 0;

    int dir_file_num = presenti_data->size / (8 + sizeof(int));

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * presenti_data->dir_1), SEEK_SET);
    for (int i = 0; i < dir_file_num; i++)
    {
        fread(filename, 8, 1, myfs);
        fread(inodenumber, sizeof(int), 1, myfs);

        if (strcmp(givenname, filename) == 0)
        {
            saveinumber = *inodenumber;
            break;
        }
        else
        {
            count++;
        }
    }
    //읽을 파일의 아이노드 받기
    INODE *i_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinumber - 1), SEEK_SET);
    fread(i_data, sizeof(INODE), 1, myfs);

    //이진파일내용 읽어서 출력
    char *datablock_ptr = (char *)malloc(sizeof(char));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (i_data-> dir_1)), SEEK_SET);
    for(int i = 0; i < i_data-> size; i++)
    {
        fread(datablock_ptr, sizeof(char), 1, myfs);

        if(*datablock_ptr == -1)
        {
            break;
        }
        else
        {
            printf("%c", *datablock_ptr);
        }
    }
    
    free(presenti_data);
    free(filename);
    free(inodenumber);
    free(i_data);
    free(datablock_ptr);
    return;
}

/*
이름    : myshowfile 함수
작성자  : 양인석
기능    : 파일 지정된 부분 출력 명령어
받는값  : 바이트시작값, 바이트종료값, 파일명
리턴값  : X
*/
void myshowfile(char *startbyte, char *endbyte, char *givenname)
{
    //문자열 정수로 바꾸기
    int intstartbyte = stringtoint(startbyte);
    int intendbyte = stringtoint(endbyte);
    printf("%d\n%d\n", intstartbyte, intendbyte);

    //예외처리
    if (givenname == NULL)
    {
        printf("파일명이 필요합니다.\n");
        return;
    }

    if (strlen(givenname) > 7)
    {
        printf("문자열이 너무 깁니다. (최대 7글자)\n");

        return;
    }

    FILE *myfs;
    myfs = fopen("myfs", "rb+");
    
    //현재 디렉터리 아이노드 받기
    int saveinode = rear_dir_list_ptr->inode; //현재 디렉토리의 inode 번호 저장
    //현재 디렉토리의 inode 정보 불러오기
    INODE *presenti_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinode - 1), SEEK_SET);
    fread(presenti_data, sizeof(INODE), 1, myfs);
    
    //파일명이 같은 파일이 있는지 검사
    char *filename = (char *)malloc(sizeof(char) * 8);
    int *inodenumber = (int *)malloc(sizeof(int));
    int saveinumber;
    unsigned count = 0;

    int dir_file_num = presenti_data->size / (8 + sizeof(int));

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * presenti_data->dir_1), SEEK_SET);
    for (int i = 0; i < dir_file_num; i++)
    {
        fread(filename, 8, 1, myfs);
        fread(inodenumber, sizeof(int), 1, myfs);

        if (strcmp(givenname, filename) == 0)
        {
            saveinumber = *inodenumber;
            break;
        }
        else
        {
            count++;
        }
    }
    //읽을 파일의 아이노드 받기
    INODE *i_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinumber - 1), SEEK_SET);
    fread(i_data, sizeof(INODE), 1, myfs);

    //특정부분 읽은 후 출력
    char *datablock_ptr = (char *)malloc(sizeof(char));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (i_data-> dir_1)) + (intstartbyte - 1), SEEK_SET);
    for(int i = intstartbyte; i < intendbyte; i++)
    {
        fread(datablock_ptr, sizeof(char), 1, myfs);

        if(*datablock_ptr == -1)
        {
            break;
        }
        else
        {
            printf("%c", *datablock_ptr);
        }
    }
    
    free(presenti_data);
    free(filename);
    free(inodenumber);
    free(i_data);
    free(datablock_ptr);
    return;
}
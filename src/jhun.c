//헤더파일
#include "user.h"


//매크로
#define     COMMAND_SIZE            30      //command() 함수에서 입력받을 수 있는 명령어의 길이


//함수
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
받는값  : inode 번호(문자열)
리턴값  : X
*/
void myinode(const char *ptr)
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

    ptr_offset = 0; //변수 재사용을 위해 초기화

    for (int i = (figure - 1); i >= 0; i--)
    {
        inode += (*(ptr + ptr_offset) - 48) * (int_pow(10, i));

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

    // myfs에서 superblock 정보 읽기
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
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (inode < 65) // inode_2에 정보가 들어있는 경우
    {
        mask = 1 << ((inode - 32) - 1);
        if ((sb_ptr->inode_2 & mask) == 0)
        {
            printf("해당 inode는 사용 중이 아닙니다.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (inode < 97) // inode_3에 정보가 들어있는 경우
    {
        mask = 1 << ((inode - 64) - 1);
        if ((sb_ptr->inode_3 & mask) == 0)
        {
            printf("해당 inode는 사용 중이 아닙니다.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else // inode_4에 정보가 들어있는 경우
    {
        mask = 1 << ((inode - 96) - 1);
        if ((sb_ptr->inode_4 & mask) == 0)
        {
            printf("해당 inode는 사용 중이 아닙니다.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }

    // myfs에서 inode list 정보 읽기
    INODE *inode_data_ptr = (INODE *)malloc(sizeof(INODE)); // inode 정보를 저장할 메모리 공간을 가리키는 포인터
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (inode - 1)), SEEK_SET);
    fread(inode_data_ptr, sizeof(INODE), 1, myfs);

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


/*
이름    : mydatablock 함수
작성자  : 이준혁
기능    : 지정된 데이터 블록에 들어있는 내용을 출력한다
받는값  : datablock 번호(문자열)
리턴값  : X
*/
void mydatablock(const char *ptr)
{
    // ptr이 가리키는 문자열을 정수로 전환, 올바른 값인지 검사
    int datablock = 0;  // datablock 번호
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
                printf("datablock 번호가 잘못되었습니다.\n");
                return;
            }
            else
            {
                ptr_offset++;
                figure++;
            }
        }
    }

    ptr_offset = 0; //변수 재사용을 위해 초기화

    for (int i = (figure - 1); i >= 0; i--)
    {
        datablock += (*(ptr + ptr_offset) - 48) * (int_pow(10, i));

        ptr_offset++;
    }

    if ((datablock > 256) || (datablock < 1)) // 1~256인 정수가 아닌 경우
    {
        printf("datablock 번호가 잘못되었습니다.\n");
        return;
    }

    //파일 열기
    FILE *myfs;
    myfs = fopen("myfs.bin", "rb");
    if (myfs == NULL)
    {
        printf("mydatablock() 함수 : 파일 열기에 실패했습니다.\n");
        abort();
    }

    // myfs에서 superblock 정보 읽기
    SUPERBLOCK *sb_ptr = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
    fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
    fread(sb_ptr, sizeof(SUPERBLOCK), 1, myfs);

    // datablock 사용 여부 확인하기
    unsigned mask;

    if (datablock < (32 * 1 + 1)) // data_block_1에 정보가 들어있는 경우
    {
        mask = 1 << (datablock - 1);
        if ((sb_ptr->data_block_1 & mask) == 0)
        {
            printf("해당 datablock은 사용 중이 아닙니다.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 2 + 1)) // data_block_2에 정보가 들어있는 경우
    {
        mask = 1 << ((datablock - (32 * 1)) - 1);
        if ((sb_ptr->data_block_2 & mask) == 0)
        {
            printf("해당 datablock은 사용 중이 아닙니다.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 3 + 1)) // data_block_3에 정보가 들어있는 경우
    {
        mask = 1 << ((datablock - (32 * 2)) - 1);
        if ((sb_ptr->data_block_3 & mask) == 0)
        {
            printf("해당 datablock은 사용 중이 아닙니다.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 4 + 1)) // data_block_4에 정보가 들어있는 경우
    {
        mask = 1 << ((datablock - (32 * 3)) - 1);
        if ((sb_ptr->data_block_4 & mask) == 0)
        {
            printf("해당 datablock은 사용 중이 아닙니다.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 5 + 1)) // data_block_5에 정보가 들어있는 경우
    {
        mask = 1 << ((datablock - (32 * 4)) - 1);
        if ((sb_ptr->data_block_5 & mask) == 0)
        {
            printf("해당 datablock은 사용 중이 아닙니다.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 6 + 1)) // data_block_6에 정보가 들어있는 경우
    {
        mask = 1 << ((datablock - (32 * 5)) - 1);
        if ((sb_ptr->data_block_6 & mask) == 0)
        {
            printf("해당 datablock은 사용 중이 아닙니다.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 7 + 1)) // data_block_7에 정보가 들어있는 경우
    {
        mask = 1 << ((datablock - (32 * 6)) - 1);
        if ((sb_ptr->data_block_7 & mask) == 0)
        {
            printf("해당 datablock은 사용 중이 아닙니다.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 8 + 1)) // data_block_8에 정보가 들어있는 경우
    {
        mask = 1 << ((datablock - (32 * 7)) - 1);
        if ((sb_ptr->data_block_8 & mask) == 0)
        {
            printf("해당 datablock은 사용 중이 아닙니다.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }

    //datablock 출력
    char *datablock_ptr = (char *)malloc(sizeof(char));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (datablock - 1)), SEEK_SET);

    for(int i = 0; i < 256; i++)
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

    printf("\n");
    
    //동적 메모리 할당 공간 반납
    free(sb_ptr);
    free(datablock_ptr);

    //파일 닫기
    fclose(myfs);

    return;
}


/*
이름    : command 함수
작성자  : 이준혁
기능    : linux 명령어를 실행한다
받는값  : X
리턴값  : X
*/
void command(void)
{
    //linux 명령어 입력
    char *command_ptr = (char *)malloc(sizeof(char) * COMMAND_SIZE);
    char tmp_char;
    int loop_char_num = 0;

    printf("linux 명령어를 입력하세요 : ");

    while((tmp_char = getchar()) != '\n')
    {
        *(command_ptr + loop_char_num) = tmp_char;

        loop_char_num++;
    }
    *(command_ptr + loop_char_num) = 0; //NULL 배정

    //linux 명령어 실행
    system(command_ptr);

    return;
}


/*
이름    : 거듭제곱 함수
작성자  : 이준혁
기능    : 작성한 두 인자로 거듭제곱을 수행한다
받는값  : 거듭제곱을 수행할 두 정수
리턴값  : 거듭제곱 계산 결과값
*/
int int_pow(int a, int b) //a의 b 거듭제곱을 리턴한다
{
    int value = 1;

    for(int i = 0; i < b; i++)
    {
        value *= a;
    }

    return value;
}


/*
이름    : mytree 함수
작성자  : 이준혁
기능    : 디렉토리 구조를 출력한다
받는값  : 구조를 출력할 디렉토리의 절대경로
리턴값  : X
*/
void mytree(const char *path_ptr)
{
    int tree_inode; //구조를 출력할 디렉토리의 inode 번호를 저장할 변수

    //명령어로부터 전달된 인자 처리
    if(path_ptr == NULL) //인자가 입력되지 않은 경우
    {
        tree_inode = rear_dir_list_ptr->inode; //현재 디렉토리의 inode 배정
    }
    else //인자가 입력된 경우
    {
        tree_inode = path_to_inode(path_ptr); //해당 디렉토리의 inode를 배정

        //경로가 잘못된 경우에 대한 예외처리
        if(tree_inode == 0)
        {
            printf("잘못된 경로입니다.\n");

            return;
        }
    }

    //디렉토리 구조 출력
    

    return;
}


/*
이름    : path_to_inode 함수
작성자  : 이준혁
기능    : 절대경로를 받아서 해당 디렉토리의 inode 번호를 리턴한다
받는값  : 
리턴값  : X
*/
int path_to_inode(const char *path_ptr)
{
    int inode = 1; //inode 번호를 저장할 변수. 기본값 1(root)
    
    int tmp_datablock; //datablock 번호를 저장할 변수
    
    INODE *inode_ptr = (INODE *)malloc(sizeof(INODE)); //inode를 가리킬 포인터

    char *tmp_cmd_string_ptr = (char *)malloc(sizeof(char) * 8); //path에서 추출한 디렉토리명을 가리킬 포인터
    
    char *tmp_dir_string_ptr = (char *)malloc(sizeof(char) * 8); //디렉토리의 datablock에서 추출한 디렉토리명을 가리킬 포인터
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //디렉토리의 datablock에서 추출한 inode 번호를 가리킬 포인터

    FILE *myfs;
    myfs = fopen("myfs.bin", "rb");

    path_ptr++; //루트(/) 넘어가기

    while(1)
    {
        //path에서 문자열 분리
        if(sscanf(path_ptr ,"%[^/\\0]", tmp_cmd_string_ptr) != 1) //tmp_string_ptr에는 디렉토리명이 저장되고, 맨 끝에는 NULL이 들어감
        {
            break;
        }

        path_ptr = path_ptr + (strlen(tmp_cmd_string_ptr) + 1);

        //inode의 디렉토리로 이동, 해당 datablock으로 이동
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (inode - 1)), SEEK_SET);
        fread(inode_ptr, sizeof(INODE), 1, myfs);

        tmp_datablock = (int)(inode_ptr->dir_1 + 1);
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (256 * (tmp_datablock - 1)), SEEK_SET);

        //datablock의 디렉토리명 추출, tmp_cmd_string_ptr과 비교
        int diff_error = 0;
        for(int i = 0; i < (inode_ptr->size) / (8 + sizeof(int)); i++)
        {
            fread(tmp_dir_string_ptr, 8, 1, myfs);
            
            if(strcmp(tmp_dir_string_ptr, tmp_cmd_string_ptr) == 0) //두 문자열이 같은 경우
            {
                fread(tmp_inode_ptr, sizeof(int), 1, myfs);
                inode = *tmp_inode_ptr;

                break;
            }
            else //두 문자열이 다른 경우
            {
                fread(tmp_inode_ptr, sizeof(int), 1, myfs); //inode 번호 건너뛰기
                diff_error++;
            }
        }

        if(diff_error == ((inode_ptr->size) / (8 + sizeof(int))))
        {
            free(tmp_cmd_string_ptr);
            free(inode_ptr);
            free(tmp_dir_string_ptr);
            free(tmp_inode_ptr);

            fclose(myfs);

            return 0;
        }
    }

    free(tmp_cmd_string_ptr);
    free(inode_ptr);
    free(tmp_dir_string_ptr);
    free(tmp_inode_ptr);

    fclose(myfs);

    return inode;
}


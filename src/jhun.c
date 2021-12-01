//헤더파일
#include "user.h"


//매크로
#define     COMMAND_SIZE            30      //command() 함수에서 입력받을 수 있는 명령어의 길이


//전역변수
//move. 이 파일에서만 사용한다. 문제 있으면 user.h에 선언 작성해두기
int mytree_dir_layer = 0;


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
    myfs = fopen("myfs", "rb");
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
    unsigned mask = 1 << 31;

    if (inode < 33) // inode_1에 정보가 들어있는 경우
    {
        mask >>= (inode - 1);
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
        mask >>= ((inode - 32) - 1);
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
        mask >>= ((inode - 64) - 1);
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
        mask >>= ((inode - 96) - 1);
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
        printf("19%02d/", inode_data_ptr->year);
    }
    else
    {
        printf("20%02d/", inode_data_ptr->year);
    }
    printf("%02d/%02d ", inode_data_ptr->month, inode_data_ptr->date);
    printf("%02d:%02d:%02d\n", inode_data_ptr->hour, inode_data_ptr->minute, inode_data_ptr->second);

    //크기
    printf("크기 : %d\n", inode_data_ptr->size);

    // direct
    printf("직접 블록 목록 : \n");

    printf(" #0 직접 데이터 블록 : %d\n", inode_data_ptr->dir_1 + 1);

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
    //인자가 작성되지 않은 경우
    if(ptr == NULL)
    {
        printf("인자를 입력하세요.\n");

        return;
    }

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
    myfs = fopen("myfs", "rb");
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
    unsigned mask = 1 << 31;

    if (datablock < (32 * 1 + 1)) // data_block_1에 정보가 들어있는 경우
    {
        mask >>= (datablock - 1);
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
        mask >>= ((datablock - (32 * 1)) - 1);
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
        mask >>= ((datablock - (32 * 2)) - 1);
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
        mask >>= ((datablock - (32 * 3)) - 1);
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
        mask >>= ((datablock - (32 * 4)) - 1);
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
        mask >>= ((datablock - (32 * 5)) - 1);
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
        mask >>= ((datablock - (32 * 6)) - 1);
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
        mask >>= ((datablock - (32 * 7)) - 1);
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
    FILE *myfs;
    myfs = fopen("myfs", "rb+");
    if (myfs == NULL)
    {
        printf("mytree() 함수 : 파일 열기에 실패했습니다.\n");
        abort();
    }

    dir_print(tree_inode, myfs);

    printf("\n");

    fclose(myfs);

    return;
}


/*
이름    : path_to_inode 함수
작성자  : 이준혁
기능    : 절대경로를 받아서 해당 디렉토리의 inode 번호를 리턴한다
받는값  : 디렉토리의 절대경로
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
    myfs = fopen("myfs", "rb");
    if (myfs == NULL)
    {
        printf("path_to_inode() 함수 : 파일 열기에 실패했습니다.\n");
        abort();
    }

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
        for(int i = 0; i < ((inode_ptr->size) / (8 + sizeof(int))); i++)
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


/*
이름    : dir_print 함수
작성자  : 이준혁
기능    : 재귀함수로 사용되어 디렉토리 구조를 출력한다
받는값  : inode 번호, myfs 파일포인터
리턴값  : X
*/
void dir_print(int inode, FILE *myfs)
{
    mytree_dir_layer++;

    INODE *inode_ptr = (INODE *)malloc(sizeof(INODE)); //inode를 가리킬 포인터
    INODE *inode_file_ptr = (INODE *)malloc(sizeof(INODE)); //디렉토리 내부 파일의 inode를 가리킬 포인터

    int tmp_datablock; //inode의 datablock 번호를 저장할 변수

    char *tmp_dir_string_ptr = (char *)malloc(sizeof(char) * 8); //디렉토리의 datablock에서 추출한 디렉토리명을 가리킬 포인터
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //디렉토리의 datablock에서 추출한 inode 번호를 가리킬 포인터

    //해당 inode 번호의 datablock 번호 추출
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (inode - 1)), SEEK_SET);
    fread(inode_ptr, sizeof(INODE), 1, myfs);
    tmp_datablock = (int)(inode_ptr->dir_1 + 1);

    //파일 종류 검사
    if(inode_ptr->type == 1) //일반 파일인 경우
    {
        printf("해당 파일은 일반 파일입니다.\n");

        return;
    }

    //inode에 해당하는 이름 출력
    for(int i = 0; i < (inode_ptr->size / (8 + sizeof(int))); i++)
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (256 * (tmp_datablock - 1)) + (i * (8 + sizeof(int))), SEEK_SET);
        fread(tmp_dir_string_ptr, 8, 1, myfs); //디렉토리명
        fread(tmp_inode_ptr, sizeof(int), 1, myfs); //inode

        if(strcmp(tmp_dir_string_ptr, "..") == 0)
        {
            //디렉토리명 출력
            if(inode == 1)
            {
                printf("%-7s", "/");
            }
            else
            {
                //..에서 inode 찾아서 디렉토리명 출력
                printf("%-7s", current_dir_find(inode, *tmp_inode_ptr, myfs));
            }
        }
    }

    //디렉토리 처리
    int first = 0;
    for(int i = 0; i < (inode_ptr->size / (8 + sizeof(int))); i++)
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (256 * (tmp_datablock - 1)) + (i * (8 + sizeof(int))), SEEK_SET);
        fread(tmp_dir_string_ptr, 8, 1, myfs); //디렉토리명
        fread(tmp_inode_ptr, sizeof(int), 1, myfs); //inode

        if(strcmp(tmp_dir_string_ptr, "..") == 0)
        {
            ;
        }
        else if(strcmp(tmp_dir_string_ptr, ".") == 0)
        {
            ;
        }
        else
        {
            //해당 파일이 디렉토리인지 검사
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (*tmp_inode_ptr - 1)), SEEK_SET);
            fread(inode_file_ptr, sizeof(INODE), 1, myfs);

            if(inode_file_ptr->type == 1) //해당 파일이 일반 파일인 경우
            {
                continue;
            }
            
            //화살표 출력
            if(first != 0) //첫 번째로 출력되는 디렉토리가 아닌 경우
            {
                printf("\n");

                for(int j = 0; j < mytree_dir_layer; j++)
                {
                    if(j == (mytree_dir_layer - 1)) //마지막에는 ->까지 출력
                    {
                        printf("       -> ");
                    }
                    else
                    {
                        printf("          ");
                    }
                }
            }
            else //첫 번째로 출력되는 디렉토리인 경우
            {
                printf("-> ");

                first++;
            }
            
            dir_print(*tmp_inode_ptr, myfs);
        }
    }


    free(inode_ptr);
    free(inode_file_ptr);
    free(tmp_dir_string_ptr);
    free(tmp_inode_ptr);

    mytree_dir_layer--;

    return;
}


/*
이름    : current_dir_find 함수
작성자  : 이준혁
기능    : 해당 디렉토리의 inode 번호와 부모 디렉토리의 inode 번호로 해당 디렉토리의 디렉토리명을 구한다
받는값  : inode 번호, 상위 디렉토리의 inode 번호, myfs 파일포인터
리턴값  : X
*/
char *current_dir_find(int inode, int high_inode, FILE *myfs)
{
    INODE *inode_ptr = (INODE *)malloc(sizeof(INODE)); //inode를 가리킬 포인터

    char *tmp_dir_string_ptr = (char *)malloc(sizeof(char) * 8); //디렉토리의 datablock에서 추출한 디렉토리명을 가리킬 포인터
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //디렉토리의 datablock에서 추출한 inode 번호를 가리킬 포인터

    int tmp_datablock; //inode의 datablock 번호를 저장할 변수

    //..의 디렉토리 리스트가 저장된 datablock 번호 추출
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (high_inode - 1)), SEEK_SET);
    fread(inode_ptr, sizeof(INODE), 1, myfs);
    tmp_datablock = (int)(inode_ptr->dir_1 + 1);

    //디렉토리 찾아서 리턴
    for(int i = 0; i < (inode_ptr->size / (8 + sizeof(int))); i++)
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (256 * (tmp_datablock - 1)) + (i * (8 + sizeof(int))), SEEK_SET);
        fread(tmp_dir_string_ptr, 8, 1, myfs); //디렉토리명
        fread(tmp_inode_ptr, sizeof(int), 1, myfs); //inode

        if(inode == *tmp_inode_ptr)
        {
            break;
        }
    }    

    return tmp_dir_string_ptr;
}


/*
이름    : mymkfs 함수
작성자  : 이준혁
기능    : 파일시스템을 만들고, 여러 값들을 초기화한다.
받는값  : X
리턴값  : X
*/
void mymkfs(void)
{
    //myfs 파일 존재 여부 확인
    int exist; //파일이 존재하지 않는 경우 0, 존재하는 경우 1
    FILE *myfs_exist;

    if((myfs_exist = fopen("myfs", "rb")) == NULL) //파일이 존재하지 않는 경우
    {
        exist = 0;
    }
    else //파일이 존재하는 경우
    {
        exist = 1;

        fclose(myfs_exist);
    }

    //fs 생성
    int new_fs; //새 파일시스템을 생성할 경우 1

    if(exist == 0) //파일이 존재하지 않는 경우
    {
        printf("파일시스템이 없습니다. 파일시스템을 만듭니다.\n");
        
        new_fs = 1;
    }
    else //파일이 존재하는 경우
    {
        char char_tmp;

        while(1)
        {
            printf("파일시스템이 있습니다. 다시 만들겠습니까? (y/n) ");
            char_tmp = getchar();

            rewind(stdin); //버퍼 비우기

            if((char_tmp == 'y') || (char_tmp == 'Y'))
            {
                printf("파일시스템을 다시 만들었습니다.\n");
                new_fs = 1;
                break;
            }
            else if((char_tmp == 'n') || (char_tmp == 'N'))
            {
                printf("파일시스템 재생성을 취소했습니다.\n");
                return;
            }
            else
            {
                printf("잘못된 입력입니다.\n");
            }
        }
    }

    if(new_fs == 1)
    {
        FILE *myfs;

        //myfs 파일 생성
        myfs = fopen("myfs", "wb");

        //superblock 초기화
        SUPERBLOCK *sb_ptr = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
        sb_ptr->inode_1 = 0;
        sb_ptr->inode_2 = 0;
        sb_ptr->inode_3 = 0;
        sb_ptr->inode_4 = 0;
        sb_ptr->data_block_1 = 0;
        sb_ptr->data_block_2 = 0;
        sb_ptr->data_block_3 = 0;
        sb_ptr->data_block_4 = 0;
        sb_ptr->data_block_5 = 0;
        sb_ptr->data_block_6 = 0;
        sb_ptr->data_block_7 = 0;
        sb_ptr->data_block_8 = 0;

        fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
        fwrite(sb_ptr, sizeof(SUPERBLOCK), 1, myfs);

        fclose(myfs);

        //root 디렉토리 생성
        mymkdir("/");
    }

    return;
}


/*
이름    : mymv 함수
작성자  : 양인석, 이준혁
기능    : 파일의 이름을 바꾸거나 다른 디렉토리로 이동시킨다
받는값  : X
리턴값  : X
*/
void mymv(char *file_1, char *file_2)
{
    //인자가 작성되지 않은 경우
    if((file_1 == NULL) || (file_2 == NULL))
    {
        printf("인자를 입력하세요.\n");

        return;
    }

    FILE *myfs;
    myfs = fopen("myfs", "rb+");

    //현재 디렉토리의 inode 정보 읽기
    int saveinode = rear_dir_list_ptr->inode; //현재 디렉토리의 inode 번호를 저장할 변수
    INODE *i_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (saveinode - 1), SEEK_SET);
    fread(i_data, sizeof(INODE), 1, myfs);

    int number_of_file = (i_data->size / (8 + sizeof(int)));

    //file_2 찾기
    char *tmp_filename = (char *)malloc(sizeof(char) * 8);
    int *tmp_inodenumber = (int *)malloc(sizeof(int));
    
    int dir_inode = 0;
    int filetype = 1; //두 번째 인자로 작성한 파일의 종류를 저장할 변수. 0이면 디렉토리 1이면 일반 파일
    INODE *f_inode = (INODE *)malloc(sizeof(INODE)); //현재 디렉토리 내부 파일의 inode 정보를 저장할 변수

    for(int i = 0; i < number_of_file; i++) //디렉토리 내의 파일 개수만큼의 횟수로 루프 생성
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * i_data->dir_1) + (i * (8 + sizeof(int))), SEEK_SET);
        fread(tmp_filename, 8, 1, myfs); //첫 번째 인자에 들어온 파일의 이름
        fread(tmp_inodenumber, sizeof(int), 1, myfs); //첫 번째 인자에 들어온 파일의 inode

        if(strcmp(file_2, tmp_filename) == 0)
        {
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (*tmp_inodenumber - 1)), SEEK_SET);
            fread(f_inode, sizeof(INODE), 1, myfs);

            if(f_inode->type == 0) //해당 파일이 디렉토리인 경우
            {
                filetype = 0;
                dir_inode = *tmp_inodenumber;
                
                break;
            }
            else //해당 파일이 일반 파일인 경우
            {
                printf("%s 은/는 이미 존재하는 파일입니다.\n", file_2);

                free(i_data);
                free(tmp_filename);
                free(tmp_inodenumber);
                free(f_inode);

                fclose(myfs);

                return;
            }
        }
    }

    //file_1 찾기
    int file_inode = 0;
    int count = 0; //현재 디렉토리에서, 첫 번째 인자로 작성한 파일의 위치를 저장할 변수

    for(int i = 0; i < number_of_file; i++) //디렉토리 내의 파일 개수만큼의 횟수로 루프 생성
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * i_data->dir_1) + (i * (8 + sizeof(int))), SEEK_SET);
        fread(tmp_filename, 8, 1, myfs); //첫 번째 인자에 들어온 파일의 이름
        fread(tmp_inodenumber, sizeof(int), 1, myfs); //첫 번째 인자에 들어온 파일의 inode

        if(strcmp(file_1, tmp_filename) == 0)
        {
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (*tmp_inodenumber - 1)), SEEK_SET);
            fread(f_inode, sizeof(INODE), 1, myfs);

            if(f_inode->type == 0) //디렉토리인 경우
            {
                printf("%s 은/는 디렉토리입니다.\n", file_1);

                free(i_data);
                free(tmp_filename);
                free(tmp_inodenumber);
                free(f_inode);

                return;
            }
            else //일반 파일인 경우
            {
                file_inode = *tmp_inodenumber;

                break;   
            }
        }
        else
        {
            count++;
        }
    }

    if(file_inode == 0) //file_1과 동일한 파일이 존재하지 않는 경우
    {
        printf("%s 이/가 존재하지 않습니다.\n", file_1);

        return;
    }

    //작업 수행
    if(filetype == 1) //해당 파일이 일반 파일인 경우(source_file, dest_file인 경우)
    {
        //file_2로 이름 변경
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * i_data->dir_1) + ((8 + sizeof(int)) * count), SEEK_SET);
        fwrite(file_2, 8, 1, myfs);
    }
    else //해당 파일이 디렉토리인 경우(file, directory인 경우)
    {
        //file_2 디렉토리로 해당 파일 이동
        mv_to_dir(myfs, file_inode, dir_inode, file_1, count);
    }

    free(i_data);
    free(tmp_filename);
    free(tmp_inodenumber);
    free(f_inode);

    fclose(myfs);
    
    return;
}


/*
이름    : mv_to_dir 함수
작성자  : 이준혁
기능    : 파일을 다른 디렉토리로 이동시킨다
받는값  : myfs 파일포인터, file_1의 inode, file_2의 inode, file_1의 이름, count 변수
리턴값  : X
*/
void mv_to_dir(FILE *myfs, int file_1_inode, int file_2_inode, char *file_1_name, int count)
{
    //현재 디렉토리의 inode 정보 읽기
    int saveinode = rear_dir_list_ptr->inode; //현재 디렉토리의 inode 번호를 저장할 변수
    INODE *i_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (saveinode - 1), SEEK_SET);
    fread(i_data, sizeof(INODE), 1, myfs);

    int number_of_file = (i_data->size / (8 + sizeof(int)));

    //현재 디렉토리 datablock에서 file_1 제거
    char *tmp_filename = (char *)malloc(sizeof(char) * 8);
    int *tmp_inodenumber = (int *)malloc(sizeof(int));

    for(int i = count; i < (number_of_file - 1); i++)
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * i_data->dir_1) + ((i + 1) * (8 + sizeof(int))), SEEK_SET);
        fread(tmp_filename, 8, 1, myfs);
        fread(tmp_inodenumber, sizeof(int), 1, myfs);

        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * i_data->dir_1) + (i * (8 + sizeof(int))), SEEK_SET);
        fwrite(tmp_filename, 8, 1, myfs);
        fwrite(tmp_inodenumber, sizeof(int), 1, myfs);
    }

    //맨 마지막에 -1 저장
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * i_data->dir_1) + ((number_of_file - 1) * (8 + sizeof(int))), SEEK_SET);
    char *tmp_char = (char *)malloc(sizeof(char));
    *tmp_char = -1;
    fwrite(tmp_char, sizeof(char), 1, myfs);

    //현재 디렉토리의 크기 수정
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (saveinode - 1), SEEK_SET);
    i_data->size = i_data->size - (8 + sizeof(int));
    fwrite(i_data, sizeof(INODE), 1, myfs);

    //file_2 디렉토리 datablock의 맨 마지막 위치로 이동, file_1 추가
    INODE *dir_inode = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (file_2_inode - 1), SEEK_SET);
    fread(dir_inode, sizeof(INODE), 1, myfs);

    int *tmp_int = (int *)malloc(sizeof(int));
    *tmp_int = file_1_inode;

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * dir_inode->dir_1) + dir_inode->size, SEEK_SET);
    fwrite(file_1_name, 8, 1, myfs);
    fwrite(tmp_int, sizeof(int), 1, myfs);

    //맨 마지막에 -1 저장
    fwrite(tmp_char, sizeof(char), 1, myfs);

    //file_2 디렉토리의 크기 수정
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (file_2_inode - 1), SEEK_SET);
    dir_inode->size = dir_inode->size + (8 + sizeof(int));
    fwrite(dir_inode, sizeof(INODE), 1, myfs);

    free(i_data);
    free(tmp_filename);
    free(tmp_inodenumber);
    free(tmp_char);
    free(dir_inode);
    free(tmp_int);

    return;
}

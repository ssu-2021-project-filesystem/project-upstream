#include "user.h"


/*
이름    : myls 함수
작성자  : 이우진
기능    : 파일 목록 자세히 보기
받는값  : X
리턴값  : X
*/
void myls(const char *ptr)
{
    int inode = 0;  // inode 번호
    int num = 0; // ptr이 가리키고 있는 문자열의 자릿수
    int ptr_a = 0;

    while (1)
    {
        if (*(ptr + ptr_a) == 0)
        {
            break;
        }
        else
        {
            if ((*(ptr + ptr_a) < 48) || (*(ptr + ptr_a) > 57)) // 정수 문자가 아닌 문자가 있는 경우
            {
                printf("inode 번호가 잘못되었습니다.\n");
                return;
            }
            else
            {
                ptr_a++;
                num++;
            }
        }
    }

    ptr_a = 0; // 변수 재사용을 위해 초기화

    for (int i = (num - 1); i >= 0; i--)
    {
        inode += (*(ptr + ptr_a) - 48) * (int_pow(10, i));

        ptr_a;
    }

    if ((inode < 1) || (inode > 128)) // 1~128인 정수가 아닌 경우
    {
        printf("inode 번호가 잘못되었습니다.\n");
        return;
    }

    // 파일 열기
    FILE *myfs;
    myfs = fopen("myfs", "rb");

    if(myfs == NULL)
    {
        printf("파일 열기에 실패했습니다.\n");
        abort();
    }

    // superblock 정보 읽기
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


    while(inode < 128 + 1 ) // 파일 업데이트 시간, 파일 유형, inode 번호, 파일 크기
    {
        // inode list 정보 읽기
        INODE *inode_ptr = (INODE *)malloc(sizeof(INODE)); 
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (inode - 1)), SEEK_SET);
        fread(inode_ptr, sizeof(INODE), 1, myfs);


        // 생성일자
        printf("    생성일자 : ");
        
        if (inode_ptr->year > 50)
        {
            printf(" %d/", inode_ptr->year);
        }
        else
        {
            printf(" %d/", inode_ptr->year);
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

        printf("    inode번호 : %d", inode);


        // 크기
        printf("    크기 : %d\n", inode_ptr->size);

        free(inode_ptr);

        inode++;
    }
    
    free(sb_ptr);
    fclose(myfs);

    return;
}




/*
이름    : mycat 함수
작성자  : 이우진
기능    : 파일 내용 출력 명령어
받는값  : X
리턴값  : X
*/
void mycat(char *file)
{
    FILE *myfs;
    myfs = fopen("myfs", "rb");

    INODE *inode_data = (INODE *)malloc(sizeof(INODE));
    int num = rear_dir_list_ptr->inode; //현재 디렉토리의 inode 번호를 저장할 변수
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (num - 1), SEEK_SET);
    fread(inode_data, sizeof(INODE), 1, myfs);
    int file_num = (inode_data->size / (8 + sizeof(int)));
        

    // 파일 찾기
    int dir_inode = 0;

    INODE *f_inode = (INODE *)malloc(sizeof(INODE)); //현재 디렉토리 내부 파일의 inode 정보를 저장할 변수
    char *fn = (char *)malloc(sizeof(char) * 8);  // filename
    int *in = (int *)malloc(sizeof(int));// inodenumber

    for(int i = 0; i < file_num; i++) 
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * inode_data->dir_1) + (i * (8 + sizeof(int))), SEEK_SET);
        fread(fn, 8, 1, myfs); 
        fread(in, sizeof(int), 1, myfs); 

        if(strcmp(file, fn) == 0)
        {
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (*in - 1)), SEEK_SET);
            fread(f_inode, sizeof(INODE), 1, myfs);

            if(f_inode->type == 0) //디렉토리인 경우
            {
                printf("%s 은/는 디렉토리입니다.\n", file);

                free(inode_data);
                free(fn);
                free(in);
                free(f_inode);

                return;
            }
            else //일반 파일인 경우
            {
                int number = *in;
                if(number == 0)
                {
                    printf("%s 이/가 존재하지 않습니다.\n", file);

                    return;
                }

                else // 출력
                {
                    char *test;
                    fread(test,sizeof(file),1,myfs);
                    printf("%s", test);
                }


                break;   
            }
        }
    }

    free(inode_data);
    free(fn);
    free(in);
    free(f_inode);

    fclose(myfs);
    
    return;
}


/*
이름    : myshowfile 함수
작성자  : 이우진
기능    : 파일 지정된 부분 출력 명령어
받는값  : X
리턴값  : X
*/
void myshowfile(char *num1, char *num2, char *file)
{
    FILE *myfs;
    myfs = fopen("myfs", "rb");

    INODE *inode_data = (INODE *)malloc(sizeof(INODE));

    int num = rear_dir_list_ptr->inode; //현재 디렉토리의 inode 번호를 저장할 변수
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (num - 1), SEEK_SET);
    fread(inode_data, sizeof(INODE), 1, myfs);
    int file_num = (inode_data->size / (8 + sizeof(int)));
        

    // 파일 찾기
    int dir_inode = 0;

    INODE *f_inode = (INODE *)malloc(sizeof(INODE)); //현재 디렉토리 내부 파일의 inode 정보를 저장할 변수
    char *fn = (char *)malloc(sizeof(char) * 8);  // filename
    int *in = (int *)malloc(sizeof(int));// inodenumber

    for(int i = 0; i < file_num; i++) 
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * inode_data->dir_1) + (i * (8 + sizeof(int))), SEEK_SET);
        fread(fn, 8, 1, myfs); 
        fread(in, sizeof(int), 1, myfs); 

        if(strcmp(file, fn) == 0)
        {
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (*in - 1)), SEEK_SET);
            fread(f_inode, sizeof(INODE), 1, myfs);

            if(f_inode->type == 0) //디렉토리인 경우
            {
                printf("%s 은/는 디렉토리입니다.\n", file);

                free(inode_data);
                free(fn);
                free(in);
                free(f_inode);

                return;
            }
            else //일반 파일인 경우
            {
                int number = *in;

                if(number == 0) 
                {
                    printf("%s 가 존재하지 않습니다.\n", file);

                    return;
                }

                else //부분 출력
                {
                    int first_num = atoi(num1);
                    int end_num = atoi(num2);

                    fseek(myfs,  first_num - 1, SEEK_CUR);
                    char *test;

                    fread(test, (end_num - first_num), 1, myfs);
                    printf("%s", test);
                }
                break;
            }
        }
    }

    free(inode_data);
    free(fn);
    free(in);
    free(f_inode);
    fclose(myfs);
    
    return;
}
#include "user.h"


/*
이름    : myls 함수
작성자  : 이우진
기능    : 파일 목록 자세히 보기
받는값  : 파일 명
리턴값  : X
*/
void myls(const char* file )
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

    while(c = getchar() != EOF)
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (data_num)) + (8 + sizeof(int)) * (i + 1), SEEK_SET);
        fread(filename, 8, 1, myfs);
        fread(fileinode, sizeof(int), 1, myfs);

        // 파일명
        printf("%d" filename);

        // inode list 정보 읽기
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (fileinode - 1)), SEEK_SET);
        INODE *file_inode_tmp_ptr = (INODE *)malloc(sizeof(INODE));
        fread(inode_data_ptr, sizeof(INODE), 1, myfs);

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
        printf("    inode번호 : %d", inode_num);


        // 크기
        printf("    크기 : %d\n", inode_ptr->size);

        fseek(myfs, -1, SEEK_CUR);
        fseek(myfs,sizeof(int),SEEK_CUR);
    }


    free(inode_ptr);
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


                break



        while((c = getchar()) != EOF){
    
            fseek(myfs, -1, SEEK_CUR);
            fseek(myfs,sizeof(int),SEEK_CUR);//현재 포인터 위치로부터 int형 크기만큼 이동
            fread(tmp_file_name_ptr, sizeof(char) * 8, 1, myfs);
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
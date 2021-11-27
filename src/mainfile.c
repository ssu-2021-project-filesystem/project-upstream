//헤더파일
#include "user.h"


//전역변수
//작업 디렉토리 경로를 저장할 linked list
DIR_LIST *front_dir_list_ptr = NULL;
DIR_LIST *rear_dir_list_ptr = NULL;


//함수
/*
이름    : main 함수
작성자  : 이준혁
기능    : X
받는값  : X
리턴값  : X
*/
int main(void)
{
    //pwd linked list 생성
    front_dir_list_ptr = (DIR_LIST *)malloc(sizeof(DIR_LIST));
    front_dir_list_ptr->inode = 1;
    front_dir_list_ptr->name = "/";
    front_dir_list_ptr->next_ptr = NULL;
    rear_dir_list_ptr = front_dir_list_ptr;

    //쉘
    shell();

    return 0;
}


/*
이름    : 쉘 함수
작성자  : 이준혁
기능    : 프롬프트를 띄우고 명령어를 입력받아 관련 함수를 호출한다(쉘 기능 수행)
받는값  : X
리턴값  : X
*/
void shell(void)
{
    //명령어 처리를 위한 포인터들
    char **com_sep_ptr = (char **)malloc(sizeof(char *) * COM_SEP_NUM); //4개의 포인터를 가리킬 수 있는 이차원 포인터

    for(int i = 0; i < COM_SEP_NUM; i++)
    {   
        *(com_sep_ptr + i) = (char *)malloc(COM_SEP_SIZE); //각 포인터에 동적 메모리 할당
    }

    //쉘 출력, 명령어 처리
    while(1)
    {
        //myfs 파일 존재여부 확인
        int exist = 0; //파일이 존재하지 않는 경우 0, 존재하는 경우 1 저장
        FILE *myfs_exist;
        if((myfs_exist = fopen("myfs", "rb")) != NULL) //파일이 존재하지 않는 경우
        {
            exist = 1;
        }
        fclose(myfs_exist);

        //쉘 출력
        if(exist == 0) //myfs 파일이 존재하지 않는 경우
        {
            printf("$ ");
        }
        else //myfs 파일이 존재하는 경우
        {
            printf("[");
            mypwd(); //mypwd() 함수로 경로 출력
            printf(" ]$ ");
        }

        //포인터가 가리키는 메모리 공간 초기화
        for(int i = 0; i < COM_SEP_NUM; i++)
        {   
            for(int j = 0; j < COM_SEP_SIZE; j++)
            {
                *(*(com_sep_ptr + i) + j) = 0;
            }
        }

        //명령어 입력
        char char_tmp;
        int com_ptr_num = 0; //포인터 변수의 인덱스
        int com_ptr_char_num = 0; //포인터 변수가 가리키는 문자의 인덱스

        rewind(stdin); //버퍼 비우기

        while((char_tmp = getchar()) != '\n')
        {
            if(char_tmp != ' ') //공백 문자가 아닌 경우
            {
                *(*(com_sep_ptr + com_ptr_num) + com_ptr_char_num) = char_tmp;
            }
            else //공백 문자인 경우
            {
                *(*(com_sep_ptr + com_ptr_num) + com_ptr_char_num) = 0; //NULL 문자 배정
                com_ptr_num++;
                com_ptr_char_num = 0;
                continue;
           }

            com_ptr_char_num++;
        }
        *(*(com_sep_ptr + com_ptr_num) + com_ptr_char_num) = 0;

        //입력 데이터 초기화, 전처리
        char **com_tmp_ptr = (char **)malloc(sizeof(char *) * COM_SEP_NUM); //4개의 포인터를 가리킬 수 있는 이차원 포인터

        for(int i = 0; i < (com_ptr_num + 1); i++)
        {
            *(com_tmp_ptr + i) = *(com_sep_ptr + i);
        }
        for(int i = (com_ptr_num + 1); i < COM_SEP_NUM; i++)
        {
            *(com_tmp_ptr + i) = NULL;
        }

        //명령어 함수 호출
        if(!strcmp("mymkfs", *(com_tmp_ptr)))
        {
            mymkfs();
        }
        else if((!strcmp("myls", *(com_tmp_ptr))) && (exist == 1))
        {
            //myls 함수
        }
        else if((!strcmp("mycat", *(com_tmp_ptr))) && (exist == 1))
        {
            //mycat 함수
        }
        else if((!strcmp("myshowfile", *(com_tmp_ptr))) && (exist == 1))
        {
            //myshowfile 함수
        }
        else if((!strcmp("mypwd", *(com_tmp_ptr))) && (exist == 1))
        {
            mypwd();
            printf("\n");
        }
        else if((!strcmp("mycd", *(com_tmp_ptr))) && (exist == 1))
        {
            //mycd 함수
        }
        else if((!strcmp("mycp", *(com_tmp_ptr))) && (exist == 1))
        {
            //mycp 함수
        }
        else if((!strcmp("mycpto", *(com_tmp_ptr))) && (exist == 1))
        {
            //mycpto 함수
        }
        else if((!strcmp("mycpfrom", *(com_tmp_ptr))) && (exist == 1))
        {
            //mycpfrom 함수
        }
        else if((!strcmp("mymkdir", *(com_tmp_ptr))) && (exist == 1))
        {
            mymkdir(*(com_tmp_ptr + 1));
        }
        else if((!strcmp("myrmdir", *(com_tmp_ptr))) && (exist == 1))
        {
            myrmdir(*(com_tmp_ptr + 1));
        }
        else if((!strcmp("myrm", *(com_tmp_ptr))) && (exist == 1))
        {
            //myrm 함수
        }
        else if((!strcmp("mymv", *(com_tmp_ptr))) && (exist == 1))
        {
            mymv(*(com_tmp_ptr + 1), *(com_tmp_ptr + 2));
        }
        else if((!strcmp("mytouch", *(com_tmp_ptr))) && (exist == 1))
        {
            mytouch(*(com_tmp_ptr + 1));
        }
        else if((!strcmp("myinode", *(com_tmp_ptr))) && (exist == 1))
        {
            myinode(*(com_tmp_ptr + 1));
        }
        else if((!strcmp("mydatablock", *(com_tmp_ptr))) && (exist == 1))
        {
            mydatablock(*(com_tmp_ptr + 1));
        }
        else if((!strcmp("mystate", *(com_tmp_ptr))) && (exist == 1))
        {
            mystate();
        }
        else if((!strcmp("mytree", *(com_tmp_ptr))) && (exist == 1))
        {
            mytree(*(com_tmp_ptr + 1));
        }
        else if((!strcmp("command", *(com_tmp_ptr))) && (exist == 1))
        {
            command();
        }
        else if(!strcmp("exit", *(com_tmp_ptr)))
        {
            printf("Bye....\n\n");
            return;
        }
        else if(exist == 0)
        {
            printf("파일 시스템이 존재하지 않습니다.\n");
        }
        else
        {
            printf("잘못된 명령어입니다.\n");
        }

        printf("\n");
    }

    return;
}
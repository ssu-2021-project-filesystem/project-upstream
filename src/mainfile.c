#include "user.h"


//작업 디렉토리 경로를 저장할 linked list
DIR_LIST *front_dir_list_ptr;
DIR_LIST *rear_dir_list_ptr;


/*
이름    : main 함수
작성자  : 이준혁
기능    : X
받는값  : X
리턴값  : X
*/
int main(void)
{
    //linked list 초기화(move. 나중에 mymkfs() 함수로 옮기기)
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
        //쉘 출력
        printf("[");
        mypwd(); //mypwd() 함수로 경로 출력
        printf(" ]$ ");

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

        //입력 데이터 전처리
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
            //mymkfs 함수
        }
        else if(!strcmp("myls", *(com_tmp_ptr)))
        {
            //myls 함수
        }
        else if(!strcmp("mycat", *(com_tmp_ptr)))
        {
            //mycat 함수
        }
        else if(!strcmp("myshowfile", *(com_tmp_ptr)))
        {
            //myshowfile 함수
        }
        else if(!strcmp("mypwd", *(com_tmp_ptr)))
        {
            mypwd();
            printf("\n");
        }
        else if(!strcmp("mycd", *(com_tmp_ptr)))
        {
            //mycd 함수
        }
        else if(!strcmp("mycp", *(com_tmp_ptr)))
        {
            //mycp 함수
        }
        else if(!strcmp("mycpto", *(com_tmp_ptr)))
        {
            //mycpto 함수
        }
        else if(!strcmp("mycpfrom", *(com_tmp_ptr)))
        {
            //mycpfrom 함수
        }
        else if(!strcmp("mymkdir", *(com_tmp_ptr)))
        {
            //mymkdir 함수
        }
        else if(!strcmp("myrmdir", *(com_tmp_ptr)))
        {
            //myrmdir 함수
        }
        else if(!strcmp("myrm", *(com_tmp_ptr)))
        {
            //myrm 함수
        }
        else if(!strcmp("mymv", *(com_tmp_ptr)))
        {
            //mymv 함수
        }
        else if(!strcmp("mytouch", *(com_tmp_ptr)))
        {
            //mytouch 함수
        }
        else if(!strcmp("myinode", *(com_tmp_ptr)))
        {
            myinode(*(com_tmp_ptr + 1));
        }
        else if(!strcmp("mydatablock", *(com_tmp_ptr)))
        {
            mydatablock(*(com_tmp_ptr + 1));
        }
        else if(!strcmp("mystate", *(com_tmp_ptr)))
        {
            //mystate 함수
        }
        else if(!strcmp("mytree", *(com_tmp_ptr)))
        {
            //mytree 함수
        }
        else if(!strcmp("command", *(com_tmp_ptr)))
        {
            command();
        }
        else if(!strcmp("exit", *(com_tmp_ptr)))
        {
            printf("Bye....\n\n");
            return;
        }
        else
        {
            printf("잘못된 명령어입니다.\n");
        }

        printf("\n");
    }

    return;
}
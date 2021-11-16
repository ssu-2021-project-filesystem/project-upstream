#include "user.h"
/*
이름    : mycd 함수
작성자  : 오규빈
기능    : 작업 디렉토리의 경로를 변경한다
받는값  : 경로 문자열
리턴값  : X
*/
void mycd (const char* path){
    if(path == NULL)
    {//path 인자가 없을시
        rear_dir_list_ptr == front_dir_list_ptr;//작업 디렉터리를 홈디렉터리로 설정
    }

    else if (path != NULL)
    {
        int i = 0;
        int first = 0;
        DIR_LIST *tmp_ptr;
        char *nm_ptr = strtok(path, "/");
        while (nm_ptr != NULL)
        {
         if (first == 0)
         {
          if (nm_ptr == ".")
          {
          tmp_ptr = rear_dir_list_ptr -> next_ptr;
          tmp_ptr -> next_ptr -> name = nm_ptr;
          nm_ptr = strtok(NULL, "/");
          first++;
          }
          else if (nm_ptr == "..")
          {
           tmp_ptr = front_dir_list_ptr -> next_ptr;
           front_dir_list_ptr -> next_ptr -> name = nm_ptr;
           nm_ptr = strtok(NULL, "/");
           first++;
          }
          else if (nm_ptr == "/")
          {
           tmp_ptr = front_dir_list_ptr -> next_ptr;
           front_dir_list_ptr -> next_ptr -> name = nm_ptr;
           nm_ptr = strtok(NULL, "/");
           first++;
          }
          else
          {
              printf("잘못된 입력 방식입니다.");
              break;
          }
         }
         else if (first != 0)
         {
          tmp_ptr -> next_ptr -> name = nm_ptr;
          nm_ptr = strtok(NULL, "/");
         }
         tmp_ptr->next_ptr == NULL;
         rear_dir_list_ptr == tmp_ptr;
        }
    }
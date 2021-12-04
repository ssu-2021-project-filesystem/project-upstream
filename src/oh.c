#include "user.h"


/*
이름    : mycd 함수
작성자  : 이준혁
기능    : 작업 디렉토리의 경로를 변경한다
받는값  : 경로 문자열
리턴값  : X
*/
void mycd (char *path)
{
    DIR_LIST *tmp_delete_ptr; //지울 노드를 가리킬 포인터
    DIR_LIST *tmp_delete_back_ptr; //지울 노드의 전 노드를 가리킬 포인터

    //경로가 입력되지 않은 경우(root로 이동)
    if((path == NULL) || (strcmp(path, "/") == 0))
    {
        //노드 전부 제거하기
        while(1)
        {
            tmp_delete_ptr = front_dir_list_ptr;
            tmp_delete_back_ptr = NULL;

            //제일 마지막 노드로 이동
            while(tmp_delete_ptr->next_ptr != NULL)
            {
                tmp_delete_back_ptr = tmp_delete_ptr;
                tmp_delete_ptr = tmp_delete_ptr->next_ptr;
            }

            //노드 제거, 종료
            if(tmp_delete_back_ptr == NULL) //노드가 하나 뿐인 경우(root인 경우)
            {
                tmp_delete_ptr->next_ptr = NULL;

                rear_dir_list_ptr = front_dir_list_ptr;

                return;
            }
            else
            {                
                tmp_delete_back_ptr->next_ptr = NULL;
                free(tmp_delete_ptr->name);
                free(tmp_delete_ptr);
            }
        }
    }
    
    //path 데이터 전처리(맨 앞 문자열의 종류 확인)
    if(*path == '/') //맨 앞이 /인 경우
    {
        //노드 전부 제거하기(현재 위치 /)
        while(1)
        {
            tmp_delete_ptr = front_dir_list_ptr;
            tmp_delete_back_ptr = NULL;

            //제일 마지막 노드로 이동
            while(tmp_delete_ptr->next_ptr != NULL)
            {
                tmp_delete_back_ptr = tmp_delete_ptr;
                tmp_delete_ptr = tmp_delete_ptr->next_ptr;
            }

            //노드 제거, 종료
            if(tmp_delete_back_ptr == NULL) //노드가 하나 뿐인 경우(root인 경우)
            {
                tmp_delete_ptr->next_ptr = NULL;

                rear_dir_list_ptr = front_dir_list_ptr;

                break;
            }
            else
            {
                tmp_delete_back_ptr->next_ptr = NULL;
                free(tmp_delete_ptr->name);
                free(tmp_delete_ptr);
            }
        }

        //path에서 / 제거하기
        path++;
    }
    else if((*path == '.') && (*(path + 1) == '/')) //맨 앞이 .인 경우 (..은 아니어야 함)
    {
        //노드 그대로 사용하기(현재 위치 .)
        //path에서 ./ 제거하기
        path += 2;
    }
    else // /과 ./ 둘 다 아닌 경우에는 path를 그대로 사용
    {
        ;
    }

    //처리된 path로 노드 수정하기 -> ..이면 노드 하나 제거, ..이 아니면 노드 추가
    FILE *myfs;
    myfs = fopen("myfs", "rb");

    INODE *i_data = (INODE *)malloc(sizeof(INODE)); //현재 디렉토리의 inode 정보를 저장한 구조체를 가리킬 포인터

    char *filename = (char *)malloc(sizeof(char) * 8);
    int *inodenumber = (int *)malloc(sizeof(int));
    
    int count = 0;
    int dir_file_num;

    char *nm_ptr = strtok(path, "/");
    do
    {
        if(strcmp(nm_ptr, "..") == 0) //..인 경우 -> 노드 하나 제거
        {
            //제일 마지막 노드로 이동
            tmp_delete_ptr = front_dir_list_ptr;
            tmp_delete_back_ptr = NULL;

            while(tmp_delete_ptr->next_ptr != NULL)
            {
                tmp_delete_back_ptr = tmp_delete_ptr;
                tmp_delete_ptr = tmp_delete_ptr->next_ptr;
            }

            //예외처리(노드가 하나 남은 경우)
            if(tmp_delete_back_ptr == NULL)
            {
                printf("잘못된 경로입니다.(root의 부모 디렉토리는 존재하지 않음.)\n");

                return;
            }

            //마지막 노드 제거
            free(tmp_delete_ptr->name);
            free(tmp_delete_ptr);
            tmp_delete_back_ptr->next_ptr = NULL;
            rear_dir_list_ptr = tmp_delete_back_ptr;
        }
        else //..이 아닌 경우 -> 현재 디렉토리에서 검색해서 새 노드 추가
        {
            //현재 디렉토리에서 해당 파일명 검색, inode 번호 획득
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (rear_dir_list_ptr->inode - 1), SEEK_SET);
            fread(i_data, sizeof(INODE), 1, myfs);

            dir_file_num = i_data->size / (8 + sizeof(int));
            count = 0;

            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * i_data->dir_1), SEEK_SET);
            for (int i = 0; i < dir_file_num; i++)
            {
                fread(filename, 8, 1, myfs);
                fread(inodenumber, sizeof(int), 1, myfs);

                if (strcmp(nm_ptr, filename) == 0)
                {
                    break;
                }
                else
                {
                    count++;
                }
            }

            //예외처리(.인 경우)
            if(strcmp(nm_ptr, ".") == 0)
            {
                printf("잘못된 경로입니다.(.은 현재 디렉토리를 나타냄.)\n");

                return;
            }

            if (count == dir_file_num) //현재 디렉토리에 해당 파일이 없는 경우
            {
                printf("잘못된 경로입니다.(경로에 존재하지 않는 파일명이 포함됨.)\n");

                return;
            }
            else //현재 디렉토리에 해당 파일이 있는 경우
            {
                //파일 종류 검사
                INODE *file_inode_tmp_ptr = (INODE *)malloc(sizeof(INODE));
                fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (*inodenumber - 1), SEEK_SET);
                fread(file_inode_tmp_ptr, sizeof(INODE), 1, myfs);

                if(file_inode_tmp_ptr->type == 1) //해당 파일이 일반 파일인 경우
                {
                    printf("잘못된 경로입니다.(경로에 일반 파일명이 포함됨.)\n");

                    return;
                }
                else //해당 파일이 디렉토리인 경우
                {
                    //파일명, inode 번호를 사용해서 새 노드 생성
                    rear_dir_list_ptr->next_ptr = (DIR_LIST *)malloc(sizeof(DIR_LIST));
                    rear_dir_list_ptr = rear_dir_list_ptr->next_ptr;
                    rear_dir_list_ptr->inode = *inodenumber;
                    rear_dir_list_ptr->name = (char *)malloc(sizeof(char) * 8);
                    strcpy(rear_dir_list_ptr->name, nm_ptr);
                    rear_dir_list_ptr->next_ptr = NULL;
                }

                free(file_inode_tmp_ptr);
            }
        }
    }
    while((nm_ptr = strtok(NULL, "/")) != NULL);

    //메모리 반납
    free(i_data);
    free(filename);
    free(inodenumber);

    fclose(myfs);

    return;
}

/*
이름    : mycpto 함수
작성자  : 오규빈
기능    : MY 파일시스템에 있는 파일을 호스트 컴퓨터로 복사한다.
받는값  : 파일명 1,2
리턴값  : 리턴값
*/
void mycpto (const char* source_file, const char* dest_file  )
{
    //변수 선언
    FILE *ofp;
    FILE *myfs;
    int c;
    char d;
    int test = 0;
    int i = 0;
    int inode = 1;
    int tmp_datablock;

    int presentinode = rear_dir_list_ptr-> inode; //현재 디렉터리의 아이노드 번호를 저장할 변수
    INODE *presenti_data = (INODE *)malloc(sizeof(INODE)); //현재 디렉터리의 inode 정보를 저장할 구조체를 가리킬 포인터
    INODE *file_inode_tmp_ptr = (INODE *)malloc(sizeof(INODE));//받아올 파일의 inode 정보를 저장할 구조체를 가리킬 포인터

    //인자 예외처리
    if(source_file ==NULL || dest_file == NULL)
    {
        printf("오류 : 인자가 부족합니다\n");
        return;
    }

    //파일 열기
    myfs = fopen("myfs", "rb");

    //현재 디렉터리의 아이노드 정보 받기
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20*(presentinode - 1), SEEK_SET);
    fread(presenti_data, sizeof(INODE), 1, myfs);

    //함수인자의 파일명과 데이터블록의 파일명 비교
    int n = presenti_data->size / (8 + sizeof(int)); //for문을 위한 변수지정
    char *filename = (char *)malloc(sizeof(char) * 8); //파일명을 읽기위한 변수
    int *fileinode = (int *)malloc(sizeof(int));

    unsigned count;
    int none_tmp = 0;

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (presenti_data-> dir_1)), SEEK_SET); //현재 디렉토리의 datablock으로 이동

    for(int i=0; i<n; i++)
    {
        fread(filename, 8, 1, myfs);
        fread(fileinode, sizeof(int), 1, myfs);

        if(strcmp(source_file, filename) == 0) //파일 목록에 같은 이름의 파일이 있는 경우
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
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (*fileinode - 1), SEEK_SET);
        fread(file_inode_tmp_ptr, sizeof(INODE), 1, myfs);

        if (file_inode_tmp_ptr->type == 0) //해당 파일이 디렉터리인 경우
        {
            printf("해당 파일은 일반 파일이 아닙니다.\n");
            free(presenti_data);
            free(filename);
            return;
        }
    }

    //호스트 컴퓨터의 파일 열기
    if ((ofp = fopen(dest_file, "w")) == NULL)
    {
        printf("오류 : 파일을 열지 못했습니다.\n");
        return;
    }

    //datablock 읽어서 호스트 컴퓨터 파일에 작성
    int datablock_num; //datablock 번호를 저장할 변수(저장된 정수 + 1 값임)

    for(int print_loop = 0; print_loop < 8; print_loop++)
    {
        if((file_inode_tmp_ptr->size) > (256 * print_loop))
        {
            datablock_num = *((char *)file_inode_tmp_ptr + 11 + print_loop);

            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + DATA_BLOCK_SIZE * datablock_num, SEEK_SET);

            for(int print_loop_2 = 0; print_loop_2 < 256; print_loop_2++)
            {
                fread(&d, sizeof(char), 1, myfs);

                if(d == -1)
                {
                    break;
                }
                else
                {
                    putc(d, ofp);
                }
            }
        }
    }

    //single indirect를 사용하는 경우
    char *indir_dir_num = (char *)malloc(sizeof(char)); //indir를 위한 datablock 번호를 저장할 공간을 가리킬 포인터

    if(file_inode_tmp_ptr->size > (256 * 8)) //direct만으로 저장할 수 있는 크기보다 크기가 큰 경우
    {
        for(int print_loop = 0; print_loop < 8; print_loop++)
        {
            if((file_inode_tmp_ptr->size - (256 * 8)) > (256 * print_loop))
            {
                //indir의 datablock으로 이동해서 datablock 번호 검사
                fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * file_inode_tmp_ptr->indir) + print_loop, SEEK_SET);
                fread(indir_dir_num, sizeof(char), 1, myfs);
                fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + DATA_BLOCK_SIZE * (*indir_dir_num), SEEK_SET);

                for(int print_loop_2 = 0; print_loop_2 < 256; print_loop_2++)
                {
                    fread(&d, sizeof(char), 1, myfs);

                    if(d == -1)
                    {
                        break;
                    }
                    else
                    {
                        putc(d, ofp);
                    }
                }
            }
        }
    }

    free(file_inode_tmp_ptr);
    free(fileinode);
    free(indir_dir_num);

    fclose(ofp);
    fclose(myfs);

    return;
}

/*
이름    : mycpfrom 함수
작성자  : 오규빈
기능    : 호스트 컴퓨터에 있는 파일을 MY 파일시스템으로 복사한다.
받는값  : 파일명 1,2
리턴값  : 리턴값
*/
void mycpfrom (char* source_file, char* dest_file )
{
    //변수 선언
    FILE *ifp;
    FILE *myfs;
    int c, size_F = 0;
    char d;
    time_t Time;
    struct tm* TimeInfo;

    int presentinode = rear_dir_list_ptr-> inode; //현재 디렉터리의 아이노드번호
    INODE *presenti_data = (INODE *)malloc(sizeof(INODE)); //현재 디렉터리의 아이노드 구조체
    INODE *file_inode_tmp_ptr = (INODE *)malloc(sizeof(INODE));//받아올 파일의 아이노드 구조체

    //인자 예외처리
    if(source_file == NULL || dest_file == NULL)
    {
        printf("오류 : 인자가 부족합니다\n");

        free(presenti_data);
        free(file_inode_tmp_ptr);

        return;
    }

    //호스트 컴퓨터의 파일 열기
    if ((ifp = fopen(source_file, "r")) == NULL)
    {
        printf("오류 : %s 파일이 존재하지 않거나 열 수 없습니다.\n", source_file);
        
        free(presenti_data);
        free(file_inode_tmp_ptr);

        return;
    }

    //파일 열기
    myfs = fopen("myfs", "rb+");

    //현재 디렉터리의 아이노드 받기
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (presentinode - 1), SEEK_SET);
    fread(presenti_data, sizeof(INODE), 1, myfs);
    
    //함수인자의 파일명과 데이터블록의 파일명 비교
    int n = presenti_data->size / (8 + sizeof(int)); //for문을 위한 변수지정
    char *filename = (char *)malloc(sizeof(char) * 8); //파일명을 읽기위한 변수
    int *fileinode = (int *)malloc(sizeof(int));

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (presenti_data->dir_1)), SEEK_SET);
    for(int i = 0; i < n; i++)
    {
        fread(filename, 8, 1, myfs);
        fread(fileinode, sizeof(int), 1, myfs);

        if(strcmp(dest_file, filename) == 0)
        {
            printf("해당 이름의 파일이 존재합니다.\n");

            free(presenti_data);
            free(file_inode_tmp_ptr);
            free(filename);
            free(fileinode);

            return;
        }
    }
    
    //파일 닫기
    fclose(myfs);

    //dest_file의 이름을 가지는 파일 생성
    mytouch(dest_file);

    //파일 열기
    myfs = fopen("myfs", "rb+");

    //해당 파일의 inode 정보 얻기
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (presentinode - 1), SEEK_SET);
    fread(presenti_data, sizeof(INODE), 1, myfs);
    n = presenti_data->size / (8 + sizeof(int));

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (presenti_data->dir_1)), SEEK_SET);    
    for(int i = 0; i < n; i++)
    {
        fread(filename, 8, 1, myfs);
        fread(fileinode, sizeof(int), 1, myfs);

        if(strcmp(dest_file, filename) == 0)
        {
            break;
        }
    }

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (*fileinode - 1), SEEK_SET);
    fread(file_inode_tmp_ptr, sizeof(INODE), 1, myfs);

    //호스트 컴퓨터의 파일 크기 알아내기.
    int host_file_size;
    fseek(ifp, 0, SEEK_END);
    host_file_size = ftell(ifp);
    rewind(ifp);

    //호스트 컴퓨터 파일 읽어서 datablock에 작성
    char minousone = -1; //파일 끝에 저장할 -1을 저장할 변수
    int savedbnumber = 0; //datablock 번호를 저장할 변수
    SUPERBLOCK *sb_data = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));

    for(int print_loop = 0; print_loop < 8; print_loop++)
    {
        if(host_file_size > (256 * print_loop))
        {
            //datablock 할당하기
            if(print_loop == 0) //맨 처음 루프인 경우->이미 datablock 하나가 할당되어 있음
            {
                savedbnumber = file_inode_tmp_ptr->dir_1 + 1;
            }
            else //처음 루프가 아닌 경우->새로운 datablock 할당 필요
            {
                //새 datablock 할당하기
                fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
                fread(sb_data, sizeof(SUPERBLOCK), 1, myfs);
                
                savedbnumber = 0;
                savedbnumber = checkbit(savedbnumber, sb_data->data_block_1);
                savedbnumber = checkbit(savedbnumber, sb_data->data_block_2);
                savedbnumber = checkbit(savedbnumber, sb_data->data_block_3);
                savedbnumber = checkbit(savedbnumber, sb_data->data_block_4);
                savedbnumber = checkbit(savedbnumber, sb_data->data_block_5);
                savedbnumber = checkbit(savedbnumber, sb_data->data_block_6);
                savedbnumber = checkbit(savedbnumber, sb_data->data_block_7);
                savedbnumber = checkbit(savedbnumber, sb_data->data_block_8);
                savedbnumber++;

                //datablock에 반영하기
                change_superblock(0, savedbnumber, sb_data);

                fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
                fwrite(sb_data, sizeof(SUPERBLOCK), 1, myfs);

                //inode에 datablock 번호 반영하기
                *((char *)file_inode_tmp_ptr + 11 + print_loop) = (unsigned)(savedbnumber - 1);
            }

            //datablock에 내용 작성하기
            for(int print_loop_2 = 0; print_loop_2 < 256; print_loop_2++)
            {
                d = getc(ifp);

                fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (savedbnumber - 1)) + print_loop_2, SEEK_SET);

                if(d == EOF)
                {
                    fwrite(&minousone, sizeof(char), 1, myfs);

                    break;
                }
                else
                {
                    fwrite(&d, sizeof(char), 1, myfs);
                }
            }
        }
    }

    //single indirect를 사용하는 경우
    char *indir_dir_num = (char *)malloc(sizeof(char)); //indir를 위한 datablock 번호를 저장할 공간을 가리킬 포인터
    unsigned char indir_db_tmp; //indir에 저장할 datablock 번호를 임시 저장할 변수

    if(host_file_size > (256 * 8)) //direct만으로 저장할 수 있는 크기보다 크기가 큰 경우
    {
        //indir datablock 할당하기
        fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
        fread(sb_data, sizeof(SUPERBLOCK), 1, myfs);
        
        savedbnumber = 0;
        savedbnumber = checkbit(savedbnumber, sb_data->data_block_1);
        savedbnumber = checkbit(savedbnumber, sb_data->data_block_2);
        savedbnumber = checkbit(savedbnumber, sb_data->data_block_3);
        savedbnumber = checkbit(savedbnumber, sb_data->data_block_4);
        savedbnumber = checkbit(savedbnumber, sb_data->data_block_5);
        savedbnumber = checkbit(savedbnumber, sb_data->data_block_6);
        savedbnumber = checkbit(savedbnumber, sb_data->data_block_7);
        savedbnumber = checkbit(savedbnumber, sb_data->data_block_8);
        savedbnumber++;

        change_superblock(0, savedbnumber, sb_data);

        fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
        fwrite(sb_data, sizeof(SUPERBLOCK), 1, myfs);

        //inode에 datablock 번호 반영하기
        file_inode_tmp_ptr->indir = (unsigned)(savedbnumber - 1);

        //indir datablock 기본값 작성하기
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + DATA_BLOCK_SIZE * (savedbnumber - 1), SEEK_SET);
        char zero_db_value = 0;
        for(int loop_zero = 0; loop_zero < 8; loop_zero++)
        {
            fwrite(&zero_db_value, sizeof(char), 1, myfs);
        }
        fwrite(&minousone, sizeof(char), 1, myfs);

        //파일 크기 검사해서 datablock에 작성하기
        for(int print_loop = 0; print_loop < 8; print_loop++)
        {
            if((host_file_size - (256 * 8)) > (256 * print_loop))
            {
                //indir의 datablock에 작성할 datablock 할당하기
                fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
                fread(sb_data, sizeof(SUPERBLOCK), 1, myfs);
                
                savedbnumber = 0;
                savedbnumber = checkbit(savedbnumber, sb_data->data_block_1);
                savedbnumber = checkbit(savedbnumber, sb_data->data_block_2);
                savedbnumber = checkbit(savedbnumber, sb_data->data_block_3);
                savedbnumber = checkbit(savedbnumber, sb_data->data_block_4);
                savedbnumber = checkbit(savedbnumber, sb_data->data_block_5);
                savedbnumber = checkbit(savedbnumber, sb_data->data_block_6);
                savedbnumber = checkbit(savedbnumber, sb_data->data_block_7);
                savedbnumber = checkbit(savedbnumber, sb_data->data_block_8);
                savedbnumber++;

                //datablock에 반영하기
                change_superblock(0, savedbnumber, sb_data);

                fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
                fwrite(sb_data, sizeof(SUPERBLOCK), 1, myfs);

                //indir의 datablock에 새로 할당한 datablock 번호 반영하기
                indir_db_tmp = savedbnumber - 1;
                fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + DATA_BLOCK_SIZE * (file_inode_tmp_ptr->indir) + print_loop, SEEK_SET);
                fwrite(&indir_db_tmp, sizeof(char), 1, myfs);

                //할당한 datablock에 내용 작성하기
                fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + DATA_BLOCK_SIZE * (savedbnumber - 1), SEEK_SET);
                for(int print_loop_2 = 0; print_loop_2 < 256; print_loop_2++)
                {
                    d = getc(ifp);

                    if(d == EOF)
                    {
                        fwrite(&minousone, sizeof(char), 1, myfs);

                        break;
                    }
                    else
                    {
                        fwrite(&d, sizeof(char), 1, myfs);
                    }
                }
            }
        }
    }

    //inode의 size 갱신하기
    file_inode_tmp_ptr -> size = host_file_size;
    
    //inode 반영하기
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (*fileinode - 1), SEEK_SET);
    fwrite(file_inode_tmp_ptr, sizeof(INODE), 1, myfs);

    free(presenti_data);
    free(file_inode_tmp_ptr);
    free(filename);
    free(fileinode);
    free(sb_data);
    free(indir_dir_num);

    fclose(ifp);
    fclose(myfs);

    return;
}

/*
이름    : mycp 함수
작성자  : 오규빈
기능    : MY 파일시스템에 있는 파일을 복사하는 명령어
받는값  : 파일명 1,2
리턴값  : 리턴값
*/

void mycp(char* source_file, char* dest_file  )
{
    FILE*fp;

    if(source_file ==NULL || dest_file == NULL)
    {
        printf("오류 : 인자가 부족합니다");
        return;
    }//인자가 부족한 경우 예외처리

    if(strcmp(source_file, dest_file) == 0)
    {
        printf("오류 : 인자가 같습니다");
        return;
    }//인자가 같을경우 예외처리

    fp = fopen("tmp","wb");
    fclose(fp);
    
    mycpto(source_file,"tmp");
    mycpfrom("tmp",dest_file);
    
    system("rm tmp");

    return;
}

/*
이름    : myrm 함수
작성자  : 양인석
기능    : 파일을 삭제하는 명령어
받는값  : 파일명
리턴값  : x
*/

void myrm(const char* file)
{
    FILE *myfs;
    myfs = fopen("myfs", "rb+");

    //현재 디렉터리의 아이노드 받기
    int presentinode = rear_dir_list_ptr-> inode; //현재 디렉터리의 아이노드번호
    INODE *presenti_data = (INODE *)malloc(sizeof(INODE)); //현재 디렉터리의 아이노드 구조체
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20*(presentinode-1), SEEK_SET);
    fread(presenti_data, sizeof(INODE), 1, myfs);
    
    //함수인자의 파일명과 데이터블록의 파일명 비교
    int n = presenti_data-> size/12; //for문을 위한 변수지정
    char *filename = (char *)malloc(sizeof(char) * 8); //파일명을 읽기위한 변수
    int *fileinode = (int *)malloc(sizeof(int));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (presenti_data-> dir_1)), SEEK_SET);
    unsigned count;
    int none_tmp = 0;
    for(int i=0; i<n; i++)
    {
        fread(filename, 8, 1, myfs);
        fread(fileinode, sizeof(int), 1, myfs);
        if(strcmp(file, filename) == 0)
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
        if (file_inode_tmp_ptr->type == 0) //해당 파일이 디렉터리인 경우
        {
            printf("해당 파일은 일반 파일이 아닙니다.\n");

            free(file_inode_tmp_ptr);
            free(fileinode);
            free(presenti_data);
            free(filename);

            return;
        }
        else //해당 파일이 일반 파일인 경우
        {
            free(file_inode_tmp_ptr);
        }
    }

    //삭제할 파일의 아이노드 호출
    INODE *i_data = (INODE *)malloc(sizeof(INODE)); //삭제할 파일의 아이노드 구조체
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20*(*fileinode-1), SEEK_SET);
    fread(i_data, sizeof(INODE), 1, myfs);
    int saveinumber = *fileinode; //삭제할 파일의 아이노드 번호저장
    if(none_tmp < n && i_data-> type == 1)
    {
        //파일 삭제
        for(int i = count; i<(n); i++)
        {
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (presenti_data->dir_1)) + (8 + sizeof(int)) * (i + 1), SEEK_SET);
            fread(filename, 8, 1, myfs);
            fread(fileinode, sizeof(int), 1, myfs);
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (presenti_data->dir_1)) + (8 + sizeof(int)) * (i), SEEK_SET);
            fwrite(filename, 8, 1, myfs);
            fwrite(fileinode, sizeof(int), 1, myfs);            
        }
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (presenti_data->dir_1)) + (8 + sizeof(int)) * (n - 1), SEEK_SET);
        char *minusone = (char *)malloc(sizeof(char));
        *minusone = -1;
        fwrite(minusone, sizeof(char), 1, myfs);
        free(minusone);

        //슈퍼블록 수정
        SUPERBLOCK *sb_data = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK)); 
        fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
        fread(sb_data, sizeof(SUPERBLOCK), 1, myfs);
        unsigned mask = 1 << 31;
        if(saveinumber>0 && saveinumber<=32)
        {
            mask >>= (saveinumber - 1);
            sb_data-> inode_1 = sb_data-> inode_1 ^ mask;
        }
        else if(saveinumber>33 && saveinumber<=64)
        {
            mask >>= (saveinumber - 33);
            sb_data-> inode_2 = sb_data-> inode_2 ^ mask;
        }
        else if(saveinumber>65 && saveinumber<=96)
        {
            mask >>= (saveinumber - 65);
            sb_data-> inode_3 = sb_data-> inode_3 ^ mask;
        }
        else if(saveinumber>97 && saveinumber<=128)
        {
            mask >>= (saveinumber - 97);
            sb_data-> inode_4 = sb_data-> inode_4 ^ mask;
        }
        unsigned mask1 = 1 << 31;
        if(i_data-> dir_1 >= 0 && i_data-> dir_1 <32)
        {
            mask1 >>= (i_data-> dir_1);
            sb_data-> data_block_1 = sb_data-> data_block_1 ^ mask1;
        }
        else if(i_data-> dir_1 >= 32 && i_data-> dir_1 <64)
        {
            mask1 >>= (i_data-> dir_1 - 32);
            sb_data-> data_block_2 = sb_data-> data_block_2 ^ mask1;
        }
        else if(i_data-> dir_1 >= 64 && i_data-> dir_1 <96)
        {
            mask1 >>= (i_data-> dir_1 - 64);
            sb_data-> data_block_3 = sb_data-> data_block_3 ^ mask1;
        }
        else if(i_data-> dir_1 >= 96 && i_data-> dir_1 <128)
        {
            mask1 >>= (i_data-> dir_1 - 96);
            sb_data-> data_block_4 = sb_data-> data_block_4 ^ mask1;
        }
        else if(i_data-> dir_1 >= 128 && i_data-> dir_1 <160)
        {
            mask1 >>= (i_data-> dir_1 - 128);
            sb_data-> data_block_5 = sb_data-> data_block_5 ^ mask1;
        }
        else if(i_data-> dir_1 >= 160 && i_data-> dir_1 <192)
        {
            mask1 >>= (i_data-> dir_1 - 160);
            sb_data-> data_block_6 = sb_data-> data_block_6 ^ mask1;
        }
        else if(i_data-> dir_1 >= 192 && i_data-> dir_1 <224)
        {
            mask1 >>= (i_data-> dir_1 - 192);
            sb_data-> data_block_7 = sb_data-> data_block_7 ^ mask1;
        }
        else if(i_data-> dir_1 >= 224 && i_data-> dir_1 <256)
        {
            mask1 >>= (i_data-> dir_1 - 224);
            sb_data-> data_block_8 = sb_data-> data_block_8 ^ mask1;
        }
        fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
        fwrite(sb_data, sizeof(SUPERBLOCK), 1, myfs);
        free(sb_data);

        //현재디렉터리 사이즈 변경
        presenti_data-> size -= (8 + sizeof(int));
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20*(presentinode-1), SEEK_SET);
        fwrite(presenti_data, sizeof(INODE), 1 ,myfs);
    }

    free(presenti_data);
    free(filename);
    free(fileinode);
    free(i_data);
    fclose(myfs);
    return;
}
/*
이름    : cntfound 함수
작성자  : 오규빈
기능    : dir_list에서 현재 이어진 디렉토리 수 리턴
받는값  : X
리턴값  : count값
*/

int cntfound() 
{
    int cnt = 0;
    DIR_LIST* tmp_dir = front_dir_list_ptr;
    while ((tmp_dir->next_ptr) != NULL)
    {
        {
            tmp_dir = tmp_dir->next_ptr;
            cnt++;
            printf("fff\n");
        }
    }
    return cnt;//탐색 실패시

}

/*
이름    : prtpwd 함수
작성자  : 오규빈
기능    : pwd를 문자열로 리턴
받는값  : X
리턴값  : pwd 문자열값
*/
char* prtpwd()
{
    char* s;
    char* pwd;
    DIR_LIST *tmp_ptr = front_dir_list_ptr;

    while (1)
    {
        s = tmp_ptr->name;
        pwd = strcat(pwd,s);
        if (tmp_ptr->next_ptr == NULL)
        {
            break;
        }
        else
        {
            pwd = strcat(pwd,"/");
            tmp_ptr = tmp_ptr->next_ptr;
        }
    }
    return pwd;
}

/*
이름    : acc_inode 함수
작성자  : 오규빈
기능    : 가용중이지 않은 inode 번호를 가져오고, 그 inode번호에 해당하는 슈퍼블록을 1로 채운다.
받는값  : X
리턴값  : 가용중이지 않은 inode 번호
*/

int acc_inode()
{
    FILE* myfs;
    int t = 1;

    myfs = fopen("myfs", "rb");
    if (myfs == NULL)
    {
        printf("acc_inode() 함수 : 파일 열기에 실패했습니다.\n");
        abort();
    }

    SUPERBLOCK *sb_ptr = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
    fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
    fread(sb_ptr, sizeof(SUPERBLOCK), 1, myfs);//사용중인 슈퍼블록 확인하여 가용중이지 않은 아이노드에 파일 넣기 위함.

    unsigned mask;
    while(1){ // 빈 inode가 있을때 까지 무한 반복
    if (t < 33) // inode_1에 빈 inode가 있을 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->inode_1 & mask) == 0) //빈 아이노드가 있었을 경우
        {
            sb_ptr->inode_1 = sb_ptr->inode_1 | 1;//빈 아이노드 채우기
            break;
        }
        t++;
    }
    else if (t < 65) // inode_2에 빈 inode가 있을 경우
    {
        mask = 1 << ((t- 32) - 1);
        if ((sb_ptr->inode_2 & mask) == 0)
        {
            sb_ptr->inode_2 = sb_ptr->inode_2 | 1;
            break;
        }
        t++;
    }
    else if (t < 97) // inode_3에 빈 inode가 있을 경우
    {
        mask = 1 << ((t - 64) - 1);
        if ((sb_ptr->inode_3 & mask) == 0)
        {
          sb_ptr->inode_3 = sb_ptr->inode_3 | 1;
          break;
        }
        t++;
    }
    else // inode_4에 빈 inode가 있을 경우
    {
        mask = 1 << ((t - 96) - 1);
        if ((sb_ptr->inode_4 & mask) == 0)
        {
            sb_ptr->inode_4 = sb_ptr->inode_4 | 1;
            break;
        }
        t++;
    }
    }
    return t;
}

/*
이름    : acc_data 함수
작성자  : 오규빈
기능    : 가용중이지 않은 datablock 번호를 가져오고, 그 inode번호에 해당하는 슈퍼블록을 1로 채운다.
받는값  : X
리턴값  : 가용중이지 않은 datablock 번호
*/
int acc_data()
{
    FILE* myfs;
    int t = 1;

    myfs = fopen("myfs", "rb");
    if (myfs == NULL)
    {
        printf("acc_data() 함수 : 파일 열기에 실패했습니다.\n");
        abort();
    }

    SUPERBLOCK *sb_ptr = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
    fseek(myfs, BOOT_BLOCK_SIZE+128, SEEK_SET);
    fread(sb_ptr, sizeof(SUPERBLOCK), 1, myfs);//사용중인 슈퍼블록 확인하여 가용중이지 않은 데이터블록에 파일 넣기 위함.

    unsigned mask;
    while(1)
    { // 빈 데이터블록이 있을때 까지 무한 반복
    if (t < 33) // data_block_1에 빈 data_block이 있을 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_1 & mask) == 0) //빈 슈퍼블록이 있었을 경우
        {
            sb_ptr->data_block_1 = sb_ptr->data_block_1 | 1;//빈 슈퍼블록 채우기
            break;
        }
        t++;
    }
    else if (t < 65) // data_block_2에 빈 data_block이 있을 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_2 & mask) == 0) //빈 슈퍼블록이 있었을 경우
        {
            sb_ptr->data_block_2 = sb_ptr->data_block_2 | 1;//빈 슈퍼블록 채우기
            break;
        }
        t++;
    }
    else if (t < 97) // data_block_3에 빈 data_block이 있을 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_3 & mask) == 0) //빈 슈퍼블록이 있었을 경우
        {
            sb_ptr->data_block_3 = sb_ptr->data_block_3 | 1;//빈 슈퍼블록 채우기
            break;
        }
        t++;
    }
    else if (t < 129) // data_block_4에 빈 data_block이 있을 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_4 & mask) == 0) //빈 슈퍼블록이 있었을 경우
        {
            sb_ptr->data_block_4 = sb_ptr->data_block_4 | 1;//빈 슈퍼블록 채우기
            break;
        }
        t++;
    }
    else if (t < 161) // data_block_5에 빈 data_block이 있을 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_5 & mask) == 0) //빈 슈퍼블록이 있었을 경우
        {
            sb_ptr->data_block_5 = sb_ptr->data_block_5 | 1;//빈 슈퍼블록 채우기
            break;
        }
        t++;
    }
    else if (t < 193) // data_block_6에 빈 data_block이 있을 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_6 & mask) == 0) //빈 슈퍼블록이 있었을 경우
        {
            sb_ptr->data_block_6 = sb_ptr->data_block_6 | 1;//빈 슈퍼블록 채우기
            break;
        }
        t++;
    }
    else if (t < 225) // data_block_7에 빈 data_block이 있을 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_7 & mask) == 0) //빈 슈퍼블록이 있었을 경우
        {
            sb_ptr->data_block_7 = sb_ptr->data_block_7 | 1;//빈 슈퍼블록 채우기
            break;
        }
        t++;
    }
    else // data_block_3에 빈 data_block이 있을 경우
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_8 & mask) == 0) //빈 슈퍼블록이 있었을 경우
        {
            sb_ptr->data_block_8 = sb_ptr->data_block_8 | 1;//빈 슈퍼블록 채우기
            break;
        }
        t++;
    }
    }
    return t;
}

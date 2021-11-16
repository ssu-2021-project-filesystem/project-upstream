#include "user.h"

void mycp (const char* source_file, const char* dest_file  ){
    FILE *i; // 읽을파일
    FILE *o; // 새 파일
    if((i = fopen("source_file", "rb"))== NULL)
    {
        printf("error");
    }
    
    if((o = fopen("source_file", "wb"))== NULL)
    {
        printf("error");
    }

}
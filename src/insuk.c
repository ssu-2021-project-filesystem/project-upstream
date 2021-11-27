#include "user.h"
/*



*/
int checkbit(int number, unsigned block)
{
    if((number % 32) == 0)
    {
        int n = number + 32;
        unsigned mask = 1 << 31;
        for(;number<n; number++)
        {
            if((block & mask) == 0)
            {
                break;
            }
            mask >>= 1;
        }
    }
    return number;
}
/*



*/
void change_superblock(int saveinumber, int savedbnumber, SUPERBLOCK *sb_data)
{
    if(saveinumber >= 1 && saveinumber <= 32)
    {
        unsigned mask = 1 << 31;
        mask >>= saveinumber;
        sb_data-> inode_1 = sb_data-> inode_1 | mask;
    }
    else if(saveinumber >= 33 && saveinumber <= 64)
    {
        unsigned mask = 1 << 31;
        mask >>= (saveinumber - 32);
        sb_data-> inode_2 = sb_data-> inode_2 | mask;
    }
    else if(saveinumber >= 65 && saveinumber <= 96)
    {
        unsigned mask = 1 << 31;
        mask >>= (saveinumber - 64);
        sb_data-> inode_3 = sb_data-> inode_3 | mask;
    }
    else if(saveinumber >= 97 && saveinumber <= 128)
    {
        unsigned mask = 1 << 31;
        mask >>= (saveinumber - 96);
        sb_data-> inode_4 = sb_data-> inode_4 | mask;
    }

    if(savedbnumber >= 1 && savedbnumber <= 32)
    {
        unsigned mask = 1 << 31;
        mask >>= savedbnumber;
        sb_data-> data_block_1 = sb_data-> data_block_1 | mask;
    }
    else if(savedbnumber >= 33 && savedbnumber <= 64)
    {
        unsigned mask = 1 << 31;
        mask >>= (savedbnumber - 32);
        sb_data-> data_block_2 = sb_data-> data_block_2 | mask;
    }
    else if(savedbnumber >= 65 && savedbnumber <= 96)
    {
        unsigned mask = 1 << 31;
        mask >>= (savedbnumber - 64);
        sb_data-> data_block_3 = sb_data-> data_block_3 | mask;
    }
    else if(savedbnumber >= 97 && savedbnumber <= 128)
    {
        unsigned mask = 1 << 31;
        mask >>= (savedbnumber - 96);
        sb_data-> data_block_4 = sb_data-> data_block_4 | mask;
    }
    else if(savedbnumber >= 129 && savedbnumber <= 160)
    {
        unsigned mask = 1 << 31;
        mask >>= (savedbnumber - 128);
        sb_data-> data_block_5 = sb_data-> data_block_5 | mask;
    }
    else if(savedbnumber >= 161 && savedbnumber <= 192)
    {
        unsigned mask = 1 << 31;
        mask >>= (savedbnumber - 160);
        sb_data-> data_block_6 = sb_data-> data_block_6 | mask;
    }
    else if(savedbnumber >= 193 && savedbnumber <= 224)
    {
        unsigned mask = 1 << 31;
        mask >>= (savedbnumber - 192);
        sb_data-> data_block_7 = sb_data-> data_block_7 | mask;
    }
    else if(savedbnumber >= 225 && savedbnumber <= 256)
    {
        unsigned mask = 1 << 31;
        mask >>= (savedbnumber - 224);
        sb_data-> data_block_8 = sb_data-> data_block_8 | mask;
    }
}
/*



*/
void bit_print(unsigned a)
{
    unsigned mask = 1 << 31;
    for(int i = 1; i <= 32; ++i)
    {
        putchar(((a & mask) == 0) ? '0' : '1');
        mask >>= 1;
        if(i % 4 == 0)
            putchar(' ');
    }
}
/*



*/
void mymkdir(char *dir_name)
{
    FILE *myfs;
    myfs = fopen("myfs", "rb+");
    SUPERBLOCK *sb_data = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK)); 
    fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
    fread(sb_data, sizeof(SUPERBLOCK), 1, myfs);
    int saveinumber = 0;
    int savedbnumber = 0;
    saveinumber = checkbit(saveinumber, sb_data-> inode_1);
    saveinumber = checkbit(saveinumber, sb_data-> inode_2);
    saveinumber = checkbit(saveinumber, sb_data-> inode_3);
    saveinumber = checkbit(saveinumber, sb_data-> inode_4);
    saveinumber++;

    savedbnumber = checkbit(savedbnumber, sb_data-> data_block_1);
    savedbnumber = checkbit(savedbnumber, sb_data-> data_block_2);
    savedbnumber = checkbit(savedbnumber, sb_data-> data_block_3);
    savedbnumber = checkbit(savedbnumber, sb_data-> data_block_4);
    savedbnumber = checkbit(savedbnumber, sb_data-> data_block_5);
    savedbnumber = checkbit(savedbnumber, sb_data-> data_block_6);
    savedbnumber = checkbit(savedbnumber, sb_data-> data_block_7);
    savedbnumber = checkbit(savedbnumber, sb_data-> data_block_8);
    savedbnumber++;

    change_superblock(saveinumber, savedbnumber, sb_data);
    fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
    fwrite(sb_data, sizeof(SUPERBLOCK), 1, myfs);

    INODE *i_data = (INODE *)malloc(sizeof(INODE)); 
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20*(saveinumber-1), SEEK_SET);
    fread(i_data, sizeof(INODE), 1, myfs);

    time_t timer = time(NULL);
    struct tm* t;
    t = localtime(&timer);
    i_data-> type = 0;
    i_data-> year = (t-> tm_year + 1900);
    i_data-> month = (t-> tm_mon + 1);
    i_data-> date = t-> tm_mday;
    i_data-> hour = t-> tm_hour;
    i_data-> minute = t-> tm_min;
    i_data-> second = t-> tm_sec;
    i_data-> size = 24;
    i_data-> dir_1 = savedbnumber;
    i_data-> dir_2 = 0;
    i_data-> dir_3 = 0;
    i_data-> dir_4 = 0;
    i_data-> dir_5 = 0;
    i_data-> dir_6 = 0;
    i_data-> dir_7 = 0;
    i_data-> dir_8 = 0;
    i_data-> indir = 0;
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20*(saveinumber-1), SEEK_SET);
    rewind(myfs);
    fwrite(i_data, sizeof(INODE), 1, myfs);

    char *f_name1 = ".", *f_name2 = "..";
    int *saveinode;
    *saveinode = rear_dir_list_ptr-> inode;
    INODE *i_data2 = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20*(*saveinode-1), SEEK_SET);
    rewind(myfs);
    fread(i_data2, sizeof(INODE), 1, myfs);

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (i_data2-> dir_1 - 1)) + i_data2-> size, SEEK_SET);
    rewind(myfs);
    fwrite(dir_name, sizeof(dir_name), 1, myfs);
    fwrite(saveinode, sizeof(int), 1, myfs);
    char *minusone = (char *)malloc(sizeof(char));
    minusone = "-1";
    fwrite(minusone, sizeof(char), 1, myfs);

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (savedbnumber - 1)), SEEK_SET);
    fwrite(f_name1, sizeof(8), 1, myfs);
    fwrite(&saveinumber, sizeof(int), 1, myfs);
    fwrite(f_name2, sizeof(8), 1, myfs);
    fwrite(saveinode, sizeof(int), 1 ,myfs);

    fclose(myfs);

    return;
}
/*
    


*/
void myrmdir(char *givenname)
{
    int saveinode, *fileinode, count, saveinumber;
    saveinode = rear_dir_list_ptr-> inode;
    FILE *myfs;
    myfs = fopen("myfs.bin", "rb+");
    INODE *presenti_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20*(saveinode-1), SEEK_SET);
    fread(presenti_data, sizeof(INODE), 1, myfs);
    int n = presenti_data-> size/12;
    char *filename;
    for(int i=0; i<n; i++)
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (presenti_data-> dir_1 - 1)), SEEK_SET);
        fread(filename, sizeof(8), 1, myfs);
        fread(fileinode, sizeof(int), 1, myfs);
        if(strcmp(givenname, filename) == 0)
        {
            count = i;
            break;
        }
    }
    INODE *i_data = (INODE *)malloc(sizeof(INODE)); 
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20*(*fileinode-1), SEEK_SET);
    fread(i_data, sizeof(INODE), 1, myfs);
    saveinumber = *fileinode;

    if(i_data-> type == 0 && i_data-> size == 24) //데이터 크기 = 빈 디렉터리 크기
    {
        //디렉터리 삭제
        for(int i = count; i<(n-1); i++)
        {
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (presenti_data->dir_1 - 1)) + 12 * (i + 1), SEEK_SET);
            fread(filename, sizeof(8), 1, myfs);
            fread(fileinode, sizeof(int), 1, myfs);
            rewind(myfs);
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (presenti_data->dir_1 - 1)) + 12 * (i), SEEK_SET);
            fwrite(filename, sizeof(8), 1, myfs);
            fwrite(fileinode, sizeof(int), 1, myfs);
            rewind(myfs);            
        }
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (presenti_data->dir_1 - 1)) + 12 * (n - 1), SEEK_SET);
        char *minusone = "-1";
        fwrite(minusone, sizeof(char), 1, myfs);

        //슈퍼블록 수정
        SUPERBLOCK *sb_data = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK)); 
        fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
        fread(sb_data, sizeof(SUPERBLOCK), 1, myfs);
        if(saveinumber>0 && saveinumber<=32)
        {
            unsigned mask = 1;
            mask <<= (32 - saveinumber);
            sb_data-> inode_1 = sb_data-> inode_1 ^ mask;
        }
        else if(saveinumber>33 && saveinumber<=64)
        {
            unsigned mask = 1;
            mask <<= (32 - (saveinumber - 32));
            sb_data-> inode_1 = sb_data-> inode_1 ^ mask;
        }
        else if(saveinumber>65 && saveinumber<=96)
        {
            unsigned mask = 1;
            mask <<= (32 - (saveinumber - 64));
            sb_data-> inode_2 = sb_data-> inode_2 ^ mask;
        }
        else if(saveinumber>97 && saveinumber<=128)
        {
            unsigned mask = 1;
            mask <<= (32 - (saveinumber - 96));
            sb_data-> inode_1 = sb_data-> inode_1 ^ mask;
        }
        fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
        fwrite(sb_data, sizeof(SUPERBLOCK), 1, myfs);
    }
    else
    {
        printf("삭제하려는 디렉터리에 파일이 존재합니다.\n디렉터리를 삭제할수 없습니다.");
    }

    fclose(myfs);

    return;
}
/*



*/
void mystate(void)
{
    FILE *myfs;
    myfs = fopen("myfs.bin", "rb");
    SUPERBLOCK *sb_data = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK)); 
    fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
    fread(sb_data, sizeof(SUPERBLOCK), 1, myfs);
    int saveinumber = 0;
    int savedbnumber = 0;
    saveinumber = checkbit(saveinumber, sb_data-> inode_1);
    saveinumber = checkbit(saveinumber, sb_data-> inode_2);
    saveinumber = checkbit(saveinumber, sb_data-> inode_3);
    saveinumber = checkbit(saveinumber, sb_data-> inode_4);
    saveinumber++;

    savedbnumber = checkbit(savedbnumber, sb_data-> data_block_1);
    savedbnumber = checkbit(savedbnumber, sb_data-> data_block_2);
    savedbnumber = checkbit(savedbnumber, sb_data-> data_block_3);
    savedbnumber = checkbit(savedbnumber, sb_data-> data_block_4);
    savedbnumber = checkbit(savedbnumber, sb_data-> data_block_5);
    savedbnumber = checkbit(savedbnumber, sb_data-> data_block_6);
    savedbnumber = checkbit(savedbnumber, sb_data-> data_block_7);
    savedbnumber = checkbit(savedbnumber, sb_data-> data_block_8);
    savedbnumber++;

    printf("Inode state :\n");
    printf("Total : 128\n");
    printf("used : %d\n", (saveinumber - 1));
    printf("Available : %d\n", (129 - saveinumber));
    printf("Inode map :\n");
    bit_print(sb_data-> inode_1);
    bit_print(sb_data-> inode_2);
    bit_print(sb_data-> inode_3);
    bit_print(sb_data-> inode_4);
    printf("\n");

    printf("Data block state :\n");
    printf("Total : 128\n");
    printf("used : %d\n", (savedbnumber - 1));
    printf("Available : %d\n", (129 - savedbnumber));
    printf("Data block map :\n");
    bit_print(sb_data-> data_block_1);
    bit_print(sb_data-> data_block_2);
    bit_print(sb_data-> data_block_3);
    bit_print(sb_data-> data_block_4);
    bit_print(sb_data-> data_block_5);
    bit_print(sb_data-> data_block_6);
    bit_print(sb_data-> data_block_7);
    bit_print(sb_data-> data_block_8);
    printf("\n");

    fclose(myfs);

    return;
}
/*



*/
void mytouch(char *givenname)
{
    int saveinode;
    saveinode = rear_dir_list_ptr-> inode;
    FILE *myfs;
    myfs = fopen("myfs.bin", "rb+");
    INODE *i_data = (INODE *)malloc(sizeof(INODE)); 
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20*(saveinode-1), SEEK_SET);
    fread(i_data, sizeof(INODE), 1, myfs);
    
    char *filename;
    int *inodenumber, inodenumber2; 
    unsigned count = 0;
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (i_data-> dir_1 - 1)), SEEK_SET);
    while(1)
    {
        fread(filename, sizeof(8), 1, myfs);
        fread(inodenumber, sizeof(int), 1, myfs);
        inodenumber2 = *inodenumber;
        if(filename == NULL)
        {
            count = -1;
            break;
        }
        if(strcmp(givenname, filename) == 0)
        {
            break;
        }
        count++;
    }
    if(count == -1)
    {
        //파일생성
        SUPERBLOCK *sb_data = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK)); 
        fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
        fread(sb_data, sizeof(SUPERBLOCK), 1, myfs);
        int saveinumber = 0;
        int savedbnumber = 0;
        saveinumber = checkbit(saveinumber, sb_data-> inode_1);
        saveinumber = checkbit(saveinumber, sb_data-> inode_2);
        saveinumber = checkbit(saveinumber, sb_data-> inode_3);
        saveinumber = checkbit(saveinumber, sb_data-> inode_4);
        saveinumber++;

        savedbnumber = checkbit(savedbnumber, sb_data-> data_block_1);
        savedbnumber = checkbit(savedbnumber, sb_data-> data_block_2);
        savedbnumber = checkbit(savedbnumber, sb_data-> data_block_3);
        savedbnumber = checkbit(savedbnumber, sb_data-> data_block_4);
        savedbnumber = checkbit(savedbnumber, sb_data-> data_block_5);
        savedbnumber = checkbit(savedbnumber, sb_data-> data_block_6);
        savedbnumber = checkbit(savedbnumber, sb_data-> data_block_7);
        savedbnumber = checkbit(savedbnumber, sb_data-> data_block_8);
        savedbnumber++;

        change_superblock(saveinumber, savedbnumber, sb_data);
        fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
        fwrite(sb_data, sizeof(SUPERBLOCK), 1, myfs);

        INODE *i_data3 = (INODE *)malloc(sizeof(INODE));
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (20 * (saveinumber - 1)), SEEK_SET);
        fread(i_data3, sizeof(INODE), 1, myfs);
        time_t timer = time(NULL);
        struct tm* t;
        t = localtime(&timer);
        i_data3->type = 1;
        i_data3->year = (t->tm_year + 1900);
        i_data3->month = (t->tm_mon + 1);
        i_data3->date = t->tm_mday;
        i_data3->hour = t->tm_hour;
        i_data3->minute = t->tm_min;
        i_data3->second = t->tm_sec;
        i_data3->size = 0;
        i_data3->dir_1 = savedbnumber;
        i_data3->dir_2 = 0;
        i_data3->dir_3 = 0;
        i_data3->dir_4 = 0;
        i_data3->dir_5 = 0;
        i_data3->dir_6 = 0;
        i_data3->dir_7 = 0;
        i_data3->dir_8 = 0;
        i_data3->indir = 0;
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (20 * (saveinumber - 1)), SEEK_SET);
        fwrite(i_data3, sizeof(INODE), 1, myfs);
    }
    else
    {
        //파일수정시간 변경
        INODE *i_data2 = (INODE *)malloc(sizeof(INODE)); 
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (20*(inodenumber2 - 1)), SEEK_SET);
        fread(i_data2, sizeof(INODE), 1, myfs);
        time_t timer = time(NULL);
        struct tm* t;
        t = localtime(&timer);
        i_data2-> year = (t-> tm_year + 1900);
        i_data2-> month = (t-> tm_mon + 1);
        i_data2-> date = t-> tm_mday;
        i_data2-> hour = t-> tm_hour;
        i_data2-> minute = t-> tm_min;
        i_data2-> second = t-> tm_sec;
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (20*(inodenumber2 - 1)), SEEK_SET);
        fwrite(i_data2, sizeof(INODE), 1, myfs);
    }

    fclose(myfs);

    return;
}
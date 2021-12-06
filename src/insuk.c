#include "user.h"
/*
�̸�    : checkbit �Լ�
�ۼ���  : ���μ�
���    : ��Ʈ���˻�
�޴°�  : int, unsigned
���ϰ�  : int
*/
int checkbit(int number, unsigned block)
{
    if ((number % 32) == 0)
    {
        int n = number + 32;
        unsigned mask = 1 << 31;
        for (; number < n; number++)
        {
            if ((block & mask) == 0)
            {
                break;
            }
            mask >>= 1;
        }
    }
    return number;
}
/*
�̸�    : change_superblock �Լ�
�ۼ���  : ���μ�
���    : ���ۺ�ϱ���ü����
�޴°�  : int, int, SUPERBLOCK *
���ϰ�  : x
*/
void change_superblock(int saveinumber, int savedbnumber, SUPERBLOCK *sb_data)
{
    unsigned mask = 1 << 31;
    if (saveinumber >= 1 && saveinumber <= 32)
    {
        mask >>= (saveinumber - 1);
        sb_data->inode_1 = sb_data->inode_1 | mask;
    }
    else if (saveinumber >= 33 && saveinumber <= 64)
    {
        mask >>= (saveinumber - 33);
        sb_data->inode_2 = sb_data->inode_2 | mask;
    }
    else if (saveinumber >= 65 && saveinumber <= 96)
    {
        mask >>= (saveinumber - 65);
        sb_data->inode_3 = sb_data->inode_3 | mask;
    }
    else if (saveinumber >= 97 && saveinumber <= 128)
    {
        mask >>= (saveinumber - 97);
        sb_data->inode_4 = sb_data->inode_4 | mask;
    }

    unsigned mask1 = 1 << 31;
    if (savedbnumber >= 1 && savedbnumber <= 32)
    {
        mask1 >>= (savedbnumber - 1);
        sb_data->data_block_1 = sb_data->data_block_1 | mask1;
    }
    else if (savedbnumber >= 33 && savedbnumber <= 64)
    {
        mask1 >>= (savedbnumber - 33);
        sb_data->data_block_2 = sb_data->data_block_2 | mask1;
    }
    else if (savedbnumber >= 65 && savedbnumber <= 96)
    {
        mask1 >>= (savedbnumber - 65);
        sb_data->data_block_3 = sb_data->data_block_3 | mask1;
    }
    else if (savedbnumber >= 97 && savedbnumber <= 128)
    {
        mask1 >>= (savedbnumber - 97);
        sb_data->data_block_4 = sb_data->data_block_4 | mask1;
    }
    else if (savedbnumber >= 129 && savedbnumber <= 160)
    {
        mask1 >>= (savedbnumber - 129);
        sb_data->data_block_5 = sb_data->data_block_5 | mask1;
    }
    else if (savedbnumber >= 161 && savedbnumber <= 192)
    {
        mask1 >>= (savedbnumber - 161);
        sb_data->data_block_6 = sb_data->data_block_6 | mask1;
    }
    else if (savedbnumber >= 193 && savedbnumber <= 224)
    {
        mask1 >>= (savedbnumber - 193);
        sb_data->data_block_7 = sb_data->data_block_7 | mask1;
    }
    else if (savedbnumber >= 225 && savedbnumber <= 256)
    {
        mask1 >>= (savedbnumber - 225);
        sb_data->data_block_8 = sb_data->data_block_8 | mask1;
    }
}
/*
�̸�    : bit_print �Լ�
�ۼ���  : ���μ�
���    : ��Ʈ�����
�޴°�  : unsigned
���ϰ�  : x
*/
void bit_print(unsigned a)
{
    unsigned mask = 1 << 31;
    for (int i = 1; i <= 32; ++i)
    {
        putchar(((a & mask) == 0) ? '0' : '1');
        mask >>= 1;
        if (i % 4 == 0)
            putchar(' ');
    }
    return;
}
/*
�̸�    : mymkdir �Լ�
�ۼ���  : ���μ�
���    : ���͸�����
�޴°�  : char
���ϰ�  : x
*/
void mymkdir(char *dir_name)
{
    if (dir_name == NULL)
    {
        printf("���͸����� �ʿ��մϴ�.\n");
        return;
    }

    if (strlen(dir_name) > 7)
    {
        printf("���͸����� �ʹ� ��ϴ�.\n");
        return;
    }

    FILE *myfs;
    myfs = fopen("myfs", "rb+");

    //������ ���丮���� /�� �ƴ� ���, ������ ���ϸ��� ���� ����ó��
    if (strcmp(dir_name, "/") != 0)
    {
        int presentinode = rear_dir_list_ptr->inode;
        INODE *inode_data = (INODE *)malloc(sizeof(INODE));
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (presentinode - 1), SEEK_SET);
        fread(inode_data, sizeof(INODE), 1, myfs);
        char *filename = (char *)malloc(sizeof(char) * 8);
        int *inodenumber = (int *)malloc(sizeof(int));
        int n = inode_data->size / (8 + sizeof(int));

        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (inode_data->dir_1)), SEEK_SET);
        for (int i = 0; i < n; i++)
        {
            fread(filename, 8, 1, myfs);
            fread(inodenumber, sizeof(int), 1, myfs);

            if (strcmp(dir_name, filename) == 0)
            {
                printf("�̹� �����ϴ� �����Դϴ�.\n");
                return;
            }
        }
        free(inode_data);
    }

    // superblock �˻�
    SUPERBLOCK *sb_data = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
    fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
    fread(sb_data, sizeof(SUPERBLOCK), 1, myfs);

    int saveinumber = 0;
    int savedbnumber = 0;
    saveinumber = checkbit(saveinumber, sb_data->inode_1);
    saveinumber = checkbit(saveinumber, sb_data->inode_2);
    saveinumber = checkbit(saveinumber, sb_data->inode_3);
    saveinumber = checkbit(saveinumber, sb_data->inode_4);
    saveinumber++;

    savedbnumber = checkbit(savedbnumber, sb_data->data_block_1);
    savedbnumber = checkbit(savedbnumber, sb_data->data_block_2);
    savedbnumber = checkbit(savedbnumber, sb_data->data_block_3);
    savedbnumber = checkbit(savedbnumber, sb_data->data_block_4);
    savedbnumber = checkbit(savedbnumber, sb_data->data_block_5);
    savedbnumber = checkbit(savedbnumber, sb_data->data_block_6);
    savedbnumber = checkbit(savedbnumber, sb_data->data_block_7);
    savedbnumber = checkbit(savedbnumber, sb_data->data_block_8);
    savedbnumber++;

    change_superblock(saveinumber, savedbnumber, sb_data);
    fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
    fwrite(sb_data, sizeof(SUPERBLOCK), 1, myfs);

    INODE *i_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinumber - 1), SEEK_SET);
    fread(i_data, sizeof(INODE), 1, myfs);

    time_t timer = time(NULL);
    struct tm *t;
    t = localtime(&timer);
    i_data->type = 0;
    if (t->tm_year >= 100)
    {
        i_data->year = t->tm_year - 100;
    }
    else
    {
        i_data->year = t->tm_year;
    }
    i_data->month = (t->tm_mon + 1);
    i_data->date = t->tm_mday;
    i_data->hour = t->tm_hour;
    i_data->minute = t->tm_min;
    i_data->second = t->tm_sec;
    i_data->size = 2 * (8 + sizeof(int));
    i_data->dir_1 = savedbnumber - 1;
    i_data->dir_2 = 0;
    i_data->dir_3 = 0;
    i_data->dir_4 = 0;
    i_data->dir_5 = 0;
    i_data->dir_6 = 0;
    i_data->dir_7 = 0;
    i_data->dir_8 = 0;
    i_data->indir = 0;
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinumber - 1), SEEK_SET);
    fwrite(i_data, sizeof(INODE), 1, myfs);

    char *f_name1 = ".", *f_name2 = "..";
    int *saveinode = (int *)malloc(sizeof(int));
    INODE *i_data2 = (INODE *)malloc(sizeof(INODE));
    char *minusone = (char *)malloc(sizeof(char));
    *minusone = -1;

    if (strcmp(dir_name, "/") == 0)
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (savedbnumber - 1)), SEEK_SET);
        fwrite(f_name1, 8, 1, myfs);
        fwrite(&saveinumber, sizeof(int), 1, myfs);
        fwrite(f_name2, 8, 1, myfs);
        fwrite(&saveinumber, sizeof(int), 1, myfs);
        fwrite(minusone, sizeof(char), 1, myfs);
    }
    else
    {
        //���� ���丮�� inode ��������
        *saveinode = rear_dir_list_ptr->inode;
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (*saveinode - 1), SEEK_SET);
        fread(i_data2, sizeof(INODE), 1, myfs);

        //���� ���丮 datablock�� ���丮 �߰�
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (i_data2->dir_1)) + i_data2->size, SEEK_SET);
        fwrite(dir_name, 8, 1, myfs);
        fwrite(&saveinumber, sizeof(int), 1, myfs);
        fwrite(minusone, sizeof(char), 1, myfs);

        //���� ���丮 inode ����(size ���)
        i_data2->size += (8 + sizeof(int));
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (*saveinode - 1), SEEK_SET);
        fwrite(i_data2, sizeof(INODE), 1, myfs);

        //�� ���丮 datablock�� . .. �߰�
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (savedbnumber - 1)), SEEK_SET);
        fwrite(f_name1, 8, 1, myfs);
        fwrite(&saveinumber, sizeof(int), 1, myfs);
        fwrite(f_name2, 8, 1, myfs);
        fwrite(saveinode, sizeof(int), 1, myfs);
        fwrite(minusone, sizeof(char), 1, myfs);
    }

    free(sb_data);
    free(i_data);
    free(saveinode);
    free(i_data2);
    free(minusone);

    fclose(myfs);

    return;
}
/*
�̸�    : myrmdir �Լ�
�ۼ���  : ���μ�
���    : ���͸� ����
�޴°�  : char *
���ϰ�  : x
*/
void myrmdir(char *givenname)
{
    int saveinode, count, saveinumber;
    int *fileinode = (int *)malloc(sizeof(int));
    saveinode = rear_dir_list_ptr->inode;

    FILE *myfs;
    myfs = fopen("myfs", "rb+");

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

        if (strcmp(givenname, filename) == 0)
        {
            count = i;
            break;
        }
        else
        {
            none_tmp++;
        }
    }

    if (none_tmp == n) //�ش� �̸��� ������ �������� �ʴ� ���
    {
        printf("�ش� �̸��� ������ �������� �ʽ��ϴ�.\n");

        return;
    }
    else //�ش� �̸��� ������ �����ϴ� ���
    {
        //�ش� ������ �Ϲ� �������� �˻�
        INODE *file_inode_tmp_ptr = (INODE *)malloc(sizeof(INODE));
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (*fileinode - 1), SEEK_SET);
        fread(file_inode_tmp_ptr, sizeof(INODE), 1, myfs);
        if (file_inode_tmp_ptr->type == 1) //�ش� ������ �Ϲ� ������ ���
        {
            printf("�ش� ������ ���丮�� �ƴմϴ�.\n");

            free(file_inode_tmp_ptr);
            free(fileinode);
            free(presenti_data);
            free(filename);

            return;
        }
        else //�ش� ������ ���丮�� ���
        {
            free(file_inode_tmp_ptr);
        }
    }

    INODE *i_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (*fileinode - 1), SEEK_SET);
    fread(i_data, sizeof(INODE), 1, myfs);
    saveinumber = *fileinode;

    if (i_data->type == 0 && i_data->size == ((8 + sizeof(int)) * 2)) //������ ũ�� = �� ���͸� ũ��
    {
        //���͸� ����
        for (int i = count; i < (n); i++)
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

        //���ۺ�� ����
        SUPERBLOCK *sb_data = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
        fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
        fread(sb_data, sizeof(SUPERBLOCK), 1, myfs);
        unsigned mask = 1 << 31;
        if (saveinumber > 0 && saveinumber <= 32)
        {
            mask >>= (saveinumber - 1);
            sb_data->inode_1 = sb_data->inode_1 ^ mask;
        }
        else if (saveinumber > 33 && saveinumber <= 64)
        {
            mask >>= (saveinumber - 33);
            sb_data->inode_2 = sb_data->inode_2 ^ mask;
        }
        else if (saveinumber > 65 && saveinumber <= 96)
        {
            mask >>= (saveinumber - 65);
            sb_data->inode_3 = sb_data->inode_3 ^ mask;
        }
        else if (saveinumber > 97 && saveinumber <= 128)
        {
            mask >>= (saveinumber - 97);
            sb_data->inode_4 = sb_data->inode_4 ^ mask;
        }
        unsigned mask1 = 1 << 31;
        if (i_data->dir_1 >= 0 && i_data->dir_1 < 32)
        {
            mask1 >>= (i_data->dir_1);
            sb_data->data_block_1 = sb_data->data_block_1 ^ mask1;
        }
        else if (i_data->dir_1 >= 32 && i_data->dir_1 < 64)
        {
            mask1 >>= (i_data->dir_1 - 32);
            sb_data->data_block_2 = sb_data->data_block_2 ^ mask1;
        }
        else if (i_data->dir_1 >= 64 && i_data->dir_1 < 96)
        {
            mask1 >>= (i_data->dir_1 - 64);
            sb_data->data_block_3 = sb_data->data_block_3 ^ mask1;
        }
        else if (i_data->dir_1 >= 96 && i_data->dir_1 < 128)
        {
            mask1 >>= (i_data->dir_1 - 96);
            sb_data->data_block_4 = sb_data->data_block_4 ^ mask1;
        }
        else if (i_data->dir_1 >= 128 && i_data->dir_1 < 160)
        {
            mask1 >>= (i_data->dir_1 - 128);
            sb_data->data_block_5 = sb_data->data_block_5 ^ mask1;
        }
        else if (i_data->dir_1 >= 160 && i_data->dir_1 < 192)
        {
            mask1 >>= (i_data->dir_1 - 160);
            sb_data->data_block_6 = sb_data->data_block_6 ^ mask1;
        }
        else if (i_data->dir_1 >= 192 && i_data->dir_1 < 224)
        {
            mask1 >>= (i_data->dir_1 - 192);
            sb_data->data_block_7 = sb_data->data_block_7 ^ mask1;
        }
        else if (i_data->dir_1 >= 224 && i_data->dir_1 < 256)
        {
            mask1 >>= (i_data->dir_1 - 224);
            sb_data->data_block_8 = sb_data->data_block_8 ^ mask1;
        }
        fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
        fwrite(sb_data, sizeof(SUPERBLOCK), 1, myfs);
        free(sb_data);

        //������͸� ������ ����
        presenti_data->size -= (8 + sizeof(int));
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinode - 1), SEEK_SET);
        fwrite(presenti_data, sizeof(INODE), 1, myfs);
    }
    else
    {
        printf("�����Ϸ��� ���͸��� ������ �����մϴ�.\n���͸��� ������ �� �����ϴ�.\n");
    }

    free(presenti_data);
    free(i_data);
    free(fileinode);
    free(filename);
    fclose(myfs);

    return;
}
/*
�̸�    : mystate �Լ�
�ۼ���  : ���μ�
���    : ����������
�޴°�  : x
���ϰ�  : x
*/
void mystate(void)
{
    FILE *myfs;
    myfs = fopen("myfs", "rb");
    SUPERBLOCK *sb_data = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
    fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
    fread(sb_data, sizeof(SUPERBLOCK), 1, myfs);
    int saveinumber = 0;
    int savedbnumber = 0;
    saveinumber = checkbit(saveinumber, sb_data->inode_1);
    saveinumber = checkbit(saveinumber, sb_data->inode_2);
    saveinumber = checkbit(saveinumber, sb_data->inode_3);
    saveinumber = checkbit(saveinumber, sb_data->inode_4);
    saveinumber++;

    savedbnumber = checkbit(savedbnumber, sb_data->data_block_1);
    savedbnumber = checkbit(savedbnumber, sb_data->data_block_2);
    savedbnumber = checkbit(savedbnumber, sb_data->data_block_3);
    savedbnumber = checkbit(savedbnumber, sb_data->data_block_4);
    savedbnumber = checkbit(savedbnumber, sb_data->data_block_5);
    savedbnumber = checkbit(savedbnumber, sb_data->data_block_6);
    savedbnumber = checkbit(savedbnumber, sb_data->data_block_7);
    savedbnumber = checkbit(savedbnumber, sb_data->data_block_8);
    savedbnumber++;

    printf("Inode state :\n");
    printf("Total : 128\n");
    printf("used : %d\n", (saveinumber - 1));
    printf("Available : %d\n", (129 - saveinumber));
    printf("Inode map :\n");
    bit_print(sb_data->inode_1);
    bit_print(sb_data->inode_2);
    bit_print(sb_data->inode_3);
    bit_print(sb_data->inode_4);
    printf("\n");

    printf("Data block state :\n");
    printf("Total : 256 blocks / 65536 byte\n");
    printf("used : %d blocks / %d byte\n", (savedbnumber - 1), ((savedbnumber - 1) * 256));
    printf("Available : %d blocks / %d byte\n", (257 - savedbnumber), ((257 - savedbnumber) * 256));
    printf("Data block map :\n");
    bit_print(sb_data->data_block_1);
    bit_print(sb_data->data_block_2);
    bit_print(sb_data->data_block_3);
    bit_print(sb_data->data_block_4);
    bit_print(sb_data->data_block_5);
    bit_print(sb_data->data_block_6);
    bit_print(sb_data->data_block_7);
    bit_print(sb_data->data_block_8);
    printf("\n");

    free(sb_data);
    fclose(myfs);

    return;
}
/*
�̸�    : mytouch �Լ�
�ۼ���  : ���μ�
���    : ���ϻ��� �� �ð�����
�޴°�  : char *
���ϰ�  : x
*/
void mytouch(char *givenname)
{
    if (givenname == NULL)
    {
        printf("���ϸ��� �ʿ��մϴ�.\n");
        return;
    }

    if (strlen(givenname) > 7)
    {
        printf("���ڿ��� �ʹ� ��ϴ�. (�ִ� 7����)\n");

        return;
    }

    int saveinode;
    saveinode = rear_dir_list_ptr->inode; //���� ���丮�� inode ��ȣ ����
    FILE *myfs;
    myfs = fopen("myfs", "rb+");

    //���� ���丮�� inode ���� �ҷ�����
    INODE *i_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinode - 1), SEEK_SET);
    fread(i_data, sizeof(INODE), 1, myfs);

    char *filename = (char *)malloc(sizeof(char) * 8);
    int *inodenumber = (int *)malloc(sizeof(int));
    int inodenumber2;
    unsigned count = 0;

    int dir_file_num = i_data->size / (8 + sizeof(int));

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * i_data->dir_1), SEEK_SET);
    for (int i = 0; i < dir_file_num; i++)
    {
        fread(filename, 8, 1, myfs);
        fread(inodenumber, sizeof(int), 1, myfs);

        if (strcmp(givenname, filename) == 0)
        {
            inodenumber2 = *inodenumber;

            break;
        }
        else
        {
            count++;
        }
    }

    if (count == dir_file_num)
    {
        //���ϻ���
        SUPERBLOCK *sb_data = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
        fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
        fread(sb_data, sizeof(SUPERBLOCK), 1, myfs);
        int saveinumber = 0;
        int savedbnumber = 0;
        saveinumber = checkbit(saveinumber, sb_data->inode_1);
        saveinumber = checkbit(saveinumber, sb_data->inode_2);
        saveinumber = checkbit(saveinumber, sb_data->inode_3);
        saveinumber = checkbit(saveinumber, sb_data->inode_4);
        saveinumber++;

        savedbnumber = checkbit(savedbnumber, sb_data->data_block_1);
        savedbnumber = checkbit(savedbnumber, sb_data->data_block_2);
        savedbnumber = checkbit(savedbnumber, sb_data->data_block_3);
        savedbnumber = checkbit(savedbnumber, sb_data->data_block_4);
        savedbnumber = checkbit(savedbnumber, sb_data->data_block_5);
        savedbnumber = checkbit(savedbnumber, sb_data->data_block_6);
        savedbnumber = checkbit(savedbnumber, sb_data->data_block_7);
        savedbnumber = checkbit(savedbnumber, sb_data->data_block_8);
        savedbnumber++;

        change_superblock(saveinumber, savedbnumber, sb_data);
        fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
        fwrite(sb_data, sizeof(SUPERBLOCK), 1, myfs);

        INODE *i_data3 = (INODE *)malloc(sizeof(INODE));
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (20 * (saveinumber - 1)), SEEK_SET);
        fread(i_data3, sizeof(INODE), 1, myfs);
        time_t timer = time(NULL);
        struct tm *t;
        t = localtime(&timer);
        i_data3->type = 1;
        if (t->tm_year >= 100)
        {
            i_data3->year = t->tm_year - 100;
        }
        else
        {
            i_data3->year = t->tm_year;
        }
        i_data3->month = (t->tm_mon + 1);
        i_data3->date = t->tm_mday;
        i_data3->hour = t->tm_hour;
        i_data3->minute = t->tm_min;
        i_data3->second = t->tm_sec;
        i_data3->size = 0;
        i_data3->dir_1 = (unsigned)(savedbnumber - 1);
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

        //���� ���丮�� datablock�� �ݿ�
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * i_data->dir_1) + i_data->size, SEEK_SET);
        fwrite(givenname, 8, 1, myfs);
        fwrite(&saveinumber, sizeof(int), 1, myfs);
        char minus_tmp = -1;
        fwrite(&minus_tmp, sizeof(char), 1, myfs);

        //���� ���丮�� size ����� �ݿ�
        i_data->size += 8 + sizeof(int);
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + 20 * (saveinode - 1), SEEK_SET);
        fwrite(i_data, sizeof(INODE), 1, myfs);

        //������ ������ datablock�� -1 ����
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (savedbnumber - 1)), SEEK_SET);
        fwrite(&minus_tmp, sizeof(char), 1, myfs);

        free(sb_data);
        free(i_data3);
    }
    else
    {
        //���ϼ����ð� ����
        INODE *i_data2 = (INODE *)malloc(sizeof(INODE));
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (20 * (inodenumber2 - 1)), SEEK_SET);
        fread(i_data2, sizeof(INODE), 1, myfs);
        time_t timer = time(NULL);
        struct tm *t;
        t = localtime(&timer);
        if (t->tm_year >= 100)
        {
            i_data2->year = t->tm_year - 100;
        }
        else
        {
            i_data2->year = t->tm_year;
        }
        i_data2->month = (t->tm_mon + 1);
        i_data2->date = t->tm_mday;
        i_data2->hour = t->tm_hour;
        i_data2->minute = t->tm_min;
        i_data2->second = t->tm_sec;
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (20 * (inodenumber2 - 1)), SEEK_SET);
        fwrite(i_data2, sizeof(INODE), 1, myfs);

        free(i_data2);
    }

    free(i_data);
    free(filename);
    free(inodenumber);

    fclose(myfs);

    return;
}
//�������
#include "user.h"


//��ũ��
#define     COMMAND_SIZE            30      //command() �Լ����� �Է¹��� �� �ִ� ��ɾ��� ����


//��������
//move. �� ���Ͽ����� ����Ѵ�. ���� ������ user.h�� ���� �ۼ��صα�
int mytree_dir_layer = 0;


//�Լ�
/*
�̸�    : mypwd �Լ�
�ۼ���  : ������
���    : �۾� ���丮�� �����θ� ����Ѵ�
�޴°�  : X
���ϰ�  : X
*/
void mypwd(void)
{   
    DIR_LIST *tmp_ptr = front_dir_list_ptr;

    if (tmp_ptr == NULL)
    {
        printf("pwd�� ���� linked list�� �������� �ʽ��ϴ�.\n");

        return;
    }

    int first = 0;

    while (1)
    {
        printf("%s", tmp_ptr->name);

        //��� �̵�
        if (tmp_ptr->next_ptr == NULL)
        {
            break;
        }
        else
        {
            tmp_ptr = tmp_ptr->next_ptr;
        }

        //���
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
�̸�    : myinode �Լ�
�ۼ���  : ������
���    : ������ inode�� ������ ����Ѵ�
�޴°�  : inode ��ȣ(���ڿ�)
���ϰ�  : X
*/
void myinode(const char *ptr)
{
    if(ptr == NULL)
    {
        printf("���ڸ� �ۼ��ϼ���.\n");

        return;
    }

    // ptr�� ����Ű�� ���ڿ��� ������ ��ȯ, �ùٸ� ������ �˻�
    int inode = 0;  // inode ��ȣ
    int figure = 0; // ptr�� ����Ű�� �ִ� ���ڿ��� �ڸ���
    int ptr_offset = 0;

    while (1)
    {
        if (*(ptr + ptr_offset) == 0)
        {
            break;
        }
        else
        {
            if ((*(ptr + ptr_offset) < 48) || (*(ptr + ptr_offset) > 57)) //���� ���ڰ� �ƴ� ���ڰ� �ִ� ���
            {
                printf("inode ��ȣ�� �߸��Ǿ����ϴ�.\n");
                return;
            }
            else
            {
                ptr_offset++;
                figure++;
            }
        }
    }

    ptr_offset = 0; //���� ������ ���� �ʱ�ȭ

    for (int i = (figure - 1); i >= 0; i--)
    {
        inode += (*(ptr + ptr_offset) - 48) * (int_pow(10, i));

        ptr_offset++;
    }

    if ((inode > 128) || (inode < 1)) // 1~128�� ������ �ƴ� ���
    {
        printf("inode ��ȣ�� �߸��Ǿ����ϴ�.\n");
        return;
    }

    //���� ����
    FILE *myfs;
    myfs = fopen("myfs", "rb");
    if (myfs == NULL)
    {
        printf("myinode() �Լ� : ���� ���⿡ �����߽��ϴ�.\n");
        abort();
    }

    // myfs���� superblock ���� �б�
    SUPERBLOCK *sb_ptr = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
    fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
    fread(sb_ptr, sizeof(SUPERBLOCK), 1, myfs);

    // inode ��� ���� Ȯ���ϱ�
    unsigned mask = 1 << 31;

    if (inode < 33) // inode_1�� ������ ����ִ� ���
    {
        mask >>= (inode - 1);
        if ((sb_ptr->inode_1 & mask) == 0)
        {
            printf("�ش� inode�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (inode < 65) // inode_2�� ������ ����ִ� ���
    {
        mask >>= ((inode - 32) - 1);
        if ((sb_ptr->inode_2 & mask) == 0)
        {
            printf("�ش� inode�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (inode < 97) // inode_3�� ������ ����ִ� ���
    {
        mask >>= ((inode - 64) - 1);
        if ((sb_ptr->inode_3 & mask) == 0)
        {
            printf("�ش� inode�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else // inode_4�� ������ ����ִ� ���
    {
        mask >>= ((inode - 96) - 1);
        if ((sb_ptr->inode_4 & mask) == 0)
        {
            printf("�ش� inode�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }

    // myfs���� inode list ���� �б�
    INODE *inode_data_ptr = (INODE *)malloc(sizeof(INODE)); // inode ������ ������ �޸� ������ ����Ű�� ������
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (inode - 1)), SEEK_SET);
    fread(inode_data_ptr, sizeof(INODE), 1, myfs);

    // inode ���� ����ϱ�
    //����
    printf("���� : ");
    if (inode_data_ptr->type == 0) //���� ������ 0�� ���
    {
        printf("���丮\n");
    }
    else //���� ������ 1�� ���
    {
        printf("����\n");
    }
    //��������
    printf("�������� : ");
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

    //ũ��
    printf("ũ�� : %d\n", inode_data_ptr->size);

    // direct
    printf("���� ��� ��� : \n");

    printf(" #0 ���� ������ ��� : %d\n", inode_data_ptr->dir_1 + 1);

    if (inode_data_ptr->dir_2 != 0)
    {
        printf(" #1 ���� ������ ��� : %d\n", inode_data_ptr->dir_2 + 1);
    }
    if (inode_data_ptr->dir_3 != 0)
    {
        printf(" #2 ���� ������ ��� : %d\n", inode_data_ptr->dir_3 + 1);
    }
    if (inode_data_ptr->dir_4 != 0)
    {
        printf(" #3 ���� ������ ��� : %d\n", inode_data_ptr->dir_4 + 1);
    }
    if (inode_data_ptr->dir_5 != 0)
    {
        printf(" #4 ���� ������ ��� : %d\n", inode_data_ptr->dir_5 + 1);
    }
    if (inode_data_ptr->dir_6 != 0)
    {
        printf(" #5 ���� ������ ��� : %d\n", inode_data_ptr->dir_6 + 1);
    }
    if (inode_data_ptr->dir_7 != 0)
    {
        printf(" #6 ���� ������ ��� : %d\n", inode_data_ptr->dir_7 + 1);
    }
    if (inode_data_ptr->dir_8 != 0)
    {
        printf(" #7 ���� ������ ��� : %d\n", inode_data_ptr->dir_8 + 1);
    }

    // single indirect
    if(inode_data_ptr->indir == 0)
    {
        printf("���� ��� ��ȣ : 0\n");    
    }
    else
    {
        printf("���� ��� ��ȣ : %d\n", inode_data_ptr->indir + 1);
    }

    //���� �޸� �Ҵ� ���� �ݳ�
    free(inode_data_ptr);
    free(sb_ptr);

    //���� �ݱ�
    fclose(myfs);

    return;
}


/*
�̸�    : mydatablock �Լ�
�ۼ���  : ������
���    : ������ ������ ��Ͽ� ����ִ� ������ ����Ѵ�
�޴°�  : datablock ��ȣ(���ڿ�)
���ϰ�  : X
*/
void mydatablock(const char *ptr)
{
    //���ڰ� �ۼ����� ���� ���
    if(ptr == NULL)
    {
        printf("���ڸ� �ۼ��ϼ���.\n");

        return;
    }

    // ptr�� ����Ű�� ���ڿ��� ������ ��ȯ, �ùٸ� ������ �˻�
    int datablock = 0;  // datablock ��ȣ
    int figure = 0; // ptr�� ����Ű�� �ִ� ���ڿ��� �ڸ���
    int ptr_offset = 0;

    while (1)
    {
        if (*(ptr + ptr_offset) == 0)
        {
            break;
        }
        else
        {
            if ((*(ptr + ptr_offset) < 48) || (*(ptr + ptr_offset) > 57)) //���� ���ڰ� �ƴ� ���ڰ� �ִ� ���
            {
                printf("datablock ��ȣ�� �߸��Ǿ����ϴ�.\n");
                return;
            }
            else
            {
                ptr_offset++;
                figure++;
            }
        }
    }

    ptr_offset = 0; //���� ������ ���� �ʱ�ȭ

    for (int i = (figure - 1); i >= 0; i--)
    {
        datablock += (*(ptr + ptr_offset) - 48) * (int_pow(10, i));

        ptr_offset++;
    }

    if ((datablock > 256) || (datablock < 1)) // 1~256�� ������ �ƴ� ���
    {
        printf("datablock ��ȣ�� �߸��Ǿ����ϴ�.\n");
        return;
    }

    //���� ����
    FILE *myfs;
    myfs = fopen("myfs", "rb");
    if (myfs == NULL)
    {
        printf("mydatablock() �Լ� : ���� ���⿡ �����߽��ϴ�.\n");
        abort();
    }

    // myfs���� superblock ���� �б�
    SUPERBLOCK *sb_ptr = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
    fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
    fread(sb_ptr, sizeof(SUPERBLOCK), 1, myfs);

    // datablock ��� ���� Ȯ���ϱ�
    unsigned mask = 1 << 31;

    if (datablock < (32 * 1 + 1)) // data_block_1�� ������ ����ִ� ���
    {
        mask >>= (datablock - 1);
        if ((sb_ptr->data_block_1 & mask) == 0)
        {
            printf("�ش� datablock�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 2 + 1)) // data_block_2�� ������ ����ִ� ���
    {
        mask >>= ((datablock - (32 * 1)) - 1);
        if ((sb_ptr->data_block_2 & mask) == 0)
        {
            printf("�ش� datablock�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 3 + 1)) // data_block_3�� ������ ����ִ� ���
    {
        mask >>= ((datablock - (32 * 2)) - 1);
        if ((sb_ptr->data_block_3 & mask) == 0)
        {
            printf("�ش� datablock�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 4 + 1)) // data_block_4�� ������ ����ִ� ���
    {
        mask >>= ((datablock - (32 * 3)) - 1);
        if ((sb_ptr->data_block_4 & mask) == 0)
        {
            printf("�ش� datablock�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 5 + 1)) // data_block_5�� ������ ����ִ� ���
    {
        mask >>= ((datablock - (32 * 4)) - 1);
        if ((sb_ptr->data_block_5 & mask) == 0)
        {
            printf("�ش� datablock�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 6 + 1)) // data_block_6�� ������ ����ִ� ���
    {
        mask >>= ((datablock - (32 * 5)) - 1);
        if ((sb_ptr->data_block_6 & mask) == 0)
        {
            printf("�ش� datablock�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 7 + 1)) // data_block_7�� ������ ����ִ� ���
    {
        mask >>= ((datablock - (32 * 6)) - 1);
        if ((sb_ptr->data_block_7 & mask) == 0)
        {
            printf("�ش� datablock�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }
    else if (datablock < (32 * 8 + 1)) // data_block_8�� ������ ����ִ� ���
    {
        mask >>= ((datablock - (32 * 7)) - 1);
        if ((sb_ptr->data_block_8 & mask) == 0)
        {
            printf("�ش� datablock�� ��� ���� �ƴմϴ�.\n");
            free(sb_ptr);
            fclose(myfs);
            return;
        }
    }

    //datablock ���
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
    
    //���� �޸� �Ҵ� ���� �ݳ�
    free(sb_ptr);
    free(datablock_ptr);

    //���� �ݱ�
    fclose(myfs);

    return;
}


/*
�̸�    : command �Լ�
�ۼ���  : ������
���    : linux ��ɾ �����Ѵ�
�޴°�  : X
���ϰ�  : X
*/
void command(void)
{
    //linux ��ɾ� �Է�
    char *command_ptr = (char *)malloc(sizeof(char) * COMMAND_SIZE);
    char tmp_char;
    int loop_char_num = 0;

    printf("linux ��ɾ �Է��ϼ��� : ");

    while((tmp_char = getchar()) != '\n')
    {
        *(command_ptr + loop_char_num) = tmp_char;

        loop_char_num++;
    }
    *(command_ptr + loop_char_num) = 0; //NULL ����

    //linux ��ɾ� ����
    system(command_ptr);

    return;
}


/*
�̸�    : �ŵ����� �Լ�
�ۼ���  : ������
���    : �ۼ��� �� ���ڷ� �ŵ������� �����Ѵ�
�޴°�  : �ŵ������� ������ �� ����
���ϰ�  : �ŵ����� ��� �����
*/
int int_pow(int a, int b) //a�� b �ŵ������� �����Ѵ�
{
    int value = 1;

    for(int i = 0; i < b; i++)
    {
        value *= a;
    }

    return value;
}


/*
�̸�    : mytree �Լ�
�ۼ���  : ������
���    : ���丮 ������ ����Ѵ�
�޴°�  : ������ ����� ���丮�� ������
���ϰ�  : X
*/
void mytree(const char *path_ptr)
{
    int tree_inode; //������ ����� ���丮�� inode ��ȣ�� ������ ����

    //��ɾ�κ��� ���޵� ���� ó��
    if(path_ptr == NULL) //���ڰ� �Էµ��� ���� ���
    {
        tree_inode = rear_dir_list_ptr->inode; //���� ���丮�� inode ����
    }
    else //���ڰ� �Էµ� ���
    {
        tree_inode = path_to_inode(path_ptr); //�ش� ���丮�� inode�� ����

        //��ΰ� �߸��� ��쿡 ���� ����ó��
        if(tree_inode == 0)
        {
            printf("�߸��� ����Դϴ�.\n");

            return;
        }
    }

    //���丮 ���� ���
    FILE *myfs;
    myfs = fopen("myfs", "rb+");
    if (myfs == NULL)
    {
        printf("mytree() �Լ� : ���� ���⿡ �����߽��ϴ�.\n");
        abort();
    }

    dir_print(tree_inode, myfs);

    printf("\n");

    fclose(myfs);

    return;
}


/*
�̸�    : path_to_inode �Լ�
�ۼ���  : ������
���    : �����θ� �޾Ƽ� �ش� ���丮�� inode ��ȣ�� �����Ѵ�
�޴°�  : ���丮�� ������
���ϰ�  : X
*/
int path_to_inode(const char *path_ptr)
{
    int inode = 1; //inode ��ȣ�� ������ ����. �⺻�� 1(root)
    
    int tmp_datablock; //datablock ��ȣ�� ������ ����
    
    INODE *inode_ptr = (INODE *)malloc(sizeof(INODE)); //inode�� ����ų ������

    char *tmp_cmd_string_ptr = (char *)malloc(sizeof(char) * 8); //path���� ������ ���丮���� ����ų ������
    
    char *tmp_dir_string_ptr = (char *)malloc(sizeof(char) * 8); //���丮�� datablock���� ������ ���丮���� ����ų ������
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //���丮�� datablock���� ������ inode ��ȣ�� ����ų ������

    FILE *myfs;
    myfs = fopen("myfs", "rb");
    if (myfs == NULL)
    {
        printf("path_to_inode() �Լ� : ���� ���⿡ �����߽��ϴ�.\n");
        abort();
    }

    path_ptr++; //��Ʈ(/) �Ѿ��

    while(1)
    {
        //path���� ���ڿ� �и�
        if(sscanf(path_ptr ,"%[^/\\0]", tmp_cmd_string_ptr) != 1) //tmp_string_ptr���� ���丮���� ����ǰ�, �� ������ NULL�� ��
        {
            break;
        }

        path_ptr = path_ptr + (strlen(tmp_cmd_string_ptr) + 1);

        //inode�� ���丮�� �̵�, �ش� datablock���� �̵�
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (inode - 1)), SEEK_SET);
        fread(inode_ptr, sizeof(INODE), 1, myfs);

        tmp_datablock = (int)(inode_ptr->dir_1 + 1);
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (256 * (tmp_datablock - 1)), SEEK_SET);

        //datablock�� ���丮�� ����, tmp_cmd_string_ptr�� ��
        int diff_error = 0;
        for(int i = 0; i < ((inode_ptr->size) / (8 + sizeof(int))); i++)
        {
            fread(tmp_dir_string_ptr, 8, 1, myfs);

            if(strcmp(tmp_dir_string_ptr, tmp_cmd_string_ptr) == 0) //�� ���ڿ��� ���� ���
            {
                fread(tmp_inode_ptr, sizeof(int), 1, myfs);
                inode = *tmp_inode_ptr;

                break;
            }
            else //�� ���ڿ��� �ٸ� ���
            {
                fread(tmp_inode_ptr, sizeof(int), 1, myfs); //inode ��ȣ �ǳʶٱ�
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
�̸�    : dir_print �Լ�
�ۼ���  : ������
���    : ����Լ��� ���Ǿ� ���丮 ������ ����Ѵ�
�޴°�  : inode ��ȣ, myfs ����������
���ϰ�  : X
*/
void dir_print(int inode, FILE *myfs)
{
    mytree_dir_layer++;

    INODE *inode_ptr = (INODE *)malloc(sizeof(INODE)); //inode�� ����ų ������
    INODE *inode_file_ptr = (INODE *)malloc(sizeof(INODE)); //���丮 ���� ������ inode�� ����ų ������

    int tmp_datablock; //inode�� datablock ��ȣ�� ������ ����

    char *tmp_dir_string_ptr = (char *)malloc(sizeof(char) * 8); //���丮�� datablock���� ������ ���丮���� ����ų ������
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //���丮�� datablock���� ������ inode ��ȣ�� ����ų ������

    //�ش� inode ��ȣ�� datablock ��ȣ ����
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (inode - 1)), SEEK_SET);
    fread(inode_ptr, sizeof(INODE), 1, myfs);
    tmp_datablock = (int)(inode_ptr->dir_1 + 1);

    //���� ���� �˻�
    if(inode_ptr->type == 1) //�Ϲ� ������ ���
    {
        printf("�ش� ������ �Ϲ� �����Դϴ�.\n");

        return;
    }

    //inode�� �ش��ϴ� �̸� ���
    for(int i = 0; i < (inode_ptr->size / (8 + sizeof(int))); i++)
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (256 * (tmp_datablock - 1)) + (i * (8 + sizeof(int))), SEEK_SET);
        fread(tmp_dir_string_ptr, 8, 1, myfs); //���丮��
        fread(tmp_inode_ptr, sizeof(int), 1, myfs); //inode

        if(strcmp(tmp_dir_string_ptr, "..") == 0)
        {
            //���丮�� ���
            if(inode == 1)
            {
                printf("%-7s", "/");
            }
            else
            {
                //..���� inode ã�Ƽ� ���丮�� ���
                printf("%-7s", current_dir_find(inode, *tmp_inode_ptr, myfs));
            }
        }
    }

    //���丮 ó��
    int first = 0;
    for(int i = 0; i < (inode_ptr->size / (8 + sizeof(int))); i++)
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (256 * (tmp_datablock - 1)) + (i * (8 + sizeof(int))), SEEK_SET);
        fread(tmp_dir_string_ptr, 8, 1, myfs); //���丮��
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
            //�ش� ������ ���丮���� �˻�
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (*tmp_inode_ptr - 1)), SEEK_SET);
            fread(inode_file_ptr, sizeof(INODE), 1, myfs);

            if(inode_file_ptr->type == 1) //�ش� ������ �Ϲ� ������ ���
            {
                continue;
            }
            
            //ȭ��ǥ ���
            if(first != 0) //ù ��°�� ��µǴ� ���丮�� �ƴ� ���
            {
                printf("\n");

                for(int j = 0; j < mytree_dir_layer; j++)
                {
                    if(j == (mytree_dir_layer - 1)) //���������� ->���� ���
                    {
                        printf("       -> ");
                    }
                    else
                    {
                        printf("          ");
                    }
                }
            }
            else //ù ��°�� ��µǴ� ���丮�� ���
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
�̸�    : current_dir_find �Լ�
�ۼ���  : ������
���    : �ش� ���丮�� inode ��ȣ�� �θ� ���丮�� inode ��ȣ�� �ش� ���丮�� ���丮���� ���Ѵ�
�޴°�  : inode ��ȣ, ���� ���丮�� inode ��ȣ, myfs ����������
���ϰ�  : X
*/
char *current_dir_find(int inode, int high_inode, FILE *myfs)
{
    INODE *inode_ptr = (INODE *)malloc(sizeof(INODE)); //inode�� ����ų ������

    char *tmp_dir_string_ptr = (char *)malloc(sizeof(char) * 8); //���丮�� datablock���� ������ ���丮���� ����ų ������
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //���丮�� datablock���� ������ inode ��ȣ�� ����ų ������

    int tmp_datablock; //inode�� datablock ��ȣ�� ������ ����

    //..�� ���丮 ����Ʈ�� ����� datablock ��ȣ ����
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (high_inode - 1)), SEEK_SET);
    fread(inode_ptr, sizeof(INODE), 1, myfs);
    tmp_datablock = (int)(inode_ptr->dir_1 + 1);

    //���丮 ã�Ƽ� ����
    for(int i = 0; i < (inode_ptr->size / (8 + sizeof(int))); i++)
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (256 * (tmp_datablock - 1)) + (i * (8 + sizeof(int))), SEEK_SET);
        fread(tmp_dir_string_ptr, 8, 1, myfs); //���丮��
        fread(tmp_inode_ptr, sizeof(int), 1, myfs); //inode

        if(inode == *tmp_inode_ptr)
        {
            break;
        }
    }    

    return tmp_dir_string_ptr;
}


/*
�̸�    : mymkfs �Լ�
�ۼ���  : ������
���    : ���Ͻý����� �����, ���� ������ �ʱ�ȭ�Ѵ�.
�޴°�  : X
���ϰ�  : X 
*/
void mymkfs(void)
{
    //pwd linked list�� �����ϴ� ���, ����
    if(front_dir_list_ptr != NULL)
    {
        //��� ���� �����ϱ�
        DIR_LIST *tmp_delete_ptr; //���� ��带 ����ų ������
        DIR_LIST *tmp_delete_back_ptr; //���� ����� �� ��带 ����ų ������
        
        while(1)
        {
            tmp_delete_ptr = front_dir_list_ptr;
            tmp_delete_back_ptr = NULL;

            //���� ������ ���� �̵�
            while(tmp_delete_ptr->next_ptr != NULL)
            {
                tmp_delete_back_ptr = tmp_delete_ptr;
                tmp_delete_ptr = tmp_delete_ptr->next_ptr;
            }

            //��� ����, ����
            if(tmp_delete_back_ptr == NULL) //��尡 �ϳ� ���� ���(root�� ���)
            {
                free(tmp_delete_ptr->name);
                free(tmp_delete_ptr);

                break;
            }
            else
            {
                tmp_delete_back_ptr->next_ptr = NULL;
                free(tmp_delete_ptr->name);
                free(tmp_delete_ptr);
            }
        }
    }

    //pwd linked list ����
    front_dir_list_ptr = (DIR_LIST *)malloc(sizeof(DIR_LIST));
    front_dir_list_ptr->inode = 1;
    front_dir_list_ptr->name = (char *)malloc(sizeof(char) * 8);
    strcpy(front_dir_list_ptr->name, "/");
    front_dir_list_ptr->next_ptr = NULL;
    rear_dir_list_ptr = front_dir_list_ptr;

    //myfs ���� ���� ���� Ȯ��
    int exist; //������ �������� �ʴ� ��� 0, �����ϴ� ��� 1
    FILE *myfs_exist;

    if((myfs_exist = fopen("myfs", "rb")) == NULL) //������ �������� �ʴ� ���
    {
        exist = 0;
    }
    else //������ �����ϴ� ���
    {
        exist = 1;

        fclose(myfs_exist);
    }

    //fs ����
    int new_fs; //�� ���Ͻý����� ������ ��� 1

    if(exist == 0) //������ �������� �ʴ� ���
    {
        printf("���Ͻý����� �����ϴ�. ���Ͻý����� ����ϴ�.\n");
        
        new_fs = 1;
    }
    else //������ �����ϴ� ���
    {
        char char_tmp;
        char trash;

        while(1)
        {
            printf("���Ͻý����� �ֽ��ϴ�. �ٽ� ����ڽ��ϱ�? (y/n) ");
            char_tmp = getchar();
            trash = getchar();

            rewind(stdin); //���� ����

            if((char_tmp == 'y') || (char_tmp == 'Y'))
            {
                printf("���Ͻý����� �ٽ� ��������ϴ�.\n");
                new_fs = 1;
                break;
            }
            else if((char_tmp == 'n') || (char_tmp == 'N'))
            {
                printf("���Ͻý��� ������� ����߽��ϴ�.\n");
                return;
            }
            else
            {
                printf("�߸��� �Է��Դϴ�.\n");
            }
        }
    }

    if(new_fs == 1)
    {
        FILE *myfs;

        //myfs ���� ����
        myfs = fopen("myfs", "wb");

        //superblock �ʱ�ȭ
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

        //root ���丮 ����
        mymkdir("/");
    }

    return;
}


/*
�̸�    : mymv �Լ�
�ۼ���  : ���μ�, ������
���    : ������ �̸��� �ٲٰų� �ٸ� ���丮�� �̵���Ų��
�޴°�  : X
���ϰ�  : X
*/
void mymv(char *file_1, char *file_2)
{
    //���ڰ� �ۼ����� ���� ���
    if((file_1 == NULL) || (file_2 == NULL))
    {
        printf("���ڸ� �Է��ϼ���.\n");

        return;
    }

    FILE *myfs;
    myfs = fopen("myfs", "rb+");

    //���� ���丮�� inode ���� �б�
    int saveinode = rear_dir_list_ptr->inode; //���� ���丮�� inode ��ȣ�� ������ ����
    INODE *i_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (saveinode - 1), SEEK_SET);
    fread(i_data, sizeof(INODE), 1, myfs);

    int number_of_file = (i_data->size / (8 + sizeof(int)));

    //file_2 ã��
    char *tmp_filename = (char *)malloc(sizeof(char) * 8);
    int *tmp_inodenumber = (int *)malloc(sizeof(int));
    
    int dir_inode = 0;
    int filetype = 1; //�� ��° ���ڷ� �ۼ��� ������ ������ ������ ����. 0�̸� ���丮 1�̸� �Ϲ� ����
    INODE *f_inode = (INODE *)malloc(sizeof(INODE)); //���� ���丮 ���� ������ inode ������ ������ ����

    for(int i = 0; i < number_of_file; i++) //���丮 ���� ���� ������ŭ�� Ƚ���� ���� ����
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * i_data->dir_1) + (i * (8 + sizeof(int))), SEEK_SET);
        fread(tmp_filename, 8, 1, myfs); //ù ��° ���ڿ� ���� ������ �̸�
        fread(tmp_inodenumber, sizeof(int), 1, myfs); //ù ��° ���ڿ� ���� ������ inode

        if(strcmp(file_2, tmp_filename) == 0)
        {
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (*tmp_inodenumber - 1)), SEEK_SET);
            fread(f_inode, sizeof(INODE), 1, myfs);

            if(f_inode->type == 0) //�ش� ������ ���丮�� ���
            {
                filetype = 0;
                dir_inode = *tmp_inodenumber;
                
                break;
            }
            else //�ش� ������ �Ϲ� ������ ���
            {
                printf("%s ��/�� �̹� �����ϴ� �����Դϴ�.\n", file_2);

                free(i_data);
                free(tmp_filename);
                free(tmp_inodenumber);
                free(f_inode);

                fclose(myfs);

                return;
            }
        }
    }

    //file_1 ã��
    int file_inode = 0;
    int count = 0; //���� ���丮����, ù ��° ���ڷ� �ۼ��� ������ ��ġ�� ������ ����

    for(int i = 0; i < number_of_file; i++) //���丮 ���� ���� ������ŭ�� Ƚ���� ���� ����
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * i_data->dir_1) + (i * (8 + sizeof(int))), SEEK_SET);
        fread(tmp_filename, 8, 1, myfs); //ù ��° ���ڿ� ���� ������ �̸�
        fread(tmp_inodenumber, sizeof(int), 1, myfs); //ù ��° ���ڿ� ���� ������ inode

        if(strcmp(file_1, tmp_filename) == 0)
        {
            fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (*tmp_inodenumber - 1)), SEEK_SET);
            fread(f_inode, sizeof(INODE), 1, myfs);

            if(f_inode->type == 0) //���丮�� ���
            {
                printf("%s ��/�� ���丮�Դϴ�.\n", file_1);

                free(i_data);
                free(tmp_filename);
                free(tmp_inodenumber);
                free(f_inode);

                return;
            }
            else //�Ϲ� ������ ���
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

    if(file_inode == 0) //file_1�� ������ ������ �������� �ʴ� ���
    {
        printf("%s ��/�� �������� �ʽ��ϴ�.\n", file_1);

        return;
    }

    //�۾� ����
    if(filetype == 1) //�ش� ������ �Ϲ� ������ ���(source_file, dest_file�� ���)
    {
        //file_2�� �̸� ����
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * i_data->dir_1) + ((8 + sizeof(int)) * count), SEEK_SET);
        fwrite(file_2, 8, 1, myfs);
    }
    else //�ش� ������ ���丮�� ���(file, directory�� ���)
    {
        //file_2 ���丮�� �ش� ���� �̵�
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
�̸�    : mv_to_dir �Լ�
�ۼ���  : ������
���    : ������ �ٸ� ���丮�� �̵���Ų��
�޴°�  : myfs ����������, file_1�� inode, file_2�� inode, file_1�� �̸�, count ����
���ϰ�  : X
*/
void mv_to_dir(FILE *myfs, int file_1_inode, int file_2_inode, char *file_1_name, int count)
{
    //���� ���丮�� inode ���� �б�
    int saveinode = rear_dir_list_ptr->inode; //���� ���丮�� inode ��ȣ�� ������ ����
    INODE *i_data = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (saveinode - 1), SEEK_SET);
    fread(i_data, sizeof(INODE), 1, myfs);

    int number_of_file = (i_data->size / (8 + sizeof(int)));

    //���� ���丮 datablock���� file_1 ����
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

    //�� �������� -1 ����
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * i_data->dir_1) + ((number_of_file - 1) * (8 + sizeof(int))), SEEK_SET);
    char *tmp_char = (char *)malloc(sizeof(char));
    *tmp_char = -1;
    fwrite(tmp_char, sizeof(char), 1, myfs);

    //���� ���丮�� ũ�� ����
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (saveinode - 1), SEEK_SET);
    i_data->size = i_data->size - (8 + sizeof(int));
    fwrite(i_data, sizeof(INODE), 1, myfs);

    //file_2 ���丮 datablock�� �� ������ ��ġ�� �̵�, file_1 �߰�
    INODE *dir_inode = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (file_2_inode - 1), SEEK_SET);
    fread(dir_inode, sizeof(INODE), 1, myfs);

    int *tmp_int = (int *)malloc(sizeof(int));
    *tmp_int = file_1_inode;

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * dir_inode->dir_1) + dir_inode->size, SEEK_SET);
    fwrite(file_1_name, 8, 1, myfs);
    fwrite(tmp_int, sizeof(int), 1, myfs);

    //�� �������� -1 ����
    fwrite(tmp_char, sizeof(char), 1, myfs);

    //file_2 ���丮�� ũ�� ����
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

#include "user.h"


/*
�̸�    : myls �Լ�
�ۼ���  : �̿���
���    : ���� ��� �ڼ��� ����
�޴°�  : ���� ��
���ϰ�  : X
*/
void myls(const char* filename)
{
    // ���� ����
    FILE *myfs;
    myfs = fopen("myfs", "rb");

    //���� ���丮���� ���ڷ� �Է¹��� ���丮���� inode ��ȣ ã��

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

    //�ش� inode�� datablock ��ȣ ã��

    INODE *file_d = (INODE *)malloc(sizeof(INODE));
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (*fileinode - 1), SEEK_SET);
    fread(file_d, sizeof(INODE), 1, myfs);
    int data_num = (int)(file_d->dir_1 + 1);


    // datablock���� ������ ���� ã�Ƴ��� �� ���� ��� ������ inode�� ���λ��� ����

    n = file_d->size / (8 + sizeof(int));


    for(int i = count; i < n; i++)
    {
        int i = 0;
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + ((DATA_BLOCK_SIZE * (data_num)) + i), SEEK_SET);
        fread(filename, 8, 1, myfs);
        fread(fileinode, sizeof(int), 1, myfs);

        // ���ϸ�
        printf("%s" ,filename);

        // inode list ���� �б�
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (*fileinode - 1)), SEEK_SET);
        INODE *inode_ptr = (INODE *)malloc(sizeof(INODE));
        fread(inode_ptr, sizeof(INODE), 1, myfs);

        // ��������
        printf("    �������� : ");
        
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


        // ����
        printf("���� : ");

        if (inode_ptr->type == 0) // ���� ���� 0 = ���丮
        {
            printf(" ���丮");
        }
        else // ���� ���� 1 = �Ϲ� ����
        {
            printf(" ����");
        }

        // inode ��ȣ
        printf("    inode��ȣ : %d", inode_ptr);


        // ũ��
        printf("    ũ�� : %d\n", inode_ptr->size);

        fseek(myfs, -1, SEEK_CUR);
        fseek(myfs,sizeof(int),SEEK_CUR);

        i++;
    }


    fclose(myfs);

    return;
}
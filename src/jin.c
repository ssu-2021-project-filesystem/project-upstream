#include "user.h"


/*
�̸�    : myls �Լ�
�ۼ���  : �̿���
���    : ���� ��� �ڼ��� ����
�޴°�  : ���� ��
���ϰ�  : X
*/
void myls(const char* file )
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

    while(c = getchar() != EOF)
    {
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + INODE_LIST_SIZE + (DATA_BLOCK_SIZE * (data_num)) + (8 + sizeof(int)) * (i + 1), SEEK_SET);
        fread(filename, 8, 1, myfs);
        fread(fileinode, sizeof(int), 1, myfs);

        // ���ϸ�
        printf("%d" filename);

        // inode list ���� �б�
        fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (fileinode - 1)), SEEK_SET);
        INODE *file_inode_tmp_ptr = (INODE *)malloc(sizeof(INODE));
        fread(inode_data_ptr, sizeof(INODE), 1, myfs);

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
        printf("    inode��ȣ : %d", inode_num);


        // ũ��
        printf("    ũ�� : %d\n", inode_ptr->size);

        fseek(myfs, -1, SEEK_CUR);
        fseek(myfs,sizeof(int),SEEK_CUR);
    }


    free(inode_ptr);
    fclose(myfs);

    return;
}




/*
�̸�    : mycat �Լ�
�ۼ���  : �̿���
���    : ���� ���� ��� ��ɾ�
�޴°�  : X
���ϰ�  : X
*/
void mycat(char *file)
{
    FILE *myfs;
    myfs = fopen("myfs", "rb");

    INODE *inode_data = (INODE *)malloc(sizeof(INODE));
    int num = rear_dir_list_ptr->inode; //���� ���丮�� inode ��ȣ�� ������ ����
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (num - 1), SEEK_SET);
    fread(inode_data, sizeof(INODE), 1, myfs);
    int file_num = (inode_data->size / (8 + sizeof(int)));
        

    // ���� ã��
    int dir_inode = 0;

    INODE *f_inode = (INODE *)malloc(sizeof(INODE)); //���� ���丮 ���� ������ inode ������ ������ ����
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

            if(f_inode->type == 0) //���丮�� ���
            {
                printf("%s ��/�� ���丮�Դϴ�.\n", file);

                free(inode_data);
                free(fn);
                free(in);
                free(f_inode);

                return;
            }
            else //�Ϲ� ������ ���
            {
                int number = *in;
                if(number == 0)
                {
                    printf("%s ��/�� �������� �ʽ��ϴ�.\n", file);

                    return;
                }

                else // ���
                {
                    char *test;
                    fread(test,sizeof(file),1,myfs);
                    printf("%s", test);
                }


                break



        while((c = getchar()) != EOF){
    
            fseek(myfs, -1, SEEK_CUR);
            fseek(myfs,sizeof(int),SEEK_CUR);//���� ������ ��ġ�κ��� int�� ũ�⸸ŭ �̵�
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
�̸�    : myshowfile �Լ�
�ۼ���  : �̿���
���    : ���� ������ �κ� ��� ��ɾ�
�޴°�  : X
���ϰ�  : X
*/
void myshowfile(char *num1, char *num2, char *file)
{
    FILE *myfs;
    myfs = fopen("myfs", "rb");

    INODE *inode_data = (INODE *)malloc(sizeof(INODE));

    int num = rear_dir_list_ptr->inode; //���� ���丮�� inode ��ȣ�� ������ ����
    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + sizeof(INODE) * (num - 1), SEEK_SET);
    fread(inode_data, sizeof(INODE), 1, myfs);
    int file_num = (inode_data->size / (8 + sizeof(int)));
        

    // ���� ã��
    int dir_inode = 0;

    INODE *f_inode = (INODE *)malloc(sizeof(INODE)); //���� ���丮 ���� ������ inode ������ ������ ����
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

            if(f_inode->type == 0) //���丮�� ���
            {
                printf("%s ��/�� ���丮�Դϴ�.\n", file);

                free(inode_data);
                free(fn);
                free(in);
                free(f_inode);

                return;
            }
            else //�Ϲ� ������ ���
            {
                int number = *in;

                if(number == 0) 
                {
                    printf("%s �� �������� �ʽ��ϴ�.\n", file);

                    return;
                }

                else //�κ� ���
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
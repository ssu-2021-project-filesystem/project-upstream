//�������
#include "user.h"


//�۾� ���丮 ��θ� ������ linked list
DIR_LIST *front_dir_list_ptr = NULL;
DIR_LIST *rear_dir_list_ptr = NULL;


/*
�̸�    : main �Լ�
�ۼ���  : ������
���    : X
�޴°�  : X
���ϰ�  : X
*/
int main(void)
{
    //pwd linked list ����
    front_dir_list_ptr = (DIR_LIST *)malloc(sizeof(DIR_LIST));
    front_dir_list_ptr->inode = 1;
    front_dir_list_ptr->name = "/";
    front_dir_list_ptr->next_ptr = NULL;
    rear_dir_list_ptr = front_dir_list_ptr;

    //��
    shell();

    return 0;
}


/*
�̸�    : �� �Լ�
�ۼ���  : ������
���    : ������Ʈ�� ���� ���ɾ �Է¹޾� ���� �Լ��� ȣ���Ѵ�(�� ��� ����)
�޴°�  : X
���ϰ�  : X
*/
void shell(void)
{
    //���ɾ� ó���� ���� �����͵�
    char **com_sep_ptr = (char **)malloc(sizeof(char *) * COM_SEP_NUM); //4���� �����͸� ����ų �� �ִ� ������ ������

    for(int i = 0; i < COM_SEP_NUM; i++)
    {   
        *(com_sep_ptr + i) = (char *)malloc(COM_SEP_SIZE); //�� �����Ϳ� ���� �޸� �Ҵ�
    }

    //�� ���, ���ɾ� ó��
    while(1)
    {
        //myfs ���� ���翩�� Ȯ��
        int exist = 0; //������ �������� �ʴ� ��� 0, �����ϴ� ��� 1 ����
        FILE *test;
        if((test = fopen("myfs", "rb")) != NULL) //������ �������� �ʴ� ���
        {
            exist = 1;
        }
        fclose(test);

        //�� ���
        if(exist == 0) //myfs ������ �������� �ʴ� ���
        {
            printf("$ ");
        }
        else //myfs ������ �����ϴ� ���
        {
            printf("[");
            mypwd(); //mypwd() �Լ��� ��� ���
            printf(" ]$ ");
        }

        //�����Ͱ� ����Ű�� �޸� ���� �ʱ�ȭ
        for(int i = 0; i < COM_SEP_NUM; i++)
        {   
            for(int j = 0; j < COM_SEP_SIZE; j++)
            {
                *(*(com_sep_ptr + i) + j) = 0;
            }
        }

        //���ɾ� �Է�
        char char_tmp;
        int com_ptr_num = 0; //������ ������ �ε���
        int com_ptr_char_num = 0; //������ ������ ����Ű�� ������ �ε���

        rewind(stdin); //���� ����

        while((char_tmp = getchar()) != '\n')
        {
            if(char_tmp != ' ') //���� ���ڰ� �ƴ� ���
            {
                *(*(com_sep_ptr + com_ptr_num) + com_ptr_char_num) = char_tmp;
            }
            else //���� ������ ���
            {
                *(*(com_sep_ptr + com_ptr_num) + com_ptr_char_num) = 0; //NULL ���� ����
                com_ptr_num++;
                com_ptr_char_num = 0;
                continue;
           }

            com_ptr_char_num++;
        }
        *(*(com_sep_ptr + com_ptr_num) + com_ptr_char_num) = 0;

        //�Է� ������ �ʱ�ȭ, ��ó��
        char **com_tmp_ptr = (char **)malloc(sizeof(char *) * COM_SEP_NUM); //4���� �����͸� ����ų �� �ִ� ������ ������

        for(int i = 0; i < (com_ptr_num + 1); i++)
        {
            *(com_tmp_ptr + i) = *(com_sep_ptr + i);
        }
        for(int i = (com_ptr_num + 1); i < COM_SEP_NUM; i++)
        {
            *(com_tmp_ptr + i) = NULL;
        }

        //���ɾ� �Լ� ȣ��
        if(!strcmp("mymkfs", *(com_tmp_ptr)))
        {
            mymkfs();
        }
        else if((!strcmp("myls", *(com_tmp_ptr))) && (exist == 1))
        {
            //myls �Լ�
        }
        else if((!strcmp("mycat", *(com_tmp_ptr))) && (exist == 1))
        {
            //mycat �Լ�
        }
        else if((!strcmp("myshowfile", *(com_tmp_ptr))) && (exist == 1))
        {
            //myshowfile �Լ�
        }
        else if((!strcmp("mypwd", *(com_tmp_ptr))) && (exist == 1))
        {
            mypwd();
            printf("\n");
        }
        else if((!strcmp("mycd", *(com_tmp_ptr))) && (exist == 1))
        {
            //mycd �Լ�
        }
        else if((!strcmp("mycp", *(com_tmp_ptr))) && (exist == 1))
        {
            //mycp �Լ�
        }
        else if((!strcmp("mycpto", *(com_tmp_ptr))) && (exist == 1))
        {
            //mycpto �Լ�
        }
        else if((!strcmp("mycpfrom", *(com_tmp_ptr))) && (exist == 1))
        {
            //mycpfrom �Լ�
        }
        else if((!strcmp("mymkdir", *(com_tmp_ptr))) && (exist == 1))
        {
            //mymkdir �Լ�
        }
        else if((!strcmp("myrmdir", *(com_tmp_ptr))) && (exist == 1))
        {
            //myrmdir �Լ�
        }
        else if((!strcmp("myrm", *(com_tmp_ptr))) && (exist == 1))
        {
            //myrm �Լ�
        }
        else if((!strcmp("mymv", *(com_tmp_ptr))) && (exist == 1))
        {
            //mymv �Լ�
        }
        else if((!strcmp("mytouch", *(com_tmp_ptr))) && (exist == 1))
        {
            //mytouch �Լ�
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
            //mystate �Լ�
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
            printf("���� �ý����� �������� �ʽ��ϴ�.\n");
        }
        else
        {
            printf("�߸��� ���ɾ��Դϴ�.\n");
        }

        printf("\n");
    }

    return;
}
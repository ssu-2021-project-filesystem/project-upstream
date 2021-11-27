#include "user.h"
#include <time.h>

DIR_LIST* dirfound(char* fdir);
char* prtpwd();
int cntfound();
int acc_inode();
int acc_data();

/*
?´ë¦?    : mycd ?•¨?ˆ˜
?‘?„±?  : ?˜¤ê·œë¹ˆ
ê¸°ëŠ¥    : ?‘?—… ?””? ‰?† ë¦¬ì˜ ê²½ë¡œë¥? ë³?ê²½í•œ?‹¤
ë°›ëŠ”ê°?  : ê²½ë¡œ ë¬¸ì?—´
ë¦¬í„´ê°?  : X
*/
void mycd (char* path)
{
    if(path == NULL)
    {//path ?¸?ê°? ?—†?„?‹œ
        rear_dir_list_ptr == front_dir_list_ptr;//?‘?—… ?””? ‰?„°ë¦¬ë?? ?™ˆ?””? ‰?„°ë¦¬ë¡œ ?„¤? •
    }

    else if (path != NULL)
    {
        int first = 0;//ì²? / ?˜?¼?‚´ê¸?
        DIR_LIST *tmp_ptr;
        char *nm_ptr = strtok(path, "/");
        while (nm_ptr != NULL)//?´ë¦„ì´ NULLê°’ì´ ?•„?‹ˆ?¼ë©?
        {
         if (first == 0)//?‹œ?‘?””? ‰?† ë¦¬ê?? . , .. , /?…‹ ì¤? ?•˜?‚˜  . ??? ..?—?„œ ?‹œ?‘?• ?•Œ?˜ ê²½ìš° ?Œ?‹¨
         {
          if (nm_ptr == ".") //.?—?„œ ?‹œ?‘
          {
          tmp_ptr = rear_dir_list_ptr;
          nm_ptr = strtok(NULL, "/");
          first++;
          }
          else if (nm_ptr == "..")//..?—?„œ ?‹œ?‘
          {
           int i = 0;
           tmp_ptr = front_dir_list_ptr;
           while (i < cntfound()-1){
               tmp_ptr = tmp_ptr -> next_ptr;
           } // tmp_ptr?´ ?˜„?¬ ?””? ‰?„°ë¦? ë¦¬ìŠ¤?Š¸?—?„œ (ë§ˆì??ë§?-1)ë²ˆì§¸ ?””? ‰?† ë¦¬ë?? ê°?ë¦¬í‚¤?„ë¡? ?•¨.
           tmp_ptr = rear_dir_list_ptr;
           nm_ptr = strtok(NULL, "/");
           first++;
          }
         }
         else if (first != 0)
         {
          DIR_LIST* new_dir = malloc(sizeof(DIR_LIST)); 
          new_dir-> name = nm_ptr;
          char* s = strcat("/",nm_ptr);//s = /'?‹¤?Œ ?””? ‰?† ë¦?'
          new_dir-> inode = path_to_inode(strcat(prtpwd(),s));
          tmp_ptr -> next_ptr = new_dir;
          nm_ptr = strtok(NULL, "/");
         }
        }
    }
}

/*
?´ë¦?    : mycpto ?•¨?ˆ˜
?‘?„±?  : ?˜¤ê·œë¹ˆ
ê¸°ëŠ¥    : MY ?ŒŒ?¼?‹œ?Š¤?…œ?— ?ˆ?Š” ?ŒŒ?¼?„ ?˜¸?Š¤?Š¸ ì»´í“¨?„°ë¡? ë³µì‚¬?•œ?‹¤.
ë°›ëŠ”ê°?  : ?ŒŒ?¼ëª? 1,2
ë¦¬í„´ê°?  : ë¦¬í„´ê°?
*/
void mycpto (const char* source_file, const char* dest_file  )
{
    FILE *ofp;
    FILE *myfs;
    int c,d;
    int i = 0;
    int inode = 1;

    char *tmp_file_string_ptr = (char *)malloc(sizeof(char) * 8); //?””? ‰?† ë¦¬ì˜ datablock?—?„œ ì¶”ì¶œ?•œ ?ŒŒ?¼ëª…ì„ ê°?ë¦¬í‚¬ ?¬?¸?„°
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //?””? ‰?† ë¦¬ì˜ datablock?—?„œ ì¶”ì¶œ?•œ inode ë²ˆí˜¸ë¥? ê°?ë¦¬í‚¬ ?¬?¸?„°
    INODE *inode_ptr = (INODE *)malloc(sizeof(INODE));// inode ?¬?¸?„°

    myfs = fopen("myfs", "rb");
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*(path_to_inode(prtpwd())-1)),SEEK_SET);//?˜„?¬ ?””? ‰?† ë¦¬ì˜ ?°?´?„°ë¸”ë¡ ?•?œ¼ë¡? ?¬?¸?„° ?´?™
    fread(inode_ptr, sizeof(INODE), 1, myfs);
    int tmp_datablock = (int)(inode_ptr->dir_1 + 1);
    fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs); // ?¬?¸?„°ë¡? ?ŒŒ?¼ëª? ?™•?¸

    while(!strcmp(tmp_file_string_ptr, source_file))
    {//ì°¾ëŠ” ?ŒŒ?¼ê³? ?™•?¸?•œ ?ŒŒ?¼?˜ ?´ë¦„ì´ ê°™ì?? ?•Š?‹¤ë©?
        if((c = getchar()) != EOF)
        {//myfs?ŒŒ?¼?˜ ? ?™•?¸
            fseek(myfs, -1, SEEK_CUR);
            fseek(myfs,sizeof(int),SEEK_CUR);//?˜„?¬ ?¬?¸?„° ?œ„ì¹˜ë¡œë¶??„° int?˜• ?¬ê¸°ë§Œ?¼ ?´?™
            fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs);
        }
        else
        {
        fprintf(stderr,"?˜¤ë¥? : %s ?ŒŒ?¼?´ ?—†?Šµ?‹ˆ?‹¤.\n", source_file); //?ŒŒ?¼ëª…ì„ ?˜„?¬ ?””? ‰?† ë¦¬ì—?„œ ëª»ì°¾?œ¼ë©? ?˜¤ë¥? ?„?š°ê¸?
        exit(1);
        }
    }
    fread(tmp_inode_ptr, sizeof(int), 1, myfs);
    inode =*tmp_inode_ptr;

    if ((ofp = fopen(dest_file, "wb")) == NULL)
    {
        fprintf(stderr, "?˜¤ë¥? : %s ?ŒŒ?¼?„ ?—´ ?ˆ˜ ?—†?Šµ?‹ˆ?‹¤. \n", source_file);
        exit(1);
    }

    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*((inode_ptr->dir_1)- 1)),SEEK_SET);//?ƒˆë¡œìš´ ?ŒŒ?¼?— ë³µì‚¬
    while(i != DATA_BLOCK_SIZE)
    {
        d = getchar();
        putc(d,ofp);
        i++;
    }
    free(tmp_file_string_ptr);
    free(tmp_inode_ptr);
    free(inode_ptr);
    fclose(ofp);
    fclose(myfs);

    return;
}

/*
?´ë¦?    : mycpfrom ?•¨?ˆ˜
?‘?„±?  : ?˜¤ê·œë¹ˆ
ê¸°ëŠ¥    : ?˜¸?Š¤?Š¸ ì»´í“¨?„°?— ?ˆ?Š” ?ŒŒ?¼?„ MY ?ŒŒ?¼?‹œ?Š¤?…œ?œ¼ë¡? ë³µì‚¬?•œ?‹¤.
ë°›ëŠ”ê°?  : ?ŒŒ?¼ëª? 1,2
ë¦¬í„´ê°?  : ë¦¬í„´ê°?
*/

void mycpfrom (const char* source_file, const char* dest_file )
{
    FILE *ifp;
    FILE *myfs;
    int c,size_F;
    time_t Time;
    struct tm* TimeInfo;
 
    Time = time(NULL);                // ?˜„?¬ ?‹œê°„ì„ ë°›ìŒ
    TimeInfo = localtime(&Time); 

    myfs = fopen("myfs", "wb");
    char *tmp_dir_string_ptr = (char *)malloc(sizeof(char) * 8); //?””? ‰?† ë¦¬ì˜ datablock?—?„œ ì¶”ì¶œ?•œ ?””? ‰?† ë¦¬ëª…?„ ê°?ë¦¬í‚¬ ?¬?¸?„°
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //?””? ‰?† ë¦¬ì˜ datablock?—?„œ ì¶”ì¶œ?•œ inode ë²ˆí˜¸ë¥? ê°?ë¦¬í‚¬ ?¬?¸?„°
    INODE *inode_data_ptr = (INODE *)malloc(sizeof(INODE));
    
    int new_inode = acc_inode();
    int new_data = acc_data();
    ifp = fopen(source_file,"rb");
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*(new_data-1)),SEEK_SET);//?ƒˆë¡œìš´ ?ŒŒ?¼?— ë³µì‚¬
    while ((c = getc(ifp)) != EOF)
    {
      size_F++;
      putchar(c);
    }

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (new_inode - 1)),SEEK_SET);//INODELIST ì±„ìš°ê¸?
    fread(inode_data_ptr, sizeof(INODE), 1, myfs);
    inode_data_ptr -> type = 0;
    inode_data_ptr -> year = TimeInfo ->tm_year+1900;
    inode_data_ptr-> size = size_F;
    inode_data_ptr -> month = TimeInfo ->tm_mon+1;
    inode_data_ptr -> date = TimeInfo ->tm_mday;
    inode_data_ptr -> hour = TimeInfo ->tm_hour;
    inode_data_ptr -> minute = TimeInfo ->tm_min;
    inode_data_ptr -> second = TimeInfo ->tm_sec;
    inode_data_ptr -> dir_1 = new_data;    
    inode_data_ptr -> dir_2 = 0;
    inode_data_ptr -> dir_3 = 0;
    inode_data_ptr -> dir_4 = 0;
    inode_data_ptr -> dir_5 = 0;
    inode_data_ptr -> dir_6 = 0;
    inode_data_ptr -> dir_7 = 0;
    inode_data_ptr -> dir_8 = 0;
    inode_data_ptr -> indir = 0; 
    fwrite(inode_data_ptr,sizeof(INODE),1,myfs);

    

    fclose(ifp);
    fclose(myfs);

    return;
}

/*
?´ë¦?    : mycp ?•¨?ˆ˜
?‘?„±?  : ?˜¤ê·œë¹ˆ
ê¸°ëŠ¥    : MY ?ŒŒ?¼?‹œ?Š¤?…œ?— ?ˆ?Š” ?ŒŒ?¼?„ ë³µì‚¬?•˜?Š” ëª…ë ¹?–´
ë°›ëŠ”ê°?  : ?ŒŒ?¼ëª? 1,2
ë¦¬í„´ê°?  : ë¦¬í„´ê°?
*/

void mycp(const char* source_file, const char* dest_file  )
{
    FILE *myfs;
    int c,size_F;;
    int i = 0;
    int inode = 1;
    char* d;
    time_t Time;
    struct tm* TimeInfo;
 
    Time = time(NULL);                // ?˜„?¬ ?‹œê°„ì„ ë°›ìŒ
    TimeInfo = localtime(&Time); 

    char *tmp_file_string_ptr; //?””? ‰?† ë¦¬ì˜ datablock?—?„œ ì¶”ì¶œ?•œ ?ŒŒ?¼ëª…ì„ ê°?ë¦¬í‚¬ ?¬?¸?„°
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //?””? ‰?† ë¦¬ì˜ datablock?—?„œ ì¶”ì¶œ?•œ inode ë²ˆí˜¸ë¥? ê°?ë¦¬í‚¬ ?¬?¸?„°
    INODE *inode_data_ptr = (INODE *)malloc(sizeof(INODE));// inode ?¬?¸?„°
    char *tmp_data_string; //?””? ‰?† ë¦¬ì˜ datablock?—?„œ ì¶”ì¶œ?•œ ?°?´?„°ë¥? ê°?ë¦¬í‚¬ ?¬?¸?„°

    myfs = fopen("myfs", "rb");
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*(path_to_inode(prtpwd())-1)),SEEK_SET);//?˜„?¬ ?””? ‰?† ë¦¬ì˜ ?°?´?„°ë¸”ë¡ ?•?œ¼ë¡? ?¬?¸?„° ?´?™
    fread(inode_data_ptr, sizeof(INODE), 1, myfs);
    int tmp_datablock = (int)(inode_data_ptr->dir_1 + 1);
    fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs); // ?¬?¸?„°ë¡? ?ŒŒ?¼ëª? ?™•?¸

    while(!strcmp(tmp_file_string_ptr, source_file))
    {//ì°¾ëŠ” ?ŒŒ?¼ê³? ?™•?¸?•œ ?ŒŒ?¼?˜ ?´ë¦„ì´ ê°™ì?? ?•Š?‹¤ë©?
        if((c = getchar()) != EOF)
        {//myfs?ŒŒ?¼?˜ ? ?™•?¸
            fseek(myfs, -1, SEEK_CUR);
            fseek(myfs,sizeof(int),SEEK_CUR);//?˜„?¬ ?¬?¸?„° ?œ„ì¹˜ë¡œë¶??„° int?˜• ?¬ê¸°ë§Œ?¼ ?´?™
            fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs);
        }
        else
        {
        fprintf(stderr,"?˜¤ë¥? : %s ?ŒŒ?¼?´ ?—†?Šµ?‹ˆ?‹¤.\n", source_file); //?ŒŒ?¼ëª…ì„ ?˜„?¬ ?””? ‰?† ë¦¬ì—?„œ ëª»ì°¾?œ¼ë©? ?˜¤ë¥? ?„?š°ê¸?
        exit(1);
        }
    }
    fread(tmp_inode_ptr, sizeof(int), 1, myfs);
    inode =*tmp_inode_ptr;

    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*((inode_data_ptr->dir_1)- 1)),SEEK_SET);//ë¬¸ì?—´ ?¬?¸?„°?— ë³µì‚¬?•´?†“ê¸?
    while(i != DATA_BLOCK_SIZE)
    {
        d = getchar();
        strcat(tmp_data_string,d);
        i++;
    }

    fclose(myfs);

    int new_inode = acc_inode();
    int new_data = acc_data();
    
    myfs = fopen("myfs","wb");
    rewind(myfs);
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*(new_data-1)),SEEK_SET);//?ƒˆë¡œìš´ ?ŒŒ?¼?— ë³µì‚¬
    int k = 0;
    int j = sizeof(tmp_data_string);
    while (k < j);
    {
      size_F++;
      putchar(c);
      k++;
    }

    fseek(myfs, BOOT_BLOCK_SIZE + SUPER_BLOCK_SIZE + (sizeof(INODE) * (new_inode - 1)),SEEK_SET);//INODELIST ì±„ìš°ê¸?
    fread(inode_data_ptr, sizeof(INODE), 1, myfs);
    inode_data_ptr -> type = 0;
    inode_data_ptr -> year = TimeInfo ->tm_year+1900;
    inode_data_ptr-> size = size_F;
    inode_data_ptr -> month = TimeInfo ->tm_mon+1;
    inode_data_ptr -> date = TimeInfo ->tm_mday;
    inode_data_ptr -> hour = TimeInfo ->tm_hour;
    inode_data_ptr -> minute = TimeInfo ->tm_min;
    inode_data_ptr -> second = TimeInfo ->tm_sec;
    inode_data_ptr -> dir_1 = new_data;    
    inode_data_ptr -> dir_2 = 0;
    inode_data_ptr -> dir_3 = 0;
    inode_data_ptr -> dir_4 = 0;
    inode_data_ptr -> dir_5 = 0;
    inode_data_ptr -> dir_6 = 0;
    inode_data_ptr -> dir_7 = 0;
    inode_data_ptr -> dir_8 = 0;
    inode_data_ptr -> indir = 0; 
    fwrite(inode_data_ptr,sizeof(INODE),1,myfs);

    
    free(tmp_file_string_ptr);
    free(tmp_inode_ptr);
    fclose(myfs);

    return;
}

/*
?´ë¦?    : myrm ?•¨?ˆ˜
?‘?„±?  : ?˜¤ê·œë¹ˆ
ê¸°ëŠ¥    : ?ŒŒ?¼?„ ?‚­? œ?•˜?Š” ëª…ë ¹?–´
ë°›ëŠ”ê°?  : ?ŒŒ?¼ëª?
ë¦¬í„´ê°?  : ë¦¬í„´ê°?
*/

void myrm(const char* file)
{
    FILE *myfs;
    int c,i=0;
    int inode = 1;

    char *tmp_file_string_ptr = (char *)malloc(sizeof(char) * 8); //?””? ‰?† ë¦¬ì˜ datablock?—?„œ ì¶”ì¶œ?•œ ?ŒŒ?¼ëª…ì„ ê°?ë¦¬í‚¬ ?¬?¸?„°
    int *tmp_inode_ptr = (int *)malloc(sizeof(int)); //?””? ‰?† ë¦¬ì˜ datablock?—?„œ ì¶”ì¶œ?•œ inode ë²ˆí˜¸ë¥? ê°?ë¦¬í‚¬ ?¬?¸?„°
    INODE *inode_ptr = (INODE *)malloc(sizeof(INODE));// inode ?¬?¸?„°

    myfs = fopen("myfs", "rb");
    if (myfs == NULL)
    {
        printf("myrm() ?•¨?ˆ˜ : ?ŒŒ?¼ ?—´ê¸°ì— ?‹¤?Œ¨?–ˆ?Šµ?‹ˆ?‹¤.\n");
        abort();
    }
    fseek(myfs, BOOT_BLOCK_SIZE+SUPER_BLOCK_SIZE+(sizeof(INODE)*128)+(DATA_BLOCK_SIZE*((path_to_inode(prtpwd()))-1)),SEEK_SET);//?˜„?¬ ?””? ‰?† ë¦¬ì˜ ?°?´?„°ë¸”ë¡ ?•?œ¼ë¡? ?¬?¸?„° ?´?™
    fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs); // ?¬?¸?„°ë¡? ?ŒŒ?¼ëª? ?™•?¸

    while(!strcmp(tmp_file_string_ptr, file))
    {//ì°¾ëŠ” ?ŒŒ?¼ê³? ?™•?¸?•œ ?ŒŒ?¼?˜ ?´ë¦„ì´ ê°™ì?? ?•Š?‹¤ë©?
        if((c = getchar()) != EOF)
        {//myfs?ŒŒ?¼?˜ ? ?™•?¸
            fseek(myfs, -1, SEEK_CUR);
            fseek(myfs,sizeof(int),SEEK_CUR);//?˜„?¬ ?¬?¸?„° ?œ„ì¹˜ë¡œë¶??„° int?˜• ?¬ê¸°ë§Œ?¼ ?´?™
            fread(tmp_file_string_ptr, sizeof(char) * 8, 1, myfs);
        }
        else
        {
        fprintf(stderr,"?˜¤ë¥? : %s ?ŒŒ?¼?´ ?—†?Šµ?‹ˆ?‹¤.\n", file); //?ŒŒ?¼ëª…ì„ ?˜„?¬ ?””? ‰?† ë¦¬ì—?„œ ëª»ì°¾?œ¼ë©? ?˜¤ë¥? ?„?š°ê¸?
        exit(1);
        }
    }
    fread(tmp_inode_ptr, sizeof(int), 1, myfs);
    inode =*tmp_inode_ptr;
    fclose(myfs);

    myfs = fopen("myfs", "wb");
    rewind(myfs);
    fseek(myfs, BOOT_BLOCK_SIZE + (inode- 1) ,SEEK_SET);//INODELIST 
    putchar(0);
    fseek(myfs, BOOT_BLOCK_SIZE + 128 + ((inode_ptr -> dir_1) - 1),SEEK_SET);//DATABLOCK
    putchar(0);
    fseek(myfs, BOOT_BLOCK_SIZE + (sizeof(INODE)*128)+(DATA_BLOCK_SIZE)*((inode_ptr -> dir_1) - 1) ,SEEK_SET);
    while(i != DATA_BLOCK_SIZE)
    {
        putchar(0);
        i++;
    }
}

/*
?´ë¦?    : cntfound ?•¨?ˆ˜
?‘?„±?  : ?˜¤ê·œë¹ˆ
ê¸°ëŠ¥    : dir_list?—?„œ ?˜„?¬ ?´?–´ì§? ?””? ‰?† ë¦? ?ˆ˜ ë¦¬í„´
ë°›ëŠ”ê°?  : X
ë¦¬í„´ê°?  : countê°?
*/

int cntfound() 
{
    int cnt = 0;
    DIR_LIST* tmp_dir = front_dir_list_ptr;
    while ((tmp_dir->next_ptr) != NULL)
    {
        {
            cnt++;
            tmp_dir = tmp_dir->next_ptr;
        }
    }
    return cnt;//?ƒ?ƒ‰ ?‹¤?Œ¨?‹œ

}

/*
?´ë¦?    : prtpwd ?•¨?ˆ˜
?‘?„±?  : ?˜¤ê·œë¹ˆ
ê¸°ëŠ¥    : pwdë¥? ë¬¸ì?—´ë¡? ë¦¬í„´
ë°›ëŠ”ê°?  : X
ë¦¬í„´ê°?  : pwd ë¬¸ì?—´ê°?
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
?´ë¦?    : acc_inode ?•¨?ˆ˜
?‘?„±?  : ?˜¤ê·œë¹ˆ
ê¸°ëŠ¥    : ê°??š©ì¤‘ì´ì§? ?•Š??? inode ë²ˆí˜¸ë¥? ê°?? ¸?˜¤ê³?, ê·? inodeë²ˆí˜¸?— ?•´?‹¹?•˜?Š” ?Šˆ?¼ë¸”ë¡?„ 1ë¡? ì±„ìš´?‹¤.
ë°›ëŠ”ê°?  : X
ë¦¬í„´ê°?  : ê°??š©ì¤‘ì´ì§? ?•Š??? inode ë²ˆí˜¸
*/

int acc_inode()
{
    FILE* myfs;
    int t = 1;

    myfs = fopen("myfs", "rb");
    if (myfs == NULL)
    {
        printf("acc_inode() ?•¨?ˆ˜ : ?ŒŒ?¼ ?—´ê¸°ì— ?‹¤?Œ¨?–ˆ?Šµ?‹ˆ?‹¤.\n");
        abort();
    }

    SUPERBLOCK *sb_ptr = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
    fseek(myfs, BOOT_BLOCK_SIZE, SEEK_SET);
    fread(sb_ptr, sizeof(SUPERBLOCK), 1, myfs);//?‚¬?š©ì¤‘ì¸ ?Šˆ?¼ë¸”ë¡ ?™•?¸?•˜?—¬ ê°??š©ì¤‘ì´ì§? ?•Š??? ?•„?´?…¸?“œ?— ?ŒŒ?¼ ?„£ê¸? ?œ„?•¨.

    unsigned mask;
    while(1){ // ë¹? inodeê°? ?ˆ?„?•Œ ê¹Œì?? ë¬´í•œ ë°˜ë³µ
    if (t < 33) // inode_1?— ë¹? inodeê°? ?ˆ?„ ê²½ìš°
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->inode_1 & mask) == 0) //ë¹? ?•„?´?…¸?“œê°? ?ˆ?—ˆ?„ ê²½ìš°
        {
            sb_ptr->inode_1 = sb_ptr->inode_1 | 1;//ë¹? ?•„?´?…¸?“œ ì±„ìš°ê¸?
            break;
        }
        t++;
    }
    else if (t < 65) // inode_2?— ë¹? inodeê°? ?ˆ?„ ê²½ìš°
    {
        mask = 1 << ((t- 32) - 1);
        if ((sb_ptr->inode_2 & mask) == 0)
        {
            sb_ptr->inode_2 = sb_ptr->inode_2 | 1;
            break;
        }
        t++;
    }
    else if (t < 97) // inode_3?— ë¹? inodeê°? ?ˆ?„ ê²½ìš°
    {
        mask = 1 << ((t - 64) - 1);
        if ((sb_ptr->inode_3 & mask) == 0)
        {
          sb_ptr->inode_3 = sb_ptr->inode_3 | 1;
          break;
        }
        t++;
    }
    else // inode_4?— ë¹? inodeê°? ?ˆ?„ ê²½ìš°
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
?´ë¦?    : acc_data ?•¨?ˆ˜
?‘?„±?  : ?˜¤ê·œë¹ˆ
ê¸°ëŠ¥    : ê°??š©ì¤‘ì´ì§? ?•Š??? datablock ë²ˆí˜¸ë¥? ê°?? ¸?˜¤ê³?, ê·? inodeë²ˆí˜¸?— ?•´?‹¹?•˜?Š” ?Šˆ?¼ë¸”ë¡?„ 1ë¡? ì±„ìš´?‹¤.
ë°›ëŠ”ê°?  : X
ë¦¬í„´ê°?  : ê°??š©ì¤‘ì´ì§? ?•Š??? datablock ë²ˆí˜¸
*/
int acc_data()
{
    FILE* myfs;
    int t = 1;

    myfs = fopen("myfs", "rb");
    if (myfs == NULL)
    {
        printf("acc_data() ?•¨?ˆ˜ : ?ŒŒ?¼ ?—´ê¸°ì— ?‹¤?Œ¨?–ˆ?Šµ?‹ˆ?‹¤.\n");
        abort();
    }

    SUPERBLOCK *sb_ptr = (SUPERBLOCK *)malloc(sizeof(SUPERBLOCK));
    fseek(myfs, BOOT_BLOCK_SIZE+128, SEEK_SET);
    fread(sb_ptr, sizeof(SUPERBLOCK), 1, myfs);//?‚¬?š©ì¤‘ì¸ ?Šˆ?¼ë¸”ë¡ ?™•?¸?•˜?—¬ ê°??š©ì¤‘ì´ì§? ?•Š??? ?°?´?„°ë¸”ë¡?— ?ŒŒ?¼ ?„£ê¸? ?œ„?•¨.

    unsigned mask;
    while(1)
    { // ë¹? ?°?´?„°ë¸”ë¡?´ ?ˆ?„?•Œ ê¹Œì?? ë¬´í•œ ë°˜ë³µ
    if (t < 33) // data_block_1?— ë¹? data_block?´ ?ˆ?„ ê²½ìš°
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_1 & mask) == 0) //ë¹? ?Šˆ?¼ë¸”ë¡?´ ?ˆ?—ˆ?„ ê²½ìš°
        {
            sb_ptr->data_block_1 = sb_ptr->data_block_1 | 1;//ë¹? ?Šˆ?¼ë¸”ë¡ ì±„ìš°ê¸?
            break;
        }
        t++;
    }
    else if (t < 65) // data_block_2?— ë¹? data_block?´ ?ˆ?„ ê²½ìš°
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_2 & mask) == 0) //ë¹? ?Šˆ?¼ë¸”ë¡?´ ?ˆ?—ˆ?„ ê²½ìš°
        {
            sb_ptr->data_block_2 = sb_ptr->data_block_2 | 1;//ë¹? ?Šˆ?¼ë¸”ë¡ ì±„ìš°ê¸?
            break;
        }
        t++;
    }
    else if (t < 97) // data_block_3?— ë¹? data_block?´ ?ˆ?„ ê²½ìš°
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_3 & mask) == 0) //ë¹? ?Šˆ?¼ë¸”ë¡?´ ?ˆ?—ˆ?„ ê²½ìš°
        {
            sb_ptr->data_block_3 = sb_ptr->data_block_3 | 1;//ë¹? ?Šˆ?¼ë¸”ë¡ ì±„ìš°ê¸?
            break;
        }
        t++;
    }
    else if (t < 129) // data_block_4?— ë¹? data_block?´ ?ˆ?„ ê²½ìš°
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_4 & mask) == 0) //ë¹? ?Šˆ?¼ë¸”ë¡?´ ?ˆ?—ˆ?„ ê²½ìš°
        {
            sb_ptr->data_block_4 = sb_ptr->data_block_4 | 1;//ë¹? ?Šˆ?¼ë¸”ë¡ ì±„ìš°ê¸?
            break;
        }
        t++;
    }
    else if (t < 161) // data_block_5?— ë¹? data_block?´ ?ˆ?„ ê²½ìš°
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_5 & mask) == 0) //ë¹? ?Šˆ?¼ë¸”ë¡?´ ?ˆ?—ˆ?„ ê²½ìš°
        {
            sb_ptr->data_block_5 = sb_ptr->data_block_5 | 1;//ë¹? ?Šˆ?¼ë¸”ë¡ ì±„ìš°ê¸?
            break;
        }
        t++;
    }
    else if (t < 193) // data_block_6?— ë¹? data_block?´ ?ˆ?„ ê²½ìš°
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_6 & mask) == 0) //ë¹? ?Šˆ?¼ë¸”ë¡?´ ?ˆ?—ˆ?„ ê²½ìš°
        {
            sb_ptr->data_block_6 = sb_ptr->data_block_6 | 1;//ë¹? ?Šˆ?¼ë¸”ë¡ ì±„ìš°ê¸?
            break;
        }
        t++;
    }
    else if (t < 225) // data_block_7?— ë¹? data_block?´ ?ˆ?„ ê²½ìš°
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_7 & mask) == 0) //ë¹? ?Šˆ?¼ë¸”ë¡?´ ?ˆ?—ˆ?„ ê²½ìš°
        {
            sb_ptr->data_block_7 = sb_ptr->data_block_7 | 1;//ë¹? ?Šˆ?¼ë¸”ë¡ ì±„ìš°ê¸?
            break;
        }
        t++;
    }
    else // data_block_3?— ë¹? data_block?´ ?ˆ?„ ê²½ìš°
    {
        mask = 1 << (t - 1);
        if ((sb_ptr->data_block_8 & mask) == 0) //ë¹? ?Šˆ?¼ë¸”ë¡?´ ?ˆ?—ˆ?„ ê²½ìš°
        {
            sb_ptr->data_block_8 = sb_ptr->data_block_8 | 1;//ë¹? ?Šˆ?¼ë¸”ë¡ ì±„ìš°ê¸?
            break;
        }
        t++;
    }
    }
    return t;
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
    char name[32];
    char phone[20];
    int age;
}Contact;

typedef struct ContactBook{
    Contact *data;
    struct ContactBook *next;
}ContactBook;

ContactBook *book=NULL;
ContactBook *Head=NULL;

int size=0;


void delete(char *username) {
    //删除一个元素
    ContactBook * target = Head->next;
    ContactBook * last = Head;
    

    while ( target != NULL ){
        if ( strcmp(target->data->name,username)==0 ) {
            break;
        }
        last = target;
        target = target->next;
    }

    if ( target == NULL )
        fprintf(stderr, "Not Find! Can't delete!\n");
    else {
        last->next = target->next;
        if ( target == book ) {
            book = last;
        }
        size--;
        free(target->data);
        free(target);
    }
}

void find (char *username) {
    //查找一个元素
    ContactBook * target = Head->next;

    while ( target != NULL ){
        if ( strcmp(target->data->name,username)==0 ) {
            break;
        }
        target = target->next;
    }

    if ( target == NULL )
        fprintf(stderr,"Not Find!\n");
    else {
        printf( "FIND!\n");
        printf( "name:%s\n", target->data->name );
        printf( "phone:%s\n", target->data->phone );
        printf( "age:%d\n", target->data->age );
    }
}

void list(){
    //列出所有元素
    printf("summary:%d\n", size );
    
    ContactBook * target = Head->next;
    int num=0;

    while ( target!=NULL) {
        num++;
        printf( "No. %d\n", num );
        printf( "   name:%s\n", target->data->name );
        printf( "   phone:%s\n", target->data->phone );
        printf( "   age:%d\n", target->data->age );
        target = target->next;
    }
}




int split_line(char line[], char *arr[]) {
    int count = 0;

    // 按空格、制表符、换行分割
    char *token = strtok(line, " \t\n");

    while ( token != NULL ) {
        arr[count++] = token;
        token = strtok(NULL, " \t\n");
    }
    return count;
}

void increase(char *username, char *userphone, int userage) {
    //增加一个新元素
    //防御性编程
    if (book == NULL) {
        fprintf(stderr, "book is NULL, cannot append\n");
        return;
    }
    ContactBook * last = book;
    book = (ContactBook *)malloc(sizeof(ContactBook));

    if (book == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    book->data = (Contact*)malloc(sizeof(Contact));

    if (book->data == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return;
    }

    last->next = book;
    book->next = NULL;

    size++;
    if ( username !=NULL ){
        strncpy(book->data->name, username, sizeof(book->data->name) - 1);
        book->data->name[sizeof(book->data->name) - 1] = '\0';
    }
        
    if ( userphone != NULL ){
        strncpy(book->data->phone, userphone, sizeof(book->data->phone) - 1);
        book->data->phone[sizeof(book->data->phone) - 1] = '\0';
    }
        
    ( book->data ) ->age = userage;
    
}

void end_program() {
    //释放所有内存
    ContactBook * target = Head->next;
    ContactBook * last = Head;

    while ( target != NULL ) {
        last=target;
        target=target->next;
        if (last != NULL ) free ( last->data );
        free(last);
    }
    free(Head);
}

void change_phone (char *username,char *userphone_new) {
    //改变用户手机号
    ContactBook * target = Head->next;

    while ( target != NULL ){
        if ( strcmp(target->data->name,username)==0 ) {
            break;
        }
        target = target->next;
    }

    if ( target == NULL )
        fprintf(stderr,"Not Find!\n");
    else {
        strncpy(target->data->phone, userphone_new, sizeof(target->data->phone) - 1);
        target->data->phone[sizeof(target->data->phone) - 1] = '\0'; 
    }
}

void change_age (char *username,int userage_new) {
    //改变用户年龄
    ContactBook * target = Head->next;

    while ( target != NULL ){
        if ( strcmp(target->data->name,username)==0 ) {
            break;
        }
        target = target->next;
    }

    if ( target == NULL )
        fprintf(stderr,"Not Find!\n");
    else {
        target->data->age=userage_new;
    }
}

int judge(char **arr, int count_readline) {
    switch (arr[0][0]) {
            case '+':
                if (count_readline < 4) {
                    fprintf(stderr, "Insufficient arguments for +\n");
                    break;
                }
                increase(arr[1], arr[2], atoi(arr[3]));
                break;

            case '-':
                delete(arr[1]);
                break;


            case 'f':
                find(arr[1]);
                break;


            case 'l':
                list();
                break;


            case 'c':
                if (count_readline < 4) {
                    fprintf(stderr, "Insufficient arguments for c\n");
                    break;
                }
                if (strcmp(arr[1], "phone") == 0) {
                    change_phone(arr[2], arr[3]);  
                }
                else if (strcmp(arr[1], "age") == 0) {
                    change_age(arr[2], atoi(arr[3]));
                }
                else {
                    fprintf(stderr, "Can't change this thing!\n");
                }
                break;
            
            case 'e':
                end_program();
                printf("end.");
                return 1;

            default :{
                fprintf(stderr,"not find this command!\n");
                break;

            }
    }
    return 0;
}

int main() {
    int end_program_1=0;
    FILE * fp_read = fopen("./doc/read.txt","r");
    if ( fp_read == NULL ) {
        fprintf(stderr,"Error opening file");
        return 1;
    }
    book = (ContactBook *)malloc(sizeof(ContactBook));
    if (book == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        return 1;
    }
    Head = book;
    book->data = NULL;
    book->next = NULL;

    char line[120];
    char *arr[5];
    while ( fgets(line, sizeof(line), fp_read) ) {
        int count_readline;
        count_readline=split_line(line,arr);
        if (count_readline == 0) {
            continue;
        }
        end_program_1=judge(arr,count_readline);
        if ( end_program_1 == 1 ) break;       
        }
    fclose(fp_read);
    return 0;
}

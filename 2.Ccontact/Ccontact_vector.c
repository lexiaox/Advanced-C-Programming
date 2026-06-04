#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[32];
    char phone[20];
    int age;
} Contact;

typedef struct {
    Contact *data;
    size_t size;
    size_t capacity;
} ContactBook;

ContactBook book;

int split_line(char line[], char *arr[]);
int judge(char **arr, int count_readline, FILE *fp_read_Address_book, FILE *fp_read_command);
void increase(char *name, char *phone, int age);
void delete(char *username);
void find (char *username);
void list();
void change_phone (char *username,char *userphone_new);
void change_age (char *username,int userage_new);
void end_program(FILE *fp_read_Address_book, FILE *fp_read_command);

void read_Address_book(FILE *fp, ContactBook *book) {
    char line[120];
    fgets(line, sizeof(line), fp); // 读取表头
    while (fgets(line, sizeof(line), fp)) {
        if (book->size >= book->capacity) {
            book->capacity *= 2;
            Contact *new_data = realloc(book->data, book->capacity * sizeof(Contact));
            if (new_data == NULL) {
                fprintf(stderr,"Error reallocating memory");
                free(book->data);
                return;
            }
            book->data = new_data;
        }
        sscanf(line, "%31s |%19s |%d", book->data[book->size].name, book->data[book->size].phone, &book->data[book->size].age);
        book->size++;
    }
}

void read_command(FILE *fp, ContactBook *book, FILE *fp_read_Address_book, FILE *fp_read_command) {
    (void)book;
    char command[100];
    while (fgets(command, sizeof(command), fp)) {
        // Process the command
        char *arr[100];
        int count_readline = split_line(command, arr);
        if (count_readline > 4) {
            fprintf(stderr, "Too many arguments for command: %s", command);
            continue;
        }
        if (count_readline ==0 ) continue; // Skip empty lines
        judge(arr, count_readline, fp_read_Address_book, fp_read_command);
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

void save(void) {
    //将通讯录保存到文件
    FILE * fp_write = fopen("./doc/Address_book.txt","w");
    if ( fp_write == NULL ) {
        fprintf(stderr,"Error opening file");
        return;
    }
    fprintf(fp_write, "name   |phone    |age\n");
    for (size_t i = 0; i < book.size; i++) {
        fprintf(fp_write, "%s   |%s    |%d\n", book.data[i].name, book.data[i].phone, book.data[i].age);
    }
    fclose(fp_write);
}

int judge(char **arr, int count_readline, FILE *fp_read_Address_book, FILE *fp_read_command) {
    //判断命令并执行
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
                end_program(fp_read_Address_book, fp_read_command);
                return 1;
                

            default :{
                fprintf(stderr,"not find this command!\n");
                break;

            }
    }
    return 0;
}

void increase(char *name, char *phone, int age) {
    //增加一个元素
    if (book.size >= book.capacity) {
        book.capacity *= 2;
        Contact *new_data = realloc(book.data, book.capacity * sizeof(Contact));
        if (new_data == NULL) {
            fprintf(stderr,"Error reallocating memory");
            free(book.data);
            return;
        }
        book.data = new_data;
    }
    strncpy(book.data[book.size].name, name, sizeof(book.data[book.size].name) - 1);
    strncpy(book.data[book.size].phone, phone, sizeof(book.data[book.size].phone) - 1);
    book.data[book.size].age = age;
    book.size++;
}

void delete(char *username) {
    //删除一个元素
    int found = 0;
    for (size_t i = 0; i < book.size; i++) {
        if (strcmp(book.data[i].name, username) == 0) {
            found = 1;
            // 将最后一个元素移动到当前位置
            book.data[i] = book.data[book.size - 1];
            book.size--;
            break;
        }
    }
    if (!found) {
        fprintf(stderr, "Not Find! Can't delete!\n");
    }
}

void find (char *username) {
    //查找一个元素
    int found = 0;
    for (size_t i = 0; i < book.size; i++) {
        if (strcmp(book.data[i].name, username) == 0) {
            found = 1;
            printf( "FIND!\n");
            printf( "name:%s\n", book.data[i].name );
            printf( "phone:%s\n", book.data[i].phone );
            printf( "age:%d\n", book.data[i].age );
            break;
        }
    }
    if (!found) {
        fprintf(stderr,"Not Find!\n");
    }
}

void list() {
    //列出所有元素
    printf("name   |phone    |age\n");
    for (size_t i = 0; i < book.size; i++) {
        printf("%s   |%s    |%d\n", book.data[i].name, book.data[i].phone, book.data[i].age);
    }
}

void change_phone (char *username,char *userphone_new) {
    //改变用户手机号
    int found = 0;
    for (size_t i = 0; i < book.size; i++) {
        if (strcmp(book.data[i].name, username) == 0) {
            found = 1;
            strncpy(book.data[i].phone, userphone_new, sizeof(book.data[i].phone) - 1);
            book.data[i].phone[sizeof(book.data[i].phone) - 1] = '\0'; 
            break;
        }
    }
    if (!found) {
        fprintf(stderr,"Not Find!\n");
    }
}

void change_age (char *username,int userage_new) {
    //改变用户年龄
    int found = 0;
    for (size_t i = 0; i < book.size; i++) {
        if (strcmp(book.data[i].name, username) == 0) {
            found = 1;
            book.data[i].age = userage_new;
            break;
        }
    }
    if (!found) {
        fprintf(stderr,"Not Find!\n");
    }
}


void end_program(FILE *fp_read_Address_book, FILE *fp_read_command) {
    //结束程序
    save();
    free(book.data);
    if (fp_read_Address_book != NULL) {
        fclose(fp_read_Address_book);
    }
    if (fp_read_command != NULL) {
        fclose(fp_read_command);
    }
    printf("end.");
    exit(0);
}

int main() {
    FILE * fp_read_Address_book = fopen("./doc/Address_book.txt","r");
    if ( fp_read_Address_book == NULL ) {
        fprintf(stderr,"Address_book.txt not found, start from an empty book\n");
    }
    book.size = 0;
    book.capacity = 10; // 初始容量
    book.data = malloc(book.capacity * sizeof(Contact));
    if (book.data == NULL) {
        fprintf(stderr,"Error allocating memory");
        return 1;
    }
    if (fp_read_Address_book != NULL) {
        read_Address_book(fp_read_Address_book, &book);
    }

    FILE * fp_read_command = fopen("./doc/command.txt","r");
    if ( fp_read_command == NULL ) {
        fprintf(stderr,"Error opening file");
        free(book.data);
        return 1;
    }
    read_command(fp_read_command, &book, fp_read_Address_book, fp_read_command);
    /* ensure resources are saved/closed when commands finish */
    end_program(fp_read_Address_book, fp_read_command);
    return 0;
}

#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<dirent.h>
#include<unistd.h>
#include<sys/stat.h>
struct node{
	char*  symbol;
	struct node*  next;
};
int exists(struct node* node_ptr,char* symbol);
void  list(char* dir_name,struct node** start);
void cut(const char* s,struct node* o);
void insert_list(struct node** beg,char* symbol);
void free_list(struct node** node_ptr);
int num(char* symbol);
void delete_folder(char* symbol);
void check_for_delete(char* symbol);
void delete_symbol(struct node** start,char* symbol);
int file_exist (char *filename);
int size_of_file(char* filename);
char* file_in_text(int size,char* filename,char* input);
void make_string(char* sub_string,char* text,int buffersize);
void create_path(char* path);
void delete_all(char* symbol);
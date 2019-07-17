#include<string.h>
#include<stdlib.h>
#include<stdio.h>
#include<dirent.h>
#include<errno.h>
#include<unistd.h>
#include <sys/stat.h>
#ifndef __struct_h_
#define __struct_h_

#include "struct.h"
#endif
int exists(struct node* node_ptr,char* symbol){/*Checking if the directory exists*/
	/* symbol is the path of directory*/
	int found=0;
	while(node_ptr!=NULL){
		if(!strcmp(node_ptr->symbol,symbol)){
			found=1;
			break;
		}
		node_ptr=node_ptr->next;
	}
	if(found==1)
		return 1;
	else
		return 0;
}
void  list(char* dir_name,struct node** start){/*Creation of list of all folders of user*/
	DIR* dir_ptr;
	dir_ptr=opendir(dir_name);
	char t[50];
	strcpy(t,dir_name);
	if(dir_ptr==NULL)
		perror("opendir");
	struct dirent *p;
	while ((p= readdir(dir_ptr)) != NULL) {
    	if(!strcmp(p->d_name,"..")) continue;
    	if(!strcmp(p->d_name,".")) continue;
    	if(strstr(p->d_name, ".txt") != NULL){
    		struct node* node=malloc(sizeof(struct node));
    		char e[50];
    		strcpy(e,t);
    		sprintf(e+strlen(e),"/%s",p->d_name);
    		node->symbol=malloc((strlen(e)+1)*sizeof(char));
    		strcpy(node->symbol,e);
    		node->next=NULL;
    		if(*start==NULL)
    			*start=node;
    		else{
    			struct node* i=*start;
    			while(1){
    				if(i->next==NULL){
    					i->next=node;
    					break;
    				}
    				i=i->next;
    			}
    		}
    	}
    	if(p->d_type == DT_DIR){
    		char *subpath = malloc(strlen(dir_name)+strlen(p->d_name)+1);
				strcpy(subpath,dir_name);
				sprintf(subpath+strlen(subpath),"%s","/");
				strcat(subpath,p->d_name);
    		if(*start==NULL)
    			list(subpath,&(*start));
    		else{
    			struct node* e=*start;
	    		struct node* n;
		    	while(1){
		    		if(e->next==NULL){
		    			n=e;
		    			break;
		    		}
		    		e=e->next;
		    	}
		    	list(subpath,&n);
	    	}
			free(subpath);
    	}
   }
   closedir(dir_ptr);
}
void cut(const char* s,struct node* o){
	while(1){
		int start=0;
		int done=0;
		for(int i=0;i<strlen(o->symbol);i++){
			if(o->symbol[i]=='/')
				done=1;
			if(done==1){
				o->symbol[start]=o->symbol[i+1];
				start++;
			}
		}
		if(o->next==NULL)
			break;
		o=o->next;
	}
}
void insert_list(struct node** beg,char* symbol){/*Insert node in list*/
	struct node* node=malloc(sizeof(struct node));
	node->next=NULL;
	node->symbol=malloc((strlen(symbol)+1)*sizeof(char));
	strcpy(node->symbol,symbol);
	if(*beg==NULL)
	  *beg=node;
	else{
	  struct node* y=(*beg);
	  while(1){
	    if(y->next==NULL){
		  	y->next=node;
		  	break;
			}
			y=y->next;
	  }
	}
}
void free_list(struct node** node_ptr){/*Delete all nodes of list*/
	if(*node_ptr==NULL)
		return ;
	struct node* start=*node_ptr;
	struct node* i=(*node_ptr)->next;
	while(1){
		free(start->symbol);
		free(start);
		start=i;
		if(start==NULL)
			break;
		i=i->next;
	}
}
int num(char* symbol){/*Convert char* to int number*/
	int num=0;
	int c;
	for(int i=0;i<strlen(symbol);i++){
		c=symbol[i];
		if(c=='.')
			break;
		num=num*10+ c-'0';
	}
	return num;
}
void delete_folder(char* symbol){
	DIR* dir_ptr;
	struct dirent *dir;
	dir_ptr=opendir(symbol);
	char* destination=malloc((strlen(symbol)+1)*sizeof(char));
	strcpy(destination,symbol);
	while ((dir = readdir(dir_ptr)) != NULL){
		if(!strcmp (dir->d_name, ".")) continue;
    if(!strcmp (dir->d_name, "..")) continue;
    destination=realloc(destination,(strlen(symbol)+strlen(dir->d_name)+2)*sizeof(char));
    sprintf(destination+strlen(symbol),"/%s",dir->d_name);
		remove(destination);
	}
	free(destination);
	closedir(dir_ptr);
	rmdir(symbol);
}
void check_for_delete(char* symbol){/*Check if a folder is empty in order to delete all its content*/
	DIR* dir_ptr;
	struct dirent* dir;
	dir_ptr=opendir(symbol);
	int counter=0;
	while ((dir = readdir(dir_ptr)) != NULL){
		if(!strcmp (dir->d_name, ".")) continue;
    if(!strcmp (dir->d_name, "..")) continue;
    if(strstr(dir->d_name,".fifo")!=NULL) continue;
      counter++;
	}
	closedir(dir_ptr);
	if(counter==0)
		delete_folder(symbol);
}
void delete_symbol(struct node** start,char* symbol){/*Delete a node from list*/
	struct node* a=*start;
	struct node* b=a->next;
	if(b==NULL){
		if(!strcmp(a->symbol,symbol)){
			free(a);
			*start=NULL;
			return ;
		}
	}
	else{
		while(1){
			if(!strcmp(a->symbol,symbol)){
				struct node* f=a;
				a=a->next;
				free(f);
				return ;
			}
			if(!strcmp(b->symbol,symbol)){
				struct node* k=b;
				a->next=b->next;
				free(k);
				return ;
			}
			a=b;
			b=b->next;
		}
	}
}
int file_exist (char *filename)/*Check if a file exists*/
{
  struct stat   buffer;   
  return (stat (filename, &buffer) == 0);
}
int size_of_file(char* filename){
	FILE* file=fopen(filename,"r");
	int c;
	int counter=-1;
	while(1){
		c=fgetc(file);
		if(c==EOF)
			break;
		counter++;
	}
	fclose(file);
	return counter;/*Count how many characters have the file*/
}
char* file_in_text(int size,char* filename,char* input){/*Opening a file and return to text all its content of character*/
	char* path=malloc((strlen(filename)+strlen(input)+2)*sizeof(char));
	sprintf(path,"%s/",input);
	sprintf(path+strlen(path),"%s",filename);
	char * text=malloc(size*sizeof(char));
	FILE* file=fopen(path,"r");
	int counter=0;
	int c;
	while(1){
		c=fgetc(file);
		if(c==EOF){
			text[counter]='\0';
			break;
		}
		if(c=='\n')
			continue;
		text[counter++]=c;
	}
	return text;
}
void make_string(char* sub_string,char* text,int buffersize){/*Copy buffersize n charactes from text to sub_string*/
	int size=strlen(text);
	for(int i=0;i<buffersize;i++)
		sub_string[i]=text[i];
	sub_string[buffersize]='\0';
}
void create_path(char* path){/*Creation of Path of specific file*/
	struct stat st={0};
	char folder[PATH_MAX];
	for(int i=0;i<PATH_MAX;i++)
		folder[i]='\0';
	for(int i=0;i<strlen(path);i++){
		folder[i]=path[i];
		if(strstr(folder,".txt")!=NULL)
			return;
		if(folder[i]=='/'){
			if(stat(folder,&st)!=0)
				mkdir(folder,0700);/*Creation of Folder*/
		}
	}
}
void delete_all(char* symbol){/* Delete all content of specific Directrory*/
	DIR* d=opendir(symbol);/*Open directory*/
	struct dirent *dir;
	while ((dir = readdir(d)) != NULL){
		if(!strcmp (dir->d_name, ".")) continue;
    if(!strcmp (dir->d_name, "..")) continue;
		if(dir->d_type == DT_DIR){/*Inside of another directory */
			char destination[PATH_MAX];
			sprintf(destination,"%s/",symbol);
			sprintf(destination+strlen(destination),"%s",(dir->d_name));
			delete_all(destination);/*Recursive Method*/
		}
		if(strstr(dir->d_name,".txt")!=NULL){
			char destination[PATH_MAX];
			sprintf(destination,"%s/",symbol);
			sprintf(destination+strlen(destination),"%s",dir->d_name);
			remove(destination);/*Delete File*/
		}
	}
	closedir(d);/*Close directory*/
	rmdir(symbol);/*Remove directory*/
}
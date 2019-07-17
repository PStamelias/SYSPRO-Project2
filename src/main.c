#include <stdio.h>
#include <dirent.h>
#include <sys/inotify.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <linux/limits.h>
#include <errno.h>
#include <poll.h>
#ifndef __struct_h_
#define __struct_h_

#include "struct.h"
#endif
#define PATH_MAX 4096
struct stat st={0};
volatile sig_atomic_t end=0;/*signal variable*/
volatile sig_atomic_t done=0;/*signal variable*/
volatile sig_atomic_t count=0;/*signal variable*/
volatile sig_atomic_t val=0;/*signal variable*/
void handler(int num){	
	end=1;
}
void sig_handler(int num){
	if(done==0)
		count++;
}
void sig2_handler(int num){
	val=1;
}
int main(int argc,char** argv)
{
	int send_Bytes=0;
	int read_Bytes=0;
	int send_Files=0;
	int read_Files=0;
	int fd1,fd2;
	char* input_dir;
	char* common_dir;
	char* mirror_dir;
	int id,buffersize;
	int text_size;
	DIR* dir_ptr;
	int status;
	unsigned short int file_length;
	char* name_file;
	int text_length;
	char* arxeio;
	char* char_id;
	int parent_id=getpid();
	struct dirent *dir;
	char* logfile;
	if(argc!=13){
		printf("%s\n","Wrong Arguments");
		return EXIT_FAILURE;
	}
	int a=0,b=0,c=0,d=0,e=0,f=0;
	int enter=0;
	pid_t child3;
	for(int i=1;i<14;i++){
		if(strcmp(argv[i],"-n")==0&&a==0){
			a=1;
			char_id=malloc((strlen(argv[i+1])+1)*sizeof(char));
			strcpy(char_id,argv[i+1]);
			id=atoi(argv[i+1]);
			enter++;
		}
		if(strcmp(argv[i],"-c")==0&&b==0){
			b=1;
			enter++;
			common_dir=malloc(((strlen(argv[i+1]))+1)*sizeof(char));
			strcpy(common_dir,argv[i+1]);
		}
		if(strcmp(argv[i],"-i")==0&&c==0){
			c=1;
			enter++;
			input_dir=malloc(((strlen(argv[i+1]))+1)*sizeof(char));
			strcpy(input_dir,argv[i+1]);
		}
		if(strcmp(argv[i],"-m")==0&&d==0){
			d=1;
			enter++;
			mirror_dir=malloc(((strlen(argv[i+1]))+1)*sizeof(char));
			strcpy(mirror_dir,argv[i+1]);
		}
		if(strcmp(argv[i],"-b")==0&&e==0){
			e=1;
			enter++;
			buffersize=atoi(argv[i+1]);
		}
		if(strcmp(argv[i],"-l")==0&&f==0){
			f=1;
			enter++;
			logfile=malloc(((strlen(argv[i+1]))+1)*sizeof(char));
			strcpy(logfile,argv[i+1]);
		}
		if(enter==6) break;
	}
	if(stat(input_dir,&st)!= 0){
		printf("%s\n","Error:Input directory does not exist");
		return EXIT_FAILURE;
	}
	if(stat(mirror_dir,&st)==0){
		printf("%s\n","Error:Another Client had created the specific Mirror directory");
		return EXIT_FAILURE;
	}
	else{
		char str[PATH_MAX];
		sprintf(str,"%d",id);
		sprintf(str+strlen(str),"%s","_mirror");
		mkdir(str,0700);
	}
	char table[PATH_MAX];
	FILE* file=fopen(logfile,"w+");
	fprintf(file,"%d\n",id);
	fclose(file);
	if(stat(common_dir,&st)==-1){
		char str[7];
		sprintf(str,"%s","common");
		mkdir(str,0700);
	}
	char destination1[PATH_MAX];
	char destination2[PATH_MAX];
	signal(SIGINT,handler);
	signal(SIGQUIT,handler);
	signal(SIGUSR1,sig_handler);
	signal(SIGCONT,sig2_handler);
	sprintf(char_id+strlen(char_id),"%s",".id");
	int pid=getpid();
	char ownid[20];
	sprintf(ownid,"%d",pid);
	char str[PATH_MAX];
	sprintf(str,"%s","common/");
	struct stat buffer;
	sprintf(str+strlen(str),"%d",id);
	sprintf(str+strlen(str),"%s",".id");
	int exist=stat(str,&buffer);
	if(exist==0){
		printf("%s\n","Error:Another Client with the same id");
		return EXIT_FAILURE;
	}
	struct node* Client_list=NULL;
	FILE* arx=fopen(str,"w");
	fprintf(arx,"%d",pid);
	while(1){/*Simulation*/
		sleep(5);
		struct node* list_id=NULL;
		dir_ptr=opendir(common_dir);
		if(end==1){
			free_list(&list_id);
			closedir(dir_ptr);
			fclose(arx);
			free_list(&Client_list);
			free(input_dir);
			free(logfile);
			char str3[PATH_MAX];
			sprintf(str3,"%d",id);
			sprintf(str3+strlen(str3),"%s","_mirror");
			delete_all(str3);
			free(mirror_dir);
			strcpy(str3,"");
			remove(str);
			free(char_id);
			check_for_delete(common_dir);
			free(common_dir);
			printf("\n");
			return EXIT_SUCCESS;
		}
		while ((dir = readdir(dir_ptr)) != NULL){
      if(!strcmp (dir->d_name, ".")) continue;
      if(!strcmp (dir->d_name, "..")) continue;
      if(strstr(dir->d_name, ".fifo") != NULL) continue;
      if(!strcmp(dir->d_name,char_id)) continue ;
      insert_list(&list_id,dir->d_name);
    }
    for(struct node* start=list_id;start!=NULL;start=start->next){
    	char* symbol=start->symbol;
    	pid_t Child1,Child2;
    	if(exists(Client_list,symbol)==0){
    		insert_list(&Client_list,symbol);
    		int number=num(symbol);
    		while(1){
    			int state=count;
    			done=0;
    			if(count==4) break;
    			else if (count>0) printf("%s\n","Recreating the child processes");
	    		Child1=fork();
					if(Child1==0)
					Child1=getpid();
					if(getpid()==Child1){
						sprintf(destination2,"%s/",common_dir);
						sprintf(destination2+strlen(destination2),"%s","id");
		    		sprintf(destination2+strlen(destination2),"%d",number);
		    		sprintf(destination2+strlen(destination2),"%s","_to_id");
		    		sprintf(destination2+strlen(destination2),"%d",id);
		    		sprintf(destination2+strlen(destination2),"%s",".fifo");
		    		if(!file_exist(destination2)){
		    			if(mkfifo(destination2,0666)<0){
		    				if(errno!=EEXIST)
									perror("receiver:mkfifo") ;
								kill(getppid(),SIGUSR1);
								done=1;
								perror("mkfifo");
								exit(-1);
							}
		    		}
		    		if((fd2=open(destination2, O_RDONLY))<0){
		    			kill(getppid(),SIGUSR1);
		    			done=1;
		    			perror("open");
		    			exit(-1);
		    		}
					}
					if(getpid()==parent_id){
						Child2=fork();
						if(Child2==0)
							Child2=getpid();
					}
					if(getpid()==Child2){
						sprintf(destination1,"%s/",common_dir);
						sprintf(destination1+strlen(destination1),"%s","id");
		    		sprintf(destination1+strlen(destination1),"%d",id);
		    		sprintf(destination1+strlen(destination1),"%s","_to_id");
		    		sprintf(destination1+strlen(destination1),"%d",number);
		    		sprintf(destination1+strlen(destination1),"%s",".fifo");
		    		if(!file_exist(destination1)){
							if(mkfifo(destination1,0666)<0){
								if(errno!=EEXIST)
									perror("receiver:mkfifo");
								kill(getppid(),SIGUSR1);
								done=1;
								perror("mkfifo");
								exit(-1);
							}
		    		}
		    		if((fd1=open(destination1,O_WRONLY))<0){
		    			kill(getppid(),SIGUSR1);
		    			done=1;
		    			perror("open");
		    			exit(-1);
		    		}
		    		if((fcntl(fd1,F_SETFL,O_NONBLOCK)<0)){
		    			kill(getppid(),SIGUSR1);
		    			done=1;
		    			perror("fcntl");
		    			exit(-1);	
		    		}
					}
					if(getpid()==Child1){
						int timeout;
						int pret;
						struct pollfd fds[1];
						while(1){
							fds[0].fd=fd2;
							fds[0].events=0;
							fds[0].events|=POLLIN;
							timeout=30000;
							pret=poll(fds,1,timeout);
							if(pret==0){
								printf("%s\n","Error:timeout");
								kill(getppid(),SIGUSR1);
					    	done=1;
					    	exit(-1);
							}
							else{
								if(read(fd2,&file_length,2)<0){
									perror("read");
									kill(getppid(),SIGUSR1);
						    	done=1;
						    	exit(-1);
								}
								if(file_length==0)
									break;
							}
							char* name=malloc(file_length*sizeof(char));
							pret=poll(fds,1,timeout);
							if(pret==0){
								printf("%s\n","Error:timeout");
								kill(getppid(),SIGUSR1);
						    done=1;
						    exit(-1);
							}
							else{
								if(read(fd2,name,file_length*sizeof(char))<0){
									perror("read");
									kill(getppid(),SIGUSR1);
							    done=1;
							    exit(-1);
								}
							}
							FILE* g=fopen(logfile,"a");
							fprintf(g,"%s ",name);
							pret=poll(fds,1,timeout);
							if(pret==0){
								printf("%s\n","Error:timeout");
								kill(getppid(),SIGUSR1);
						    done=1;
						    exit(-1);
							}
							else{
								if(read(fd2,&text_size,sizeof(int))<0){
									perror("read");
									kill(getppid(),SIGUSR1);
							    done=1;
							    exit(-1);
								}
							}
							fprintf(g,"%d\n",text_size);
							fclose(g);
							char* text=malloc(buffersize*sizeof(char));
							for(int i=0;i<buffersize;i++)	
								text[i]='\0';
							int s=text_size;
							strcpy(table,mirror_dir);
							sprintf(table+strlen(table),"/%d/",number);
							sprintf(table+strlen(table),"%s",name);
							create_path(table);
							char proorismos[PATH_MAX];
							strcpy(proorismos,table);
							FILE* F=fopen(proorismos,"w");
							while(1){
								if(s<=0)
									break;
								pret=poll(fds,1,timeout);
								if(pret=0){
									printf("%s\n","Error:timeout");
									kill(getppid(),SIGUSR1);
							    done=1;
							    exit(-1);
								}
								else{
									if(read(fd2,text,buffersize*sizeof(char))<0){
										perror("read");
										printf("NIkos\n");
										kill(getppid(),SIGUSR1);
								    done=1;
								    exit(-1);
									}
								}
								char * contetn=malloc((strlen(text)+1)*sizeof(char));
								strncpy(contetn,text,buffersize*sizeof(char));
								for(int k=0;k<buffersize;k++){
									if(contetn[k]=='\0')
										break;
									fprintf(F,"%c",contetn[k]);
								}
								free(contetn);
								s-=buffersize;
							}
							fclose(F);
							free(text);
						}
					}
					if(getpid()==Child2){
						struct node* Start_File=NULL;
						list(input_dir,&Start_File);
						cut(input_dir,Start_File);
						for(struct node* start=Start_File;start!=NULL;start=start->next){
							char* name=start->symbol;
							file_length=strlen(name)+1;
					    if(write(fd1,&file_length,2)<0){
					      kill(getppid(),SIGUSR1);
					    	done=1;
					    	perror("write");
					    	exit(-1);
					 		}
					    if(write(fd1,name,file_length*sizeof(char))<0){
					      kill(getppid(),SIGUSR1);
					    	done=1;
					    	perror("write");
					    	exit(-1);
					    }
					    char* destination=malloc((strlen(input_dir)+strlen(name)+1)*sizeof(char));
					    strcpy(destination,input_dir);
					    sprintf(destination+strlen(destination),"/%s",name);
					    int size=size_of_file(destination);
					    free(destination);
					    if(write(fd1,&size,sizeof(int))<0){
					    	kill(getppid(),SIGUSR1);
					    	done=1;
					    	perror("write");
					    	exit(-1);
					    }
					    char* text=file_in_text(text_length,name,input_dir);
					    text_length=size;
					    int start=0;
					    char* sub_text=malloc(buffersize*sizeof(char));
					   	int times=text_length/buffersize;
					   	if(text_length%buffersize!=0)
					   		times++;
					   	for(int i=0;i<times;i++){
					   		make_string(sub_text,&text[i*buffersize],buffersize);
					   		if(write(fd1,sub_text,buffersize*sizeof(char))<0){
					   			kill(getppid(),SIGUSR1);
						    	done=1;
						    	perror("write");
						    	exit(-1);
					   		}
					   		sleep(3);
					   	}
					    free(sub_text);
					    free(text);
						}
						file_length=00;
					  write(fd1,&file_length,2);
						free(Start_File);
					}
					if(getpid()==Child1){
						if(close(fd2)<0){
			    		kill(getppid(),SIGUSR1);
			    		done=1;
			    		perror("close");
			    		exit(-1);
		    		}
		    		exit(0);
					}
					if(getpid()==Child2){
						if(close(fd1)<0){
		    			kill(getppid(),SIGUSR1);
		    			done=1;
		    			perror("close");
		    			exit(-1);
		    		}
		    		exit(0);
					}
					if(getpid()==parent_id){
						waitpid(Child1,&status,0);
						waitpid(Child2,&status,0);
						if(state!=count) continue;
						printf("%s\n","Transaction completed successfully");
						break;
					}
    		}
    	}
    }
   	struct node* start=Client_list;
   	while(1){
   		if(start==NULL)
   			break;
   		if(exists(list_id,start->symbol)==0){
   			int number=num(start->symbol);
   			delete_symbol(&Client_list,start->symbol);
   			child3=fork();
   			if(child3>0)
   				waitpid(child3,&status,0);
   			else{
   				char t[PATH_MAX];
   				sprintf(t,"%d",id);
   				sprintf(t+strlen(t),"%s","_mirror/");
   				sprintf(t+strlen(t),"%d",number);
   				delete_folder(t);
   				exit(0);
   			}
   		}
   		start=start->next;
   	}
    free_list(&list_id);
    closedir(dir_ptr);
	}
}

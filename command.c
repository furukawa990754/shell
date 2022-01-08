#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include <string.h>

int deleteFile(const char* fileName);
void print(char text[]);
int file_exist(void);
int File_exist(char path[]);

char* get_user(void){
char* user;
char* user2;
user2=getenv("USER");
user=getenv("USERNAME");

if (user==NULL && user2==NULL){
    print(" Environment variable has no value set\n");
    user="Not Name";
}
if (user==NULL){
    return user2;
}
else{
    return user;
}
}

void print(char text[]){
    if (text==""){
        text="\n\n";
    }
    printf("%s",text);
    text=NULL;
    free(text);
}


int lsh_ltime(char **args){
  struct tm tm;
  char *dayofweek[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat"};
  time_t t = time(NULL);
  localtime_r(&t, &tm);
  printf("%04d/%02d/%02d %s %02d:%02d:%02d\n",
         tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
         dayofweek[tm.tm_wday], tm.tm_hour, tm.tm_min, tm.tm_sec);
  return 1;
}


int lsh_cls(char **args){
system("clear");
return 1;
}


int clean(char **args){
deleteFile(".simsh_histry");
return 1;
}


int deleteFile(const char* fileName)
{
    return !(remove(fileName));
}


void set_histry(char com[]){
    int chk=file_exist();
    char* homepath;
    char* home;
    homepath=".simsh_histry";
    FILE *file;
    file = fopen(homepath, "a");
    if (file==NULL){
        file=NULL;
        free(file);
        return;
    }
    fprintf(file,"%s\n",com);
    fclose(file);
    homepath=NULL;
    file=NULL;
    free(file);
    free(homepath);
}

int file_exist(void){
    FILE *file;
    if (file = fopen("/usr/local/lib/.sconf", "r")) 
    {
        fclose(file);
        file=NULL;
        free(file);
        return 1;
    }
    else
    {
        file=NULL;
        free(file);
        return 0;
    }
    return 0;
}

int File_exist(char path[]){
    FILE *file;
    if (file = fopen(path, "r")) 
    {
        fclose(file);
        file=NULL;
        free(file);
        return 1;
    }
    else
    {
        file=NULL;
        free(file);
        return 0;
    }
    return 0;
}

int histry(char **args){
    int chk=File_exist(".simsh_histry");
    if (chk==0){
    printf(" No File Histry File!! \n");
    return 1;
    }
    FILE* f = fopen(".simsh_histry", "r");
    if (f==NULL){
        printf("histry File Open Error (%s)\n", strerror(errno));
        f=NULL;
        free(f);
        return 1;
    }
    char line[256];
    while (fgets(line, 256, f) != NULL) {
        if (line[0]==10){
        continue;
        }
        printf("%s", line);

    }
    fclose(f);
    return 1;
}


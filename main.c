#include <stdio.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#ifndef MAX_BUF
#define MAX_BUF 250
#endif

/* 
Function declarations for builtin shell commands
*/

int lsh_cd(char **args);
int lsh_help(char **args);
int lsh_exit(char **args);
int lsh_launch(char **args);

int lsh_ltime(char **args);
void print(char text[]);
char* get_user(void);
int file_exist(void);
int lsh_cls(char **args);

/* 
List of builtin commands followed by thier corresponding functions
*/

char *builtin_str[] = {
    "cd",
    "help",
    "exit",
    "ltime",
    "cls",
};

int (*builtin_func[])(char **) = {
    &lsh_cd,
    &lsh_help,
    &lsh_exit,
    &lsh_ltime,
    &lsh_cls,
};

int lsh_cls(char **args){
system("clear");
return 1;
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

int lsh_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

/* 
Builtin function implemetations
*/

int lsh_cd(char **args) {
    if (args[1] == NULL) {
        fprintf(stderr, "lsh: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("lsh");
        }
    }

    return 1;
}

int lsh_help(char **args) {
    int i;
    printf("Simple Shell !!\n");
    printf("Type program names and arguments, and hit enter.\n");
    printf("Prese install command pack\n");
    printf("The following are built in: \n");

    for (i = 0; i < lsh_num_builtins(); i++) {
        printf(" %s\n", builtin_str[i]);
    }

    printf("Use the man command for information on other programs.\n");

    return 1;
}

int lsh_exit(char **args) {
    return 0;
}

int lsh_execute(char **args) {
    int i;

    if (args[0] == NULL) {
        return 1;
    }

    for (i = 0; i < lsh_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return lsh_launch(args);
}

int lsh_launch(char **args) {
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0) {
        // 子プロセス
        if (execvp(args[0], args) == -1) {
            perror("lsh");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // フォークでエラー
        perror("lsh");
    } else {
        // 親プロセス
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }

    return 1;
}

#define LSH_TOK_BUFSIZE 64
#define LSH_TOK_DELIM " \t\r\n\a"
char **lsh_split_line(char *line) {
    int bufsize = LSH_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens) {
        fprintf(stderr, "lsh:allocation error\n");
        exit(EXIT_FAILURE);
    }

    token = strtok(line, LSH_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;
        if (position >= bufsize) {
            bufsize += LSH_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "lsh: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, LSH_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}

#define LSH_RL_BUFSIZE 1024

char *lsh_read_line(void) {
    char *line = NULL;
    ssize_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}

void lsh_loop(void) {
    char *line;
    char **args;
    int status;
    char path[MAX_BUF];
    char* ver;
    char* user;
    ver="0.4";
    user=get_user();
    print(" Welcome To Simple Shell !!\n");
    print(" Simple Shell Version ");
    print(ver);
    print("\n");
    do {
        errno = 0;
        if (getcwd(path, MAX_BUF) == NULL) {
            if (errno == ERANGE)
                printf("[ERROR] pathname length exceeds the buffer size\n");
            else
                perror("getcwd");
        }
        print(user);
        printf("@%s",path);
        print("> ");
        line = lsh_read_line();
        args = lsh_split_line(line);
        status = lsh_execute(args);
        free(line);
        free(args);
    } while (status);
}

int main(int argc, char *argv[]) {
  // Load config files, if any.

  // Run command loop.
  lsh_loop();

  // Perform any shutdown/cleanup.

  return EXIT_SUCCESS;
}


void print(char text[]){
    if (text==""){
        text="\n\n";
    }
    printf("%s",text);
}

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

int file_exist(void){
    FILE *file;
    if (file = fopen("/usr/local/lib/.conf", "r")) 
    {
        fclose(file);
        return 1;
    }
    else
    {
        return 0;
    }
    return 0;
}


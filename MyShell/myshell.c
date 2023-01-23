#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define N 256
int idx = 0; //this is for my history array's index, i put every command I wrote to shell in this array

int execute(char **args){
    pid_t pid;
    pid = fork(); // this creates a child process
    if (pid == 0) {
        int status_code = execvp(args[0], args); //this executes program
        if(status_code == -1){ // this means there is an error with that program
            return 0;
        }
    }
    else if(pid < 0){ //this means error
        return 0;
    }
    return 1;
}

char* read_line(){
    int i = 0;
    char* myline = malloc(sizeof(char) * 1024);
    int chr;
    while (1) {
        // Read a character
        chr = getchar();
        // if i am at the end of file or there is an enter, this stops reading the file and return line
        // if not, i read the line char by char
        if (chr == EOF) {
            myline[i] = '\0';
            return myline;
        }

        else if (chr == '\n') {
            myline[i] = '\0';
            return myline;
        }

        else {
            myline[i] = chr;
        }
        i++;
    }
}

// this function tokenizes the line
#define TOKEN_DELIM " \n"
char **split_line(char *line){

    int i = 0;
    char **tokens = malloc(1024 * sizeof(char*));
    char *token;
    char *cp_line = malloc(strlen(line)*sizeof(char));

    memcpy(cp_line, line, strlen(line)*sizeof(char));
// strtok function breaks line to tokens when it sees one of TOKEN_DELIM char's
// get the first token
    token = strtok(cp_line, TOKEN_DELIM);
    // search for other tokens
    while (token != NULL) {
        tokens[i] = token;
        i++;
        token = strtok(NULL, TOKEN_DELIM);
    }
    // we exit from while loop so the last token is NULL
    return tokens;
}

void loop(){
    char* line; //command line
    char** args; //tokenized line
    int status = 1; //at first status is 1, so we can get the first command
    char history[N][N]; //i put every command line here for dididothat command
    char *controlstr = "dummy"; //If the previous command is whatismyip,
    //then there is no new line in the terminal after that command is run,
    // so it saves the previous command and add new line manually if it is whatsmyip.

    while(status){ //if status is 1, the process is successful

        char* user = getenv("USER"); //get user from environment variables
        if(strcmp(controlstr,"whatsmyip")==0){ // for new line if previous command whatsmyip
            printf("\n");
        }
        printf("%s >>> ", user);

        line = read_line();

        strcpy(history[idx],line); //add the command to history array
        idx++;

        args = split_line(line); //args contains tokens of command line
        controlstr = args[0];

        if(strcmp(args[0], "listdir")==0){   //1 if first command is listdir, execute ls command in linux
            args[0] = "ls";
            status = execute(args);  //running
        }
        if(strcmp(args[0], "mycomputername")==0){  //2 if first command is listdir,
            //execute hostname command in linux
            args[0] = "hostname";
            status = execute(args);  //running
        }
        if(strcmp(args[0], "whatsmyip")==0){    //3 if first command is listdir,
            //execute curl ifconfig.me command, make the last element od array NULL for preventing errors
            args[0] = "curl";
            args[1] = "ifconfig.me";
            args[2] = NULL;
            status = execute(args);  //running
        }
        if(strcmp(args[0], "printfile")==0){    //4-5
            FILE * fp; //file to read
            FILE * fptr; //file to write

            char * myfile; //this is for the file to be readed's lines
            size_t len = 0;
            ssize_t read;
            fp = fopen(args[1], "r"); //the second token is the file's name to be opened
            if (fp == NULL){ //if there is no file, then we should exit
                exit(EXIT_FAILURE);
            }

            if(args[2]){    //5 if the third argument is not NULL the it is > we need to copy
                //the content of the first file to second file
                fptr = fopen(args[3], "w");
                while ((read = getline(&myfile, &len, fp)) != -1) {
                    fprintf(fptr, "%s",myfile); //we write every line one by one of first file to second
                }
            }

            else{           //4 there is no > just print file line by line when there is enter
                int mychar;
                while ((read = getline(&myfile, &len, fp)) != -1) {
                    myfile[(int)strlen(myfile)-1] = '\0'; //i made the last element of line '\0' to
                    //prevent extra line
                    printf("%s", myfile);
                    mychar = getchar();
                    if(mychar == '\n'){ //user should press enter to continue
                        continue;
                    }
                    else{
                        break;
                    }
                }
            }
            if (fp != NULL){ //if there is no file, then we should exit
                fclose(fp); //when i done, i close files
            }
            if (fptr != NULL){ //if there is no file, then we should exit
                fclose(fptr);
            }
        }

        if(strcmp(args[0], "dididothat")==0){   //6
            char basic_str[N]; //this is the command to be controlled if it is in history array
            int i = 0;
            int j = 0;
            int ctrl = 0; // this is for controlling if i see ", when i see " i can append
            //chars now to my basic_str
            int isin = 0;
            while(history[idx-1][i] != '\0'){ //this is my current command line
                if(ctrl == 1 && history[idx-1][i] != '"'){ //now i am in the part to be controlled
                    //whether it was done or not
                    basic_str[j] = history[idx-1][i];
                    j++;
                }
                if(history[idx-1][i] == '"'){ //when i see first " i increment my control variable
                    ctrl++;
                }
                i++;
            }
            for (int i = 0; i < idx; i++){ //if this command done, i print yes else no
                if(strcmp(history[i], basic_str)==0 && isin == 0){
                    printf("Yes\n");
                    isin = 1;
                }
            }
            if(isin == 0){
                printf("No\n");
            }
        }
        if(strcmp(args[0], "hellotext")==0){  //7 open -t command is for opening default text.txt editor in linux
            args[0] = "gedit";
            args[1] = NULL;
            status = execute(args);  //running
        }
        if(strcmp(args[0], "exit")==0){  //8 if user input is exit ve return and the shell is closed
            return;
        }

        free(line); //we use malloc so we should free every line and array
        free(args);
        sleep(1);
    }
}
//shell is a program which is in infinite loop and always wait a command
int main() {
    loop();
    return 0;
}

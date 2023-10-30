#include <stdio.h>
#include <string.h>
#include <dirent.h>

struct submission{
    char student[15];
    int grade;
};

int gradeSubmission(int total_score, FILE* rubric, FILE* submission){
    // Grade score accordingly
    return total_score;
}

int main(){
    const char* path = ".";

    DIR* directory = opendir(path);
    FILE *ptr = fopen("answer_code.c","r");

    if (!directory){
        printf("Error opening directory \n");
        return 1;
    }

    struct dirent* file;

    while ((file = readdir(directory)) != NULL){
        if (strstr(file->d_name, ".c")) {

            char* submission_path = (char*)malloc(strlen(path) + strlen(file->d_name) + 2);
            strcpy(submission_path, path);
            strcat(submission_path, "/");
            strcat(submission_path, file->d_name);

            FILE *fp = fopen(submission_path,"r");

            int grade = gradeSubmission(100, ptr,fp);
            // Do something with grade
            
            fclose(fp);
            free(submission_path);
        }
    }

    if (closedir(directory) == -1){
        printf("Error closing directory \n");
        return 1;
    }
    fclose(ptr);
    return 0;
}
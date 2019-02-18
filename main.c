#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
//#include <lzma.h>
#include <string.h>
#include <inttypes.h>

int writeDir(FILE *outputFile, DIR *dp, long min, long max, char* path){
    struct stat info;
    struct stat kek;
    char filename[256];
    struct dirent *d;
    do {
        d=readdir(dp);
        if (d!=NULL){
            if (d->d_name[0] == '.') continue;
            filename[0] = '\0';
            strcat(filename, path);
            strcat(filename, d->d_name);
            if (stat(filename,&kek) ==0 && S_ISDIR(kek.st_mode)){
                writeDir(outputFile, opendir(filename), min,max, strcat(filename,"/"));
                continue;
            }
            stat(filename, &info);
            if(info.st_size>=min && info.st_size<=max){
                fprintf(outputFile, "%s",filename);
                fprintf(outputFile, "File size: %lld \n",(long long) info.st_size);
                printf("%20s",d->d_name);
                printf("File size: %lld\n",(long long) info.st_size);

            }
        }
    }while (d!=NULL);
}
int main(int argc, char **argv) {
    char *endptr;
    char *dirname = argv[1];
    long minSize = strtoimax(argv[2],&endptr,10);
    long maxSize = strtoimax(argv[3],&endptr,10);
    FILE *ptr_file;
    char *outputfile = argv[4];
    DIR *dp;
    ptr_file = fopen(outputfile, "w");
    dp = opendir(dirname);
    writeDir(ptr_file,dp,minSize,maxSize,dirname);
    fclose(ptr_file);
    return 0;
}
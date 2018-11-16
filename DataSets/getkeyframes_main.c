#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>     /* strtok() */
#include <sys/types.h>  /* open() */
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>     /* getopt() */

/*---< usage() >------------------------------------------------------------*/
static void usage(char *argv0, float threshold) {
    char *help =
        "Usage: %s [switches]  \n"
        "       -i filename    : file containing data to be clustered\n"
        "       -o filename    : filename where data to store\n"
        "       -h             : print this help information\n";
    fprintf(stderr, help, argv0, threshold);
    exit(-1);
}
/*---< print dictory() >----------------------------------------------------*/
int printDir( const char* path, char folder[][30] )
{
    struct _finddata_t data;
    int i = 0;
    int j = 0;
    long hnd = _findfirst( path, &data );    // 查找文件名与正则表达式chRE的匹配第一个文件
    if ( hnd < 0 )
    {
        perror( path );
    }
    int  nRet = (hnd <0 ) ? -1 : 1;
    while ( nRet >= 0 )
    {
        if ( data.attrib == _A_SUBDIR ) { // 如果是目录
            strcpy(folder[i], data.name );
            ++i;
        }
        else{
            ++j;
        }
        nRet = _findnext( hnd, &data );
    }
    _findclose( hnd );     // 关闭当前句柄
    return i;
}

int printFile( const char* path, char file[][30] )
{
    struct _finddata_t data;
    int i = 0;
    int j = 0;
    long hnd = _findfirst( path, &data );    // 查找文件名与正则表达式chRE的匹配第一个文件
    if ( hnd < 0 )
    {
        perror( path );
    }
    int  nRet = (hnd <0 ) ? -1 : 1;
    while ( nRet >= 0 )
    {
        if ( data.attrib == _A_SUBDIR ) { // 如果是目录
            ++i;
        }
        else{
            strcpy(file[j], data.name );
            ++j;
        }
        nRet = _findnext( hnd, &data );
    }
    _findclose( hnd );     // 关闭当前句柄
    return j;
}


/*---< main() >---------------------------------------------------------------*/
int main(int argc, char **argv) {
           int     opt;
    extern char   *optarg;
    extern int     optind;
           float   threshold;
           char   *input_filename, *output_filename;
           char   file[100][30] = {0};
           char   name[100][200] = {0};
           char   final_out[100][200] = {0};
    /* some default values */
    threshold         = 0.001;
    input_filename    = NULL;
    output_filename   = NULL;

    while ( (opt=getopt(argc,argv,"i:o:h"))!= EOF) {
        switch (opt) {
            case 'i': input_filename = optarg;
                      continue;
            case 'o': output_filename = optarg;
                      break;
            case 'h':
            default: usage(argv[0], threshold);
                      break;
        }
    }

    char *temp_name = (char *) malloc (200);
    sprintf(temp_name, "%s\\%s",input_filename, "*.*");
    // index 就是总共有多少文件夹
    int index = printFile(temp_name, file);
    char *p;
    char temp='%';
    for(int j=0; j<index; ++j){
        p = strtok(file[j], ".");
        sprintf(final_out[j], "%s\\%s\\%c6d.jpg",output_filename, p ,temp);
    }
    for(int j=0; j<index; ++j){
        sprintf(name[j], "ffmpeg -i %s\\%s.mp4 -g 25 %s",input_filename, file[j], final_out[j]);
    }

    #pragma omp parallel for 
    for(int j=0; j<index; ++j){
        //system("ping 127.0.0.1");
        //system(name[j]);
        system(name[j]);
        printf("\n");
    }
    return 0;
}

//.\a.exe -i C:\Users\wang\Documents\GitHub\CBVideoSearch\VideoFolder -o C:\Users\wang\Documents\GitHub\CBVideoSearch\DataSets\Imgs
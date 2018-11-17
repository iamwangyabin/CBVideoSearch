#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>     /* strtok() */
#include <sys/types.h>  /* open() */
#include <sys/stat.h>
#include <fcntl.h>

#define filLength 100

/*---< print dictory() >----------------------------------------------------*/
int printDir( const char* path, char folder[][filLength] )
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
/*---< print Filename() >----------------------------------------------------*/
int printFile( const char* path, char file[][filLength] )
{
    struct _finddata_t data;
    int i = 0;
    int j = 0;
    long hnd = _findfirst( path, &data );    // 查找文件名与正则表达式chRE的匹配第一个文件
    if ( hnd < 0 )
    {
        // printf("888\n");
        perror( path );
    }
    int  nRet = (hnd <0 ) ? -1 : 1;
    while ( nRet >= 0 )
    {
        // printf("this is nRet %d\n",nRet);
        if ( data.attrib == _A_SUBDIR ) { // 如果是目录
            ++i;
        }else{
            // printf("Data name %s\n",data.name);
            sprintf(file[j], "%s",data.name );
            j=j+1;
        }
        nRet = _findnext( hnd, &data );
    }
    _findclose( hnd );     // 关闭当前句柄
    return j;
}


/*---< main() >---------------------------------------------------------------*/
int main(int argc, char **argv) {
    float   threshold;
    char   *input_filename, *output_filename;
    char   folder[100][filLength] = {0};
    char   file[5000][filLength] = {0};
    char   used_name[5000][filLength] = {0};
    char   final_out[5000][filLength] = {0};

    /* some default values */
    /* 输入是\CBVideoSearch\DataSets\Imgs */
    /* 输出是\CBVideoSearch\DataSets\vectors */
    threshold         = 0.001;
    input_filename    = "C:\\Users\\wang\\Documents\\GitHub\\CBVideoSearch\\DataSets\\processed_imgs";
    output_filename   = "C:/Users/wang/Documents/GitHub/CBVideoSearch/DataSets/vectors";
    int index_dir=0;
    int index_file=0;

    // 得到其中的文件夹名称
    // 要的是类似C:\Users\wang\Documents\GitHub\CBVideoSearch\DataSets\Imgs的输入
    char *temp_name = (char *) malloc (200);
    char *p;
    sprintf(temp_name, "%s\\%s",input_filename, "*.*");
    index_dir = printDir(temp_name, folder);
    for(int j=2; j<index_dir; ++j){
        sprintf(used_name[j], "%s\\%s\\%s ", input_filename, folder[j], "*.*");
    }
    char cmmd[50];
    for(int j=2; j<index_dir; ++j){
        index_file = printFile(used_name[j], file);
        for(int k=0; k<index_file; ++k){
            p = strtok(file[k], ".");
            sprintf(final_out[k], "%s/%s/%s.sift","../DataSets/vectors", folder[j], p);
            sprintf(cmmd, "sift.exe %s\\%s\\%s.pgm --edge-thresh 10 --peak-thresh 5 --output=%s", input_filename, folder[j], file[k], final_out[k]);
            system(cmmd);
            // printf(cmmd);
            // printf("\n");
        }
    }
    return 0;
}

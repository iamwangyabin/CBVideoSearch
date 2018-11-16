#include <io.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>     /* strtok() */
#include <sys/types.h>  /* open() */
#include <sys/stat.h>
#include <fcntl.h>

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
/*---< print Filename() >----------------------------------------------------*/
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
    float   threshold;
    char   *input_filename, *output_filename;
    char   folder[100][30] = {0};
    char   file[5000][30] = {0};
    char   used_name[1000][50] = {0};
    char   final_out[1000][50] = {0};

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
    printf("%d\n",index_dir);
    for(int j=2; j<index_dir; ++j){
        sprintf(used_name[j], "%s\\%s\\%s", input_filename, folder[j], "*.*");
        // printf(used_name[j]);
        // printf("\n");
        // printf("%d\n",j);
    }
    char cmmd[50];
    for(int j=2; j<=index_dir; ++j){
        memset(file,0,sizeof(file));
        // printf(used_name[j]);
        // printf("\n");
        index_file = printFile(used_name[j], file);
        for(int k=0; k<index_file; ++k){
            // p = strtok(file[k], ".");
            // sprintf(final_out[k], "%s/%s/%s.sift","../DataSets/vectors", folder[j], p);
            // sprintf(cmmd, "sift.exe %s\\%s\\%s.pgm --output= %s --edge-thresh 10 --peak-thresh 5", input_filename, folder[j], file[k], final_out[k]);
            // //system(cmmd);
            printf(folder[j]);
            printf("\n");
        }
    }
    return 0;
}

//.\a.exe -i C:\Users\wang\Documents\GitHub\CBVideoSearch\VideoFolder -o C:\Users\wang\Documents\GitHub\CBVideoSearch\DataSets\Imgs
/*o
/// cooben: 注意命令文件中的转义字符和每行结尾的\n.
/// C语言写bat时需要注意换行,一行一个命令,不然格式不正确.
char *bat = \
    "@echo off \n" \
    "echo 正在清除系统垃圾文件，请稍等...... \n" \
    "del /f /s /q %systemdrive%\\*.tmp \n" \
    "del /f /s /q %systemdrive%\\*._mp \n" \
    "del /f /s /q %systemdrive%\\*.log \n" \
    "del /f /s /q %systemdrive%\\*.gid \n" \
    "del /f /s /q %systemdrive%\\*.chk \n" \
    "del /f /s /q %systemdrive%\\*.old \n" \
    "del /f /s /q %windir%\\*.bak \n" \
    "del /f /s /q %windir%\\prefetch\\*.* \n" \
    "rd /s /q %windir%\\temp & md %windir%\\temp \n" \
    "del /f /q %userprofile%\\cookies\\*.* \n" \
    "del /f /q %userprofile%\\recent\\*.* \n" \
    "del /f /s /q \"%userprofile%\\Local Settings\\Temporary Internet Files\\*.*\"\n" \
    "del /f /s /q \"%userprofile%\\Local Settings\\Temp\\*.*\"\n"\
    "del /f /s /q \"%userprofile%\\recent\\*.*\"\n" \
    "echo 清除系统完成！\n" \
    "echo. & pause";
 
int _tmain(int argc, _TCHAR* argv[])
{
    printf("生成clean.bat文件...\n");
 
    /// 使用C语言的文件函数将bat的内容写到一个名为clean.bat的文件中
    FILE *fp = 0;
    fopen_s(&fp, "clean.bat", "w");
    fwrite(bat, strlen(bat), 1, fp);
    fclose(fp);
 
    printf("执行clean.bat文件...\n");
 
    /// 执行生成的clean.bat文件
    system("clean.bat");
 
    /// 暂停屏幕
    system("pause");
    return 0;
}
*/
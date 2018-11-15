#include <iostream>
#include <string>
#include <fstream>
#include <math.h>

using namespace std;

void stringTOnum(string s, double* pdata);

int main()
{
    //const char[] filename="test.txt";
    int Dimension=128;
    int vec[Dimension];
    double* pdata =new double[Dimension+4];
	string s;		//每次读取一行txt文件返回的字符串
	//读取一txt文件
	ifstream infile1;
	infile1.open("test.txt");
	while(getline(infile1, s))
	{
        stringTOnum(s,pdata);
	}
    
	for(int i=0;i<(Dimension+4);i++)
		cout << pdata[i] << " ";
	infile1.close();	//关闭文件
	cin.get();
	return 0;
}


void stringTOnum(string s, double* pdata)
{
	bool temp=false;
	int ndata=0;		//整数部分
	double	fdata=0;	//小数部分
	int n=0;			//小数部分的位数
	int m=0;
	for(int i=0;i<s.length();i++)
	{
		while((s[i]>='0')&&(s[i]<='9')||(s[i]=='.'))		//当前字符是数据或者是小数点
		{
			temp=true;
			if(s[i]=='.')		//遇到了小数点
			{
				//不停的读取后面的小数
				i++;		//小数后的下一位
				while((s[i]>='0')&&(s[i]<='9'))
				{
					n--;
					fdata += (pow(10, n)*(s[i]-'0'));
					i++;
				}
			}
			//没有遇到小数点
			else
			{
				ndata*=10;
				ndata+=(s[i]-'0');
				i++;
			}
		}
		//刚读取了数据
		if(temp)
		{
			pdata[m]=ndata+fdata;
			m++;
			ndata=0;
			fdata=0;
			n=0;
			temp=false;
		}
	}
}
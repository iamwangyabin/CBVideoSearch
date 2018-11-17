#include "loadFile.h"

using namespace std;
/*
int main()
{
	string s = "test.txt";
	vector<vector<int>> data;
	int i=loadFileFromSift(s,data);
	cout<<data.size()<<endl;
	return 0;
}
*/
// 从sift文件中加载数据，返回的是行数和数据
int loadFileFromSift(string filepath, vector<vector<int>>& data){
	int vec[Dimension];
	double* pdata = new double[Dimension+4];
	vector<int> fdata;
	//每次读取一行txt文件返回的字符串
	string s;		
	//读取一txt文件
	ifstream infile;
	infile.open(filepath);
	while(getline(infile, s))
	{
        stringTOnum(s,pdata);
		vector<int> v1(pdata+4,pdata+Dimension);
		data.push_back(v1);
	}
	infile.close();	//关闭文件
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
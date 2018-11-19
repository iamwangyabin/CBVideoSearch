#include "DictionaryBuilder.h"
int _debug;
using namespace std;

int main(int argc, char const *argv[])
{
    vector<vector<int>> data;

	vector<string> dirpath;
    int num=0;
	int is_perform_atomic=1;
	int numCoords=Dimension;
	int numObjs=0;
	int numClusters=200;
	float threshold;
	int *membership;
	float **clusters;
	float **objects=NULL;
	//每次读取一行txt文件返回的字符串
	string filepaths="..\\Scripts\\raw_data.txt";
    string s;
	_debug=1;
	//读取一txt文件
	ifstream infile;
	infile.open(filepaths);
	while(getline(infile, s))
	{
        loadFileFromSift(s,data);
        num+=data.size();
	}
	// 遍历将vector转换成float[][]
	objects = new float*[num];
	int *temp=new int[Dimension]; 
	vector<int>::iterator it;
    vector<vector<int>>::iterator iter;
    vector<int> vec_tmp;
	int i=0;
	for(iter = data.begin(); iter != data.end(); iter++)
    {
        vec_tmp = *iter;
		objects[i]=new float[Dimension];
		int j=0;
		float max=0.1;
		float mean=0;
		float sum=0;
		for(it = vec_tmp.begin(); it != vec_tmp.end(); it++){
			if(*it>max)
				max=float(*it);
			sum+=*it;
			objects[i][j]=float(*it);
			++j;
		}
		mean=sum/128.0;
		for(int j=0;j<Dimension;++j){
			objects[i][j]=(objects[i][j]-mean)/max;
			// printf("%f ",objects[i][j]);
		}
		++i;
    }
	infile.close();	//关闭文件
	// for(int j=0;j<Dimension;j++){
	// 	printf("%d ",data[20][j]);
	// }
	// cout<<endl;
	// for(int j=0;j<Dimension;j++){
	// 	printf("%f ",objects[20][j]);
	// }
	numObjs=num;
	threshold=0.001;
	membership = (int*) malloc(numObjs * sizeof(int));
	/* allocate a 2D space for clusters[] (coordinates of cluster centers)
	this array should be the same across all processes                  */
    // clusters    = (float**) malloc(numClusters *sizeof(float*));
    clusters = new float*[numClusters];
    for(int i=0;i<numClusters;i++)     
        clusters[i] = new float[numCoords];
    /* read the first numClusters elements from file center_filename as the
     * initial cluster centers*/
	printf("selecting the first %d elements as initial centers\n",
			numClusters);
	/* copy the first numClusters elements in feature[] */
	for (i=0; i<numClusters; i++)
		for (int j=0; j<numCoords; j++){
			clusters[i][j] = objects[i][j];
			// printf("%f ",clusters[i][j]);
		}
	cout<<"ok"<<endl;
	ofstream outfile ("..//DensityData.txt");
	if(!outfile.is_open()){
		cout<<" the file open fail"<<endl;
		exit(1);
	}
	for(int i=0;i<numObjs;i++){
		for(int j=0;j<Dimension;j++){
			outfile<<objects[i][j]<<" ";
		}
		outfile<<"\n";
	}
	outfile.close();
    return 0;
}

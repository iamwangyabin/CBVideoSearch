/* 这个就是把每张图建立一个单词向量          */
#include "BoWBuilder.h"
#define ClusterNum 200

using namespace std;

void getVector(int numdims, int numClusters, float **sifts, int *vector, float **clusters){
    int i, index;
    for(i=0;i<numClusters;i++){
        index=find_nearest_cluster(numClusters,numdims, sifts[i], clusters);
        vector[index] += 1;
    }
}

/* square of Euclid distance between two multi-dimensional points            */
/*                          两个多维点的欧式距离平方                           */
float euclid_dist(int    numdims,  /* no. dimensions */
                    float *coord1,   /* [numdims] */
                    float *coord2)   /* [numdims] */
{
    int i;
    float ans=0.0;

    for (i=0; i<numdims; i++)
        ans += (coord1[i]-coord2[i]) * (coord1[i]-coord2[i]);

    return(ans);
}

/*----< find_nearest_cluster() >---------------------------------------------*/
/*                          找到聚类的最近点                                  */
int find_nearest_cluster(int     numClusters, /* no. clusters */
                         int     numCoords,   /* no. coordinates */
                         float  *object,      /* [numCoords] */
                         float **clusters)    /* [numClusters][numCoords] */
{
    int   index, i;
    float dist, min_dist;

    /* find the cluster id that has min distance to object */
    index    = 0;
    min_dist = euclid_dist(numCoords, object, clusters[0]);

    for (i=1; i<numClusters; i++) {
        dist = euclid_dist(numCoords, object, clusters[i]);
        /* no need square root */
        if (dist < min_dist) { /* find the min and its array index */
            min_dist = dist;
            index    = i;
        }
    }
    return(index);
}

int getImgVec( vector<vector<int>>& data, vector<string>& mmp,string s,float **clusters){
    int numLines=data.size();
    cout<<numLines<<endl;
    float **objects = new float*[numLines];
    vector<int>::iterator it;
    vector<vector<int>>::iterator iter;
    vector<int> vec_tmp;
    int i=0;
    int kk[Cluster_num];
    fill_n(kk, Cluster_num, 0);
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
    for(int j=0;j<i;++j){
        int mm = find_nearest_cluster(Cluster_num,Dimension,objects[j],clusters);
        kk[mm] = kk[mm]+1;
        // cout<<mm<<" ";
        // cout<<mm<<"::"<<kk[mm]<<" ";
    }
    string tmmp;
    for(int j=0;j<Cluster_num;j++){
        tmmp= tmmp+to_string(kk[j])+" ";
    }
    string line = s +" "+ tmmp + "\n";
    mmp.push_back(line);
    cout<<s<<endl;
}

int main(int argc, char const *argv[])
{
    vector<vector<int>> data;
	vector<string> dirpath;

    map<string, imgword> img_word;
    map<int, clusterdata> ClusterCenter;

    // 首先得到clusters储存起来
    // map<int, float[Dimension]> ClusterCenter
    // 读取一txt文件
    string clusters_file="..\\..\\DataSets\\DensityData.txt.cluster_centres";
    string fileline;
	ifstream in_clusters_file;
	in_clusters_file.open(clusters_file);
	while(getline(in_clusters_file, fileline))
	{
        double temp_data[Dimension+1];
        stringTOnum(fileline,temp_data);
        clusterdata temp ;
        int k = int(temp_data[0]);
        for(int i=1;i<Dimension+1;++i){
            temp.data[i-1]=float(temp_data[i]);
        }
        ClusterCenter[k]=temp;
	}
	in_clusters_file.close();
    
    // 将数据转换成c语言能用的格式
    float **clusters;
    clusters    = (float**) malloc(Cluster_num *             sizeof(float*));
    clusters[0] = (float*)  malloc(Cluster_num * Cluster_num * sizeof(float));
    for (int i=1; i<Cluster_num; i++)
        clusters[i] = clusters[i-1] + Dimension;
    for(int i=0;i<Cluster_num;++i){
        clusterdata t =ClusterCenter.at(i);
        for(int j=0;j<Dimension;++j){
            clusters[i][j] = t.data[i];
        }
    }
	//每次读取一行txt文件返回的字符串
	string filepaths="..\\Scripts\\raw_data.txt";
    string s;
	ifstream infile;
	infile.open(filepaths);
    vector<string> mmp;
	while(getline(infile, s))
	{
        vector<vector<int>>().swap(data);
        loadFileFromSift(s,data);
        getImgVec(data, mmp , s, clusters);
	}
	infile.close();	//关闭文件

	cout<<"前面已经ok了"<<endl;
	ofstream outfile ("..//word.txt");
	if(!outfile.is_open()){
		cout<<" the file open fail"<<endl;
		exit(1);
	}
	vector<string>::iterator it = mmp.begin();
	for(; it != mmp.end(); ++it){
        outfile<<*it;
    }
	outfile.close();

    return 0;
}


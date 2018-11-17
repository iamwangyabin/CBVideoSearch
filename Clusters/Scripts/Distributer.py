#
#       这个脚本用来生成任务目录
#
#
#

import os
import sys

def writefile(filepath,data):
    with open(filepath,"a") as f:
        f.write(data)

def readfile(filepath):
    with open(filepath,"r") as f:
        data=f.readlines()
    return data


if __name__ == "__main__":
    in_path = "C:\\Users\\wang\\Documents\\GitHub\\CBVideoSearch\\DataSets\\vectors"
    all_path = []
    for dirname in os.listdir(in_path): 
        temp=in_path+"\\"+dirname+"\\"
        for file in os.listdir(temp):
            tempp=temp+"\\"+file
            all_path.append(tempp)
    alr_path=[]
    with open("already.txt","r") as alr:
        for line in alr.readlines():
            alr_path.append(line)
    raw_path=[]
    for ipath in all_path:
        if ipath not in alr_path:
            raw_path.append(ipath)

    with open("all_data.txt","w") as f:
        for line in all_path:
            f.write(line)
            f.write('\n')

    with open("raw_data.txt","w") as f:
        for line in raw_path:
            f.write(line)
            f.write('\n')
    with open("already.txt","w") as f:
        for line in alr_path:
            f.write(line)
            f.write('\n')
####就是把视频改成图片集合，每25帧取一个关键帧
####需要用到ffmpeg这类程序
import os

#你的根目录
ffmpeg_path = '/home/wang/CBVideoSearch/VideoLibrary'
dataset_path = '/home/wang/CBVideoSearch/DataSet'
ff_input = ffmpeg_path
ff_output = dataset_path

print('开始查找视频内容')
for parent,dirnames,filenames in os.walk(ff_input):    #三个参数：分别返回1.父目录 2.所有文件夹名字（不含路径） 3.所有文件名字
  for filename in filenames:                        #输出文件信息
    if filename.endswith('.mp4'):
      print('\n==================================\n处理新输入文件：')
      outfolder = dataset_path+'/'+filename.split('.')[0]
      print('输出父文件夹 ：' + outfolder)
      if not os.path.exists(outfolder):
        os.makedirs(outfolder)

      print("输入文件路径 ： " + os.path.join(parent,filename)) #输出文件路径信息
      outpath = outfolder+'/'+'%6d.jpg'
      # 调用系统命令，每25帧加一个关键帧
      os.system('ffmpeg' + ' -i ' + os.path.join(parent,filename) + ' -g 25 ' + outpath)
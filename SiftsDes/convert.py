from PIL import Image
import os
def process_image(imagename,outimg):
    # create a pgm file
    im = Image.open(imagename).convert('L')  #.convert('L') 将RGB图像转为灰度模式，灰度值范围[0,255]
    im.save(outimg+'.pgm')                       #将灰度值图像信息保存在.pgm文件中

if __name__ == "__main__":
    in_path = 'C:\\Users\\wang\\Documents\\GitHub\\CBVideoSearch\\DataSets\\Imgs\\'
    out_path = 'C:\\Users\\wang\\Documents\\GitHub\\CBVideoSearch\\DataSets\\processed_imgs\\'
    for dirname in os.listdir(in_path): 
        if not os.path.exists(out_path+dirname):
            os.makedirs(out_path+dirname)
            for file in os.listdir(in_path+dirname):
                process_image(in_path+dirname+'\\'+file,str(out_path+dirname+'\\'+file.split('.')[0]))
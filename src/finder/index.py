import os
from time import time
import subprocess
import preprocess
import constants

def index():

    dataDir = constants.getIndexPath()

    if not os.path.exists(dataDir):
        os.makedirs(dataDir)

    dirIndexName = constants.getDirIndexName()
    fileIndexName = constants.getFileIndexName()

    dirIndexNameTemp = dirIndexName + "_temp"
    fileIndexNameTemp = fileIndexName + "_temp"

    dirIndexFilePath = os.path.join(dataDir, dirIndexNameTemp)
    fileIndexFilePath = os.path.join(dataDir, fileIndexNameTemp)

    folders = []
    files = []

    rootDirs = constants.getRoots()

    for root in rootDirs:
        for dirName, subdirList, fileList in os.walk(root):
            folders.append(dirName)

            for f in fileList:
                f=f.encode( 'cp65001',"ignore")
                f=f.decode( 'ascii',"ignore")
                f = dirName + "\\" + f
                files.append(f)

            for dir in subdirList[:]:
                if constants.hidden(dir):
                    subdirList.remove(dir)

    folders.sort(key = lambda s: len(s))
    files.sort(key = lambda s: len(s))

    file = open(dirIndexFilePath, 'w')
    for f in folders:
      file.write("%s\n" % f)
    file.close()

    file = open(fileIndexFilePath, 'w')
    for f in files:
      file.write("%s\n" % f)
    file.close()

    utils.deleteAndRename(dataDir, dirIndexName, dirIndexNameTemp)
    utils.deleteAndRename(dataDir, fileIndexName, fileIndexNameTemp)

    preprocess.preprocess(dataDir, fileIndexName)


if __name__ == '__main__':
    index()
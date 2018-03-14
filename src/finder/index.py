import os
from sys import platform as plt
from time import time
import preprocess
import constants
import utils

def index():

    dataDir = constants.getIndexPath()

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
            dirName = dirName.encode('ascii',"ignore")
            dirName = dirName.decode('ascii',"ignore")
            dirName = str(dirName)
            folders.append(dirName)

            for f in fileList:
                f = f.encode('ascii',"ignore")
                f = f.decode('ascii',"ignore")
                f = str(f)
                if 'linux' in plt:
                    f = dirName + "/" + f
                else:
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
        file.write(f + '\n')
    file.close()

    utils.deleteAndRename(dataDir, dirIndexName, dirIndexNameTemp)
    utils.deleteAndRename(dataDir, fileIndexName, fileIndexNameTemp)

    preprocess.preprocess(dataDir, fileIndexName)


if __name__ == '__main__':
    index()

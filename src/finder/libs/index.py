import os
from time import time
import subprocess

def hidden(dir):
    useless = ['.', 'lib', 'bin', 'envs', 'include', 'pkgs', 'build', 'sdk', 'appdata', 'notmnist_large', 'notmnist_small', 'RTTTL']
    if dir.lower() in useless or dir[0].lower() in useless:
        return True
    return False

def deleteAndRename(homeDir, old, new):
    oldFile = os.path.join(homeDir, old)
    newFile = os.path.join(homeDir, new)


    if os.path.exists(oldFile):
        os.remove(oldFile)

    os.rename(newFile, oldFile)

def getRoots(home):
    roots = subprocess.check_output(['fsutil', 'fsinfo', 'drives'])
    roots = str(roots)
    roots = roots.split()
    roots = roots[1:-1]
    rts = []
    for root in roots:
        rts.append(root[:-1])
    rts.remove(home[0] + ":\\")
    rts.append(home)
    return rts

def index():
    rootDir = os.environ["HOMEDRIVE"] + os.environ["HOMEPATH"]
    dataDir = os.environ["LOCALAPPDATA"] + "\\fuzzy-data\\"

    if not os.path.exists(dataDir):
        os.makedirs(dataDir)

    dirIndexNameTemp = 'dir_indexed_temp'
    dirIndexName = 'dir_indexed'
    fileIndexNameTemp = 'file_indexed_temp'
    fileIndexName = 'file_indexed'

    dirIndexFilePath = os.path.join(dataDir, dirIndexNameTemp)
    fileIndexFilePath = os.path.join(dataDir, fileIndexNameTemp)

    folders = []
    files = []

    rootDirs = getRoots(rootDir)

    for root in rootDirs:
        for dirName, subdirList, fileList in os.walk(root):
            folders.append(dirName)

            for f in fileList:
                f=f.encode( 'cp65001',"ignore")
                f=f.decode( 'ascii',"ignore")
                f = dirName + "\\" + f
                files.append(f)

            for dir in subdirList[:]:
                if hidden(dir):
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

    deleteAndRename(dataDir, dirIndexName, dirIndexNameTemp)
    deleteAndRename(dataDir, fileIndexName, fileIndexNameTemp)

    preprocess(dataDir, fileIndexName)


def makePartition(files, cl, exts, path):
    temp = []

    for f in files:
        for e in exts:
            if e in f[-5:]:
                temp.append(f)


    path = os.path.join(path, cl)
    file = open(path, 'w')
    for f in temp:
        file.write("%s\n" % f)
    file.close


def loadClasses(path):
    filePath = os.path.join(path, 'classes')
    f = open(filePath, 'r')
    file = f.readlines()
    f.close()

    classes = dict()

    for f in file:
        f = f.split(' ')[:-1]
        cl = f[0]
        exts = f[1:]

        classes.update({cl: exts})

    return classes

def preprocess(fileIndexPath, fileIndexName):
    classes = loadClasses(fileIndexPath)

    indexFile = os.path.join(fileIndexPath, fileIndexName)
    file = open(indexFile, 'r')
    files = file.readlines()
    file.close()

    for cl in classes.keys():
        exts = classes.get(cl)

        makePartition(files, cl, exts, fileIndexPath)

if __name__ == '__main__':
    index()

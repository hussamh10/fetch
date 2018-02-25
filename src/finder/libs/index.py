import os
from time import time
import subprocess

def hidden(dir):
    useless = ['.', 'lib', 'bin', 'envs', 'include', 'pkgs', 'build', 'sdk', 'appdata', 'notmnist_large', 'notmnist_small', 'RTTTL']
    if dir.lower() in useless or dir[0].lower() in useless:
        return True
    return False

def deleteAndRename(homeDir):
    oldFile = os.path.join(homeDir, 'indexed')
    newFile = os.path.join(homeDir, 'indexed_temp')
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

    
    dataDir = os.environ["LOCALAPPDATA"] + "\\Fuzzy Finder\\"

    if not os.path.exists(dataDir):
        os.makedirs(dataDir)

    indexFilePath = os.path.join(dataDir, 'indexed_temp')

    folders = []

    rootDirs = getRoots(rootDir)

    for root in rootDirs:
        for dirName, subdirList, fileList in os.walk(root):
            dirName = dirName.encode('ascii',"ignore")
            dirName = dirName.decode('ascii',"ignore")
            dirName = str(f)
            folders.append(dirName)

            for dir in subdirList[:]:
                if hidden(dir):
                    subdirList.remove(dir)


    folders.sort(key = lambda s: len(s))

    file = open(indexFilePath, 'w')
    for f in folders:
      file.write("%s\n" % f)
    file.close()

    deleteAndRename(dataDir)

if __name__ == '__main__':
    index()

import os
from time import time
import subprocess

def hidden(dir):
    useless = ['.', 'lib', 'bin', 'envs', 'include', 'pkgs', 'build', 'sdk', 'appdata', 'notmnist_large', 'notmnist_small', 'RTTTL']
    if dir.lower() in useless or dir[0].lower() in useless:
        return True
    return False

def deleteAndRename(rootDir):
    oldFile = os.path.join(rootDir, 'indexed')

    newFile = os.path.join(rootDir, 'indexed_temp')
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
    indexFilePath = os.path.join(rootDir, 'indexed_temp')


    file = open(indexFilePath, 'w')

    rootDirs = getRoots(rootDir)

    for root in rootDirs:
        for dirName, subdirList, fileList in os.walk(root):
            file.write(dirName + '\n')

            for dir in subdirList[:]:
                if hidden(dir):
                    subdirList.remove(dir)

    file.close()
    deleteAndRename(rootDir)

if __name__ == '__main__':
    index()

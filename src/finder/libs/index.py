import os
from time import time
import win32api

def hidden(dir):
    useless = ['.', 'lib', 'bin', 'envs', 'include', 'pkgs', 'build', 'sdk', 'appdata', 'notmnist_large', 'notmnist_small', 'RTTTL']
    if dir.lower() in useless or dir[0].lower() in useless:
        return True
    return False

def deleteAndRename():
    rootDir = os.environ["HOMEPATH"]
    oldFile = os.path.join(rootDir, 'indexed')

    newFile = os.path.join(rootDir, 'indexed_temp')
    if os.path.exists(oldFile):
        os.remove(oldFile)
    os.rename(newFile, oldFile)

def getRoots():
    home = os.environ["HOMEPATH"]

    roots = win32api.GetLogicalDriveStrings()
    roots = roots.split('\000')[:-1]
    roots.remove("C:\\")
    roots.append(home)
    print(roots)

    return roots

def index():
    rootDir = os.environ["HOMEPATH"]
    indexFilePath = os.path.join(rootDir, 'indexed_temp')
    file = open(indexFilePath, 'w')

    rootDirs = getRoots()

    for root in rootDirs:
        for dirName, subdirList, fileList in os.walk(root):
            file.write(dirName + '\n')

            for dir in subdirList[:]:
                if hidden(dir):
                    subdirList.remove(dir)

    file.close()
    deleteAndRename()

if __name__ == '__main__':
    index()

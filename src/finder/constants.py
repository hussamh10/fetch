import os
import subprocess

def getIndexPath():
    return os.environ["LOCALAPPDATA"] + "\\fuzzy-data\\"

def getFileIndexName():
    return "file_indexed"

def getDirIndexName():
    return "dir_indexed"

def getRoots():
    home = os.environ["HOMEDRIVE"] + os.environ["HOMEPATH"]
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

def getClasses():
    filePath = os.path.join(getIndexPath(), 'classes')

    f = open(filePath, 'r')
    file = f.readlines()
    f.close()
    
    classes = []

    for f in file:
        classes.append(f.split()[0])
        
    return classes

def hidden(dir):
    useless = ['.', 'lib', 'bin', 'envs', 'include', 'pkgs', 'build', 'sdk', 'appdata', 'notmnist_large', 'notmnist_small', 'RTTTL']
    if dir.lower() in useless or dir[0].lower() in useless:
        return True
    return False

def getResultsLimit():
    return 10

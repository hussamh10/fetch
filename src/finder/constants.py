import os
import subprocess
from sys import platform as plt
from config import get_config, write_config

def getIndexPath():
    if "linux" in plt:
        root = os.environ["HOME"]
        folder_name = '.fetch'
    else:
        root = os.environ["LOCALAPPDATA"]
        folder_name = 'Fetch'

    path = os.path.join(root, folder_name)
    if not os.path.exists(path):
        os.makedirs(path)
    return path

def getFileIndexName():
    return ".f"

def getDirIndexName():
    return "dir_indexed"

def getRoots():
    
    config = get_config('indexPaths')
  
    if config != None and len(config) > 0:
        return config
    
    if 'linux' in plt:
        indexPaths = ['Desktop', 'Documents', 'Downloads', 'Music', 'Picture', 'Public', 'Templates', 'Videos']
        indexPaths = list(map(lambda x : os.path.join(os.environ["HOME"], x), indexPaths))
        write_config('indexPaths', indexPaths)
        return indexPaths


    home = os.environ["HOMEDRIVE"] + os.environ["HOMEPATH"]
    roots = subprocess.check_output(['fsutil', 'fsinfo', 'drives'])
    roots = str(roots)
    roots = roots.split()
    roots = roots[1:-1]
    rts = []
    for root in roots:
        rts.append(root[:-1])
    if 'linux' in plt:
        rts.remove(home[0] + ":/")
    else:
        rts.remove(home[0] + ":\\")
    rts.append(home)
    
    write_config('indexPaths', rts)
    return rts

def getFilesIndexFile():
    filePath = os.path.join(getIndexPath(), getFileIndexName())

def getClasses():
    filePath = 'classes'

    if not os.path.isfile(filePath):
        return []

    f = open(filePath, 'r')
    file = f.readlines()
    f.close()
    
    classes = ['.f']

    for f in file:
        classes.append(f.split()[0])
        
    return classes

def hidden(dir):
    useless = get_config('exclusions')
    
    if useless == None:
        useless = []
    
    if dir.lower() in useless or dir[0].lower() in useless:
        return True

    return False

def getResultsLimit():
    return 4

def getAdvancedScore():
    return 8

def getMatchScore():
    return 1

def getSpaceScore():
    return 10

def getStartScore():
    return 10

def getEndScore():
    return 10

def getSubstringScore():
    return 13

def getFullStringScore():
    return 15

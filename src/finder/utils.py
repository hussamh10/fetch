from item import File
from item import Folder
import os
import constants

def readFile(file_name):
    return open(file_name, 'r').readlines()

def initFilesList(file_names, path):
    files = dict.fromkeys(file_names)

    for name in file_names:
        file_name = os.path.join(path, name)

        file = readFile(file_name)
        temp = []
        for p in file:
            temp.append(File(p))

        files[name] = temp

    return files

def initFoldersList(file_name):
    file = readFile(file_name)
    folders = []
    for path in file:
        folders.append(Folder(path))

    return folders

def sort(f):
    return sorted(f, key=lambda x: x.score, reverse=True)

def printItems(f):
    limit = constants.getResultsLimit()
    temp = f[:limit]
    for t in temp:
        print(t.name + "|" + t.path[:-1])

def deleteAndRename(homeDir, old, new):
    oldFile = os.path.join(homeDir, old)
    newFile = os.path.join(homeDir, new)


    if os.path.exists(oldFile):
        os.remove(oldFile)

    os.rename(newFile, oldFile)

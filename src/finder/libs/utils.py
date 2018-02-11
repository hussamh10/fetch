from libs.folder import Folder
from libs.file import File
import os

def getFuzzyFolder():
    return '\\fuzzy-data\\'

def readFile(file_name):
    return open(file_name, 'r').readlines()

def initFilesList(file_names, path):
    
    files = dict.fromkeys(file_names)

    for name in file_names:
        file_name = os.path.join(path, name)
        print(file_name)

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
    temp = f[:10]
    for t in temp:
        print(t.name[:-1] + "|" + t.path[:-1])

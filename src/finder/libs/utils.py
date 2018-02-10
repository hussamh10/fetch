from libs.folder import Folder
import os

def getFuzzyFolder():
    return '\\fuzzy-data\\'

def readFile(file_name):
    return open(file_name, 'r').readlines()

def initFoldersList(file_name):
    file = readFile(file_name)
    folders = []
    for path in file:
        folders.append(Folder(path))

    return folders

def sort(f):
    return sorted(f, key=lambda x: x.score, reverse=True)

def printFolders(f):
    temp = f[:10]
    for t in temp:
        print(t.name[:-1] + "|" + t.path[:-1])

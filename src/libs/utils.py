from libs.folder import Folder
import os
import re

def generateBasicRegex(q):
    q = q.split(' ')[-1]
    basic_ptrn = '.*'.join(q)
    return re.compile(basic_ptrn)

def generateSubRegex(q):
    q = q.split(' ')[-1]
    return re.compile(q)

def generateSpaceRegex(q):
    q = q.split()
    q = q[0]
    return re.compile(q)

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

def foldersString(f):
    temp = f[0:3]
    string = ''
    for t in temp:
        string = string + (t.path) + '\n'
    return string

def printFolders(f):
    temp = f[:]
    for t in temp:
        print(t.name[:-1] + "|" + t.path[:-1] + str(t.score))

def openFolder(folder):
    os.system('explorer \"' + folder.path + '\"')

from libs.folder import Folder
import libs.scores as scores
import os
import re

def generatePreficRegex(q):
    q = ".*" + q + ".*"
    return re.compile(q)

def generateBasicRegex(q):
    q = q.split(' ')[-1]
    basic_ptrn = '.*'.join(q)
    return re.compile(basic_ptrn)

def generateSubRegex(q):
    q = q.split(' ')[-1]
    return re.compile(q)

def generateSpaceRegex(q):
    if ' ' not in q:
        return None

    q = q.split(' ')[:-1]
    q = ' '.join(q)
    q = '.*'.join(q)
    q = q.replace(' ', '\\\\*')
    return re.compile(q)

def readFile(file_name):
    return open(file_name, 'r').readlines()

def initFoldersList(file_name):
    file = readFile(file_name)
    folders = []
    for path in file:
        folders.append(Folder(path))

    return folders

def initFoldersListWithPrefix(file_name, prefix):
    prefix_rx = generatePreficRegex(prefix)
    folders = initFoldersList(file_name)
    folders = scores.getCandidateFolders(prefix_rx, folders)
    return folders

def sort(f):
    return sorted(f, key=lambda x: x.score, reverse=True)

def foldersString(f):
    temp = f[:10]
    string = ''
    for t in temp:
        string = string + (t.path) + '\n'
    return string

def printFolders(f):
    temp = f[:10]
    for t in temp:
        print(t.name[:-1] + "|" + t.path[:-1])

def openFolder(folder):
    os.system('explorer \"' + folder.path + '\"')

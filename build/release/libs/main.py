import os
import sys
import re
from msvcrt import getch

from folder import Folder
from index import index


def getScore(basic_rx, sub_rx, string):
    match_basic = basic_rx.search(string)
    match_sub = sub_rx.search(string)

    if match_basic == None:
        return 0
    
    score = 1
    
    if match_basic.start() == 0:
        score += 10

    if match_basic.end() == len(match_basic.string):
        score += 10

    match_substring = sub_rx.search(string)

    if not match_substring == None:
        score += 20

    return score


def generateBasicRegex(q):
    basic_ptrn = '.*'.join(q)
    return re.compile(basic_ptrn)

def generateSubRegex(q):
    return re.compile(q)

def readFile(file_name):
    return open(file_name, 'r').readlines()

def createFolders(file_name):
    file = readFile(file_name)
    folders = []
    for path in file:
        folders.append(Folder(path))
        
    return folders

def updateScores(basic_rx, sub_rx, folders):
    temp = []
    for folder in folders:
        folder.score = getScore(basic_rx, sub_rx, folder.name.lower())

        if folder.score > 0:
            temp.append(folder)

    return temp

def sort(f):
    return sorted(f, key=lambda x: x.score, reverse=True)

def printFolders(f):
    temp = f[0:3]
    for t in temp:
        print(t.path)
    print()

def getQuery(args):
    q = ''
    for arg in args[1:]:
        q += arg + '\\'

    return q[:-1]


def advancedScoreUpdate(q, folders):

    j = 0
    for f in folders:
        w = ""
        for i in re.split(' |\.|-|_', f.name.lower()):
            if len(i) > 0:
                w += i[0]
        for c in q:
            if c in w:
                folders[j].score = folders[j].score + 10
                #print(c, w, f.name)
        folders[j].score = folders[j].score - len(re.split(' |\.|-|_', f.name.lower()))
    
        j += 1   



    return folders
        

def fuzzOnce(q):
    rootDir = os.environ["HOMEPATH"]
    indexFilePath = os.path.join(rootDir, 'indexed')

    if not os.path.isfile(indexFilePath):
        print("Indexing, this only happens the first time, kindly repot back the number below (when it appears")
        index()

    folders = createFolders(indexFilePath)
    b_rx = generateBasicRegex(q)
    s_rx = generateSubRegex(q)
    folders = updateScores(b_rx, s_rx, folders)
    folders = advancedScoreUpdate(q, folders)
    folders = sort(folders)
    folder = folders[0]
    print(folder.path)
    return folder.path


def openFolder(folder):
    os.system('explorer \"' + folder.path + '\"')

def main():
    q = getQuery(sys.argv)
    folder = fuzzOnce(q)
    openFolder(folder)


def fuzz():
    rootDir = os.environ["HOMEPATH"]
    indexFilePath = os.path.join(rootDir, 'indexed')

    if not os.path.isfile(indexFilePath):
        print("Indexing, this only happens the first time, kindly repot back the number below (when it appears")
        index()

    folders = createFolders(indexFilePath)

    exit = False
    q = ''
    while not exit:
        char = chr(ord(getch()))
        q = q + char
        basic_rx = generateBasicRegex(q)
        sub_rx = generateSubRegex(q)

        if char == '\r':
            folder = folders[0]
            print(folder.path)
            openFolder(folder)
            exit = True


        folders = updateScores(basic_rx, sub_rx, folders)
        folders = advancedScoreUpdate(q, folders)
        folders = sort(folders)

        printFolders(folders)


fuzz()

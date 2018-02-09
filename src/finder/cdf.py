import os
import sys

import subprocess
from libs.index import index
import libs.scores as scores
import libs.utils as utils
from msvcrt import getch


def start(path):
    subprocess.call(['explorer.py', path])

def printFolders(folders, cd):
    i = 1
    prefix = os.getcwd().lower()
    for f in folders:
        print(str(i) + '> ' + f.name[:-1] + ' - ' + f.path.lower().replace(cd, '.'))
        i+=1

def main():
    q = sys.argv[1:]
    q = " ".join(q)

    if not q:
        print('Enter a query after cdf.')
        return

    rootDir = os.environ["HOMEDRIVE"] + os.environ["HOMEPATH"]
    indexFilePath = os.path.join(rootDir, 'indexed')
    
    cd = os.getcwd().lower()
    prefix = cd.replace("\\", "\\\\")

    folders = utils.initFoldersListWithPrefix(indexFilePath, prefix)

    basic_rx = utils.generateBasicRegex(q)
    sub_rx = utils.generateSubRegex(q)
    space_rx = utils.generateSpaceRegex(q)

    folders = scores.updateScores(basic_rx, sub_rx, folders) # chooses that match post space q
    folders = scores.advancedScoreUpdate(q, folders) # no idea what this does
    folders = scores.spaceScores(q, space_rx, folders) # chooses only paths that match pre space q
    folders = utils.sort(folders)

    printFolders(folders, cd)

    exit = False

    if len(folders) == 0:
        return

    while(not exit):
        num = ord(getch()) - 48
        if (num > 0 and num <= len(folders) and num < 10):
            start(folders[num-1].path)
            exit = True

        elif (num + 48) == 27:
            exit = True


main()

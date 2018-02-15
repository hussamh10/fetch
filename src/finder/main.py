import os
from index import index
from setup import createClassesFile
import scores
import utils
import regex
import error
import constants

def fuzzy_file(q, classes, files):
    print("files")

    while q not in classes:
        q = input().lower()
        if(len(q) == 0):
            return


    old_len = 0
    curr_len = 0

    cl = q
    candidates = files[cl][:]
    q = input().lower()
    if q[-1] != ' ':
        print("No such class")

    while True:
        q = input().lower()
        print(":" + q)

        old_len = curr_len
        curr_len = len(q)

        if curr_len <= 1:
            return

        if not old_len < curr_len or q.count(' ') > 1:
            candidates = files[cl][:]

        print(q.replace(cl+' ', '', 1))
        candidates = fuzz(q.replace(cl, '', 1), candidates)
        utils.printItems(candidates)
        return

def fuzzy_dir(q, folders):
    old_len = 0
    curr_len = 0
    candidates = folders[:]
    print(":" + q)

    while True:
        old_len = curr_len
        curr_len = len(q)

        if (curr_len == 0):
            return

        if(not old_len < curr_len or ' ' in q):
            candidates = folders[:]

        candidates = fuzz(q, candidates)
        utils.printItems(candidates)

        q = input().lower()
        print(":" + q)

def fuzz(q, index):
    basic_rx = regex.generateBasicRegex(q)
    sub_rx = regex.generateSubRegex(q)
    space_rx = regex.generateSpaceRegex(q)

    index = scores.spaceScores(q, space_rx, index) # chooses only paths that match pre space q
    index = scores.updateScores(basic_rx, sub_rx, index) # chooses that match post space q
    index = scores.advancedScoreUpdate(q, index) # no idea what this does
    index = utils.sort(index)

    return index

def init():
    indexDir = constants.getIndexPath()
    indexFilePath = os.path.join(indexDir, constants.getDirIndexName())

    if not os.path.isfile(indexFilePath):
        createClassesFile()
        index()

    classes = constants.getClasses()
    
    if not classes:
        error.noClasses()
        return

    
    for c in classes:
        if not os.path.isfile(os.path.join(indexDir, c)):
            index()

    print(":indexed")

    folders = utils.initFoldersList(indexFilePath)
    files = utils.initFilesList(classes, indexDir)
    
    return folders, files

def main():
    folders, files = init()
    candidates = folders
    classes = constants.getClasses()
    q = ''
    while(True):
        q = input().lower()
        if '.' in q:
            fuzzy_file(q, classes, files)
        else:
            fuzzy_dir(q, candidates)

main()

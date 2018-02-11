import os
import index
import scores
import utils
import regex
import constants

def fuzzy_file(classes, files):

    q = ''
    while q not in classes:
        q = input()

    print("ol")

    while True:
        index = files[q]
        q = input()
        index = fuzz(q, index)
        utils.printItems(index)

        return

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
    classes = constants.getClasses()

    if not os.path.isfile(indexFilePath):
        index()

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

    old_len = 0
    curr_len = 0

    classes = constants.getClasses()
    
    fuzzy_file(classes, files)

    while q != 'dir':
        fuzzy_file(q, classes, files)
        q = input()

    while True:
        q = input().lower()
        print(":" + q)

        old_len = curr_len
        curr_len = len(q)

        if(not old_len < curr_len or ' ' in q):
            candidates = folders

        candidates = fuzz(q, candidates)
        utils.printItems(candidates)
main()

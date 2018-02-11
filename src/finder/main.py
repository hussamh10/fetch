import os
from libs.index import index
import libs.scores as scores
import libs.utils as utils
import libs.regex as regex

def fuzzy_file(q, classes, files):

    if q not in classes:
        return

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

def main():
    indexDir = os.environ["LOCALAPPDATA"] + utils.getFuzzyFolder()
    indexFilePath = os.path.join(indexDir, 'dir_indexed')

    classes = ['code', 'pic']


    if not os.path.isfile(indexFilePath):
        index()

    for c in classes:
        if not os.path.isfile(os.path.join(indexDir, c)):
            index()


    print(":indexed")
    
    old_len = 0
    curr_len = 0

    folders = utils.initFoldersList(indexFilePath)
    files = utils.initFilesList(['code', 'pic'], indexDir)

    q = input()

    while q != 'dir':
        fuzzy_file(q, classes, files)
        q = input()

    while True:
        result = 'None'

        q = input().lower()
        print(":" + q)

        old_len = curr_len
        curr_len = len(q)

        if(not old_len < curr_len or ' ' in q):
            folders = utils.initFoldersList(indexFilePath)

        folders = fuzz(q, folders)

        utils.printItems(folders)

main()

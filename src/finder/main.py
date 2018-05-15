import os
from index import index
from setup import createClassesFile
import scores
import utils
import regex
import error
import constants

def inputs(q):
    return input()

def updateFolders():
    indexDir = constants.getIndexPath()
    indexFilePath = os.path.join(indexDir, constants.getDirIndexName())
    folders = utils.initFoldersList(indexFilePath)
    return folders


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

def file_fuzzy(string, classes, files):
    string = string.split()

    cl = string[0]

    if cl not in classes:
        return files

    if len(string) == 1:
        return files

    q = ' '.join(string[1:])
    candidates = files[cl]

    results = fuzz(q, candidates)
    utils.printItems(results)

    files[cl] = results

    return files

def folder_fuzzy(q, folders):
    candidates = folders

    candidates = fuzz(q, candidates)
    utils.printItems(candidates)

    return candidates

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
    folders, files = init()
    classes = constants.getClasses()
    
    candidate_files = dict(files)
    candidate_folders = folders[:]

    q = ''
    old_len = 0
    curr_len = 0

    while(True):
        q = inputs(q).lower()
        print(":" + q)

        old_len = curr_len
        curr_len = len(q)

        if curr_len < old_len or ' ' in q:
            candidate_folders = updateFolders()
            candidate_files = dict(files)

        if curr_len > 0 and '.' == q[0]:
            pass
            #candidate_files = file_fuzzy(q, classes, candidate_files)

        if curr_len > 0 and not '.' == q[0]:
            candidate_folders = folders[:]
            candidate_folders = folder_fuzzy(q, candidate_folders)

main()

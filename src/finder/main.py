import os
from libs.index import index
import libs.scores as scores
import libs.utils as utils

def main():
    rootDir = os.environ["HOMEPATH"]
    indexFilePath = os.path.join(rootDir, 'indexed')

    if not os.path.isfile(indexFilePath):
        index()

    print(":indexed")
    
    old_len = 0
    curr_len = 0

    folders = utils.initFoldersList(indexFilePath)

    while True:
        result = 'None'

        q = input().lower()
        print(":")
        
        old_len = curr_len
        curr_len = len(q)

        if(not old_len < curr_len or ' ' in q):
            folders = utils.initFoldersList(indexFilePath)

        basic_rx = utils.generateBasicRegex(q)
        sub_rx = utils.generateSubRegex(q)
        space_rx = utils.generateSpaceRegex(q)

        folders = scores.updateScores(basic_rx, sub_rx, folders) # chooses that match post space q
        folders = scores.advancedScoreUpdate(q, folders) # no idea what this does
        folders = scores.spaceScores(q, space_rx, folders) # chooses only paths that match pre space q
        folders = utils.sort(folders)

        utils.printFolders(folders)
        result = utils.foldersString(folders)

main()

import os
import sys

from libs.index import index
import libs.scores as scores
import libs.utils as utils
        
def main():
    q = sys.argv[1:]
    q = " ".join(q)

    if not q:
        print('Enter a query after cdf.')
        return

    rootDir = os.environ["HOMEPATH"]
    indexFilePath = os.path.join(rootDir, 'indexed')
    
    prefix = os.getcwd().lower()
    prefix = prefix.replace("\\", "\\\\")

    folders = utils.initFoldersListWithPrefix(indexFilePath, prefix)

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

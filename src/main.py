import os
from msvcrt import getch
from libs.index import index
import libs.scores as scores
import libs.utils as utils

def main():
    rootDir = os.environ["HOMEPATH"]
    indexFilePath = os.path.join(rootDir, 'indexed')

    if not os.path.isfile(indexFilePath):
        print("Indexing, this only happens the first time, kindly repot back the number below (when it appears")
        index()

    folders = utils.initFoldersList(indexFilePath)

    exit = False
    q = ''
    result = 'None'

    while not exit:
        print(":")
        char = chr(ord(getch()))
        q = q + char
        basic_rx = utils.generateBasicRegex(q)
        sub_rx = utils.generateSubRegex(q)

        '''
        if char == '\r':
            if len(folders) == 0:
                print("Folder not found")
            else:
                folder = folders[0]
                utils.openFolder(folder)
            exit = True
        '''

        folders = scores.updateScores(basic_rx, sub_rx, folders)
        folders = scores.advancedScoreUpdate(q, folders)
        folders = utils.sort(folders)

        utils.printFolders(folders)
        result = utils.foldersString(folders)

main()

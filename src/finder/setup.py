import constants
import os

def createClassesFile():
    file = open('classes', 'r')
    lines = file.readlines()
    file.close()

    file = open(constants.getIndexPath() + 'classes', 'w')

    for f in lines:
        file.write(f)
    
    return

def createHiddenFile():
    file = open('hidden', 'r')
    lines = file.readlines()
    file.close()

    file = open(constants.getIndexPath() + 'hidden', 'w')

    for f in lines:
        file.write(f)
    
    return

if __name__ == '__main__':
    createClassesFile()
    createHiddenFile()

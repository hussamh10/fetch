import constants
import os

def createClassesFile():
    file = open('classes', 'r')
    lines = file.readlines()
    file.close()

    file = open(os.path.join(constants.getIndexPath(), 'classes'), 'w')

    for f in lines:
        file.write(f)
    
    return

def setup():
    createClassesFile()

if __name__ == '__main__':
    setup()

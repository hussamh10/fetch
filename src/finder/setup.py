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
        

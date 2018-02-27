import re 
import libs.constants as constants

def updateScores(basic_rx, sub_rx, folders):
    temp = []
    for folder in folders:
        folder.score = getScore(basic_rx, sub_rx, folder.name.lower())
        #folder.score += getScore(basic_rx, sub_rx, folder.name.lower()) removed for better results on all match

        if folder.score > 0:
            temp.append(folder)

    return temp

def advancedScoreUpdate(q, folders):
    j = 0
    for f in folders:
        w = ""
        for i in re.split(' |\.|-|_', f.name.lower()):
            if len(i) > 0:
                w += i[0]
        for c in q:
            if c in w:
                folders[j].score = folders[j].score + constants.getAdvancedScore()
        folders[j].score = folders[j].score - len(re.split(' |\.|-|_', f.name.lower()))
    
        j += 1   

    return folders

def spaceScores(q, r, folders):
    if ' ' not in q:
        return folders
    
    temp = []

    for folder in folders:
        string = folder.path.split('\\')[:-1]
        string = ':'.join(string)

        match = r.search(string.lower())

        if not match == None:
            folder.score += constants.getSpaceScore()
            temp.append(folder)

    return temp


def getScore(basic_rx, sub_rx, string):
    match_basic = basic_rx.search(string)
    match_sub = sub_rx.search(string)

    if match_basic == None:
        return 0
    
    score = constants.getMatchScore()
   
    if match_basic.start() == 0:
        score += constants.getStartScore()

    if match_basic.end() == len(match_basic.string):
        score += constants.getEndScore()

    match_substring = sub_rx.search(string)

    if not match_substring == None:
        score += constants.getSubstringScore()

    return score

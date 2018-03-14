import re 
from sys import platform as plt
import constants

def updateScores(basic_rx, sub_rx, index):
    temp = []
    for item in index:
        item.score = getScore(basic_rx, sub_rx, item.name.lower())
        if item.score > 0:
            temp.append(item)
    return temp

def advancedScoreUpdate(q, index):
    j = 0
    for f in index:
        w = ""
        for i in re.split(' |\.|-|_', f.name.lower()):
            if len(i) > 0:
                w += i[0]
        for c in q:
            if c in w:
                index[j].score = index[j].score + constants.getAdvancedScore()

        index[j].score = index[j].score - len(re.split(' |\.|-|_', f.name.lower()))
        j += 1   

    return index

def spaceScores(q, r, index):
    if ' ' not in q:
        return index
    
    temp = []
    for item in index:
        if 'linux' in plt:
            string = item.path.split('/')[:-1]
        else:
            string = item.path.split('\\')[:-1]
        string = ':'.join(string)

        match = r.search(string.lower())

        if not match == None:
            item.score += constants.getSpaceScore()
            temp.append(item)

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
        if match_substring.span() == (0, len(match_substring.string)):
            score += constants.getFullStringScore()

    return score

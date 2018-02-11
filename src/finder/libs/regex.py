import re

def generateBasicRegex(q):
    q = q.split(' ')[-1]
    basic_ptrn = '.*'.join(q)
    return re.compile(basic_ptrn)

def generateSubRegex(q):
    q = q.split(' ')[-1]
    return re.compile(q)

def generateSpaceRegex(q):
    if ' ' not in q:
        return None

    q = q.split(' ')[:-1]
    q = ' '.join(q)
    q = '.*'.join(q)
    q = q.replace(' ', '\\\\*')
    return re.compile(q)

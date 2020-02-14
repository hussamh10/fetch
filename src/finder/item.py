from sys import platform as plt

class Folder():
    def __init__(self, path):
        if 'linux' in plt:
            self.name = path.split('/')[-1][:-1]
        else:
            self.name = path.split('\\')[-1][:-1]

        self.path = path
        self.score = 0

class File():
    def __init__(self, path):
        if 'linux' in plt:
            name = path.split('/')[-1]
        else:
            name = path.split('\\')[-1]
        name = name.split('.')
        self.name = name[0]
        self.ext = name[-1]
        self.path = path
        self.score = 0

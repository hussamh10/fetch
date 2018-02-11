class Folder():
    def __init__(self, path):
        self.name = path.split('\\')[-1]
        self.path = path
        self.score = 0

class File():
    def __init__(self, path):
        name = path.split('\\')[-1]
        name = name.split('.')

        self.name = name[0]
        self.ext = name[-1]
        self.path = path
        self.score = 0

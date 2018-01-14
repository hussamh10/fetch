class Folder():
    def __init__(self, path):
        self.name = path.split('\\')[-1]
        self.path = path
        self.score = 0

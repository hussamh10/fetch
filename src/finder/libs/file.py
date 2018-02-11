class File():
    def __init__(self, path):
        self.name = path.split('\\')[-1]
        self.path = path.replace(name, '')
        self.score = 0

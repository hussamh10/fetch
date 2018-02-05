import os

def hidden(dir):
    useless = ['.', 'lib', 'bin', 'envs', 'include', 'pkgs', 'build', 'sdk', 'appdata', 'notmnist_large', 'notmnist_small', 'RTTTL']
    if dir.lower() in useless or dir[0].lower() in useless:
        return True
    return False

def index():

    rootDir = os.environ["HOMEPATH"]
    indexFilePath = os.path.join(rootDir, 'indexed')
    file = open(indexFilePath, 'w')

    for dirName, subdirList, fileList in os.walk(rootDir):
        file.write(dirName + '\n')

        for dir in subdirList[:]:
            if hidden(dir):
                subdirList.remove(dir)

if __name__ == '__main__':
    index()

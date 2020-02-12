import json
import os
from sys import platform as plt

def get_config_path():
    if "linux" in plt:
        root = os.environ["HOME"]
        folder_name = '.fetch'
    else:
        root = os.environ["LOCALAPPDATA"]
        folder_name = 'Fetch'
        
    return os.path.join(root, folder_name, 'config.json')

def get_config(key):
    if not os.path.exists(get_config_path()):
        return None
    with open(get_config_path()) as f:
        config = json.load(f)
        return config.get(key, None)
    	
def write_config(key, value):
    with open(get_config_path()) as f:
        config = json.load(f)
        config[key] = value
    	
    with open(get_config_path(), 'w') as f:
        json.dump(config, f)

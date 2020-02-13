import json
import os
import constants

def get_config_path():
    return os.path.join(constants.getIndexPath(), 'config.json')

def get_config(key):
    try:
        with open(get_config_path()) as f:
            config = json.load(f)
            return config.get(key, None)
    except:
        return None

def write_config(key, value):
    try:
        # overwrite previous config
        with open(get_config_path()) as f:
            config = json.load(f)
            config[key] = value
    except:
        # couldn't open previous config, create new
        config = {key: value}
    finally:
        with open(get_config_path(), 'w') as f:
            json.dump(config, f)

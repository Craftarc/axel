import json
import os

app_root = os.environ['APP_ROOT']
CONFIG_PATH = 'config/config.json'

class DotDict:
    def __init__(self, dictionary):
        self.__dict__ = {k: DotDict(v) if isinstance(v, dict) else v
                         for k, v in dictionary.items()}

    def __getattr__(self, item):
        return self.__dict__.get(item, None)

def read_json_file(file_path):
    with open(file_path, 'r') as file:
        return json.load(file)

# Load the configuration when this module is imported
config = DotDict(read_json_file(os.path.join(app_root, CONFIG_PATH)))

import re
import urllib.parse


def is_valid_base64url(string):
    pattern = re.compile('[A-Za-z0-9_-]{43}')  # 43 base64url characters
    return pattern.fullmatch(string)


def add_query_parameters(url, parameters):
    return url + '?' + urllib.parse.urlencode(parameters)

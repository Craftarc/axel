import re
import urllib.parse
import textwrap
import requests


def is_valid_base64url(string):
    pattern = re.compile('[A-Za-z0-9_-]{43}')  # 43 base64url characters
    return pattern.fullmatch(string)


def add_query_parameters(url, parameters):
    return url + '?' + urllib.parse.urlencode(parameters)


def print_roundtrip(response, *args, **kwargs):
    def format_headers(d): return '\n'.join(f'{k}: {v}' for k, v in d.items())
    print(textwrap.dedent('''
        ---------------- request ----------------
        {req.method} {req.url}
        {reqhdrs}

        {req.body}
        ---------------- response ----------------
        {res.status_code} {res.reason} {res.url}
        {reshdrs}

        {res.text}
    ''').format(
        req=response.request,
        res=response,
        reqhdrs=format_headers(response.request.headers),
        reshdrs=format_headers(response.headers),
    ))

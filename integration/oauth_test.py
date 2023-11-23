import requests
import pytest
import os
import subprocess
import time
import logging
import urllib.parse
import server

from util import util
from util.config import config


# Logger setup
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)


def test_base_domain():
    """
    OK response from base domain (no trailing slash)
    """
    response = requests.get('https://staging.pathofaxel.com')
    assert (response.status_code == 200)


def test_root():
    """
    OK response from root path ('/')
    """
    response = requests.get('https://staging.pathofaxel.com/')
    assert (response.status_code == 200)


def test_auth():
    """
    Check response from Axel's '/auth' endpoint
    Expected to be a redirect to an auth server endpoint
    Redrect is not actually carried out
    """
    response = requests.get(
        'https://staging.pathofaxel.com/auth', allow_redirects=False)

    # the response is a redirect
    assert (response.status_code == requests.codes.found)

    # Headers contain session_id cookie
    cookies = response.cookies.get_dict(path='/auth')
    assert ('session_id' in cookies)
    session_id = cookies['session_id']
    assert (len(session_id) == 43)  # Axel always produces 43 byte session ids

    # Headers contain 'Location', the redirect url
    headers = response.headers
    assert ('Location' in headers)
    redirect_url = headers['Location']

    # Check url sections
    redirect_url_params = urllib.parse.urlparse(redirect_url)
    # Path on GGG auth server
    assert (redirect_url_params.path == '/oauth/authorize')

    # Check query string
    # returns key:value pair dicts, values are LIST
    query_string_params = urllib.parse.parse_qs(redirect_url_params.query,
                                                keep_blank_values=True,
                                                strict_parsing=True)

    assert ('client_id' in query_string_params)
    client_id = query_string_params['client_id']
    assert (len(client_id) == 1)  # only one element
    assert (client_id[0] == 'axel')

    assert ('response_type' in query_string_params)
    response_type = query_string_params['response_type']
    assert (len(response_type) == 1)
    assert (response_type[0] == 'code')

    assert ('scope' in query_string_params)
    scope = query_string_params['scope']
    assert (len(scope) == 1)
    # Spaces are important
    assert (scope[0] == ('account:profile '
                         'account:characters '
                         'account:stashes '
                         'account:league_accounts '
                         'account:item_filter'))

    assert ('state' in query_string_params)
    state = query_string_params['state']
    assert (util.is_valid_base64url(state[0]))

    assert ('challenge' in query_string_params)
    code_challenge = query_string_params['challenge']
    assert (len(code_challenge) == 1)
    assert (util.is_valid_base64url(code_challenge[0]))

    assert ('code_challenge_method' in query_string_params)
    code_challenge_method = query_string_params['code_challenge_method']
    assert (len(code_challenge_method) == 1)
    assert (code_challenge_method[0] == 'S256')

    assert ('redirect_uri' in query_string_params)
    callback_url = query_string_params['redirect_uri']
    assert (len(callback_url) == 1)
    # Callback path on Axel
    assert (urllib.parse.urlparse(callback_url[0]).path == '/auth/callback')

    # Only now, follow the redirect to GGG's auth server
    # GGG will redirect user back to Axel's callback url - this time we follow it
    # Response refers to that of GET(axel's callback url)
    redirect_response = requests.get(
        redirect_url, cookies=cookies)

    assert (redirect_response.status_code == requests.codes.ok)

    # The redirect url is correct, as per what is registered with GGG
    redirect_response_headers = redirect_response.headers

    # Check the axel session id sent back
    redirect_cookies = redirect_response.cookies
    assert (util.is_valid_base64url(redirect_cookies.get('axel_session_id')))


def test_auth_callback_path_invalid_session_id():
    """
    Rejects the access if the session id is invalid
    """
    response = requests.get('https://staging.pathofaxel.com/auth/callback',
                            cookies={'session_id': 'invalid_session_id'})
    assert (response.status_code == requests.codes.forbidden)


def test_auth_callback_path_no_state_in_query_params():
    """
    Rejects if there is no 'state' query parameter
    """
    response = requests.get('https://staging.pathofaxel.com/auth/callback',
                            params={'code': 'code'})
    assert (response.status_code == requests.codes.forbidden)


def test_auth_callback_path_no_code_in_query_params():
    """
    Rejects if there is no 'code' query parameter
    """
    response = requests.get('https://staging.pathofaxel.com/auth/callback',
                            params={'state': 'state'})
    assert (response.status_code == requests.codes.forbidden)

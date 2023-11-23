import logging
import requests
from flask import Flask, request
from util import util
from util.config import config

app = Flask(__name__)
logging.basicConfig(level=logging.INFO)
logger = logging.getLogger(__name__)


@app.route('/oauth/authorize', methods=['GET'])
def oauth_authorize():
    """ 
    Extracts 'state' and 'challenge' query parameters, then sends the
    user back to Axel's callback url.
    """

    # Store in server for later verification
    code_challenge = request.args.get('challenge')
    callback_url = 'https://' + config.auth.host.axel.staging + \
        config.auth.endpoint.redirect

    authorization_code = 'code'
    state = request.args.get('state')
    final_callback_url = util.add_query_parameters(callback_url,
                                                   {'code': authorization_code,
                                                    'state': state})

    return app.redirect(final_callback_url, code=requests.codes.found)


@app.route('/update', methods=['GET'])
def update():
    """
    Provides update JSON information back to the player
    """
    pass


def print_response(response):
    logger.info('Response headers')
    for header, value in response.headers.items():
        print(f"{header}: {value}")

    return response


app.after_request(print_response)


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=8080, debug=True)

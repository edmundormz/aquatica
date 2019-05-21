"""Services to handle a external database connection"""

from __future__ import print_function

import requests


class Database(object):

    def __init__(self, user_id):
        """Set the api-endpoint

        :param user_id: the userID to check in the external database
        """
        # api-endpoint
        self.api_endpoint = 'http://10.144.33.79/checkStringID/{}'.format(user_id)

    def get_response(self):
        """Get response from external server

        The external server can returns the following values:
        - String:true: => when a userID exists in the database and has access
        - String:false => when a userID exists in the database and has not access
        - String:No    => when a userID is not in database

        :return:
            - a dictionary that contains if the connection was established or not and the response received from
              the external server
        """
        timeout = 3  # timeout to close connection with the database (unit:seconds)

        try:
            # sending get request and saving the response as response object
            response = requests.get(url=self.api_endpoint, timeout=timeout)
            data = {'status': 'connected', 'response': 'No'} if response.text == 'No' \
                else {'status': 'connected', 'response': response.json()}
            return data  # returns a boolean
        except requests.ConnectTimeout as err:
            print('(error) - failed to connect to external database due to: {}'.format(err))
            return {'status': 'disconnected', 'response': False}


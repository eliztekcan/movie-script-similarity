import requests
import json

def get_movie_genre(movie_name, api_key='192bd2fc'):
    url = 'http://www.omdbapi.com/'

    full_url = url + '?apikey=' + api_key + '&t=' + movie_name
    result = requests.get(full_url).json()
    if 'Genre' not in result:
        return ['Genre not found.']
    genres =  [genre.strip() for genre in result['Genre'].split(',')]
    return genres 


import requests
import json

def get_movie_genres(movie_name, api_key):
    url = 'http://www.omdbapi.com/'

    full_url = url + '?apikey=' + api_key + '&t=' + movie_name
    result = requests.get(full_url).json()
    genres =  [genre.strip() for genre in result['Genre'].split(',')]
    return genres 

api_key = '192bd2fc'
print(get_movie_genres('Affliction', api_key))
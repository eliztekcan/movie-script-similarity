import string

from nltk.corpus import stopwords
from nltk.tokenize import word_tokenize
from stemming.porter2 import stem
import os
import pickle
from nltk.util import ngrams
import re

def filter_annotations(scriptString):
    s = scriptString.replace('VOICE', '')
    s = s.replace('(O.S.)', '')
    s = s.replace('NORM', '')
    s = s.replace('(V.O.)', '')
    s = s.replace('CUT TO:', '')
    return s

def filter_stopwords(script_string):
    extra_stopwords = ['\'ve', '...', '--', '\'s', '\'m', '\'d', 'n\'t', '\'\'']
    stop_words = set(stopwords.words('english'))

    for stopword in extra_stopwords:
        stop_words.add(stopword)
    script_tokens = word_tokenize(script_string.lower())
    filtered_script = []

    for w in script_tokens:
        if w not in stop_words and w not in string.punctuation:
            filtered_script.append(w)

    return filtered_script

def stem_script(script_tokens):
    stemmed_script = []
    for w in script_tokens:
        stemmed_script.append(stem(w))
    return stemmed_script


def get_data(path):
    files = os.listdir(path)
    movie_tokens = dict()
    for filename in files:
        movie_name = filename[7:-4]
        with open(os.path.join(path,filename), 'r') as myfile:
            data = myfile.read().replace('\n', '')

            data = filter_annotations(data)
            data = filter_stopwords(data)
            data = stem_script(data)
            movie_tokens[movie_name] = data
            print(len(data))

    return movie_tokens

def get_ngrams(path):
    files = os.listdir(path)
    movie_ngrams = dict()
    for filename in files:
        movie_name = filename[7:-4]
        with open(os.path.join(path,filename), 'r') as myfile:
            data = myfile.read().lower()
            data = re.sub('[' + string.punctuation + ']+', '', data)
            data = re.sub('[\s]+',' ', data)
            data = data.strip()
            movie_ngrams[movie_name] = list(ngrams(data,9))

    return movie_ngrams

path = '/Users/eliztekcan/PycharmProjects/WebScale/venv/resources'
#movie_tokens = get_data(path)
#print(movie_tokens)
movie_ngrams = get_ngrams(path)

print(movie_ngrams)
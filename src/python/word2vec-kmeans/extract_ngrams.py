from modify_ngrams import modify_ngrams
from nltk.corpus import stopwords
from nltk import ngrams
import os,re 
import string

def get_words(base_path, filename):
    print("inside get_words")
    a = set(get_ngrams(base_path, filename, 1))
    return list(a)

def get_ngram_lists(base_path, modified_movies, modification_rate, N=3):
    print("inside get_ngram_lists")
    filenames = os.listdir(base_path)

    ngram_lists = []
    for count, filename in enumerate(filenames):
        if count in modified_movies:
            ngram_lists.append(get_ngrams(base_path, filename, N, modification_rate))
        ngram_lists.append(get_ngrams(base_path, filename, N))

    return ngram_lists

def get_ngrams(base_path, filename, N=3, modification_rate=0):
    print("inside get_ngrams")
    with open(os.path.join(base_path, filename), 'r') as myfile:
        data = myfile.read().lower()
        data = re.sub('[' + string.punctuation + ']+', '', data)
        data = re.sub('[\s]+',' ', data)
        data = data.strip().split(' ')

        stop_words = set(stopwords.words('english'))

        sw_removed = []
        for word in data:
            if word not in stop_words:
                sw_removed.append(word)

        data = sw_removed     

        ngram_tuples = list(ngrams(data, N))
        movie_ngrams = []
        for ngram_tuple in ngram_tuples:
            movie_ngrams.append(" ".join([i for i in ngram_tuple]))

        if modification_rate != 0:
            return modify_ngrams(modification_rate, movie_ngrams)

    return movie_ngrams
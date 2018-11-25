from extract_ngrams import get_ngram_lists
import os, pickle

def create_vocab(movie_ngram_lists):
    print("inside create_vocab")
    vocabulary = set()
    for ngram_list in movie_ngram_lists:
        for ngram in ngram_list:
            vocabulary.add(ngram)

    with open('vocabulary.pkl', 'wb') as pickle_file:
        pickle.dump(vocabulary, pickle_file)
    
    print("vocabulary length = ", len(vocabulary))
    return list(vocabulary)
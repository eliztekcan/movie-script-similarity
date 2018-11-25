import os
from create_vocab import create_vocab
from find_similarity import * 
from extract_ngrams import get_ngram_lists
from preprocess import get_movie_names

def test_similarity():
    current_path = os.path.dirname(os.path.abspath(__file__))
    base_path = os.path.join(current_path, 'resources')
    modified_movies = [3]
    movie_names = get_movie_names(base_path, modified_movies)

    for modification_rate in range(0, 100, 5):   
        movie_ngram_lists = get_ngram_lists(base_path, modified_movies, modification_rate/100)
        movie_ngram_sets = [set(ngram_list) for ngram_list in movie_ngram_lists]
        vocabulary = create_vocab(movie_ngram_lists)
        movie_vectors = hash_ngrams(movie_ngram_sets, vocabulary)
        signature_matrix = min_hash_vectors(movie_vectors, len(vocabulary))
        for bands in [10, 25, 50, 100]:
            similars = lsh(signature_matrix, movie_names, bands)
            print('modification rate = ', modification_rate)
            print('bands = ', bands)
            for similar in similars:
                print('similar=', similar)

test_similarity()
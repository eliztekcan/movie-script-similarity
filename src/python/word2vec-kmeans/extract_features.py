from load_wordvectors import load_wordvectors
from extract_ngrams import get_words
from cluster import find_summary_words, cluster_movies, plot_predictions
from preprocess import get_movie_names
from get_genres import get_movie_genre
import os

def movie_wordvectors(words, model_dict):
    print("inside movie_wordvectors")
    word_vectors = []
    valid_words = set()
    for word in words:
        if word in model_dict:
            valid_words.add(word)
            word_vectors.append(model_dict[word])

    return word_vectors, valid_words

def test_clustering():
    current_path = os.path.dirname(os.path.abspath(__file__))
    resources_path = os.path.join(current_path, 'resources')
    movie_names = get_movie_names(resources_path, modified_movies = [])
    movie_genres = [get_movie_genre(movie_name) for movie_name in movie_names]

    filenames = os.listdir(resources_path)
    model, model_dict = load_wordvectors(os.path.join(current_path, 'model.pkl'), 'wiki.en.vec')
    summary_words_vectors = []
    for count, filename in enumerate(filenames):
        words = get_words(resources_path, filename)
        word_vectors, valid_words = movie_wordvectors(words, model_dict)
        summary_words_vector = find_summary_words(word_vectors, model, valid_words)
        summary_words_vectors.append(summary_words_vector)

    centers, predictions = cluster_movies(summary_words_vectors)

    for count in range(len(filenames)):
        print("---------------------")
        print("Name:", movie_names[count])
        print("Genre:", movie_genres[count])
        print("Prediction:", predictions[count])
    
    plot_predictions(predictions, summary_words_vectors, movie_names, movie_genres)

test_clustering()
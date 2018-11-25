import os, pickle
import numpy as np
import random

def hash_ngrams(movie_ngram_sets, vocabulary):
    print("inside hash_ngrams")
    movie_vectors = []
    for movie_ngram_set in movie_ngram_sets:
        movie_vector = []
        count = 0 
        for ngram in vocabulary:
            if ngram in movie_ngram_set:
                movie_vector.append(count)
            count = count + 1
        movie_vectors.append(movie_vector)
    
    return movie_vectors

def min_hash_vectors(movie_vectors, vocab_length, permutation_count=100):
    print("inside min_hash_vectors")
    prime = 10000007
    upper_bound = 32768

    permutations = []
    for i in range(permutation_count):
        a = random.randint(1, upper_bound)
        b = random.randint(1, upper_bound)

        permutation = []
        for row in range(0, vocab_length):
            permutation.append(((a + b*row)%prime)%vocab_length)
        permutations.append(permutation)
    
    signature_matrix = np.matrix(np.ones((permutation_count, len(movie_vectors))) * np.inf)

    for i, movie_vector in enumerate(movie_vectors):
        for j, permutation in enumerate(permutations):
            for element in movie_vector:
                if signature_matrix[j, i] > permutation[element]:
                    signature_matrix[j, i] = permutation[element]
    
    return signature_matrix

def lsh(signature_matrix, movie_names, bands):
    print("inside lsh")
    permutation_count = signature_matrix.shape[0]
    movie_count = signature_matrix.shape[1]
    rows = (int)(permutation_count/bands) # integers in a band

    similars = set()
    for i in range(bands):
        band_signatures = []
        for k in range(movie_count):
            band_signature = []
            for j in range(i*rows, (i+1)*rows): # o banddeki tum rowlar icin
                band_signature.append(signature_matrix[j, k])
            band_signatures.append(band_signature)

        for k in range(movie_count):
            for j in range(k+1, movie_count):
                if band_signatures[k] == band_signatures[j]:
                    similars.add((movie_names[j], movie_names[k]))

    return similars            
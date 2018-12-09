from gensim.models import KeyedVectors
import os, pickle
import numpy as np

def load_wordvectors(pickle_path, vectors_path):
    print("inside load_wordvectors")
    if os.path.isfile(pickle_path):
        with open(pickle_path, 'rb') as pickle_file: 
            model = pickle.load(pickle_file)
            model_dict = {}
            for word in model.vocab:
                model_dict[str(word)] = list(model[word].tolist())
            return model, model_dict
        

    else:
        model = KeyedVectors.load_word2vec_format(vectors_path, limit=100000)
        model_dict = {}
        for word in model.vocab:
            model_dict[str(word)] = list(model[word].tolist())
        with open(pickle_path, 'wb') as pickle_file:
            pickle.dump(model, pickle_file)
            return model, model_dict

 

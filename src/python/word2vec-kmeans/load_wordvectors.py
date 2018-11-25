from gensim.models import KeyedVectors
import os, pickle
import numpy as np

def load_wordvectors(pickle_path, vectors_path):
   if os.path.isfile(pickle_path):
      with open(pickle_path, 'rb') as pickle_file:
      	return pickle.load(pickle_file)     
   else:
      model = KeyedVectors.load_word2vec_format(vectors_path)
      with open(pickle_path, 'wb') as pickle_file:
      	pickle.dump(model, pickle_file)
      	return model

pickle_file_path = 'pkl/word2vec.pkl'
vec_file_path = 'wiki.xmf.vec'

model = load_wordvectors(pickle_file_path, vec_file_path) 

minn = np.inf
maxx = -np.inf
 

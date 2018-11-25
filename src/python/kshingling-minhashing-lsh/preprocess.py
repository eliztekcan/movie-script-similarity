import string
import os

def get_movie_names(base_path, modified_movies):
    print("inside get_movie_names.")
    filenames = os.listdir(base_path)
    movie_names = []
    for count, filename in enumerate(filenames):
        if count in modified_movies:
            movie_names.append(filename[7:-4] + '_modified')
        movie_names.append(filename[7:-4])
    return movie_names
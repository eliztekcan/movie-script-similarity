"""
TODO burda dursun belki ise yara
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

"""
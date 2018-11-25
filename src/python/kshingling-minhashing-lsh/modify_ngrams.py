import random

def modify(ngram):
    modified_ngram = ""
    alphabet = ["a","b","c","d","e","f","g","h","i","j","k","l","m","n","o","p","q","r","s","t","u","v","w","x","v","z"]
    for i in range(len(ngram)):
        modified_ngram += alphabet[random.randrange(0, len(alphabet))]
    return modified_ngram

def modify_ngrams(modification_rate, ngrams):
    print("inside modify_ngrams")
    ngrams_count = len(ngrams)
    modification_count = int(ngrams_count * modification_rate)
    modified_indices = set()

    for i in range(modification_count):
        rand = random.randrange(0, ngrams_count)
        while rand in modified_indices:
            rand = random.randrange(0, ngrams_count)
        modified_indices.add(rand)

    modified_ngrams = ngrams

    for index, ngram in enumerate(modified_indices):
        modified_ngrams[index] = modify(ngrams[index])

    return modified_ngrams

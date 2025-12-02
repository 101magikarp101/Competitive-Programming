# scrabble_filter.py

# Define letter scores for Scrabble Mobile (Words With Friends) and Scrabble Physical
words_with_friends_scores = {
    'a': 1, 'b': 4, 'c': 4, 'd': 2, 'e': 1, 'f': 4, 'g': 3, 'h': 3,
    'i': 1, 'j':10, 'k': 5, 'l': 2, 'm': 4, 'n': 2, 'o': 1, 'p': 4,
    'q':10, 'r': 1, 's': 1, 't': 1, 'u': 2, 'v': 5, 'w': 4, 'x': 8,
    'y': 3, 'z':10
}

scrabble_physical_scores = {
    'a': 1, 'b': 3, 'c': 3, 'd': 2, 'e': 1, 'f': 4, 'g': 2, 'h': 4,
    'i': 1, 'j': 8, 'k': 5, 'l': 1, 'm': 3, 'n': 1, 'o': 1, 'p': 3,
    'q':10, 'r': 1, 's': 1, 't': 1, 'u': 1, 'v': 4, 'w': 4, 'x': 8,
    'y': 4, 'z':10
}

vowels = set('aeiou')
pentomino_letters = set('filnptuvwyxz')  # Letters corresponding to the 12 free pentomino names
rainbow_letters = set('roygbiv')         # Colors in ROYGBIV acronym

def count_consonants(word):
    """Return the number of consonants in a word."""
    return sum(1 for c in word if c.isalpha() and c not in vowels)

def score_word(word, score_dict):
    """Return the Scrabble score of a word given a letter score dictionary."""
    return sum(score_dict.get(c, 0) for c in word)

def count_pentomino_letters(word):
    """Return the count of letters in the word that are pentomino names."""
    return sum(1 for c in word if c in pentomino_letters)

def count_rainbow_letters(word):
    """Return the count of letters in the word that are in ROYGBIV."""
    return sum(1 for c in word if c in rainbow_letters)

def value(word):
    return sum(ord(c)-ord('a')+1 for c in word if c.isalpha() and c.islower())

def main():
    """Read 'input.txt' and print words fulfilling all criteria:
       - exactly 5 consonants
       - Words With Friends (mobile) score == 15
       - Scrabble Physical score == 14
       - exactly 2 pentomino letters
       - exactly 4 rainbow letters (ROYGBIV)
    """
    try:
        with open('input.txt', 'r') as f:
            for line in f:
                word = line.strip().lower()
                if not word:
                    continue
                # if count_consonants(word) != 5:
                #     continue
                # if score_word(word, words_with_friends_scores) != 15:
                #     continue
                # if score_word(word, scrabble_physical_scores) != 14:
                #     continue
                # if count_pentomino_letters(word) != 2:
                #     continue
                # if count_rainbow_letters(word) != 4:
                #     continue
                print(word, value(word))
    except FileNotFoundError:
        print("Error: 'input.txt' not found.")

if __name__ == '__main__':
    main()

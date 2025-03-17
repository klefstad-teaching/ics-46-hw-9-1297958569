#include "ladder.h"

int main() {
    set<string> word_list;
    load_words(word_list, "words.txt");
    
  
    
    string start_word, end_word;
    
    cout << "Enter the start word: ";
    cin >> start_word;

    for (char& c : start_word) {
        c = tolower(c);
    }
    
    cout << "Enter the end word: ";
    cin >> end_word;
    
    for (char& c : end_word) {
        c = tolower(c);
    }
  
    if (start_word == end_word) {
        error(start_word, end_word, "Start and end words are the same!");
        return 1;
    }
    
   
    if (word_list.find(end_word) == word_list.end()) {
        error(start_word, end_word, "End word is not in the dictionary!");
        return 1;
    }
    
   
    vector<string> ladder = generate_word_ladder(start_word, end_word, word_list);
    
    
    print_word_ladder(ladder);
    
    return 0;
}
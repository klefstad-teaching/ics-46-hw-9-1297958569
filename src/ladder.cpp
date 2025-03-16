#include "ladder.h"

void error(string word1, string word2, string msg) {
    cout << "Error: " << msg << endl;
    cout << "Word 1: " << word1 << endl; 
    cout << "Word 2: " << word2 << endl;
}

bool edit_distance_within(const string& str1, const string& str2, int d) {
    // Quick length check
    int len1 = str1.length();
    int len2 = str2.length();
    if (abs(len1 - len2) > d) {
        return false;
    }
    
    // If one string is empty, return true if other string's length <= d
    if (len1 == 0) return len2 <= d;
    if (len2 == 0) return len1 <= d;
    
    // Use smaller array to save memory
    vector<int> prev(len2 + 1);
    vector<int> curr(len2 + 1);
    
    // Initialize first row
    for (int j = 0; j <= len2; j++) {
        prev[j] = j;
    }
    
    // Fill the matrix
    for (int i = 1; i <= len1; i++) {
        curr[0] = i;
        
        for (int j = 1; j <= len2; j++) {
            if (tolower(str1[i-1]) == tolower(str2[j-1])) {
                curr[j] = prev[j-1];
            } else {
                curr[j] = 1 + min({prev[j],     // deletion
                                  curr[j-1],     // insertion
                                  prev[j-1]});   // substitution
            }
        }
        
        // Early exit if we can't get edit distance <= d
        if (*min_element(curr.begin(), curr.end()) > d) {
            return false;
        }
        
        prev = curr;
    }
    
    return curr[len2] <= d;
}

bool is_adjacent(const string& word1, const string& word2) {
    // Quick length check before doing edit distance
    int len_diff = abs((int)word1.length() - (int)word2.length());
    if (len_diff > 1) {
        return false;
    }
    return edit_distance_within(word1, word2, 1);
}

void load_words(set<string>& word_list, const string& file_name) {
    ifstream file(file_name);
    if (!file) {
        throw runtime_error("Could not open dictionary file");
    }
    
    string word;
    while (getline(file, word)) {
        if (!word.empty()) {
            // Convert to lowercase and add to set
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            word_list.insert(word);
        }
    }
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    // Convert input words to lowercase for comparison
    string start = begin_word;
    string end = end_word;
    transform(start.begin(), start.end(), start.begin(), ::tolower);
    transform(end.begin(), end.end(), end.begin(), ::tolower);
    
    // Initialize queue with start word
    queue<vector<string>> ladder_queue;
    ladder_queue.push({start});
    
    // Keep track of visited words
    set<string> visited;
    visited.insert(start);
    
    const int MAX_LADDER_LENGTH = 100;  // Reasonable limit to prevent infinite loops
    
    // BFS
    while (!ladder_queue.empty()) {
        vector<string> current_ladder = ladder_queue.front();
        ladder_queue.pop();
        
        // Skip if ladder is too long
        if (current_ladder.size() >= MAX_LADDER_LENGTH) {
            continue;
        }
        
        string last_word = current_ladder.back();
        
        // Check each word in dictionary
        for (const string& word : word_list) {
            // Skip if word already visited or not adjacent
            if (visited.find(word) != visited.end() || !is_adjacent(last_word, word)) {
                continue;
            }
            
            vector<string> new_ladder = current_ladder;
            new_ladder.push_back(word);
            
            if (word == end) {
                return new_ladder;
            }
            
            visited.insert(word);
            ladder_queue.push(new_ladder);
        }
    }
    
    return vector<string>();
}

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << endl;
        return;
    }
    
    cout << "Word ladder found: ";
    for (size_t i = 0; i < ladder.size(); i++) {
        cout << ladder[i];
        cout << " ";  // Add space after every word, including the last one
    }
    cout << endl;
}

void verify_word_ladder() {
    string start_word, end_word;
    
    cout << "Enter start word: ";
    cin >> start_word;
    if (start_word.empty()) {
        cout << "Error: Start word cannot be empty" << endl;
        return;
    }
    
    cout << "Enter end word: ";
    cin >> end_word;
    if (end_word.empty()) {
        cout << "Error: End word cannot be empty" << endl;
        return;
    }
    
    // Convert both words to lowercase for comparison
    transform(start_word.begin(), start_word.end(), start_word.begin(), ::tolower);
    transform(end_word.begin(), end_word.end(), end_word.begin(), ::tolower);
    
    // Check if words are the same
    if (start_word == end_word) {
        error(start_word, end_word, "Start and end words must be different");
        return;
    }
    
    // Load dictionary
    set<string> dictionary;
    try {
        load_words(dictionary, "words.txt");
        if (dictionary.empty()) {
            cout << "Error: Dictionary is empty" << endl;
            return;
        }
    } catch (const runtime_error& e) {
        cout << "Error: " << e.what() << endl;
        return;
    }
    
    // Verify end word is in dictionary
    if (dictionary.find(end_word) == dictionary.end()) {
        error(start_word, end_word, "End word not found in dictionary");
        return;
    }
    
    // Generate and print ladder
    vector<string> ladder = generate_word_ladder(start_word, end_word, dictionary);
    print_word_ladder(ladder);  // Use the print_word_ladder function instead of custom printing
}

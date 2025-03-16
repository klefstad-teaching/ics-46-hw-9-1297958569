#include "ladder.h"

void error(string word1, string word2, string msg) {
    cout << "Error: " << msg << endl;
    cout << "Word 1: " << word1 << endl; 
    cout << "Word 2: " << word2 << endl;
}

bool edit_distance_within(const string& str1, const string& str2, int d) {
    if (abs((int)str1.length() - (int)str2.length()) > d) return false;
    
    vector<vector<int>> dp(str1.length() + 1, vector<int>(str2.length() + 1));
    
    for (size_t i = 0; i <= str1.length(); i++)
        dp[i][0] = i;
    for (size_t j = 0; j <= str2.length(); j++)
        dp[0][j] = j;
        
    for (size_t i = 1; i <= str1.length(); i++) {
        for (size_t j = 1; j <= str2.length(); j++) {
            if (tolower(str1[i-1]) == tolower(str2[j-1]))
                dp[i][j] = dp[i-1][j-1];
            else
                dp[i][j] = 1 + min({dp[i-1][j], dp[i][j-1], dp[i-1][j-1]});
        }
    }
    return dp[str1.length()][str2.length()] <= d;
}

bool is_adjacent(const string& word1, const string& word2) {
    return edit_distance_within(word1, word2, 1);
}

vector<string> generate_neighbors(const string& word) {
    vector<string> neighbors;
    const string letters = "abcdefghijklmnopqrstuvwxyz";

    // Try changing each letter
    for (size_t i = 0; i < word.size(); ++i) {
        for (char c : letters) {
            if (c == word[i]) continue;
            string neighbor = word;
            neighbor[i] = c;
            neighbors.push_back(neighbor);
        }
    }

    // Try inserting a letter
    for (size_t i = 0; i <= word.size(); ++i) {
        for (char c : letters) {
            neighbors.push_back(word.substr(0, i) + c + word.substr(i));
        }
    }

    // Try deleting a letter
    for (size_t i = 0; i < word.size(); ++i) {
        neighbors.push_back(word.substr(0, i) + word.substr(i + 1));
    }

    return neighbors;
}

void load_words(set<string>& word_list, const string& file_name) {
    ifstream file(file_name);
    if (!file) {
        throw runtime_error("Could not open dictionary file");
    }
    
    string word;
    while (getline(file, word)) {
        if (!word.empty()) {
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
    
    if (start == end) return {};
    
    if (word_list.find(end) == word_list.end()) {
        return {};
    }
    
    queue<vector<string>> ladder_queue;
    set<string> visited;
    
    ladder_queue.push({start});
    visited.insert(start);
    
    while (!ladder_queue.empty()) {
        vector<string> current_ladder = ladder_queue.front();
        ladder_queue.pop();
        
        string last_word = current_ladder.back();
        
        // First check if any word in dictionary is adjacent
        for (const string& word : word_list) {
            if (visited.find(word) != visited.end()) continue;
            
            if (is_adjacent(last_word, word)) {
                vector<string> new_ladder = current_ladder;
                new_ladder.push_back(word);
                
                if (word == end) {
                    return new_ladder;
                }
                
                visited.insert(word);
                ladder_queue.push(new_ladder);
            }
        }
    }
    
    return {};  // No ladder found
}

void print_word_ladder(const vector<string>& ladder) {
    if (ladder.empty()) {
        cout << "No word ladder found." << endl;
        return;
    }
    
    cout << "Word ladder found:";
    for (const auto& word : ladder) {
        cout << " " << word;
    }
    cout << " \n";
}

void verify_word_ladder() {
    string start_word, end_word;
    
    cout << "Enter start word: ";
    cin >> start_word;
    
    cout << "Enter end word: ";
    cin >> end_word;
    
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
    print_word_ladder(ladder);
}

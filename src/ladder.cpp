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

    // 尝试改变每个字母
    for (size_t i = 0; i < word.size(); ++i) {
        for (char c : letters) {
            if (c == word[i]) continue;
            string neighbor = word;
            neighbor[i] = c;
            neighbors.push_back(neighbor);
        }
    }

    // 尝试插入一个字母
    for (size_t i = 0; i <= word.size(); ++i) {
        for (char c : letters) {
            neighbors.push_back(word.substr(0, i) + c + word.substr(i));
        }
    }

    // 尝试删除一字母
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
        // 删除所有尾随空格或换行符
        word.erase(word.find_last_not_of(" \n\r\t") + 1);
        if (!word.empty()) {
            // 转换为小写
            transform(word.begin(), word.end(), word.begin(), ::tolower);
            word_list.insert(word);
        }
    }
}

vector<string> generate_word_ladder(const string& begin_word, const string& end_word, const set<string>& word_list) {
    // 将输入的单词转换为小写以便进行比较
    string start = begin_word;
    string end = end_word;
    transform(start.begin(), start.end(), start.begin(), ::tolower);
    transform(end.begin(), end.end(), end.begin(), ::tolower);
    
    if (start == end) return {};
    
    if (word_list.find(end) == word_list.end()) {
        return {};
    }
    
    // 使用优先级队列确保最短路径
    auto compare = [](const vector<string>& a, const vector<string>& b) {
        if (a.size() != b.size()) {
            return a.size() > b.size();
        }
        return a.back() > b.back();
    };
    
    priority_queue<vector<string>, vector<vector<string>>, decltype(compare)> ladder_queue(compare);
    set<string> visited;
    
    ladder_queue.push({start});
    visited.insert(start);
    
    const int MAX_DEPTH = 20;
    
    while (!ladder_queue.empty()) {
        vector<string> current_ladder = ladder_queue.top();
        ladder_queue.pop();
        
        if (current_ladder.size() >= MAX_DEPTH) continue;
        
        string last_word = current_ladder.back();
        
        if (last_word == end) {
            return current_ladder;
        }
        
        // 尝试改变一个字母
        for (size_t i = 0; i < last_word.length(); i++) {
            string new_word = last_word;
            for (char c = 'a'; c <= 'z'; c++) {
                new_word[i] = c;
                if (word_list.find(new_word) != word_list.end() && visited.find(new_word) == visited.end()) {
                    visited.insert(new_word);
                    vector<string> new_ladder = current_ladder;
                    new_ladder.push_back(new_word);
                    ladder_queue.push(new_ladder);
                }
            }
        }
        
        // 尝试插入一个字母
        for (size_t i = 0; i <= last_word.length(); i++) {
            for (char c = 'a'; c <= 'z'; c++) {
                string new_word = last_word;
                new_word.insert(i, 1, c);
                if (word_list.find(new_word) != word_list.end() && visited.find(new_word) == visited.end()) {
                    visited.insert(new_word);
                    vector<string> new_ladder = current_ladder;
                    new_ladder.push_back(new_word);
                    ladder_queue.push(new_ladder);
                }
            }
        }
        
        // 尝试删除一个字母
        for (size_t i = 0; i < last_word.length(); i++) {
            string new_word = last_word;
            new_word.erase(i, 1);
            if (word_list.find(new_word) != word_list.end() && visited.find(new_word) == visited.end()) {
                visited.insert(new_word);
                vector<string> new_ladder = current_ladder;
                new_ladder.push_back(new_word);
                ladder_queue.push(new_ladder);
            }
        }
    }
    
    return {}; 
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
    cout << " \n";  // 换行符前的空格与预期输出相匹配
}

void verify_word_ladder() {
    string start_word, end_word;
    
    cout << "Enter start word: ";
    cin >> start_word;
    
    cout << "Enter end word: ";
    cin >> end_word;
    
    // 将两个单词都转换为小写以便进行比较
    transform(start_word.begin(), start_word.end(), start_word.begin(), ::tolower);
    transform(end_word.begin(), end_word.end(), end_word.begin(), ::tolower);
    
    // 检查单词是否相同
    if (start_word == end_word) {
        error(start_word, end_word, "Start and end words must be different");
        return;
    }
    
    // 加载词典
    set<string> dictionary;
    try {
        load_words(dictionary, "words.txt");
    } catch (const runtime_error& e) {
        cout << "Error: " << e.what() << endl;
        return;
    }
    
    // 验证结束词是否在词典中
    if (dictionary.find(end_word) == dictionary.end()) {
        error(start_word, end_word, "End word not found in dictionary");
        return;
    }
    
    // 生成并打印梯子
    vector<string> ladder = generate_word_ladder(start_word, end_word, dictionary);
    print_word_ladder(ladder);
}

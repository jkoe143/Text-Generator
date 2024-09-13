#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <ctime>
#include <cctype>  
#include <sstream> 
using namespace std;


int randomInteger(int low, int high) {
    static bool initialized = false;
    if (!initialized) {
        srand(int(time(NULL)));
        rand();   
        initialized = true;
    }
    double d = rand() / (double(RAND_MAX) + 1);
    double s = d * (double(high) - low + 1);
    return int(floor(low + s));
}


void promptN(int &N){
    while(N <= 1){
        cout << "N must be > 1, try again: "; 
        cin >> N; 
    }
}


void promptTotalWords(int &totalWords, int N){
    while(totalWords < N){
        cout << "Total words must be at least N, try again: ";
        cin >> totalWords;
    }
}


void promptInputFile(ifstream &inputFile, string &fileName){
    while(!inputFile){
        cout << "Invalid file, try again: "; 
        cin >> fileName;
        inputFile.open(fileName); 
    }
}


void addToMap(map<vector<string>, vector<string>> &nGramMap, vector<string> key, string word){
    if(nGramMap.find(key) != nGramMap.end()){ 
        nGramMap[key].push_back(word); 
    } else{
        nGramMap[key] = {word};
    }
}


void buildMap(ifstream &inputFile, map<vector<string>, vector<string>> &nGramMap, int N){
    vector<string> window; 
    vector<string> wrapArounds; 
    string word;

    while(inputFile >> word){ 
        if(window.size() < static_cast<size_t>(N - 1)){
            window.push_back(word); 
        } else{ 
            vector<string> key(window);
            addToMap(nGramMap, key, word); 
            window.push_back(word);
            window.erase(window.begin());
        }
        wrapArounds.push_back(word);
    }

    for(int i = 0; i < N - 1; i++){
        vector<string> key(window); 
        string suffix = wrapArounds[i]; 
        addToMap(nGramMap, key, suffix); 
        window.push_back(suffix); 
        window.erase(window.begin()); 
    }
}


void printMap(const map<vector<string>, vector<string>> &nGramMap){
    for(const auto& pair: nGramMap){
        cout << "{";
        for(const auto& prefix: pair.first){
            cout << prefix << " "; 
        }
        cout << "} -> {"; 
        for(const auto& suffix: pair.second){
            cout << suffix << " ";
        }
        cout << "}";
        cout << endl;
    }
}


void generateText(const map<vector<string>, vector<string>> &nGramMap, int totalWords, int N){
    int index = randomInteger(0, nGramMap.size() - 1); 
    vector<string> randomText;
    vector<string> slidingWindow; 
    for(const auto& pair: nGramMap){
        if(index == 0){
            slidingWindow = pair.first; 
            randomText = pair.first;  
            break;
        }
        index --;
    }

    for(int i= 0; i < totalWords - N + 1; i++){
        vector<string> suffixes = nGramMap.at(slidingWindow); 
        int suffixIndex = randomInteger(0, suffixes.size() - 1);
        randomText.push_back(suffixes[suffixIndex]);
        slidingWindow.erase(slidingWindow.begin());
        slidingWindow.push_back(suffixes[suffixIndex]);
    }
    cout << "...";
    for(const string& words: randomText){
        cout << words << " ";
    }
    cout << "..." << endl;
}


int main() {
    string fileName; 
    int N; 
    int totalWords; 
    map<vector<string>, vector<string>> nGramMap; 

    while(true){
        cout << "Welcome to the Text Generator." << endl;
        cout << "This program makes random text based on a document." << endl;
        cout << "Input file name? ";
        cin >> fileName;
        ifstream inputFile(fileName);
        promptInputFile(inputFile, fileName); 

        cout << "Value of N? ";
        cin >> N;
        promptN(N);

        cout << "Total words you'd like to generate? ";
        cin >> totalWords;
        promptTotalWords(totalWords, N);

        buildMap(inputFile, nGramMap, N);
        inputFile.close();

        char input = ' ';
        do {
            cout << "Type b-build map, p-print map, g-generate text, s-start over, x-to exit: ";
            cin >> input;
            cout << "\n\n";
            if(input == 'b'){
                cout << "...Building map: " << fileName << "...\n\n\n";
            } else if(input == 'p'){
                printMap(nGramMap);
                cout << "\n\n";
            } else if(input == 'g'){
                generateText(nGramMap, totalWords, N);
                cout << "\n\n";
            }
        } while(input != 'x' && input != 's');

        if(input == 'x'){ 
            break;
        }
    }
}

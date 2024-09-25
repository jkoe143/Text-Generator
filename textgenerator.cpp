#include <iostream>
#include <map>
#include <vector>
#include <fstream>
#include "myrandom.h"
using namespace std;

//receives reference to integer variable N
//asks user for the value of N ensuring it is above 1
//void function, returns nothing
void promptN(int &N){
    while(N <= 1){
        cout << "N must be > 1, try again: "; //prompts for user input
        cin >> N; //takes in input
    }
}


//prompts user for total words, making sure it is above N
//recieves reference to total words and integer N
//void, returns nothing
void promptTotalWords(int &totalWords, int N){
    while(totalWords < N){
        cout << "Total words must be at least N, try again: ";//prompts for input
        cin >> totalWords;//takes in input
    }
}


//prompts the user for the input file name, if invalid asks again
//recieves reference to ifstream inputFile and as well as filename
//void, returns nothing
void promptInputFile(ifstream &inputFile, string &fileName){
    while(!inputFile){
        cout << "Invalid file, try again: "; //prompts for file input
        cin >> fileName;
        inputFile.open(fileName); //attempts to open file
    }
}


//using the key, adds the corresponding word to the map
//recieves reference to nGramMap, vector key, and string word
//void, returns nothing
void addToMap(map<vector<string>, vector<string>> &nGramMap, vector<string> key, string word){
    if(nGramMap.find(key) != nGramMap.end()){ //checks if key already exists
        nGramMap[key].push_back(word); //if so, adds word to existing key
    } else{
        nGramMap[key] = {word};//else, creates new key with word
    }
}


// reads the input file and N to build the map of nGrams
// receives reference to nGramMap, ifstream inputFile, and integer N
// void, returns nothing
void buildMap(ifstream &inputFile, map<vector<string>, vector<string>> &nGramMap, int N){
    vector<string> window; //vector for N-gram window
    vector<string> wrapArounds; //vector for wrap-around processing
    string word; //variable that holds each words from file reading

    //reads each word from the file
    while(inputFile >> word){ 
        if(window.size() < static_cast<size_t>(N - 1)){
            window.push_back(word); //if window not full, word is added
        } else{ //if full, key is created from window and word is added as value
            vector<string> key(window);
            addToMap(nGramMap, key, word); //key and value pair added to the map
            window.push_back(word); //current word added to window
            window.erase(window.begin()); //first word removed from window
        }
        wrapArounds.push_back(word); //updates the word for later wraparound process
    }

    for(int i = 0; i < N - 1; i++){
        vector<string> key(window); //key is created from current window
        string suffix = wrapArounds[i]; 
        addToMap(nGramMap, key, suffix); //key, value pair added to map
        window.push_back(suffix); //the suffix is added
        window.erase(window.begin()); //first word removed from window
    }
}


//prints out the nGramMap contents in a specific format {} -> {}
//takes in a reference of nGramMap as parameter
//void, returns nothing
void printMap(const map<vector<string>, vector<string>> &nGramMap){
    //iterates over key-value pair in nGramMap
    for(const auto& pair: nGramMap){
        cout << "{";
        //prints the prefixes(keys)
        for(const auto& prefix: pair.first){
            cout << prefix << " "; //prints each word in prefix 
        }
        cout << "} -> {"; //arrow to indicate change from prefix to suffix
        //prints the suffixes(values)
        for(const auto& suffix: pair.second){
            cout << suffix << " ";
        }
        cout << "}";//marks the end of a pair 
        cout << endl;
    }
}


//generates random text using the nGram map
//takes in reference of nGramMap, integers totalWords and N
//returns void, so nothing
void generateText(const map<vector<string>, vector<string>> &nGramMap, int totalWords, int N){
    int index = randomInteger(0, nGramMap.size() - 1); //a random index is chosen using randomInteger()
    vector<string> randomText;//vector to store generated text
    vector<string> slidingWindow; //vector stores N-1 words before sliding
    //iteration over nGramMap to find N-gram with randomized index
    for(const auto& pair: nGramMap){
        if(index == 0){
            slidingWindow = pair.first; //sliding window is set with N-1 words
            randomText = pair.first;  //same words are added to randomText vector
            break;
        }
        index --;
    }

    //rest of text is generated
    for(int i= 0; i < totalWords - N + 1; i++){
        vector<string> suffixes = nGramMap.at(slidingWindow); //suffixes for the N-1 words are grabbed
        int suffixIndex = randomInteger(0, suffixes.size() - 1);//a random suffix is chosen 
        randomText.push_back(suffixes[suffixIndex]);//the random suffix is added to randomText
        slidingWindow.erase(slidingWindow.begin());//first word removed from sliding window
        slidingWindow.push_back(suffixes[suffixIndex]);//new suffix added to sliding window
    }
    //generated text is printed out
    cout << "...";
    for(const string& words: randomText){
        cout << words << " ";
    }
    cout << "..." << endl;
}


int main() {
    string fileName; //variable that store the file name
    int N; //variable for N value
    int totalWords; //variable for total words
    map<vector<string>, vector<string>> nGramMap; //map that stores N-gram prefixes and suffixes

    while(true){
        //shows welcome messages
        cout << "Welcome to the Text Generator." << endl;
        cout << "This program makes random text based on a document." << endl;
        cout << "Input file name? ";
        cin >> fileName;
        ifstream inputFile(fileName);
        promptInputFile(inputFile, fileName); //calls to check for valid file existance

        cout << "Value of N? ";
        cin >> N;
        promptN(N);//calls to check for valid N

        cout << "Total words you'd like to generate? ";
        cin >> totalWords;
        promptTotalWords(totalWords, N);//calls to check for valid total words

        buildMap(inputFile, nGramMap, N);//calls to build the N-gram Map
        inputFile.close();

        char input = ' ';
        //command menu options using the do while loop 
        do {
            cout << "Type b-build map, p-print map, g-generate text, s-start over, x-to exit: ";
            cin >> input;
            cout << "\n\n";
            if(input == 'b'){
                cout << "...Building map: " << fileName << "...\n\n\n";
            } else if(input == 'p'){
                printMap(nGramMap);//calls to print the map
                cout << "\n\n";
            } else if(input == 'g'){
                generateText(nGramMap, totalWords, N);//calls to generate random text
                cout << "\n\n";
            }
        } while(input != 'x' && input != 's');//repeats do-while until 'x' or 's' is entered

        if(input == 'x'){ //if 'x' is entered, the while loop ends
            break;
        }
    }
}


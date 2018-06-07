#include <iostream>
#include <string>
#include <locale>
#include <algorithm>
#include <curl/curl.h>
#include <unistd.h>
#include <vector>
#include <regex>
using namespace std;


// ============================================================================
// User defined classes.
// ============================================================================
#include "Colors.h"
#include "bst.h"
#include "SortedDoublyLinkedList.h"

// ============================================================================
// Global constants
// ============================================================================
const int TO_LOAD_1000 = 1000 * 1;
const string ALPHABET = "abcdefghijklmnopqrstuvwxyz";


// ============================================================================
// Function prototypes.
// ============================================================================
bool validateUserInput(BST t, string input);
SortedDoublyLinkedList<string> getDictionary(string letter);
SortedDoublyLinkedList<string> parseResult(string);
string getUrl(string letter);
string getWord(string);
string getDefinition(string);
bool validateChoice(char);


// ============================================================================
// Main.
// ============================================================================
int main(void) {
    cout << "Hello and welcome to the dictionary app!" << endl;
    cout << "In this app, you can view words and their definitions." << endl;
    cout << "The words are downloaded from the web, stored in " << endl;
    cout << "complex data structures (Hash tables, Trees) and may be " << endl;
    cout << "sorted in different ways for you to view." << endl << endl;

    cout << "This dictionary is version 1.0 – "
         << "Because the dictionary has a huge amount of words " << endl;
    cout << "and definitions, version 1.0 will contain only the letter A." << endl;
    cout << "In future versions, we will add letters accordingly." << endl << endl;
    cout << "Let's look up some words!" << endl << endl;;

    cout << FGREY_PURPLE << "Please make sure you have an internet connection"
         << " during the download process. It should be quick!" << RST << endl;

    cout << FBLACK_RED << "To conserve memory, the program will only load"
         << " the first " << TO_LOAD_1000 << " words." << RST << endl;


    cout << endl << endl;

    // Load the alphabet into a BST tree.
    BST tree;
    for (int i = 0; i < ALPHABET.length(); ++i) {
        string s = "";
        s.push_back(ALPHABET.at(i));
        tree.insert(s);
    }

    string userInput = " ";
    while (validateUserInput(tree, userInput) == false) {
        cout << "Please enter the letter of the words you would like to lookup (i.e. A, B, C): ";
        cin >> userInput;
    }

    cout << endl << endl << "Downloading words and definitions for the letter "
         << userInput << endl;

    SortedDoublyLinkedList<string> list = getDictionary(userInput);

    if (list.getTotal() > 0) {
        cout << "The dictionary is ready to view!" << endl;
    }

    string choice = " ";
    while (validateChoice(choice.at(0)) == false) {
        cout << "Please choose from the following menu: " << endl << endl;
        cout << "v)iew top N words in the list." << endl;
        cout << "s)earch for a word" << endl;
        cout << "q)uit dictionary" << endl << endl;

        cout << "Please enter a choice: ";
        cin >> choice;

        switch (choice.at(0)) {
            case 'v': {
                int count;
                cout << "Enter how many words you want to view: ";
                cin >> count;

                if (count < 0 || count > list.getSize()) {
                    cout << "Sorry we don't have enough words for that request." << endl;
                    break;
                }

                list.displayTop(count);
                break;
            }
            case 's': {
                string word;
                cout << "Enter the word: ";
                cin >> word;

                if (word == " " || word == "\n") {
                    cout << "Invalid word." << endl;
                    break;
                }

                string def = list.inList(word);
                cout << FBLACK_GREEN << def << RST << endl;
            }

            cin.get();
            break;
            case 'q':
                exit(1);
            default:
                std::cin.clear();
                break;
        }
    }

    return 0;
}

// ============================================================================
// Validates the user input.
// ============================================================================
bool validateChoice(char choice) {
    if (choice == ' ' || choice == '\n' || isalnum(choice) == false) return false;
    if (choice != 'v' || choice != 's' || choice != 'q') {
        cout << FBLACK_RED << "Invalid option, please try again..."
             << RST << endl << endl << endl;
        return false;
    }

    return true;
}

// ============================================================================
// Gets the URL for the desired dictionary.
// ============================================================================
string getUrl(string letter) {
    transform(letter.begin(), letter.end(), letter.begin(), ::tolower);
    
    // if (letter == "a") {
        return "http://m.uploadedit.com/bbtc/1528322039672.txt";
    // }

    // Add more if statements as the dictionary implementation grows.
}

// ============================================================================
// Writes downloaded data to a buffer.
// ============================================================================
size_t write_data(void *buffer, size_t size, size_t nmemb, void *param) {
  std::string& text = *static_cast<std::string*>(param);
  size_t totalsize = size * nmemb;
  text.append(static_cast<char*>(buffer), totalsize);

  return totalsize;
}

// ============================================================================
// Downloads the dictionary from the web.
// ============================================================================
SortedDoublyLinkedList<string> getDictionary(string letter) {
    string result;
    CURL *curl;
    CURLcode res;
    string letterURL = getUrl(letter);
    char url[letterURL.length()];
    strcpy(url, letterURL.c_str());

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_CAINFO, "./ca-bundle.crt");
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, false);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, false);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &result);

        res = curl_easy_perform(curl);
        if(CURLE_OK != res) {
            cout << "Something went wrong!" << endl;
            cout << "We couldn't fetch the dictionary from the web." << endl;
            cout << "Please check your internet connection, and try again!" << endl;
            exit(0);
        }

        curl_easy_cleanup(curl);
    }

    return parseResult(result);
}

// ============================================================================
// Parse result.
// ============================================================================
SortedDoublyLinkedList<string> parseResult(string result) {
    SortedDoublyLinkedList<string> temp;

    string line = "";
    for (int i = 0; i < result.length(); ++i) {
        if (result.at(i) == '\n') {
            string word = getWord(line);
            string definition = getDefinition(line);

            temp.addList(word, definition);
            if (temp.getSize() == TO_LOAD_1000) break;

            line = "";
            definition = "";
            word = "";
            i++;
            i++;
        }

        line.push_back(result.at(i));
    }

    return temp;
}

// ============================================================================
// Parses the word.
// ============================================================================
string getWord(string line) {
    string word = "";
    int i = 0;
    while (line.at(i) != '(') {
        i++;
    }

    word = line.substr(0, i);
    word.erase(std::remove(word.begin(), word.end(), '"'), word.end());
    word.erase(std::remove(word.begin(), word.end(), '\n'), word.end());
    word.erase(std::remove(word.begin(), word.end(), '('), word.end());
    word.erase(std::find_if(word.rbegin(), word.rend(), std::bind1st(std::not_equal_to<char>(), ' ')).base(), word.end());

    return word;
}

// ============================================================================
// Parses the definition.
// ============================================================================
string getDefinition(string line) {
    string def = "";
    int i = 0;
    while (line.at(i) != ')') {
        i++;
    }

    def = line.substr(i, line.length() - 1);
    def.erase(std::remove(def.begin(), def.end(), '"'), def.end());
    def.erase(std::remove(def.begin(), def.end(), ')'), def.end());

    return def;
}

// ============================================================================
// Validates user input.
// ============================================================================
bool validateUserInput(BST t, string input) {
    transform(input.begin(), input.end(), input.begin(), ::tolower);
    if (t.find(input) && input.at(0) == 'a') return true;

    cout << "Sorry, at this point we only have definitions for the letter A"
         << ", please try again." << endl;
    return false;
}
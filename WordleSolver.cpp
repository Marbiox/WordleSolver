#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <string>
#include <vector>

void addWord(std::map<int, char> &greenLetters,
             std::map<char, std::vector<int>> &yellowLetters,
             std::set<char> &greyLetters, std::map<char, int> &letterAmount);
void addYellowLetter(std::map<char, std::vector<int>> &yellowLetters,
                     std::map<char, int> &letterAmount);
void addGreenLetter(std::map<int, char> greenLetters,
                    std::map<char, int> letterAmount);
bool isValid(std::map<int, char> greenLetters,
             std::map<char, std::vector<int>> yellowLetters,
             std::set<char> greyLetters, std::map<char, int> letterAmount,
             std::string word);
void addToBestList(std::vector<std::pair<std::string, int>> &list,
                   std::pair<std::string, int>);
void printBestWords(std::map<std::string, int> words,
                    std::map<int, char> greenLetters,
                    std::map<char, std::vector<int>> yellowLetters,
                    std::set<char> greyLetters, 
                    std::map<char, int> letterAmount);
std::string findWord(std::map<std::string, int> words, std::set<char> letters);
void populateWords(std::map<std::string, int> &list);
void printMenu();
int getInt(int low, int high, std::string prompt, std::string errorMessage);
char getChar(std::string prompt, std::string errorMessage);

int main() {
  std::map<std::string, int> words;
  std::map<int, char> greenLetters;
  std::map<char, std::vector<int>> yellowLetters;
  std::set<char> greyLetters;
  std::map<char, int> letterAmount;
  populateWords(words);

  while (true) {
    printMenu();
    int menuIndex = getInt(1, 8, ">>", "INVALID");
    if (menuIndex == 1) {
      addWord(greenLetters, yellowLetters, greyLetters, letterAmount);
      printBestWords(words, greenLetters, yellowLetters, greyLetters, letterAmount);
    } else if (menuIndex == 2) {
      std::set<char> letters;
      std::string word;
      std::cout << "Enter Letters: ";
      getline(std::cin, word);
      for (int i = 0; i < word.length(); i++) {
        letters.insert(word[i]);
      }
      std::cout << findWord(words, letters) << std::endl;
    }
    if (menuIndex == 3) {
      char letter = getChar("Add Letter: ", "INVALID");
      greyLetters.insert(letter);
    } else if (menuIndex == 4) {
      addYellowLetter(yellowLetters, letterAmount);
    } else if (menuIndex == 5) {
      addGreenLetter(greenLetters, letterAmount);
    } else if (menuIndex == 6) {
      printBestWords(words, greenLetters, yellowLetters, greyLetters, letterAmount);
    } else if (menuIndex == 7) {
      for (auto itr = words.begin(); itr != words.end(); itr++) {
        std::string word = itr->first;
        if (isValid(greenLetters, yellowLetters, greyLetters, letterAmount,
                    word))
          std::cout << word << std::endl;
      }
    } else if (menuIndex == 8) {
      greenLetters.clear();
      yellowLetters.clear();
      greyLetters.clear();
      letterAmount.clear();
    }
  }

  return 0;
}

void addWord(std::map<int, char> &greenLetters,
             std::map<char, std::vector<int>> &yellowLetters,
             std::set<char> &greyLetters, std::map<char, int> &letterAmount) {
  std::string word;
  std::cout << "Enter the word: ";
  getline(std::cin, word);
  std::string colors;
  std::cout << "Enter colors (0 for grey, 1 for yellow, 2 for green): ";
  getline(std::cin, colors);
  std::map<char, int> wordLetterAmount;
  for (int i = 0; i < word.length(); i++) {
    if (colors[i] != '0')
      wordLetterAmount[word[i]] += 1;
  }
  for (auto itr = wordLetterAmount.begin(); itr != wordLetterAmount.end();
       itr++) {
    if (itr->second > letterAmount[itr->first])
      letterAmount[itr->first] = itr->second;
  }
  for (int i = 0; i < word.length(); i++) {
    if (colors[i] == '1') {
      char letter = word[i];
      int position = i;
      std::vector<int> letterPositions;
      if (yellowLetters.find(letter) != yellowLetters.end()) {
        letterPositions = yellowLetters[letter];
      }
      letterPositions.push_back(position);
      yellowLetters[letter] = letterPositions;
    } else if (colors[i] == '2') {
      greenLetters[i] = word[i];
    }
  }
  for (int i = 0; i < word.length(); i++) {
    if (colors[i] == '0' &&
        yellowLetters.find(word[i]) == yellowLetters.end()) {
      bool valid = true;
      for (auto itr = greenLetters.begin(); itr != greenLetters.end(); itr++) {
        if (itr->second == word[i]) {
          valid = false;
          break;
        }
      }
      if (valid)
        greyLetters.insert(word[i]);
    }
  }
}

void addYellowLetter(std::map<char, std::vector<int>> &yellowLetters,
                     std::map<char, int> &letterAmount) {
  char letter = getChar("Add Letter: ", "INVALID");
  int position = getInt(1, 5, "Which Position: ", "INVALID") - 1;
  std::vector<int> letterPositions;
  if (yellowLetters.find(letter) != yellowLetters.end()) {
    letterPositions = yellowLetters[letter];
  }
  letterPositions.push_back(position);
  yellowLetters[letter] = letterPositions;

  int amount = getInt(1, 5, "How many of that letter: ", "INVALID");
  letterAmount[letter] = amount;
}

void addGreenLetter(std::map<int, char> greenLetters,
                    std::map<char, int> letterAmount) {
  char letter = getChar("Add Letter: ", "INVALID");
  int position = getInt(1, 5, "Which Position: ", "INVALID") - 1;
  greenLetters[position] = letter;

  int amount = getInt(1, 5, "How many of that letter: ", "INVALID");
  letterAmount[letter] = amount;
}

bool isValid(std::map<int, char> greenLetters,
             std::map<char, std::vector<int>> yellowLetters,
             std::set<char> greyLetters, std::map<char, int> letterAmount,
             std::string word) {
  std::map<char, int> wordLetterAmount;
  for (int i = 0; i < word.length(); i++) {
    char letter = word[i];
    if (wordLetterAmount.find(letter) == wordLetterAmount.end())
      wordLetterAmount[letter] = 0;

    if (greyLetters.find(letter) != greyLetters.end()) {
      return false;
    }
    if (greenLetters.find(i) != greenLetters.end()) {
      if (greenLetters[i] != letter)
        return false;
      wordLetterAmount[letter] += 1;
    } else if (yellowLetters[letter].size() != 0) {
      std::vector<int> positions = yellowLetters[letter];
      int letterPosition = i;
      for (int position : positions) {
        if (letterPosition == position) {
          return false;
        }
      }
      wordLetterAmount[letter] += 1;
    }
  }
  for (std::map<char, int>::iterator itr = letterAmount.begin();
       itr != letterAmount.end(); itr++) {
    if (wordLetterAmount.find(itr->first) == wordLetterAmount.end() ||
        wordLetterAmount[itr->first] < itr->second)
      return false;
  }
  return true;
}

void addToBestList(std::vector<std::pair<std::string, int>> &list,
                   std::pair<std::string, int> word) {
  int index = -1;
  for (int i = 0; i < list.size(); i++) {
    if (word.second > list[i].second) {
      index = i;
      break;
    }
  }
  if (index == -1 && list.size() == 10)
    return;
  else if (list.size() < 10 && index == -1)
    list.push_back(word);
  else if (index != -1) {
    std::vector<std::pair<std::string, int>>::iterator itr = list.begin();
    list.insert(itr + index, word);
    if (list.size() > 10)
      list.pop_back();
  }
}

void printBestWords(std::map<std::string,int> words, 
                    std::map<int, char> greenLetters, 
                    std::map<char, std::vector<int>> yellowLetters, 
                    std::set<char> greyLetters, 
                    std::map<char, int> letterAmount) {
    std::vector<std::pair<std::string, int>> list;
    for (auto itr = words.begin(); itr != words.end(); itr++) {
      std::string word = itr->first;
      std::pair<std::string, int> wordPair;
      wordPair.first = word;
      wordPair.second = itr->second;
      if (isValid(greenLetters, yellowLetters, greyLetters, letterAmount,
                  word))
        addToBestList(list, wordPair);
    }
    for (std::pair<std::string, int> wordPair : list) {
      std::cout << wordPair.first << std::endl;
    }
  }

std::string findWord(std::map<std::string, int> words, std::set<char> letters) {
  for (auto itr = words.begin(); itr != words.end(); itr++) {
    std::set<char> letterList = letters;
    std::string word = itr->first;
    for (int i = 0; i < word.length(); i++) {
      if (letterList.find(word[i]) != letterList.end())
        letterList.erase(word[i]);
    }
    if (letterList.empty()) {
      return word;
    }
  }
  return "Not Found";
}

void populateWords(std::map<std::string, int> &list) {
  std::fstream input;
  std::map<char, std::map<int, int>> map;
  input.open("letterProbability.txt");
  while (true) {
    std::string line;
    getline(input, line);
    if (input.fail())
      break;
    char letter = line[0];
    int index = stoi(line.substr(1, 1));
    int value = stoi(line.substr(2));
    map[letter][index] = value;
  }
  input.close();
  input.clear();
  input.open("words.txt");
  while (true) {
    std::string line;
    getline(input, line);
    if (input.fail())
      break;
    std::set<char> letters;
    int points = 0;
    bool duplicate = false;
    for (int i = 0; i < line.length(); i++) {
      if (letters.find(line[i]) == letters.end())
        points += map[line[i]][i];
      else
        duplicate = true;
      letters.insert(line[i]);
    }
    if (duplicate)
      points /= 4;
    list.insert(std::pair<std::string, int>(line, points));
  }
}

void printMenu() {
  std::cout << "1. add word" << std::endl;
  std::cout << "2. get word with letters" << std::endl;
  std::cout << "3. add gray letter" << std::endl;
  std::cout << "4. add yellow letter" << std::endl;
  std::cout << "5. add green letter" << std::endl;
  std::cout << "6. print best words" << std::endl;
  std::cout << "7. print all words" << std::endl;
  std::cout << "8. reset" << std::endl;
}

int getInt(int low, int high, std::string prompt, std::string errorMessage) {
  while (true) {
    try {
      std::cout << prompt;
      std::string line;
      getline(std::cin, line);
      int num = stoi(line);
      if (num < low || num > high)
        throw std::invalid_argument("Not in range");
      return num;
    } catch (const std::exception &e) {
      std::cout << errorMessage << std::endl;
    }
  }
}

char getChar(std::string prompt, std::string errorMessage) {
  while (true) {
    try {
      std::cout << prompt;
      std::string line;
      getline(std::cin, line);
      if (line.size() != 1)
        std::invalid_argument("Not in range");
      char letter = line[0];
      if (isalpha(letter) == 0)
        std::invalid_argument("Not in range");
      letter = (char)tolower((int)letter);
      return letter;
    } catch (const std::exception &e) {
      std::cout << errorMessage << std::endl;
    }
  }
}
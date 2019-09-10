#include <iostream>
#include <bits/stdc++.h>
using namespace std;

class Words
{
public:
  Words()
  {
    this->readKeywords();
    this->readOperators();
    this->readPunctuators();
  }

  void readKeywords()
  {
    string key, val;
    std::ifstream file("keywords.txt", std::ios::in);
    if (file)
    {
      while (!file.eof())
      {
        file >> key;
        file >> val;
        this->keywords[key] = val;
      }
    }
  }

  void readOperators()
  {
    string key, val;
    std::ifstream file("operators.txt", std::ios::in);
    if (file)
    {
      while (!file.eof())
      {
        file >> key;
        file >> val;
        this->operators[key] = val;
      }
    }
  }

  void readPunctuators()
  {
    string key, val;
    std::ifstream file("punctuators.txt", std::ios::in);
    if (file)
    {
      while (!file.eof())
      {
        file >> key;
        file >> val;
        this->punctuators[key] = val;
      }
    }
  }

  string findKeyword(string key)
  {

    if (this->keywords.find(key) != this->keywords.end())
    {
      string result = this->keywords.find(key)->second;
      return result;
    }
    else
    {
      return "Not present";
    }
  }


  string findOperator(string key)
  {

    if (this->operators.find(key) != this->operators.end())
    {
      string result = this->operators.find(key)->second;
      return result;
    }
    else
    {
      return "Not present";
    }
  }


  string findPunctuator(string key)
  {

    if (this->punctuators.find(key) != this->punctuators.end())
    {
      string result = this->punctuators.find(key)->second;
      return result;
    }
    else
    {
      return "Not present";
    }
  }


  void print()
  {
    map<string, string>::iterator itr;
    cout << "\nThe keywords are:\n";
    for (itr = this->keywords.begin(); itr != this->keywords.end(); ++itr)
    {
      cout << itr->first << " " << itr->second << '\n';
    }
    cout << endl;

    cout << "\nThe operators are:\n";
    for (itr = this->operators.begin(); itr != this->operators.end(); ++itr)
    {
      cout << itr->first << " " << itr->second << '\n';
    }
    cout << endl;

    cout << "\nThe punctuators are:\n";
    for (itr = this->punctuators.begin(); itr != this->punctuators.end(); ++itr)
    {
      cout << itr->first << " " << itr->second << '\n';
    }
    cout << endl;
  }

private:
  map<string, string> keywords;
  map<string, string> operators;
  map<string, string> punctuators;
};
#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <regex>
#include "Words.h"
using namespace std;

bool slashEven(string word);
bool double_op(char character, char next_character);
bool isInt(string value);

int main()
{
  /*
  std::ifstream file ("test.txt",std::ios::in);
  if (file) {
    while (!file.eof()) str.push_back(file.get());
  }
*/
  Words words;
  string word = "hello'how\\\\'++b.35 hi";
  string *wordArray = new string[word.length()];
  int counter = 0;
  int line_no = 0;
  for (int i = 0; i < word.length(); i++)
  {

    if (wordArray[counter][0] != '\"' && wordArray[counter][0] != '\'')
    {

      if (word[i] == '\\' && word[i + 1] == '*')
      {
        if (wordArray[counter] != "")
          counter++;

        while (word[i - 1] != '*' && word[i] != '/')
        {
          if (word[i] == '\n')
            line_no++;
          i++;
        }
        continue;
      }

      if (word[i] == '/' && word[i + 1] == '/')
      {
        if (wordArray[counter] != "")
          counter++;

        while (word[i] != '13' || i < word.length())
          i++;

        line_no++;
        continue;
      }

      if (word[i] == ' ')
      {
        counter++;
        continue;
      }

      if (word[i] == '\"' || word[i] == '\'')
      {
        counter++;
        wordArray[counter].push_back(word[i]);
        continue;
      }

      if (double_op(word[i], word[i + 1]))
      {
        wordArray[counter].push_back(word[i]);
        wordArray[counter].push_back(word[i + 1]);
        i++;
        counter++;
        continue;
      }

      if (word[i] == '-' && word[i + 1] == '>')
      {
        wordArray[counter].push_back(word[i]);
        wordArray[counter].push_back(word[i + 1]);
        i++;
        counter++;
        continue;
      }

      string s(1, word[i]);
      if (words.findOperator(s) != "Not present")
      {
        wordArray[counter].push_back(word[i]);
        counter++;
        continue;
      }

      if (words.findPunctuator(s) != "Not present")
      {
        if (words.findPunctuator(s) == ".")
        {
          string search(1, word[i + 1]);
          if (isInt(wordArray[counter]) && isInt(search))
          {
            wordArray[counter].push_back(word[i]);
            i++;
            search = "";
            search.push_back(word[i]);

            while (isInt(search) && i < word.length())
            {
              wordArray[counter].push_back(word[i]);
              i++;
              search = "";
              search.push_back(word[i]);
            }
            counter++;
            continue;
          }

          if (isInt(search))
          {
            counter++;
            wordArray[counter].push_back(word[i]);
            i++;
            search = "";
            search.push_back(word[i]);

            while (isInt(search) && i < word.length())
            {
              wordArray[counter].push_back(word[i]);
              i++;
              search = "";
              search.push_back(word[i]);
            }
            counter++;
            continue;
          }
          else
          {

            wordArray[counter].push_back(word[i]);
            counter++;
            continue;
          }
        }
        else
        {

          wordArray[counter].push_back(word[i]);
          counter++;
          continue;
        }
      }
    }

    if (word[i] == '13')
    {
      wordArray[counter].push_back(word[i]);
      counter++;
      line_no++;
      continue;
    }

    //cout << word[i] << word[i + 1] << endl;

    if ((word[i] == '\"' && wordArray[counter][0] == '\"') || ((word[i] == '\'' && wordArray[counter][0] == '\'')) && slashEven(wordArray[counter]))
    {
      wordArray[counter].push_back(word[i]);
      counter++;
      continue;
    }

    wordArray[counter].push_back(word[i]);
  }

  for (int i = 0; i <= counter; i++)
  {
    cout << wordArray[i] << endl;
  }

  system("pause");
}

bool isInt(string value)
{

  regex regInt("((\\+[1-9][0-9]*)|(\\-[1-9][0-9]*)|([1-9][0-9]*))");
  if (regex_match(value, regInt))
    return true;

  return false;
}

bool double_op(char character, char next_character)
{

  if ((character == '+' && next_character == '+') || (character == '-' && next_character == '-') || (character == '=' && next_character == '=') || (character == '&' && next_character == '&') || (character == '|' && next_character == '|'))
  {
    return true;
  }

  if (character == '!' && next_character == '=')
  {
    return true;
  }

  if (character == '>' && next_character == '=')
  {
    return true;
  }

  if (character == '<' && next_character == '=')
  {
    return true;
  }

  return false;
}

bool slashEven(string word)
{
  int slashes = 0;
  for (int i = word.length() - 1; i >= 0; i--)
  {
    if (word[i] != '\\')
    {
      break;
    }
    slashes++;
  }

  return slashes % 2 == 0 ? true : false;
}
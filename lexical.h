#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <string>
#include <regex>
#include "lexeme.h"
#include "Words.h"
using namespace std;

class LexicalAnalyzer
{
  public:
  LexicalAnalyzer()
  {
  }

  void addLexeme(Lexeme &lex)
  {
    lexemes.push_back(lex);
  }

  void breakWords(string filename)
  {

    string word = "";
    std::ifstream file(filename, std::ios::in);
    if (file)
    {
      while (!file.eof())
        word.push_back(file.get());
    }

    //Words this->words;
    //string word = "hello'how\\\\'++b.35 hi";
    string temp = "";
    int counter = 0;
    int line_no = 0;
    for (int i = 0; i < word.length(); i++)
    {

      if (temp[0] != '\"' && temp[0] != '\'')
      {

        if (word[i] == '\\' && word[i + 1] == '*')
        {
          if (temp != "")
          {
            breakTemp(temp, line_no);
            temp = "";
          }

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
          if (temp != "")
          {
            breakTemp(temp, line_no);
            temp = "";
          }

          while (word[i] != '13' && i < word.length())
            i++;

          line_no++;
          continue;
        }

        if (word[i] == ' ')
        {
          continue;
        }

        if (word[i] == '\"' || word[i] == '\'')
        {
          breakTemp(temp, line_no);
          temp = "";
          temp.push_back(word[i]);
          continue;
        }

        if (double_op(word[i], word[i + 1]))
        {
          temp.push_back(word[i]);
          temp.push_back(word[i + 1]);
          i++;
          breakTemp(temp, line_no);
          temp = "";
          continue;
        }

        if (word[i] == '-' && word[i + 1] == '>')
        {
          temp.push_back(word[i]);
          temp.push_back(word[i + 1]);
          i++;
          breakTemp(temp, line_no);
          temp = "";
          continue;
        }

        string s(1, word[i]);
        if (this->words.findOperator(s) != "Not present")
        {
          temp.push_back(word[i]);
          breakTemp(temp, line_no);
          temp = "";
          continue;
        }

        if (this->words.findPunctuator(s) != "Not present")
        {
          if (this->words.findPunctuator(s) == ".")
          {
            string search(1, word[i + 1]);
            if (isInt(temp) && isInt(search))
            {
              temp.push_back(word[i]);
              i++;
              search = "";
              search.push_back(word[i]);

              while (isInt(search) && i < word.length())
              {
                temp.push_back(word[i]);
                i++;
                search = "";
                search.push_back(word[i]);
              }
              breakTemp(temp, line_no);
              temp = "";
              continue;
            }

            if (isInt(search))
            {
              breakTemp(temp, line_no);
              temp = "";
              temp.push_back(word[i]);
              i++;
              search = "";
              search.push_back(word[i]);

              while (isInt(search) && i < word.length())
              {
                temp.push_back(word[i]);
                i++;
                search = "";
                search.push_back(word[i]);
              }
              breakTemp(temp, line_no);
              temp = "";
              continue;
            }
            else
            {

              temp.push_back(word[i]);
              breakTemp(temp, line_no);
              temp = "";
              continue;
            }
          }
          else
          {

            temp.push_back(word[i]);
            breakTemp(temp, line_no);
            temp = "";
            continue;
          }
        }
      }

      if (word[i] == '\n')
      {
        temp.push_back(word[i]);
        breakTemp(temp, line_no);
        temp = "";
        line_no++;
        continue;
      }

      //cout << word[i] << word[i + 1] << endl;

      if ((word[i] == '\"' && temp[0] == '\"') || ((word[i] == '\'' && temp[0] == '\'')) && slashEven(temp))
      {
        temp.push_back(word[i]);
        breakTemp(temp, line_no);
        temp = "";
        continue;
      }

      temp.push_back(word[i]);
    }
    breakTemp(temp, line_no);
  }

  void breakTemp(string temp, int line_no)
  {
    cout << "Word " << temp << " at line" << line_no << endl;
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

private:
  std::vector<Lexeme> lexemes;
  Words words;
};
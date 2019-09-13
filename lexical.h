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
    this->lexemes.push_back(lex);
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
    file.close();

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
            this->classifyWord(temp, line_no);
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
            this->classifyWord(temp, line_no);
            temp = "";
          }

          while (word[i] != '\n' && i < word.length())
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
          this->classifyWord(temp, line_no);
          temp = "";
          temp.push_back(word[i]);
          continue;
        }

        if (double_op(word[i], word[i + 1]))
        {
          temp.push_back(word[i]);
          temp.push_back(word[i + 1]);
          i++;
          this->classifyWord(temp, line_no);
          temp = "";
          continue;
        }

        if (word[i] == '-' && word[i + 1] == '>')
        {
          temp.push_back(word[i]);
          temp.push_back(word[i + 1]);
          i++;
          this->classifyWord(temp, line_no);
          temp = "";
          continue;
        }

        string s(1, word[i]);
        if (this->words.findOperator(s) != "Not present")
        {
          temp.push_back(word[i]);
          this->classifyWord(temp, line_no);
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
              this->classifyWord(temp, line_no);
              temp = "";
              continue;
            }

            if (isInt(search))
            {
              this->classifyWord(temp, line_no);
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
              this->classifyWord(temp, line_no);
              temp = "";
              continue;
            }
            else
            {

              temp.push_back(word[i]);
              this->classifyWord(temp, line_no);
              temp = "";
              continue;
            }
          }
          else
          {

            temp.push_back(word[i]);
            this->classifyWord(temp, line_no);
            temp = "";
            continue;
          }
        }
      }

      if (word[i] == '\n')
      {
        temp.push_back(word[i]);
        this->classifyWord(temp, line_no);
        temp = "";
        line_no++;
        continue;
      }

      //cout << word[i] << word[i + 1] << endl;

      if ((word[i] == '\"' && temp[0] == '\"') || ((word[i] == '\'' && temp[0] == '\'')) && slashEven(temp))
      {
        temp.push_back(word[i]);
        this->classifyWord(temp, line_no);
        temp = "";
        continue;
      }

      temp.push_back(word[i]);
    }
    temp.pop_back();
    this->classifyWord(temp, line_no);
  }

  void classifyWord(string value, int line_no)
  {
    Lexeme Lex;
    int type = wordType(value[0]);
    switch (type)
    {
    case 1:
      if (isIdentifier(value))
      {
        Lex.setLexeme(line_no, "ID", value);
        this->addLexeme(Lex);
      }
      else
      {
        Lex.setLexeme(line_no, "Invalid Lexeme", value);
        this->addLexeme(Lex);
      }
      break;

    case 2:
      if (isIdentifier(value))
      {
        string classPart = this->words.findKeyword(value);
        if (classPart != "Not present")
        {
          Lex.setLexeme(line_no, classPart, value);
          this->addLexeme(Lex);
        }
        else
        {
          Lex.setLexeme(line_no, "ID", value);
          this->addLexeme(Lex);
        }
      }
      else
      {
        Lex.setLexeme(line_no, "Invalid Lexeme", value);
        this->addLexeme(Lex);
      }
      break;

    case 3:
      if (value == ".")
      {
        Lex.setLexeme(line_no, ".", value);
        this->addLexeme(Lex);
        break;
      }

      if (isInt(value))
      {
        Lex.setLexeme(line_no, "IntConst", value);
        this->addLexeme(Lex);
        break;
      }

      if (isFloat(value))
      {
        Lex.setLexeme(line_no, "FloatConst", value);
        this->addLexeme(Lex);
        break;
      }
      Lex.setLexeme(line_no, "Invalid Lexeme", value);
      this->addLexeme(Lex);
      break;

    case 4:
      if (isChar(value))
      {
        Lex.setLexeme(line_no, "CharConst", value);
        this->addLexeme(Lex);
      }
      else
      {
        Lex.setLexeme(line_no, "Invalid Lexeme", value);
        this->addLexeme(Lex);
      }
      break;

    case 5:
      if (isString(value))
      {
        Lex.setLexeme(line_no, "StringConst", value);
        this->addLexeme(Lex);
      }
      else
      {
        Lex.setLexeme(line_no, "Invalid Lexeme", value);
        this->addLexeme(Lex);
      }
      break;

    default:

      if (value == "\n")
      {
        Lex.setLexeme(line_no, "Terminator", value);
        this->addLexeme(Lex);
        break;
      }

      string classPart = this->words.findOperator(value);

      if (classPart != "Not present")
      {
        Lex.setLexeme(line_no, classPart, value);
        this->addLexeme(Lex);
        break;
      }
      classPart = this->words.findPunctuator(value);

      if (classPart != "Not present")
      {
        Lex.setLexeme(line_no, classPart, value);
        this->addLexeme(Lex);
        break;
      }

      Lex.setLexeme(line_no, "Invalid Lexeme", value);
      this->addLexeme(Lex);
      break;
    }
    //cout << "Word " << value << " at line" << line_no << endl;
  }

  int wordType(char startChar)
  {
    if (startChar == '_')
      return 1;

    if ((startChar >= 'A' && startChar <= 'Z') || (startChar >= 'a' && startChar <= 'z'))
      return 2;

    if ((startChar >= '0' && startChar <= '9') || startChar == '.')
      return 3;

    if (startChar == '\'')
      return 4;

    if (startChar == '\"')
      return 5;

    return 0;
  }

  bool isInt(string value)
  {
    regex regInt("((\\+[1-9][0-9]*)|(\\-[1-9][0-9]*)|([1-9][0-9]*))");
    if (regex_match(value, regInt))
      return true;

    return false;
  }

  bool isFloat(string value)
  {
    regex regFloat("((\\+[0-9]*\\.[0-9]+)|([0-9]*\\.[0-9]+)|(\\-[0-9]*\\.[0-9]+))");
    if (regex_match(value, regFloat))
      return true;

    return false;
  }

  bool isChar(string value)
  {
    regex regChar("(\\'((\\n)|(\\')|(\\\\)|(\\r)|(\\t)|(\\b)|(\\f)|(\\v)|(\\0)|(\")|(.)|([0-9]{1,3}))\\')");
    if (regex_match(value, regChar))
      return true;

    return false;
  }

  bool isString(string value)
  {
    regex regString("(\"(.*)\")");
    if (regex_match(value, regString))
      return true;

    return false;
  }

  bool isIdentifier(string value)
  {
    regex regIdentifier("(([A-Za-z]*\\_+[A-Za-z]*[0-9]*\\_*[A-Za-z]*\\_*)|([A-Za-z][A-Za-z]*[0-9]*[A-Za-z]*))");
    if (regex_match(value, regIdentifier))
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

  void print()
  {
    for (int i = 0; i < this->lexemes.size(); i++)
    {
      cout << this->lexemes.at(i);
    }
  }

  void write()
  {
    ofstream file("tokens.txt");

    for (int i = 0; i < this->lexemes.size(); i++)
    {
      file << this->lexemes.at(i);
    }
    file.close();
  }

private:
  std::vector<Lexeme> lexemes;
  Words words;
};
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
    this->counter = 0;
  }

  void addLexeme(Lexeme &lex)
  {
    this->lexemes.push_back(lex);
  }

  std::vector<Lexeme> getLexemes()
  {
    return this->lexemes;
  }

  bool backTrack(string word, int pos)
  {
    for (int j = (pos - 1); j >= 0; j--)
    {
      string s(1, word[j]);
      if (this->words.findOperator(s) != "Not present" || this->words.findPunctuator(s) != "Not present")
        return true;

      if (word[j] == ' ')
        continue;

      if (word[j] == '\n')
        return true;

      return false;
    }
    return true;
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
    int line_no = 1;
    for (int i = 0; i < word.length(); i++)
    {

      if (temp[0] == '\'' && temp.length() > 3)
      {
        this->classifyWord(temp, line_no);
        temp = "";
      }

      if (temp[0] != '\"' && temp[0] != '\'')
      {

        if (word[i] == '/' && word[i + 1] == '*')
        {
          if (temp != "")
          {
            this->classifyWord(temp, line_no);
            temp = "";
          }
          i++;
          i++;
          do
          {
            if (word[i] == '\n')
              line_no++;
            i++;
          } while (word[i - 1] != '*' && word[i] != '/' && i < word.length());
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
          if (temp != "")
          {
            this->classifyWord(temp, line_no);
            temp = "";
          }
          continue;
        }

        if (word[i] == '\"' || word[i] == '\'')
        {
          if (temp != "")
          {
            this->classifyWord(temp, line_no);
            temp = "";
          }
          temp.push_back(word[i]);
          continue;
        }

        if (double_op(word[i], word[i + 1]))
        {
          if (temp != "")
          {
            this->classifyWord(temp, line_no);
            temp = "";
          }
          temp.push_back(word[i]);
          temp.push_back(word[i + 1]);
          i++;
          this->classifyWord(temp, line_no);
          temp = "";
          continue;
        }

        if ((word[i] == '-' && word[i + 1] == '>') || (word[i] == ':' && word[i + 1] == ':'))
        {
          if (temp != "")
          {
            this->classifyWord(temp, line_no);
            temp = "";
          }
          temp.push_back(word[i]);
          temp.push_back(word[i + 1]);
          i++;
          this->classifyWord(temp, line_no);
          temp = "";
          continue;
        }

        string intcheck(1, word[i + 1]);
        string floatcheck = "";
        floatcheck.push_back(word[i + 1]);
        floatcheck.push_back(word[i + 2]);

        if ((word[i] == '+' || word[i] == '-') && (this->backTrack(word, i)) && (isInt(intcheck) || isFloat(floatcheck)))
        {

          if (isInt(intcheck) || isFloat(floatcheck))
          {

            if (temp != "")
            {
              this->classifyWord(temp, line_no);
              temp = "";
            }

            temp.push_back(word[i]);
            temp.push_back(word[i + 1]);
            i++;
            i++;

            //s.push_back(word[i]);

            while ((isInt(intcheck) || isFloat(floatcheck)) && i < word.length())
            {
              temp.push_back(word[i]);
              floatcheck.push_back(word[i + 1]);
              intcheck.push_back(word[i]);
              i++;
            }
            if (isFloat(floatcheck))
            {
              floatcheck.pop_back();
              temp.push_back(word[i]);
              i++;
            }
            this->classifyWord(temp, line_no);
            temp = "";
            continue;
          }
        }

        string s(1, word[i]);
        if (this->words.findOperator(s) != "Not present")
        {
          if (temp != "")
          {
            this->classifyWord(temp, line_no);
            temp = "";
          }

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
              i--;
              continue;
            }

            if (isInt(search))
            {
              if (temp != "")
              {
                this->classifyWord(temp, line_no);
                temp = "";
              }

              //this->classifyWord(temp, line_no);
              //temp = "";
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
              i--;
              continue;
            }
            else
            {
              if (temp != "")
              {
                this->classifyWord(temp, line_no);
                temp = "";
              }
              temp.push_back(word[i]);
              this->classifyWord(temp, line_no);
              temp = "";
              continue;
            }
          }
          else
          {
            if (temp != "")
            {
              this->classifyWord(temp, line_no);
              temp = "";
            }
            temp.push_back(word[i]);
            this->classifyWord(temp, line_no);
            temp = "";
            continue;
          }
        }
      }

      if (word[i] == '\n')
      {
        if (temp != "")
        {
          this->classifyWord(temp, line_no);
          temp = "";
        }

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

    if (temp.length() != 0)
      temp.pop_back();

    if (temp != "")
      this->classifyWord(temp, line_no);

    Lexeme lex;
    lex.setLexeme(line_no, "$", "$");
    this->addLexeme(lex);
  }

  void classifyWord(string value, int line_no)
  {
    int type;
    Lexeme Lex;
    if (value == "->")
    {
      type = 0;
    }
    else
    {
      type = wordType(value[0]);
    }

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

      if (value == "++" || value == "--")
      {
        Lex.setLexeme(line_no, "inc_dec", value);
        this->addLexeme(Lex);
        break;
      }

      if (value == "+" || value == "-")
      {
        Lex.setLexeme(line_no, "PM", value);
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

    if ((startChar >= '0' && startChar <= '9') || (startChar == '.') || (startChar == '-') || (startChar == '+'))
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
    regex regChar("(\\'((\\\\n)|(\\')|(\\\\\\\\)|(\\\\r)|(\\\\t)|(\\\\b)|(\\\\f)|(\\\\v)|(\\\\0)|(\")|(.)|([0-9]{1,3}))\\')");
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

  string syntaxStart()
  {
    if (this->lexemes.size() <= 0)
    {
      return "No code to examine";
    }

    if (this->start())
    {
      if (this->lexemes.at(this->counter).getClassName() == "$")
      {
        return "Valid Syntax";
      }
      else
      {
        return "Invalid Syntax";
      }
    }
    return "Invalid Syntax";
  }

  bool start()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "function" || temp == "static" || temp == "#" || temp == "ID" || temp == "DT" || temp == "if" || temp == "loop" || temp == "return" || temp == "exit" || temp == "inc_dec" || temp == "Terminator" || temp == "var")
    {
      if (this->SST_IFL2())
        if (this->start_null())
          return true;
    }
    else
    {
      if (temp == "abstract" || temp == "class")
      {
        if (this->class_st())
          if (this->start_null())
            return true;
      }
    }
    return false;
  }

  bool SST_IFL2()
  {
    string temp = this->lexemes.at(this->counter).getClassName();

    if (temp == "static" || temp == "#" || temp == "ID" || temp == "DT" || temp == "if" || temp == "loop" || temp == "return" || temp == "exit" || temp == "inc_dec" || temp == "Terminator" || temp == "var")
    {
      if (this->SST_IFL())
        return true;
    }
    else
    {
      if (temp == "function")
      {
        if (this->fun_st())
          return true;
      }
    }
    return false;
  }

  bool fun_st()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "function")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        this->counter++;
        if (this->lexemes.at(this->counter).getClassName() == "(")
        {
          this->counter++;
          if (this->pl())
          {
            if (this->lexemes.at(this->counter).getClassName() == ")")
            {
              this->counter++;
              if (this->body_fn())
                return true;
            }
          }
        }
      }
    }

    return false;
  }

  bool pl()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "#" || temp == "ID")
    {
      if (this->IDs())
        return true;
    }
    else
    {
      if (temp == ")")
      {
        return true;
      }
    }
    return false;
  }

  bool IDs()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "#")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        this->counter++;
        if (this->next())
          return true;
      }
    }
    return true;
  }

  bool next()
  {
    if (this->lexemes.at(this->counter).getClassName() == ",")
    {
      this->counter++;
      if (this->IDs())
      {
        return true;
      }
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == ")")
      {
        return true;
      }
    }
    false;
  }

  bool term()
  {
    if (this->lexemes.at(this->counter).getClassName() == "Terminator")
    {
      this->counter++;
      return true;
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == "{")
      {
        return true;
      }
    }
    return false;
  }

  bool body_fn()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "static" || temp == "#" || temp == "ID" || temp == "DT" || temp == "if" || temp == "loop" || temp == "return" || temp == "exit" || temp == "inc_dec" || temp == "Terminator" || temp == "var")
    {
      if (this->SST_IFL())
        return true;
    }
    else
    {
      if (temp == "{")
      {
        this->counter++;
        if (MST_IFL())
        {
          if (this->lexemes.at(this->counter).getClassName() == "}")
          {
            this->counter++;
            return true;
          }
        }
      }
    }
    return false;
  }

  bool MST_IFL()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "static" || temp == "#" || temp == "ID" || temp == "DT" || temp == "if" || temp == "loop" || temp == "return" || temp == "exit" || temp == "inc_dec" || temp == "Terminator" || temp == "var")
    {
      if (this->SST_IFL())
        return true;
    }
    else
    {
      if (temp == "}")
        return true;
    }
    return false;
  }

  bool SST_IFL()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "static")
    {
      if (this->static_sts())
        return true;
    }

    if (temp == "#" || temp == "ID")
    {
      if (this->hash_or_ID_start())
        return true;
    }

    if (temp == "if")
    {
      if (this->if_else())
        return true;
    }

    if (temp == "DT" || temp == "var")
    {
      if (this->Dec())
        return true;
    }

    if (temp == "loop")
    {
      if (this->loop_st())
        return true;
    }

    if (temp == "exit")
    {
      if (this->exit_st())
        return true;
    }

    if (temp == "return")
    {
      if (this->return_st())
        return true;
    }

    if (temp == "inc_dec")
    {
      if (this->inc_dec_start())
        return true;
    }

    if (temp == "Terminator")
    {
      this->counter++;
      return true;
    }

    return false;
  }

  bool CONST()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      this->counter++;
      return true;
    }
    return false;
  }

  bool static_sts()
  {
    if (this->lexemes.at(this->counter).getClassName() == "static")
    {
      this->counter++;
      if (function_or_dec())
      {
        return true;
      }
    }
    return false;
  }

  bool function_or_dec()
  {
    if (this->lexemes.at(this->counter).getClassName() == "function")
    {
      if (fun_st())
        return true;
    }

    if (this->lexemes.at(this->counter).getClassName() == "DT" || this->lexemes.at(this->counter).getClassName() == "var")
    {
      if (Dec())
        return true;
    }

    if (this->lexemes.at(this->counter).getClassName() == "ID")
    {
      this->counter++;
      if (obj_dec())
      {
        if (this->lexemes.at(this->counter).getClassName() == "Terminator")
        {
          this->counter++;
          return true;
        }
      }
    }
    return false;
  }

  bool inc_dec_start()
  {
  }

  bool return_st()
  {
  }

  bool exit_st()
  {
  }

  bool loop_st()
  {
  }

  bool if_else()
  {
  }

  bool hash_or_ID_start()
  {
  }

  bool Dec()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "DT" || temp == "var")
    {
      if (DTs())
      {
        if (p_st_or_null_p_st())
        {
          if (list())
            return true;
        }
      }
    }
    return false;
  }

  bool p_st_or_null_p_st()
  {
    if (this->lexemes.at(this->counter).getClassName() == "#")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        this->counter++;
        if (new_init())
        {
          return true;
        }
      }
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        this->counter++;
        if (init())
        {
          return false;
        }
      }
    }
    return false;
  }

  bool new_init()
  {
    if (this->lexemes.at(this->counter).getClassName() == "=")
    {
      this->counter++;
      if (new_arr_const())
      {
        return true;
      }
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == "," || this->lexemes.at(this->counter).getClassName() == "Terminator")
      {
        return true;
      }
    }
    return false;
  }

  bool new_arr_const()
  {
    if (this->lexemes.at(this->counter).getClassName == "new")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName == "DT")
      {
        this->counter++;
        if (arr_or_null())
        {
          return true;
        }
      }
    }
    else
    {
      if (CONST())
        return true;
    }
  }

  bool arr_or_null()
  {
    if (this->lexemes.at(this->counter).getClassName == "[")
    {
      this->counter++;
      if (OE())
      {
        if (this->lexemes.at(this->counter).getClassName == "]")
        {
          this->counter++;
          return true;
        }
      }
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName == "," || this->lexemes.at(this->counter).getClassName == "Terminator")
      {
        return true;
      }
    }
    return false;
  }

  bool init()
  {
    if (this->lexemes.at(this->counter).getClassName == "=")
    {
      this->counter++;
      if (new_arr_init())
      {
        return true;
      }
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName == "," || this->lexemes.at(this->counter).getClassName == "Terminator")
      {
        return true;
      }
    }
    return false;
  }

  bool DTs()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "DT" || temp == "var")
    {
      this->counter++;
      return true;
    }
    return false;
  }

  bool start_null()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "function" || temp == "static" || temp == "#" || temp == "ID" || temp == "DT" || temp == "if" || temp == "loop" || temp == "return" || temp == "exit" || temp == "inc_dec" || temp == "Terminator" || temp == "var")
    {
      if (this->start())
        return true;
    }
    else
    {
      if (temp == "$")
      {
        return true;
      }
    }
    return false;
  }

  bool new_arr_init()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (CONST())
    {
      return true;
    }
    else
    {
      if (temp == "new")
      {
        this->counter++;
        if (this->lexemes.at(this->counter).getClassName() == "[")
        {
          this->counter++;
          if (OE())
          {
            if (this->lexemes.at(this->counter).getClassName() == "]")
            {
              this->counter++;
              return true;
            }
          }
        }
      }
      else
      {
        if (this->lexemes.at(this->counter).getClassName() == "#")
        {
          this->counter++;
          if (this->lexemes.at(this->counter).getClassName() == "ID")
          {
            this->counter++;
            if (static_ref_or_null())
              if (trail_oe())
                return true;
          }
        }
        else
        {
          if (this->lexemes.at(this->counter).getClassName() == "ID")
          {
            this->counter++;
            if (static_ref_or_null())
              if (trail_oe())
                return true;
          }
        }
      }
    }
    return false;
  }

  bool list()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ",")
    {
      this->counter++;
      if (p_st_or_null_p_st())
        if (list())
          return true;
    }
    else
    {
      if (temp == "Terminator")
      {
        this->counter++;
        return true;
      }
    }
    return false;
  }

  bool obj_dec()
  {
    if (this->lexemes.at(this->counter).getClassName() == "#" || this->lexemes.at(this->counter).getClassName() == "ID")
    {
      if (p())
      {
        if (this->lexemes.at(this->counter).getClassName() == "ID")
        {
          this->counter++;
          if (init1())
            if (list1())
              return true;
        }
      }
    }
    return false;
  }

  bool p()
  {
    if (this->lexemes.at(this->counter).getClassName() == "#")
    {
      this->counter++;
      return true;
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        return true;
      }
    }
    return false;
  }

  bool init1()
  {
    if (this->lexemes.at(this->counter).getClassName() == "[")
    {
      this->counter++;
      if (OE())
      {
        if (this->lexemes.at(this->counter).getClassName() == "]")
        {
          this->counter++;
          return true;
        }
      }
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == "=")
      {
        this->counter++;
        if (this->lexemes.at(this->counter).getClassName() == "new")
        {
          this->counter++;
          if (pl_or_arr())
          {
            return true;
          }
        }
      }
      else
      {
        if (this->lexemes.at(this->counter).getClassName() == "," || this->lexemes.at(this->counter).getClassName() == "Terminator")
        {
          return true;
        }
      }
    }
    return false;
  }

  bool pl_or_arr()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "(")
    {
      this->counter++;
      if (OEs())
        if (this->lexemes.at(this->counter).getClassName() == ")")
        {
          this->counter++;
          return true;
        }
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == "[")
      {
        this->counter++;
        if (OE())
          if (this->lexemes.at(this->counter).getClassName() == "]")
          {
            this->counter++;
            return true;
          }
      }
    }
    return false;
  }

  bool OEs()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (OE())
        if (next1())
          return true;
    }
    return false;
  }

  bool class_st()
  {
  }

  bool OE()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (AE())
        if (OE1())
          return true;
    }
    return false;
  }

  bool AE()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (RE())
        if (AE1())
          return true;
    }
    return false;
  }

  bool RE()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (PME())
        if (RE1())
          return true;
    }
    return false;
  }

  bool PME()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (MDME())
        if (PME1())
          return true;
    }
    return false;
  }

  bool MDME()
  {
  }

  bool PME1()
  {
  }

  bool RE1()
  {
    if (this->lexemes.at(this->counter).getClassName() == "ROP")
    {
      this->counter++;
      if (PME())
        if (RE1())
          return true;
    }
    else
    {
      string temp = this->lexemes.at(this->counter).getClassName();
      if (temp == "]" || temp == ")" || temp == "," || temp == "Terminator" || temp == ";" || temp == "||" || temp == "&&")
      {
        return true;
      }
    }
    return false;
  }

  bool AE1()
  {
    if (this->lexemes.at(this->counter).getClassName() == "&&")
    {
      this->counter++;
      if (RE())
        if (AE1())
          return true;
    }
    else
    {
      string temp = this->lexemes.at(this->counter).getClassName();
      if (temp == "]" || temp == ")" || temp == "," || temp == "Terminator" || temp == ";" || temp == "||")
      {
        return true;
      }
    }
    return false;
  }

  bool OE1()
  {
    if (this->lexemes.at(this->counter).getClassName() == "||")
    {
      this->counter++;
      if (AE())
        if (OE1())
          return true;
    }
    else
    {
      string temp = this->lexemes.at(this->counter).getClassName();
      if (temp == "]" || temp == ")" || temp == "," || temp == "Terminator" || temp == ";")
      {
        return true;
      }
    }
    return false;
  }

  bool static_ref_or_null()
  {
  }

  bool trail_oe()
  {
  }

  bool list1()
  {
    if (this->lexemes.at(this->counter).getClassName() == ",")
    {
      this->counter++;
      if (obj_dec())
        return true;
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == "," || this->lexemes.at(this->counter).getClassName() == "Terminator")
        return true;
    }
    return false;
  }

  bool next1()
  {
    if (this->lexemes.at(this->counter).getClassName() == ",")
    {
      this->counter++;
      if (OE())
        return true;
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == ")")
        return true;
    }
    return false;
  }

  void print()
  {
    for (int i = 0; i < this->lexemes.size(); i++)
    {
      cout << this->lexemes.at(i);
    }
  }

  void write(string filename)
  {
    ofstream file(filename);

    for (int i = 0; i < this->lexemes.size(); i++)
    {
      file << this->lexemes.at(i);
    }
    file.close();
  }

private:
  std::vector<Lexeme> lexemes;
  Words words;
  int counter;
};
#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <string>
#include <regex>
#include "lexeme.h"
#include "Words.h"
using namespace std;

class Analyzer
{
public:
  Analyzer()
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
      if (this->words.findOperator(s) != "Not present" || (this->words.findPunctuator(s) != "Not present" && this->words.findPunctuator(s) != ")"))
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
            //            cout<<word[i]<<i<<endl;

            i++;
            //cout << word[i - 1] << word[i] << endl;
            if (word[i - 1] == '*' && word[i] == '/')
              break;

          } while (i < word.length());
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
        return "Valid Syntax.";
      }
      else
      {
        return "Invalid Syntax.";
      }
    }
    return "Invalid Syntax.";
  }

  bool start()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "function" || temp == "static" || temp == "#" || temp == "ID" || temp == "DT" || temp == "if" || temp == "loop" || temp == "return" || temp == "exit" || temp == "inc_dec" || temp == "Terminator" || temp == "var" || temp == "executionMod")
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

  bool class_st()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "abstract" || temp == "class")
    {
      if (this->abstract_st())
        if (this->lexemes.at(this->counter).getClassName() == "class")
        {
          this->counter++;
          if (this->lexemes.at(this->counter).getClassName() == "ID")
          {
            this->counter++;
            if (this->extends_st())
              if (this->term())
                if (this->class_body())
                  return true;
          }
        }
    }
    return false;
  }

  bool abstract_st()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "abstract")
    {
      this->counter++;
      return true;
    }
    else
    {
      if (temp == "class")
      {
        return true;
      }
    }
    return false;
  }

  bool extends_st()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "extends")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        this->counter++;
        return true;
      }
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == "Terminator" || this->lexemes.at(this->counter).getClassName() == "{")
      {
        return true;
      }
    }
    return false;
  }

  bool class_body()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "{")
    {
      this->counter++;
      if (this->MST_class())
        if (this->lexemes.at(this->counter).getClassName() == "}")
        {
          this->counter++;
          return true;
        }
    }
    return false;
  }

  bool MST_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "accessModifier" || temp == "Terminator")
    {
      if (this->SST_class())
        if (this->MST_class())
          return true;
    }
    else
    {
      if (temp == "}")
      {
        return true;
      }
    }
    return false;
  }

  bool SST_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID")
    {
      if (this->constructor_st())
        return true;
    }
    else
    {
      if (temp == "accessModifier")
      {
        this->counter++;
        if (this->func_or_dec_objdec())
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
    }
    return false;
  }

  bool constructor_st()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "(")
      {
        this->counter++;
        if (this->pl())
          if (this->lexemes.at(this->counter).getClassName() == ")")
          {
            this->counter++;
            if (this->term())
              if (this->lexemes.at(this->counter).getClassName() == "{")
              {
                this->counter++;
                if (this->body_con())
                  if (this->lexemes.at(this->counter).getClassName() == "}")
                  {
                    this->counter++;
                    return true;
                  }
              }
          }
      }
    }
    return false;
  }

  bool body_con()
  {
    string temp = this->lexemes.at(this->counter).getClassName();

    if (temp == "static" || temp == "#" || temp == "ID" || temp == "DT" || temp == "this" || temp == "if" || temp == "loop" || temp == "return" || temp == "exit" || temp == "inc_dec" || temp == "Terminator" || temp == "var" || temp == "executionMod")
    {
      if (this->MST_fn_class())
        return true;
    }
    return false;
  }

  bool func_or_dec_objdec()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "function" || temp == "static" || temp == "virtual" || temp == "abstract" || temp == "DT" || temp == "var" || temp == "ID")
    {
      if (this->static_or_null())
        if (this->f_d_o())
          return true;
    }
    return false;
  }

  bool static_or_null()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "static")
    {
      this->counter++;
      return true;
    }
    else
    {
      if (temp == "virtual" || temp == "abstract" || temp == "function" || temp == "DT" || temp == "var" || temp == "ID")
        return true;
    }
    return false;
  }

  bool f_d_o()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "virtual" || temp == "abstract" || temp == "function")
    {
      if (this->VA())
        if (this->fun_st_class())
          return true;
    }
    else
    {
      if (temp == "DT" || temp == "var")
      {
        if (this->DEC_class())
          return true;
      }
      else
      {
        if (temp == "ID")
        {
          this->counter++;
          if (this->obj_dec_class())
            if (this->lexemes.at(this->counter).getClassName() == "Terminator")
            {
              this->counter++;
              return true;
            }
        }
      }
    }
    return false;
  }

  bool VA()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "virtual" || temp == "abstract")
    {
      this->counter++;
      return true;
    }
    else
    {
      if (temp == "function")
        return true;
    }
    return false;
  }

  bool fun_st_class()
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
              if (this->term())
                if (this->body_fn_class())
                  return true;
            }
          }
        }
      }
    }

    return false;
  }

  bool body_fn_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "{")
    {
      this->counter++;
      if (this->MST_fn_class())
      {
        if (this->lexemes.at(this->counter).getClassName() == "}")
        {
          this->counter++;
          return true;
        }
      }
    }
    return false;
  }

  bool MST_fn_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "static" || temp == "#" || temp == "ID" || temp == "DT" || temp == "this" || temp == "if" || temp == "loop" || temp == "return" || temp == "exit" || temp == "inc_dec" || temp == "Terminator" || temp == "var" || temp == "executionMod")
    {
      if (this->SST_fn_class())
        if (this->MST_fn_class())
          return true;
    }
    else
    {
      if (temp == "}")
        return true;
    }
    return false;
  }

  bool SST_fn_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "static")
    {
      if (this->static_sts_class())
        return true;
    }

    if (temp == "#" || temp == "ID" || temp == "this")
    {
      if (this->hash_or_ID_start_class())
        return true;
    }

    if (temp == "if")
    {
      if (this->if_else_class())
        return true;
    }

    if (temp == "DT" || temp == "var")
    {
      if (this->DEC_class())
        return true;
    }

    if (temp == "loop")
    {
      if (this->loop_st_class())
        return true;
    }

    if (temp == "exit")
    {
      if (this->exit_st_class())
        return true;
    }

    if (temp == "return")
    {
      if (this->return_st_class())
        return true;
    }

    if (temp == "inc_dec")
    {
      if (this->inc_dec_start_class())
        return true;
    }

    if (temp == "Terminator")
    {
      this->counter++;
      return true;
    }

    if (temp == "executionMod")
    {
      this->counter++;
      return true;
    }

    return false;
  }

  bool static_sts_class()
  {
    if (this->lexemes.at(this->counter).getClassName() == "static")
    {
      this->counter++;
      if (this->objdec_or_dec())
      {
        return true;
      }
    }
    return false;
  }

  bool objdec_or_dec()
  {

    if (this->lexemes.at(this->counter).getClassName() == "DT" || this->lexemes.at(this->counter).getClassName() == "var")
    {
      if (this->DEC_class())
        return true;
    }
    else
    {

      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        this->counter++;
        if (this->obj_dec_class())
        {
          if (this->lexemes.at(this->counter).getClassName() == "Terminator")
          {
            this->counter++;
            return true;
          }
        }
      }
    }
    return false;
  }

  bool inc_dec_start_class()
  {
    if (this->lexemes.at(this->counter).getClassName() == "inc_dec")
    {
      this->counter++;
      if (this->this_or_ID_inc_dec())
        if (this->lexemes.at(this->counter).getClassName() == "Terminator")
        {
          this->counter++;
          return true;
        }
    }
    return false;
  }

  bool this_or_ID_inc_dec()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "#" || temp == "ID")
    {
      if (this->p())
        if (this->lexemes.at(this->counter).getClassName() == "ID")
        {
          this->counter++;
          if (this->static_ref_or_null())
            if (this->trail_class())
              return true;
        }
    }
    else
    {
      if (temp == "this")
      {
        this->counter++;
        if (this->trail_this())
          return true;
      }
    }
    return false;
  }

  bool return_st_class()
  {
    if (this->lexemes.at(this->counter).getClassName() == "return")
    {
      this->counter++;
      if (this->OE_class())
        if (this->lexemes.at(this->counter).getClassName() == "Terminator")
        {
          this->counter++;
          return true;
        }
    }
    return false;
  }

  bool exit_st_class()
  {
    if (this->lexemes.at(this->counter).getClassName() == "exit")
    {
      this->counter++;
      if (this->exit_st1_class())
        if (this->lexemes.at(this->counter).getClassName() == "Terminator")
        {
          this->counter++;
          return true;
        }
    }
    return false;
  }

  bool exit_st1_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "#" || temp == "this" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (this->const_or_ID_class())
        return true;
    }
    else
    {
      if (temp == "(")
      {
        this->counter++;
        if (this->const_or_ID_class())
          if (this->lexemes.at(this->counter).getClassName() == ")")
          {
            this->counter++;
            return true;
          }
      }
    }
    return false;
  }

  bool const_or_ID_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (this->CONST())
        return true;
    }
    else
    {
      if (temp == "ID" || temp == "#")
      {
        if (this->p())
          if (this->lexemes.at(this->counter).getClassName() == "ID")
          {
            this->counter++;
            if (this->trail_class())
              return true;
          }
      }
      else
      {
        if (temp == "this")
        {
          this->counter++;
          if (this->trail_this())
            return true;
        }
      }
    }
    return false;
  }

  bool loop_st_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "loop")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "(")
      {
        this->counter++;
        if (this->c1_class())
          if (this->c2_class())
            if (this->c3_class())
              if (this->lexemes.at(this->counter).getClassName() == ")")
              {
                this->counter++;
                if (this->term())
                  if (this->body_fn_class())
                    return true;
              }
      }
    }
    return false;
  }

  bool c1_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "DT" || temp == "var")
    {
      if (this->DEC_st1_class())
        if (this->lexemes.at(this->counter).getClassName() == ";")
        {
          this->counter++;
          return true;
        }
    }
    else
    {
      if (temp == "#" || temp == "ID")
      {
        if (this->assign_st_class1())
          if (this->lexemes.at(this->counter).getClassName() == ";")
          {
            this->counter++;
            return true;
          }
      }
      else
      {
        if (temp == ";")
        {
          this->counter++;
          return true;
        }
      }
    }
    return false;
  }

  bool DEC_st1_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "DT" || temp == "var")
    {
      if (this->DTs())
      {
        if (this->p_st_or_null_p_st_class())
        {
          if (this->list_class_loop())
            return true;
        }
      }
    }
    return false;
  }

  bool list_class_loop()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ",")
    {
      this->counter++;
      if (this->p_st_or_null_p_st_class())
        if (this->list_class_loop())
          return true;
    }
    else
    {
      if (temp == ";")
      {
        return true;
      }
    }
    return false;
  }

  bool assign_st_class1()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "#" || temp == "ID" || temp == "this")
    {
      if (this->assign_st_this_or_ID())
        if (this->assign_st_class())
          return true;
    }
    return false;
  }

  bool assign_st_this_or_ID()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "#" || temp == "ID")
    {
      if (this->p())
        if (this->lexemes.at(this->counter).getClassName() == "ID")
        {
          this->counter++;
          if (this->static_ref_or_null())
            if (this->fn_call_class_or_null_no_fc_end())
              return true;
        }
    }
    else
    {
      if (temp == "this")
      {
        this->counter++;
        if (this->trail_this_no_fc_end())
          return true;
      }
    }
    return false;
  }

  bool trail_this_no_fc_end()
  {
    if (this->lexemes.at(this->counter).getClassName() == "->")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        this->counter++;
        if (this->fn_call_class_or_null_no_fc_end())
          return true;
      }
    }
    return false;
  }

  bool trail_no_null_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        this->counter++;
        if (this->fn_call_class_or_null_no_fc_end())
          return true;
      }
    }
    else
    {
      if (temp == "[")
      {
        this->counter++;
        if (this->OE_class())
          if (this->lexemes.at(this->counter).getClassName() == "]")
          {
            this->counter++;
            if (this->trail_no_arr_no_fc_end_class())
              return true;
          }
      }
    }
    return false;
  }

  bool trail_no_fc_end_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        this->counter++;
        if (this->fn_call_class_or_null_no_fc_end())
          return true;
      }
    }
    else
    {
      if (temp == "[")
      {
        this->counter++;
        if (this->OE_class())
          if (this->lexemes.at(this->counter).getClassName() == "]")
          {
            this->counter++;
            if (this->trail_no_arr_no_fc_end_class())
              return true;
          }
      }
      else
      {
        if (temp == "=" || temp == "inc_dec")
          return true;
      }
    }
    return false;
  }

  bool fn_call_class_or_null_no_fc_end()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "(")
    {
      if (this->fn_call_class())
        if (this->trail_no_null_class())
          return true;
    }
    else
    {
      if (temp == "." || temp == "[" || temp == "=" || temp == "inc_dec")
        if (this->trail_no_fc_end_class())
          return true;
    }
    return false;
  }

  bool c2_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "this" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (this->OE_class())
        if (this->lexemes.at(this->counter).getClassName() == ";")
        {
          this->counter++;
          return true;
        }
    }
    else
    {
      if (temp == ";")
      {
        this->counter++;
        return true;
      }
    }
    return false;
  }

  bool c3_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "#" || temp == "ID" || temp == "this")
    {
      if (this->assign_st_or_inc_dec1_class())
        return true;
    }
    else
    {
      if (temp == "inc_dec")
      {
        if (this->inc_dec_start_class1())
          return true;
      }
      else
      {
        if (temp == ")")
          return true;
      }
    }
    return false;
  }

  bool assign_st_or_inc_dec1_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "#" || temp == "ID" || temp == "this")
    {
      if (this->assign_st_this_or_ID())
        if (this->assign_or_inc_dec_class())
          return true;
    }
    return false;
  }

  bool inc_dec_start_class1()
  {
    if (this->lexemes.at(this->counter).getClassName() == "inc_dec")
    {
      this->counter++;
      if (this->p())
        if (this->lexemes.at(this->counter).getClassName() == "ID")
        {
          this->counter++;
          if (this->static_ref_or_null())
            if (this->trail_class())
              return true;
        }
    }
    return false;
  }

  bool if_else_class()
  {
    if (this->lexemes.at(this->counter).getClassName() == "if")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "(")
      {
        this->counter++;
        if (this->OE_class())
          if (this->lexemes.at(this->counter).getClassName() == ")")
          {
            this->counter++;
            if (this->term())
              if (this->body_fn_class())
                if (this->oelse_class())
                  return true;
          }
      }
    }
    return false;
  }

  bool oelse_class()
  {
    if (this->lexemes.at(this->counter).getClassName() == "Terminator")
    {
      this->counter++;
      return true;
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == "else")
      {
        this->counter++;
        if (this->term())
          if (this->body_fn_class())
            return true;
      }
    }
    return false;
  }

  bool hash_or_ID_start_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "#")
    {
      this->counter++;
      if (temp == "ID")
      {
        this->counter++;
        if (this->static_ref_or_null())
          if (this->trail_oe_class_or_fn())
            if (this->lexemes.at(this->counter).getClassName() == "Terminator")
            {
              this->counter++;
              return true;
            }
      }
    }
    else
    {
      if (temp == "ID")
      {
        this->counter++;
        if (this->trail_or_obj_class())
          if (this->lexemes.at(this->counter).getClassName() == "Terminator")
          {
            this->counter++;
            return true;
          }
      }
      else
      {
        if (temp == "this")
        {
          this->counter++;
          if (this->trail_oe_this())
            string temp = this->lexemes.at(this->counter).getClassName();
          if (this->lexemes.at(this->counter).getClassName() == "Terminator")
          {
            this->counter++;
            return true;
          }
        }
      }
    }
    return false;
  }

  bool trail_oe_class_or_fn()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "(")
    {
      if (this->fn_call_class())
        return true;
    }
    else
    {
      if (temp == "." || temp == "[" || temp == "=" || temp == "inc_dec")
        if (this->trail_oe_class())
          return true;
    }
    return false;
  }

  bool trail_or_obj_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "#" || temp == "ID")
    {
      if (this->obj_dec_class())
      {
        return true;
      }
    }
    else
    {
      if (temp == "::" || temp == "inc_dec" || temp == "=" || temp == "." || temp == "(" || temp == "[")
        if (this->static_ref_or_null())
          if (this->trail_oe_class_or_fn())
            return true;
    }
    return false;
  }

  bool DEC_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "DT" || temp == "var")
    {
      if (this->DTs())
      {
        if (this->p_st_or_null_p_st_class())
        {
          if (this->list_class())
            return true;
        }
      }
    }
    return false;
  }

  bool p_st_or_null_p_st_class()
  {
    if (this->lexemes.at(this->counter).getClassName() == "#")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        this->counter++;
        if (this->new_init_class())
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
        if (this->init_class())
        {
          return true;
        }
      }
    }
    return false;
  }

  bool new_init_class()
  {
    if (this->lexemes.at(this->counter).getClassName() == "=")
    {
      this->counter++;
      if (this->new_arr_const_class())
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

  bool new_arr_const_class()
  {
    if (this->lexemes.at(this->counter).getClassName() == "new")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "DT")
      {
        this->counter++;
        if (this->arr_or_null_class())
        {
          return true;
        }
      }
    }
    else
    {
      string temp = this->lexemes.at(this->counter).getClassName();
      if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "this" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
        if (this->OE_class())
          return true;
    }
  }

  bool arr_or_null_class()
  {
    if (this->lexemes.at(this->counter).getClassName() == "[")
    {
      this->counter++;
      if (this->OE_class())
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
      if (this->lexemes.at(this->counter).getClassName() == "," || this->lexemes.at(this->counter).getClassName() == "Terminator")
      {
        return true;
      }
    }
    return false;
  }

  bool init_class()
  {
    if (this->lexemes.at(this->counter).getClassName() == "=")
    {
      this->counter++;
      if (this->new_arr_init_class())
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

  bool new_arr_init_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "new")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "[")
      {
        this->counter++;
        if (this->OE_class())
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
      string temp = this->lexemes.at(this->counter).getClassName();
      if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "this" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
        if (this->OE_class())
          return true;
    }
    return false;
  }

  bool trail_oe_this()
  {
    if (this->lexemes.at(this->counter).getClassName() == "->")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        this->counter++;
        if (this->function_or_trail_class())
          return true;
      }
    }
    return false;
  }

  bool list_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ",")
    {
      this->counter++;
      if (this->p_st_or_null_p_st_class())
        if (this->list_class())
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

  bool obj_dec_class()
  {
    if (this->lexemes.at(this->counter).getClassName() == "#" || this->lexemes.at(this->counter).getClassName() == "ID")
    {
      if (this->p())
      {
        if (this->lexemes.at(this->counter).getClassName() == "ID")
        {
          this->counter++;
          if (this->init1_class())
            if (this->list1_class())
              return true;
        }
      }
    }
    return false;
  }

  bool init1_class()
  {
    if (this->lexemes.at(this->counter).getClassName() == "[")
    {
      this->counter++;
      if (this->OE_class())
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
          if (this->lexemes.at(this->counter).getClassName() == "ID")
          {
            this->counter++;
            if (this->pl_or_arr_class())
            {
              return true;
            }
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

  bool pl_or_arr_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "(")
    {
      this->counter++;
      if (this->OEs_class_or_null())
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
        if (this->OE_class())
          if (this->lexemes.at(this->counter).getClassName() == "]")
          {
            this->counter++;
            return true;
          }
      }
    }
    return false;
  }

  bool OEs_class_or_null()
  {

    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "this" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (this->OEs_class())
        return true;
    }
    else
    {
      if (temp == ")")
        return true;
    }
    return false;
  }

  bool OEs_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "this" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (this->OE_class())
        if (this->next1_class())
          return true;
    }
    return false;
  }

  bool OE_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "this" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (this->AE_class())
        if (this->OE_class1())
          return true;
    }
    return false;
  }

  bool AE_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "this" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (this->RE_class())
        if (this->AE_class1())
          return true;
    }
    return false;
  }

  bool RE_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "this" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (this->PME_class())
        if (this->RE_class1())
          return true;
    }
    return false;
  }

  bool PME_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "this" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (this->MDME_class())
        if (this->PME_class1())
          return true;
    }
    return false;
  }

  bool MDME_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "this" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (this->OPs_class())
        if (this->MDME_class1())
          return true;
    }
    return false;
  }

  bool OPs_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "#" || temp == "ID")
    {
      if (this->p())
      {
        if (this->lexemes.at(this->counter).getClassName() == "ID")
        {
          this->counter++;
          if (this->static_ref_or_null())
          {
            if (this->trail_or_fn_class())
              return true;
          }
        }
      }
    }
    else
    {
      if (temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst" || temp == "IntConst")
      {
        this->counter++;
        return true;
      }
      else
      {
        if (temp == "(")
        {
          this->counter++;
          if (this->OE_class())
          {
            if (this->lexemes.at(this->counter).getClassName() == ")"){
              this->counter++;
              return true;
              }
          }
        }
        else
        {
          if (temp == "!")
          {
            this->counter++;
            if (this->OPs_class())
              return true;
          }
          else
          {
            if (temp == "inc_dec")
            {
              this->counter++;
              if (this->this_or_ID())
                return true;
            }
            else
            {
              if (temp == "this")
              {
                this->counter++;
                if (this->trail_this())
                  return true;
              }
            }
          }
        }
      }
    }
    return false;
  }

  bool this_or_ID()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "this")
    {
      this->counter++;
      if (this->trail_this())
        return true;
    }
    else
    {
      if (temp == "ID")
      {
        this->counter++;
        if (this->trail_class())
          return true;
      }
    }
    return false;
  }

  bool trail_this()
  {
    if (this->lexemes.at(this->counter).getClassName() == "->")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        this->counter++;
        if (this->fn_call_or_null_class())
          if (this->trail_class())
            return true;
      }
    }
    return false;
  }

  bool trail_or_fn_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "(")
    {
      if (this->fn_call_class())
        if (this->trail_class())
          return true;
    }
    else
    {
      if (temp == "[" || temp == ".")
      {
        if (this->trail_class())
          if (this->inc_dec_or_null_class())
            return true;
      }
      else
      {
        if (temp == "MDM" || temp == "PM" || temp == "ROP" || temp == "&&" || temp == "||" || temp == ")" || temp == "]" || temp == "Terminator" || temp == ";" || temp == ",")
          return true;
      }
    }
    return false;
  }

  bool fn_call_class()
  {
    if (this->lexemes.at(this->counter).getClassName() == "(")
    {
      this->counter++;
      if (this->OEs_class_or_null())
        if (this->lexemes.at(this->counter).getClassName() == ")")
        {
          this->counter++;
          return true;
        }
    }
    return false;
  }

  bool inc_dec_or_null_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "inc_dec")
    {
      this->counter++;
      return true;
    }
    else
    {
      if (temp == "MDM" || temp == "PM" || temp == "ROP" || temp == "&&" || temp == "||" || temp == ")" || temp == "]" || temp == "Terminator" || temp == ";" || temp == ",")
        return true;
    }
    return false;
  }

  bool trail_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        this->counter++;

        if (this->fn_call_or_null_class())
          if (this->trail_class())
            return true;
      }
    }
    else
    {
      if (temp == "[")
      {
        this->counter++;
        if (this->OE_class())
        {
          if (this->lexemes.at(this->counter).getClassName() == "]")
          {
            this->counter++;
            if (this->trail_no_arr_class())
              return true;
          }
        }
      }
      else
      {
        if (temp == "MDM" || temp == "PM" || temp == "ROP" || temp == "&&" || temp == "||" || temp == ")" || temp == "]" || temp == "Terminator" || temp == ";" || temp == "," || temp == "inc_dec")
          return true;
      }
    }
    return false;
  }

  bool trail_no_arr_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        this->counter++;
        if (this->fn_call_or_null_class())
          if (this->trail_class())
            return true;
      }
    }
    else
    {
      if (temp == "MDM" || temp == "PM" || temp == "ROP" || temp == "&&" || temp == "||" || temp == ")" || temp == "]" || temp == "Terminator" || temp == ";" || temp == "," || temp == "inc_dec")
        return true;
    }

    return false;
  }

  bool fn_call_or_null_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "(")
    {
      if (this->fn_call_class())
        return true;
    }
    else
    {
      if (temp == "MDM" || temp == "PM" || temp == "ROP" || temp == "&&" || temp == "||" || temp == ")" || temp == "]" || temp == "Terminator" || temp == ";" || temp == "," || temp == "inc_dec" || temp == "." || temp == "[")
        return true;
    }
    return false;
  }

  bool trail_no_arr_no_fc_end_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        this->counter++;
        if (this->fn_call_class_or_null_no_fc_end())
          return true;
      }
    }
    else
    {
      if (temp == "=" || temp == "inc_dec")
        return true;
    }
    return false;
  }

  bool MDME_class1()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "MDM")
    {
      this->counter++;
      if (this->OPs_class())
        if (this->MDME_class1())
          return true;
    }
    else
    {
      string temp = this->lexemes.at(this->counter).getClassName();
      if (temp == "]" || temp == ")" || temp == "," || temp == "Terminator" || temp == ";" || temp == "||" || temp == "&&" || temp == "ROP" || temp == "PM")
      {
        return true;
      }
    }
  }

  bool PME_class1()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "PM")
    {
      this->counter++;
      if (this->MDME_class())
        if (this->PME_class1())
          return true;
    }
    else
    {
      string temp = this->lexemes.at(this->counter).getClassName();
      if (temp == "]" || temp == ")" || temp == "," || temp == "Terminator" || temp == ";" || temp == "||" || temp == "&&" || temp == "ROP")
      {
        return true;
      }
    }
  }

  bool RE_class1()
  {
    if (this->lexemes.at(this->counter).getClassName() == "ROP")
    {
      this->counter++;
      if (this->PME_class())
        if (this->RE_class1())
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

  bool AE_class1()
  {
    if (this->lexemes.at(this->counter).getClassName() == "&&")
    {
      this->counter++;
      if (this->RE_class())
        if (this->AE_class1())
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

  bool OE_class1()
  {
    if (this->lexemes.at(this->counter).getClassName() == "||")
    {
      this->counter++;
      if (this->AE_class())
        if (this->OE_class1())
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

  bool trail_oe_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        this->counter++;
        if (this->function_or_trail_class())
          return true;
      }
    }
    else
    {
      if (temp == "[")
      {
        this->counter++;
        if (this->OE_class())
          if (this->lexemes.at(this->counter).getClassName() == "]")
          {
            this->counter++;
            if (this->trail_oe_no_arr_class())
              return true;
          }
      }
      else
      {
        if (temp == "=" || temp == "inc_dec")
          if (this->assign_or_inc_dec_class())
            return true;
      }
    }
    return false;
  }

  bool assign_or_inc_dec_class()
  {
    if (this->lexemes.at(this->counter).getClassName() == "inc_dec")
    {
      this->counter++;
      return true;
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == "=")
        if (this->assign_st_class())
          return true;
    }
    return false;
  }

  bool assign_st_class()
  {
    if (this->lexemes.at(this->counter).getClassName() == "=")
    {
      this->counter++;
      if (this->OE_class())
        return true;
    }
    return false;
  }

  bool function_or_trail_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "(")
    {
      if (this->fn_call_class())
        if (this->trail_oe_null_class())
          return true;
    }
    else
    {
      if (temp == "." || temp == "[" || temp == "inc_dec" || temp == "=")
        if (this->trail_oe_class())
          return true;
    }
    return false;
  }

  bool trail_oe_null_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (temp == "ID")
      {
        this->counter++;
        if (this->function_or_trail_class())
          return true;
      }
    }
    else
    {
      if (temp == "[")
      {
        this->counter++;
        if (this->OE_class())
          if (this->lexemes.at(this->counter).getClassName() == "]")
          {
            this->counter++;
            if (this->trail_oe_no_arr_class())
              return true;
          }
      }
      else
      {
        if (temp == "Terminator" || temp == ",")
        {
          return true;
        }
      }
    }
    return false;
  }

  bool trail_oe_no_arr_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        this->counter++;
        if (this->function_or_trail_class())
          return true;
      }
    }
    else
    {
      this->counter;
      if (temp == "=" || temp == "inc_dec")
        if (this->assign_or_inc_dec_class())
          return true;
    }
    return false;
  }

  bool list1_class()
  {
    if (this->lexemes.at(this->counter).getClassName() == ",")
    {
      this->counter++;
      if (this->obj_dec_class())
        return true;
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == "Terminator")
        return true;
    }
    return false;
  }

  bool next1_class()
  {
    if (this->lexemes.at(this->counter).getClassName() == ",")
    {
      this->counter++;
      if (this->OEs_class())
        return true;
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == ")")
        return true;
    }
    return false;
  }

  //========== class end =============================================================================================

  bool SST_IFL2()
  {
    string temp = this->lexemes.at(this->counter).getClassName();

    if (temp == "static" || temp == "#" || temp == "ID" || temp == "DT" || temp == "if" || temp == "loop" || temp == "return" || temp == "exit" || temp == "inc_dec" || temp == "Terminator" || temp == "var" || temp == "executionMod")
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
              if (this->term())
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
    if (temp == "#" || temp == "ID")
    {
      if (this->p())
        if (this->lexemes.at(this->counter).getClassName() == "ID")
        {
          this->counter++;
          if (this->next())
            return true;
        }
    }
    return false;
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
    if (temp == "{")
    {
      this->counter++;
      if (this->MST_IFL())
      {
        if (this->lexemes.at(this->counter).getClassName() == "}")
        {
          this->counter++;
          return true;
        }
      }
    }
    return false;
  }

  bool MST_IFL()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "static" || temp == "#" || temp == "ID" || temp == "DT" || temp == "if" || temp == "loop" || temp == "return" || temp == "exit" || temp == "inc_dec" || temp == "Terminator" || temp == "var" || temp == "executionMod")
    {
      if (this->SST_IFL())
        if (this->MST_IFL())
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

    if (temp == "executionMod")
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
      if (this->function_or_dec())
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
      if (this->fun_st())
        return true;
    }else{

    if (this->lexemes.at(this->counter).getClassName() == "DT" || this->lexemes.at(this->counter).getClassName() == "var")
    {
      if (this->Dec())
        return true;
    }else{

    if (this->lexemes.at(this->counter).getClassName() == "ID")
    {
      this->counter++;
      if (this->obj_dec())
      {
        if (this->lexemes.at(this->counter).getClassName() == "Terminator")
        {
          this->counter++;
          return true;
        }
      }
    }
    }
    }
    return false;
  }

  bool inc_dec_start()
  {
    if (this->lexemes.at(this->counter).getClassName() == "inc_dec")
    {
      this->counter++;
      if (this->p())
        if (this->lexemes.at(this->counter).getClassName() == "ID")
        {
          this->counter++;
          if (this->static_ref_or_null())
            if (this->trail())
              if (this->lexemes.at(this->counter).getClassName() == "Terminator")
              {
                this->counter++;
                return true;
              }
        }
    }
    return false;
  }

  bool return_st()
  {
    if (this->lexemes.at(this->counter).getClassName() == "return")
    {
      this->counter++;
      if (this->OE())
        if (this->lexemes.at(this->counter).getClassName() == "Terminator")
        {
          this->counter++;
          return true;
        }
    }
    return false;
  }

  bool exit_st()
  {
    if (this->lexemes.at(this->counter).getClassName() == "exit")
    {
      this->counter++;
      if (this->exit_st1())
        if (this->lexemes.at(this->counter).getClassName() == "Terminator")
        {
          this->counter++;
          return true;
        }
    }
    return false;
  }

  bool exit_st1()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "#" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (this->const_or_ID())
        return true;
    }
    else
    {
      if (temp == "(")
      {
        this->counter++;
        if (this->const_or_ID())
          if (this->lexemes.at(this->counter).getClassName() == ")")
          {
            this->counter++;
            return true;
          }
      }
    }
    return false;
  }

  bool const_or_ID()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (this->CONST())
        return true;
    }
    else
    {
      if (temp == "ID" || temp == "#")
      {
        if (this->p())
          if (this->lexemes.at(this->counter).getClassName() == "ID")
          {
            this->counter++;
            if (this->trail())
              return true;
          }
      }
    }
    return false;
  }

  bool loop_st()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "loop")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "(")
      {
        this->counter++;
        if (this->c1())
          if (this->c2())
            if (this->c3())
              if (this->lexemes.at(this->counter).getClassName() == ")")
              {
                this->counter++;
                if (this->term())
                  if (this->body_fn())
                    return true;
              }
      }
    }
    return false;
  }

  bool c1()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "DT" || temp == "var")
    {
      if (this->DEC_st1())
        if (this->lexemes.at(this->counter).getClassName() == ";")
        {
          this->counter++;
          return true;
        }
    }
    else
    {
      if (temp == "#" || temp == "ID")
      {
        if (this->assign_st1())
          if (this->lexemes.at(this->counter).getClassName() == ";")
          {
            this->counter++;
            return true;
          }
      }
      else
      {
        if (temp == ";")
        {
          this->counter++;
          return true;
        }
      }
    }
    return false;
  }

  bool DEC_st1()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "DT" || temp == "var")
    {
      if (this->DTs())
      {
        if (this->p_st_or_null_p_st())
        {
          if (this->list_loop())
            return true;
        }
      }
    }
    return false;
  }

  bool list_loop()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ",")
    {
      this->counter++;
      if (this->p_st_or_null_p_st())
        if (this->list_loop())
          return true;
    }
    else
    {
      if (temp == ";")
      {
        return true;
      }
    }
    return false;
  }

  bool assign_st1()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "#" || temp == "ID")
    {
      if (this->p())
        if (this->lexemes.at(this->counter).getClassName() == "ID")
        {
          this->counter++;
          if (this->static_ref_or_null())
            if (this->fn_call_or_null_no_fc_end())
              if (this->assign_st())
                return true;
        }
    }
    return false;
  }

  bool trail_no_null()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        this->counter++;
        if (this->fn_call_or_null_no_fc_end())
          return true;
      }
    }
    else
    {
      if (temp == "[")
      {
        this->counter++;
        if (this->OE())
          if (this->lexemes.at(this->counter).getClassName() == "]")
          {
            this->counter++;
            if (this->trail_no_arr_no_fc_end())
              return true;
          }
      }
    }
    return false;
  }

  bool trail_no_fc_end()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        this->counter++;
        if (this->fn_call_or_null_no_fc_end())
          return true;
      }
    }
    else
    {
      if (temp == "[")
      {
        this->counter++;
        if (this->OE_class())
          if (this->lexemes.at(this->counter).getClassName() == "]")
          {
            this->counter++;
            if (this->trail_no_arr_no_fc_end())
              return true;
          }
      }
      else
      {
        if (temp == "=" || temp == "inc_dec")
          return true;
      }
    }
    return false;
  }

  bool fn_call_or_null_no_fc_end()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "(")
    {
      if (this->fn_call())
        if (this->trail_no_null())
          return true;
    }
    else
    {
      if (temp == "=" || temp == "." || temp == "[" || temp == "inc_dec")
        if (this->trail_no_fc_end())
          return true;
    }
    return false;
  }

  bool c2()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (this->OE())
        if (this->lexemes.at(this->counter).getClassName() == ";")
        {
          this->counter++;
          return true;
        }
    }
    else
    {
      if (temp == ";")
      {
        this->counter++;
        return true;
      }
    }
    return false;
  }

  bool c3()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "#" || temp == "ID")
    {
      if (this->assign_st1_or_inc_dec1())
        return true;
    }
    else
    {
      if (temp == "inc_dec")
      {
        if (this->inc_dec_start1())
          return true;
      }
      else
      {
        if (temp == ")")
          return true;
      }
    }
    return false;
  }

  bool assign_st1_or_inc_dec1()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "#" || temp == "ID")
    {
      if (this->p())
        if (this->lexemes.at(this->counter).getClassName() == "ID")
        {
          this->counter++;
          if (this->static_ref_or_null())
            if (this->fn_call_or_null_no_fc_end())
              if (this->assign_or_inc_dec())
              return true;
        }
    }
    return false;
  }

  bool inc_dec_start1()
  {
    if (this->lexemes.at(this->counter).getClassName() == "inc_dec")
    {
      this->counter++;
      if (this->p())
        if (this->lexemes.at(this->counter).getClassName() == "ID")
        {
          this->counter++;
          if (this->static_ref_or_null())
            if (this->trail())
              return true;
        }
    }
    return false;
  }

  bool if_else()
  {
    if (this->lexemes.at(this->counter).getClassName() == "if")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "(")
      {
        this->counter++;
        if (this->OE())
          if (this->lexemes.at(this->counter).getClassName() == ")")
          {
            this->counter++;
            if (this->term())
              if (this->body_fn())
                if (this->oelse())
                  return true;
          }
      }
    }
    return false;
  }

  bool oelse()
  {
    if (this->lexemes.at(this->counter).getClassName() == "Terminator")
    {
      this->counter++;
      return true;
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == "else")
      {
        this->counter++;
        if (this->term())
          if (this->body_fn())
            return true;
      }
    }
    return false;
  }

  bool hash_or_ID_start()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "#")
    {
      this->counter++;
      if (temp == "ID")
      {
        this->counter++;
        if (this->static_ref_or_null())
          if (this->trail_oe_or_fn())
            if (this->lexemes.at(this->counter).getClassName() == "Terminator")
            {
              this->counter++;
              return true;
            }
      }
    }
    else
    {
      if (temp == "ID")
      {
        this->counter++;
        if (this->trail_or_obj())
          if (this->lexemes.at(this->counter).getClassName() == "Terminator")
          {
            this->counter++;
            return true;
          }
      }
    }
    return false;
  }

  bool trail_oe_or_fn()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "(")
    {
      if (this->fn_call())
        return true;
    }
    else
    {
      if (temp == "." || temp == "[" || temp == "=" || temp == "inc_dec")
        if (this->trail_oe())
          return true;
    }
    return false;
  }

  bool trail_or_obj()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "#" || temp == "ID")
    {
      if (this->obj_dec())
      {
        return true;
      }
    }
    else
    {
      if (temp == "::" || temp == "inc_dec" || temp == "=" || temp == "." || temp == "(" || temp == "[")
        if (this->static_ref_or_null())
          if (this->trail_oe_or_fn())
            return true;
    }
    return false;
  }

  bool Dec()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "DT" || temp == "var")
    {
      if (this->DTs())
      {
        if (this->p_st_or_null_p_st())
        {
          if (this->list())
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
        if (this->new_init())
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
        if (this->init())
        {
          return true;
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
      if (this->new_arr_const())
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
    if (this->lexemes.at(this->counter).getClassName() == "new")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "DT")
      {
        this->counter++;
        if (this->arr_or_null())
        {
          return true;
        }
      }
    }
    else
    {
      string temp = this->lexemes.at(this->counter).getClassName();
      if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
      {
        if (this->OE())
          return true;
      }
    }
    return false;
  }

  bool arr_or_null()
  {
    if (this->lexemes.at(this->counter).getClassName() == "[")
    {
      this->counter++;
      if (this->OE())
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
      if (this->lexemes.at(this->counter).getClassName() == "," || this->lexemes.at(this->counter).getClassName() == "Terminator")
      {
        return true;
      }
    }
    return false;
  }

  bool init()
  {
    if (this->lexemes.at(this->counter).getClassName() == "=")
    {
      this->counter++;
      if (this->new_arr_init())
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
    if (temp == "function" || temp == "static" || temp == "#" || temp == "ID" || temp == "DT" || temp == "if" || temp == "loop" || temp == "return" || temp == "exit" || temp == "inc_dec" || temp == "Terminator" || temp == "var" || temp == "class" || temp == "abstract")
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
    if (temp == "new")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "[")
      {
        this->counter++;
        if (this->OE())
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
      if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
      {
        if (this->OE())
          return true;
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
      if (this->p_st_or_null_p_st())
        if (this->list())
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
      if (this->p())
      {
        if (this->lexemes.at(this->counter).getClassName() == "ID")
        {
          this->counter++;
          if (this->init1())
            if (this->list1())
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
      if (this->OE())
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
          if (this->lexemes.at(this->counter).getClassName() == "ID")
          {
            this->counter++;
            if (this->pl_or_arr())
            {
              return true;
            }
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
      if (this->OEs_or_null())
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
        if (this->OE())
          if (this->lexemes.at(this->counter).getClassName() == "]")
          {
            this->counter++;
            return true;
          }
      }
    }
    return false;
  }

  bool OEs_or_null()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (this->OEs())
        return true;
    }
    else
    {
      if (temp == ")")
        return true;
    }
    return false;
  }

  bool OEs()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (this->OE())
        if (this->next1())
          return true;
    }
    return false;
  }

  bool OE()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (this->AE())
        if (this->OE1())
          return true;
    }
    return false;
  }

  bool AE()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (this->RE())
        if (this->AE1())
          return true;
    }
    return false;
  }

  bool RE()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (this->PME())
        if (this->RE1())
          return true;
    }
    return false;
  }

  bool PME()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (this->MDME())
        if (this->PME1())
          return true;
    }
    return false;
  }

  bool MDME()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (this->OPs())
        if (this->MDME1())
          return true;
    }
    return false;
  }

  bool OPs()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "#" || temp == "ID")
    {
      if (this->p())
      {
        if (this->lexemes.at(this->counter).getClassName() == "ID")
        {
          this->counter++;
          if (this->static_ref_or_null())
          {
            if (this->trail_or_fn())
              return true;
          }
        }
      }
    }
    else
    {
      if (temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst" || temp == "IntConst")
      {
        this->counter++;
        return true;
      }
      else
      {
        if (temp == "(")
        {
          this->counter++;
          if (this->OE())
          {
            if (this->lexemes.at(this->counter).getClassName() == ")"){
              this->counter++;
              return true;
              }
          }
        }
        else
        {
          if (temp == "!")
          {
            this->counter++;
            if (this->OPs())
              return true;
          }
          else
          {
            if (temp == "inc_dec")
            {
              this->counter++;
              if (this->lexemes.at(this->counter).getClassName() == "ID")
              {
                this->counter++;
                if (this->trail())
                  return true;
              }
            }
          }
        }
      }
    }
    return false;
  }

  bool trail_or_fn()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "(")
    {
      if (this->fn_call())
        if (this->trail())
          return true;
    }
    else
    {
      if (temp == "[" || temp == ".")
      {
        if (this->trail())
          if (this->inc_dec_or_null())
            return true;
      }
      else
      {
        if (temp == "MDM" || temp == "PM" || temp == "ROP" || temp == "&&" || temp == "||" || temp == ")" || temp == "]" || temp == "Terminator" || temp == ";" || temp == ",")
          return true;
      }
    }
    return false;
  }

  bool fn_call()
  {
    if (this->lexemes.at(this->counter).getClassName() == "(")
    {
      this->counter++;
      if (this->OEs_or_null())
        if (this->lexemes.at(this->counter).getClassName() == ")")
        {
          this->counter++;
          return true;
        }
    }
    return false;
  }

  bool inc_dec_or_null()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "inc_dec")
    {
      this->counter++;
      return true;
    }
    else
    {
      if (temp == "MDM" || temp == "PM" || temp == "ROP" || temp == "&&" || temp == "||" || temp == ")" || temp == "]" || temp == "Terminator" || temp == ";" || temp == ",")
        return true;
    }
    return false;
  }

  bool trail()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        this->counter++;

        if (this->fn_call_or_null())
          if (this->trail())
            return true;
      }
    }
    else
    {
      if (temp == "[")
      {
        this->counter++;
        if (this->OE())
        {
          if (this->lexemes.at(this->counter).getClassName() == "]")
          {
            this->counter++;
            if (this->trail_no_arr())
              return true;
          }
        }
      }
      else
      {
        if (temp == "MDM" || temp == "PM" || temp == "ROP" || temp == "&&" || temp == "||" || temp == ")" || temp == "]" || temp == "Terminator" || temp == ";" || temp == "," || temp == "inc_dec")
          return true;
      }
    }
    return false;
  }

  bool trail_no_arr()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        this->counter++;
        if (this->fn_call_or_null())
          if (this->trail())
            return true;
      }
    }
    else
    {
      if (temp == "MDM" || temp == "PM" || temp == "ROP" || temp == "&&" || temp == "||" || temp == ")" || temp == "]" || temp == "Terminator" || temp == ";" || temp == "," || temp == "inc_dec")
        return true;
    }
    return false;
  }

  bool fn_call_or_null()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "(")
    {
      if (this->fn_call())
        return true;
    }
    else
    {
      if (temp == "MDM" || temp == "PM" || temp == "ROP" || temp == "&&" || temp == "||" || temp == ")" || temp == "]" || temp == "Terminator" || temp == ";" || temp == "," || temp == "inc_dec" || temp == "." || temp == "[")
        return true;
    }
    return false;
  }

  bool trail_no_arr_no_fc_end()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        this->counter++;
        if (this->fn_call_or_null_no_fc_end())
          return true;
      }
    }
    else
    {
      if (temp == "=" || temp == "inc_dec")
        return true;
    }
    return false;
  }

  bool MDME1()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "MDM")
    {
      this->counter++;
      if (this->OPs())
        if (this->MDME1())
          return true;
    }
    else
    {
      string temp = this->lexemes.at(this->counter).getClassName();
      if (temp == "]" || temp == ")" || temp == "," || temp == "Terminator" || temp == ";" || temp == "||" || temp == "&&" || temp == "ROP" || temp == "PM")
      {
        return true;
      }
    }
  }

  bool PME1()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "PM")
    {
      this->counter++;
      if (this->MDME())
        if (this->PME1())
          return true;
    }
    else
    {
      string temp = this->lexemes.at(this->counter).getClassName();
      if (temp == "]" || temp == ")" || temp == "," || temp == "Terminator" || temp == ";" || temp == "||" || temp == "&&" || temp == "ROP")
      {
        return true;
      }
    }
  }

  bool RE1()
  {
    if (this->lexemes.at(this->counter).getClassName() == "ROP")
    {
      this->counter++;
      if (this->PME())
        if (this->RE1())
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
      if (this->RE())
        if (this->AE1())
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
      if (this->AE())
        if (this->OE1())
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
    if (this->lexemes.at(this->counter).getClassName() == "::")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        this->counter++;
        return true;
      }
    }
    else
    {
      string temp = this->lexemes.at(this->counter).getClassName();
      if (temp == "." || temp == "[" || temp == "Terminator" || temp == "(" || temp == "inc_dec" || temp == "MDM" || temp == "PM" || temp == "ROP" || temp == "||" || temp == "&&" || temp == "]" || temp == ")" || temp == "," || temp == ";")
        return true;
    }
    return false;
  }

  bool trail_oe()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        this->counter++;
        if (this->function_or_trail())
          return true;
      }
    }
    else
    {
      if (temp == "[")
      {
        this->counter++;
        if (this->OE())
          if (this->lexemes.at(this->counter).getClassName() == "]")
          {
            this->counter++;
            if (this->trail_oe_no_arr())
              return true;
          }
      }
      else
      {
        if (temp == "=" || temp == "inc_dec")
          if (this->assign_or_inc_dec())
            return true;
      }
    }
    return false;
  }

  bool assign_or_inc_dec()
  {
    if (this->lexemes.at(this->counter).getClassName() == "inc_dec")
    {
      this->counter++;
      return true;
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == "=")
        if (this->assign_st())
          return true;
    }
    return false;
  }

  bool assign_st()
  {
    if (this->lexemes.at(this->counter).getClassName() == "=")
    {
      this->counter++;
      if (this->OE())
        return true;
    }
    return false;
  }

  bool function_or_trail()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "(")
    {
      if (this->fn_call())
        if (this->trail_oe_null())
          return true;
    }
    else
    {
      if (temp == "." || temp == "[" || temp == "inc_dec" || temp == "=")
        if (this->trail_oe())
          return true;
    }
    return false;
  }

  bool trail_oe_null()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (temp == "ID")
      {
        this->counter++;
        if (this->function_or_trail())
          return true;
      }
    }
    else
    {
      if (temp == "[")
      {
        this->counter++;
        if (this->OE())
          if (this->lexemes.at(this->counter).getClassName() == "]")
          {
            this->counter++;
            if (this->trail_oe_no_arr())
              return true;
          }
      }
      else
      {
        if (temp == "Terminator" || temp == ",")
        {
          return true;
        }
      }
    }
    return false;
  }

  bool trail_oe_no_arr()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        this->counter++;
        if (this->function_or_trail())
          return true;
      }
    }
    else
    {
      if (temp == "=" || temp == "inc_dec")
        if (this->assign_or_inc_dec())
          return true;
    }
    return false;
  }

  bool list1()
  {
    if (this->lexemes.at(this->counter).getClassName() == ",")
    {
      this->counter++;
      if (this->obj_dec())
        return true;
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == "Terminator")
        return true;
    }
    return false;
  }

  bool next1()
  {
    if (this->lexemes.at(this->counter).getClassName() == ",")
    {
      this->counter++;
      if (this->OEs())
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
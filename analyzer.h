#include <iostream>
#include <vector>
#include <bits/stdc++.h>
#include <string>
#include <regex>
#include "lexeme.h"
#include "Words.h"
#include "symbolTable.h"
using namespace std;

class Analyzer
{
public:
  Analyzer()
  {
    this->counter = 0;
    this->fromFunc = false;
    this->code = "";
    this->label = 0;
    this->Register = 0;
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

  bool from_fun()
  {
    if (this->fromFunc)
    {
      this->fromFunc = false;
      return true;
    }

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

    sym.CreateScope();

    if (this->start())
    {
      for (int i = 0; i < this->semErrors.size(); i++)
      {
        cout << this->semErrors.at(i) << endl;
      }

    ofstream file("ICG.txt");

    for (int i = 0; i < this->code.length(); i++)
    {
      file <<this->code[i];
    }
      

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
      string TM = "";
      if (this->abstract_st(TM))
        if (this->lexemes.at(this->counter).getClassName() == "class")
        {
          string type = "class";
          this->counter++;
          if (this->lexemes.at(this->counter).getClassName() == "ID")
          {
            string N = this->lexemes.at(this->counter).getWord();
            this->counter++;
            string Parent = "";
            if (this->extends_st(Parent))
              if (this->term())
              {
                sym.setCurrentClass(N);
                Data d(N, type, Parent, TM);
                if (!sym.insertData(d))
                {
                  this->semErrors.push_back("Class Redeclaration at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
                }

                if (this->class_body())
                  return true;
              }
          }
        }
    }
    return false;
  }

  bool abstract_st(string &TM)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "abstract")
    {
      TM = this->lexemes.at(this->counter).getWord();

      this->counter++;
      return true;
    }
    else
    {
      if (temp == "class")
      {
        TM = "";
        return true;
      }
    }
    return false;
  }

  bool extends_st(string &P)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "extends")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        P = this->lexemes.at(this->counter).getWord();
        string TM = "";
        if (sym.lookupDT(P, TM) != "class")
        {
          this->semErrors.push_back("Can't inherit from undeclared class " + P + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }
        this->counter++;
        return true;
      }
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == "Terminator" || this->lexemes.at(this->counter).getClassName() == "{")
      {
        P = "";
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
          sym.setCurrentClass("");
          this->counter++;
          return true;
        }
    }
    return false;
  }

  bool MST_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "accessModifier" || temp == "Terminator")
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
    if (temp == "accessModifier")
    {
      string AM = this->lexemes.at(this->counter).getWord();

      this->counter++;
      if (this->func_or_dec_objdec(AM))
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

  bool constructor_st(string N, string AM, string TM)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (this->lexemes.at(this->counter).getClassName() == "(")
    {
      sym.CreateScope();
      int count = 0;
      this->counter++;
      if (this->pl(count))
        if (this->lexemes.at(this->counter).getClassName() == ")")
        {
          ClassData d(N, to_string(count), AM, TM);

          if (!sym.insertClassData(sym.getCurrentClass(), d))
          {
            semErrors.push_back("Constructor Redeclaration at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }
          this->counter++;
          if (this->term())
            if (this->lexemes.at(this->counter).getClassName() == "{")
            {
              this->counter++;
              if (this->body_con())
                if (this->lexemes.at(this->counter).getClassName() == "}")
                {
                  sym.DestroyScope();
                  this->counter++;
                  return true;
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

  bool func_or_dec_objdec(string AM)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "function" || temp == "virtual" || temp == "abstract" || temp == "DT" || temp == "var" || temp == "ID")
    {
      if (this->f_d_o(AM))
        return true;
    }
    else
    {
      if (temp == "static")
      {
        string TM = this->lexemes.at(this->counter).getWord();

        this->counter++;
        if (this->f_d_o_stat(AM, TM))
          return true;
      }
    }
    return false;
  }

  bool f_d_o(string AM)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "virtual" || temp == "abstract" || temp == "function")
    {
      string TM = "";
      if (this->VA(TM))
        if (this->fun_st_class(AM, TM))
          return true;
    }
    else
    {
      if (temp == "DT" || temp == "var")
      {
        string TM = "";
        if (this->DEC_class(AM, TM, false))
          return true;
      }
      else
      {
        if (temp == "ID")
        {
          string TM = "";
          string N = this->lexemes.at(this->counter).getWord();

          this->counter++;
          if (this->constructor_or_obj_dec(N, AM, TM))
            return true;
        }
      }
    }
    return false;
  }

  bool f_d_o_stat(string AM, string TM)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "function")
    {
      if (this->fun_st_class(AM, TM))
        return true;
    }
    else
    {
      if (temp == "DT" || temp == "var")
      {
        if (this->DEC_class(AM, TM, false))
          return true;
      }
      else
      {
        if (temp == "ID")
        {
          string N = this->lexemes.at(this->counter).getWord();

          this->counter++;
          if (this->constructor_or_obj_dec(N, AM, TM))
            return true;
        }
      }
    }
    return false;
  }

  bool constructor_or_obj_dec(string N, string AM, string TM)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "(")
    {
      if (this->constructor_st(N, AM, TM))
        return true;
    }
    else
    {
      if (temp == "ID" || temp == "#")
      {
        string TM = "";
        if (sym.lookupDT(N, TM) != "class")
        {
          this->semErrors.push_back("Undeclared Class " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }

        if (TM == "abstract")
        {
          this->semErrors.push_back("Cannot declare object for abstract class at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }

        if (this->obj_dec_class(N, AM, TM, false))
          if (this->lexemes.at(this->counter).getClassName() == "Terminator")
          {
            this->counter++;
            return true;
          }
      }
    }
    return false;
  }

  bool VA(string &TM)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "virtual" || temp == "abstract")
    {
      TM = this->lexemes.at(this->counter).getWord();

      this->counter++;
      return true;
    }
    else
    {
      if (temp == "function")
      {
        TM = "";
        return true;
      }
    }
    return false;
  }

  bool fun_st_class(string AM, string TM)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "function")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        string N = this->lexemes.at(this->counter).getWord();

        this->counter++;
        if (this->lexemes.at(this->counter).getClassName() == "(")
        {
          sym.CreateScope();

          int count = 0;
          this->counter++;
          if (this->pl(count))
          {
            this->code += this->sym.getCurrentClass() +  N + "_" + to_string(count) + " proc\n";
            Data d(N, to_string(count), "", "");
            ClassData da(N, to_string(count), AM, TM);

            if (!sym.insertClassData(sym.getCurrentClass(), da))
            {
              semErrors.push_back("Function Redeclaration at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            if (this->lexemes.at(this->counter).getClassName() == ")")
            {

              this->counter++;
              if (this->term())
                if (this->body_fn_class()){
                  this->code += N + " Endp\n";
                  return true;
                  }
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
          sym.DestroyScope();
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
      if (this->DEC_class("", "", true))
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
      if (this->DEC_class("", "", true))
        return true;
    }
    else
    {

      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        string N = this->lexemes.at(this->counter).getWord();

        this->counter++;
        if (this->obj_dec_class(N, "", "", true))
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
      string OP = this->lexemes.at(this->counter).getWord();

      this->counter++;
      if (this->this_or_ID_inc_dec(OP))
        if (this->lexemes.at(this->counter).getClassName() == "Terminator")
        {
          this->counter++;
          return true;
        }
    }
    return false;
  }

  bool this_or_ID_inc_dec(string OP)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "#" || temp == "ID")
    {
      string name = "";
      if (this->p(name))
        if (this->lexemes.at(this->counter).getClassName() == "ID")
        {
          string N = this->lexemes.at(this->counter).getWord();

          bool statCheck = false;
          string T = "";

          this->counter++;
          string name = "";
          if (this->static_ref_or_null(statCheck, T, N,name))
            if (this->trail_class(N, statCheck, T,name))
            {

              T = sym.compatibilityCheck(T, OP);
              if (T == "Uncompatible")
              {

                this->semErrors.push_back("Invalid operator with type at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }
              return true;
            }
        }
    }
    else
    {
      if (temp == "this")
      {
        string T = sym.getCurrentClass();
        this->counter++;
        string name = "";
        if (this->trail_this(name,T))
        {

          T = sym.compatibilityCheck(T, OP);
          if (T == "Uncompatible")
          {

            this->semErrors.push_back("Invalid operator with type at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          return true;
        }
      }
    }
    return false;
  }

  bool return_st_class()
  {
    if (this->lexemes.at(this->counter).getClassName() == "return")
    {
      string RT = "";
      this->counter++;
      if (this->OE_class(RT))
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
      string T = "";
      if (this->CONST(T))
      {
        if (T != "int")
        {
          this->semErrors.push_back("Exit code should be int at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }
        return true;
      }
    }
    else
    {
      if (temp == "ID" || temp == "#")
      {
        string name = "";

        if (this->p(name))
          if (this->lexemes.at(this->counter).getClassName() == "ID")
          {
            string N = this->lexemes.at(this->counter).getWord();

            bool statCheck = false;
            string T = "";
            string name = "";
            this->counter++;
            if (this->static_ref_or_null(statCheck, T, N,name))
              if (this->trail_class(N, statCheck, T,name))
              {
                if (T != "int")
                {
                  this->semErrors.push_back("Exit code should be int at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
                }
                return true;
              }
          }
      }
      else
      {
        if (temp == "this")
        {
          string T = sym.getCurrentClass();
          this->counter++;
          string name = "";
          if (this->trail_this(name,T))
          {
            if (T != "int")
            {
              this->semErrors.push_back("Exit code should be int at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }
            return true;
          }
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
        sym.CreateScope();
        this->counter++;
        if (this->c1_class()){
          
          this->code += "L" + to_string(++this->label) + " :\n";
          
          if (this->c2_class()){
            string tempLab = to_string(this->label);

            this->code += "if( T" + to_string(this->Register) + " == false) jmp L" + to_string(++this->label) + " :\n";

            string tempLab2 = to_string(this->label);
            if (this->c3_class())
              if (this->lexemes.at(this->counter).getClassName() == ")")
              {
                this->counter++;
                if (this->term())
                  if (this->body_fn_class()){
                    
                    this->code += "jmp L" + tempLab + "\n";
                    this->code += "L" + tempLab2 + " :\n"; 
                    return true;
                    }
              }
              }
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
      string T = "";
      if (this->DTs(T))
      {
        if (this->p_st_or_null_p_st_class(T, "", "", true))
        {
          if (this->list_class_loop(T))
            return true;
        }
      }
    }
    return false;
  }

  bool list_class_loop(string T)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ",")
    {
      this->counter++;
      if (this->p_st_or_null_p_st_class(T, "", "", true))
        if (this->list_class_loop(T))
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
      string T = "";
      if (this->assign_st_this_or_ID(T))
        if (this->assign_st_class(T))
          return true;
    }
    return false;
  }

  bool assign_st_this_or_ID(string &T)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "#" || temp == "ID")
    {
      string name = "";
      if (this->p(name))
        if (this->lexemes.at(this->counter).getClassName() == "ID")
        {
          string N = this->lexemes.at(this->counter).getWord();

          bool statCheck = false;
          this->counter++;
          if (this->static_ref_or_null(statCheck, T, N,name))
            if (this->fn_call_class_or_null_no_fc_end(N, T, statCheck))
              return true;
        }
    }
    else
    {
      if (temp == "this")
      {
        T = sym.getCurrentClass();
        bool statCheck = false;
        this->counter++;
        if (this->trail_this_no_fc_end(T, statCheck))
          return true;
      }
    }
    return false;
  }

  bool trail_this_no_fc_end(string &T, bool &statCheck)
  {
    if (this->lexemes.at(this->counter).getClassName() == "->")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        string N = this->lexemes.at(this->counter).getWord();

        this->counter++;
        if (this->fn_call_class_or_null_no_fc_end(N, T, statCheck))
          return true;
      }
    }
    return false;
  }

  bool trail_no_null_class(string N, string &T, bool &statCheck)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        if (!from_fun())
        {
          if (T == "")
          {
            T = sym.lookupST(N);
            if (T == "none")
            {
              string parent = sym.getParent(sym.getCurrentClass());
              
              string AM = "",TM = "";

              if(parent != "none"){
                  T = sym.lookupClassData(parent,N,AM,TM);
                }

              if(AM == "private"){
                this->semErrors.push_back("Can't access private member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
                

              if(T == "none")
              this->semErrors.push_back(" Undeclared class object " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            N = this->lexemes.at(this->counter).getWord();
          }
          else
          {
            string AM = "", TM = "", temp = T,parentAM="",parentTM="";
            
            string parent = sym.getParent(T);
            
            T = sym.lookupClassData(T, N, AM, TM);

            if (T == "none")
            {
              if(parent != "none"){
                  T = sym.lookupClassData(parent,N,parentAM,parentTM);
                }

                if(T == "none")
              this->semErrors.push_back(" Undeclared class object " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            N = this->lexemes.at(this->counter).getWord();

            if (statCheck && TM != "static" && parentTM != "static")
            {
              this->semErrors.push_back("Called Function should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            if ((AM == "private" || AM == "protected") && temp != sym.getCurrentClass())
            {
              this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            if(parentAM == "private"){
                this->semErrors.push_back("Can't access private or member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
          }
        }
        statCheck = false;

        this->counter++;
        if (this->fn_call_class_or_null_no_fc_end(N, T, statCheck))
          return true;
      }
    }
    else
    {
      if (temp == "[")
      {
        this->counter++;
        string RT = "";
        if (this->OE_class(RT))
          if (this->lexemes.at(this->counter).getClassName() == "]")
          {
            if (RT != "int")
            {
              this->semErrors.push_back(" Uncompatible index type at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            this->counter++;
            if (this->trail_no_arr_no_fc_end_class(N, T, statCheck))
              return true;
          }
      }
    }
    return false;
  }

  bool trail_no_fc_end_class(string N, string &T, bool &statCheck)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        if (T == "")
        {
          T = sym.lookupST(N);
          if (T == "none")
          {
              string parent = sym.getParent(sym.getCurrentClass());
              
              string AM = "",TM = "";

              if(parent != "none"){
                  T = sym.lookupClassData(parent,N,AM,TM);
                }

              if(AM == "private"){
                this->semErrors.push_back("Can't access private member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
                

            if(T == "none")
            this->semErrors.push_back(" Undeclared class object " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          N = this->lexemes.at(this->counter).getWord();
        }
        else
        {
              string AM = "", TM = "",temp = T, parentAM="",parentTM="" , parent = sym.getParent(T);
          
          T = sym.lookupClassData(T, N, AM, TM);

          if (T == "none")
          {
            if(parent != "none"){
              T = sym.lookupClassData(parent,N,parentAM,parentTM);
              }

          if(T == "none")
            this->semErrors.push_back(" Undeclared class object " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          N = this->lexemes.at(this->counter).getWord();

          if (statCheck && TM != "static" && parentTM != "static")
          {
            this->semErrors.push_back("Called Function should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if ((AM == "private" || AM == "protected") && temp != sym.getCurrentClass())
          {
            this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

            if(parentAM == "private"){
                this->semErrors.push_back("Can't access private or member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
        }
        statCheck = false;

        this->counter++;
        if (this->fn_call_class_or_null_no_fc_end(N, T, statCheck))
          return true;
      }
    }
    else
    {
      if (temp == "[")
      {
        this->counter++;
        string RT = "";
        if (this->OE_class(RT))
          if (this->lexemes.at(this->counter).getClassName() == "]")
          {
            if (RT != "int")
            {
              this->semErrors.push_back(" Uncompatible index type at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            this->counter++;
            if (this->trail_no_arr_no_fc_end_class(N, T, statCheck))
            {
              return true;
            }
          }
      }
      else
      {
        if (temp == "=" || temp == "inc_dec")
        {
          if (T == "")
          {
            T = sym.lookupST(N);
            if (T == "none")
            {
              string parent = sym.getParent(sym.getCurrentClass());
              
              string AM = "",TM = "";

              if(parent != "none"){
                  T = sym.lookupClassData(parent,N,AM,TM);
                }

              if(AM == "private"){
                this->semErrors.push_back("Can't access private member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
                

              if(T == "none")
              this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }
          }
          else
          {
            string AM = "", TM = "",temp = T,parentAM="",parentTM="";
            
            string parent = sym.getParent(T);
            
            T = sym.lookupClassData(T, N, AM, TM);

            if (T == "none")
            {
              if(parent != "none"){
                T = sym.lookupClassData(parent,N,parentAM,parentTM);
              }

              if(T == "none")
              this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            if (statCheck && TM != "static" && parentTM != "static")
            {
              this->semErrors.push_back("Called Function should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            if ((AM == "private" || AM == "protected") && temp != sym.getCurrentClass())
            {
              this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }
                            
            if(parentAM == "private"){
              this->semErrors.push_back("Can't access private or member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
            }

          }
          statCheck = false;

          return true;
        }
      }
    }
    return false;
  }

  bool fn_call_class_or_null_no_fc_end(string N, string &T, bool &statCheck)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "(")
    {
      if (this->fn_call_class(T, N, statCheck))
        if (this->trail_no_null_class(N, T, statCheck))
          return true;
    }
    else
    {
      if (temp == "." || temp == "[" || temp == "=" || temp == "inc_dec")
        if (this->trail_no_fc_end_class(N, T, statCheck))
          return true;
    }
    return false;
  }

  bool c2_class()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "this" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      string T = "";
      if (this->OE_class(T))
      {
        if (T != "bool" && T != "var")
        {
          this->semErrors.push_back("Condition should be a boolean at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }

        if (this->lexemes.at(this->counter).getClassName() == ";")
        {
          this->counter++;
          return true;
        }
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
      string T = "";
      if (this->assign_st_this_or_ID(T))
        if (this->assign_or_inc_dec_class(T))
          return true;
    }
    return false;
  }

  bool inc_dec_start_class1()
  {
    if (this->lexemes.at(this->counter).getClassName() == "inc_dec")
    {
      string OP = this->lexemes.at(this->counter).getWord();

      this->counter++;
      if (this->this_or_ID_inc_dec(OP))
        return true;
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
        string T = "";
        this->counter++;

        if (this->OE_class(T))
        {
          this->code += "if( T" + to_string(this->Register) + " == false) jmp L" + to_string(++this->label) + "\n"; 

          if (T != "bool" && T != "var")
          {
            this->semErrors.push_back("Condition should be a boolean at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if (this->lexemes.at(this->counter).getClassName() == ")")
          {
            sym.CreateScope();
            string temp = to_string(this->label);

            this->counter++;
            if (this->term())
              if (this->body_fn_class())
                if (this->oelse_class(temp))
                  return true;
          }
        }
      }
    }
    return false;
  }

  bool oelse_class(string temp)
  {
    if (this->lexemes.at(this->counter).getClassName() == "Terminator")
    {
            this->code += "L" + temp + " :\n"; 
      this->counter++;
      return true;
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == "else")
      {
                this->code += "jmp L" + to_string(++this->label) + "\n";
        this->code += "L" + temp + " :\n";
        sym.CreateScope();
        this->counter++;
        if (this->term())
          if (this->body_fn_class()){
            code += "L" + to_string(this->label) + " :\n";
            return true;
            }
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
        string N = this->lexemes.at(this->counter).getWord();

        string T = "";
        bool statCheck = false;
        string name = "";
        this->counter++;
        if (this->static_ref_or_null(statCheck, T, N,name))
          if (this->trail_oe_class_or_fn(N, statCheck, T))
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
        string N = this->lexemes.at(this->counter).getWord();

        this->counter++;
        if (this->trail_or_obj_class(N))
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
          string T = sym.getCurrentClass();
          this->counter++;
          if (this->trail_oe_this(T))
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

  bool trail_oe_class_or_fn(string N, bool &statCheck, string &T)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "(")
    {
      if (this->fn_call_class(T, N, statCheck))
        return true;
    }
    else
    {
      if (temp == "." || temp == "[" || temp == "=" || temp == "inc_dec")
        if (this->trail_oe_class(N, statCheck, T))
          return true;
    }
    return false;
  }

  bool trail_or_obj_class(string N)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "#" || temp == "ID")
    {
      string TM = "";
      if (sym.lookupDT(N, TM) != "class")
      {
        this->semErrors.push_back("Undeclared class " + N + " used at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
      }

      if (TM == "abstract")
      {

        this->semErrors.push_back("Can't declare object of abstract class at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
      }

      if (this->obj_dec_class(N, "", "", true))
      {
        return true;
      }
    }
    else
    {
      if (temp == "::" || temp == "inc_dec" || temp == "=" || temp == "." || temp == "(" || temp == "[")
      {
        bool statCheck = false;
        string T = "";
        string name = "";
        if (this->static_ref_or_null(statCheck, T, N,name))
          if (this->trail_oe_class_or_fn(N, statCheck, T))
            return true;
      }
    }
    return false;
  }

  bool DEC_class(string AM, string TM, bool scopeFlag)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "DT" || temp == "var")
    {
      string T = "";
      if (this->DTs(T))
      {
        if (this->p_st_or_null_p_st_class(T, AM, TM, scopeFlag))
        {
          if (this->list_class(T, AM, TM, scopeFlag))
            return true;
        }
      }
    }
    return false;
  }

  bool p_st_or_null_p_st_class(string T, string AM, string TM, bool scopeFlag)
  {
    if (this->lexemes.at(this->counter).getClassName() == "#")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        string N = this->lexemes.at(this->counter).getWord();

        if (!scopeFlag)
        {
          ClassData D(N, T, AM, TM);
          if (!sym.insertClassData(sym.getCurrentClass(), D))
          {

            this->semErrors.push_back("Variable Redeclaration at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }
        }
        else
        {
          Scope S(N, T, sym.getScope());

          if (!sym.insertST(S))
          {
            this->semErrors.push_back("Variable Redeclaration at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }
        }

        this->counter++;
        if (this->new_init_class(N,T))
        {
          return true;
        }
      }
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        string N = this->lexemes.at(this->counter).getWord();

        if (!scopeFlag)
        {
          ClassData D(N, T, AM, TM);
          if (!sym.insertClassData(sym.getCurrentClass(), D))
          {

            this->semErrors.push_back("Variable Redeclaration at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }
        }
        else
        {
          Scope S(N, T, sym.getScope());

          if (!sym.insertST(S))
          {
            this->semErrors.push_back("Variable Redeclaration at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }
        }
        this->counter++;
        if (this->init_class(N,T))
        {
          return true;
        }
      }
    }
    return false;
  }

  bool new_init_class(string name,string T)
  {
    if (this->lexemes.at(this->counter).getClassName() == "=")
    {
      string OP = this->lexemes.at(this->counter).getWord();

      this->counter++;
      if (this->new_arr_const_class(name,OP, T))
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

  bool new_arr_const_class(string name,string OP, string T)
  {
    if (this->lexemes.at(this->counter).getClassName() == "new")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "DT")
      {
        if (this->lexemes.at(this->counter).getWord() != T)
        {
          this->semErrors.push_back("Uncompatible type while declaring variable at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }
        this->counter++;
        if (this->arr_or_null_class(name))
        {
          return true;
        }
      }
    }
    else
    {
      string temp = this->lexemes.at(this->counter).getClassName();
      if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "this" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
      {
        string RT = "";
        if (this->OE_class(RT))
        {
          this->code += "# " + name + " = T" + to_string(this->Register) + "\n";

          if (sym.compatibilityCheck(T, RT, OP) == "Uncompatible")
          {
            this->semErrors.push_back("Cannot assign type " + RT + " to type " + T + " at " + to_string((this->lexemes.at(this->counter).getLineNo())));
          }
          return true;
        }
      }
    }
    return false;
  }

  bool arr_or_null_class(string name)
  {
    if (this->lexemes.at(this->counter).getClassName() == "[")
    {
      string RT = "";
      this->counter++;
      if (this->OE_class(RT))
      {
        if (RT != "int")
        {
          this->semErrors.push_back("Uncompatible index type at " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }

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

  bool init_class(string name,string T)
  {
    if (this->lexemes.at(this->counter).getClassName() == "=")
    {
      string OP = this->lexemes.at(this->counter).getWord();

      this->counter++;
      if (this->new_arr_init_class(name,OP, T))
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

  bool new_arr_init_class(string name,string OP, string T)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "new")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "[")
      {
        this->counter++;
        string RT = "";
        if (this->OE_class(RT))
        {
          if (RT != "int")
          {
            this->semErrors.push_back("Uncompatible index type at " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }
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
      {
        string RT = "";
        if (this->OE_class(RT))
        {
          this->code += name + " = T" + to_string(this->Register) + "\n";
          
          if (sym.compatibilityCheck(T, RT, OP) == "Uncompatible")
          {

            this->semErrors.push_back("Type " + T + " is uncompatible with type " + RT + " at line " + to_string((this->lexemes.at(this->counter).getLineNo())));
          }
          return true;
        }
      }
    }
    return false;
  }

  bool trail_oe_this(string T)
  {
    if (this->lexemes.at(this->counter).getClassName() == "->")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        string N = this->lexemes.at(this->counter).getWord();
        bool statCheck = false;
        this->counter++;
        if (this->function_or_trail_class(N, statCheck, T))
          return true;
      }
    }
    return false;
  }

  bool list_class(string T, string AM, string TM, bool scopeFlag)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ",")
    {
      this->counter++;
      if (this->p_st_or_null_p_st_class(T, AM, TM, scopeFlag))
        if (this->list_class(T, AM, TM, scopeFlag))
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

  bool obj_dec_class(string T, string AM, string TM, bool scopeFlag)
  {
    if (this->lexemes.at(this->counter).getClassName() == "#" || this->lexemes.at(this->counter).getClassName() == "ID")
    {
      string name = "";
      if (this->p(name))
      {
        if (this->lexemes.at(this->counter).getClassName() == "ID")
        {
          string N = this->lexemes.at(this->counter).getWord();

          if (!scopeFlag)
          {
            ClassData D(N, T, AM, TM);
            if (!sym.insertClassData(sym.getCurrentClass(), D))
            {

              this->semErrors.push_back("Variable Redeclaration at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }
          }
          else
          {
            Scope S(N, T, sym.getScope());

            if (!sym.insertST(S))
            {
              this->semErrors.push_back("Variable Redeclaration at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }
          }

          this->counter++;
          if (this->init1_class(T))
            if (this->list1_class(T, AM, TM, scopeFlag))
              return true;
        }
      }
    }
    return false;
  }

  bool init1_class(string T)
  {
    if (this->lexemes.at(this->counter).getClassName() == "[")
    {
      this->counter++;
      string RT = "";
      if (this->OE_class(RT))
      {
        if (RT != "int")
        {
          this->semErrors.push_back("Uncompatible index type at " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }

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
            string ID = this->lexemes.at(this->counter).getWord();

            string TM = "";
            if (this->lexemes.at(this->counter).getWord() != T && sym.lookupDT(this->lexemes.at(this->counter).getWord(), T, TM) == "none")
            {
              this->semErrors.push_back("Class has no child " + ID + " or it doesn't exist at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            if (TM == "abstract")
            {
              this->semErrors.push_back("Cannot declare object of abstract class at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }
            this->counter++;
            if (this->pl_or_arr_class(T))
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

  bool pl_or_arr_class(string CN)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "(")
    {
      this->counter++;
      int count = 0;
      if (this->OEs_class_or_null(count))
        if (this->lexemes.at(this->counter).getClassName() == ")")
        {
          string AM = "", TM = "";
          if (sym.lookupFunction(CN, to_string(count), CN, AM, TM) == "none")
          {
            this->semErrors.push_back("No suitable constructor exists for " + CN + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if ((AM == "private" || AM == "protected") && CN != sym.getCurrentClass())
          {
            this->semErrors.push_back("Can't access private or protected data at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          this->counter++;
          return true;
        }
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == "[")
      {
        this->counter++;
        string RT = "";
        if (this->OE_class(RT))
          if (this->lexemes.at(this->counter).getClassName() == "]")
          {
            if (RT != "int")
            {
              this->semErrors.push_back("Uncompatible index type at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }
            this->counter++;
            return true;
          }
      }
    }
    return false;
  }

  bool OEs_class_or_null(int &count)
  {

    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "this" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (this->OEs_class(count))
        return true;
    }
    else
    {
      if (temp == ")")
        return true;
    }
    return false;
  }

  bool OEs_class(int &count)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "this" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      count++;
      string T = "";
      if (this->OE_class(T))
        if (this->next1_class(count))
          return true;
    }
    return false;
  }

  bool OE_class(string &T2)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "this" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      string T1 = "";
      if (this->AE_class(T1))
        if (this->OE_class1(T1, T2))
          return true;
    }
    return false;
  }

  bool AE_class(string &T2)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "this" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      string T1 = "";
      if (this->RE_class(T1))
        if (this->AE_class1(T1, T2))
          return true;
    }
    return false;
  }

  bool RE_class(string &T2)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "this" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      string T1 = "";
      if (this->PME_class(T1))
        if (this->RE_class1(T1, T2))
          return true;
    }
    return false;
  }

  bool PME_class(string &T2)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "this" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      string T1 = "";
      if (this->MDME_class(T1))
        if (this->PME_class1(T1, T2))
          return true;
    }
    return false;
  }

  bool MDME_class(string &T2)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "this" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      string T1 = "";
      if (this->OPs_class(T1))
        if (this->MDME_class1(T1, T2))
          return true;
    }
    return false;
  }

  bool OPs_class(string &T)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "#" || temp == "ID")
    {
      string name = "";
      if (this->p(name))
      {
        if (this->lexemes.at(this->counter).getClassName() == "ID")
        {
          string N = this->lexemes.at(this->counter).getWord();

          bool statCheck = false;
          name += N;
          this->counter++;
          if (this->static_ref_or_null(statCheck, T, N,name))
          {
            if (this->trail_or_fn_class(N, statCheck, T,name))
              return true;
          }
        }
      }
    }
    else
    {
      if (temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst" || temp == "IntConst")
      {
        T = this->ConstType(this->lexemes.at(this->counter).getClassName());

        this->code += "T" + to_string(this->Register) + " = "+ this->lexemes.at(this->counter).getWord()+"\n";
        
        this->counter++;
        return true;
      }
      else
      {
        if (temp == "(")
        {
          this->counter++;
          if (this->OE_class(T))
          {
            if (this->lexemes.at(this->counter).getClassName() == ")")
            {
              this->counter++;
              return true;
            }
          }
        }
        else
        {
          if (temp == "!")
          {
            string OP = this->lexemes.at(this->counter).getWord();

            this->counter++;
            string RT = "";

            if (this->OPs_class(RT))
            {
              string regR = to_string(this->Register);
              
              this->code += "T" + to_string(++this->Register) + " = " + OP + " T" + regR + "\n";
              
              T = sym.compatibilityCheck(RT, OP);

              if (T == "Uncompatible")
              {

                this->semErrors.push_back("Invalid operator with type at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }
              return true;
            }
          }
          else
          {
            if (temp == "inc_dec")
            {
              string OP = this->lexemes.at(this->counter).getWord();

              this->counter++;
              if (this->this_or_ID(T))
              {
                                    string regR = to_string(this->Register);
        
                    this->code += "T" + to_string(++this->Register) + " = " + OP + " T" + regR + "\n";

                T = sym.compatibilityCheck(T, OP);
                if (T == "Uncompatible")
                {

                  this->semErrors.push_back("Invalid operator with type at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
                }
                return true;
              }
            }
            else
            {
              if (temp == "this")
              {
                string name = "";
                name  += "this";
                T = sym.getCurrentClass();
                this->counter++;
                if (this->trail_this(name,T))
                  return true;
              }
            }
          }
        }
      }
    }
    return false;
  }

  bool this_or_ID(string &T)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "this")
    {
      string name = "";
      name += "this";
      T = sym.getCurrentClass();
      this->counter++;
      if (this->trail_this(name,T))
        return true;
    }
    else
    {
      if (temp == "ID")
      {
        string N = this->lexemes.at(this->counter).getWord();
        bool statCheck = false;
        string name = "";
        name += N;
        this->counter++;
        {
          if (this->static_ref_or_null(statCheck, T, N,name))
            if (this->trail_class(N, statCheck, T,name))
              return true;
        }
      }
    }
    return false;
  }

  bool trail_this(string &name,string &T)
  {
    if (this->lexemes.at(this->counter).getClassName() == "->")
    {
      name += "->";
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        string N = this->lexemes.at(this->counter).getWord();
        bool statCheck = false;
        name += N;
        this->counter++;
        if (this->fn_call_or_null_class(N, T, statCheck,name))
          if (this->trail_class(N, statCheck, T,name))
            return true;
      }
    }
    return false;
  }

  bool trail_or_fn_class(string N, bool &statCheck, string &T,string &name)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "(")
    {
      if (this->fn_call_class(T, N, statCheck))
        if (this->trail_class(N, statCheck, T,name))
          return true;
    }
    else
    {
      if (temp == "[" || temp == "." || temp == "MDM" || temp == "PM" || temp == "ROP" || temp == "&&" || temp == "||" || temp == ")" || temp == "]" || temp == "Terminator" || temp == ";" || temp == ",")
      {
        if (this->trail_class(N, statCheck, T,name))
          if (this->inc_dec_or_null(T))
            return true;
      }
    }
    return false;
  }

  bool fn_call_class(string &T, string N, bool &statCheck)
  {
    if (this->lexemes.at(this->counter).getClassName() == "(")
    {
      this->counter++;
      int count = 0;
      if (this->OEs_class_or_null(count))
        if (this->lexemes.at(this->counter).getClassName() == ")")
        {
          string AM = "", TM = "",temp = T,parentAM="",parentTM="";
          
          string parent = sym.getParent(T);

          T = sym.lookupFunction(N, to_string(count), T, AM, TM);

          if (T == "none")
          {
                if(parent != "none"){
                
                T = sym.lookupClassData(parent,N,parentAM,parentTM);
                }

             if(T == "none")
            this->semErrors.push_back("Undeclared function " + N + " called at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if (statCheck && TM != "static" && parentTM != "static")
          {
            this->semErrors.push_back("Function can't be called with class name at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if (!statCheck && (TM == "static" || parentTM == "static"))
          {
            this->semErrors.push_back("Static Function can't be called by object at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if ((AM == "private" || AM == "protected") && temp != sym.getCurrentClass())
          {
            this->semErrors.push_back("Can't access private or protected data at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if(parentAM == "private"){
            this->semErrors.push_back("Can't access private or member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
            }
          statCheck = false;
          this->fromFunc = true;
          this->counter++;
          return true;
        }
    }
    return false;
  }

  bool trail_class(string N, bool &statCheck, string &T,string &name)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        if (!from_fun())
        {
          if (T == "")
          {
            T = sym.lookupST(N);
            if (T == "none")
            {
              string parent = sym.getParent(sym.getCurrentClass());
              
              string AM = "",TM = "";

              if(parent != "none"){
                  T = sym.lookupClassData(parent,N,AM,TM);
                }

              if(AM == "private"){
                this->semErrors.push_back("Can't access private member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
                
              if(T == "none")
              this->semErrors.push_back(" Undeclared class object " + N + "at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            N = this->lexemes.at(this->counter).getWord();
          }
          else
          {
            string AM = "", TM = "",temp = T,parentAM="",parentTM="";
            string parent = sym.getParent(T);
            T = sym.lookupClassData(T, N, AM, TM);

            if (T == "none")
            {
                if(parent != "none"){
                  T = sym.lookupClassData(parent,N,parentAM,parentTM);
                }

              if(T == "none")
              this->semErrors.push_back(" Undeclared class object " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            N = this->lexemes.at(this->counter).getWord();

            if (statCheck && TM != "static" && parentTM != "static")
            {
              this->semErrors.push_back("Called Function should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            if ((AM == "private" || AM == "protected") && temp != sym.getCurrentClass())
            {
              this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            if(parentAM == "private"){
                this->semErrors.push_back("Can't access private or member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }

          }
        }
        this->counter++;
        statCheck = false;
        string name = "";
        if (this->fn_call_or_null_class(N, T, statCheck,name))
          if (this->trail_class(N, statCheck, T,name))
            return true;
      }
    }
    else
    {
      if (temp == "[")
      {
        this->counter++;
        string RT = "";

        if (this->OE_class(RT))
        {
          if (RT != "int")
          {
            this->semErrors.push_back("Uncompatible index type at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if (this->lexemes.at(this->counter).getClassName() == "]")
          {
            this->counter++;
            if (this->trail_no_arr_class(N, statCheck, T))
              return true;
          }
        }
      }
      else
      {
        if (temp == "MDM" || temp == "PM" || temp == "ROP" || temp == "&&" || temp == "||" || temp == ")" || temp == "]" || temp == "Terminator" || temp == ";" || temp == "," || temp == "inc_dec")
        {
          if (!from_fun())
          {
            if (T == "")
            {
              T = sym.lookupST(N);
              if (T == "none")
              {
                string parent = sym.getParent(sym.getCurrentClass());
                string AM = "",TM = "";

              if(parent != "none"){
                  T = sym.lookupClassData(parent,N,AM,TM);
                }

              if(AM == "private"){
                this->semErrors.push_back("Can't access private member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
                

              if(T == "none")
                this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }
            }
            else
            {
              string AM = "", TM = "",temp = T,parentAM="",parentTM="";
              string parent = sym.getParent(T);
              T = sym.lookupClassData(T, N, AM, TM);


              if (T == "none")
              {
                if(parent != "none"){
                  T = sym.lookupClassData(parent,N,parentAM,parentTM);
                }

                if(T == "none")
                  this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (statCheck && (TM != "static" && parentTM != "static"))
              {
                this->semErrors.push_back("Called Member should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if ((AM == "private" || AM == "protected") && temp != sym.getCurrentClass() && parent == "none")
              {
                this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if(parentAM == "private"){
                this->semErrors.push_back("Can't access private or member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
            }
          }
          return true;
        }
      }
    }
    return false;
  }

  bool trail_no_arr_class(string N, bool statCheck, string &T)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        if (T == "")
        {
          T = sym.lookupST(N);
          if (T == "none")
          {
              string parent = sym.getParent(sym.getCurrentClass());
              
              string AM = "",TM = "";

              if(parent != "none"){
                  T = sym.lookupClassData(parent,N,AM,TM);
                }

              if(AM == "private"){
                this->semErrors.push_back("Can't access private member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
                
            if(T == "none")
            this->semErrors.push_back(" Undeclared class object " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          N = this->lexemes.at(this->counter).getWord();
        }
        else
        {
          string AM = "", TM = "",temp = T,parentAM="",parentTM="";
          
          string parent = sym.getParent(T);

          T = sym.lookupClassData(T, N, AM, TM);

          if (T == "none")
          {
            if(parent != "none"){
              T = sym.lookupClassData(parent,N,parentAM,parentTM);
            }

            if(T == "none")
            this->semErrors.push_back(" Undeclared class object " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          N = this->lexemes.at(this->counter).getWord();

          if (statCheck && TM != "static" && parentTM != "static")
          {
            this->semErrors.push_back("Called Function should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if ((AM == "private" || AM == "protected") && temp != sym.getCurrentClass())
          {
            this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if(parentAM == "private"){
            this->semErrors.push_back("Can't access private or member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
          }
        }
        statCheck = false;
        this->counter++;
        string name = "";
        if (this->fn_call_or_null_class(N, T, statCheck,name))
          if (this->trail_class(N, statCheck, T,name))
            return true;
      }
    }
    else
    {
      if (temp == "MDM" || temp == "PM" || temp == "ROP" || temp == "&&" || temp == "||" || temp == ")" || temp == "]" || temp == "Terminator" || temp == ";" || temp == "," || temp == "inc_dec")
      {
        if (T == "")
        {
          T = sym.lookupST(N);
          if (T == "none")
          {
              string parent = sym.getParent(sym.getCurrentClass());
              
              string AM = "",TM = "";

              if(parent != "none"){
                  T = sym.lookupClassData(parent,N,AM,TM);
                }

              if(AM == "private"){
                this->semErrors.push_back("Can't access private member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
                
              if(T == "none")
            this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }
        }
        else
        {
          string AM = "", TM = "",temp = T,parentAM="",parentTM="";
          
          string parent = sym.getParent(T);

          T = sym.lookupClassData(T, N, AM, TM);

          if (T == "none")
          {
            if(parent != "none"){
              T = sym.lookupClassData(parent,N,parentAM,parentTM);
            }

            if(T == "none")
            this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if (statCheck && TM != "static" && parentTM != "static")
          {
            this->semErrors.push_back("Called Function should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if ((AM == "private" || AM == "protected") && temp != sym.getCurrentClass())
          {
            this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if(parentAM == "private"){
            this->semErrors.push_back("Can't access private or member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
          }

        }

        return true;
      }
    }

    return false;
  }

  bool fn_call_or_null_class(string N, string &T, bool &statCheck,string &name)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "(")
    {
      if (this->fn_call_class(T, N, statCheck))
        return true;
    }
    else
    {
      if (temp == "MDM" || temp == "PM" || temp == "ROP" || temp == "&&" || temp == "||" || temp == ")" || temp == "]" || temp == "Terminator" || temp == ";" || temp == "," || temp == "inc_dec" || temp == "." || temp == "[")
        return true;
    }
    return false;
  }

  bool trail_no_arr_no_fc_end_class(string N, string &T, bool &statCheck)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        if (T == "")
        {
          T = sym.lookupST(N);
          if (T == "none")
          {
              string parent = sym.getParent(sym.getCurrentClass());
              string AM = "",TM = "";

              if(parent != "none"){
                  T = sym.lookupClassData(parent,N,AM,TM);
                }

              if(AM == "private"){
                this->semErrors.push_back("Can't access private member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
                

              if(T == "none")
            this->semErrors.push_back(" Undeclared class object " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          N = this->lexemes.at(this->counter).getWord();
        }
        else
        {
          string AM = "", TM = "",temp = T,parentAM="",parentTM="";
          
          string parent = sym.getParent(T);
          T = sym.lookupClassData(T, N, AM, TM);

          if (T == "none")
          {
            if(parent != "none"){
              T = sym.lookupClassData(parent,N,parentAM,parentTM);
            }

            if(T == "none")
            this->semErrors.push_back(" Undeclared class object " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          N = this->lexemes.at(this->counter).getWord();

          if (statCheck && TM != "static" && parentTM != "static")
          {
            this->semErrors.push_back("Called Function should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if ((AM == "private" || AM == "protected") && temp != sym.getCurrentClass())
          {
            this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if(parentAM == "private"){
                this->semErrors.push_back("Can't access private or member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
        }
        statCheck = false;

        this->counter++;
        if (this->fn_call_class_or_null_no_fc_end(N, T, statCheck))
          return true;
      }
    }
    else
    {
      if (temp == "=" || temp == "inc_dec")
      {
        if (T == "")
        {
          T = sym.lookupST(N);
          if (T == "none")
          {
              string parent = sym.getParent(sym.getCurrentClass());
              
              string AM = "",TM = "";

              if(parent != "none"){
                  T = sym.lookupClassData(parent,N,AM,TM);
                }

              if(AM == "private"){
                this->semErrors.push_back("Can't access private member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
                

            if(T == "none")
            this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }
        }
        else
        {
              string AM = "", TM = "",temp = T,parentAM="",parentTM="";
              string parent = sym.getParent(T);
          T = sym.lookupClassData(T, N, AM, TM);

          if (T == "none")
          {
              if (T == "none")
              {
                if(parent != "none"){
                  T = sym.lookupClassData(parent,N,parentAM,parentTM);
                }

                if(T == "none")
            this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if (statCheck && TM != "static" && parentTM != "static")
          {
            this->semErrors.push_back("Called Function should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if ((AM == "private" || AM == "protected") && temp != sym.getCurrentClass())
          {
            this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

            if(parentAM == "private"){
                this->semErrors.push_back("Can't access private or member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
            }
          }
        }
        statCheck = false;

        return true;
      }
    }
    return false;
  }

  bool MDME_class1(string Tl, string &T)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "MDM")
    {
            string reg = to_string(this->Register);
      string OP = this->lexemes.at(this->counter).getWord();
      string TR = "";

      this->counter++;
      if (this->OPs_class(TR))
      {
        string regR = to_string(this->Register);
        
        this->code += "T" + to_string(++this->Register) + " = T" + reg + " " + OP + " T" + regR + "\n";

        string TA = sym.compatibilityCheck(Tl, TR, OP);
        if (TA == "Uncompatible")
        {
          this->semErrors.push_back("Type mismatch at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }
        if (this->MDME_class1(TA, T))
          return true;
      }
    }
    else
    {
      string temp = this->lexemes.at(this->counter).getClassName();
      if (temp == "]" || temp == ")" || temp == "," || temp == "Terminator" || temp == ";" || temp == "||" || temp == "&&" || temp == "ROP" || temp == "PM")
      {
        T = Tl;
        return true;
      }
    }
  }

  bool PME_class1(string Tl, string &T)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "PM")
    {
            string reg = to_string(this->Register);
      string OP = this->lexemes.at(this->counter).getWord();

      string TR = "";
      this->counter++;
      if (this->MDME_class(TR))
      {
        string regR = to_string(this->Register);
        
        this->code += "T" + to_string(++this->Register) + " = T" + reg + " " + OP + " T" + regR + "\n";

        string TA = sym.compatibilityCheck(Tl, TR, OP);

        if (TA == "Uncompatible")
        {
          this->semErrors.push_back("Type mismatch at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }

        if (this->PME_class1(TA, T))
          return true;
      }
    }
    else
    {
      string temp = this->lexemes.at(this->counter).getClassName();
      if (temp == "]" || temp == ")" || temp == "," || temp == "Terminator" || temp == ";" || temp == "||" || temp == "&&" || temp == "ROP")
      {
        T = Tl;
        return true;
      }
    }
  }

  bool RE_class1(string Tl, string &T)
  {
    if (this->lexemes.at(this->counter).getClassName() == "ROP")
    {
      string reg = to_string(this->Register);

      string OP = this->lexemes.at(this->counter).getWord();

      string TR = "";
      this->counter++;
      if (this->PME_class(TR))
      {
        string regR = to_string(this->Register);
        
        this->code += "T" + to_string(++this->Register) + " = T" + reg + " " + OP + " T" + regR + "\n";

        string TA = sym.compatibilityCheck(Tl, TR, OP);
        if (TA == "Uncompatible")
        {
          this->semErrors.push_back("Type mismatch at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }
        if (this->RE_class1(TA, T))
          return true;
      }
    }
    else
    {
      string temp = this->lexemes.at(this->counter).getClassName();
      if (temp == "]" || temp == ")" || temp == "," || temp == "Terminator" || temp == ";" || temp == "||" || temp == "&&")
      {
        T = Tl;
        return true;
      }
    }
    return false;
  }

  bool AE_class1(string Tl, string &T)
  {
    if (this->lexemes.at(this->counter).getClassName() == "&&")
    {
      string reg = to_string(this->Register);

      string OP = this->lexemes.at(this->counter).getWord();
      string TR = "";

      this->counter++;
      if (this->RE_class(TR))
      {
        string regR = to_string(this->Register);
        
        this->code += "T" + to_string(++this->Register) + " = T" + reg + " " + OP + " T" + regR + "\n";

        string TA = sym.compatibilityCheck(Tl, TR, OP);

        if (TA == "Uncompatible")
        {
          this->semErrors.push_back("Type mismatch at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }
        if (this->AE_class1(TA, T))
          return true;
      }
    }
    else
    {
      string temp = this->lexemes.at(this->counter).getClassName();
      if (temp == "]" || temp == ")" || temp == "," || temp == "Terminator" || temp == ";" || temp == "||")
      {
        T = Tl;
        return true;
      }
    }
    return false;
  }

  bool OE_class1(string Tl, string &T)
  {
    if (this->lexemes.at(this->counter).getClassName() == "||")
    {
            string reg = to_string(this->Register);

      string OP = this->lexemes.at(this->counter).getWord();

      this->counter++;
      string TR = "";
      if (this->AE_class(TR))
      {
                string regR = to_string(this->Register);

        this->code += "T" + to_string(++this->Register) + " = T" + reg + " " + OP + " T" + regR + "\n";

        string TA = sym.compatibilityCheck(Tl, TR, OP);

        if (TA == "Uncompatible")
        {
          this->semErrors.push_back("Type mismatch at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }
        if (this->OE_class1(TA, T))
          return true;
      }
    }
    else
    {
      string temp = this->lexemes.at(this->counter).getClassName();
      if (temp == "]" || temp == ")" || temp == "," || temp == "Terminator" || temp == ";")
      {
        T = Tl;
        return true;
      }
    }
    return false;
  }

  bool trail_oe_class(string N, bool &statCheck, string &T)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        if (T == "")
        {
          T = sym.lookupST(N);
          if (T == "none")
          {
                string parent = sym.getParent(sym.getCurrentClass());
                string AM = "",TM = "";

              if(parent != "none"){
                  T = sym.lookupClassData(parent,N,AM,TM);
                }

              if(AM == "private"){
                this->semErrors.push_back("Can't access private member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
                

              if(T == "none")
            this->semErrors.push_back(" Undeclared class object " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          N = this->lexemes.at(this->counter).getWord();
        }
        else
        {
          string AM = "", TM = "",temp = T,parentAM="",parentTM="";
          
          string parent = sym.getParent(T);
          T = sym.lookupClassData(T, N, AM, TM);

          if (T == "none")
          {
            if(parent != "none"){
              T = sym.lookupClassData(parent,N,parentAM,parentTM);
            }

            if(T == "none")
            this->semErrors.push_back(" Undeclared class object " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          N = this->lexemes.at(this->counter).getWord();

          if (statCheck && TM != "static" && parentTM != "static")
          {
            this->semErrors.push_back("Called Function should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if ((AM == "private" || AM == "protected") && temp != sym.getCurrentClass())
          {
            this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }
            if(parentAM == "private"){
                this->semErrors.push_back("Can't access private or member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }

        }
        statCheck = false;

        this->counter++;
        if (this->function_or_trail_class(N, statCheck, T))
          return true;
      }
    }
    else
    {
      if (temp == "[")
      {
        string RT = "";
        this->counter++;
        if (this->OE_class(RT))
          if (this->lexemes.at(this->counter).getClassName() == "]")
          {
            if (RT != "int")
            {
              this->semErrors.push_back(" Uncompatible index type at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            this->counter++;
            if (this->trail_oe_no_arr_class(N, statCheck, T))
              return true;
          }
      }
      else
      {
        if (temp == "=" || temp == "inc_dec")
        {
          if (T == "")
          {
            T = sym.lookupST(N);
            if (T == "none")
            {
                string parent = sym.getParent(sym.getCurrentClass());
                
                string AM = "",TM = "";

              if(parent != "none"){
                  T = sym.lookupClassData(parent,N,AM,TM);
                }

              if(AM == "private"){
                this->semErrors.push_back("Can't access private member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
                

              if(T == "none")
              this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }
          }
          else
          {
              string AM = "", TM = "",temp = T,parentAM="",parentTM="";
              string parent = sym.getParent(T);

            T = sym.lookupClassData(T, N, AM, TM);

            if (T == "none")
            {
                if(parent != "none"){
                  T = sym.lookupClassData(parent,N,parentAM,parentTM);
                }

                if(T == "none")
              this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            if (statCheck && TM != "static"  && parentTM != "static")
            {
              this->semErrors.push_back("Called Function should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            if ((AM == "private" || AM == "protected") && temp != sym.getCurrentClass())
            {
              this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            if(parentAM == "private"){
                this->semErrors.push_back("Can't access private or member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }

          }
          if (this->assign_or_inc_dec_class(T))
            return true;
        }
      }
    }
    return false;
  }

  bool assign_or_inc_dec_class(string &T)
  {
    if (this->lexemes.at(this->counter).getClassName() == "inc_dec")
    {
      string OP = this->lexemes.at(this->counter).getWord();

      T = sym.compatibilityCheck(T, OP);
      if (T == "Uncompatible")
      {

        this->semErrors.push_back("Invalid operator with type at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
      }
      this->counter++;
      return true;
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == "=")
        if (this->assign_st_class(T))
          return true;
    }
    return false;
  }

  bool assign_st_class(string T)
  {
    if (this->lexemes.at(this->counter).getClassName() == "=")
    {
      string OP = this->lexemes.at(this->counter).getWord();

      this->counter++;
      string RT = "";
      if (this->OE_class(RT))
      {
        T = sym.compatibilityCheck(T, RT, OP);

        if (T == "Uncompatible")
        {

          this->semErrors.push_back("Type not compatibile for assignment at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }

        return true;
      }
    }
    return false;
  }

  bool function_or_trail_class(string N, bool &statCheck, string &T)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "(")
    {
      if (this->fn_call_class(T, N, statCheck))
        if (this->trail_oe_null_class(N, statCheck, T))
          return true;
    }
    else
    {
      if (temp == "." || temp == "[" || temp == "inc_dec" || temp == "=")
        if (this->trail_oe_class(N, statCheck, T))
          return true;
    }
    return false;
  }

  bool trail_oe_null_class(string N, bool &statCheck, string &T)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (temp == "ID")
      {
        if (!from_fun())
        {
          if (T == "")
          {
            T = sym.lookupST(N);
            if (T == "none")
            {
              string parent = sym.getParent(sym.getCurrentClass());
              
              string AM = "",TM = "";

              if(parent != "none"){
                  T = sym.lookupClassData(parent,N,AM,TM);
                }

              if(AM == "private"){
                this->semErrors.push_back("Can't access private member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
                

              if(T == "none")
              this->semErrors.push_back(" Undeclared class object " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            N = this->lexemes.at(this->counter).getWord();
          }
          else
          {
              string AM = "", TM = "",temp = T,parentAM="",parentTM="";
              string parent = sym.getParent(T);

            T = sym.lookupClassData(T, N, AM, TM);

            if (T == "none")
            {
                if(parent != "none"){
                  T = sym.lookupClassData(parent,N,parentAM,parentTM);
                }

                if(T == "none")
              this->semErrors.push_back(" Undeclared class object " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            N = this->lexemes.at(this->counter).getWord();

            if (statCheck && TM != "static" && parentTM != "static")
            {
              this->semErrors.push_back("Called Function should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            if ((AM == "private" || AM == "protected") && temp != sym.getCurrentClass())
            {
              this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            
            }

              if(parentAM == "private"){
                this->semErrors.push_back("Can't access private or member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }

          }
        }
        statCheck = false;

        this->counter++;
        if (this->function_or_trail_class(N, statCheck, T))
          return true;
      }
    }
    else
    {
      if (temp == "[")
      {
        this->counter++;
        string RT = "";

        if (this->OE_class(RT))
          if (this->lexemes.at(this->counter).getClassName() == "]")
          {
            if (RT != "int")
            {
              this->semErrors.push_back(" Uncompatible index type at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }
            this->counter++;
            if (this->trail_oe_no_arr_class(N, statCheck, T))
              return true;
          }
      }
      else
      {
        if (temp == "Terminator" || temp == ",")
        {
          if (!from_fun())
          {
            if (T == "")
            {
              T = sym.lookupST(N);
              if (T == "none")
              {
                string parent = sym.getParent(sym.getCurrentClass());
                string AM = "",TM = "";

              if(parent != "none"){
                  T = sym.lookupClassData(parent,N,AM,TM);
                }

              if(AM == "private"){
                this->semErrors.push_back("Can't access private member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
                

              if(T == "none")
                this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }
            }
            else
            {

              string AM = "", TM = "",temp = T,parentAM="",parentTM="";
              string parent = sym.getParent(T);
              T = sym.lookupClassData(T, N, AM, TM);

              if (T == "none")
              {
                if(parent != "none"){
                  T = sym.lookupClassData(parent,N,parentAM,parentTM);
                }

                if(T == "none")
                this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (statCheck && TM != "static" && parentTM != "static")
              {
                this->semErrors.push_back("Called Function should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if ((AM == "private" || AM == "protected") && temp != sym.getCurrentClass())
              {
                this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if(parentAM == "private"){
                this->semErrors.push_back("Can't access private or member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }


            }
          }
          return true;
        }
      }
    }
    return false;
  }

  bool trail_oe_no_arr_class(string N, bool &statCheck, string &T)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        if (T == "")
        {
          T = sym.lookupST(N);
          if (T == "none")
          {
              string parent = sym.getParent(sym.getCurrentClass());
              
              string AM = "",TM = "";

              if(parent != "none"){
                  T = sym.lookupClassData(parent,N,AM,TM);
                }

              if(AM == "private"){
                this->semErrors.push_back("Can't access private member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
                
              if(T == "none")
            this->semErrors.push_back(" Undeclared class object " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          N = this->lexemes.at(this->counter).getWord();
        }
        else
        {
              string AM = "", TM = "",temp = T,parentAM="",parentTM="";
              string parent = sym.getParent(T);
          T = sym.lookupClassData(T, N, AM, TM);

          if (T == "none")
          {
                if(parent != "none"){
                  T = sym.lookupClassData(parent,N,parentAM,parentTM);
                }

            if(T == "none")
            this->semErrors.push_back(" Undeclared class object " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          N = this->lexemes.at(this->counter).getWord();

          if (statCheck && TM != "static" && parentTM != "static")
          {
            this->semErrors.push_back("Called Function should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if ((AM == "private" || AM == "protected") && temp != sym.getCurrentClass())
          {
            this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

            if(parentAM == "private"){
                this->semErrors.push_back("Can't access private or member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }

        }
        statCheck = false;

        this->counter++;
        if (this->function_or_trail_class(N, statCheck, T))
          return true;
      }
    }
    else
    {
      this->counter;
      if (temp == "=" || temp == "inc_dec")
      {
        if (T == "")
        {
          T = sym.lookupST(N);
          if (T == "none")
          {
               string parent = sym.getParent(sym.getCurrentClass());
              string AM = "",TM = "";

              if(parent != "none"){
                  T = sym.lookupClassData(parent,N,AM,TM);
                }

              if(AM == "private"){
                this->semErrors.push_back("Can't access private member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
                

              if(T == "none")
            this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }
        }
        else
        {
              string AM = "", TM = "",temp = T,parentAM="",parentTM="";
              string parent = sym.getParent(T);

          T = sym.lookupClassData(T, N, AM, TM);

          if (T == "none")
          {
                            if(parent != "none"){
                  T = sym.lookupClassData(parent,N,parentAM,parentTM);
                }

                if(T == "none")
            this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if (statCheck && TM != "static" && parentTM != "static")
          {
            this->semErrors.push_back("Called Function should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if ((AM == "private" || AM == "protected") && temp != sym.getCurrentClass())
          {
            this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

            if(parentAM == "private"){
                this->semErrors.push_back("Can't access private or member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }

        }
        statCheck = false;

        if (this->assign_or_inc_dec_class(T))
          return true;
      }
    }
    return false;
  }

  bool list1_class(string T, string AM, string TM, bool scopeFlag)
  {
    if (this->lexemes.at(this->counter).getClassName() == ",")
    {
      this->counter++;
      if (this->obj_dec_class(T, AM, TM, scopeFlag))
        return true;
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == "Terminator")
        return true;
    }
    return false;
  }

  bool next1_class(int &count)
  {
    if (this->lexemes.at(this->counter).getClassName() == ",")
    {
      this->counter++;
      if (this->OEs_class(count))
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
        string N = this->lexemes.at(this->counter).getWord();
        this->counter++;
        if (this->lexemes.at(this->counter).getClassName() == "(")
        {
          this->counter++;
          sym.CreateScope();
          int count = 0;
          if (this->pl(count))
          {
            this->code += N + "_" + to_string(count) + " proc\n";
            Data d(N, to_string(count), "", "");
            if (!sym.insertData(d))
            {
              semErrors.push_back("Function Redeclaration at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            if (this->lexemes.at(this->counter).getClassName() == ")")
            {
              this->counter++;
              if (this->term())
                if (this->body_fn())
                {
                  this->code += N + " Endp\n";
                  return true;
                
                }            
            }
          }
        }
      }
    }

    return false;
  }

  bool pl(int &count)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "#" || temp == "ID")
    {
      if (this->IDs(count))
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

  bool IDs(int &count)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "#" || temp == "ID")
    {
      string name = "";
      if (this->p(name))
        if (this->lexemes.at(this->counter).getClassName() == "ID")
        {
          Scope S(this->lexemes.at(this->counter).getWord(), "var", sym.getScope());

          if (!sym.insertST(S))
          {
            this->semErrors.push_back("Variable Redeclaration at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }
          count++;
          this->counter++;
          if (this->next(count))
            return true;
        }
    }
    return false;
  }

  bool next(int &count)
  {
    if (this->lexemes.at(this->counter).getClassName() == ",")
    {
      this->counter++;
      if (this->IDs(count))
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
        sym.DestroyScope();
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

  bool CONST(string &T)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      T = this->ConstType(this->lexemes.at(this->counter).getClassName());
      this->counter++;
      return true;
    }
    T = "";
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
    }
    else
    {

      if (this->lexemes.at(this->counter).getClassName() == "DT" || this->lexemes.at(this->counter).getClassName() == "var")
      {
        if (this->Dec())
          return true;
      }
      else
      {

        if (this->lexemes.at(this->counter).getClassName() == "ID")
        {
          string CN = this->lexemes.at(this->counter).getWord();
          string TM = "";
          if (sym.lookupDT(CN, TM) != "class")
          {
            this->semErrors.push_back("Undeclared Class " + CN + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if (TM == "abstract")
          {
            this->semErrors.push_back("Cannot declare object for abstract class at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          this->counter++;
          if (this->obj_dec(CN))
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
      string OP = this->lexemes.at(this->counter).getWord();
      string name = "";
      this->counter++;
      if (this->p(name))
        if (this->lexemes.at(this->counter).getClassName() == "ID")
        {
          
          string N = this->lexemes.at(this->counter).getWord();
          name += N;

          bool statCheck = false;
          string T = "";

          this->counter++;
          if (this->static_ref_or_null(statCheck, T, N,name))
            if (this->trail(N, statCheck, T,name))
            {
              string regR = to_string(this->Register);
        
              this->code += "T" + to_string(++this->Register) + " = " + OP + " T" + regR + "\n";

              T = sym.compatibilityCheck(T, OP);
              if (T == "Uncompatible")
              {

                this->semErrors.push_back("Invalid operator with type at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

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

  bool return_st()
  {
    if (this->lexemes.at(this->counter).getClassName() == "return")
    {
      this->counter++;
      string RT = "";
      if (this->OE(RT))
        if (this->lexemes.at(this->counter).getClassName() == "Terminator")
        {
          this->code += "return T" + to_string(this->Register) + "\n";
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
      string T = "";
      code +=  "exit " + this->lexemes.at(this->counter).getWord() + "\n";
      if (this->CONST(T))
      {
         
        if (T != "int")
        {
          this->semErrors.push_back("Exit code should be int at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }
        return true;
      }
    }
    else
    {
      if (temp == "ID" || temp == "#")
      {
        string name = "";

        if (this->p(name))
          if (this->lexemes.at(this->counter).getClassName() == "ID")
          {
            string N = this->lexemes.at(this->counter).getWord();
            bool statCheck = false;
            string T = "";
            this->counter++;
            if (this->static_ref_or_null(statCheck, T, N,name))
              if (this->trail(N, statCheck, T,name))
              {
                if (T != "int")
                {
                  this->semErrors.push_back("Exit code should be int at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
                }
                return true;
              }
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
        sym.CreateScope();
        this->counter++;
        if (this->c1()){
          
          this->code += "L" + to_string(++this->label) + " :\n";

          if (this->c2()){
            string tempLab = to_string(this->label);

            this->code += "if( T" + to_string(this->Register) + " == false) jmp L" + to_string(++this->label) + " :\n";

            string tempLab2 = to_string(this->label);
            
            if (this->c3())
              if (this->lexemes.at(this->counter).getClassName() == ")")
              {
                this->counter++;
                if (this->term())
                  if (this->body_fn()){
                    this->code += "jmp L" + tempLab + "\n";
                    this->code += "L" + tempLab2 + " :\n"; 
                    return true;
                    }
              }
              }
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
      string T = "";
      if (this->DTs(T))
      {
        if (this->p_st_or_null_p_st(T))
        {
          if (this->list_loop(T))
            return true;
        }
      }
    }
    return false;
  }

  bool list_loop(string T)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ",")
    {
      this->counter++;
      if (this->p_st_or_null_p_st(T))
        if (this->list_loop(T))
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
                string name = "";

      if (this->p(name))
        if (this->lexemes.at(this->counter).getClassName() == "ID")
        {
          string N = this->lexemes.at(this->counter).getWord();
          bool statCheck = false;
          string T = "";
          name += N;
          this->counter++;
          if (this->static_ref_or_null(statCheck, T, N,name))
            if (this->fn_call_or_null_no_fc_end(N, statCheck, T,name))
              if (this->assign_st(T))
                return true;
        }
    }
    return false;
  }

  bool trail_no_null(string N, bool &statCheck, string &T,string &name)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        name += "." + this->lexemes.at(this->counter).getWord();

        if (!from_fun())
        {
          if (T == "")
          {
            T = sym.lookupST(N);
            if (T == "none")
            {
              this->semErrors.push_back(" Undeclared class object " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            N = this->lexemes.at(this->counter).getWord();
          }
          else
            {
              string AM = "", TM = "",temp = T,parentAM="",parentTM="";
              string parent = sym.getParent(T);
              T = sym.lookupClassData(T, N, AM, TM);


              if (T == "none")
              {
                if(parent != "none"){
                  T = sym.lookupClassData(parent,N,parentAM,parentTM);
                }

                if(T == "none")
                  this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (statCheck && (TM != "static" && parentTM != "static"))
              {
                this->semErrors.push_back("Called Member should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (AM == "private" || AM == "protected")
              {
                this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if(parentAM == "private" || parentAM == "protected"){
                this->semErrors.push_back("Can't access private or protected member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
            }        
          }
        statCheck = false;

        this->counter++;
        if (this->fn_call_or_null_no_fc_end(N, statCheck, T,name))
          return true;
      }
    }
    else
    {
      if (temp == "[")
      {
        this->counter++;
        string RT = "";
        if (this->OE(RT))
          if (this->lexemes.at(this->counter).getClassName() == "]")
          {
            name += "[T" + to_string(this->Register) + "]"; 
            
            if (RT != "int")
            {
              this->semErrors.push_back(" Uncompatible index type at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }
            this->counter++;
            if (this->trail_no_arr_no_fc_end(N, statCheck, T,name))
              return true;
          }
      }
    }
    return false;
  }

  bool trail_no_fc_end(string N, bool &statCheck, string &T,string & name)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        name += "." + this->lexemes.at(this->counter).getWord();

        if (T == "")
        {
          T = sym.lookupST(N);
          if (T == "none")
          {
            this->semErrors.push_back(" Undeclared class object " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          N = this->lexemes.at(this->counter).getWord();
        }
        else
            {
              string AM = "", TM = "",temp = T,parentAM="",parentTM="";
              string parent = sym.getParent(T);
              T = sym.lookupClassData(T, N, AM, TM);


              if (T == "none")
              {
                if(parent != "none"){
                  T = sym.lookupClassData(parent,N,parentAM,parentTM);
                }

                if(T == "none")
                  this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (statCheck && (TM != "static" && parentTM != "static"))
              {
                this->semErrors.push_back("Called Member should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (AM == "private" || AM == "protected")
              {
                this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if(parentAM == "private"  || parentAM == "protected"){
                this->semErrors.push_back("Can't access private or protected member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
            }        
            statCheck = false;

        this->counter++;
        if (this->fn_call_or_null_no_fc_end(N, statCheck, T,name))
          return true;
      }
    }
    else
    {
      if (temp == "[")
      {
        string RT = "";
        this->counter++;
        if (this->OE(RT))
          if (this->lexemes.at(this->counter).getClassName() == "]")
          {
            name += "[T" + to_string(this->Register) + "]"; 
            
            if (RT != "int")
            {
              this->semErrors.push_back(" Uncompatible index type at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }
            this->counter++;
            if (this->trail_no_arr_no_fc_end(N, statCheck, T,name))
              return true;
          }
      }
      else
      {
        if (temp == "=" || temp == "inc_dec")
        {
            this->Register++;
           this->code += "T" + to_string(this->Register) + " = "+ name +"\n";
          if (T == "")
          {
            T = sym.lookupST(N);
            if (T == "none")
            {
              this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }
          }
          else
            {
              string AM = "", TM = "",temp = T,parentAM="",parentTM="";
              string parent = sym.getParent(T);
              T = sym.lookupClassData(T, N, AM, TM);


              if (T == "none")
              {
                if(parent != "none"){
                  T = sym.lookupClassData(parent,N,parentAM,parentTM);
                }

                if(T == "none")
                  this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (statCheck && (TM != "static" && parentTM != "static"))
              {
                this->semErrors.push_back("Called Member should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (AM == "private" || AM == "protected")
              {
                this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if(parentAM == "private"  || parentAM == "protected"){
                this->semErrors.push_back("Can't access private or protected member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
            }          
            statCheck = false;

          return true;
        }
      }
    }
    return false;
  }

  bool fn_call_or_null_no_fc_end(string N, bool &statCheck, string &T,string & name)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "(")
    {
      if (this->fn_call(T, N, statCheck,name))
        if (this->trail_no_null(N, statCheck, T,name))
          return true;
    }
    else
    {
      if (temp == "=" || temp == "." || temp == "[" || temp == "inc_dec")
        if (this->trail_no_fc_end(N, statCheck, T,name))
          return true;
    }
    return false;
  }

  bool c2()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      string T = "";
      if (this->OE(T))
      {
        if (T != "bool" && T != "var")
        {
          this->semErrors.push_back("Condition should be a boolean at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }
        if (this->lexemes.at(this->counter).getClassName() == ";")
        {
          this->counter++;
          return true;
        }
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
      string name = "";
      if (this->p(name))
        if (this->lexemes.at(this->counter).getClassName() == "ID")
        {
          string N = this->lexemes.at(this->counter).getWord();

          bool statCheck = false;
          string T = "";
           name += N ;
          this->counter++;
          if (this->static_ref_or_null(statCheck, T, N,name))
            if (this->fn_call_or_null_no_fc_end(N, statCheck, T,name))
              if (this->assign_or_inc_dec(T))
                return true;
        }
    }
    return false;
  }

  bool inc_dec_start1()
  {
    if (this->lexemes.at(this->counter).getClassName() == "inc_dec")
    {
      string OP = this->lexemes.at(this->counter).getWord();
                string name = "";

      this->counter++;
      if (this->p(name))
        if (this->lexemes.at(this->counter).getClassName() == "ID")
        {

          string N = this->lexemes.at(this->counter).getWord();
          name += N;
          bool statCheck = false;
          string T = "";
          this->counter++;
          if (this->static_ref_or_null(statCheck, T, N,name))
            if (this->trail(N, statCheck, T,name))
            {
              string regR = to_string(this->Register);
        
              this->code += "T" + to_string(++this->Register) + " = " + OP + " T" + regR + "\n";
              
              T = sym.compatibilityCheck(T, OP);
              if (T == "Uncompatible")
              {
                this->semErrors.push_back("Invalid operator with type at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }
              return true;
            }
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
        string T = "";
        if (this->OE(T))
        {
          this->code += "if( T" + to_string(this->Register) + " == false) jmp L" + to_string(++this->label) + "\n"; 
          if (T != "bool" && T != "var")
          {
            this->semErrors.push_back("Condition should be a boolean at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if (this->lexemes.at(this->counter).getClassName() == ")")
          {
            sym.CreateScope();
            string temp = to_string(this->label);
            this->counter++;
            if (this->term())
              if (this->body_fn())
                if (this->oelse(temp))
                  return true;
          }
        }
      }
    }
    return false;
  }

  bool oelse(string lab)
  {
    if (this->lexemes.at(this->counter).getClassName() == "Terminator")
    {
      this->code += "L" + lab + " :\n"; 
      this->counter++;
      return true;
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == "else")
      {
        this->code += "jmp L" + to_string(++this->label) + "\n";
        this->code += "L" + lab + " :\n"; 
        sym.CreateScope();
        this->counter++;
        if (this->term())
          if (this->body_fn())
          {
            code += "L" + to_string(this->label) + " :\n";
            return true;
            
            }
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
        string N = this->lexemes.at(this->counter).getWord();  
        string T = "";
        bool statCheck = false;
        string name = "";
        name += "#" + N;
        this->counter++;
        if (this->static_ref_or_null(statCheck, T, N,name))
          if (this->trail_oe_or_fn(N, statCheck, T,name))
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
        string N = this->lexemes.at(this->counter).getWord();
        string name = "" + N;
        this->counter++;
        if (this->trail_or_obj(name,N))
          if (this->lexemes.at(this->counter).getClassName() == "Terminator")
          {
            this->counter++;
            return true;
          }
      }
    }
    return false;
  }

  bool trail_oe_or_fn(string N, bool &statCheck, string &T,string & name)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "(")
    {
      if (this->fn_call(T, N, statCheck,name))
        return true;
    }
    else
    {
      if (temp == "." || temp == "[" || temp == "=" || temp == "inc_dec")
        if (this->trail_oe(N, statCheck, T,name))
          return true;
    }
    return false;
  }

  bool trail_or_obj(string& name, string N)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "#" || temp == "ID")
    {
      string TM = "";
      if (sym.lookupDT(N, TM) != "class")
      {
        this->semErrors.push_back("Undeclared class " + N + " used at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
      }

      if (TM == "abstract")
      {

        this->semErrors.push_back("Can't declare object of abstract class at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
      }

      if (this->obj_dec(N))
      {
        return true;
      }
    }
    else
    {
      if (temp == "::" || temp == "inc_dec" || temp == "=" || temp == "." || temp == "(" || temp == "[")
      {
        bool statCheck = false;
        string T = "";
        if (this->static_ref_or_null(statCheck, T, N,name))
          if (this->trail_oe_or_fn(N, statCheck, T,name))
            return true;
      }
    }
    return false;
  }

  bool Dec()
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "DT" || temp == "var")
    {
      string T = "";
      if (this->DTs(T))
      {
        if (this->p_st_or_null_p_st(T))
        {
          if (this->list(T))
            return true;
        }
      }
    }
    return false;
  }

  bool p_st_or_null_p_st(string T)
  {
    if (this->lexemes.at(this->counter).getClassName() == "#")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        string N = this->lexemes.at(this->counter).getWord();

        Scope S(N, T, sym.getScope());

        if (!sym.insertST(S))
        {
          this->semErrors.push_back("Variable Redeclaration at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }

        this->counter++;
        if (this->new_init(N,T))
        {
          return true;
        }
      }
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        string N = this->lexemes.at(this->counter).getWord();

        Scope S(N, T, sym.getScope());

        if (!sym.insertST(S))
        {
          this->semErrors.push_back("Variable Redeclaration at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }

        this->counter++;
        if (this->init(N,T))
        {
          return true;
        }
      }
    }
    return false;
  }

  bool new_init(string name,string T)
  {
    if (this->lexemes.at(this->counter).getClassName() == "=")
    {
      string OP = this->lexemes.at(this->counter).getWord();
      this->counter++;
      if (this->new_arr_const(name,OP, T))
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

  bool new_arr_const(string name,string OP, string T)
  {
    if (this->lexemes.at(this->counter).getClassName() == "new")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "DT")
      {
        if (this->lexemes.at(this->counter).getWord() != T)
        {
          this->semErrors.push_back("Uncompatible type while declaring variable at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }

        this->counter++;
        if (this->arr_or_null(name))
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
        string RT = "";
        if (this->OE(RT))
        {
          this->code += "# " + name + " = T" + to_string(this->Register) + "\n";

          if (sym.compatibilityCheck(T, RT, OP) == "Uncompatible")
          {
            this->semErrors.push_back("Cannot assign type " + RT + " to type " + T + " at " + to_string((this->lexemes.at(this->counter).getLineNo())));
          }
          return true;
        }
      }
    }
    return false;
  }

  bool arr_or_null(string name)
  {
    if (this->lexemes.at(this->counter).getClassName() == "[")
    {
      this->counter++;
      string RT = "";
      if (this->OE(RT))
      {
        if (RT != "int")
        {
          this->semErrors.push_back("Uncompatible index type at " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }
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

  bool init(string name,string T)
  {
    if (this->lexemes.at(this->counter).getClassName() == "=")
    {
      string OP = this->lexemes.at(this->counter).getWord();

      this->counter++;
      if (this->new_arr_init(name,OP, T))
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

  bool DTs(string &T)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "DT" || temp == "var")
    {
      T = this->lexemes.at(this->counter).getWord();
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

  bool new_arr_init(string name,string OP, string T)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "new")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "[")
      {
        this->counter++;
        string RT = "";
        if (this->OE(RT))
        {
          if (RT != "int")
          {
            this->semErrors.push_back("Uncompatible index type at " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

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
        string RT = "";
        if (this->OE(RT))
        {
          this->code += name + " = T" + to_string(this->Register) + "\n"; 
          if (sym.compatibilityCheck(T, RT, OP) == "Uncompatible")
          {

            this->semErrors.push_back("Type " + T + " is uncompatible with type " + RT + " at line " + to_string((this->lexemes.at(this->counter).getLineNo())));
          }
          return true;
        }
      }
    }
    return false;
  }

  bool list(string T)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ",")
    {
      this->counter++;
      if (this->p_st_or_null_p_st(T))
        if (this->list(T))
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

  bool obj_dec(string CN)
  {
    if (this->lexemes.at(this->counter).getClassName() == "#" || this->lexemes.at(this->counter).getClassName() == "ID")
    {
      string name = "";
      if (this->p(name))
      {
        if (this->lexemes.at(this->counter).getClassName() == "ID")
        {
          string N = this->lexemes.at(this->counter).getWord();
          name += N;
          Scope S(this->lexemes.at(this->counter).getWord(), CN, sym.getScope());

          if (!sym.insertST(S))
          {
            this->semErrors.push_back("Object Redeclaration at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }
          this->counter++;
          if (this->init1(name,CN))
            if (this->list1(CN))
              return true;
        }
      }
    }
    return false;
  }

  bool p(string & name)
  {
    if (this->lexemes.at(this->counter).getClassName() == "#")
    {
      name += "#";
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

  bool init1(string name,string CN)
  {
    if (this->lexemes.at(this->counter).getClassName() == "[")
    {
      this->counter++;
      string RT = "";
      if (this->OE(RT))
      {
        if (RT != "int")
        {
          this->semErrors.push_back("Uncompatible index type at " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }
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
            string ID = this->lexemes.at(this->counter).getWord();
            string TM = "";
            if (this->lexemes.at(this->counter).getWord() != CN && sym.lookupDT(this->lexemes.at(this->counter).getWord(), CN, TM) == "none")
            {
              this->semErrors.push_back("Class has no child " + ID + " or it doesn't exist at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            if (TM == "abstract")
            {
              this->semErrors.push_back("Cannot declare object of abstract class at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            this->counter++;
            if (this->pl_or_arr(name,CN))
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

  bool pl_or_arr(string name,string CN)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "(")
    {
      this->counter++;
      int count = 0;
      if (this->OEs_or_null(count))
        if (this->lexemes.at(this->counter).getClassName() == ")")
        {
          this->code += name + " = Call " + CN + "_" + CN + "," + to_string(count) + "\n";
           
          string AM = "", TM = "";
          if (sym.lookupFunction(CN, to_string(count), CN, AM, TM) == "none")
          {
            this->semErrors.push_back("No suitable constructor exists for " + CN + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if (AM == "private" || AM == "protected")
          {
            this->semErrors.push_back("Can't access private or protected data at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }
          this->counter++;
          return true;
        }
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == "[")
      {
        this->counter++;
        string RT = "";
        if (this->OE(RT))
          if (this->lexemes.at(this->counter).getClassName() == "]")
          {
            if (RT != "int")
            {
              this->semErrors.push_back("Uncompatible index type at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }
            this->counter++;
            return true;
          }
      }
    }
    return false;
  }

  bool OEs_or_null(int &count)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      if (this->OEs(count))
        return true;
    }
    else
    {
      if (temp == ")")
        return true;
    }
    return false;
  }

  bool OEs(int &count)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {

      string RT = "";
      if (this->OE(RT))
      {
      string var = to_string(this->Register);
        count++;
        if (this->next1(count)){
          this->code += "Param T" + var + "\n";
          return true;
        }      
      }
    }
    return false;
  }

  bool OE(string &T2)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      string T1 = "";
      if (this->AE(T1))
      {

        if (this->OE1(T1, T2))
          return true;
      }
    }
    return false;
  }

  bool AE(string &T2)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      string T1 = "";
      if (this->RE(T1))
      {
        if (this->AE1(T1, T2))
          return true;
      }
    }
    return false;
  }

  bool RE(string &T2)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      string T1 = "";
      if (this->PME(T1))
        if (this->RE1(T1, T2))
          return true;
    }
    return false;
  }

  bool PME(string &T2)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      string T1 = "";
      if (this->MDME(T1))
        if (this->PME1(T1, T2))
          return true;
    }
    return false;
  }

  bool MDME(string &T2)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "ID" || temp == "(" || temp == "!" || temp == "inc_dec" || temp == "#" || temp == "IntConst" || temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst")
    {
      string T1 = "";
      if (this->OPs(T1))
        if (this->MDME1(T1, T2))
          return true;
    }
    return false;
  }

  bool OPs(string &T)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "#" || temp == "ID")
    {
      string name = "";
      if (this->p(name))
      {
        if (this->lexemes.at(this->counter).getClassName() == "ID")
        {

          string N = this->lexemes.at(this->counter).getWord();
          this->counter++;
          bool statCheck = false;

          name += N;           

          if (this->static_ref_or_null(statCheck, T, N,name))
          {
            if (this->trail_or_fn(N, statCheck, T,name))
              return true;
          }
        }
      }
    }
    else
    {
      if (temp == "StringConst" || temp == "CharConst" || temp == "FloatConst" || temp == "BoolConst" || temp == "IntConst")
      {

        T = this->ConstType(this->lexemes.at(this->counter).getClassName());
        this->Register++;
          
        this->code += "T" + to_string(this->Register) + " = "+ this->lexemes.at(this->counter).getWord()+"\n";
        
        this->counter++;
        return true;
      }
      else
      {
        if (temp == "(")
        {
          this->counter++;
          if (this->OE(T))
          {

            if (this->lexemes.at(this->counter).getClassName() == ")")
            {
              this->counter++;
              return true;
            }
          }
        }
        else
        {
          if (temp == "!")
          {
            
            string OP = this->lexemes.at(this->counter).getWord();

            this->counter++;
            string RT = "";

            if (this->OPs(RT))
            {
              string regR = to_string(this->Register);
        
              this->code += "T" + to_string(++this->Register) + " = " + OP + " T" + regR + "\n";
              
              T = sym.compatibilityCheck(RT, OP);
              if (T == "Uncompatible")
              {

                this->semErrors.push_back("Invalid operator with type at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }
              return true;
            }
          }
          else
          {
            if (temp == "inc_dec")
            {
              string OP = this->lexemes.at(this->counter).getWord();
              this->counter++;
              if (this->lexemes.at(this->counter).getClassName() == "ID")
              {
                bool statCheck = false;
                string N = this->lexemes.at(this->counter).getWord();
                string name = "" + N;
                this->counter++;
                if (this->static_ref_or_null(statCheck, T, N,name))
                {
                  if (this->trail(N, statCheck, T,name))
                  {
                    string regR = to_string(this->Register);
        
                    this->code += "T" + to_string(++this->Register) + " = " + OP + " T" + regR + "\n";
                    
                    T = sym.compatibilityCheck(T, OP);
                    if (T == "Uncompatible")
                    {

                      this->semErrors.push_back("Invalid operator with type at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
                    }

                    return true;
                  }
                }
              }
            }
          }
        }
      }
    }
    return false;
  }

  bool trail_or_fn(string N, bool statCheck, string &T,string & name)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "(")
    {
      if (this->fn_call(T, N, statCheck,name))
        if (this->trail(N, statCheck, T,name))
          return true;
    }
    else
    {
      if (temp == "[" || temp == "." || temp == "MDM" || temp == "PM" || temp == "ROP" || temp == "&&" || temp == "||" || temp == ")" || temp == "]" || temp == "Terminator" || temp == ";" || temp == ",")
      {
        if (this->trail(N, statCheck, T,name))
          if (this->inc_dec_or_null(T))
            return true;
      }
    }
    return false;
  }

  bool fn_call(string &T, string N, bool &statCheck,string &name)
  {
    if (this->lexemes.at(this->counter).getClassName() == "(")
    {

      this->counter++;
      int count = 0;
      if (this->OEs_or_null(count))
        if (this->lexemes.at(this->counter).getClassName() == ")")
        {
          this->code += "T" + to_string(++this->Register) + " = Call " + name + + "," + to_string(count) + "\n";

          name = "T" + to_string(this->Register);

          string AM = "", TM = "",temp = T,parentAM="",parentTM="";
          
          string parent = sym.getParent(T);          
          T = sym.lookupFunction(N, to_string(count), T, AM, TM);

          if (T == "none")
          {
              if(parent != "none"){
                
                T = sym.lookupClassData(parent,N,parentAM,parentTM);
              }

             if(T == "none")
            this->semErrors.push_back("Undeclared function " + N + " called at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if (statCheck && TM != "static" && parentTM != "static")
          {
            this->semErrors.push_back("Function can't be called with class name at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if (!statCheck &&( TM == "static" || parentTM == "static"))
          {
            this->semErrors.push_back("Static Function can't be called by object at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if (AM == "private" || AM == "protected")
          {
            this->semErrors.push_back("Can't access private or protected data at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if(parentAM == "private"  || parentAM == "protected"){
            this->semErrors.push_back("Can't access private or protected member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
            }

          statCheck = false;
          this->fromFunc = true;
          this->counter++;
          return true;
        }
    }
    return false;
  }

  bool inc_dec_or_null(string &T)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "inc_dec")
    {
      string OP = this->lexemes.at(this->counter).getWord();
      
      string regR = to_string(this->Register);
        
      this->code += "T" + to_string(++this->Register) + " = " + " T" + regR +  OP + "\n";

      T = sym.compatibilityCheck(T, OP);
      if (T == "Uncompatible")
      {
        
        this->semErrors.push_back("Invalid operator with type at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
      }

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

  bool trail(string N, bool &statCheck, string &T,string & name)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        name += "." + this->lexemes.at(this->counter).getWord();

        if (!from_fun())
        {
          if (T == "")
          {
            T = sym.lookupST(N);
            if (T == "none")
            {
              this->semErrors.push_back(" Undeclared class object " + N + "at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            N = this->lexemes.at(this->counter).getWord();
          }
          else
            {
              string AM = "", TM = "",temp = T,parentAM="",parentTM="";
              string parent = sym.getParent(T);
              T = sym.lookupClassData(T, N, AM, TM);


              if (T == "none")
              {
                if(parent != "none"){
                  T = sym.lookupClassData(parent,N,parentAM,parentTM);
                }

                if(T == "none")
                  this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (statCheck && (TM != "static" && parentTM != "static"))
              {
                this->semErrors.push_back("Called Member should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (AM == "private" || AM == "protected")
              {
                this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if(parentAM == "private"  || parentAM == "protected"){
                this->semErrors.push_back("Can't access private or protected member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
            }        
            }
        this->counter++;
        statCheck = false;
        if (this->fn_call_or_null(N, statCheck, T,name))
          if (this->trail(N, statCheck, T,name))
            return true;
      }
    }
    else
    {
      if (temp == "[")
      {
        this->counter++;
        string RT = "";
        if (this->OE(RT))
        {
          name += "[T" + to_string(this->Register) + "]"; 
          if (RT != "int")
          {
            this->semErrors.push_back("Uncompatible index type at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          if (this->lexemes.at(this->counter).getClassName() == "]")
          {
            this->counter++;
            if (this->trail_no_arr(N, statCheck, T,name))
              return true;
          }
        }
      }
      else
      {
        if (temp == "MDM" || temp == "PM" || temp == "ROP" || temp == "&&" || temp == "||" || temp == ")" || temp == "]" || temp == "Terminator" || temp == ";" || temp == "," || temp == "inc_dec")
        {
           this->Register++;
           this->code += "T" + to_string(this->Register) + " = "+ name +"\n";


          if (!from_fun())
          {
            if (T == "")
            {
              T = sym.lookupST(N);
              if (T == "none")
              {
                this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }
            }
            else
            {
              string AM = "", TM = "",temp = T,parentAM="",parentTM="";
              string parent = sym.getParent(T);
              T = sym.lookupClassData(T, N, AM, TM);


              if (T == "none")
              {
                if(parent != "none"){
                  T = sym.lookupClassData(parent,N,parentAM,parentTM);
                }

                if(T == "none")
                  this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (statCheck && (TM != "static" && parentTM != "static"))
              {
                this->semErrors.push_back("Called Member should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (AM == "private" || AM == "protected")
              {
                this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if(parentAM == "private"  || parentAM == "protected"){
                this->semErrors.push_back("Can't access private or protected member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
            }          
            }
          return true;
        }
      }
    }
    return false;
  }

  bool trail_no_arr(string N, bool statCheck, string &T,string & name)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        name += "." + this->lexemes.at(this->counter).getWord();
        if (T == "")
        {
          T = sym.lookupST(N);
          if (T == "none")
          {
            this->semErrors.push_back(" Undeclared class object " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          N = this->lexemes.at(this->counter).getWord();
        }
        else
            {
              string AM = "", TM = "",temp = T,parentAM="",parentTM="";
              string parent = sym.getParent(T);
              T = sym.lookupClassData(T, N, AM, TM);


              if (T == "none")
              {
                if(parent != "none"){
                  T = sym.lookupClassData(parent,N,parentAM,parentTM);
                }

                if(T == "none")
                  this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (statCheck && (TM != "static" && parentTM != "static"))
              {
                this->semErrors.push_back("Called Member should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (AM == "private" || AM == "protected")
              {
                this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if(parentAM == "private"  || parentAM == "protected"){
                this->semErrors.push_back("Can't access private or protected member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
            }        
          statCheck = false;
        this->counter++;
        if (this->fn_call_or_null(N, statCheck, T,name))
          if (this->trail(N, statCheck, T,name))
            return true;
      }
    }
    else
    {
      if (temp == "MDM" || temp == "PM" || temp == "ROP" || temp == "&&" || temp == "||" || temp == ")" || temp == "]" || temp == "Terminator" || temp == ";" || temp == "," || temp == "inc_dec"){
           
           this->Register++;
           this->code += "T" + to_string(this->Register) + " = "+ name +"\n";

        if (T == "")
        {
          T = sym.lookupST(N);
          if (T == "none")
          {
            this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }
        }
        else
            {
              string AM = "", TM = "",temp = T,parentAM="",parentTM="";
              string parent = sym.getParent(T);
              T = sym.lookupClassData(T, N, AM, TM);


              if (T == "none")
              {
                if(parent != "none"){
                  T = sym.lookupClassData(parent,N,parentAM,parentTM);
                }

                if(T == "none")
                  this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (statCheck && (TM != "static" && parentTM != "static"))
              {
                this->semErrors.push_back("Called Member should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (AM == "private" || AM == "protected")
              {
                this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if(parentAM == "private" || parentAM == "protected"){
                this->semErrors.push_back("Can't access private or protected member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
            }      
            return true;
            }
    }
    return false;
  }

  bool fn_call_or_null(string N, bool statCheck, string &T,string & name)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "(")
    {
      if (this->fn_call(T, N, statCheck,name))
        return true;
    }
    else
    {
      if (temp == "MDM" || temp == "PM" || temp == "ROP" || temp == "&&" || temp == "||" || temp == ")" || temp == "]" || temp == "Terminator" || temp == ";" || temp == "," || temp == "inc_dec" || temp == "." || temp == "[")
        return true;
    }
    return false;
  }

  bool trail_no_arr_no_fc_end(string N, bool &statCheck, string &T,string & name)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        name += "." + this->lexemes.at(this->counter).getWord();

        if (T == "")
        {
          T = sym.lookupST(N);
          if (T == "none")
          {
            this->semErrors.push_back(" Undeclared class object " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          N = this->lexemes.at(this->counter).getWord();
        }
        else
            {
              string AM = "", TM = "",temp = T,parentAM="",parentTM="";
              string parent = sym.getParent(T);
              T = sym.lookupClassData(T, N, AM, TM);


              if (T == "none")
              {
                if(parent != "none"){
                  T = sym.lookupClassData(parent,N,parentAM,parentTM);
                }

                if(T == "none")
                  this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (statCheck && (TM != "static" && parentTM != "static"))
              {
                this->semErrors.push_back("Called Member should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (AM == "private" || AM == "protected")
              {
                this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if(parentAM == "private" || parentAM == "protected"){
                this->semErrors.push_back("Can't access private or protected member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
            }        
            statCheck = false;

        this->counter++;
        if (this->fn_call_or_null_no_fc_end(N, statCheck, T,name))
          return true;
      }
    }
    else
    {
      if (temp == "=" || temp == "inc_dec")
      {
                   this->Register++;
           this->code += "T" + to_string(this->Register) + " = "+ name +"\n";
        if (T == "")
        {
          T = sym.lookupST(N);
          if (T == "none")
          {
            this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }
        }
        else
            {
              string AM = "", TM = "",temp = T,parentAM="",parentTM="";
              string parent = sym.getParent(T);
              T = sym.lookupClassData(T, N, AM, TM);


              if (T == "none")
              {
                if(parent != "none"){
                  T = sym.lookupClassData(parent,N,parentAM,parentTM);
                }

                if(T == "none")
                  this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (statCheck && (TM != "static" && parentTM != "static"))
              {
                this->semErrors.push_back("Called Member should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (AM == "private" || AM == "protected")
              {
                this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if(parentAM == "private" || parentAM == "protected"){
                this->semErrors.push_back("Can't access private or protected member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
            }        
            statCheck = false;

        return true;
      }
    }
    return false;
  }

  bool MDME1(string Tl, string &T)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "MDM")
    {
      string reg = to_string(this->Register);
      string OP = this->lexemes.at(this->counter).getWord();

      this->counter++;

      string TR = "";
      if (this->OPs(TR))
      {
        string regR = to_string(this->Register);
        
        this->code += "T" + to_string(++this->Register) + " = T" + reg + " " + OP + " T" + regR + "\n";
        
        string TA = sym.compatibilityCheck(Tl, TR, OP);
        if (TA == "Uncompatible")
        {
          this->semErrors.push_back("Type mismatch at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }

        if (this->MDME1(TA, T))
          return true;
      }
    }
    else
    {
      string temp = this->lexemes.at(this->counter).getClassName();
      if (temp == "]" || temp == ")" || temp == "," || temp == "Terminator" || temp == ";" || temp == "||" || temp == "&&" || temp == "ROP" || temp == "PM")
      {
        T = Tl;
        return true;
      }
    }
  }

  bool PME1(string Tl, string &T)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "PM")
    {
      string reg = to_string(this->Register);
      
      string OP = this->lexemes.at(this->counter).getWord();

      this->counter++;
      string TR = "";
      if (this->MDME(TR))
      {
        string regR = to_string(this->Register);
        
        this->code += "T" + to_string(++this->Register) + " = T" + reg + " " + OP + " T" + regR + "\n";

        string TA = sym.compatibilityCheck(Tl, TR, OP);
        if (TA == "Uncompatible")
        {
          this->semErrors.push_back("Type mismatch at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }

        if (this->PME1(TA, T))
          return true;
      }
    }
    else
    {
      string temp = this->lexemes.at(this->counter).getClassName();
      if (temp == "]" || temp == ")" || temp == "," || temp == "Terminator" || temp == ";" || temp == "||" || temp == "&&" || temp == "ROP")
      {
        T = Tl;
        return true;
      }
    }
  }

  bool RE1(string Tl, string &T)
  {
    if (this->lexemes.at(this->counter).getClassName() == "ROP")
    {
      string reg = to_string(this->Register);
      
      string OP = this->lexemes.at(this->counter).getWord();
      this->counter++;
      string TR = "";
      if (this->PME(TR))
      {
        string regR = to_string(this->Register);
        
        this->code += "T" + to_string(++this->Register) + " = T" + reg + " " + OP + " T" + regR + "\n";

        string TA = sym.compatibilityCheck(Tl, TR, OP);
        if (TA == "Uncompatible")
        {
          this->semErrors.push_back("Type mismatch at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }
        if (this->RE1(TA, T))
          return true;
      }
    }
    else
    {
      string temp = this->lexemes.at(this->counter).getClassName();
      if (temp == "]" || temp == ")" || temp == "," || temp == "Terminator" || temp == ";" || temp == "||" || temp == "&&")
      {
        T = Tl;
        return true;
      }
    }
    return false;
  }

  bool AE1(string Tl, string &T)
  {
    if (this->lexemes.at(this->counter).getClassName() == "&&")
    {
      string reg = to_string(this->Register);
      
      string OP = this->lexemes.at(this->counter).getWord();

      this->counter++;
      string TR = "";
      if (this->RE(TR))
      {
        string regR = to_string(this->Register);
        
        this->code += "T" + to_string(++this->Register) + " = T" + reg + " " + OP + " T" + regR + "\n";

        string TA = sym.compatibilityCheck(Tl, TR, OP);
        if (TA == "Uncompatible")
        {
          this->semErrors.push_back("Type mismatch at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }

        if (this->AE1(TA, T))
          return true;
      }
    }
    else
    {
      string temp = this->lexemes.at(this->counter).getClassName();
      if (temp == "]" || temp == ")" || temp == "," || temp == "Terminator" || temp == ";" || temp == "||")
      {
        T = Tl;
        return true;
      }
    }
    return false;
  }

  bool OE1(string Tl, string &T)
  {
    if (this->lexemes.at(this->counter).getClassName() == "||")
    {
      string reg = to_string(this->Register);
      
      string OP = this->lexemes.at(this->counter).getWord();

      this->counter++;
      string TR = "";
      if (this->AE(TR))
      {
        string regR = to_string(this->Register);

        this->code += "T" + to_string(++this->Register) + " = T" + reg + " " + OP + " T" + regR + "\n";
        
        string TA = sym.compatibilityCheck(Tl, TR, OP);
        if (TA == "Uncompatible")
        {
          this->semErrors.push_back("Type mismatch at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }
        if (this->OE1(TA, T))
          return true;
      }
    }
    else
    {
      string temp = this->lexemes.at(this->counter).getClassName();
      if (temp == "]" || temp == ")" || temp == "," || temp == "Terminator" || temp == ";")
      {
        T = Tl;
        return true;
      }
    }
    return false;
  }

  bool static_ref_or_null(bool &statCheck, string &T, string &N,string &name)
  {
    if (this->lexemes.at(this->counter).getClassName() == "::")
    {
      name += "::";
      string TM = "";
      if (sym.lookupDT(N, TM) != "class")
      {
        this->semErrors.push_back("Undeclared Class " + N + " used at " + to_string(this->lexemes.at(this->counter).getLineNo()));
      }
      T = N;
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        N = this->lexemes.at(this->counter).getWord();

        name += N;
        statCheck = true;

        this->counter++;
        return true;
      }
    }
    else
    {
      string temp = this->lexemes.at(this->counter).getClassName();

      if (temp == "." || temp == "[" || temp == "Terminator" || temp == "(" || temp == "inc_dec" || temp == "MDM" || temp == "PM" || temp == "ROP" || temp == "||" || temp == "&&" || temp == "]" || temp == ")" || temp == "," || temp == ";")
      {
        N = N;
        T = "";
        statCheck = false;
        return true;
      }
    }
    return false;
  }

  bool trail_oe(string N, bool &statCheck, string &T,string & name)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        name += "." + this->lexemes.at(this->counter).getWord();
        if (T == "")
        {
          T = sym.lookupST(N);
          if (T == "none")
          {
            this->semErrors.push_back(" Undeclared class object " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          N = this->lexemes.at(this->counter).getWord();
        }
        else
            {
              string AM = "", TM = "",temp = T,parentAM="",parentTM="";
              string parent = sym.getParent(T);
              T = sym.lookupClassData(T, N, AM, TM);


              if (T == "none")
              {
                if(parent != "none"){
                  T = sym.lookupClassData(parent,N,parentAM,parentTM);
                }

                if(T == "none")
                  this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (statCheck && (TM != "static" && parentTM != "static"))
              {
                this->semErrors.push_back("Called Member should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (AM == "private" || AM == "protected")
              {
                this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if(parentAM == "private" || parentAM == "protected"){
                this->semErrors.push_back("Can't access private or protected member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
            }
        statCheck = false;
        this->counter++;
        if (this->function_or_trail(N, statCheck, T,name))
          return true;
      }
    }
    else
    {
      if (temp == "[")
      {
        this->counter++;
        string RT = "";
        if (this->OE(RT))
          if (this->lexemes.at(this->counter).getClassName() == "]")
          {
            name += "[T" + to_string(this->Register) + "]"; 

            if (RT != "int")
            {
              this->semErrors.push_back(" Uncompatible index type at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }
            this->counter++;
            if (this->trail_oe_no_arr(N, statCheck, T,name))
              return true;
          }
      }
      else
      {
        if (temp == "=" || temp == "inc_dec"){
          this->Register++;
           this->code += "T" + to_string(this->Register) + " = "+ name +"\n";

          if (T == "")
          {
            T = sym.lookupST(N);
            if (T == "none")
            {
              this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }
          }
          else
            {
              string AM = "", TM = "",temp = T,parentAM="",parentTM="";
              string parent = sym.getParent(T);
              T = sym.lookupClassData(T, N, AM, TM);


              if (T == "none")
              {
                if(parent != "none"){
                  T = sym.lookupClassData(parent,N,parentAM,parentTM);
                }

                if(T == "none")
                  this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (statCheck && (TM != "static" && parentTM != "static"))
              {
                this->semErrors.push_back("Called Member should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (AM == "private" || AM == "protected")
              {
                this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if(parentAM == "private" || parentAM == "protected"){
                this->semErrors.push_back("Can't access private or protected member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
            }        
            if (this->assign_or_inc_dec(T))
          return true;
          }
      }
    }
    return false;
  }

  bool assign_or_inc_dec(string &T)
  {
    if (this->lexemes.at(this->counter).getClassName() == "inc_dec")
    {
      string OP = this->lexemes.at(this->counter).getWord();
      string regR = to_string(this->Register);

      this->code += "T" + to_string(++this->Register) + " = "  + " T" + regR +  OP + "\n";

      T = sym.compatibilityCheck(T, OP);
      if (T == "Uncompatible")
      {

        this->semErrors.push_back("Invalid operator with type at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
      }
      this->counter++;
      return true;
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == "=")
        if (this->assign_st(T))
          return true;
    }
    return false;
  }

  bool assign_st(string T)
  {
    if (this->lexemes.at(this->counter).getClassName() == "=")
    {
      string OP = this->lexemes.at(this->counter).getWord();
      
      string LT = to_string(this->Register);
      
      this->counter++;
      string RT = "";
      if (this->OE(RT))
      {

           this->code += "T" + LT + " = "+ "T" + to_string(this->Register) +"\n";

        T = sym.compatibilityCheck(T, RT, OP);
        if (T == "Uncompatible")
        {

          this->semErrors.push_back("Type not compatibile for assignment at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
        }

        return true;
      }
    }
    return false;
  }

  bool function_or_trail(string N, bool &statCheck, string &T,string &name)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == "(")
    {
      if (this->fn_call(T, N, statCheck,name))
        if (this->trail_oe_null(N, statCheck, T,name))
          return true;
    }
    else
    {
      if (temp == "." || temp == "[" || temp == "inc_dec" || temp == "=")
        if (this->trail_oe(N, statCheck, T,name))
          return true;
    }
    return false;
  }

  bool trail_oe_null(string N, bool &statCheck, string &T,string &name)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (temp == "ID")
      {
        name += "." + this->lexemes.at(this->counter).getWord();
        if (!from_fun())
        {
          if (T == "")
          {
            T = sym.lookupST(N);
            if (T == "none")
            {
              this->semErrors.push_back(" Undeclared class object " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }

            N = this->lexemes.at(this->counter).getWord();
          }
          else
            {
              string AM = "", TM = "",temp = T,parentAM="",parentTM="";
              string parent = sym.getParent(T);
              T = sym.lookupClassData(T, N, AM, TM);


              if (T == "none")
              {
                if(parent != "none"){
                  T = sym.lookupClassData(parent,N,parentAM,parentTM);
                }

                if(T == "none")
                  this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (statCheck && (TM != "static" && parentTM != "static"))
              {
                this->semErrors.push_back("Called Member should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (AM == "private" || AM == "protected")
              {
                this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if(parentAM == "private" || parentAM == "protected"){
                this->semErrors.push_back("Can't access private or protected member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
            }
                    }
        statCheck = false;

        this->counter++;
        if (this->function_or_trail(N, statCheck, T,name))
          return true;
      }
    }
    else
    {
      if (temp == "[")
      {
        this->counter++;
        string RT = "";
        if (this->OE(RT))
          if (this->lexemes.at(this->counter).getClassName() == "]")
          {
            name += "[T" + to_string(this->Register) + "]";
            
            if (RT != "int")
            {
              this->semErrors.push_back(" Uncompatible index type at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
            }
            this->counter++;
            if (this->trail_oe_no_arr(N, statCheck, T,name))
              return true;
          }
      }
      else
      {
        if (temp == "Terminator" || temp == ",")
        {
          if (!from_fun())
          {
            if (T == "")
            {
              T = sym.lookupST(N);
              if (T == "none")
              {
                this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }
            }
            else
            {
              string AM = "", TM = "",temp = T,parentAM="",parentTM="";
              string parent = sym.getParent(T);
              T = sym.lookupClassData(T, N, AM, TM);


              if (T == "none")
              {
                if(parent != "none"){
                  T = sym.lookupClassData(parent,N,parentAM,parentTM);
                }

                if(T == "none")
                  this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (statCheck && (TM != "static" && parentTM != "static"))
              {
                this->semErrors.push_back("Called Member should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (AM == "private" || AM == "protected")
              {
                this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if(parentAM == "private" || parentAM == "protected"){
                this->semErrors.push_back("Can't access private or protected member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
            }      
                }
          return true;
        }
      }
    }
    return false;
  }

  bool trail_oe_no_arr(string N, bool &statCheck, string &T,string &name)
  {
    string temp = this->lexemes.at(this->counter).getClassName();
    if (temp == ".")
    {
      this->counter++;
      if (this->lexemes.at(this->counter).getClassName() == "ID")
      {
        name += "." + this->lexemes.at(this->counter).getWord();
        if (T == "")
        {
          T = sym.lookupST(N);
          if (T == "none")
          {
            this->semErrors.push_back(" Undeclared class object " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }

          N = this->lexemes.at(this->counter).getWord();
        }
        else
            {
              string AM = "", TM = "",temp = T,parentAM="",parentTM="";
              string parent = sym.getParent(T);
              T = sym.lookupClassData(T, N, AM, TM);


              if (T == "none")
              {
                if(parent != "none"){
                  T = sym.lookupClassData(parent,N,parentAM,parentTM);
                }

                if(T == "none")
                  this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (statCheck && (TM != "static" && parentTM != "static"))
              {
                this->semErrors.push_back("Called Member should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (AM == "private" || AM == "protected")
              {
                this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if(parentAM == "private" || parentAM == "protected"){
                this->semErrors.push_back("Can't access private or protected member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
            } 
                   statCheck = false;

        this->counter++;
        if (this->function_or_trail(N, statCheck, T,name))
          return true;
      }
    }
    else
    {
      if (temp == "=" || temp == "inc_dec"){
        if (T == "")
        {
            this->Register++;
           this->code += "T" + to_string(this->Register) + " = "+ name +"\n";

          T = sym.lookupST(N);
          if (T == "none")
          {
            this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
          }
        }
        else
            {
              string AM = "", TM = "",temp = T,parentAM="",parentTM="";
              string parent = sym.getParent(T);
              T = sym.lookupClassData(T, N, AM, TM);


              if (T == "none")
              {
                if(parent != "none"){
                  T = sym.lookupClassData(parent,N,parentAM,parentTM);
                }

                if(T == "none")
                  this->semErrors.push_back(" Undeclared variable " + N + " at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (statCheck && (TM != "static" && parentTM != "static"))
              {
                this->semErrors.push_back("Called Member should be static at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if (AM == "private" || AM == "protected")
              {
                this->semErrors.push_back("Can't access private or protected member at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
              }

              if(parentAM == "private" || parentAM == "protected"){
                this->semErrors.push_back("Can't access private or protected member of parent class "+parent+" at line " + to_string(this->lexemes.at(this->counter).getLineNo()));
 
              }
            }
                  statCheck = false;

      if (this->assign_or_inc_dec(T))
        return true;
        }
    }
    return false;
  }

  bool list1(string CN)
  {
    if (this->lexemes.at(this->counter).getClassName() == ",")
    {
      this->counter++;
      if (this->obj_dec(CN))
        return true;
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == "Terminator")
        return true;
    }
    return false;
  }

  bool next1(int &count)
  {
    if (this->lexemes.at(this->counter).getClassName() == ",")
    {
      this->counter++;
      if (this->OEs(count))
        return true;
    }
    else
    {
      if (this->lexemes.at(this->counter).getClassName() == ")")
        return true;
    }
    return false;
  }

  string ConstType(string type)
  {
    if (type == "IntConst")
    {
      return "int";
    }

    if (type == "FloatConst")
    {
      return "float";
    }

    if (type == "StringConst")
    {
      return "string";
    }

    if (type == "CharConst")
    {
      return "char";
    }

    if (type == "BoolConst")
    {
      return "bool";
    }
    return "none";
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
  bool fromFunc;
  std::vector<Lexeme> lexemes;
  std::vector<string> semErrors;
  Words words;
  int counter;
  string code;
  int label;
  int Register;
  SymbolTable sym;
};
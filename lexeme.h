#include <iostream>
using namespace std;

class Lexeme
{
  public:
  Lexeme()
  {
    this->line_no = 0;
    this->class_name = "";
    this->word = "";
  }

  Lexeme(int line_no, string class_name, string word)
  {
    this->line_no = line_no;
    this->class_name = class_name;
    this->word = word;
  }

  void setLexeme(int line_no, string class_name, string word)
  {
    this->line_no = line_no;
    this->class_name = class_name;
    this->word = word;
  }

  int getLineNo()
  {
    return this->line_no;
  }

  string getClassName()
  {
    return this->class_name;
  }

  string getWord()
  {
    return this->word;
  }

  friend ostream &operator<<(ostream &out, Lexeme &lex)
  {
    return out << lex.word << " " << lex.class_name << " " << lex.line_no << endl;
  }

private:
  int line_no;
  string class_name;
  string word;
};
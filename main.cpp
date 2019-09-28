#include <iostream>
#include <regex>
#include "lexical.h"
using namespace std;

int main()
{
  /*string value = "\'\\'";
  regex regChar("(\\'((\\n)|(\\')|(\\\\)|(\\r)|(\\t)|(\\b)|(\\f)|(\\v)|(\\0)|(\")|(.)|([0-9]{1,3}))\\')");
  if (regex_match(value, regChar))
  {
    cout << "YES!";
  }*/
  LexicalAnalyzer analyzer;
  analyzer.breakWords("code.txt");
  analyzer.print();
  analyzer.write("tokens.txt");
  system("pause");
}

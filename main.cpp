#include <iostream>
#include <regex>
#include <vector>
#include "lexical.h"
using namespace std;

int main()
{
  LexicalAnalyzer analyzer;
  analyzer.breakWords("code.txt");
  analyzer.print();
  analyzer.write("tokens.txt");
  cout << analyzer.syntaxStart() << endl;
  system("pause");
}

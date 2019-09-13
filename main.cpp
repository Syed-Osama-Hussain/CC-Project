#include <iostream>
#include "lexical.h"
using namespace std;

int main()
{
  LexicalAnalyzer analyzer;
  analyzer.breakWords("code.txt");
  analyzer.print();
  analyzer.write();
  system("pause");
}

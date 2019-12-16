#include <iostream>
#include <vector>
#include "data.h"
#include "scope.h"
using namespace std;

class SymbolTable
{
  public:
  SymbolTable()
  {
    this->currentClass = "";
  }

  private:
  string currentClass;
  vector<Data> DataTable;
  vector<Scope> ScopeTable;
  stack <int> scopeStack;
};

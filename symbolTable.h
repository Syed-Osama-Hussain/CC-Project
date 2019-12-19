#include <iostream>
#include <vector>
#include <Stack>
#include "data.h"
#include "scope.h"
#include <algorithm>
#include <bits/stdc++.h>
using namespace std;

class SymbolTable
{
public:
  SymbolTable()
  {
    this->currentClass = "";
    this->constructCompTable();
  }

  void constructCompTable()
  {
    string key, val;
    std::ifstream file("compatibility.txt", std::ios::in);
    if (file)
    {
      while (!file.eof())
      {
        file >> key;
        file >> val;
        this->compTable[key] = val;
      }
    }
    file.close();
  }

  bool insertData(Data data)
  {
    if (this->lookupDT(data.getName()) == "none")
    {
      this->DataTable.push_back(data);
      return true;
    }
    else
    {
      return false;
    }
  }

  string lookupDT(string name)
  {

    auto it = find_if(this->DataTable.begin(), this->DataTable.end(), [&name](Data obj) { return obj.getName() == name; });

    if (it != this->DataTable.end())
    {
      auto index = std::distance(this->DataTable.begin(), it);
      return this->DataTable.at(index).getType();
    }
    return "none";
  }

  string lookupDT(string name, string parent)
  {

    auto it = find_if(this->DataTable.begin(), this->DataTable.end(), [&name, &parent](Data obj) { return obj.getName() == name && obj.getParent() == parent; });

    if (it != this->DataTable.end())
    {
      auto index = std::distance(this->DataTable.begin(), it);
      return this->DataTable.at(index).getType();
    }
    return "none";
  }

  bool insertClassData(string name, ClassData data)
  {
    auto it = find_if(this->DataTable.begin(), this->DataTable.end(), [&name](Data obj) { return obj.getName() == name; });

    if (it != this->DataTable.end())
    {
      auto index = std::distance(this->DataTable.begin(), it);
      return this->DataTable.at(index).insertCDT(data);
    }
    return false;
  }

  string lookupClassData(string classname, string name, string &AM, string &TM)
  {
    auto it = find_if(this->DataTable.begin(), this->DataTable.end(), [&classname](Data obj) { return obj.getName() == classname; });

    if (it != this->DataTable.end())
    {
      auto index = std::distance(this->DataTable.begin(), it);
      return this->DataTable.at(index).lookupCDT(name, AM, TM);
    }
    return "none";
  }

  bool insertST(Scope data)
  {
    if (this->lookupST(data.getName()) == "none")
    {
      this->ScopeTable.push_back(data);
      return true;
    }
    else
    {
      return false;
    }
  }

  string lookupST(string name)
  {
    stack<int> copyStack = this->scopeStack;

    while (!copyStack.empty())
    {
      auto it = find_if(this->ScopeTable.begin(), this->ScopeTable.end(), [&name, &copyStack](Scope obj) { return obj.getName() == name && obj.getScope() == copyStack.top(); });

      if (it != this->ScopeTable.end())
      {
        auto index = std::distance(this->ScopeTable.begin(), it);
        return this->ScopeTable.at(index).getType();
      }
      copyStack.pop();
    }

    if (this->currentClass != "")
    {
      string AM = "", TM = "";
      return this->lookupClassData(this->currentClass, name, AM, TM);
    }

    return "none";
  }

  bool lookupFunction(string name, string args, string classname, string &AM, string &TM)
  {
    if (classname == "")
    {
      auto it = find_if(this->DataTable.begin(), this->DataTable.end(), [&name, &args](Data obj) { return obj.getName() == name && obj.getType() == args; });

      if (it != this->DataTable.end())
      {
        return true;
      }
    }
    else
    {
      auto it = find_if(this->DataTable.begin(), this->DataTable.end(), [&classname](Data obj) { return obj.getName() == classname; });

      if (it != this->DataTable.end())
      {
        auto index = std::distance(this->DataTable.begin(), it);
        return this->DataTable.at(index).lookupClassFunction(name, args, AM, TM);
      }
    }

    return false;
  }

  string compatibilityCheck(string operand1, string operand2, string Operator)
  {
    if (this->compTable.find(operand1 + operand2 + Operator) != this->compTable.end())
    {
      string result = this->compTable.find(operand1 + operand2 + Operator)->second;
      return result;
    }

    if (this->compTable.find(operand2 + operand1 + Operator) != this->compTable.end())
    {
      string result = this->compTable.find(operand2 + operand1 + Operator)->second;
      return result;
    }

    return "Uncompatible";
  }

  string compatibilityCheck(string operand, string Operator)
  {
    if (this->compTable.find(operand + Operator) != this->compTable.end())
    {
      string result = this->compTable.find(operand + Operator)->second;
      return result;
    }
    else
    {
      return "Uncompatible";
    }
  }

  int getScope()
  {
    if (this->scopeStack.empty())
    {
      return -1;
    }
    else
    {
      return this->scopeStack.top();
    }
  }

  void CreateScope()
  {
    if (this->scopeStack.empty())
    {
      this->scopeStack.push(0);
    }
    else
    {
      this->scopeStack.push(this->scopeStack.top()++);
    }
  }

  void DestroyScope()
  {
    if (!this->scopeStack.empty())
    {
      this->scopeStack.pop();
    }
  }

private:
  string currentClass;
  vector<Data> DataTable;
  vector<Scope> ScopeTable;
  stack<int> scopeStack;
  map<string, string> compTable;
};

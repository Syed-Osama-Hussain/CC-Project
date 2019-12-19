#include <iostream>
#include "classData.h"
#include <algorithm>
using namespace std;

class Data
{
public:
  Data()
  {
    this->name = "";
    this->type = "";
    this->parent = "";
  }

  Data(string name, string type, string parent)
  {
    this->name = name;
    this->type = type;
    this->parent = parent;
  }

  Data operator=(const Data &copy)
  {
    this->name = copy.name;
    this->type = copy.type;
    this->parent = copy.parent;
    this->CDT = copy.CDT;
  }

  string getName()
  {
    return this->name;
  }

  string getType()
  {
    return this->type;
  }

  string getParent()
  {
    return this->parent;
  }

  void setName(string name)
  {
    this->name = name;
  }

  void setType(string type)
  {
    this->type = type;
  }

  void setParent(string parent)
  {
    this->parent = parent;
  }

  bool insertCDT(ClassData data)
  {
    string AM = "", TM = "";
    if (this->lookupCDT(data.getName(), AM, TM) == "none")
    {
      this->CDT.push_back(data);
      return true;
    }
    else
    {
      return false;
    }
  }

  string lookupCDT(string name, string &AM, string &TM)
  {
    auto it = find_if(this->CDT.begin(), this->CDT.end(), [&name](ClassData obj) { return obj.getName() == name; });

    if (it != this->CDT.end())
    {
      auto index = std::distance(this->CDT.begin(), it);
      AM = this->CDT.at(index).getAM();
      TM = this->CDT.at(index).getTM();
      return this->CDT.at(index).getType();
    }
    return "none";
  }

  bool lookupClassFunction(string name, string args, string &AM, string &TM)
  {
    auto it = find_if(this->CDT.begin(), this->CDT.end(), [&name, &args](ClassData obj) { return obj.getName() == name && obj.getType() == args; });

    if (it != this->CDT.end())
    {
      auto index = std::distance(this->CDT.begin(), it);
      AM = this->CDT.at(index).getAM();
      TM = this->CDT.at(index).getTM();
      return true;
    }
    return false;
  }

private:
  string name;
  string type;
  string parent;
  vector<ClassData> CDT;
};
#include <iostream>
#include "classData.h"
using namespace std;


class Data{
  public:

    Data(){
      this->name = "";
      this->type = "";
      this->parent = "";
    }


    Data(string name,string type,string parent,ClassData CDT){
      this->name = name;
      this->type = type;
      this->parent = parent;
      this->CDT = CDT;
    }


    void operator = (const Data &copy){
      this->name = copy.name;
      this->type = copy.type;
      this->parent = copy.parent;
      this->CDT = copy.CDT;
    }

    string getName(){
      return this->name;
    }

    string getType(){
      return this->type;
    }

    string getParent(){
      return this->parent;
    }

    
    void setName(string name){
      this->name = name;
    }

    void setType(string type){
      this->type = type;
    }

    void setParent(string parent){
      this->parent = parent;
    }

    bool InsertCDT(ClassData data){
      this->CDT.push_back(data);
    }


    string LookupCDT(string name,string AM, string TM){

      auto it = find_if(this->CDT.begin(), this->CDT.end(), [&name](const ClassData& obj) {return obj.getName() == name;})

      if (it != this->CDT.end())
      {
        // found element. it is an iterator to the first matching element.
        // if you really need the index, you can also get it:
        auto index = std::distance(this->CDT.begin(), it);
        return this->CDT.at(index).getType();
      }
      return "none";
    }

  private:
    string name;
    string type;
    string parent;
    vector <ClassData> CDT;
};
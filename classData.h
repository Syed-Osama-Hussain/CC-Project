#include <iostream>
using namespace std;

class ClassData{

  public:
    ClassData(string name,string type,string AM,string TM){
      this->name = name;
      this->type = type;
      this->AM = AM;
      this->TM = TM;
    }


    void operator = (const ClassData &copy){
      this->name = copy.name;
      this->type = copy.type;
      this->AM = copy.AM;
      this->TM = copy.TM;
    }

    string getName(){
      return this->name;
    }

    string getType(){
      return this->type;
    }

    string getAM(){
      return this->AM;
    }

    string getTM(){
      return this->TM;
    }

    
    void setName(string name){
      this->name = name;
    }

    void setType(string type){
      this->type = type;
    }

    void setAM(string AM){
      this->AM = AM;
    }

    void setTM(string TM){
      this->TM = TM;
    }

  private:
    string name;
    string type;
    string AM;
    string TM;
    
};
#include <iostrem>
using namespace std;

class Scope{
    public:
    Scope(string name,string type,int scope){
      this->name = name;
      this->type = type;
      this->scope = scope;
    }


    void operator = (const Scope &copy){
      this->name = copy.name;
      this->type = copy.type;
      this->scope = copy.scope;
    }

    string getName(){
      return this->name;
    }

    string getType(){
      return this->type;
    }

    int getScope(){
      return this->scope;
    }

    
    void setName(string name){
      this->name = name;
    }

    void setType(string type){
      this->type = type;
    }

    void setScope(int scope){
      this->scope = scope;
    }


  private:
    string name;
    string type;
    int scope;

};
#include <iostream>
#include <regex>

using namespace std;

int main()
{
  string a = "\"ab65@\"\"";
  regex regInt("((\\+[1-9][0-9]*)|(\\-[1-9][0-9]*)|([1-9][0-9]*))");
  regex regFloat("((\\+[0-9]*\\.[0-9]+)|([0-9]*\\.[0-9]+)|(\\-[0-9]*\\.[0-9]+))");
  regex regIdentifier("(([A-Za-z]*\\_+[A-Za-z]*[0-9]*\\_*[A-Za-z]*\\_*)|([A-Za-z][A-Za-z]*[0-9]*[A-Za-z]*))");
  regex regChar("(\\'((\\n)|(\\')|(\\\\)|(\\r)|(\\t)|(\\b)|(\\f)|(\\v)|(\\0)|(\")|(.)|([0-9]{1,3}))\\')");
  regex regString("(\"(.*)\")");

  if (regex_match(a, regString))
  {
    cout << "Regex matched" << endl;
  }
  else
  {
    cout << "Not matched";
  }
  system("pause");
}
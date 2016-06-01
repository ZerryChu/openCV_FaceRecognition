#ifndef NUMBER_TO_PERSON_MAP_H_INCLUDED
#define NUMBER_TO_PERSON_MAP_H_INCLUDED
#include <string>
#include <map>
using namespace std;

struct Person {
public:
    string name;
    int age;
    string info;
	Person(string name, int age, string info);
	Person();
};

map<int, Person > mp;


#endif // NUMBER_TO_PERSON_MAP_H_INCLUDED

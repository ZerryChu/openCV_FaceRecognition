 #ifndef MAP_AND_PATH_H_INCLUDED
#define MAP_AND_PATH_H_INCLUDED

#include <string>
#include <map>
using namespace std;
#define FILEPATH "C://Users//fangli//Desktop//master//openCV_FaceRecognition-master//FaceRecognition//VideoMFC//einfacedata//trainingdata//at.txt"
#define PICTUREPATH "C:\\Users\\fangli\\Desktop\\master\\openCV_FaceRecognition-master\\FaceRecognition\\VideoMFC\\einfacedata\\trainingdata\\"
#define XMLPATH  "C:\\Users\\fangli\\Desktop\\master\\openCV_FaceRecognition-master\\FaceRecognition\\VideoMFC\\einfacedata\\trainingdata\\trainresult.xml"
#define ConfigXML "C:\\Users\\fangli\\Desktop\\master\\openCV_FaceRecognition-master\\FaceRecognition\\VideoMFC\\VideoMFC\\student.xml"

struct Person {
public:
    string name;
    int age;
    string info;
	Person(string name, int age, string info);
	Person();
};

void parseXML();

extern map<int, Person> mp;

#endif // MAP_AND_PATH_H_INCLUDED

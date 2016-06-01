 #ifndef MAP_AND_PATH_H_INCLUDED
#define MAP_AND_PATH_H_INCLUDED

#include <string>
#include <map>
using namespace std;
// #define FILEPATH "C:\\Users\\Administrator\\Desktop\\openCV_FaceRecognition-master\\FaceRecognition\\VideoMFC\\einfacedata\\trainingdata\\at.txt"
#define FILEPATH ".\\trainingdata\\at.txt"

// #define PICTUREPATH "C:\\Users\\Administrator\\Desktop\\openCV_FaceRecognition-master\\FaceRecognition\\VideoMFC\\einfacedata\\trainingdata\\"
#define PICTUREPATH ".\\trainingdata\\"

// #define XMLPATH  "C:\\Users\\Administrator\\Desktop\\openCV_FaceRecognition-master\\FaceRecognition\\VideoMFC\\einfacedata\\trainingdata\\trainresult.xml"
#define XMLPATH  ".\\trainingdata\\trainresult.xml"

// #define ConfigXML "C:\\Users\\Administrator\\Desktop\\openCV_FaceRecognition-master\\FaceRecognition\\VideoMFC\\VideoMFC\\student.xml"
#define ConfigXML ".\\student.xml"

// #define SOURCEPIC "C:\\Users\\Administrator\\Desktop\\openCV_FaceRecognition-master\\FaceRecognition\\VideoMFC\\sourcePic\\target.jpg"
#define SOURCEPIC ".\\target.jpg"

struct Person {
public:
    string name;
    string info;
	Person(string name, string info);
	Person();
};

void parseXML();

extern map<int, Person> mp;

#endif // MAP_AND_PATH_H_INCLUDED

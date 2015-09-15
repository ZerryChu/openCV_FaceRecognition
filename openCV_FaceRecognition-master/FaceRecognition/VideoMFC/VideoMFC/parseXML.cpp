#include "stdafx.h"
#include "map_and_path.h"
#include <iostream>
#include <sstream>
#include <stdio.h>
#include "tinyxml.h"
#include "tinystr.h"
using namespace std;

Person:: Person()
{
};

Person:: Person(string name, int age, string info)
{
	this->name = name;
	this->age = age;
	this->info = info;
}

map<int, Person> mp;

void parseXML() {
	try {
		string fileName = ConfigXML;
		TiXmlDocument *myDocument = new TiXmlDocument(fileName.c_str());
		myDocument->LoadFile();
		//��ø�Ԫ�أ���Persons��
		TiXmlElement *RootElement = myDocument->RootElement();
		//��õ�һ��Person�ڵ㡣

		TiXmlElement *FirstPerson = RootElement->FirstChildElement();
		//��õ�һ��Person��name�ڵ��age�ڵ��ID���ԡ�
		while (NULL != FirstPerson) {
			TiXmlElement *NameElement = FirstPerson->FirstChildElement();
			TiXmlElement *AgeElement = NameElement->NextSiblingElement();
			TiXmlElement *InfoElement = AgeElement->NextSiblingElement();
			TiXmlElement *NumberElement = InfoElement->NextSiblingElement();
			string name(NameElement->FirstChild()->Value());
			string info(InfoElement->FirstChild()->Value());
			string ageString(AgeElement->FirstChild()->Value());
			string iiid(FirstPerson->FirstAttribute()->Value());
			stringstream ss;
			ss << ageString;
			int age;
			ss >> age;
			Person person(name, age, info);
			string idString(FirstPerson->FirstAttribute()->Value());
			stringstream ssss;
			ssss << idString;
			int id;
			ssss >> id;
			mp[id] = person;
			FirstPerson = FirstPerson->NextSiblingElement();
			//MessageBox(NULL, TEXT(mp[1].name.c_str()), TEXT(" "), MB_OK);
		}
	}
	catch (string& e)
	{
		cerr << "error.\n";
		exit(1);
	}
}


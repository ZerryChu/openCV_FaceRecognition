#include "stdafx.h"
#include "map_and_path.h"
#include <iostream>
#include <sstream>
#include <stdio.h>
#include "tinyxml.h"
#include "tinystr.h"
using namespace std;

Person:: Person(){};

Person::Person(string name, string info)
{
	this->name = name;
	this->info = info;
}

map<int, Person> mp;

// ���������ļ�
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
			TiXmlElement *InfoElement = NameElement->NextSiblingElement();
			string name(NameElement->FirstChild()->Value());
			string info(InfoElement->FirstChild()->Value());
			string idString(FirstPerson->FirstAttribute()->Value());
			
			// ��ʼ��person
			Person person(name, info);

			stringstream ssss;
			ssss << idString;
			int id;
			ssss >> id;
			mp[id] = person;

			// ������һ��person��ǩ
			FirstPerson = FirstPerson->NextSiblingElement();
		}
	}
	catch (string& e)
	{
		cerr << "error.\n";
		exit(1);
	}
}


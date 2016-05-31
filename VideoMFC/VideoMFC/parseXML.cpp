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

// 解析配置文件
void parseXML() {
	try {
		string fileName = ConfigXML;
		TiXmlDocument *myDocument = new TiXmlDocument(fileName.c_str());
		myDocument->LoadFile();
		//获得根元素，即Persons。
		TiXmlElement *RootElement = myDocument->RootElement();
		//获得第一个Person节点。

		TiXmlElement *FirstPerson = RootElement->FirstChildElement();
		//获得第一个Person的name节点和age节点和ID属性。
		while (NULL != FirstPerson) {
			TiXmlElement *NameElement = FirstPerson->FirstChildElement();
			TiXmlElement *InfoElement = NameElement->NextSiblingElement();
			string name(NameElement->FirstChild()->Value());
			string info(InfoElement->FirstChild()->Value());
			string idString(FirstPerson->FirstAttribute()->Value());
			
			// 初始化person
			Person person(name, info);

			stringstream ssss;
			ssss << idString;
			int id;
			ssss >> id;
			mp[id] = person;

			// 解析下一个person标签
			FirstPerson = FirstPerson->NextSiblingElement();
		}
	}
	catch (string& e)
	{
		cerr << "error.\n";
		exit(1);
	}
}


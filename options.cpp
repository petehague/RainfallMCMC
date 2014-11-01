/*
	Command line options object
	Author: Peter Hague
	Created: 20/07/14
*/
#include <algorithm>
#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>
#include <cinttypes>

#include "include/options.hpp"
#include "include/defaults.hpp"

using namespace std;

//--------------------------------------------------
//Appending a key and value to the list of options

void options::addval(string k, double v, opt_vetostate m, opt_rank r) {
	transform(k.begin(), k.end(), k.begin(), ::tolower);
	if (r==opt_array) arrayindex.push_back(keycount(k)); else arrayindex.push_back(0);
	key.push_back(k);
	double_val.push_back(v);
	string_val.push_back("");
	type.push_back(opt_double);
	veto.push_back(m);
	rank.push_back(r);
}

void options::addval(string k, string v, opt_vetostate m, opt_rank r) {
	transform(k.begin(), k.end(), k.begin(), ::tolower);
	if (r==opt_array) arrayindex.push_back(keycount(k)); else arrayindex.push_back(0);
	key.push_back(k);
	double_val.push_back(0.0);
	string_val.push_back(v);
	type.push_back(opt_string);
	veto.push_back(m);
	rank.push_back(r);
}

//--------------------------------------------------
//Setting the value of an already existing key

void options::setval(string k, double v) {
	transform(k.begin(), k.end(), k.begin(), ::tolower);
	int index = 0;
	while(k.compare(key[index])!=0) index++;
	if (index<k.size()) {
		double_val[index] = v;
		veto[index] = opt_noveto;
	} else addval(k, v, opt_noveto, opt_single);
}

void options::setval(string k, string v) {
	transform(k.begin(), k.end(), k.begin(), ::tolower);
	int index = 0;
	while(k.compare(key[index])!=0) index++;
	if (index<k.size()) {
		string_val[index] = v;
		veto[index] = opt_noveto;
	} else addval(k, v, opt_noveto, opt_single);
}

opt_type options::gettype(string k) {
	transform(k.begin(), k.end(), k.begin(), ::tolower);
	int index = 0;
	while(k.compare(key[index])!=0) index++;
	return type[index];
}

opt_rank options::getrank(string k) {
	transform(k.begin(), k.end(), k.begin(), ::tolower);
	int index = 0;
	while(k.compare(key[index])!=0) index++;
	return rank[index];
}

void *options::getval(string k) {
	transform(k.begin(), k.end(), k.begin(), ::tolower);
	int index = 0;
	while(k.compare(key[index])!=0) index++;
	if (type[index]==opt_double) return &double_val[index];
	return &string_val[index];	
}

double options::getdoubleval(string k) {
	transform(k.begin(), k.end(), k.begin(), ::tolower);
	int index = 0;
	while(k.compare(key[index])!=0) index++;
	return double_val[index];
}

string options::getstringval(string k) {
	transform(k.begin(), k.end(), k.begin(), ::tolower);
	int index = 0;
	while(k.compare(key[index])!=0) index++;
	return string_val[index];
}

//--------------------------------------------------
//Setting the value of an array key

bool options::setval(string k, int i, double v) {
	int index = 0;
	
	transform(k.begin(), k.end(), k.begin(), ::tolower);
	for(int j=i;j>0;j--) {
		while(k.compare(key[index])!=0) index++;
		if (rank[index]==opt_emptyarray) {
			double_val[index] = v;
			rank[index]=opt_array;
			return true;
		}
	}

	if (i>=keycount(k)) { addval(k, v, opt_noveto, opt_array); return true; }
	while(k.compare(key[index])!=0) index++;
	if (index<k.size()) {
		double_val[index] = v;
		return true;
	} else return false;
}

bool options::setval(string k, int i, string v) {
	int index = 0;

	transform(k.begin(), k.end(), k.begin(), ::tolower);
	for(int j=i;j>0;j--) {
		while(k.compare(key[index])!=0) index++;
		if (rank[index]==opt_emptyarray) {
			string_val[index] = v;
			rank[index]=opt_array;
			return true;
		}
	}
	
	if (i>=keycount(k)) addval(k, v, opt_noveto, opt_array);
	while(k.compare(key[index])!=0) index++;
	if (index<k.size()) {
		string_val[index] = v;
		return true;
	} else return false;
}

void *options::getval(string k, int i) {
	transform(k.begin(), k.end(), k.begin(), ::tolower);
	int index = 0;
	for(i=i;i>0;i--)
		while(k.compare(key[index])!=0) index++;
	while(k.compare(key[index])!=0) index++;
	if (index>=k.size()) return NULL;
	if (type[index]==opt_double) return &double_val[index];
	return (void *)&string_val[index];	
}

double options::getdoubleval(string k, int i) {
	int index = 0;
	transform(k.begin(), k.end(), k.begin(), ::tolower);
	while (arrayindex[index]!=i || k.compare(key[index])!=0) index++;
	return double_val[index];
}

string options::getstringval(string k, int i) {
	int index = 0;
	transform(k.begin(), k.end(), k.begin(), ::tolower);
	while (arrayindex[index]!=i || k.compare(key[index])!=0) index++;
	return string_val[index];
}

//--------------------------------------------------

bool options::vetoCheck() {
	bool result = true;
	
	for(int i=0;i<veto.size();i++) 
		if (veto[i]==opt_veto) {
			cout << "Mandatory option missing: " << key[i] << endl;
			result=false;
		}
		
	return result;
}

//--------------------------------------------------

int options::keycount(string k) {
	transform(k.begin(), k.end(), k.begin(), ::tolower);
	int result = 0;
	for (int i=0;i<key.size();i++)
		if (k.compare(key[i])==0) result++;
	return result;
}

//--------------------------------------------------

void options::parse(string item) {
	char c_item[item.size()];
	char *keyname, *value;
	
	string *valstr;
	strcpy(c_item, item.c_str());
	keyname = strtok(c_item, "= ");
	if (keyname!=NULL && keyname[0]!='#') {
		value = strtok(NULL, "= ");
		if (value==NULL) {
			cout << "Using option file " << keyname << endl;
			parseFile(keyname);
		} else {	
			valstr = new string(value);
			if (keycount(keyname)==0) {
				if ((*valstr).find_first_not_of("0123456789+-.")==string::npos) 
					addval(keyname, stod(*valstr), opt_noveto, opt_single);
				else
					addval(keyname, *valstr, opt_noveto, opt_single);
			} else if (getrank(keyname)==opt_array || getrank(keyname)==opt_emptyarray) 
				if (gettype(keyname)==opt_double) setval(keyname, keycount(keyname)+1, stod(*valstr)); else setval(keyname, keycount(keyname)+1, *valstr);
			else 
				if (gettype(keyname)==opt_double) setval(keyname, stod(*valstr)); else setval(keyname, *valstr);
			delete valstr;
		}
	}
}

void options::parseFile(string filename) {
	fstream optfile;
	
	optfile.open(filename.c_str(), fstream::in);
	//while(getline(optfile, buffer)) parse(buffer);
	for (string line; getline(optfile, line); ) parse(line);
	optfile.close();
}

void options::parseCL(int argc, char **argv) {
	for (int i=1;i<argc;i++) parse(argv[i]);
}

//--------------------------------------------------

void options::report() {
	cout << "======================================" << endl;
	cout << "Options used" << endl;
	cout << "======================================" << endl;
	for(int i=0;i<key.size();i++) {
		cout << key[i];
		if (rank[i]==opt_emptyarray) cout << "[]";
		if (rank[i]==opt_array) cout << "[" << arrayindex[i] << "]"; 
		cout << " = ";
		if (type[i]==opt_double) 
			cout << double_val[i] << endl;
		else
			cout << string_val[i] << endl;
	}
	cout << "======================================" << endl;
}

//--------------------------------------------------

options::options() {
	ADD_DEFAULTS
}

options::options(int argc, char **argv) {
	for (int i=1;i<argc;i++) parse(argv[i]);
}
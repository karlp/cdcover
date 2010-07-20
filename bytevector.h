/*
    STRINGLIB
	Ulli Meybohm's C++ String Library
	
    Copyright (C) 2000	Ulli Meybohm, www.meybohm.de (ulli@meybohm.de)

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/


#ifndef binary_H
#define binary_H

/*
	Bytevector-Class , Storage for Strings or binary data
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <vector>


#ifdef STRINGLIB_USE_GLIBC

#include <fnmatch.h>
#include <wordexp.h>
#include <regex.h>

#endif


#ifndef BYTEVECTOR_CFGSEPERATOR
#define BYTEVECTOR_CFGSEPERATOR "="
#endif



class VectorIndexList : public std::vector<int> {
};


class Bytevector{
private:
	
	void create(int newsize);

public:
	int size;	//data size
	char* data;
	/*Container for binary/string data
    data automatically terminates with Zero at data[size]
	First Item is data[0] , Last Item is data[size-1] !
	Bytevector.data is public for speed advantage, so don't
	play with it and only use read-only access!!
	Manual free(data), data=XY or other pointer arithmethic operations
	will surely cause a segmentation fault!
	*/
	
	Bytevector();
	Bytevector(Bytevector &b);
	Bytevector(const Bytevector &b);
	Bytevector(int n);
	Bytevector(const char* pchar);
	Bytevector(char* pchar);
	~Bytevector();
	
	void copyFromString(char* pchar);
	void copyFromString(const char* pchar);
	void copyFromBytevector(Bytevector &source, int start, int len);
	void copyFromBytevector(const Bytevector &source, int start, int len);
	void resize(int newsize);
	
	int  find(char c, int startpos);
	int  find(Bytevector &b, int startpos);
	VectorIndexList findall(Bytevector b);
	
	bool freadln(int file,char seperator); // returns false when eof
	bool freadln(int file); // returns false when eof
	void freadeof(int file);
	void fwrite(int file);
	void fwriteln(int file);
	bool readfile(Bytevector file);
	
	
	void replace(Bytevector from, Bytevector to);
	void append(Bytevector &source);
	void append(const Bytevector &source);
	void override(Bytevector &source, int overridepos);
	void override(const Bytevector &source, int overridepos);
	Bytevector copy(int start, int len);
	Bytevector copyRange(int indexA, int indexB);
	
	int toInt();
	bool toBool();
	
	//For config-files id=value
	Bytevector id();
	Bytevector value();
	void setValue(Bytevector &value);
	void setValue(const Bytevector &value);
	void setValue(const char *c);
	void setValue(int n);
	void setValue(bool b);


#ifdef STRINGLIB_USE_GLIBC
	bool fnmatch(Bytevector filenamepattern);
	bool getstdout(Bytevector shellcommand);
	bool regexmatch(Bytevector regex);

	//SET/GET Environmentvariables
	void setEnv(Bytevector varname);
	void getEnv(Bytevector varname);
	void cfg2env(); //= setenv(id(),value());

#endif
	
	Bytevector* clone();
	char* clonePCHAR();
	// Note: The memory, which is occupied by the cloned data
	// WILL NOT be released when Bytevector-Object is deleted
	// use delete(Bytevector*) or free(pchar)
	
	char& operator[](int i);
	//slow! Use Bytevector.data[i] for high speed access
	
	char* operator*();
	//returns Bytevector.data
	
	Bytevector operator+(Bytevector b);
	Bytevector& operator+=(Bytevector &b);
	Bytevector& operator+=(const Bytevector &b);
	Bytevector& operator+=(const char *c);
	Bytevector& operator+=(char *c);
	Bytevector& operator=(Bytevector &b);
	Bytevector& operator=(int n);
	Bytevector& operator=(const Bytevector &b);
	Bytevector& operator=(const char* pchar);
	Bytevector& operator=(char* pchar);
	bool operator==(Bytevector &b);
	bool operator!=(Bytevector &b);
	
/*	friend ifstream& operator >> (ifstream& s, Configuration& c);
   	friend ofstream& operator << (ofstream& s, Configuration& c);
 	friend ostream& operator << (ostream& s, Configuration& c);
	friend istream& operator >> (istream& s, Configuration& c);*/


};





#endif

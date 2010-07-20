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


#ifndef stringlist_H
#define stringlist_H
#include <vector>
#include <iostream>
#include <stdio.h>

#ifdef STRINGLIB_USE_GLIBC
#include <glob.h>
#include <wordexp.h>
#endif

#include "bytevector.h"

/*Definition of default Line and Field Seperators*/

#ifndef STRINGLIST_FS
#define STRINGLIST_FS ","
#endif
#ifndef STRINGLIST_LS
#define STRINGLIST_LS "\n"
#endif


//class VectorIndexList in "bytevector.h"

class MatrixIndex{
public:
	int x;
	int y;	
	MatrixIndex(){x=0;y=0;};
	MatrixIndex(MatrixIndex &m){ (*this)=m; };
	MatrixIndex(int x, int y) { this->x=x; this->y=y;  };
	MatrixIndex& operator=(MatrixIndex &m) {
	 this->x=m.x; this->y=m.y; return *this;	
	}
};

class MatrixIndexList: public std::vector<MatrixIndex> {
};



class Stringlist : public std::vector<Bytevector*> {
private:
    Stringlist::iterator getID(Bytevector id);
	
public:

	Bytevector lineSeperator; //Default='\n'
    Bytevector fieldSeperator; // Default=','
	
    Stringlist();
    Stringlist(Stringlist &s);
    Stringlist(Stringlist &s, VectorIndexList& itemsToCopy);
    Stringlist(int argc, char *argv[]);
    ~Stringlist(); //Destroys Binaries!

    void copySplit(Bytevector &source, Bytevector lineSeperator);
    void copySplit(Bytevector &source);

    void readfile(int file);
    void writefile(int file);


    void clear();
	void add(int argc,char *argv[]);
	void add(char *c);
	void add(const char *c);  //add copy of *b 	
	void add(Bytevector *b);  //insert *b without making a copy (fast, but little dangerous!)
    void add(Bytevector &b);  //insert copy
    void add(const Bytevector &b);  //insert copy
    void add(Stringlist &s);
    void add(Stringlist &s, VectorIndexList& itemsToAdd);
    void insert(Bytevector *b, int pos);  //takeover *b without copying
    void insert(Bytevector &b, int pos);  //insert copy
    void del(int pos);


#ifdef STRINGLIB_USE_GLIBC

	void glob(Bytevector filenamepattern);
	int wordexpand(Bytevector source);
    VectorIndexList grep(Bytevector regex);
#endif
	
    VectorIndexList fgrep(Bytevector searchstring);   //fast grep without regular expressions
    VectorIndexList findall(Bytevector b); //Returns all "b==Stringlist[i]"
    int find(Bytevector b,int startpos);   //Returns first occurance from startpos of b in Stringlist
	bool includes(Bytevector b);           // b in *this ?


    Bytevector* text();  // uses default line Seperator
    Bytevector* text(Bytevector Seperator);  //uses alternative LineSeperator
	Bytevector toBytevector();
	int totalTextSize(Bytevector seperator);

	

    //Configfile functions, maybe outsourced to derived class later!
    //structure: id=value
	//Can also be used as kind of hashtable (slow!)
    void set(Bytevector id, Bytevector value);
    void set(Bytevector id, const char *value);
    void set(Bytevector id, char *value);
    void set(Bytevector id, int value);
    void set(Bytevector id, bool value);
    Bytevector get(Bytevector id);
    bool getBool(Bytevector id);
    int  getInt(Bytevector id);


	//Functions for commandline-options
	bool option(Bytevector option);
	bool option(Bytevector option, Bytevector longoption);
	Bytevector optionvalue(Bytevector option); // command -option value
	Bytevector optionvalue(Bytevector option, Bytevector longoption); 
	

    void println(); //just for testing!

   	Bytevector& operator[](int i);
   	Bytevector operator()(int line, int field);
   	
   	Stringlist& operator=(Stringlist &s);
};



#endif

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


#include "stringlist.h"

using namespace std;

Stringlist::Stringlist(){
	lineSeperator=STRINGLIST_LS;
	fieldSeperator=STRINGLIST_FS;
}

Stringlist::Stringlist(Stringlist &s){
	lineSeperator=STRINGLIST_LS;
	fieldSeperator=STRINGLIST_FS;
	add(s);
}

Stringlist::Stringlist(int argc,char *argv[]){
	lineSeperator=STRINGLIST_LS;
	fieldSeperator=STRINGLIST_FS;
	add(argc,argv);
}

Stringlist::Stringlist(Stringlist &s, VectorIndexList& itemsToCopy){
	lineSeperator=STRINGLIST_LS;
	fieldSeperator=STRINGLIST_FS;
	add(s,itemsToCopy);
}

void Stringlist::add(int argc, char *argv[]){
	for (int i=0; i<argc; i++){
		add(argv[i]);
	}
}


bool Stringlist::includes(Bytevector b){
	return find(b,0)!=-1;
}


bool Stringlist::option(Bytevector option) {
	return  find(option,0)>=0;
}

bool Stringlist::option(Bytevector option, Bytevector longoption){
	if ( find(option,0)>=0 ) return true;
	if ( find(longoption,0)>=0 ) return true;
	return false;
}


Bytevector Stringlist::optionvalue(Bytevector option){
	int pos=find(option,0);
	if ( (pos<0) || ((unsigned int)pos>=size()) )
		return Bytevector();
	else
		return (*this)[pos];
}

Bytevector Stringlist::optionvalue(Bytevector option, Bytevector longoption){
	int pos=find(option,0);
	if ( (pos>=0) && ((unsigned int)pos<size()) )
		return (*this)[pos];
	pos=find(longoption,0);
	if ( (pos>=0) && ((unsigned int)pos<size()) )
		return (*this)[pos];
	return Bytevector();
}


#ifdef STRINGLIB_USE_GLIBC

VectorIndexList Stringlist::grep(Bytevector regex){
	int index=0;
	VectorIndexList vi;
	for (Stringlist::iterator i=begin(); i!=end(); i++, index++){
		if ( (*i)->regexmatch(regex)) {
			vi.push_back(index);
		}
	}
	return VectorIndexList(vi);
}


int Stringlist::wordexpand(Bytevector source){
	int err;
	wordexp_t *result;
	result= new wordexp_t;
	if ((err=std::wordexp(source.data, result, 0))==0) {
		clear();
		add(result->we_wordc,result->we_wordv);
	}
	wordfree(result);
	delete result;
	return err;
}


void Stringlist::glob(Bytevector filenamepattern){
	clear();
	glob_t *result;
	result= new glob_t;
	std::glob(filenamepattern.data, GLOB_MARK, 0, result);
	add(result->gl_pathc,result->gl_pathv);
	globfree(result);
	delete result;
}

#endif

void Stringlist::readfile(int file){
	Bytevector *b;
	b=new Bytevector;
	while( b->freadln(file) ){
		add(b);
		b=new Bytevector;
	}
}

void Stringlist::writefile(int file){
	for (Stringlist::iterator i=begin(); i!=end(); i++) {
		(*i)->fwriteln(file);
	}
}


void Stringlist::copySplit(Bytevector &source){
	copySplit(source, lineSeperator);
}


Stringlist& Stringlist::operator=(Stringlist &s) {
	Stringlist::clear();
	add(s);
	return *this;
}


Bytevector Stringlist::operator()(int line, int field){
	if ( line<(int)size() ){
		Stringlist s;
		s.copySplit( (*this)[line] , fieldSeperator);
		if ( field<(int)s.size() ) {
			return s[field];
		}
	};
	return Bytevector("");
}



void Stringlist::add(Stringlist &s, VectorIndexList& itemsToAdd){
	for ( VectorIndexList::iterator i=itemsToAdd.begin(); i!=itemsToAdd.end(); i++) {
		add(s[*i]);
	}
}


Stringlist::iterator Stringlist::getID(Bytevector id){
	for (Stringlist::iterator i=begin(); i!=end(); i++){
		if ( (*i)->id()==id ) {
		 	return i;
		}	
	}
	//else
	return end();
}




VectorIndexList Stringlist::fgrep(Bytevector searchstring){
	int index=0;
	VectorIndexList vi;
	for (Stringlist::iterator i=begin(); i!=end(); i++, index++){
		if ( (*i)->find(searchstring,0)!=-1 ) {
			vi.push_back(index);
		}
	}
	return VectorIndexList(vi);
}

VectorIndexList Stringlist::findall(Bytevector b){
	int index=0;
	VectorIndexList vi;
	for (Stringlist::iterator i=begin(); i!=end(); i++, index++){
		if ( (*(*i))==b ) {
			vi.push_back(index);
		}
	}
	return VectorIndexList(vi);
}

int Stringlist::find(Bytevector b, int startpos){
	int index=startpos;
	for (Stringlist::iterator i=begin()+startpos; i!=end(); i++, index++){
		if ( (*(*i))==b ) {
			return index;
		}
	}
	return -1;
}


void Stringlist::set(Bytevector id, Bytevector value){
	Stringlist::iterator item;
	item=getID(id);
	if (item!=end()) {
		(*item)->setValue(value);
	} else {
		Bytevector newItem;
		newItem+=id;
		newItem+="=";
		newItem+=value;
		add(Bytevector(newItem));	
	}
}

void Stringlist::set(Bytevector id, const char *value){
	set(id,Bytevector(value));
}

void Stringlist::set(Bytevector id, char *value){
	set(id,Bytevector(value));
}


void Stringlist::set(Bytevector id, int value){
	set(id, Bytevector(value));
}

void Stringlist::set(Bytevector id, bool value){
	set(id, value?1:0 );
}

Bytevector Stringlist::get(Bytevector id){
	Stringlist::iterator item;
	item=getID(id);
	if (item==end()) return Bytevector("");
	return (*getID(id))->value();
}

bool Stringlist::getBool(Bytevector id){
	Stringlist::iterator item;
	item=getID(id);
	if (item==end()) return false;//Exception better??
	return (*getID(id))->value().toBool();
}

int  Stringlist::getInt(Bytevector id){
	Stringlist::iterator item;
	item=getID(id);
	if (item==end()) return 0;//Exception better??
	return (*getID(id))->value().toInt();
}



Stringlist::~Stringlist(){
    Stringlist::clear();
}

void Stringlist::clear(){
	for (Stringlist::iterator i=begin(); i!=end(); i++){
		delete *i;
	}
	vector<Bytevector *>::clear();
}


int Stringlist::totalTextSize(Bytevector seperator){
	int result=0;
	for (Stringlist::iterator i=begin(); i!=end(); i++){
		result+=(*(*i)).size;
	}
	result+=seperator.size*size();
	return result;
}


Bytevector* Stringlist::text(){
	return text(Bytevector(lineSeperator));
}

//fast
Bytevector* Stringlist::text(Bytevector Seperator) {
	Bytevector *b;
	b=new Bytevector;
	int s=totalTextSize(Seperator);
	b->resize(s);
	int overridepos=0;
	for (Stringlist::iterator i=begin(); i!=end(); i++) {
		b->override( *(*i), overridepos);
		overridepos+=(*i)->size;
		b->override( Seperator, overridepos);
		overridepos+=Seperator.size;
	}
	return b;
}

//slow
Bytevector Stringlist::toBytevector(){
	Bytevector b;
	int s=totalTextSize(lineSeperator);
	b.resize(s);
	int overridepos=0;
	for (Stringlist::iterator i=begin(); i!=end(); i++) {
		b.override( *(*i), overridepos);
		overridepos+=(*i)->size;
		b.override( lineSeperator, overridepos);
		overridepos+=lineSeperator.size;
	}
	return Bytevector(b);
}






void Stringlist::add(Stringlist &s) {
	for (Stringlist::iterator i=s.begin(); i!=s.end(); i++) {
		push_back( (*i)->clone() );
	}
}


void Stringlist::add(Bytevector *b){
	push_back(b);
}

void Stringlist::add(char *c){
	Bytevector *b;
	b=new Bytevector;
	*b=c;
	push_back(b);
}

void Stringlist::add(const char *c){
	Bytevector *b;
	b=new Bytevector;
	*b=c;
	push_back(b);
}

void Stringlist::add(Bytevector &b){
	add(b.clone());
}

void Stringlist::add(const Bytevector &b){
	add(Bytevector(b));
}


void Stringlist::insert(Bytevector *b, int pos){
	iterator i=begin()+pos;
	vector<Bytevector *>::insert(i,b);
}

void Stringlist::insert(Bytevector &b, int pos){
	insert(b.clone(),pos);
}

void Stringlist::del(int pos){
	erase(begin()+pos);
}


void Stringlist::println() {
	for (int i=0; i<(int)size(); i++) {
		printf("%d. -%s-\n",i,(*this)[i].data);
	}
}




void Stringlist::copySplit(Bytevector &source, Bytevector lineSeperator){
	int startpos=0;
	int index;
	Bytevector *b;
	while( (index=source.find(lineSeperator,startpos)) != -1 ) {
		b=new Bytevector;
		if (index!=startpos) {
			*b=source.copyRange(startpos,index-1);
		} else {
			*b="";
		}
		push_back(b);
		startpos=index+lineSeperator.size;				
	}
	if (startpos<(source.size-1)){
		b=new Bytevector;
		*b=source.copyRange(startpos,source.size-1);
		push_back(b);
	}
}


Bytevector& Stringlist::operator[](int i){
	return *(vector<Bytevector *>::operator[](i));
}


// ARGUMENTVECTOR

/*char** Stringlist::argv(){
	char** args;
	args=(char **) malloc (size()*sizeof(char*));
	int index=0;
	for (Stringlist::iterator i=begin(); i!=end(); i++, index++) {
		args[index]= (*i)->clonePCHAR();
	}
	return args;
}*/






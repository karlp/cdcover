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


#include "bytevector.h"

using namespace std;

	Bytevector::Bytevector(){
		data=NULL;
		create(0);
	}
	

	
	Bytevector::Bytevector(Bytevector &b){
		data=NULL;
		create(0);
		operator=(b);	
	}	
	
#ifdef STRINGLIB_USE_GLIBC
	bool Bytevector::fnmatch(Bytevector filenamepattern){
		return ( std::fnmatch(filenamepattern.data, this->data, 0)==0);
	}

	bool Bytevector::regexmatch(Bytevector regex){
		regex_t *r;
		r=new regex_t;
		if (regcomp(r,regex.data,0)){
			/*ERROR*/
			delete r;
			return false;
		}
		if (regexec(r,data,0,0,0)) {
			/*Some Error occured or string does not match*/
			regfree(r);
			delete r;
			return false;
		} else {
			regfree(r);
			delete r;
			return true;
		}		
	}

	
	bool Bytevector::getstdout(Bytevector shellcommand){
		int err;
		wordexp_t *result;
		result= new wordexp_t;
		Bytevector source;
		source=Bytevector(Bytevector("\"`")+shellcommand+Bytevector("`\""));
		if ((err=std::wordexp(source.data, result, 0))==0) {
			(*this)=result->we_wordv[0];
		}
		wordfree(result);
		delete result;
		return (err!=0);
	}

	void Bytevector::setEnv(Bytevector varname){
		setenv(varname.data,data,true);
	}
	
	void Bytevector::getEnv(Bytevector varname){
		copyFromString(getenv(varname.data));			
	}


	void Bytevector::cfg2env(){
		setenv(id().data,value().data,true);
	}	

#endif	
	
	bool Bytevector::readfile(Bytevector file){
		int fd=open(file.data,O_RDONLY);
		if (fd>=0) {
			freadeof(fd);
			close(fd);
			return(true);
		} else return(false);
	}
	
	


	
	void Bytevector::fwrite(int file){
		write(file, data, size);
	}
	
	void Bytevector::fwriteln(int file){
		fwrite(file);
		write(file, "\n", 1);
	}

		
	bool Bytevector::Bytevector::freadln(int file){
		return freadln(file,'\n');
	}

	bool Bytevector::freadln(int file,char seperator){
		vector<Bytevector*>	v;
		Bytevector *b;
		int index=0;
		int status;
		b=new Bytevector;
		b->resize(1000);
		while( (status=read(file,&(b->data[index]),1)) && b->data[index++]!=seperator ){
			if (index==1000) {
				v.push_back(b);
				b=new Bytevector;	
				b->resize(1000);
				index=0;
			}
		}
		if (status==1) { index--; };
		v.push_back(b);
		resize((v.size()-1)*1000+index);
		for (int i=0 ; i<(int)(v.size()-1); i++) {
			for (int j=0; j<1000; j++) {
				data[i*1000+j]=(v[i])->data[j];
			}
		}
		for (int i=0; i<index; i++) {
			data[(v.size()-1)*1000+i]=(v[v.size()-1])->data[i];
		}
		for (int i=0; i<(int)v.size(); i++) {
			delete v[i];
		}
		return status;
	}
	
	
	void Bytevector::freadeof(int file){
		vector<Bytevector*>	v;
		Bytevector *b;
		int index=0;
		int status;
		b=new Bytevector;
		b->resize(1000);
		while( 0<(status=read(file,&(b->data[index++]),1))){
			if (index==1000) {
				v.push_back(b);
				b=new Bytevector;	
				b->resize(1000);
				index=0;
			}
		}
		index--;
		v.push_back(b);
		resize((v.size()-1)*1000+index);
		for (int i=0 ; i<(int)(v.size()-1); i++) {
			for (int j=0; j<1000; j++) {
				data[i*1000+j]=(v[i])->data[j];
			}
		}
		for (int i=0; i<index; i++) {
			data[(v.size()-1)*1000+i]=(v[v.size()-1])->data[i];
		}
		for (int i=0; i<(int)v.size(); i++) {
			delete v[i];
		}
	}
	
	
	int Bytevector::toInt(){
	 	bool negative=false;
	 	int result=0;
	 	int index=0;
	 	Bytevector b(*this);
	 	b.replace(Bytevector(" "),Bytevector());
	 	if (b.size>0) {
	 		switch(b[0]) {
	 			case '+':
	 				index++;
	 				break;
	 			case '-':
	 				negative=true;
	 				index++;
					break;
	 		}
	 		while ( index<size ) {
	 			if ( isdigit( data[index] ) ) {
	 				result*=10;
	 				result+=( data[index]-'0');
	 			}
	 			index++;
	 		}
	 	}
	 	return negative?-result:result;
	}
	
	bool Bytevector::toBool(){
		if (toInt()==0) {
		 	return false;
		} else {
			return true;
		}
	}

	char* Bytevector::operator*() {
		return data;
	}

	char& Bytevector::operator[](int i) {
		return data[i];
	}
	
	
	Bytevector::Bytevector(const Bytevector &b){
		data=NULL;
		create(0);
		operator=(b);	
	}	
	
	void Bytevector::setValue(const char *c){
		setValue(Bytevector(c));
	}	
	
	//Little strange:
	void Bytevector::setValue(int n){
		Bytevector newValue;
		newValue=n;
		setValue(newValue);		
	}
	
	void Bytevector::setValue(bool b) {
		if (b) {
			setValue(1);
		} else {
			setValue(0);
		}
	}	
	
	void Bytevector::setValue(Bytevector &value){
		int pos;
		if ((pos=find('=',0))!=-1) {
			resize(pos+value.size+1);
			override(value,pos+1);
		}
	}
	
	void Bytevector::setValue(const Bytevector &value){
		int pos;
		if ((pos=find('=',0))!=-1) {
			resize(pos+value.size+1);
			override(value,pos+1);
		}
	}

	
	void Bytevector::override(Bytevector &source, int overridepos){
	 	for (int i=0; i<source.size; i++) {
	 		data[overridepos+i]=source.data[i];
	 	}
	}
	
	void Bytevector::override(const Bytevector &source, int overridepos){
	 	for (int i=0; i<source.size; i++) {
	 		data[overridepos+i]=source.data[i];
	 	}
	}

	
	
	Bytevector& Bytevector::operator=( char* pchar){
	 	copyFromString(pchar);
	 	return *this;
	}
	
	Bytevector& Bytevector::operator=( const char* pchar){
	 	copyFromString(pchar);
	 	return *this;
	}

	
	Bytevector& Bytevector::operator=(Bytevector &b){
		create(0);
		copyFromBytevector(b,0,b.size);
		return *this;
	}
	

	Bytevector& Bytevector::operator=(const Bytevector &b){
		create(0);
		copyFromBytevector(b,0,b.size);
		return *this;
	}
	
	Bytevector& Bytevector::operator=(int n){
		char c[50]; // is 50 Enough???
		int pos=0;
		int value=(n>=0)?n:-n;
		Bytevector newValue;
		do {
			c[pos]='0'+value%10;
			pos++;
		} while ((value/=10)>0 );
		if (n<0) c[pos]='-'; else pos--;
		newValue.resize(pos+1);
		while (pos>=0) {
			newValue.data[newValue.size-1-pos]=c[pos];
			pos--;
		}
		*this=newValue;
		return *this;
	}
	
		
	Bytevector Bytevector::operator+(Bytevector b){
		Bytevector *c;
		c=new Bytevector;
		*c=*this;
		c->append(b);
		return *c;
	}
	
	Bytevector& Bytevector::operator+=(Bytevector &b){
  	    append(b);
		return *this;
	}
	
	Bytevector& Bytevector::operator+=(const Bytevector &b){
  	    append(Bytevector(b));
		return *this;
	}
	Bytevector& Bytevector::operator+=(const char *c){
  	    append(Bytevector(c));
		return *this;
	}
	
	Bytevector& Bytevector::operator+=(char *c){
  	    append(Bytevector(c));
		return *this;
	}

	Bytevector::Bytevector(int n){
		data=NULL;
		size=0;
	    *this=n;
	}
	
	Bytevector::Bytevector(const char* pchar){
		data=NULL;
		create(0);
		copyFromString(pchar);
	}
	
	Bytevector::Bytevector(char* pchar){
		data=NULL;
		create(0);
		copyFromString(pchar);
	}
	
	
	Bytevector::~Bytevector(){
		if (data) free(data);
	}
	
	void Bytevector::copyFromString(char* pchar){
		int i=0;
		create(strlen(pchar));
		while(i<size) {
			data[i]=pchar[i];
			i++;
		}
	}         	
	
	void Bytevector::copyFromString(const char* pchar){
		int i=0;
		create(strlen(pchar));
		while(i<size) {
			data[i]=pchar[i];
			i++;
		}
	}         	
	
	char* Bytevector::clonePCHAR(){
		char * result;
		int i=0;
		result = (char*) malloc (sizeof(char)*size+1);
		result[size]=0;
		while(i<size) {
			result[i]=data[i];
			i++;
		}
		return result;
	}

	
	void Bytevector::create(int newsize){
		if (data) free(data);
		size=newsize;
		data=(char*)malloc(sizeof(char)*size+1);
		data[size]=0;
	}
	
	void Bytevector::resize(int newsize){
		char *newdata;
		if (size!=0) {
			newdata=(char*)malloc(sizeof(char)*newsize+1);
			for (int pos=0; pos<newsize; pos++){
				if (pos<size) {
					newdata[pos]=data[pos];
				} else {
					newdata[pos]=0;
				}
			}
			newdata[newsize]=0;
			free(data);
			size=newsize;
			data=newdata;
		} else {
			create(newsize);
		}
	}
	
	
	
	void Bytevector::copyFromBytevector(Bytevector &source, int start, int len){
		create(len);
		for (int i=0;  i<len; i++){
			data[i]=source.data[start+i];
		}
	}
	
	void Bytevector::copyFromBytevector(const Bytevector &source, int start, int len){
		create(len);
		for (int i=0;  i<len; i++){
			data[i]=source.data[start+i];
		}
	}
	
	
	int Bytevector::find(Bytevector &b, int startpos){
		for (int i=startpos; i<(size-b.size+1); i++) {
			if (data[i]==b.data[0]) {
				bool eq=true;
				for (int j=0; j<b.size; j++) {
			        	eq = eq && (b.data[j]==data[i+j]);
			        	if (!eq) j=b.size;
				}
				if (eq) {
					return i;
				}
			}
		}
		return -1;
	}
	
	VectorIndexList Bytevector::findall(Bytevector b){
		int startpos=0,fpos=0;
		VectorIndexList result;
		while ( (fpos=find(b,startpos)) !=-1 ) {
			result.push_back(fpos);
			startpos=fpos+b.size;	
		}
		return VectorIndexList(result);
	}
	
	int Bytevector::find(char c, int startpos){
		for (int i=startpos; i<size; i++) {
			if (data[i]==c) {
				return i;
			}
		}
		return -1;
	}
	
	Bytevector* Bytevector::clone(){
		Bytevector *b;
		b=new Bytevector(*this);
		return b;	
	}
	
	void Bytevector::replace(Bytevector from, Bytevector to){
		if (!size) return;
		int matches=0;
		int pos,startpos=0;
		//calculate new size
		while ( (pos=find(from,startpos)) != -1 ) {
			matches++;
			startpos=pos+from.size;
		}
		int newsize=size+matches*(to.size-from.size);
		char *newdata=(char*) malloc(newsize*sizeof(char)+1);
		//save new data;
		startpos=0;
		int ubound;
		int newindex=0;
		do {
			//find next item
			pos=find(from,startpos);
			//set upper bound for copying from old stream
			if (pos==-1) {
				ubound=size;
			} else {
				ubound=pos;
			}
			//copying from old stream until eof or pos
			for (int i=startpos; i<ubound; i++) {
				newdata[newindex]=data[i];
				newindex++;
			}
			//if item is found then insert new item "to"
			if (pos!=-1) {
				//cout<<"Insert to.data into target stream on pos "<<newindex<<"\n";
				for (int i=0; i<to.size; i++) {
					newdata[newindex]=to.data[i];
					newindex++;
				}			
			}
			//Set new starting posistion
			startpos=pos+from.size;
		} while (pos!=-1); //Stop if nothing is found
		free(data);
		newdata[newsize]=0;
		data=newdata;
		size=newsize;
	}

			
	void Bytevector::append(Bytevector &source){
		int oldsize=size;
		resize(size+source.size);
		for (int i=0; i<source.size; i++){
			data[oldsize+i]=source.data[i];
		}
	}
	
	void Bytevector::append(const Bytevector &source){
		int oldsize=size;
		resize(size+source.size);
		for (int i=0; i<source.size; i++){
			data[oldsize+i]=source.data[i];
		}
	}

	
	Bytevector Bytevector::copy(int start, int len){
		Bytevector b;
		b.copyFromBytevector(*this,start,len);
		return Bytevector(b.data);
	}
	
	Bytevector Bytevector::copyRange(int indexA, int indexB){
		Bytevector b;
		if (indexA!=indexB) {
			int start=indexA<indexB?indexA:indexB;
			int end=indexA<indexB?indexB:indexA;
			int len=end-start+1;
			b.copyFromBytevector(*this,start,len);
		} else {
			b.resize(1);
			b.data[0]=data[indexA];
		}
		return Bytevector(b);
	}
		
	Bytevector Bytevector::id(){
		Bytevector b;
		Bytevector split;
		split=BYTEVECTOR_CFGSEPERATOR;
		int pos=find(split,0);
		if (pos>0) {
			b.copyFromBytevector(*this,0,pos);
		}
		return Bytevector(b);
	}
	
	Bytevector Bytevector::value(){
		Bytevector b;
		Bytevector split;
		split=BYTEVECTOR_CFGSEPERATOR;
		int pos=find(split,0);
		if (pos>0) {
			b.copyFromBytevector(*this,pos+1,size-pos-1);
		}
		return Bytevector(b);
	}
	
	
	bool Bytevector::operator==(Bytevector &b){
		bool result=size==b.size;
		if (!result) return result;
		result=true;
		for (int i=0; i<size; i++) {
			result=result && ( data[i]==b.data[i] );
		}
		return result;
	}
	bool Bytevector::operator!=(Bytevector &b){
		return !operator==(b);
	}

/*
    CCC - CD COVER CREATOR
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
#include "stringlist.h"
#include "ccchelp.h"
#include "texmaker.h"


/*String Ressources:*/

#include "cccpara.h"
Stringlist parameter(cccparaC,cccparaV);

#include "cccpara2.h"
Stringlist standalone_parameter(cccpara2C,cccpara2V);



/* Global Variables */
Stringlist arg;
Bytevector temp;
Cover cover; //Cover is defined in texmaker.h
Bytevector outputfile;
Bytevector errormsg("cdcover: ");



/* Error Messages */

void error_exit(){
	errormsg.fwriteln(fileno(stderr));
	exit(1);
}

void print_help() {
	errormsg=Stringlist(ccchelpC,ccchelpV).toBytevector();
	error_exit();
}

void missing_value_error(Bytevector para){
	errormsg+=
		Bytevector("No value for last parameter ")+
		para;
	error_exit();
}

void unknown_parameter_error(Bytevector para){
	errormsg+=
		Bytevector("Unknown parameter: ")+
		para;
	error_exit();
}

void count_error(Bytevector cnt){
	errormsg+=
		Bytevector("wrong value for-c or --count: ")+
		cnt;		
	error_exit();
}

void fopen_error(){
	errormsg+=
		Bytevector("can't open outputfile ")+
		outputfile;		
	error_exit();
}



/* Parsing the program arguments */

void parse_arguments(){
	int pnr;
	int i=1;
	while ((unsigned int)i<arg.size()){
		pnr=parameter.find(arg[i],0);
		if ( ((unsigned int)i==(arg.size()-1)) && 
			(pnr!=-1) && 
			(!standalone_parameter.includes(arg[i]))
		   ) missing_value_error(arg[i]);	
		switch (pnr) {
			/*-h, --help*/
			case 0: 
			case 1:
				print_help();
				break;
			/*-t, --title*/
			case 2: 
			case 3:
				cover.title=arg[++i];
				break;
			/*-s, --subtitle*/
			case 4: 
			case 5:
				cover.subtitle=arg[++i];
				break;
			/*-b, --backcover*/
			case 6: 
			case 7:
				cover.backcover=arg[++i];
				break;
			/*-c, --count*/
			case 8:
			case 9:
				cover.count=arg[++i].toInt();
				if (cover.count<1) count_error(arg[i]);
				break;
			/*-o, --outputfile*/
			case 10:
			case 11:
				outputfile=arg[++i];
				break;			
			/*-l, --sidebartext-left*/
			case 12:
			case 13:
				cover.sidetext_left=arg[++i];
				break;
			/*-r, --sidebartext-right*/
			case 14:
			case 15:
				cover.sidetext_right=arg[++i];
				break;
			default:
				unknown_parameter_error(arg[i]);
				break;		
		}
		i++;
	}
}


int main(int argc, char * argv[]){
  	Stringlist temp(argc,argv);
	arg=temp; // arg=Stringlist(argc,argv) didn't work under solaris
	if (arg.size()==1) print_help();
	parse_arguments();
	if (outputfile.size!=0) {
		FILE* f=fopen(outputfile.data, "w+b");
		if (!f){
			fopen_error();
		}
		cover.texcover().fwrite(fileno(f));
		fclose(f);
	} else {
		cover.texcover().fwrite(fileno(stdout));
	}
	return 0;
}

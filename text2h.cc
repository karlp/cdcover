/*
    text2h - Converting textfiles to C/C++ includefiles 
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

Bytevector head("#ifndef %VAR%_H\n#define %VAR%_H\n\n");
Bytevector pcharstart("char * %VAR%V[]=\n{\n");
Bytevector pcharend("};\n");
Bytevector linecount("\nint %VAR%C=%COUNT%;\n\n");
Bytevector tail("\n#endif\n");

int main(int argc, char* argv[]){
	Bytevector varname;
	Bytevector input;
	int count=0;
	if (argc<=1) {
		fprintf(stderr,"text2h, converts any text to a c-headerfile with a char* vector[]\n");
		fprintf(stderr,"example:  cat <inputfile> | text2h <VARNAME>  >  <outputfile>\n");
		exit(1);
	}
	varname=argv[1];	
	head.replace("%VAR%",varname);
	tail.replace("%VAR%",varname);
	pcharstart.replace("%VAR%",varname);
	pcharend.replace("%VAR%",varname);
	linecount.replace("%VAR%",varname);
	
	Bytevector("/* Created by text2h , (c) by Ulli Meybohm, www.meybohm.de   */").fwriteln(1);
	head.fwrite(1);
	pcharstart.fwrite(1);
	
	while( input.freadln(0) ) {
		count++;
		Bytevector("{\"").fwrite(1);
		input.replace("\\","\\\\");
		input.replace("\"","\\\"");
		input.fwrite(1);
		Bytevector("\"},").fwriteln(1);
	}	
	Bytevector("{\"\"}\n").fwrite(1);
	pcharend.fwrite(1);
	linecount.replace("%COUNT%",Bytevector(count));
	linecount.fwrite(1);
	tail.fwrite(1);
	return 0;
}

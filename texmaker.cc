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

#include "texmaker.h"

#include "reshead.h"
#include "resfront.h"
#include "resback.h"

/*Cover Class-Constructors */

Cover::Cover(){
	count=1;
}


Cover::Cover(const Cover &c){
	title=c.title;
	subtitle=c.subtitle;
	backcover=c.backcover;
	count=c.count;
	sidetext_left=c.sidetext_left;
	sidetext_right=c.sidetext_right;
}


/*Replace special charachters with tex-entities*/

Bytevector Cover::replaceVar(Bytevector b, int i){
	Bytevector tab(1);
	tab.data[0]=9;
	
	b.replace("%n",count);
	b.replace("%i",i);

	/*
	Fucking Backslash-Sequenzes
	*/
	b.replace("$","%%$EURO_RULEZ$%%");
	b.replace("\\","$\\backslash$");
	b.replace("%%$EURO_RULEZ$%%","\\$");
	
	b.replace("#","\\#");
	b.replace("%","\\%");
	b.replace("&","\\&");
	b.replace("~","\\~");
	b.replace("_","\\_");
	b.replace("{","\\{");
	b.replace("}","\\}");
	b.replace("[","{[}");
	b.replace("]","{]}");
	b.replace("*","{*}");
	b.replace(" ","~");
	b.replace(tab,"~~~~~~~~");
	b.replace("^",""); // made some trouble...

#ifndef I_AM_A_SUCKER
	/* 
	notice: 
	Microsoft is a trademark of 
	Microsoft Cooperation, USA
	
	To Microsoft:
	Be lucky that this is an open
	source project :-)
	*/
	b.replace("Microsoft","Microsoft SUCKS");
	b.replace("MICROSOFT","MICROSOFT SUCKS");
	/* 
		Revenge for kerberos!
		It's Judgement Day!
		Isn't it fun to be a hacker?

		SPASS MUSS SEIN  (german wise saying)
	*/
#endif
	return b;
}



Bytevector Cover::getCover(int i){
	Bytevector t=replaceVar(title,i);
	Bytevector s=replaceVar(subtitle,i);	
	Bytevector sl=replaceVar(sidetext_left,i);	
	Bytevector sr=replaceVar(sidetext_right,i);
	Bytevector b=replaceVar(backcover,i);
	Bytevector front=resfront;
	Bytevector back=resback;
	Bytevector nr(i);
	/* need alphas as tex-identifiers */
	for (int x=0; x< nr.size; x++)
		nr.data[x]='a'+(nr.data[x]-'0');
	back.replace("%i",nr);
	front.replace("%TITLE%",t);
	front.replace("%SUBTITLE%",s);
	
	if (sidetext_left.size>0) {
		back.replace("%SIDETEXTLEFT%",sl);
	} else {	
		back.replace("%SIDETEXTLEFT%",t);
	}
	if (sidetext_right.size>0) {
		back.replace("%SIDETEXTRIGHT%",sr);
	} else {	
		back.replace("%SIDETEXTRIGHT%",s);
	}
	b.replace("\n","\\\\");
	back.replace("%BACKCOVERTEXT%",b);
	return Bytevector ( front+back );
}


/*
public function, which assembles and returns
the latex code of the cd-cover
*/ 

Bytevector Cover::texcover()
{
	resback=Stringlist(resbackC,resbackV).toBytevector();
	resfront=Stringlist(resfrontC,resfrontV).toBytevector();
	Bytevector cover(Stringlist(resheadC,resheadV).toBytevector());
	for (int i=1; i<=count; i++) {
		cover+=getCover(i);			
	}	
	cover+=Bytevector("\n\\end{document}\n");
	return cover;
}




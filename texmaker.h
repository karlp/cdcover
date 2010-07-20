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

#ifndef texmaker_H
#define texmaker_H

#include "bytevector.h"
#include "stringlist.h"


/* Class, which produces the latex-output */

class Cover{
private:
	Bytevector getCover(int i);
	Bytevector replaceVar(Bytevector b, int i);
	Bytevector resfront, resback;
public:
	Cover();
	Cover(const Cover &c);

	Bytevector texcover();

	Bytevector title;
	Bytevector subtitle;
	Bytevector backcover;
	Bytevector sidetext_left;
	Bytevector sidetext_right;
	int count;
};



#endif

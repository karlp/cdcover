/* Created by text2h , (c) by Ulli Meybohm, www.meybohm.de   */
#ifndef ccchelp_H
#define ccchelp_H

char * ccchelpV[]=
{
"CD Cover Creator  Version 0.9.1 (stable)                       ",
"Creating LaTeX cd-cover files",
"Copyright (C) 2000, Ulli Meybohm, www.meybohm.de (GNU General Public License)",
"",
"Start: cdcover [options]",
"",
"Options:     ",
"-t, --title <TITLE>                  Set Frontcover Title",
"-s, --subtitle <SUBTITLE>            Set Frontcover Subtitle",
"-b, --backcovertext <BACKCOVERTEXT>  Set Backcovertext       (can be multiline)",
"-l, --sidetext-left <LEFTSIDETEXT>   Set left sidetext        (default=<TITLE>) ",
"-r, --sidetext-right <RIGHTSIDETEXT> Set right   \"         (default=<SUBTITLE>) ",
"-c, --count <COUNT>                  Set Number of CDs              (default=1)",
"-o, --outputfile <FILENAME>          Set Outputfile (*.tex)    (default=stdout)",
"",
"You can use the variables %i and %n in the string-parameters if you make covers",
"for cd-sets with more than one disk. (see example below, 6 CDs) ",
"",
"Examples:",
"cdcover -t \"Debian GNU/Linux 3.1\" -s \"Disc %i/%n\" -b \"Debian GNU/Linux\" -c 10 -o sarge.tex",
"cdcover -t \"Backup\" -s \"Date: 04-2000\" -b \"`ls -1 /backup/*.tgz`\" -o backup.tex",
""
};

int ccchelpC=21;


#endif

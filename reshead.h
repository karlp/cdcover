/* Created by text2h , (c) by Ulli Meybohm, www.meybohm.de   */
#ifndef reshead_H
#define reshead_H

char * resheadV[]=
{
"\\documentclass[]{article}",
"\\usepackage{ifthen,isolatin1,graphicx,color}",
"\\graphicspath{{/tmp/disc-cover-8568/}}",
"\\renewcommand{\\thepage}{}",
"\\setlength{\\oddsidemargin}{0pt}",
"\\setlength{\\evensidemargin}{0pt}",
"\\setlength{\\voffset}{-100pt}",
"\\setlength{\\footskip}{0pt}",
"\\setlength{\\textheight}{780pt}",
"\\hyphenpenalty=10000",
"\\renewcommand{\\dotfill}{\\leaders\\hbox to 2mm{\\hfil.\\hfil}\\hfill}",
"",
"\\newcommand{\\mylabel}{\\color{black}}",
"",
"",
"\\begin{document}",
"",
"\\noindent",
"\\parbox{11cm}",
"{",
"{\\small Printed with \\emph{cd cover creator} (C) (GPL) by Ulli Meybohm, www.meybohm.de",
"The program uses the tex-template from Disc-Cover.",
"Disc-Cover is available at  \\texttt{http://www.liacs.nl/\\~{}jvhemert/disc-cover}}",
"}",
"\\vspace{0.5cm}",
""
};

int resheadC=25;


#endif

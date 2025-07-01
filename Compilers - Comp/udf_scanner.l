%option c++ prefix="udf_scanner_" outfile="udf_scanner.cpp"
%option stack noyywrap yylineno 8bit debug
%{ 
// make relevant includes before including the parser's tab file
#include <string>
#include <sstream>
#include <cdk/ast/sequence_node.h>
#include <cdk/ast/expression_node.h>
#include <cdk/ast/lvalue_node.h>
#include "udf_parser.tab.h"

// output stream for building string literals
std::ostringstream strlit;

// don't change this
#define yyerror LexerError
%}

SPACE       ([ \t\r]|\n)
INT         [[:digit:]]+

LETTER      [A-Za-z]
IDENTIFIER  {LETTER}({LETTER}|[[:digit:]]|[_])*

%x X_COMMENT
%x X_STRING
%x X_HEX_INT
%x X_BACKSLASH X_IGNORE

%%
  yydebug=1; set_debug(1);

  /* --------------------------------- Comments ---------------------------------- */

"//".*$                /* match the whole line (it is ignored) */

"/*"                   yy_push_state(X_COMMENT);
<X_COMMENT>"*/"        yy_pop_state();
<X_COMMENT>"/*"        yy_push_state(X_COMMENT);
<X_COMMENT>.|"\n"      ;  /* ignore inside comments */

  /* ------------------------------ Tipos de dados ------------------------------- */

"auto"                 return tTYPE_AUTO;
"int"                  return tTYPE_INT;
"real"                 return tTYPE_REAL;
"string"               return tTYPE_STRING;
"ptr"                  return tTYPE_POINTER;
"void"                 return tTYPE_VOID;
"tensor"               return tTYPE_TENSOR;

"input"                 return tINPUT;
"nullptr"               return tNULLPTR;
"objects"               return tOBJECTS;
"sizeof"                return tSIZEOF;

  /* ----------------------------- Símbolos globais ------------------------------ */

"forward"               return tFORWARD;
"public"                return tPUBLIC;

  /* -------------------------------- Instruções --------------------------------- */

"if"                   return tIF;
"elif"                 return tELIF;
"else"                 return tELSE; 
"write"                return tWRITE;
"writeln"              return tWRITELN;
"for"                  return tFOR; 
"break"                return tBREAK;
"continue"             return tCONTINUE;
"return"               return tRETURN;

  /* -------------------------- Operadores e expreções --------------------------- */

">="                   return tGE; 
"<="                   return tLE; 
"=="                   return tEQ; 
"!="                   return tNE;
"&&"                   return tAND;
"||"                   return tOR;
"**"                   return tCONTRACT;

[-+*/%=(){}[\]<>,;.~]   return *yytext;

  /* --------------------------------- Inteiros ---------------------------------- */

"0x"                      yy_push_state(X_HEX_INT);

<X_HEX_INT>[[:xdigit:]]+  { yylval.i = strtoul(yytext, NULL, 16); yy_pop_state(); return tINTEGER; }
<X_HEX_INT>.|\n           std::cerr << "WARNING: line " << lineno() << ": bad hexadecimal data!" << std::endl; exit(1);

{INT}                     { yylval.i = strtoul(yytext, nullptr, 10); return tINTEGER;
                            if(errno == ERANGE) yyerror("Cannot represent the literal integer (Overflow)"); 
                                  else return tINTEGER; }

  /* ---------------------------------- Reais ----------------------------------- */

([0-9]*\.[0-9]+|[0-9]+\.[0-9]*)([eE][-+]?[0-9]+)? { yylval.d = strtod(yytext, NULL); return tREAL;
                                                   if(errno == ERANGE) yyerror("Cannot represent the literal integer (Overflow)"); 
                                                   else return tREAL; }

  /* ----------------------------- Identificadores ------------------------------ */

{IDENTIFIER}           yylval.s = new std::string(yytext); return tID;

  /* --------------------------- Cadeia de Caracteres ---------------------------- */

"\""                    yy_push_state(X_STRING);
<X_STRING>\\            yy_push_state(X_BACKSLASH);
<X_STRING>"\""          {
                          yylval.s = new std::string(strlit.str());
                          strlit.str("");
                          yy_pop_state();
                          return tSTRING;
                        }
<X_STRING>\n            yyerror("newline in string");
<X_STRING>.             strlit << yytext;

<X_BACKSLASH>n          strlit << '\n'; yy_pop_state();
<X_BACKSLASH>r          strlit << '\r'; yy_pop_state();
<X_BACKSLASH>t          strlit << '\t'; yy_pop_state();
<X_BACKSLASH>\\         strlit << '\\'; yy_pop_state();

<X_BACKSLASH>0          yy_pop_state(); yy_pop_state(); yy_push_state(X_IGNORE);

<X_BACKSLASH>[[:xdigit:]]{1,2} {
                                 strlit << (char)(unsigned char)strtoul(yytext, NULL, 16);
                                 yy_pop_state();
                               }
<X_BACKSLASH>.                 strlit << *yytext; yy_pop_state();

<X_IGNORE>.|\n         ; /* ignore the rest of the string */

  /* ---------------------------- Outros Caracteres ----------------------------- */

{SPACE}               ; /* ignore whitespace */

.                      yyerror("Unknown character");

  /* ----------------------------------- Fim ------------------------------------ */

%%

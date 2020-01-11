/*	$NetBSD: empty.y,v 1.1.1.1 2015/01/03 22:58:23 christos Exp $	*/

%{
#ifdef YYBISON
#define YYLEX_DECL() yylex(void)
#define YYERROR_DECL() yyerror(const char *s)
static int YYLEX_DECL();
static void YYERROR_DECL();
#endif
%}
%%
start: ;

%%

#include <stdio.h>

static int
YYLEX_DECL() {
  return -1;
}

static void
YYERROR_DECL() {
  printf("%s\n",s);
}

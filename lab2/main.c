#include "y.tab.h"
#include "stdio.h"

int main() {
    fprintf(stdout, "print xPath query:\n");
    yyparse();
    return 0;
}

#include <math.h>
#include <stdio.h>
#include "tinyexpr.h"


int main() {
    double x, y;
    te_variable vars[] = {{"x", &x}};
    int err;
    te_expr *expr = te_compile("sin(x)", vars, 1, &err);
    if (!expr) {
        printf("Errore alla posizione %d\n", err);
        return 1;
    }

    for (x = -3.14; x <= 3.14; x += 0.1) {
        y = te_eval(expr);
        printf("x = %lf, y = %lf\n", x, y);
    }

    te_free(expr);
}

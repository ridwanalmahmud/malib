#include "utils.h"
#include <stdio.h>

int main(void) {
    double res;

    printf("Exact mode (prefers exact results):\n");
    for (int i = 0; i <= 25; i++) {
        if (factorial(i, &res, FLAG_PREFER_EXACT)) {
            printf("%2d! = %.0f\n", i, res);
        } else {
            printf("%2d! = [Too large]\n", i);
        }
    }

    printf("\nApproximation mode:\n");
    double vals[] = {5.5, 10.3, 20.8, 100, 170.5};
    for (int i = 0; i < 5; i++) {
        if (factorial(vals[i], &res, 0)) {
            printf("%.1f! ≈ %e\n", vals[i], res);
        }
    }

    return 0;
}

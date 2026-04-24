#include "utils/math_utils.h"


int min(int a, int b) {
    if (a < b) {
        return a;
    }
    return b;
}
int max(int a, int b) {
    if (a < b) {
        return b;
    }
    return a;
}
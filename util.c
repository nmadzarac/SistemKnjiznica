#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "header1.h"

void clearInputBuffer() {
    while (getchar() != '\n');
}

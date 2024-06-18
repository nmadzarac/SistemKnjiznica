#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "header1.h"
#include "header.h"

void clearInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

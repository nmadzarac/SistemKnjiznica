#pragma once
#ifndef HEADER1_H
#define HEADER1_H

int prijava();
int izbornik(int admin);
void clearInputBuffer();

int compareByTitle(const void* a, const void* b);
int compareById(const void* a, const void* b);
int compareByAuthor(const void* a, const void* b);
int compareByYear(const void* a, const void* b);
int compareByGenre(const void* a, const void* b);

#endif // HEADER1_H


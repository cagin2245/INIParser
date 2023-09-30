#pragma once


#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void ini_parseINIFromFile(const char* filePath);
void ini_parseINI(const char* iniData);
void ini_appendBuffer(char* buffer, char c);
void ini_stripeBuffer(char* buffer);
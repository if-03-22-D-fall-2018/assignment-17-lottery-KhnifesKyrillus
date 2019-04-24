/*-----------------------------------------------------------------------------
 *				HTBLA-Leonding / Class: <your class name here>
 *-----------------------------------------------------------------------------
 * Exercise Number: 17
 * File:			lottery.c
 * Author(s):		David Kraus
 * Due Date:		idk lol
 *-----------------------------------------------------------------------------
 * Description:
 * Implementation of a library for analyzing lottery tips.
 *-----------------------------------------------------------------------------
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "lottery.h"

#define 	UUID_LEN       36
#define 	MAX_TIP_LEN    17

#define 	MAX_LINE_LEN   (UUID_LEN + 1 + MAX_TIP_LEN)

bool contains_tip(int* tip, int tipDigit);
int get_number_of_lines(FILE* fd);

FILE* fd;
char separator;
int last_drawing[TIP_SIZE];

bool init_lottery(const char *csv_file, char csv_separator)
{
    fd = fopen(csv_file, "r");
    separator = csv_separator;
    for (int i = 0; i < TIP_SIZE; i++)
        last_drawing[i] = 0;
    return fd != 0;
}

bool get_tip(int tip_number, int tip[TIP_SIZE])
{
    if (tip_number < 0 || tip_number >= 1000000)
        return false;

    fseek(fd, 0, SEEK_SET);
    int currentLine = 0;
    while (currentLine < tip_number)
    {
        while (!feof(fd) && fgetc(fd) == '\n') 
        {
            currentLine++;
            break;
        }
    }

    fseek(fd, (UUID_LEN + 1) * sizeof(char), SEEK_CUR);
  
    char tipString[MAX_TIP_LEN];
    fgets(tipString, MAX_TIP_LEN, fd);

    char delimiter[2] = { separator, '\0' };
    char *ptr = strtok(tipString, delimiter);

    int pos = 0;
    while(ptr != NULL) 
    {
        int tipNumber = atoi(ptr);
        tip[pos] = tipNumber;
        ptr = strtok(NULL, delimiter);
        pos++;
    }
    return true;
}

bool set_drawing(int drawing_numbers[TIP_SIZE])
{
    for (int i = 0; i < TIP_SIZE; i++)
    {
        if (drawing_numbers[i] <= 0 || drawing_numbers[i] > 45)
            return false;
    }

    for (int i = 0; i < TIP_SIZE; i++)
    {
        last_drawing[i] = drawing_numbers[i];
    }
    return true;
}

bool is_invalid_tip(int tip_number)
{
    int previousPos = ftell(fd);
    fseek(fd, 0, SEEK_END);
    int endPos = ftell(fd);
    fseek(fd, previousPos, SEEK_SET);
    return tip_number * MAX_LINE_LEN > endPos;
}

bool is_drawing_set()
{
    for (int i = 0; i < TIP_SIZE; i++)
    {
        if (last_drawing[i] == 0)
            return false;
    }
    return true;
}

int get_tip_result(int tip_number) {
    if (tip_number < 0 || tip_number >= get_number_of_lines(fd))
        return -2;

    if (!is_drawing_set())
        return -1;

    int csv_tip[TIP_SIZE];
    get_tip(tip_number, csv_tip);

    int correctDigits = 0;
    for (int i = 0; i < TIP_SIZE; i++)
    {
        if (contains_tip(last_drawing, csv_tip[i]))
            correctDigits++;
    }
    return correctDigits;
}

int get_number_of_lines(FILE* fd)
{
    int previousPos = ftell(fd);
    fseek(fd, 0, SEEK_SET);
    int lines = 1;
    while (!feof(fd))
    {
        if (fgetc(fd) == '\n') lines++;
    }
    fseek(fd, previousPos, SEEK_SET);
    return lines;
}

int get_right_tips_count(int right_digits_count)
{
    if (!is_drawing_set() || right_digits_count < 0 || right_digits_count > TIP_SIZE)
        return -1;

    int count = 0;
    int numOfLines = get_number_of_lines(fd);
    for (int i = 0; i < numOfLines; i++)
    {
        int result = get_tip_result(i);
        if (result == right_digits_count)
            count++;
    }
    return count;
}

bool contains_tip(int* tip, int tipDigit) {
    for (unsigned int i = 0; i < TIP_SIZE; i++)
    {
        if (tip[i] == tipDigit) return true;
    }
    return false;
}

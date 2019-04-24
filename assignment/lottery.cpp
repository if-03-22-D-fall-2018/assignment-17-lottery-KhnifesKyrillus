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
#include "lottery.h"
#include "general.h"
#include <string.h> // for strtok to split the string to substring with separator
#include <stdlib.h> // for atoi to work (converts string to int)
#include <stdio.h>

#define 	UUID_LEN   40
#define 	MAX_TIP_LEN   17
#define 	MAX_LINE_LEN   (UUID_LEN + 1 + MAX_TIP_LEN + 1)

static FILE* stream;
static char separator;
static int drawing_of_numbers[TIP_SIZE];

bool init_lottery(const char *csv_file, char csv_separator)
{
    
  stream = fopen(csv_file, "r");
  if (stream == 0) return false;
  separator = csv_separator;
    for (int i = 0; i < TIP_SIZE; ++i) {
        drawing_of_numbers[i] = 0;
    }
  return true;
}

bool is_empty(int drawing_numbers[])
{
    for (int i = 0; i < TIP_SIZE; ++i) {
        if (drawing_numbers[i] != 0) return false;
    }
    return true;
}

bool get_tip(int tip_number, int tip[TIP_SIZE])
{
    if (tip_number < 0 || tip_number >= 44) return false;
    int count = 0;
    char line[MAX_LINE_LEN];
    while ( fgets(line, MAX_LINE_LEN, stream) != NULL) // read a line
    {
        if (count == tip_number)
        {
            count = 0;
            char str[2];
            str[0] = separator;
            str[1] = '\0';
            char* ptr = strtok(line, str); // gets the first string before separator
            while(ptr != 0)
            {
                ptr = strtok(0, str); // gets next string
                if (ptr == 0) break; // if 0 we reached EOL
                if (atoi(ptr) > 0 && atoi(ptr) <= 45) // check if it is a valid tip number
                {
                    tip[count] = atoi(ptr); // converts the string to an int
                }
                count++;
            }
            rewind(stream); // sets current to 0 to start at the beginning next time
            return true;
        }
        count++;
    }
  return false;
}

bool set_drawing(int drawing_numbers[TIP_SIZE])
{
  for (int i = 0; i < TIP_SIZE; i++) {
    if (drawing_numbers[i] <= 0 || drawing_numbers[i] > 45) return false;
    else drawing_of_numbers[i] = drawing_numbers[i];
  }
  return true;
}

int get_tip_result(int tip_number)
{
  if (tip_number < 0 || tip_number >= 44) return -2;
  if (is_empty(drawing_of_numbers))return -1;
  int tip[TIP_SIZE];
  int count = 0;
  get_tip(tip_number,tip);
  for (int i = 0; i < TIP_SIZE; i++) {
    for (int j = 0; j < TIP_SIZE; j++) {
      if (drawing_of_numbers[i] == tip[j]) count++;
    }
  }
  return count;
}

int get_right_tips_count(int right_digits_count)
{
    if (right_digits_count < 0 || right_digits_count > TIP_SIZE || is_empty(drawing_of_numbers)) return -1;
    int count = 0;
    int temp = 0;
    for (int i = 0; i < 44; ++i) {
        temp = get_tip_result(i);
        if (temp == right_digits_count)
        {
            count++;
        }
    }
  return count;
}

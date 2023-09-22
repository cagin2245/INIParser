#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

void parseINI(const char* iniData);
void appendBuffer(char* buffer, char c); //everytime read a valid name, write the valid name to buffer to remember
void stripeBuffer(char* buffer);
int main()

{

	const char * iniContent = 
		"; last modified 1 April 2001 by John Doe\n"
		"[owner]\n"
		"name = John Doe\n"
		"organization = Acme Widgets Inc.\n"
		"\n"
		"[database]\n"
		"; use IP address in case network name resolution is not working\n"
		"server = 192.0.2.62\n"
		"port = 143\n"
		"file = \"payroll.dat\"\n";

	printf("%s", iniContent);
	printf("\n--------------------------\n\n");
	printf("Parsed\n\n\n");
	parseINI(iniContent);



}

void parseINI(const char * iniData)
{
	// working buffer
	char buffer[256]; //everytime read a valid name, write the valid name to buffer to remember
	*buffer = '\0';

	// current val
	char currentSec[256];
	*currentSec = '\0';
	char currentKey[256];
	char currentVal[256];



	int32_t state = 0;
	for (const char* c = iniData; * c; c++)
	{
		/*
		* 0 - Ready to read data
		* 1 - Comment line
		* 2 - Section
		* 3 - Key
		* 4 - End Key
		* 5 - Ready to get value
		* 6 - Value start	
		* 7 - Invalid data
		*/





		// Wait for data
		if (state == 0)
		{
			if (*c == ';') state = 1;
			else if (*c == '[') state = 2;
			else if (!(*c == ' ' || *c == '\t' || *c == '\n'))
			{
				appendBuffer(buffer, *c);
				state = 3;
			}// start-of-key
			
		}
		// Comment read
		else if (state == 1)
		{ 
			if (*c == '\n') state = 0; // Stop reading the comment line
		}
		// Section read
		else if (state == 2)
		{
			if (*c == ']') // Stop reading the section
			{
				memcpy(currentSec, buffer, 256 * sizeof(char));
				*buffer = '\0';
				state = 0;
			}
			else if (*c == '\n') // invalid section (skip)
			{
				*buffer = '\0';
				state = 0;
			}
			else
			{
				appendBuffer(buffer, *c);
			}
		}
		// Key read
		else if (state == 3)
		{
			if (*c == ' ' || *c == '\t')
			{
				memcpy(currentKey, buffer, 256 * sizeof(char));
				*buffer = '\0';
				state = 4; // end-of-key
			}
			else if (*c == '\n')
			{
				state = 0; // Invalid key-val pair
			}
			else
			{
				appendBuffer(buffer, *c);
			}
		}
		// End of key read
		else if (state == 4)
		{
			if (*c == '=') state = 5; //Ready for val
			else if (*c == '\n') state = 0; // Invalid key-val pair
			else if (!(*c == ' ' || *c == '\t')) state = 7; // Invalid key-val pair

		}
		// Ready for val
		else if (state == 5)
		{
			if (*c == '\n') state = 0; // Invalid key-val pair
			else if (!(*c == ' ' || *c == '\t'))
			{
				appendBuffer(buffer, *c);
				state = 6;
			} // Begin key
		}
		// Val Start
		else if (state == 6)
		{
			if (*c == '\n')
			{
				stripeBuffer(buffer);
				memcpy(currentVal, buffer, 256 * sizeof(char));
				*buffer = '\0';
				state = 0;
				// TODO: report output
				printf("Property: \%s/%s\": \"%s\"\n", currentSec, currentKey, currentVal);
			}
			else
			{
				appendBuffer(buffer, *c);
			}
		}
	}


}
void appendBuffer(char* buffer, char c)
{
	char* cursor;
	for (cursor = buffer; *cursor; cursor++);
	cursor[0] = c;
	cursor[1] = '\0';
}
void stripeBuffer(char* buffer)
{
	char* cursor;
	for (cursor = buffer; cursor[1] != '\0'; cursor++);
	while (*cursor == ' ' || *cursor == '\t')
	{
		*cursor = '\0';
		cursor--;
	}
}
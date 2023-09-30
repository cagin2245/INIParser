
#include "INIParser.h"

void ini_parseINIFromFile(const char* filePath)
{
	FILE* file = NULL; // FILE is opaque
	fopen_s(&file, filePath, "rb");
	if (file)
	{
		//Open file to read
		size_t fileSize = 0;
		fseek(file,0, SEEK_END );
		fileSize= ftell(file);
		rewind(file);

		
		char* fileContent = malloc(fileSize+1);
		if(fileContent)
		{ 
			if (fread(fileContent, fileSize, 1, file) == 1)
			{
				fileContent[fileSize] = '\0';
				ini_parseINI(fileContent);

			}
			free(fileContent);
		}
		fclose(file);
	}
}

void ini_parseINI(const char* iniData)
{
	// Open a log file

	FILE* log = NULL;
	fopen_s(&log, "./iniparser.log", "wb");


	// working buffer
	char buffer[256]; //everytime read a valid name, write the valid name to buffer to remember
	*buffer = '\0';

	// current val
	char currentSec[256];
	*currentSec = '\0';
	char currentKey[256];
	char currentVal[256];



	int32_t state = 0;
	for (const char* c = iniData; *c; c++)
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

		switch (state)
		{
			// Wait for data
		case 0:
			switch (*c)
			{
			case ';': state = 1; break;
			case '[': state = 2; break;
			case ' ':
			case '\t':
			case '\n':
				break;
			default:
								ini_appendBuffer(buffer, *c);
				state = 3;
				break;

			}
			break;

			// Comment read
		case 1:

			if (*c == '\n') state = 0; // Stop reading the comment line
			break;

			// Section read
		case 2:
			switch (*c)
			{
			case ']':
				strcpy_s(currentSec, 256, buffer);
				*buffer = '\0';
				state = 0;
				break;
			case '\n':
				*buffer = '\0';
				state = 0;
				break;
			default:
								ini_appendBuffer(buffer, *c);
				break;
			}

			break;

			// Key read
		case 3:
			switch (*c)
			{
			case ' ':
			case '\t':
				strcpy_s(currentKey, 256, buffer);
				*buffer = '\0';
				state = 4; // end-of-key
				break;
			case '\n':
				state = 0; // Invalid key-val pair
				break;
			default:
								ini_appendBuffer(buffer, *c);

				break;
			}

			break;

			// End of key read
		case 4:
			switch (*c)
			{
			case '=': // Ready for value
				state = 5;
				break;
			case '\n': // Invalid key value pair
				state = 0;
				break;
			case ' ':
			case '\t':
				break;
			default: // Invalid key value pair
				state = 7;
				break;
			}
			break;

			// Ready for value
		case 5:
			switch (*c)
			{
			case '\n': // Invalid key value pair
				state = 0;
				break;
			case ' ':
			case '\t':
				break;
			default: // Begin value
								ini_appendBuffer(buffer, *c);
				state = 6;
				break;
			}
			break;

			// Start of value
		case 6:
			switch (*c)
			{
			case '\n': // End of value
				ini_stripeBuffer(buffer);
				strcpy_s(currentVal, 256, buffer);
				*buffer = '\0';
				state = 0;

				// Report out
				printf("Propertie: \"%s/%s\": \"%s\"\n", currentSec, currentKey, currentVal);
				if (log)
				{
					
					fprintf_s(log, "Section = \%s\" Key = \"%s\" Value = \"%s\"\n",
						currentSec, currentKey, currentVal);
				}
				break;
			case '\t': // Replace tab with spaces
				strcat_s(buffer, 256, "    ");
				break;
			default:
								ini_appendBuffer(buffer, *c);
				break;
			}
			break;

			// Invalid value
		case 7:
			if (*c == '\n') state = 0;
			break;
		}



	}

	if (log)
	{
		fclose(log);
	}


}
void ini_appendBuffer(char* buffer, char c)
{
	if(!iscntrl(c))
	{
		char str[2] = { c, '\0' };

		strcat_s(buffer, 256, str); // size cames from buffer
	}

}
void ini_stripeBuffer(char* buffer)
{
	char* c = &buffer[strlen(buffer) - 1];
	if (*c == ' ' || *c == '\t')
	{
		*c = '\0';
		ini_stripeBuffer(buffer);
	}




	/*char* cursor;
	for (cursor = buffer; cursor[1] != '\0'; cursor++);
	while (*cursor == ' ' || *cursor == '\t')
	{
		*cursor = '\0';
		cursor--;
	}*/
}
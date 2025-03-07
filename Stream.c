//------------------------------------------------------------------------------
//
// File Name:	Stream.c
// Author(s):	Harrison Parrish (harrison.parrish)
// Project:		Project 0
// Course:		CS230S25
//
// Copyright © 2025 DigiPen (USA) Corporation.
//
//------------------------------------------------------------------------------

#include "stdafx.h"
#include "Stream.h"
#include "Vector2D.h"
#include "Trace.h"

//------------------------------------------------------------------------------
// Private Constants:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Structures:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Variables:
static FILE* file;
errno_t error;
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Private Variables:
//------------------------------------------------------------------------------
static char tokenBuffer[1024];
//------------------------------------------------------------------------------
// Private Function Declarations:
//------------------------------------------------------------------------------

//------------------------------------------------------------------------------
// Public Functions:
//------------------------------------------------------------------------------

// Open a file for reading.
// (NOTE: Use fopen_s() with the "rt" option.)
// (NOTE: You must generate the following trace message if the file was not opened successfully:
//	  TraceMessage("Error: StreamOpen could not open file %s; %s", filePath, errorMsg);
//	  Where errorMsg is a char array containing the error message obtained using strerror_s().)
// Params:
//	 filePath = The file to be opened for reading.
// Returns:
//	 NULL if filePath is NULL or the file could not be opened.
//	 Pointer to a FILE object if the file was opened successfully.

Stream StreamOpen(const char* filePath)
{
	fopen_s(&file, filePath, "rt");
	char errorMsg[100];

	strerror_s(errorMsg, 100, errno);

	if (file == NULL) {
		return NULL;
	}
	else {
		return file;
	}
}

// Read a single integer from a stream.
// (NOTE: Verify that the stream is valid first.)
// (NOTE: Use fscanf_s() to scan the input stream for an integer.)
// Params:
//	 stream = The file stream from which to read.
// Returns:
//	 0 if the stream was not opened succesfully;
//	 otherwise, an integer value read from the file.
int StreamReadInt(Stream stream) {
	if (stream == NULL) {
		return 0;
		}
	int number;
	int integer = fscanf_s(stream, "%i" , &number);

		if (integer != 1) {
			return 0;
	}
		return number;
}


// Read a single float from a stream.
// (NOTE: Verify that the stream is valid first.)
// (NOTE: Use fscanf_s() to scan the input stream for a float.)
// Params:
//	 stream = The file stream from which to read.
// Returns:
//	 If the stream was opened succesfully,
//	   then return a float value read from the file,
//	   else return 0.
float StreamReadFloat(Stream stream) {
	if (stream == NULL) {
		return 0;
	}
	float number = 0;
	int result = fscanf_s(stream, "%f", &number);

	if (result != 1) {
		return 0;
	}
	return number;
}

// Read the data for a Vector2D from a stream.
// (NOTE: Verify that the stream and vector pointer are valid first.)
// (HINT: Use StreamReadFloat() to read the x and y values, in sequence.)
// Params:
//	 stream = The file stream from which to read.
// Returns:
//	 If the stream and vector pointer are both valid,
//	   then fill the vector with two float values (x & y),
//	   else do nothing (optionally, write an error message to the trace log).
void StreamReadVector2D(Stream stream, Vector2D* vector) {
	if (stream == NULL || vector == NULL) {
		return;
	}
	else {
		vector->x = StreamReadFloat(stream);
		vector->y = StreamReadFloat(stream);
	}
}

// Close an opened stream.
// (NOTE: Do not attempt to close the stream if the pointer is null.)
// (PRO TIP: Avoid dangling pointers by setting the FILE pointer to NULL.)
// Params:
//	 stream = The file stream to be closed.
void StreamClose(Stream* stream) {
	if (stream != NULL && *stream != NULL) {
		fclose(*stream);
	}
}
const char* StreamReadToken(Stream stream) {
	tokenBuffer[0] = '\0';
	if (stream) {
		if (fscanf_s(stream, "%s", tokenBuffer, (unsigned)_countof(tokenBuffer)) == 1) {
			return tokenBuffer;
		}

	}
	return tokenBuffer;
}

bool StreamReadBoolean(Stream stream) {
	int value = 0;

	if (stream) { 
		if (fscanf_s(stream, "%d", &value) != 1) {  
			value = 0;  
		}
	}

	return value != 0; 
}
void StreamReadColor(Stream stream, DGL_Color* color) {

	if (stream == NULL || color == NULL) {
	
		TraceMessage("StreamReadColor: Invalid stream or color pointer.");
		return;
	}


	color->r = StreamReadFloat(stream);
	color->g = StreamReadFloat(stream);
	color->b = StreamReadFloat(stream);
	color->a = StreamReadFloat(stream);
	if (color->r < 0.0f) color->r = 0.0f;
	if (color->g < 0.0f) color->g = 0.0f;
	if (color->b < 0.0f) color->b = 0.0f;
	if (color->a < 0.0f) color->a = 0.0f;
	if (color->r > 1.0f) color->r = 1.0f;
	if (color->g > 1.0f) color->g = 1.0f;
	if (color->b > 1.0f) color->b = 1.0f;
	if (color->a > 1.0f) color->a = 1.0f;
	
}
//------------------------------------------------------------------------------
// Private Functions:
//------------------------------------------------------------------------------


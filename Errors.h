//
// Created by Arman on 21.08.2023.
//

/// \file
/// \brief Contains errors that may occur during the program working.
/// \details Contains errors info and function to print these errors.

#ifndef ERRORS_H
#define ERRORS_H

//#define NDEBUG

#include <stdio.h>
#include <string.h>
#include <math.h>

#include "Colors.h"

//-----------------------------------------------------------------------------------------------

/// \brief Errors than may occur during the program working. 
enum class Errors 
{
    NO_ERR                      =  0,
    
    MEMORY_ALLOCATION_ERR       =  1,
    
    STACK_EMPTY_ERR             =  2,  
    STACK_IS_NULLPTR            =  3,
    STACK_CAPACITY_OUT_OF_RANGE =  4,
    STACK_SIZE_OUT_OF_RANGE     =  5,

};

//-----------------------------------------------------------------------------------------------

#ifndef NDEBUG
/// \brief Contains info about errors - File with error, line with error, error code. 
/// \warning Have to be updated with UPDATE_ERR() only
struct ErrorInfoType 
{
    Errors error;              ///< error code
    const char* fileWithError; ///< __FILE__ (file name with error)
    const char* funcWithError; ///< __func__ (function name with error)
    int lineWithError;         ///< __LINE__ (line with error)

};

#else

/// \brief Contains info about errors - File with error, line with error, error code. 
/// \warning Have to be updated with UPDATE_ERR() only
struct ErrorInfoType 
{
    Errors error;              ///< error code
};

#endif

/// \brief global errorInfo constant with error info
/// \warning this variable have to be changes only with UPDATE_ERR()
extern ErrorInfoType ErrorInfo;

//-----------------------------------------------------------------------------------------------

#ifndef NDEBUG

/// \brief updates special struct with errors errorInfo 
/// \details copyFileName copy of __FILE__ define at the moment macros is called 
/// \details copyLineNumber __LINE__ define at the moment macros is valled
/// \param [in]ERROR Errors enum with error occurred in program
#define UPDATE_ERR(ERROR)                                             \
do                                                                    \
{                                                                     \
    ErrorInfo.fileWithError = __FILE__;                               \
    ErrorInfo.lineWithError = __LINE__;                               \
    ErrorInfo.funcWithError = __func__;                               \
    ErrorInfo.error = ERROR;                                          \
} while(0)
    
#else

/// \brief updates only error code without debug info
/// \param [in] ERROR Errors enum with error occurred in program
#define UPDATE_ERR(ERROR) ErrorInfo.error = ERROR

#endif

//----------------------------------------------------------------------------------------------

/// \brief print errors from special struct with errors errorInfo.
/// \attention errorInfo have to be updated only with UPDATE_ERR();
void PrintError();

//-----------------------------------------------------------------------------------------------

/// @brief checks if the error in ErrorInfo is fatal.
/// @returns true if error is fatal and have to leave the program, otherwise false
bool IsFatalError();

//-----------------------------------------------------------------------------------------------

#endif // ERRORS_H
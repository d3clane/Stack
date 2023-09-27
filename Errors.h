//
// Created by Arman on 21.08.2023.
//

/// \file
/// \brief Contains errors that may occur during the program working. Can handle <= 63 errors.
/// \details Contains errors info and function to print these errors.

#ifndef ERRORS_H
#define ERRORS_H

#include <assert.h>

//#define NDEBUG

//-----------------------------------------------------------------------------------------------

//TODO: think about making bits structure and return it
/// \brief Errors than may occur during the program working. 
enum class Errors 
{   
    NO_ERR, 

    MEMORY_ALLOCATION_ERR,
    
    STACK_EMPTY_ERR, 
    STACK_IS_NULLPTR,
    STACK_CAPACITY_OUT_OF_RANGE,
    STACK_SIZE_OUT_OF_RANGE,
    STACK_INVALID_CANARY,
    STACK_INVALID_DATA_HASH,
    STACK_INVALID_STRUCT_HASH,
};

//-----------------------------------------------------------------------------------------------

typedef unsigned long long ErrorType;

#ifndef NDEBUG

    /// \brief Contains info about errors - File with error, line with error, error code. 
    /// \warning Have to be updated with UPDATE_ERR() only
    struct ErrorInfoType 
    {
        ErrorType error;           ///< error code

        const char* fileWithError; ///< __FILE__ (file name with error)
        const char* funcWithError; ///< __func__ (function name with error)
        int lineWithError;         ///< __LINE__ (line with error)
    };

#else

    /// \brief Contains info about errors - File with error, line with error, error code. 
    /// \warning Have to be updated with UPDATE_ERR() only
    struct ErrorInfoType 
    {
        ErrorType error;           ///< error code
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
        ErrorInfo.error |= ((ErrorType) 1 << (ErrorType)(ERROR));         \
    } while(0)
    
    #define HANDLE_ERR(ERROR)                                             \
    do                                                                    \
    {                                                                     \
        UPDATE_ERR(ERROR);                                                \
        PrintError();                                                     \
    } while (0)

    #define DELETE_ERR(ERROR) ErrorInfo.error &= ~((ErrorType)1 << (ErrorType)(ERROR));
    
    #define CLEAR_ERR() ErrorInfo.error = 0;

    
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
bool HasFatalError();

//-----------------------------------------------------------------------------------------------

inline bool GetError(const Errors error)
{
    assert((size_t) error <= sizeof(ErrorType) * 8);
    assert((size_t) error >= 0);

    return ((ErrorInfo.error) >> (ErrorType)error) & 1;    
}

/// @brief returns ErrorInfo.error
/// @return returns ErrorInfo.error
static inline ErrorType GetErrors()
{
    return ErrorInfo.error;
}

//-----------------------------------------------------------------------------------------------

#endif // ERRORS_H

/**
* Emotiv SDK
* Copyright (c) 2016 Emotiv Inc.
*
* This file is part of the Emotiv SDK.
*
*/


#ifndef EDFDATA_H
#define EDFDATA_H


#ifdef __cplusplus
extern "C" {
#endif

#if (!EDK_STATIC_LIB)
#   ifdef EDK_EXPORTS
#       ifdef _WIN32
#           define EDK_API __declspec(dllexport)
#       else
#           if (defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER || defined __clang__
#               define EDK_API __attribute__ ((visibility("default")))
#           else
#               define EDK_API
#           endif
#       endif
#   else
#       ifdef _WIN32
#           define EDK_API __declspec(dllimport)
#       else
#           define EDK_API
#       endif
#   endif
#else
#define EDK_API extern
#endif

//! Start saving EEG to EDF file for a particular user.
/*!
    \remark Only available in SDK Premium Edition.

    \param userID - user ID
    \param filename - name of EDF file
    \param patientID - patient ID embedded in the EDF file
    \param recordID - record ID embedded in the EDF file
    \param date - record date embedded in the EDF file
    \param time - record time embedded in the EDF file
    \return EDK_ERROR_CODE
            - EDK_OK if the command succeeded

    \sa IEE_EdfStopSaving()
*/
EDK_API void
IEE_EdfStartSaving(int userID,
                   const char * filename,
                   const char * patientID,
                   const char * recordID,
                   const char * date,
                   const char * time);


//! Stop saving EEG for a particular user.
/*!
    \remark Only available in SDK Premium Edition.

    \param userID - user ID
    \return EDK_ERROR_CODE
            - EDK_OK if the command succeeded

    \sa IEE_EdfStartSaving()
*/
EDK_API void
IEE_EdfStopSaving(int userID);


//! Stop saving EEG from all user.
/*!
    \remark Only available in SDK Premium Edition.

    \return EDK_ERROR_CODE
            - EDK_OK if the command succeeded

    \sa IEE_EdfStartSaving(), IEE_EdfStopSaving()
*/
EDK_API void
IEE_EdfStopSavingAll();


//! Initializes EmoEngine instance which reads data from a pre-recorded session file.
/*!
    \remark Only available in SDK Premium Edition.

    \param strFilePath - File path to the pre-recorded EEG data file.
    \param strMotionFilePath - File path to the pre-recorded motion data file.

    \return EDK_ERROR_CODE
            - EDK_OK if an instance is created successfully

    \sa IedkErrorCode.h
*/
EDK_API int
IEE_EngineLocalConnect(const char * strFilePath,
                       const char * strMotionFilePath = "");


//! Start playback of the EDF file.
/*!
    This function should only be involved after calling IEE_EngineLocalConnect() successfully.

    \remark Only available in SDK Premium Edition.

    \return EDK_ERROR_CODE
            - EDK_OK if the command succeeded

    \sa IEE_EngineLocalConnect(), IEE_EdfStop()
*/
EDK_API int
IEE_EdfStart();


//! Stop playback of EDF.
/*!
    \remark Only available in SDK Premium Edition.

    \return EDK_ERROR_CODE
            - EDK_OK if the command succeeded

    \sa IEE_EdfStart()
*/
EDK_API int
IEE_EdfStop();


//! Seek current EDF file.
/*!
    \remark Only available in SDK Premium Edition.

    \param sec - time position to seek in seconds
    \return EDK_ERROR_CODE
            - EDK_OK if the command succeeded

    \sa IEE_EdfStart()
*/
EDK_API int
IEE_EdfSeek(int sec);


//! Get total time of EDF file.
/*!
    \remark Only available in SDK Premium Edition.

    \param pTotalTimeOut - received total time of EDF File in seconds
    \return EDK_ERROR_CODE
            - EDK_OK if the command succeeded

    \sa IEE_EngineLocalConnect()
*/
EDK_API int
IEE_EdfGetTotalTime(int * pTotalTimeOut);

#ifdef __cplusplus
}
#endif
#endif // EDFDATA_H

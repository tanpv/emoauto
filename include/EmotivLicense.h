/**
* Emotiv SDK
* Copyright (c) 2016 Emotiv Inc.
*
* This file is part of the Emotiv SDK.
*
* Header file for license related API.
*
*/

#ifndef EMOTIVLICENSE_H
#define EMOTIVLICENSE_H

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
#   define EDK_API extern
#endif

    typedef enum IEE_LicenseType_enum {
        IEE_EEG = 0x001,      // Enable EEG data
        IEE_PM  = 0x002,      // Enable Performance Metric detection   
        IEE_EEG_PM = IEE_EEG | IEE_PM   // Enable EEG data and Performance Metric detection   
    } IEE_LicenseType_t;

    typedef struct IEE_LicenseInfos_struct {
        unsigned int scopes;            // license type
        unsigned int date_from;         // epoch time 
        unsigned int date_to;           // epoch time
        unsigned int seat_count;        // number of seat
        unsigned int usedQuota;         // total number of session used.
        unsigned int quota;             // total number of session got for this PC.

    } IEE_LicenseInfos_t;

    //! Check information of the license
    /*!
        \param remainingSessions - remaining session number of the license. 
        \return EDK_ERROR_CODE
                     - EDK_OK if the command succeeded

        \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_GetRemainingSessions(const char* licenseID, unsigned int* remainingSessions);


    //! Check information of the license
    /*!    
        \param licenseID - License key
        \param licenseInfo - License Information    
        \return EDK_ERROR_CODE
                     - EDK_OK if the command succeeded

        \sa IedkErrorCode.h
    */
    EDK_API int
        IEE_LicenseInformation(IEE_LicenseInfos_t * licenseInfo);


    //! Activate a license with license ID
    /*!
        \param licenseID - License key
        \param debitNum  - Indicates number of sessions will be deducted from license
        
        \return EDK_ERROR_CODE
                     - EDK_OK if the command succeeded

        \sa IedkErrorCode.h
    */
    EDK_API int 
        IEE_ActivateLicense(const char* licenseID, unsigned int debitNum);


#ifdef __cplusplus
}
#endif
#endif // EMOTIVLICENSE_H

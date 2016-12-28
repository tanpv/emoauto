/**
 * Emotiv SDK
 * Copyright (c) 2016 Emotiv Inc.
 *
 * This file is part of the Emotiv SDK.
 *
 * The main interface that allows interactions between external programs and the Emotiv detection engine.
 *
 * None of these API functions are thread-safe.
 *
 * This header file is designed to be included under C and C++ environment.
 *
 * These API are for client with special access to EEG data.
 * A separate lib with emotiv_func() will be provided to generate the security code.
 * The generated code must be passed into IEE_CheckSecurityCode() in order to
 * run the EDK.
 *
 * Example of using the security API with EDK:
 *
 *      std::string devId = "Your given developer ID";
 *      if ( IEE_EngineConnect(devId.c_str()) != EDK_OK) {
 *          throw std::exception("Emotiv Engine failed to start.");
 *      }
 *      else {
 *          double x1 = IEE_GetSecurityCode();
 *          double x2 = emotiv_func(x1);
 *          if (IEE_CheckSecurityCode(x2) != EDK_OK) {
 *              throw std::exception("Invalid security code.");
 *          }
 *      }
 *
 */

#ifndef IEDK_SECUR_H
#define IEDK_SECUR_H

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



//! Get security code.
/*!
    \remark Available only to custom built of SDK with EEG access.
 */
EDK_API double  IEE_GetSecurityCode(void);


//! Check security code.
/*!
    \remark Available only to custom built of SDK with EEG access.
 */
EDK_API int     IEE_CheckSecurityCode(double x);


#ifdef __cplusplus
}
#endif
#endif // IEDK_SECUR_H

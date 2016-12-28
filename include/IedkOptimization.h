/**
 * Emotiv Insight SDK
 * Copyright (c) 2015 Emotiv Inc.
 *
 * This file is part of the Emotiv Insight SDK.
 *
 * The main interface that allows interactions between external programs and the Emotiv detection engine.
 *
 * None of these API functions are thread-safe.
 *
 * This header file is designed to be included under C and C++ environment.
 *
 */

#ifndef IEDK_OPTIM_H
#define IEDK_OPTIM_H

#ifdef __cplusplus
extern "C"
{
#endif

#ifndef EDK_STATIC_LIB
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

#include "IedkErrorCode.h"
#include "IEmoStateDLL.h"

    //! Handle to optimization param allocated by IEE_OptimizationParamCreate.
    /*!
        \sa IEE_OptimizationParamCreate()
     */
    typedef void* OptimizationParamHandle;
    
    
    //! Create an OptimizationParamHandle.
    /*!
        Returns a handle to memory that can hold an optimization paramaeter which is used to configure the behaviour of optimization
     
        \return OptimizationParamHandle
    */
    EDK_API OptimizationParamHandle
        IEE_OptimizationParamCreate();
    

    //! Destroy an OptimizationParamHandle.
    /*!
        Free memory referenced by an optimization parameter handle.
     
        \param hParam - a handle returned by IEE_OptimizationParamCreate()
    */
    EDK_API void
        IEE_OptimizationParamFree(OptimizationParamHandle hParam);
    

    //! Enable optimization.
    /*!
        EmoEngine will try to optimize its performance according to the information passed in with optimization parameter.
        EmoEngine guarantees the correctness of the results of vital algorithms.
        For algorithms that are not vital, results are undefined.
     
        \param hParam - a handle returned by IEE_OptimizationParamCreate()
        \return EDK_ERROR_CODE
                - EDK_OK if successful
    */
    EDK_API int
        IEE_OptimizationEnable(OptimizationParamHandle hParam);
    

    //! Check status of optimization.
    /*!
        \param pEnabledOut - receives information about whether optimization is on
        \return EDK_ERROR_CODE
                - EDK_OK if successful
    */
    EDK_API int
        IEE_OptimizationIsEnabled(bool* pEnabledOut);
    

    //! Disable optimization.
    /*!
        \return EDK_ERROR_CODE
                - EDK_OK if successful
    */
    EDK_API int
        IEE_OptimizationDisable();
    

    //! Get current optimization parameter.
    /*!
        If optimization is not enabled (this can be checked with IEE_OptimmizationIsEnabled) then the results attached to the hParam parameter are undefined.
     
        \param hParam - a handle returned by IEE_OptimizationParamCreate()
        \return EDK_ERROR_CODE
                - EDK_OK if successful
    */
    EDK_API int
        IEE_OptimizationGetParam(OptimizationParamHandle hParam);
    

    //! Get a list of vital algorithms of specific suite for optimization.
    /*!
        \param hParam - a handle returned by IEE_OptimizationParamCreate()
        \param suite - suite that you are interested in
        \param pVitalAlgorithmBitVectorOut - receives a list of vital algorithm composed of IEE_FacialExpressionAlgo_t, IEE_PerformanceMetricAlgo_t or IEE_MentalCommandAction_t depending on the suite parameter
        \return EDK_ERROR_CODE
                - EDK_OK if successful
    */
    EDK_API int
        IEE_OptimizationGetVitalAlgorithm(OptimizationParamHandle hParam,
                                          IEE_EmotivSuite_t suite,
                                          unsigned int* pVitalAlgorithmBitVectorOut);

    
    //! Set a list of vital algorithms of specific suite for optimization.
    /*!
        \param hParam - a handle returned by IEE_OptimizationParamCreate()
        \param suite - suite that you are interested in
        \param vitalAlgorithmBitVector - a list of vital algorithm composed of IEE_FacialExpressionAlgo_t, IEE_PerformanceMetricAlgo_t or IEE_MentalCommandAction_t depended on the suite parameter passed in
        \return EDK_ERROR_CODE
                - EDK_OK if successful
    */
    EDK_API int
        IEE_OptimizationSetVitalAlgorithm(OptimizationParamHandle hParam,
                                          IEE_EmotivSuite_t suite,
                                          unsigned int vitalAlgorithmBitVector);
    

    //! Reset all settings and user-specific profile data for the specified detection suite
    /*!
        \param userId - user ID
        \param suite - detection suite (FacialExpression, PerformanceMetric, or MentalCommand)
        \param detectionBitVector - identifies specific detections.  Set to zero for all detections.
        \return EDK_ERROR_CODE
                - EDK_OK if successful
    */
    EDK_API int
        IEE_ResetDetection(unsigned int userId,
                           IEE_EmotivSuite_t suite,
                           unsigned int detectionBitVector);
    
#ifdef __cplusplus
}
#endif
#endif // IEDK_OPTIM_H

/**
 * Emotiv SDK
 * Copyright (c) 2016 Emotiv Inc.
 *
 * This file is part of the Emotiv SDK.
 *
 * The main interface that allows interactions between external programs and Emotiv Cloud
 *
 * None of these API functions are thread-safe.
 *
 * This header file is designed to be included under C and C++ environment.
 *
 */

#ifndef EMOTIVCLOUDPRIVATE_H
#define EMOTIVCLOUDPRIVATE_H

#ifdef __cplusplus
extern "C" {
#endif

#if (!EDK_STATIC_LIB)
#   ifdef EMOTIVCLOUDCLIENT_EXPORTS
#       ifdef _WIN32
#           define EMOTIVCLOUD_API __declspec(dllexport)
#       else
#           if (defined __GNUC__ && __GNUC__ >= 4) || defined __INTEL_COMPILER || defined __clang__
#               define EMOTIVCLOUD_API __attribute__ ((visibility("default")))
#           else
#               define EMOTIVCLOUD_API
#           endif
#       endif
#   else
#       ifdef _WIN32
#           define EMOTIVCLOUD_API __declspec(dllimport)
#       else
#           define EMOTIVCLOUD_API
#       endif
#   endif
#else
#   define EMOTIVCLOUD_API extern
#endif

#include "EmotivCloudClient.h"

#define MAX_NUM_OF_BACKUP_PROFILE_VERSION 2

typedef enum EC_RequestType {
    ECRT_Signup = 0,
    ECRT_UserDetail,
    ECRT_GetListRecording,
    ECRT_GetListActivity,
    ECRT_GetListRecordingTime,
    ECRT_GetListRecordingEx,
    ECRT_GetListRecordingTags,
    ECRT_GetListRecordingSearch,
    ECRT_GetPerformanceReport,
    ECRT_CompareAllEmotiv,
    ECRT_CompareAllActivities,
    ECRT_CompareActivity,
    ECRT_CompareSession,
    ECRT_CreateSession,
    ECRT_GetLastBaseline,
    ECRT_GetRecoringDetail,
    ECRT_GetListChallenge,
    ECRT_GetListTags
} EC_RequestType_t;


typedef enum class EC_EdfFileType {
    EEG = 0,
    MOTION
} EC_EdfFileType_t;

//! Enable cloud thread
/*!
    \return EC_ERROR_CODE
            - EC_OK if set cloud thread successfully
*/
EMOTIVCLOUD_API int EC_EnableCloudThread();

//! Enable mentalcommand thread
/*!
    \return EC_ERROR_CODE
            - EC_OK if set cloud thread successfully
*/
EMOTIVCLOUD_API int EC_EnableMentalCommandThread();
    
//! Login with Facebook account
/*!
    \param facebookToken   - facebook access token
    \return EC_ERROR_CODE
            - EC_OK if login successfully
 */
EMOTIVCLOUD_API int EC_LoginWithFacebook(const char * facebookToken);


//! Login with Google Plus account
/*!
    \param googleToken - google plus access token
    \return EC_ERROR_CODE
            - EC_OK if login successfully
 */
EMOTIVCLOUD_API int EC_LoginWithGooglePlus(const char * googleToken);

//! Create new infomation of device on Emotiv Cloud
/*!
     \param userAgent       - user agent for request
     \return EC_ERROR_CODE
            - EC_OK if set user agent successfully
*/
EMOTIVCLOUD_API int EC_SetUserAgent(const char * userAgent);

//! Sign up new account on Emotiv Cloud
/*!
    \param username        - user name of account
    \param firstname    - first name of account
    \param lastname     - last name of account
    \param email        - email of account using for login
    \param password     - password of account using for login
    \param userID        - receives number of user
    \param gender        - gender of user "M" or "F"
    \param date_of_birth- datetime string of user's birthday. Format: YYYY[-MM[-DD]]
    \return EC_ERROR_CODE
            - EC_OK if sign up successfully
*/
EMOTIVCLOUD_API int EC_Signup(const char * username, const char * firstname,
                              const char * lastname, const char * email, const char * password,
                              const char * gender, const char * date_of_birth, const char * handness,
                              double * goal, char ** responseString);


//! Create new infomation of headset on Emotiv Cloud
/*!
    \param engineUserID  - id of user in SDK
    \return EC_ERROR_CODE
            - EC_OK if create headset successfully
*/
EMOTIVCLOUD_API int EC_CreateHeadset(int userID, int engineUserID);


//! Create new session on Emotiv Cloud
/*!
    \param userID        - id of user
    \param sessionID     - receives id of session
   \return EC_ERROR_CODE
            - EC_OK if create session successfully and return id of session
*/
EMOTIVCLOUD_API int EC_CreateSession(int userID, int experimentID,
                                     char ** sessionID);

//! Create new offline session on Emotiv Cloud
/*!
    \param userID        - id of user
    \param sessionID     - receives id of session
    \return EC_ERROR_CODE
        - EC_OK if create session successfully and return id of session
*/
EMOTIVCLOUD_API int EC_CreateSessionOffline(int userID, int experimentID, char ** sessionID);

//! Start record data from headset and upload to Emotiv Cloud
/*!
    \param userID        - id of user
    \return char
            - return null if create sessionID successfully
*/
EMOTIVCLOUD_API int EC_StartRecordData(int userID);

//! Pause record data from headset
/*!
    \param userID        - id of user
    \return EC_ERROR_CODE
            - EC_OK if pause recording successfully
*/
EMOTIVCLOUD_API int EC_PauseRecordData(int userID);

//! Resume record data from headset
/*!
    \param userID        - id of user
    \return EC_ERROR_CODE
            - EC_OK if resume recording successfully
*/
EMOTIVCLOUD_API int EC_ResumeRecordData(int userID);

//! Stop and abort this recording
/*!
    \param userID        - id of user
    \return EC_ERROR_CODE
            - EC_OK if abort recording successfully
*/
EMOTIVCLOUD_API int EC_AbortRecordData(int userID);

//! Stop record data from headset
/*!
    \param userID        - id of user
    \return EC_ERROR_CODE
            - EC_OK if end recording successfully
*/
EMOTIVCLOUD_API int EC_StopRecordData(int userID);

//! Start marker of "eyes open" state and upload on Emotiv Cloud
/*!
    \param userID        - id of user
    \return EC_ERROR_CODE
            - EC_OK if marker successfully
*/
EMOTIVCLOUD_API int EC_Marker_EyeOpenStart(int userID);

//! End marker of "eyes open" state and upload on Emotiv Cloud
/*!
    \param userID        - id of user
    \return EC_ERROR_CODE
            - EC_OK if marker successfully
*/
EMOTIVCLOUD_API int EC_Marker_EyeOpenEnd(int userID);

//! Start marker of "eyes closed" state and upload on Emotiv Cloud
/*!
    \param userID        - id of user
    \return EC_ERROR_CODE
            - EC_OK if marker successfully
*/
EMOTIVCLOUD_API int EC_Marker_EyeClosedStart(int userID);

//! End marker of "eyes closed" state and upload on Emotiv Cloud
/*!
    \param userID        - id of user
    \return EC_ERROR_CODE
            - EC_OK if marker successfully
*/
EMOTIVCLOUD_API int EC_Marker_EyeClosedEnd(int userID);

//! Start marker of "recording" state and upload on Emotiv Cloud
/*!
    \param userID        - id of user
    \return EC_ERROR_CODE
            - EC_OK if marker successfully
*/
EMOTIVCLOUD_API int EC_Marker_RecordingStart(int userID);

//! Update note for a recording
/*!
    \param userID      - id of user
    \param sessionID   - id of recording
    \param newNote     - new note updated
 
    \return EC_ERROR_CODE
                 - EC_OK if update successfully
*/
EMOTIVCLOUD_API int EC_UpdateNote(int userID, const char * sessionID,
                                  const char * newNote);

//! Update tags for a recording
/*!
     \param userID      - id of user
     \param sessionID   - id of recording
     \param tag         - new tag updated
     \param tag         - number of tags

     \return EC_ERROR_CODE
                - EC_OK if update successfully
*/
EMOTIVCLOUD_API int EC_UpdateTags(int userID, const char * sessionID, const char ** tags, int numberTag);
    
//! Update mood for a recording
/*!
    \param userID      - id of user
    \param sessionID   - id of recording
        \param content     - content score
    \param relax          - relax score
       \param focus         - focus score
    \return EC_ERROR_CODE
                 - EC_OK if update successfully
*/
EMOTIVCLOUD_API int EC_UpdateMood(int userID, const char * sessionID,
                                  int content, int relax, int focus, bool isCreate);

//! Upload image for a recording
/*!
    \param userID      - id of user
    \param sessionID   - id of session
    \param filePath    - path of image file
    \return EC_ERROR_CODE
                 - EC_OK if upload image successfully
*/
EMOTIVCLOUD_API int EC_UploadSessionPhoto(int userID, const char * sessionID,
        const char * filePath);

//! Upload image for a recording
/*!
    \param userID      - id of user
    \param sessionID   - id of session
    \return EC_ERROR_CODE
                 - EC_OK if get recording detail successfully
*/
EMOTIVCLOUD_API int EC_GetRecordingDetail(int userID, const char * sessionID);

//! Delete a recording
/*!
    \param userID      - id of user
    \param sessionID   - id of session
    \return EC_ERROR_CODE
                 - EC_OK if delete record successfully
*/
EMOTIVCLOUD_API int EC_DeleteRecording(int userID, const char * sessionID);

//! Get affectiv file for session
    /*!
     \param userID      - id of user
     \param sessionID   - id of session
     \param destPath    - path will store file
     
     \return EC_ERROR_CODE
                - EC_OK if delete record successfully
*/
EMOTIVCLOUD_API int EC_AffectivDownloadFile(int userID, const char * sessionID, const char * destPath);

//! Cancel request download file
/*!
     \param userID      - id of user
     \param sessionID   - id of session
 
     \return EC_ERROR_CODE
                - EC_OK if delete record successfully
*/
EMOTIVCLOUD_API int EC_CancelDownloadFile(int userID, const char * sessionID);

//! Progress of download file
/*!
     \param sessionID          - id of session
     \param totalToDownload    - Byte size of file
     \param nowDownloaded      - current byte size downloaded
 
     \return EC_ERROR_CODE
                - EC_OK if ger result successfully
*/
EMOTIVCLOUD_API int EC_GetDownloadProgress(const char * sessionID, double * totalToDownload, double * nowDownloaded);
  
//! Check is session downloading or not
/*!
     \param sessionID          - id of session
     \return EC_ERROR_CODE
                - EC_OK if session is downloading
*/
EMOTIVCLOUD_API int EC_IsDownloadSession(int userID, const char * sessionID);

//! Change password
/*!
     \param userID        - id of user
     \param oldPassword   - old password
     \param newPassword   - new password
     
     \return EC_ERROR_CODE
                - EC_OK if update password successfully
*/
EMOTIVCLOUD_API int EC_ChangePassword(int userID, const char * oldPassword, const char * newPassword);
    
//! get default user when using multiple users
/*
    \return int - cloud user ID
*/
EMOTIVCLOUD_API  int EC_GetDefaultUser();

//! Set default path to store data
/*!
    \return EC_ERROR_CODE
            - EC_OK if set successfully
*/
EMOTIVCLOUD_API int EC_SetDefaultPath(const char * defaultPath);

//! Get accessToken of user
/*!
    \return char
            - return token of user
*/
EMOTIVCLOUD_API const char * EC_GetToken(int userID);

//! Set accessToken for user
/*!
    \return EC_ERROR_CODE
            - EC_OK if set token successfully
*/
EMOTIVCLOUD_API int EC_SetToken(const char * token);

//! Check accessToken for user
/*!
    \return EC_ERROR_CODE
                 - EC_OK if set token successfully
*/
EMOTIVCLOUD_API int EC_CheckToken(const char * token);

//! Get list recordings of user
/*!
    \param page         - page of recording list
    \param pageSize     - number result of each page
    \param protocolID   - id of protocol which only get recordings belong to, default = -1 is get all recordings of user
    \param aliasPath    - path for specify link

    \return EC_ERROR_CODE
                - EC_OK if get recording list successfully
*/
EMOTIVCLOUD_API int EC_GetListRecording(int userID, int page, int pageSize, const char * aliasPath = "", int protocolID = -1);

//! Get list recordings of user
/*!
      \param userID        - id of user
      \param page         - page of recording list
      \param pageSize     - number result of each page
      \param date         - number of date
      \param month        - number of month
      \param year         - number of year
      \param range        - number of range between 2 days
      \param protocolID   - id of protocol which only get recordings belong to, default = -1 is get all recordings of user
      \param aliasPath    - path for specify link, default = "" is get all recordings of user

      \return EC_ERROR_CODE
                  - EC_OK if get recording list successfully
*/
EMOTIVCLOUD_API int EC_GetListRecordingByTime(int userID, int page,
        int pageSize, int date, int month, int year, int range, const char * aliasPath = "", int protocolID = -1);

//! Get list recordings of user by experiment id
/*!
       \param userID       - id of user
       \param page         - page of recording list
       \param pageSize     - number result of each page
       \param activityID   - id of activity
       \param protocolID   - id of protocol which only get recordings belong to, default = -1 is get all recordings of user
       \param aliasPath    - path for specify link, default = "" is get all recordings of user

       \return EC_ERROR_CODE
                   - EC_OK if get recording list successfully
*/
EMOTIVCLOUD_API int EC_GetListRecordingByExperiment(int userID, int page,
        int pageSize, int activityID, const char * aliasPath = "", int protocolID = -1);

//! Get list recordings of user by experiment id
/*!
     \param userID       - id of user
     \param page         - page of recording list
     \param pageSize     - number result of each page
     \param tag          - tag of recording
     \param protocolID   - id of protocol which only get recordings belong to, default = -1 is get all recordings of user
     \param aliasPath    - path for specify link, default = "" is get all recordings of user

     \return EC_ERROR_CODE
                - EC_OK if get recording list successfully
*/
EMOTIVCLOUD_API int EC_GetListRecordingByTags(int userID, int page, int pageSize, const char* tag, const char * aliasPath = "", int protocolID = -1);

//! Get list recordings of user
/*!
       \param userID       - id of user
       \param page         - page of recording list
       \param pageSize     - number result of each page
       \param searchText   - text which activity contain
       \param protocolID   - id of protocol which only get recordings belong to, default = -1 is get all recordings of user
       \param aliasPath    - path for specify link, default = "" is get all recordings of user

       \return EC_ERROR_CODE
                   - EC_OK if get recording list successfully
*/
EMOTIVCLOUD_API int EC_SearchRecordingByExperimentName(int userID, int page,
        int pageSize, const char * searchText, const char * aliasPath = "", int protocolID = -1);


//! Get list experiments of user
/*!
      \param userID         - id of user
      \param page           - page of experiments list
      \param pageSize       - number result of each page
      \param haveRecording  - only get experiments have recording if true
      \param protocolID     - id of protocol which only get experiments belong to, default = -1 is get all experiments of user
      \param aliasPath      - path for specify link, default = "" is get all experiments of user

      \return EC_ERROR_CODE
                 - EC_OK if get activity list successfully
*/
EMOTIVCLOUD_API int EC_GetListExperiment(int userID, int page, int pageSize,
        bool haveRecording, const char * aliasPath = "", int protocolID = -1);

//! Get performance report of user
/*!
     \param userID       - id of user
     \param type         - type of time want to get : year, week, day
     \param year         - year of time
     \param month        - month of time
     \param day          - day of time
      \return EC_ERROR_CODE
                - EC_OK if get data successfully
*/
EMOTIVCLOUD_API int EC_GetPerformanceReport(int userID, const char * type,
        int year, int month, int day);

//! Compare with all Emotiv
/*!
     \param userID       - id of user
     \param experimentID - id of experiment which you want to compare
     \param timeZone     - time zone of device
      \return EC_ERROR_CODE
                - EC_OK if compare successfully
*/
EMOTIVCLOUD_API int EC_CompareAllEmotiv(int userID, int experimentID,
                                        const char * timeZone);

//! Compare with all experiments
/*!
     \param userID        - id of user
     \param experimentID - id of experiment which you want to compare
     \param timeZone     - time zone of device
      \return EC_ERROR_CODE
                - EC_OK if compare successfully
*/
EMOTIVCLOUD_API int EC_CompareAllExperiments(int userID, int experimentID,
        const char * timeZone);

//! Compare with another Experiment
/*!
     \param userID              - id of user
     \param currentExperiment   - id of experiment which you want to compare
     \param compareExperiment   - id of experiment which you want to compare to
     \param fromDate            - date which start to compare
     \param toDate              - date which end to compare
     \param timeZone            - time zone of device
      \return EC_ERROR_CODE
                - EC_OK if compare successfully
*/
EMOTIVCLOUD_API int EC_CompareExperiment(int userID, int currentExperiment,
        int compareExperiment, const char * fromDate, const char * toDate,
        const char * timeZone);

//! Compare a session result with all session of user
/*!
     \param userID              - id of user
     \param sessionID           - id of session
     \param minAge              - min age for compare
     \param maxAge              - max age for compare
 
     \return EC_ERROR_CODE
                - EC_OK if compare successfully
*/
EMOTIVCLOUD_API int EC_CompareSession(int userID, const char * sessionID, int minAge, int maxAge);
    
//! Update avatar for profile
/*!
        \param userID        - id of user
       \param firstname    - information of user
       \param lastname    - information of user
       \param birthday    - information of user
       \param city        - information of user
       \param state        - information of user
       \param country        - information of user
       \param ios            - information of user
       \return EC_ERROR_CODE
          - EC_OK if upload successfully
*/
EMOTIVCLOUD_API int EC_UpdateProfile(int userID, const char * firstname,
                                     const char * lastname, const char * birthday, const char * city,
                                     const char * state, const char * country, const char * iso);

//! Get time of recording session
/*!
    \param userID       - id of user
    \param sessionID    - id of session
    \return EC_ERROR_CODE
            - EC_OK if finish successfully
*/
EMOTIVCLOUD_API int EC_FinishSession(int userID, const char * sessionID);

//! Get time of recording session
/*!
    \param userID          - id of user
    \param sessionID    - id of session
    \return EC_ERROR_CODE
            - EC_OK if get time successfully
*/
EMOTIVCLOUD_API int EC_GetTimeRecording(int userID, const char * sessionID,
                                        int * time);

//! Get time of last baseline test
/*!
    \param userID        - id of user
    \return EC_ERROR_CODE
            - EC_OK if baseline successfully
*/
EMOTIVCLOUD_API int EC_GetLastBaseline(int userID);

//! join a protocol
/*!
    \param userID        - id of user
    \param protocolID - id of protocol
    \return EC_ERROR_CODE
            - EC_OK if join successfully
*/
EMOTIVCLOUD_API int EC_JoinProtocol(int userID, int protocolID, const char * aliasPath = "");

//! Request get offline report on Emotiv Cloud
/*!
    \param userID        - id of user
    \return EC_ERROR_CODE
            - EC_OK if report is generated on Emotiv Cloud successfully
*/
EMOTIVCLOUD_API int EC_GetOfflineReport(int userID, const char * sessionID,
                                        int * engagementScore, int * focusScore, int * interestScore, int * relaxScore,
                                        int * stressScore, int * exciteScore);

//! Update avatar for profile
/*!
      \param userID            - id of user
      \param value         - value for update
      \param params        - id of params for update
      \return EC_ERROR_CODE
              - EC_OK if udpate successfully
*/
EMOTIVCLOUD_API int EC_UpdateAttributes(int userID, const char * value,
                                        int params, bool isCreated);

//! Update avatar for profile
/*!
      \param userID            - id of user
      \param filePath      - path of image file which want upload
      \return EC_ERROR_CODE
              - EC_OK if upload successfully
*/
EMOTIVCLOUD_API int EC_UploadAvatar(int userID, const char * filePath);

//! join a protocol
/*!
    \param userID        - id of user
    \param sessionID     - id of session
    \param challengeID   - id of challenge

    \return EC_ERROR_CODE
            - EC_OK if map successfully
*/
EMOTIVCLOUD_API int EC_MapRecordToChallenge(int userID, const char * sessionID,
        int challengeID);

//! join a protocol
/*!
    \param userID        - id of user
    \param sessionID     - id of session
    \param score         - number of score

    \return EC_ERROR_CODE
            - EC_OK if submit successfully
*/
EMOTIVCLOUD_API int EC_SubmitScore(int userID, const char * sessionID,
                                   int score);

//! get list challenge
/*!
    \param userID      - id of user
    \param activityID  - id of activity

    \return EC_ERROR_CODE
                       - EC_OK if get successfully
*/
EMOTIVCLOUD_API int EC_GetListChallenge(int userID, int activityID);

//! get list tags
/*!
     \param userID      - id of user
     \param page        - page of tags list
     \param pageSize    - number result of each page
     \param isPublic    - get public or private tags
     
     \return EC_ERROR_CODE
     - EC_OK if get successfully
*/
EMOTIVCLOUD_API int EC_GetListTags(int userID, int page, int pageSize, bool isPublic);
    
//! get responseString
/*!
    \param userID      - id of user
    \param type        - type of request

    \return EC_ERROR_CODE
            - EC_OK if get data successfully
*/
EMOTIVCLOUD_API int EC_GetResponseString(int userID, int requestType,
        char ** responseString);

//! save offline data
/*!
    \param userID        - id of user
    \param fileName      - name of file contain data
    \return EC_ERROR_CODE
                         - EC_OK if get data successfully
 */
EMOTIVCLOUD_API int EC_SaveOfflineInfo(int userID, const char * fileName);

//!upload data//
EMOTIVCLOUD_API int EC_EdfUploadFile(int cloudUserID, const char * edfFilePath,
                                         const char * descriptions, const char * sessionID, EC_EdfFileType_t edfType);
    
//! reupload offline data
/*!
    \param userID        - id of user
    \param fileName      - name of file contain data

    \return EC_ERROR_CODE
                         - EC_OK if re-upload data successfully
 */
EMOTIVCLOUD_API int EC_ReuploadData(int userID, const char * fileName);

//! set default user when using multiple users
/*
    \param userID - cloud user ID

    \return EC_ERROR_CODE
                                    - EC_OK if success
*/
EMOTIVCLOUD_API  int EC_SetDefaultUser(int userID);

//! set default user when using offline mode
/*
    \param userID - offline user ID
     
    \return EC_ERROR_CODE
                - EC_OK if success
*/
EMOTIVCLOUD_API  int EC_SetDefaultOfflineUser(int userID);
    
//! Login as guest profile
/*!
    \param engineUserID

    \return true
                         -  if login successfully
 */

EMOTIVCLOUD_API int EC_LoginAsGuest(int engineUserID);

//! get available
/*
    \param profileID    - profileID
    \param pVersionInfo - receives array of version Informations
    \param nVersion     - receives number of versions

    \return EC_ERROR_CODE
                                    - EC_OK if success
*/
EMOTIVCLOUD_API  int EC_GetAvailableProfileVersions(int userID, int profileID,
        profileVersionInfo * pVersionInfo, int * nVersion);
//! set client id and client secret
/*
    \param clientID        - clientID
    \param clientSecret    - clientSecret
    \return EC_ERROR_CODE
                        - EC_OK if success
*/
EMOTIVCLOUD_API  int EC_SetClientSecret(const char * clientID,
                                        const char * clientSecret);


//! reupload offline edf data
/*!
    \param cloudUserID   - id of user
    \param filePath      - name of file contain data(File name should be sessionID)
    \param edfType       - type of Edf file. EEG or Motion

    \return EC_ERROR_CODE
                         - EC_OK if re-upload data successfully
*/
EMOTIVCLOUD_API int EC_EdfReupload(int cloudUserID, const char * filePath,
                                   EC_EdfFileType edfType = EC_EdfFileType::EEG);


//! Checking a file is uploaded
/*!
    \param cloudUserID   - id of user
    \param sessionID     - session ID of file when created

    \return EC_ERROR_CODE
                         - EC_OK if Edf file exist on Emotiv Cloud.
*/
EMOTIVCLOUD_API int EC_EdfIsUploaded(int cloudUserID, const char * sessionID);

//! Delete exist Edf file of user
/*!
    \param edfId         -

    \return EC_ERROR_CODE
                          - EC_OK if success
*/
EMOTIVCLOUD_API int EC_EdfDeleteFileCloud(int cloudUserID, int edfId);


//! save offline edf data
/*!
     \param cloudUserID   - id of user
     \param infofilePath  - Path of file contain data(File name should be sessionID)
     \param edfFilePath   - Path to file edf
     \param descriptions  - description for file

     \return EC_ERROR_CODE
                          - EC_OK if get save info successfully
 */
EMOTIVCLOUD_API int EC_EdfSaveOfflineInfo(int cloudUserID,
        const char * infoFilePath, const char * descriptions,
        const char * edfFilePath);

//! Donwload file Profile
/*!
    \param cloudUserID   - id of user
    \param profileId
    \param destPath
    \param version                  - default = -1 for download lastest version
    \return EC_ERROR_CODE
                                    - EC_OK if success
*/
EMOTIVCLOUD_API  int  EC_DownloadProfileFile(int cloudUserId, int profileId,
        const char * destPath, int version = -1);

//! Upload file profile of user
/*!
    \param cloudUserID   - id of user
    \param profileName
    \param filePath
    \param ptype

    \return EC_ERROR_CODE
                                        - EC_OK if success
 */
EMOTIVCLOUD_API  int EC_UploadProfileFile(int cloudUserId,
        const char * profileName, const char * filePath, profileFileType ptype);

EMOTIVCLOUD_API  int EC_UpdateProfileFile(int cloudUserId, int profileID,
        const char * filePath);

//! get lastest version of profile
/*
    \param profileID    - profileID
    \param pVersionInfo - receives array of version Informations
    \param nVersion     - receives number of versions

    \return EC_ERROR_CODE
                                        - EC_OK if success
     */
EMOTIVCLOUD_API  int EC_GetLastestProfileVersions(int userID, int profileID,
        int & nVersion);

#ifdef __cplusplus
}
#endif
#endif // EMOTIVCLOUDPRIVATE_H

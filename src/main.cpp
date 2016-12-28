#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
#include <cstdlib>
#include <stdexcept>
#include <string>

#ifdef _WIN32
    #include <conio.h>
    #include <windows.h>
#endif
#ifdef __linux__
    #include <unistd.h>
#endif
#include <chrono>
#include <thread>

#include "IEmoStateDLL.h"
#include "Iedk.h"
#include "IedkErrorCode.h"
#include "IedkSecurity.h"
#include "cusSecurity.h"
#include "EmotivCloudPrivate.h"
#include "EmotivCloudClient.h"

#include "utils.h"

using namespace std;


#ifdef _WIN32
#    include <ShlObj.h>
#    include <windows.h>
#else
#    include <sys/time.h>
#endif

std::string clientID = "P4WxCh_ggBI4;!8DMg2Q_E5@@6v8fv0_gjPDmpOx";
std::string clientSecret = "G=NF5GKA0iqC.vM722wmeeNQ.:!Pv5FTN=efgD7:ReWvQSE46jWD!N=L3X8dily-X8tJdAd3mwYsEl=j@cg.EihXtea2.M?5=xczLnjDsL@rT5ymVs:EP8EokEm@B=sX";

int static_protocolID = 20742;

#ifdef __linux__
int _kbhit(void);
#endif




//Connect to Emotiv Cloud
bool connectElsCloud()
{
    //Disconnect before connecting
    EC_Disconnect();
    if (EC_Connect() != EDK_OK)
    {
        return false;
    }
        

    EC_EnableCloudThread();    // Call this function after EC_Connect
    std::string user_agent = "Emotiv Xavier Control Panel ";
    user_agent.append("3.3.4");

#ifdef _WIN32
#ifdef _WIN64
    OSVERSIONINFO vi;
    memset(&vi, 0, sizeof vi);
    vi.dwOSVersionInfoSize = sizeof vi;
    GetVersionEx(&vi);
    std::stringstream ss;
    ss << "; Win NT " << vi.dwMajorVersion << "." << vi.dwMinorVersion <<
        "; Intel x64";
    user_agent.append(ss.str());
#else
    OSVERSIONINFO vi;
    memset(&vi, 0, sizeof vi);
    vi.dwOSVersionInfoSize = sizeof vi;
    GetVersionEx(&vi);
    std::stringstream ss;
    ss << "; Win NT " << vi.dwMajorVersion << "." << vi.dwMinorVersion <<
        "; Intel x86";
    user_agent.append(ss.str());
#endif
#endif

#ifdef __APPLE__
    SInt32 majorVersion, minorVersion, bugFixVersion;
    Gestalt(gestaltSystemVersionMajor, &majorVersion);
    Gestalt(gestaltSystemVersionMinor, &minorVersion);
    Gestalt(gestaltSystemVersionBugFix, &bugFixVersion);
    std::stringstream ss;
    ss << "; Macintosh;  Intel Mac OS X " << majorVersion << "." << minorVersion <<
        "." << bugFixVersion;
    user_agent.append(ss.str());
#endif

#ifdef __linux__        // need write more here for Ha
    user_agent += "; Linux";
#endif

    if (EC_SetUserAgent(user_agent.c_str()) != EDK_OK)
    {
        return false;
    }
        
    if (EC_SetClientSecret(clientID.c_str(), clientSecret.c_str()) != EDK_OK)
    {
        return false;
    }

    return true;
}


//LoginCloud
bool loginCloud(const char* username, const char* password, int & cloudUserID)
{
    //connect to Emotiv Cloud
    int result = -1;
    cloudUserID = -1;
    if (!connectElsCloud())
    {
        throw std::exception("Can not Connect to Emotiv Cloud");
    }

    //Login to Cloud
    result = EC_Login(username, password);
    if (result != EDK_OK)
        return false;

    result = EC_GetUserDetail(&cloudUserID);
    if (result != EDK_OK)
    {
        return false;
    }
    else
    {
        result = EC_JoinProtocol(cloudUserID, static_protocolID);
    }

    return true;
}


//Calibration
bool calibBeforeRecording(int headsetID, int userCloudID, int experimentID)
{
    bool isCalibWithEyeOpen = false;
    bool isCalibWithEyeClose = false;

    //start Calibrating with eyes open
    std::cout << "Calibrating with eyes open. Please open your eyes 30 seconds from now\n";
    if (EC_Marker_EyeOpenStart(userCloudID) == EDK_OK)
    {
        //set timer record in 30s
        std::this_thread::sleep_for(std::chrono::seconds(30));
        if (EC_Marker_EyeOpenEnd(userCloudID) == EDK_OK)
        {
            std::cout << "Calibrating with eyes open successfully\n";
            isCalibWithEyeOpen = true;
        }
    }
    else
    {
        std:cout << "Calibration with eyes open failed" << '\n';
        return false;
    }

    std::cout << "Calibrating with eyes closed. Please close your eyes 30 seconds from now\n";
    if (EC_Marker_EyeClosedStart(userCloudID) == EDK_OK)
    {
        std::this_thread::sleep_for(std::chrono::seconds(30)); //30 seconds
        if (EC_Marker_EyeClosedEnd(userCloudID) == EDK_OK)
        {
            std::cout << "Calibrating with eyes close successfully\n";
            isCalibWithEyeClose = true;
        }
    }
    else
    {
        std::cout << "Calibration with eyes close failed" << '\n';
        return false;
    }

    return (isCalibWithEyeOpen && isCalibWithEyeClose);
}

int main(int argc, char** argv) {
	
	string devId                = "1bsm2dd-3";
    std::string input;
    std::string username;
    std::string password;
    bool readytocollect         = false;
    bool onStateChanged         = false;
    bool isCalibWithEyeOpen     = false;
    bool isCalibWithEyeClose    = false;
    bool isRecordingFinish      = false;
    char * sessionID = new char[128];
    unsigned int userID = 0;
    int userCloudID = 0;
    int result;
    int state = 0;
    int isCalibrated = false;
    std::map<std::string, int> activityTypes = { { "Demo", 28601 }, { "Meditation", 28450 }, { "Yoga", 28575 }, { "Relaxation", 28577 },
    { "Gaming", 28571 }, { "Sports", 28574 }, { "Exercising", 28593 }, { "Walking", 28594 },
    { "Dancing", 28595 }, { "Driving", 28596 }, { "Playing Music", 28597 }, { "Listening to Music", 28573 },
    { "Singing", 28598 }, { "Reading", 28576 }, { "Writing", 28602 }, { "Brainstorming", 28599 }, { "Studying", 28603 },
    { "Memorizing", 28604 }, { "Language Learning", 28605 }, { "Internet Browsing", 28600 }, { "Watching Media", 28606 },
    { "Public speaking", 28608 }, { "Cooking", 28609 }, { "Drawing", 28611 }, { "Painting", 28612 }, { "Working at Computer", 30000 },
    { "Meeting", 30001 }, { "Manual Labour", 30002 }, { "Programming", 30360 }, { "Doodling", 30364 } };

    EmoEngineEventHandle eEvent = IEE_EmoEngineEventCreate();
    EmoStateHandle eState = IEE_EmoStateCreate();

    if ( IEE_EngineConnect(devId.c_str())  != EDK_OK) 
	{
		throw std::exception("Emotiv Engine start up failed.");
	}
	else
    {
		double x1 = IEE_GetSecurityCode();		
		double x2 = CustomerSecurity::emotiv_func(x1);
		if(IEE_CheckSecurityCode(x2) != EDK_OK){
			throw std::exception("Your Code ID is wrong.");
		}
	}
    std::cout << "==================================================================="
        << std::endl;
    std::cout << "  Example Recording with Calibration \n"
        << std::endl;
    std::cout << "==================================================================="
        << std::endl;
	//User Login
    //Login to clould and get clouldId
LOGIN:
    std::cout << "Login to Cloud" << '\n';
    std::cout << "Please enter username\n";
    std::getline(std::cin, username, '\n');
    std::cout << '\n';
    std::cout << "Please enter password\n";
    std::getline(std::cin, password, '\n');

    if (!loginCloud(username.c_str(), password.c_str(), userCloudID))
    {
        //throw std::exception("Login Cloud Fail");
        std::cout << "Login Failed!! Please check your account" << '\n';
        goto LOGIN;
    }
    std::cout << "LOGIN SUCCESSFULLY!" << '\n';

    std::cout << std::endl << "Please choose one of below activity types:" << std::endl;
    //loop using goto
chooseActivity:
    std::cout <<
        "Demo, Meditation, Yoga, Relaxation, Gaming, Sports," << '\n'
        << "Exercising, Walking, Dancing, Driving, Playing Music," << '\n'
        << "Listening to Music, Singing, Reading, Writing, Brainstorming," << '\n'
        << "Studying, Memorizing, Language Learning, Internet Browsing, " << '\n'
        << "Watching Media, Public speaking, Cooking, Drawing, Painting," << '\n'
        << "Working at Computer, Meeting, Manual Labour, Programming, Doodling" << '\n'
        << std::endl;

    std::getline(std::cin, input, '\n');
    //get experimentID
    int experimentID = 0;

    auto search = activityTypes.find(input);
    if (search != activityTypes.end())
    {
        experimentID = search->second;
        std::cout << "activity: " << search->first << " has experiementID " << experimentID<<'\n';
    }
    else
    {
        std::cout << "\nWrong choosen activity type. Please choose another\n";
        goto chooseActivity;
    }
    std::cout << "Please TURN ON headset to record data" << '\n';
    std::cout << "If want to exit, please press key q to exit" << '\n';


    char key_recieved = 0;
    bool isStartRecord = false;
    while (1)
    {
        if (kbhit())
        {
            key_recieved = getch();
            std::cout << "key recieved: " << key_recieved << '\n';
        }
        if (key_recieved == 'q')
        {
            break;
        }
        state = IEE_EngineGetNextEvent(eEvent);
        if (state == EDK_OK) {

            IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
            IEE_EmoEngineEventGetUserId(eEvent, &userID);

            switch (eventType)
            {
                case IEE_UserAdded:
                    std::cout << "User added" << std::endl;
                    readytocollect = true;
                    break;
                case IEE_UserRemoved:
                    std::cout << "User removed" << std::endl;
                    readytocollect = false; //single connection
                    break;
                case IEE_EmoStateUpdated:
                    onStateChanged = true;
                    IEE_EmoEngineEventGetEmoState(eEvent, eState);
                    break;
                default:
                    break;
            }
        }
        if (readytocollect && onStateChanged)
        {
            onStateChanged = false;

            if (!isCalibrated)
            {
                //Create a new session
                EC_CreateHeadset(userCloudID, userID);
                EC_CreateSession(userCloudID, experimentID, &sessionID);
                result = EC_StartRecordData(userCloudID);

                isCalibrated = calibBeforeRecording(userID, userCloudID, experimentID);
                if (isCalibrated)
                    result = EC_StopRecordData(userCloudID);
            }
            else
            {
                //record Data after Calibration                
                if (!isStartRecord)
                {
                    std::cout << "Start recording data. To stop recording, please press key 's' " << '\n';
                    result = EC_StartRecordData(userCloudID);
                    isStartRecord = (result == EDK_OK);
                }
                else if (key_recieved == 's')
                {
                    //Stop Recording
                    std::cout << "Stop and uploading record to Cloud...\n";
                    result = EC_StopRecordData(userCloudID);
                    if (result == EDK_OK)
                    {
                        isRecordingFinish = true;
                        std::cout << "Data uploaded successfully\n";
                    }
                    else
                    {
                        std::cout << "Stop recording and upload file to Cloud failed\n";
                    }
                    delete sessionID;
                    break;
                }                
            }            
        }
    }
    
    std::cout << "Do you want to generate report Yes\\No ? Y\\N" << '\n';
    std::getline(std::cin, input, '\n');
    if (input != "Y" || input != "Yes")
    {
        std::vector<reports>list_report;
        std::ofstream outFile;
        outFile.open("outReport.txt", std::ios::out | std::ios::trunc);

        //get List Recording
        result = EC_GetListRecording(userCloudID, 1, 20);
        //get Response String
        char * response = nullptr;
        result = EC_GetResponseString(userCloudID, ECRT_GetListRecording, &response);
        //Write report to File
        writeReport(parseReport(response), outFile);

        //close File
        outFile.close();
        
    }

    IEE_EngineDisconnect();
    IEE_EmoStateFree(eState);
    IEE_EmoEngineEventFree(eEvent);

	return 0;
}

#ifdef __linux__
int _kbhit(void)
{
    struct timeval tv;
    fd_set read_fd;

    tv.tv_sec=0;
    tv.tv_usec=0;

    FD_ZERO(&read_fd);
    FD_SET(0,&read_fd);

    if(select(1, &read_fd,NULL, NULL, &tv) == -1)
    return 0;

    if(FD_ISSET(0,&read_fd))
        return 1;

    return 0;
}
#endif

//--------------------------------------------------------------
/*Checklist:
- Login with correct EmotivID
- Download valid profile
- Download invalid profile
- Upload valid profile
- Upload invalid profile
*/

#define BOOST_TEST_MODULE EMOTIV_CLOUD_PROFILE
#define BOOST_TEST_NO_MAIN

#include <boost/test/auto_unit_test.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/thread.hpp>

#include <iostream>
#include <conio.h>
#include <atomic>
#include <thread>

#include "Iedk.h"
#include "IedkErrorCode.h"
#include "EmotivLicense.h"
#include <EmotivCloudClient.h>

#ifdef _WIN32
	#include <conio.h>
	#include <windows.h>
#endif

#if __linux__ || __APPLE__
	#include <unistd.h>
#endif

//--------------------------------------------------------------
namespace utf = boost::unit_test;


std::string profileName = "";
std::string pathOfProfile = "./profiles/test1.emu";
EmoEngineEventHandle eEvent = IEE_EmoEngineEventCreate();
EmoStateHandle eState = IEE_EmoStateCreate();
int engineUserID = -1;
int userCloudID = -1;
int profileID = -1;

std::string userName = "toannd";
std::string password = "3Motiv8ed";

/************** FUNCTIONS *******************/
void clean() {
	IEE_EngineDisconnect();
	IEE_EmoStateFree(eState);
	IEE_EmoEngineEventFree(eEvent);
}

/********************************************/

struct Fixture {
	//run before every test case start
	Fixture() {
	}

	//run after every test case finish
	~Fixture() {
		//BOOST_CHECK_MESSAGE(EC_Logout(userCloudID) == EDK_OK, "EC_Logout failed");
		//BOOST_CHECK_MESSAGE(EC_Disconnect() == EDK_OK, "ERROR: EC_Disconnect failed");
		//BOOST_CHECK_MESSAGE(IEE_EngineDisconnect() == EDK_OK, "ERROR: Engine Disconnect failed");

		//std::cout << "FIXTURE DESTROY - User Cloud ID = " << userCloudID << "\n";
	}
};

BOOST_FIXTURE_TEST_SUITE(EMOTIV_CLOUD_PROFILE, Fixture)

BOOST_FIXTURE_TEST_CASE(TC01_GIVEN_correct_EmotivID_WHEN_access_to_Emotiv_cloud_THEN_user_fail_to_login, Fixture) {
	BOOST_CHECK_MESSAGE(IEE_EngineConnect() == EDK_OK, "ERROR: IEE_EngineConnect failed");
	BOOST_CHECK_MESSAGE(EC_Connect() == EDK_OK, "ERROR: EC_Connect failed"); // Require dongle plugged-in
	BOOST_CHECK_MESSAGE(EC_Login(userName.c_str(), password.c_str()) == EDK_OK, "EC_Login failed");
	BOOST_CHECK_MESSAGE(EC_GetUserDetail(&userCloudID) == EDK_OK, "EC_GetUserDetail failed");
	BOOST_CHECK_MESSAGE(EC_GetAllProfileName(userCloudID) == EDK_OK, "EC_GetAllProfileName failed");
	BOOST_CHECK_MESSAGE(EC_GetProfileId(userCloudID, profileName.c_str(), &profileID) == EDK_OK, "EC_GetProfileId failed");
}

BOOST_FIXTURE_TEST_CASE(TC02_GIVEN_invalid_profile_WHEN_accessed_to_Emotiv_cloud_THEN_user_fail_to_download, Fixture) {
	std::string profileToDownload = "_invalid_profile_";
	BOOST_CHECK_MESSAGE(EC_DownloadProfileFile(userCloudID, profileID, pathOfProfile.c_str()) != EDK_OK, "EC_DownloadProfileFile failed");
}

BOOST_FIXTURE_TEST_CASE(TC03_GIVEN_invalid_profile_WHEN_accessed_to_Emotiv_cloud_THEN_user_fail_to_download, Fixture) {
	std::string profileToDownload = "test1";
	BOOST_CHECK_MESSAGE(EC_DownloadProfileFile(userCloudID, profileID, pathOfProfile.c_str()) == EDK_OK, "EC_DownloadProfileFile failed");
}

BOOST_AUTO_TEST_SUITE_END()
//--------------------------------------------------------------
// define module name
#define BOOST_TEST_MODULE SavingAndLoadingProfileCloud
// no need main function for the test
#define BOOST_TEST_NO_MAIN
// header for unit testin
#include <boost/test/auto_unit_test.hpp>

#include "Iedk.h"
#include "IedkErrorCode.h"
#include "EmotivCloudClient.h"
#include "IEmoStateDLL.h"
#include <sstream>
#include <stdio.h>
#include <stdlib.h>

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#include "Shlwapi.h"
#include <iostream>
#include <string>
#include "boost/filesystem.hpp"   // includes all needed Boost.Filesystem declarations
#include <iostream>               // for std::cout
// for ease of tutorial presentation;
//  a namespace alias is preferred practice in real code
#ifdef _WIN32
#include <conio.h>
#include <windows.h>
#endif
#if __linux__ || __APPLE__
#include <unistd.h>
#endif
//--------------------------------------------------------------
//khai bao bien
std::string userName = "binhe";
std::string password = "Vv12345678";
std::string profileName = "EmotivProfile";
std::string pathOfProfile = "C:\\binh.profile";
int version = -1; // Lastest version
int option = 0;
std::string input;
bool ready = false;
EmoEngineEventHandle eEvent = IEE_EmoEngineEventCreate();
EmoStateHandle eState = IEE_EmoStateCreate();
unsigned int engineUserID = -1;
int userCloudID = -1;
int state = 0;
int const SAVING = 0, LOADING = 1, UPLOAD = 2, DOWNLOAD = 3, EC_CONNECT = 4, EC_LOGIN = 5;
bool overwrite_if_exists = true;

int result = EDK_OK;
int add(int i, int j) {
	return 0;
}
namespace utf = boost::unit_test;
/****************************/
/*Khoi tao truoc khi test
yeu cau engineconnect
va cloud connect thanh cong*/
int init_IEE_EngineConnect() {
	//khoi tao EmoEngineEvent
	result = -1;
	result = IEE_EngineConnect();
	return result;
}

int init_EC_Connect_Login_EC_GetUserDetail() {
	//connect to cloud

	result = EC_Connect();
	if (result != EDK_OK) return result;
	//login to cloud
	result = EC_Login(userName.c_str(), password.c_str());
	std::cout << "Logged in as " << userName << std::endl;
	//getusercloudID
	result = EC_GetUserDetail(&userCloudID);
	std::cout << " init_test_condition()end" << std::endl;
	return 0;
}


bool add_user_required() {

	bool ready = false;
	std::cout << " add_user_required:" << std::endl;
	while (ready == false)
	{
		state = IEE_EngineGetNextEvent(eEvent);
		if (state == EDK_OK) {
			//check xem da cam dongle vao hay chua
			IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
			IEE_EmoEngineEventGetUserId(eEvent, &engineUserID);
			//doi den khi ready ->dongle da duoc cam vao may tinh 
			if (eventType == IEE_UserAdded) {
				std::cout << "User added" << std::endl;
				ready = true;
			}
		}
	}
	std::cout << " add_user_required():end" << std::endl;
	return ready;
}

void print(std::string a) {

	std::cout << std::endl << a << std::endl;
}

void closeAll() {

	IEE_EngineDisconnect();
	IEE_EmoStateFree(eState);
	IEE_EmoEngineEventFree(eEvent);
}
struct F {
	F() {
		init_IEE_EngineConnect();
		init_EC_Connect_Login_EC_GetUserDetail();
		add_user_required();
	}
};
/* 1. kiem tra xem donggle da duoc add hay chua
sau do thuc hien save,update, upload dơnload*/
/*********************************************/
int add_user_required_then_do_task(int TaskName) {
	ready = false;

	int count = 0;
	while (ready == false)
	{
		/*	count++;
		std::cout << count << std::endl;*/
		state = IEE_EngineGetNextEvent(eEvent);

		if (state == EDK_OK) {
			//check xem da cam dongle vao hay chua
			IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
			IEE_EmoEngineEventGetUserId(eEvent, &engineUserID);
			//doi den khi ready ->dongle da duoc cam vao may tinh 
			if (eventType == IEE_UserAdded) {
				std::cout << "User added" << std::endl;
				ready = true;
			}
		}

		if (ready)
		{
			//int getNumberProfile = EC_GetAllProfileName(userCloudID);
			//	profileName = "h";
			//	std::cout << "profile name:= " << profileName<<std::endl;

			//	// E:/profile.emu
			//	std::stringstream ss;
			//	ss<<"C:\\"<< profileName;
			//	pathOfProfile = ss.str();
			//	
			//	std::cout << "profile path to save:= "<<pathOfProfile << std::endl;

			int profileID = -1;
			switch (TaskName) {
			case DOWNLOAD: {

				result = EC_GetProfileId(userCloudID, profileName.c_str(), &profileID);

				if (profileID >= 0) {
					result = EC_DownloadProfileFile(userCloudID, profileID, pathOfProfile.c_str());

					break;

				}
			case UPLOAD: {

				result = EC_UploadProfileFile(userCloudID, profileName.c_str(), pathOfProfile.c_str(), TRAINING, overwrite_if_exists);

				break;

			}
			case SAVING: {


				result = EC_GetProfileId(userCloudID, profileName.c_str(), &profileID);

				if (profileID >= 0) {//ko lam gi
									 /*std::cout << "Profile with " << profileName << " is existed" << std::endl;
									 result = EC_UpdateUserProfile(userCloudID, engineUserID, profileID);

									 if (result == EDK_OK) {
									 std::cout << "Updating finished";
									 }
									 else std::cout << "Updating failed";*/
				}
				else {
					result = EC_SaveUserProfile(userCloudID, (int)engineUserID, profileName.c_str(), TRAINING);

					/*if (result == EDK_OK)
					{
					std::cout << "Saving finished";
					}
					else std::cout << "Saving failed";*/
				}


				break;

			}

			case LOADING: {




				break;
			}
			default:
				std::cout << "Invalid option...";
				break;


			}


			}


		}
		/*if (!boost::filesystem::exists(pathOfProfile))
		{
		return -1;
		}
		else {
		return EDK_OK;
		}*/
	}
	return 0;
}

int check_profile_exist(std::string profileName) {
	int profileID = -1;
	result = EC_GetProfileId(userCloudID, profileName.c_str(), &profileID);
	return profileID;
}
//feature define
//struct Fixture_Profile {
//
//	//run before every test case start
//	Fixture_Profile(){
//		BOOST_TEST_MESSAGE("run before every test case start");
//	}
//
//	//run after every test case finish
//	~Fixture_Profile(){
//		BOOST_TEST_MESSAGE("run after every test case finish");
//	}
//};


/***************************************/
bool const save_suite = false;
// start suite
//test this function EC_SaveUserProfile(userCloudID, (int)engineUserID, profileName.c_str(), TRAINING);
BOOST_AUTO_TEST_SUITE(EC_SaveUserProfile_TC, *utf::enable_if<save_suite>())

// start test case
BOOST_FIXTURE_TEST_CASE(TC_01_profileName_not_exist_when_save_then_profile_is_created_in_cloud, F)
{
	//init_IEE_EngineConnect();
	//init_EC_Connect_Login_EC_GetUserDetail();
	// message out


	std::cout << "TC_01_profileName_not_exist_when_save_then_profile_is_created_in_cloud" << std::endl;
	// check
	std::cout << "Enter new profile Name make sure it is not exist in cloud" << std::endl;
	std::cin >> profileName;
	//add_user_required_then_do_task(SAVING);
	//BOOST_CHECK();
	BOOST_CHECK(EC_SaveUserProfile(userCloudID, (int)engineUserID, profileName.c_str(), TRAINING) == EDK_OK);
	int kq = check_profile_exist(profileName);
	std::cout << "End of tc01 saving kq=" << kq << std::endl;
}

// start test case: profile exist in the cloud
BOOST_AUTO_TEST_CASE(TC02_profilename_exist_when_save_then_not_save, *utf::enable_if<save_suite>())
{
	std::cout << "TC02_profilename_exist_when_save_then_no_save" << std::endl;
	std::cout << "Enter profile name making sure that it is exists in the Emotive cloud:" << std::endl;
	std::cin >> profileName;
	int profileID = -1;
	BOOST_CHECK(EC_SaveUserProfile(userCloudID, (int)engineUserID, profileName.c_str(), TRAINING) != EDK_OK);

	//std::cout << ":" << EC_SaveUserProfile(userCloudID, (int)engineUserID, profileName.c_str(), TRAINING)<< std::endl;
}
// profile name is null;
BOOST_AUTO_TEST_CASE(TC03_profilename_null_when_save_then_not_save)
{
	std::cout << "TC03_profilename_null_when_save_then_not_save" << std::endl;
	profileName = "";
	BOOST_CHECK(EC_SaveUserProfile(userCloudID, (int)engineUserID, profileName.c_str(), TRAINING) != EDK_OK);

	//std::cout << ":" << EC_SaveUserProfile(userCloudID, (int)engineUserID, profileName.c_str(), TRAINING)<< std::endl;
}

//profile name is too long
BOOST_AUTO_TEST_CASE(TC04_profilename_too_long_when_save_then_not_save)
{
	std::cout << "TC04_profilename_is_too_long_when_save_then_not_save" << std::endl;
	// create a very very long profile name
	profileName = "x";
	srand(time(NULL));
	std::string s = "";
	for (int i = 0; i <= 20; i++) {
		profileName += std::to_string(rand() % 30000);
	}
	print(profileName);
	BOOST_CHECK(EC_SaveUserProfile(userCloudID, (int)engineUserID, profileName.c_str(), TRAINING) != EDK_OK);

	//std::cout << ":" << EC_SaveUserProfile(userCloudID, (int)engineUserID, profileName.c_str(), TRAINING)<< std::endl;
}

//userCloudID is invalid
BOOST_AUTO_TEST_CASE(TC05_userCloudID_invalid_when_save_then_not_OK)
{
	//create profilename that is not exist in cloud (99,9999999999%)
	std::cout << std::endl << "TC05_userCloudID_invalid_when_save_then_not_OK" << std::endl;
	userCloudID = -1;

	profileName = "a";
	std::string s = std::to_string(rand() % 32767) + std::to_string(rand() % 32767);
	profileName += s;


	BOOST_CHECK(EC_SaveUserProfile(-1, (int)engineUserID, profileName.c_str(), TRAINING) != EDK_OK);

	//std::cout << ":" << EC_SaveUserProfile(userCloudID, (int)engineUserID, profileName.c_str(), TRAINING)<< std::endl;
}
// end of test suite
BOOST_AUTO_TEST_SUITE_END()

////
////Loading
//BOOST_AUTO_TEST_SUITE(loading_profile)
//
//// start test case
//BOOST_AUTO_TEST_CASE(loading_profile_01)
//{
//	// message out
//	BOOST_TEST_MESSAGE("check add(2, 2) == 4");
//	// check
//	BOOST_CHECK(add(2, 2) == 4); //#1 continues on error
//}
//
//// start test case
//BOOST_AUTO_TEST_CASE(loading_profile_02)
//{
//	BOOST_CHECK(add(0, 2) == 2); //#1 continues on error
//}

//
//// start test case
//BOOST_AUTO_TEST_CASE(loading_profile_03)
//{
//	BOOST_CHECK(add(0, 0) == 0); //#1 continues on error
//}
//
//// start test case
//BOOST_AUTO_TEST_CASE(loading_profile_04)
//{
//	// multiple check in one case
//	BOOST_CHECK(add(2, 2) == 4); //#1 continues on error
//	BOOST_CHECK(add(0, 2) == 2); //#1 continues on error
//	BOOST_CHECK(add(0, 0) == 0); //#1 continues on error
//}
//
//BOOST_AUTO_TEST_SUITE_END()
//




///***********************************************/
//upload profile
bool const upload_suite = false;
BOOST_AUTO_TEST_SUITE(upload_profile_suite, *utf::enable_if<upload_suite>())

// start test case 
/*
profile not exist that means you upload new profile -> new profile is created in the cloud with the name you enter
then you upload data into it from file data to upload existed in a file on the local( c:\\binh.pro for example);
*/
bool const upload_suite = true;
BOOST_AUTO_TEST_CASE(TC_upload_01_valid_data_when_upload_then_success)
{
	print("TC_upload_01_valid_data_when_upload_then_success");
	std::cout << "Enter profile name not exist in the cloud: " << std::endl;
	std::getline(std::cin, profileName);
	pathOfProfile = "C:\\binh.pro";
	bool overwrite_if_exists = true;
	int result1 = -1;
	result1 = EC_UploadProfileFile(userCloudID, profileName.c_str(), pathOfProfile.c_str(), TRAINING, overwrite_if_exists);
	//check cloud xem đã tạo profile chua
	int result2 = -1;
	result2 = check_profile_exist(profileName);
	BOOST_CHECK((result1 == EDK_OK) && (result2 > 0));
}


// start test case
//profile exist; 
BOOST_AUTO_TEST_CASE(TC_upload_02_profile_is_exist)
{
	print("TC_upload_02_profile_is_exist");
	print("Enter_profile_exist_in_cloud");
	std::cout << "profile name: " << std::endl;
	std::getline(std::cin, profileName);
	pathOfProfile = "C:\\binh.pro";
	//
	bool overwrite_if_exists = true;
	int result1 = -1;
	result1 = EC_UploadProfileFile(userCloudID, profileName.c_str(), pathOfProfile.c_str(), TRAINING, overwrite_if_exists);
	//check cloud xem đã tạo profile chua
	BOOST_CHECK((result1 == EDK_OK));
	//TC_upload_01_valid_data_when_upload_then_success
	//
}

// start test case
BOOST_AUTO_TEST_CASE(TC_Upload_03_pathOfprofile_not_exist)
{
	BOOST_CHECK(add(0, 0) == 0); //#1 continues on error
}

// start test case
BOOST_AUTO_TEST_CASE(upload_profile_04)
{
	// multiple check in one case
	BOOST_CHECK(add(2, 2) == 4); //#1 continues on error
	BOOST_CHECK(add(0, 2) == 2); //#1 continues on error
	BOOST_CHECK(add(0, 0) == 0); //#1 continues on error
}

BOOST_AUTO_TEST_SUITE_END()

///**************************************/

//download profile
BOOST_AUTO_TEST_SUITE(Download_profile)

// start test case
struct FIXTURE_DOWNLOAD {
	int i;
};

bool const download_suite = false;
BOOST_FIXTURE_TEST_CASE(TC01_profile_exist_when_download_then_get_profile_successfully, F,
	*utf::enable_if<download_suite>())
{
	//init_IEE_EngineConnect();
	//init_EC_Connect_Login_EC_GetUserDetail();
	// message out
	//BOOST_TEST_MESSAGE("check add(2, 2) == 4");
	// checkopenn
	//BOOST_CHECK(add_user_required_then_do_task(DOWNLOAD)== EDK_OK); /pro/#1 continues on error
	print("TC01_profile_exist_when_download_then_get_profile_successfully");
	print("Enter profilenam make sure that it is exist in the acc you login");
	std::cin >> profileName;
	int profileID = -1;
	pathOfProfile = "c:\\binh";
	EC_GetProfileId(userCloudID, profileName.c_str(), &profileID);

	if (profileID >= 0) {
		BOOST_CHECK(EC_DownloadProfileFile(userCloudID, profileID, pathOfProfile.c_str()) == EDK_OK);
	}
	else {
		print("the profileName you have enter is not exist");
	}
}

BOOST_AUTO_TEST_CASE(TC02_profile_not_exist_when_download_then_get_profile_fail)
{
	print("TC02_profile_not_exist_when_download_then_get_profile_fail");
	print("Enter profilenam make sure that it is not exist in the acc you login");
	pathOfProfile = "c:\\binh";
	std::cin >> profileName;
	int profileID = -1;
	result = EC_GetProfileId(userCloudID, profileName.c_str(), &profileID);

	if (result == -1) {

		BOOST_CHECK(EC_DownloadProfileFile(userCloudID, profileID, pathOfProfile.c_str()) != EDK_OK);
	}
	else {
		print("the profileName you have enter is not exist");
	}
}

// start test case
/* file to save profile to is opening*/
BOOST_AUTO_TEST_CASE(TC03_profileName_exist_but_file_to_save_is_opening_when_download_then_fail)
{
	// c:\\binh.pro is opening
	print("TC03_profileName_exist_but_file_to_save_is_opening_when_download_then_fail");
	print("Enter profilenam exist in the acc you have logined");
	std::cin >> profileName;
	int profileID = -1;
	pathOfProfile = "c:\\binh.pro";
	//get profileID
	EC_GetProfileId(userCloudID, profileName.c_str(), &profileID);
	if (profileID >= 0) {
		BOOST_CHECK(EC_DownloadProfileFile(userCloudID, profileID, pathOfProfile.c_str()) != EDK_OK);
		_getch();
	}
	else {
		print("the profileName you have enter is not exist");
	}
}

BOOST_AUTO_TEST_SUITE_END()

#ifdef __linux__
int _kbhit(void)
{
	struct timeval tv;
	fd_set read_fd;

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	FD_ZERO(&read_fd);
	FD_SET(0, &read_fd);

	if (select(1, &read_fd, NULL, NULL, &tv) == -1)
		return 0;

	if (FD_ISSET(0, &read_fd))
		return 1;

	return 0;
}
#endif
#ifdef __APPLE__
int _kbhit(void)
{
	struct timeval tv;
	fd_set rdfs;

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	FD_ZERO(&rdfs);
	FD_SET(STDIN_FILENO, &rdfs);

	select(STDIN_FILENO + 1, &rdfs, NULL, NULL, &tv);
	return FD_ISSET(STDIN_FILENO, &rdfs);
}
#endif
//
//// start suite
//BOOST_AUTO_TEST_SUITE(add_sui)
//
//// start test case
//BOOST_AUTO_TEST_CASE(add_test_two_number_are_not_0)
//{	
//	// message out
//	BOOST_TEST_MESSAGE("check add(2, 2) == 4");
//	// check
//	BOOST_CHECK(add(2, 2) == 4); //#1 continues on error
//}
//
//// start test case
//BOOST_AUTO_TEST_CASE(add_test_one_of_two_number_is_0)
//{
//	BOOST_CHECK(add(0, 2) == 2); //#1 continues on error
//}
//
//// start test case
//BOOST_AUTO_TEST_CASE(add_test_two_number_are_0)
//{
//	BOOST_CHECK(add(0, 0) == 0); //#1 continues on error
//}
//
//// start test case
//BOOST_AUTO_TEST_CASE(add_test_multiple_check)
//{
//	// multiple check in one case
//	BOOST_CHECK(add(2, 2) == 4); //#1 continues on error
//	BOOST_CHECK(add(0, 2) == 2); //#1 continues on error
//	BOOST_CHECK(add(0, 0) == 0); //#1 continues on error
//}
//
//// end of test suite
//BOOST_AUTO_TEST_SUITE_END()
//
////--------------------------------------------------------------
//
//int minus(int i, int j) { return i - j; }
//
//BOOST_AUTO_TEST_SUITE(minus_suite)
//
//BOOST_AUTO_TEST_CASE(add_test_multiple_check)
//{
//	// multiple check in one case
//	BOOST_CHECK(minus(2, 2) == 0); //#1 continues on error
//	BOOST_CHECK(minus(0, 2) == -2); //#1 continues on error
//	BOOST_CHECK(minus(0, 0) == 0); //#1 continues on error
//}
//
//BOOST_AUTO_TEST_SUITE_END()
//
//
////--------------------------------------------------------------
////function
//int multiple(int i, int j) { return i*j; }
//
//
////feature define
//struct Fixture_Demo {
//
//	//run before every test case start
//	Fixture_Demo(){
//		BOOST_TEST_MESSAGE("run before every test case start");
//	}
//
//	//run after every test case finish
//	~Fixture_Demo(){
//		BOOST_TEST_MESSAGE("run after every test case finish");
//	}
//};
//
////define and bind suite to fixture at one macrco
//BOOST_FIXTURE_TEST_SUITE(multiple_suite, Fixture_Demo)
//
////test case
//BOOST_AUTO_TEST_CASE(multiple_test)
//{	
//	BOOST_TEST_MESSAGE("test multiple two integer number");
//	//BOOST_CHECK(multiple(2, 2) == 4);
//	BOOST_TEST(multiple(2, 2) == 4);
//	int a;
//	std::cin >> a;
//}
//
////end of suite
//BOOST_AUTO_TEST_SUITE_END()

/*

Running command
// run test and output report in xml file
Add_Boots_Test.exe --log_level=message  --report_format=XML --log_sink=out.xml

*/
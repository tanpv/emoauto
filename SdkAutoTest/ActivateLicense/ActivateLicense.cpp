/****************************************************************************
**
** Copyright 2015 by Emotiv. All rights reserved
** Example - ActivateLicense
** How to activate a license key and Get infomation of that license
**
****************************************************************************/
#define BOOST_TEST_MODULE ACTIVE_LICENSE
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

/**
 * License keys:
 */

// EEG, PM 2 seat
std::string const validKeyEP2 = "088413c1-1d87-4f59-81a6-9fbf8a206911";

namespace utf = boost::unit_test;
std::atomic<bool> isTimeout(false);

IEE_LicenseInfos_t licenseInfos;
int errorCode;
int withdrawSession;

// Fixture define, leave blank for future implements
struct Fixture {
	//run before every test case start
	Fixture() {
		IEE_LicenseInformation(&licenseInfos);
		std::cout << "Scope: " << std::dec << licenseInfos.scopes << "\n";
		std::cout << "Date From: " << std::dec << licenseInfos.date_from << "\n";
		std::cout << "Used quota: " << std::dec << licenseInfos.usedQuota << "\n";
	}

	//run after every test case finish
	~Fixture() {
	}


};

BOOST_FIXTURE_TEST_SUITE(ACTIVE_LICENSE, Fixture)

// REQUIRE CASE
BOOST_AUTO_TEST_CASE(TC0_CONNECT_TO_EMOTIV) {
	std::string const userName = "toannd";
	std::string const password = "3Motiv8ed";

	// Connect headset - required
	BOOST_CHECK_MESSAGE(IEE_EngineConnect() == EDK_OK, "ERROR: Engine Connect failed");
	// Connect Emotiv Cloud
	BOOST_CHECK_MESSAGE(EC_Connect() == EDK_OK, "ERROR: EC_Connect failed");
	// Login to Emotiv Cloud
	BOOST_CHECK_MESSAGE(EC_Login(userName.c_str(), password.c_str()) == EDK_OK, "EC_Login failed");


}

BOOST_AUTO_TEST_CASE(TC1_GIVEN_a_valid_license_WHEN_input_invalid_grant_number_THEN_user_could_not_activate_license) {
	// Feed grant number from -100 to -1 to IEE_ActivateLicense()
	for (auto i = -5; i < 0; i++) {
		errorCode = IEE_ActivateLicense(validKeyEP2.c_str(), i);
		BOOST_CHECK_MESSAGE(errorCode != EDK_OK, "ERROR: License activated with grant number = " << i << " - Error Code = " << std::hex << errorCode);
	}
}

BOOST_AUTO_TEST_CASE(TC2_GIVEN_a_valid_license_WHEN_input_0_as_grant_number_THEN_user_could_activate_license_successfully) {
	
	errorCode = IEE_ActivateLicense(validKeyEP2.c_str(), 0);
	BOOST_CHECK_MESSAGE(errorCode == EDK_OK, "ERROR: Unable to withdraw " << 0 << " session - Error Code = " << std::hex << errorCode);
	
	IEE_LicenseInformation(&licenseInfos);
	auto const cloudQuota = licenseInfos.remainingSession;
	BOOST_TEST_MESSAGE("sessionRemaining = " << std::dec << cloudQuota);
}

// This test case cannot verify total session got from cloud due to API's defect
BOOST_AUTO_TEST_CASE(TC3_GIVEN_a_valid_license_WHEN_input_1_as_grant_number_THEN_user_could_activate_license_successfully) {
	// Grant 0 session just for activate license to refresh cloud quota
	//auto errorCode = IEE_ActivateLicense(validKeyEP2.c_str(), 0);
	//BOOST_CHECK_MESSAGE(errorCode == EDK_OK, "ERROR: License activated with grant number = " << 0 << " - Error Code = " << std::hex << errorCode);

	// Get infomation from activated license
	//errorCode = IEE_LicenseInformation(&licenseInfos);
	//auto const quotaBeforeWithdraw = licenseInfos.quota;
	//BOOST_TEST_MESSAGE("PC Quota = " << quotaBeforeWithdraw);
	//BOOST_CHECK_MESSAGE(errorCode == EDK_OK, "ERROR: Unable to get information from license - Error Code = " << std::hex << errorCode);
	//auto const withdrawSession = 1;
	// Withraw 1 session from cloud
	BOOST_TEST_MESSAGE("Getting 1 session from Cloud");
	Sleep(10000);
	errorCode = IEE_ActivateLicense(validKeyEP2.c_str(), withdrawSession);
	BOOST_CHECK_MESSAGE(errorCode == EDK_OK, "ERROR: Unable to withraw session from cloud - Error Code = " << std::hex << errorCode);
	// Check quota after withdaw
	//IEE_LicenseInformation(&licenseInfos);
	//auto const newQuota = licenseInfos.quota;
	//BOOST_CHECK_MESSAGE(newQuota == (quotaBeforeWithdraw + withdrawSession), "Quota number error mismatch");
}

// This test case cannot verify total session got from cloud due to API's defect
BOOST_AUTO_TEST_CASE(TC4_GIVEN_a_valid_license_WHEN_input_10_as_grant_number_THEN_user_could_activate_license) {
//	// Grant 0 session just for activate license
//	//auto errorCode = IEE_ActivateLicense(validKeyEP2.c_str(), 0);
//	//BOOST_CHECK_MESSAGE(errorCode == EDK_OK, "ERROR: License activated with grant number = " << 0 << " - Error Code = " << std::hex << errorCode);
//	//Sleep(10000);
//
//	// Get infomation from activated license
//	errorCode = IEE_LicenseInformation(&licenseInfos);
//	auto quotaBeforeWithdraw = licenseInfos.quota;
//	BOOST_CHECK_MESSAGE(errorCode == EDK_OK, "ERROR: Unable to get information from license - Error Code = " << std::hex << errorCode);
//	
//
//	auto withdrawSession = 10;
//	// Withraw 10 session from cloud
//	errorCode = IEE_ActivateLicense(validKeyEP2.c_str(), withdrawSession);
//	auto cloudQuota = licenseInfos.sessionRemaining;
//
//
//	if (withdrawSession <= cloudQuota) {
//		BOOST_CHECK_MESSAGE(errorCode == EDK_OK, "ERROR: Unable to withraw session from cloud - Error Code = " << std::hex << errorCode);
//		// Check quota after withdaw
//		IEE_LicenseInformation(&licenseInfos);
//		auto newQuota = licenseInfos.quota;
//		BOOST_CHECK_MESSAGE(newQuota == (quotaBeforeWithdraw + withdrawSession), "Quota number error mismatch");
//	}
//	else {
//		BOOST_CHECK_MESSAGE(errorCode != EDK_OK, "ERROR: Able to withdraw invalid quota - Error Code = " << std::hex << errorCode);
//	}
	BOOST_TEST_MESSAGE("Getting 1 session from Cloud");
	withdrawSession = 10;
	Sleep(10000);
	errorCode = IEE_ActivateLicense(validKeyEP2.c_str(), withdrawSession);
	BOOST_CHECK_MESSAGE(errorCode == EDK_OK, "ERROR: Unable to withraw " << withdrawSession << " session from cloud - Error Code = " << std::hex << errorCode);
}

BOOST_AUTO_TEST_CASE(TC5_GIVEN_a_valid_license_WHEN_input_grant_number_more_than_cloud_remain_THEN_user_could_not_activate_license) {
	// Grant 0 session just for activate license
	//auto errorCode = IEE_ActivateLicense(validKeyEP2.c_str(), 0);
	//BOOST_CHECK_MESSAGE(errorCode == EDK_OK, "ERROR: License activated with grant number = " << 0 << " - Error Code = " << std::hex << errorCode);
	//Sleep(10000);

	// Get infomation from activated license
	//errorCode = IEE_LicenseInformation(&licenseInfos);
	//BOOST_CHECK_MESSAGE(errorCode == EDK_OK, "ERROR: Unable to get information from license - Error Code = " << std::hex << errorCode);

	auto cloudQuota = licenseInfos.remainingSession;
	//for (auto i = 0; i < 5; i++) {
	//	errorCode = IEE_ActivateLicense(validKeyEP2.c_str(), cloudQuota + i);
	//	BOOST_CHECK_MESSAGE(errorCode != EDK_OK, "ERROR: Able to withraw more available session");
	//}
	withdrawSession = cloudQuota + 1;
	errorCode = IEE_ActivateLicense(validKeyEP2.c_str(), withdrawSession);
	BOOST_CHECK_MESSAGE(errorCode != EDK_OK, "ERROR: Able to withraw more available session from CLOUD");
}

BOOST_AUTO_TEST_SUITE_END()

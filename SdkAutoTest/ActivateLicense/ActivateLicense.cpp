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

/*
#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif
*/

#include <iostream>
#include <sstream>
#include <time.h>
#include <stdio.h>
#include <ctime>

#include "Iedk.h"
#include "IedkErrorCode.h"

std::string edfFile = "../../edf/t.edf";
//defined license_key in testcase
// case EDK_LICENSE_NOT_FOUND
std::string const LICENSE_KEY_NOT_FOUND = "11b9d092-db6b-4f18-b7b4-04d640110d4c";
//case EDK_LICENSE_ERROR
std::string const LICENSE_KEY_ERROR = "50541929-7330-4059-b073-30814ff15ea2";
// case EDK_LICENSE_EXPIRED
std::string const LICENSE_KEY_EXPIRED = "a1b9d092-db6b-4f18-b7b4-04d640110d4c";
//case EDK_LICENSE_REGISTERED
std::string const LICENSE_KEY_VALID = "50541929-7330-4059-b073-30814ff15ea2";
//case EDK_LICENSE_DEVICE_LIMITED
std::string const LICENSE_KEY_DEVICE_LIMITED = "5e9f73e0-3aab-4bc7-9477-9838c3c0eb23";
//case EDK_UNKNOWN_ERROR
std::string const UNKNOWN_ERROR = "50541929-7330-4059-b073-30814ff15ea2";
//case EDK_OVER_QUOTA_IN_DAY
std::string const LICENSE_KEY_OVER_QUOTA_IN_DAY = "50541929-7330-4059-b073-30814ff15ea2";
//case EDK_OVER_QUOTA_IN_MONTH
std::string const LICENSE_KEY_OVER_QUOTA_IN_MONTH = "50541929-7330-4059-b073-30814ff15ea2";
//case EDK_OVER_QUOTA
std::string const LICENSE_KEY_OVER_QUOTA = "50541929-7330-4059-b073-30814ff15ea2";
//case EDK_ACCESS_DENIED
std::string const LICENSE_KEY_ACCESS_DENIED = "50541929-7330-4059-b073-30814ff15ea2";
//case EDK_LICENSE_ERROR
std::string const LICENSE_KEY_LICENSE_ERROR = "50541929-7330-4059-b073-30814ff15ea2";
//case EDK_NO_ACTIVE_LICENSE
std::string const LICENSE_KEY_NO_ACTIVE_LICENSE = "50541929-7330-4059-b073-30814ff15ea2";

BOOST_AUTO_TEST_SUITE(ACTIVE_LICENSE)

BOOST_AUTO_TEST_CASE(TC1_GIVEN_have_a_LICENSE_KEY_EXPIRED_WHEN_server_is_up_THEN_user_could_not_activate_license) {

	int result;
	result = IEE_ActivateLicense(LICENSE_KEY_EXPIRED.c_str());
	BOOST_CHECK(result == EDK_LICENSE_EXPIRED);
}



BOOST_AUTO_TEST_CASE(TC2_GIVEN_have_a_valid_license_WHEN_server_is_up_THEN_user_could_activate_license_successfully) {
	
	int result;
	result = IEE_ActivateLicense(LICENSE_KEY_VALID.c_str());
	bool test_result = false;

	if(result == EDK_OK)
	{
		BOOST_TEST_MESSAGE("Activate license OK");
		test_result = true;
	}

	if (result == EDK_LICENSE_REGISTERED)
	{
		BOOST_TEST_MESSAGE("License already registered");
		test_result = true;
	}

	BOOST_CHECK(test_result == true);
}


// Activate license
// Use license for 5 times, so license run out of quota
// Connect to emoengine
// Expect the return is over quota in day

BOOST_AUTO_TEST_CASE(TC3_GIVEN_have_a_valid_license_with_1_seat_WHEN_after_use_5_sessions_THEN_user_should_get_message_run_out_of_quota) {
	
	int result;
	result = IEE_ActivateLicense(LICENSE_KEY_OVER_QUOTA_IN_DAY.c_str());
	bool test_result = false;

	if (result == EDK_OK)
	{
		BOOST_TEST_MESSAGE("Activate license OK");
		test_result = true;
	}

	if (result == EDK_LICENSE_REGISTERED)
	{
		BOOST_TEST_MESSAGE("License already registered");
		test_result = true;
	}

	BOOST_CHECK(test_result == true);

}


BOOST_AUTO_TEST_CASE(TC4_GIVEN_have_a_LICENSE_KEY_OVER_QUOTA_IN_MONTH_WHEN_server_is_up_THEN_user_could_not_activate_license) {
	int result;
	result = IEE_ActivateLicense(LICENSE_KEY_OVER_QUOTA_IN_MONTH.c_str());
	BOOST_CHECK(result== EDK_OVER_QUOTA_IN_MONTH);
	IEE_LicenseInfos_t licenseInfos;
	// We can call this API any time to check current License information
	result = IEE_LicenseInformation(&licenseInfos);
	BOOST_CHECK(result== EDK_OVER_QUOTA_IN_MONTH);
	if (result==EDK_OK) {
		BOOST_TEST_MESSAGE("Quota limit in month: "<< (int)licenseInfos.quotaMonthLimit);
		BOOST_TEST_MESSAGE("Quota used in month : " << (int)licenseInfos.usedQuotaMonth);
	}
		//BOOST_TEST_MESSAGE("Total Quota Remain in day: " + remainQuotaMonth);
	}
}

BOOST_AUTO_TEST_CASE(TC5_GIVEN_have_a_LICENSE_KEY_OVER_QUOTA_WHEN_server_is_up_THEN_user_could_not_activate_license) {
	int result;
	result = IEE_ActivateLicense(LICENSE_KEY_OVER_QUOTA.c_str());
	BOOST_CHECK_EQUAL(result, EDK_OVER_QUOTA);
	
	
}
BOOST_AUTO_TEST_CASE(TC6_GIVEN_have_a_LICENSE_KEY_DEVICE_LIMITED_WHEN_server_is_up_THEN_user_could_not_activate_license) {
	int result;
	result = IEE_ActivateLicense(LICENSE_KEY_DEVICE_LIMITED.c_str());
	BOOST_CHECK_EQUAL(result, EDK_LICENSE_DEVICE_LIMITED);

}
BOOST_AUTO_TEST_CASE(TC7_GIVEN_have_a_LICENSE_KEY_NOT_FOUND_WHEN_server_is_up_THEN_user_could_not_activate_license) {
	int result;
	result = IEE_ActivateLicense(LICENSE_KEY_NOT_FOUND.c_str());
	BOOST_CHECK_EQUAL(result, EDK_LICENSE_NOT_FOUND);

}




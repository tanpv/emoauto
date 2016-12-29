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

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#endif

#include <iostream>
#include <sstream>
#include <time.h>
#include <stdio.h>
#include <ctime>

#include "Iedk.h"
#include "IedkErrorCode.h"


std::string convertEpochToTime(time_t epochTime, std::string format = "%Y-%m-%d %H:%M:%S");
std::string convertEpochToTime(time_t epochTime, std::string format)
{
	if (format == "")
		format = "%Y-%m-%d %H:%M:%S";

	struct tm timeinfo;
	localtime_s(&timeinfo, &epochTime);

	char timestamp[64] = { 0 };
	strftime(timestamp, sizeof(timestamp), format.c_str(), &timeinfo);
	return timestamp;
}

std::string intToHex(int x)
{
	std::stringstream stream;
	stream << std::hex << x;
	std::string result(stream.str());
	result = "0x" + result;

	return result;
}

void printLicenseInformation(IEE_LicenseInfos_t& licenseInfos)
{
	int licenseType = 0;

	std::cout << std::endl;
	std::cout << "Date From  : " << convertEpochToTime(licenseInfos.date_from) << std::endl;
	std::cout << "Date To    : " << convertEpochToTime(licenseInfos.date_to) << std::endl;
	std::cout << std::endl;

	std::cout << "Seat number: " << licenseInfos.seat_count << std::endl;
	std::cout << std::endl;

	std::cout << "Total Quota: " << licenseInfos.quota << std::endl;
	std::cout << "Total quota used    : " << licenseInfos.usedQuota << std::endl;
	std::cout << std::endl;

	std::cout << "Quota limit in day  : " << licenseInfos.quotaDayLimit << std::endl;
	std::cout << "Quota used in day   : " << licenseInfos.usedQuotaDay << std::endl;
	std::cout << std::endl;

	std::cout << "Quota limit in month: " << licenseInfos.quotaMonthLimit << std::endl;
	std::cout << "Quota used in month : " << licenseInfos.usedQuotaMonth << std::endl;
	std::cout << std::endl;

	switch (licenseInfos.scopes)
	{
	case IEE_EEG:
		licenseType = IEE_LicenseType_t::IEE_EEG;

		std::cout << "License type : " << "EEG" << std::endl;
		std::cout << std::endl;
		break;
	case IEE_EEG_PM:
		licenseType = IEE_LicenseType_t::IEE_EEG_PM;

		std::cout << "License type : " << "EEG + PM" << std::endl;
		std::cout << std::endl;
		break;
	case IEE_PM:
		licenseType = IEE_LicenseType_t::IEE_PM;
		std::cout << "License type : " << "PM" << std::endl;
		std::cout << std::endl;
		break;
	default:
		std::cout << "License type : " << "No type" << std::endl;
		std::cout << std::endl;
		break;
	}
}
std::string LICENSE_KEY;
//feature define
struct Fixture_Add_Licensekey {
	Fixture_Add_Licensekey() {
		BOOST_TEST_MESSAGE("INPUT LICENSE_KEY:");
		std::getline(std::cin, LICENSE_KEY, '\n');
	}

	//run after every test case finish
	~Fixture_Add_Licensekey() {
		BOOST_TEST_MESSAGE("run after every test case finish");
	}
};
BOOST_FIXTURE_TEST_SUITE(CHECK_ACTIVE_LICENSE, Fixture_Add_Licensekey)
BOOST_AUTO_TEST_CASE(GIVEN_have_a_valid_license_WHEN_server_is_up_THEN_user_could_activate_license) {
	int result = EDK_OK;
	result = IEE_ActivateLicense(LICENSE_KEY.c_str());
	BOOST_CHECK_EQUAL(result, EDK_LICENSE_REGISTERED);
	switch (result)
	{
	case EDK_LICENSE_NOT_FOUND:
		BOOST_FAIL("EDK_LICENSE_NOT_FOUND");
		break;
	case EDK_LICENSE_ERROR:
		BOOST_FAIL("EDK_LICENSE_ERROR");
		break;
	case EDK_LICENSE_EXPIRED:
		BOOST_FAIL("EDK_LICENSE_EXPIRED");
		break;
	case EDK_LICENSE_REGISTERED:
		BOOST_TEST_MESSAGE("EDK_LICENSE_REGISTERED");
		break;
	case EDK_LICENSE_DEVICE_LIMITED:
		BOOST_FAIL("EDK_LICENSE_DEVICE_LIMITED");
		break;
	case EDK_UNKNOWN_ERROR:
		BOOST_FAIL("EDK_UNKNOWN_ERROR");
		break;

	default:
		BOOST_FAIL(result);
		break;
	}
	IEE_LicenseInfos_t licenseInfos;

	// We can call this API any time to check current License information
	result = IEE_LicenseInformation(&licenseInfos);
	BOOST_TEST_MESSAGE("IEE_LicenseInfomation result = " + intToHex(result));
	BOOST_CHECK_EQUAL(result, EDK_OK);
	switch (result)
	{
	case EDK_OVER_QUOTA_IN_DAY:
		BOOST_FAIL("EDK_OVER_QUOTA_IN_DAY");
		break;
	case EDK_OVER_QUOTA_IN_MONTH:
		BOOST_FAIL("EDK_OVER_QUOTA_IN_MONTH");
		break;
	case EDK_LICENSE_EXPIRED:
		BOOST_FAIL("EDK_LICENSE_EXPIRED");
		break;
	case EDK_OVER_QUOTA:
		BOOST_FAIL("EDK_OVER_QUOTA");
		break;
	case EDK_ACCESS_DENIED:
		BOOST_FAIL("EDK_ACCESS_DENIED");
		break;
	case EDK_LICENSE_ERROR:
		BOOST_FAIL("EDK_LICENSE_ERROR");
		break;
	case EDK_NO_ACTIVE_LICENSE:
		BOOST_FAIL("EDK_NO_ACTIVE_LICENSE");
		break;
	case EDK_OK:
		BOOST_TEST_MESSAGE("EDK_OK");
		break;
	default:
		BOOST_FAIL(result);
		break;
	}

	printLicenseInformation(licenseInfos);

	_getch();
}

BOOST_AUTO_TEST_SUITE_END()


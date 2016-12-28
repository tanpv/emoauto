//#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Hardware_Information
#define BOOST_TEST_NO_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <atomic>
#include <chrono>
#include <thread>
#include <string>
#include "Iedk.h"
#include "IedkErrorCode.h"

namespace utf = boost::unit_test;
std::atomic<bool> isTimeout(false);


//handle timer expire
 void on_timeout(const boost::system::error_code& /*e*/)
 {
   isTimeout = true;
   BOOST_TEST_MESSAGE("Timer canceled or expired \n");
 }

constexpr std::chrono::seconds test_case_timer(50);
//constexpr std::chrono::seconds wait_time_for_dongle_connect_physically{ 10 };
//constexpr std::chrono::seconds wait_time_for_removing_dongle_time{ 10 };
//constexpr std::chrono::seconds wait_time_for_setting_software_flag(10);

struct EmoEngine_Fixture {
	EmoEngine_Fixture() {
		BOOST_CHECK_EQUAL(IEE_EngineConnect(), EDK_OK);
		eEvent = IEE_EmoEngineEventCreate();
		eState = IEE_EmoStateCreate();
	}
	~EmoEngine_Fixture() {
		IEE_EngineDisconnect();
		IEE_EmoStateFree(eState);
		IEE_EmoEngineEventFree(eEvent);
	}

	EmoEngineEventHandle    eEvent;
	EmoStateHandle          eState;

};

//Test suite name:Hardware Information
//Description:  Check Hardware Information. Use to  test below Api
//IEE_GetAverageBandPowers
//IEE_GetHeadsetSettings
//IEE_HardwareGetVersion
//IEE_HeadsetGetSensorDetails
//IEE_HeadsetGetSerialNumber
//IEE_SetHeadsetSettings

BOOST_FIXTURE_TEST_SUITE(Hardware_Information, EmoEngine_Fixture)


//multiple fixtures
//BOOST_GLOBAL_FIXTURE(F);
//BOOST_GLOBAL_FIXTURE(F2);
//BOOST_AUTO_TEST_CASE_EXPECTED_FAILURES(TC03, 1)

BOOST_AUTO_TEST_CASE(TC01_GIVEN_Dongle_detected_WHEN_headset_turn_on_THEN_User_get_Hardware_Information,
					* utf::label("auto_test")
					* utf::label("get_headset_information"))
{
	BOOST_TEST_MESSAGE("TC01_GIVEN_Dongle_detected_WHEN_headset_turn_on_THEN_User_get_Hardware_Information\n");

	int state = 0;
	bool isDongleConnected= false;
	bool isStateChanged = false;

	//Arrange
	//IEE_SetSoftwareFlagEnabled(true);


	boost::asio::io_service io;
	boost::asio::deadline_timer timer(io, boost::posix_time::seconds(test_case_timer.count()));
	timer.async_wait(&on_timeout);
	std::thread asioThread([&] { return io.run(); });
	isTimeout = false; //reset expiration timer
	while (1)
	{
		state = IEE_EngineGetNextEvent(eEvent);
		IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);

		if (state == EDK_OK)
		{
			if (eventType == IEE_UserAdded)
			{
				BOOST_TEST_MESSAGE("IEE_UserAdded");
				isDongleConnected = true;
			}
			if (eventType == IEE_EmoStateUpdated)
			{
				isStateChanged = true;
			}
		}

		if (isDongleConnected && isStateChanged)
		{
			//Get UserID
			isStateChanged = false;
			unsigned int userID;
			unsigned int EPOCmode;
			unsigned int eegRate;
			unsigned int eegRes;
			unsigned int memsRate;
			unsigned int memsRes;

			int result = IEE_EmoEngineEventGetUserId(eEvent,& userID);
			BOOST_CHECK_EQUAL(result, EDK_OK);
			//Get Hardware Information
			int result2 = IEE_GetHeadsetSettings(userID, &EPOCmode,
				&eegRate, &eegRes, &memsRate, &memsRes);
			BOOST_CHECK_EQUAL(result2, EDK_OK);
			BOOST_CHECK((EPOCmode == 0) || (EPOCmode == 1));
			BOOST_CHECK((eegRate >= 0) && (eegRate <= 2));
			BOOST_CHECK((eegRes >= 0) && (eegRes <= 2));
			BOOST_CHECK((memsRate >= 0) && (memsRate <= 3));
			BOOST_CHECK((memsRes >= 0) && (memsRes <= 3));
			timer.cancel();

		}
		if (isTimeout)
		{
			break;
		}

	}
	asioThread.join();
}

//TC2 Headset Signal
// GIVEN Dongle connected and headset on WHEN batterry high and not wireless signal THEN the time since EmoEngine more than 0
BOOST_AUTO_TEST_CASE(TC02,
	*utf::label("auto_test")
	* utf::label("headset_status"))
{
	BOOST_TEST_MESSAGE("TC02_GIVEN_Dongle_connected_and_headset_on_when_batterry_high_and_not_wireless_signal_THEN_the_time_since_EmoEngine_more_than_0\n");

	int state = 0;
	bool isDongleConnected = false;

	//Arrange
	//IEE_SetSoftwareFlagEnabled(true);


	boost::asio::io_service io;
	boost::asio::deadline_timer timer(io, boost::posix_time::seconds(test_case_timer.count()));
	timer.async_wait(&on_timeout);
	std::thread asioThread([&] { return io.run(); });
	isTimeout = false; //reset expiration timer
	while (1)
	{
		state = IEE_EngineGetNextEvent(eEvent);
		IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);

		if (state == EDK_OK)
		{
			if (eventType == IEE_UserAdded)
			{
				BOOST_TEST_MESSAGE("IEE_UserAdded");
				isDongleConnected = true;
			}
			if (eventType == IEE_UserRemoved)
			{
				isDongleConnected = false;
			}
		}

		//Arrange
		// headset on (1: On, 0: Off)
		if (isDongleConnected && IS_GetHeadsetOn(eState))
		{
			int chargeLevel = -1;
			int maxChargeLevel = 0;

			//Get the level of charge remaining in the headset battery
			IS_GetBatteryChargeLevel(eState, & chargeLevel,& maxChargeLevel);

			//WHEN batterry high and not wireless signal
			if ((IS_GetWirelessSignalStatus(eState) > 1) && (chargeLevel > 3) && (chargeLevel < maxChargeLevel)) //how to know batery level is high
			{
				BOOST_TEST(IS_GetTimeFromStart(eState) > 0);
				timer.cancel();
			}
		}

		if (isTimeout)
		{
			break;
		}

	}
	asioThread.join();
}


BOOST_AUTO_TEST_SUITE_END()

//--------------------------------------------------------------
/****************************************************************************
**
** Copyright 2015 by Emotiv. All rights reserved
** Example - Headset Information Logger
** This example allows getting headset infor: contactquality, wireless strength
** battery level.
** This example work on single connection
****************************************************************************/
#define BOOST_TEST_MODULE Add_Boots_Test
#define BOOST_TEST_NO_MAIN

#include <iostream>
#include <fstream>
#include <conio.h>

#include "Iedk.h"
#include "IedkErrorCode.h"
#include "IEmoStateDLL.h"

#include <boost/test/auto_unit_test.hpp>
#include <boost/asio.hpp>
#include <atomic>
#include <chrono>
#include <thread>
#include <string>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <atomic>


namespace utf = boost::unit_test;
std::atomic<bool> isTimeout(false);

// Set timeout
int const runtime = 10;
constexpr std::chrono::seconds test_case_timer(runtime);
void on_timeout(const boost::system::error_code& /*e*/)
{
	isTimeout = true;
	BOOST_TEST_MESSAGE("Timer canceled or expired \n");
}

// Fixture define
struct Fixture {

	//run before every test case start
	Fixture() {
		std::cout << "Run time: " << runtime << "s" << '\n';
		BOOST_CHECK_EQUAL(IEE_EngineConnect(), EDK_OK);
		BOOST_TEST_MESSAGE("Connecting headset");

		eEvent = IEE_EmoEngineEventCreate();
		eState = IEE_EmoStateCreate();
	}

	//run after every test case finish
	~Fixture() {
		//BOOST_TEST_MESSAGE("Disconnect headset \n Free memory referenced by an EmoState handle \n Free memory referenced by an event handle");
		IEE_EngineDisconnect();
		IEE_EmoStateFree(eState);
		IEE_EmoEngineEventFree(eEvent);
	}
	EmoEngineEventHandle    eEvent;
	EmoStateHandle          eState;

};

//--------------------------------------------------------------
//function
//int multiple(int i, int j) { return i*j; }


//define and bind suite to fixture at one macrco
BOOST_FIXTURE_TEST_SUITE(duytan, Fixture)

//test case
BOOST_AUTO_TEST_CASE(headset_information_logger)
{
	boost::asio::io_service io;
	boost::asio::deadline_timer timer(io, boost::posix_time::seconds(test_case_timer.count()));
	timer.async_wait(&on_timeout);
	std::thread asioThread([&] { return io.run(); });
	isTimeout = false; //reset expiration timer

	unsigned int userID = 0;
	int state = 0;
	IEE_EEG_ContactQuality_t* contactQuality = new IEE_EEG_ContactQuality_t;
	float systemUpTime = 0;
	int batteryLevel, maxBatteryLevel = 0;
	IEE_SignalStrength_t wirelessStrength;
	bool onStateChanged = false;

	isTimeout = false; //reset expiration timer

	while (true) {

		state = IEE_EngineGetNextEvent(eEvent);
		if (state == EDK_OK) {

			IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);
			IEE_EmoEngineEventGetUserId(eEvent, &userID);

			switch (eventType)
			{
			case IEE_UserAdded:
				BOOST_TEST_MESSAGE("User added");
				break;
			case IEE_UserRemoved:
				BOOST_TEST_MESSAGE("User removed");
				break;
			case IEE_EmoStateUpdated:
				onStateChanged = true;
				IEE_EmoEngineEventGetEmoState(eEvent, eState);
				break;
			default:
				break;
			}
		}

		if (onStateChanged)
		{
			onStateChanged = false;
			systemUpTime = IS_GetTimeFromStart(eState);
			wirelessStrength = IS_GetWirelessSignalStatus(eState);

			if (wirelessStrength != NO_SIG)
			{
				//std::cout << "Time: " << systemUpTime << std::endl;
				IS_GetBatteryChargeLevel(eState, &batteryLevel, &maxBatteryLevel);

				BOOST_CHECK(systemUpTime > 0);
				BOOST_CHECK(wirelessStrength > 0 || wirelessStrength <= 4);
				BOOST_CHECK(batteryLevel >= 0 || batteryLevel <= 4);
				BOOST_CHECK(IS_GetContactQuality(eState, IEE_CHAN_AF3) >= 0);
				BOOST_CHECK(IS_GetContactQuality(eState, IEE_CHAN_T7) >= 0);
				BOOST_CHECK(IS_GetContactQuality(eState, IEE_CHAN_Pz) >= 0);
				BOOST_CHECK(IS_GetContactQuality(eState, IEE_CHAN_T8) >= 0);
				BOOST_CHECK(IS_GetContactQuality(eState, IEE_CHAN_AF4) >= 0);
			}
		}

		if (isTimeout)
		{
			delete contactQuality;
			break;
		}
	}
	asioThread.join();
}

BOOST_AUTO_TEST_CASE(mental_command_demo) {

}

//end of suite
BOOST_AUTO_TEST_SUITE_END()

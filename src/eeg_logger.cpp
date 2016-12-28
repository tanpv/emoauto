//#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE EEG_Logger
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

BOOST_FIXTURE_TEST_SUITE(Gyro_Data, EmoEngine_Fixture)


//multiple fixtures
//BOOST_GLOBAL_FIXTURE(F);
//BOOST_GLOBAL_FIXTURE(F2);
//BOOST_AUTO_TEST_CASE_EXPECTED_FAILURES(TC03, 1)

//TC01
// GIVEN Dongle connected and headset on THEN gyro data more than 0
BOOST_AUTO_TEST_CASE(TC01,
	*utf::label("auto_test")
	* utf::label("tc01_eeg_logger"))
{
	BOOST_TEST_MESSAGE("TC01_GIVEN_Dongle_connected_and_headset_on_THEN_gyro_data_more_than_0\n");

	int state = 0;
	bool isDongleConnected = false;
	bool isStateChanged = false;
	unsigned int userID = 0;
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
		int result = IEE_EmoEngineEventGetUserId(eEvent, &userID);

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
			if (eventType == IEE_EmoStateUpdated)
			{
				isStateChanged = true;
			}
		}

		//Arrange
		// headset on (1: On, 0: Off)
		if ( isDongleConnected && isStateChanged)
		{
			isStateChanged = false;
			int gyroX = 0, gyroY = 0;
			if (IEE_HeadsetGetGyroDelta(userID, &gyroX, &gyroY) == EDK_OK)
			{
				std::cout << "gyroX " << gyroX << "\n";
				std::cout << "gyroY " << gyroY << "\n";
				BOOST_TEST(gyroX > 0);
				BOOST_TEST(gyroY > 0);
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

// alo 1234
//#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Dongle_Headset_Detection
//#define BOOST_TEST_NO_MAIN
#include <boost/test/included/unit_test.hpp>
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


constexpr std::chrono::seconds detect_dongle_headset_connection_timer{ 50 };
constexpr std::chrono::seconds wait_time_for_dongle_connect_physically{ 10 };
constexpr std::chrono::seconds wait_time_for_removing_dongle_time{ 10 };
constexpr std::chrono::seconds wait_time_for_setting_software_flag(10);

//handle timer expire
void on_timeout(const boost::system::error_code& /*e*/)
{
  isTimeout = true;
  BOOST_TEST_MESSAGE("Timer canceled or expired \n");
}

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

//Test suite name:detect_dongle_headset_connection
//Description:  Detect Dongle and Headset Connected or Dongle removed
BOOST_FIXTURE_TEST_SUITE(detect_dongle_headset_connection, EmoEngine_Fixture)

//multiple fixtures
//BOOST_GLOBAL_FIXTURE(F);
//BOOST_GLOBAL_FIXTURE(F2);

//Detect Dongle connected
BOOST_AUTO_TEST_CASE(TC01_GIVEN_Software_switch_flag_is_false_WHEN_User_physically_connects_dongle_THEN_User_cannot_see_dongle_connected, 
					* utf::label("auto_test")
					* utf::label("detect_dongle_headset_connection"))
{
	BOOST_TEST_MESSAGE("TC01_GIVEN_Software_switch_flag_is_false_WHEN_User_physically_connects_dongle_THEN_User_cannot_see_dongle_connected\n");

     int state = 0;
     bool isDongleDected = false;


	 //Arrange
	 IEE_SetSoftwareFlagEnabled(false);
	 BOOST_TEST_MESSAGE("Initial: IEE_SetSoftwareFlagEnabled(false)");

     boost::asio::io_service io;
     boost::asio::deadline_timer timer(io, boost::posix_time::seconds(detect_dongle_headset_connection_timer.count()));
     timer.async_wait(&on_timeout);
     std::thread asioThread([&] { return io.run(); });
     isTimeout = false; //reset expiration timer
     while (1)
     {
       state = IEE_EngineGetNextEvent(eEvent);
       IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);

       BOOST_CHECK((state == EDK_OK) || (state == EDK_NO_EVENT));
	   if (state == EDK_OK)
	   {
		   if (eventType == IEE_UserAdded)
		   {
			   BOOST_TEST_MESSAGE( "IEE_UserAdded");
			   isDongleDected = true;
			   timer.cancel();
		   }
	   }	   
       if (isTimeout)
       {
          break;
       }

     }
     BOOST_CHECK_EQUAL(isDongleDected, false);
     asioThread.join();
}
//TC02
BOOST_AUTO_TEST_CASE(TC02_GIVEN_Software_switch_is_false_AND_dongle_is_physically_connected_WHEN_Sofware_Switch_is_set_to_true_THEN_User_can_see_dongle_connected,
					* utf::label("auto_test")
					* utf::label("detect_dongle_headset_connection"))
{
	BOOST_TEST_MESSAGE("TC02_GIVEN_Software_switch_is_false_AND_dongle_is_physically_connected_"
				"WHEN_Sofware_Switch_is_set_to_true_THEN_User_can_see_dongle_connected\n");

	int state = 0;
	bool isDongleDected = false;


	//Arrange
	IEE_SetSoftwareFlagEnabled(false); //reset software Flag
	BOOST_TEST_MESSAGE("Initial: IEE_SetSoftwareFlagEnabled(false)");

	boost::asio::io_service io;
	boost::asio::deadline_timer timer(io, boost::posix_time::seconds(detect_dongle_headset_connection_timer.count()));
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
				BOOST_TEST_MESSAGE( "IEE_UserAdded");
				isDongleDected = true;
				timer.cancel();
			}
		}
		if (!isDongleDected)
		{
			std::this_thread::sleep_for(wait_time_for_setting_software_flag);
			//When
			IEE_SetSoftwareFlagEnabled(true);
			BOOST_TEST_MESSAGE("When: IEE_SetSoftwareFlagEnabled(true)");
		}
		
		if (isTimeout)
		{
			break;
		}
	}

	BOOST_CHECK_EQUAL(isDongleDected, true);
	asioThread.join();
}
//TC03
BOOST_AUTO_TEST_CASE(TC03_GIVEN_Sofware_switch_is_true_AND_Dongle_is_physically_connected_WHEN_Sofware_switch_is_set_to_false_THEN_User_cannot_see_dongle_connected,
				     *utf::label("auto_test")
			         * utf::label("detect_dongle_headset_connection"))
{
	BOOST_TEST_MESSAGE("TC03_GIVEN_Sofware_switch_is_true_AND_Dongle_is_physically_connected_"
				"WHEN_Sofware_switch_is_set_to_false_THEN_User_cannot_see_dongle_connected\n");

	int state = 0;
	bool isDongleDected = false;

	//Arrange
	IEE_SetSoftwareFlagEnabled(true);
	BOOST_TEST_MESSAGE("Initial: IEE_SetSoftwareFlagEnabled(true)");

	boost::asio::io_service io;
	boost::asio::deadline_timer timer(io, boost::posix_time::seconds(detect_dongle_headset_connection_timer.count()));
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
				BOOST_TEST_MESSAGE( "IEE_UserAdded");
				isDongleDected = true;
			}
			if (eventType == IEE_EmoStateUpdated)
			{
				BOOST_TEST_MESSAGE( "IEE_EmoStateUpdated");
				isDongleDected = true; //will set true if dongle still present 
				timer.cancel();
			}
		}
		if (isDongleDected)
		{
			//WHEN Sofware switch is set to false
			std::this_thread::sleep_for(wait_time_for_setting_software_flag);
			IEE_SetSoftwareFlagEnabled(false);
			BOOST_TEST_MESSAGE("Arrange: IEE_SetSoftwareFlagEnabled(false)");
			isDongleDected = false;
		}

		if (isTimeout)
		{
			break;
		}
	}

	BOOST_CHECK_EQUAL(isDongleDected, false);
	asioThread.join();
}

//TC04
BOOST_AUTO_TEST_CASE(TC04_GIVEN_Sofware_switch_is_false_WHEN_User_physically_connects_dongle_AND_USER_physically_disconnects_dongle_THEN_User_cannot_see_dongle_connected,
					*utf::label("manual_test")
					* utf::label("detect_dongle_headset_connection"))
{
	BOOST_TEST_MESSAGE("TC04_GIVEN_Sofware_switch_is_false_"
		         "WHEN_User_physically_connects_dongle_AND_USER_physically_disconnects_dongle_"
				 "THEN_User_cannot_see_dongle_connected\n");

	int state = 0;
	bool isDongleDected = false;

	//Arrange
	IEE_SetSoftwareFlagEnabled(true);
	BOOST_TEST_MESSAGE("Initial: IEE_SetSoftwareFlagEnabled(true)");

	boost::asio::io_service io;
	boost::asio::deadline_timer timer(io, boost::posix_time::seconds(detect_dongle_headset_connection_timer.count()));
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
				BOOST_TEST_MESSAGE( "IEE_UserAdded");
				isDongleDected = true;
			}
			if (eventType == IEE_EmoStateUpdated)
			{
				BOOST_TEST_MESSAGE( "IEE_EmoStateUpdated");
				isDongleDected = true; //will set true if dongle still present 
				timer.cancel();
			}
		}
		if (isDongleDected)
		{
			//WHEN USER physically disconnects dongle
			IEE_SetSoftwareFlagEnabled(false);
			BOOST_TEST_MESSAGE("When: IEE_SetSoftwareFlagEnabled(false)");

			BOOST_TEST_MESSAGE( "Please remove dongle physically \n");
			std::this_thread::sleep_for(wait_time_for_removing_dongle_time);
			isDongleDected = false; //reset dongle
		}

		if (isTimeout)
		{
			break;
		}
	}

	BOOST_CHECK_EQUAL(isDongleDected, false);
	asioThread.join();
}

//TC05
BOOST_AUTO_TEST_CASE(TC05_GIVEN_Sofware_switch_is_true_WHEN_User_physically_connects_dongle_AND_User_physically_disconnects_dongle_AND_User_physically_connects_dongle_again_THEN_User_can_see_dongle_connected,
					*utf::label("manual_test")
					* utf::label("detect_dongle_headset_connection"))
{
	BOOST_TEST_MESSAGE("TC05_GIVEN_Sofware_switch_is_true_"
		         "WHEN_User_physically_connects_dongle_AND_User_physically_disconnects_dongle_AND_User_physically_connects_dongle_again_"
		         "THEN_User_can_see_dongle_connected\n");

	int state = 0;
	bool isDongleDected = false;
	bool isDongleRemoved = false;

	//Arrange
	IEE_SetSoftwareFlagEnabled(true);
	BOOST_TEST_MESSAGE("Initial: IEE_SetSoftwareFlagEnabled(true)");

	boost::asio::io_service io;
	boost::asio::deadline_timer timer(io, boost::posix_time::seconds(detect_dongle_headset_connection_timer.count()));
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
				BOOST_TEST_MESSAGE( "IEE_UserAdded");
				isDongleDected = true;
				if (isDongleRemoved) //reconnect again
				{
					timer.cancel();
				}
				isDongleRemoved = false;
			}
			if (eventType == IEE_UserRemoved)
			{
				BOOST_TEST_MESSAGE( "IEE_UserRemoved");
				isDongleRemoved = true;
				isDongleDected = false;
			}

		}
		if (isTimeout)
		{
			break;
		}

		if (isDongleDected && !isDongleRemoved)
		{
			isDongleDected = false; //reset
			BOOST_TEST_MESSAGE( "Please removed dongle physically \n");
			std::this_thread::sleep_for(wait_time_for_removing_dongle_time);
		}

		if (isDongleRemoved)
		{
			//WHEN User physically connects dongle AND User physically disconnects dongle AND User physically connects dongle again
			BOOST_TEST_MESSAGE( "Please re-connect dongle physically \n");
			std::this_thread::sleep_for(wait_time_for_dongle_connect_physically);
		}

		
	}

	BOOST_CHECK_EQUAL(isDongleDected, true);
	asioThread.join();
}

//TC06
BOOST_AUTO_TEST_CASE(TC06_GIVEN_Sofware_switch_is_false_WHEN_User_physically_connects_dongle_AND_User_physically_disconnects_dongle_AND_User_physically_connects_dongle_again_THEN_User_can_not_see_dongle_connected,
					*utf::label("manual_test")
					* utf::label("detect_dongle_headset_connection"))
{
	BOOST_TEST_MESSAGE("TC06_GIVEN_Sofware_switch_is_false_"
		         "WHEN_User_physically_connects_dongle_AND_User_physically_disconnects_dongle_AND_User_physically_connects_dongle_again_"
		         "THEN_User_can_not_see_dongle_connected\n");

	int state = 0;
	bool isDongleDected = false;
	bool isDongleRemoved = false;


	//BOOST_CHECK_EQUAL(IEE_EngineConnect(), EDK_OK);
	//Arrange
	IEE_SetSoftwareFlagEnabled(true);
	BOOST_TEST_MESSAGE("Initial: IEE_SetSoftwareFlagEnabled(true)");

	boost::asio::io_service io;
	boost::asio::deadline_timer timer(io, boost::posix_time::seconds(detect_dongle_headset_connection_timer.count()));
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
				BOOST_TEST_MESSAGE( "IEE_UserAdded");
				isDongleDected = true;
				if (isDongleRemoved) //reconnect again
				{
					timer.cancel();
				}
				isDongleRemoved = false;
			}
			if (eventType == IEE_UserRemoved)
			{
				BOOST_TEST_MESSAGE( "IEE_UserRemoved");
				isDongleRemoved = true;
				isDongleDected = false;
			}
			if (eventType == IEE_EmoStateUpdated)
			{
				BOOST_TEST_MESSAGE( "IEE_EmoStateUpdated");
				isDongleDected = true; //will set true if dongle still present 
				timer.cancel();
			}

		}
		if (isTimeout)
		{
			break;
		}

		if (isDongleDected && !isDongleRemoved)
		{
			//isDongleDected = false;
			BOOST_TEST_MESSAGE( "Please removed dongle physically \n");
			std::this_thread::sleep_for(wait_time_for_removing_dongle_time);
		}

		if (isDongleRemoved)
		{			
			//Arrange
			IEE_SetSoftwareFlagEnabled(false);
			BOOST_TEST_MESSAGE("Arrange: IEE_SetSoftwareFlagEnabled(false)");
			//WHEN User physically connects dongle AND User physically disconnects dongle AND User physically connects dongle again
			BOOST_TEST_MESSAGE( "Please connect dongle physically \n");
			std::this_thread::sleep_for(wait_time_for_dongle_connect_physically);	
		}
	}

	BOOST_CHECK_EQUAL(isDongleDected, false);
	asioThread.join();
}

//Detect HeadSet on/off
//Detect HeadSet on/off
BOOST_AUTO_TEST_CASE(TC07_GIVEN_Software_switch_flag_is_true_and_dongle_connected_WHEN_headset_turn_on_THEN_received_EmoStateUpdated,
	*utf::label("auto_test")
	* utf::label("detect_dongle_headset_connection"))
{
	BOOST_TEST_MESSAGE("TC07_GIVEN_Software_switch_flag_is_true_and_dongle_connected_WHEN_headset_turn_on_THEN_received_EmoStateUpdated\n");

	int state = 0;
	bool isHeadsetOn = false;


	//Arrange
	IEE_SetSoftwareFlagEnabled(true);
	//BOOST_TEST_MESSAGE("Initial: IEE_SetSoftwareFlagEnabled(true)");

	boost::asio::io_service io;
	boost::asio::deadline_timer timer(io, boost::posix_time::seconds(detect_dongle_headset_connection_timer.count()));
	timer.async_wait(&on_timeout);
	std::thread asioThread([&] { return io.run(); });
	isTimeout = false; //reset expiration timer
	while (1)
	{
		state = IEE_EngineGetNextEvent(eEvent);
		IEE_Event_t eventType = IEE_EmoEngineEventGetType(eEvent);

		BOOST_CHECK((state == EDK_OK) || (state == EDK_NO_EVENT));
		if (state == EDK_OK)
		{
			if (eventType == IEE_UserAdded)
			{
				BOOST_TEST_MESSAGE("IEE_UserAdded");
			}
			if (eventType == IEE_EmoStateUpdated)
			{
				BOOST_TEST_MESSAGE("IEE_EmoStateUpdated");
				isHeadsetOn = true;
				timer.cancel();
			}
		}
		if (isTimeout)
		{
			break;
		}

	}
	BOOST_CHECK_EQUAL(isHeadsetOn, true);
	asioThread.join();
}

BOOST_AUTO_TEST_SUITE_END()

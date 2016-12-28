/*

SDK function covered :
	IEE_ActivateLicense
	IEE_LicenseInformation

	- How to explaination at the top of test file ?
	- How to put name of project ?
	- How to put function name ?
	- How to put test point in test case ?
	- How to deal with test report ?

*/

#define BOOST_TEST_MODULE Activate_License
#define BOOST_TEST_NO_MAIN
#include <boost/test/auto_unit_test.hpp>
#include "Iedk.h"
#include "IedkErrorCode.h"

//define test suite
BOOST_AUTO_TEST_SUITE(Activate_License)

//valid product license
std::string const VALUE_LICENSE_KEY = "50541929-7330-4059-b073-30814ff15ea2";
BOOST_AUTO_TEST_CASE(GIVEN_have_a_valid_license_WHEN_server_is_up_THEN_user_could_activate_license)
{
	int result;

	// activate first time
	IEE_ActivateLicense(VALUE_LICENSE_KEY.c_str());

	// activate second time
	result = IEE_ActivateLicense(VALUE_LICENSE_KEY.c_str());

	// expect result return EDK_LICENSE_REGISTERED
	// use BOOST_TEST better than BOOST_CHECK
	// demo BOOST_TEST and BOOST_CHECK
	//BOOST_CHECK(result == EDK_LICENSE_REGISTERED);
	BOOST_TEST(result == EDK_OK);
}

//expired product license


BOOST_AUTO_TEST_SUITE_END()
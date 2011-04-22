/**
 * TMS_fixtures.hpp
 *
 * Author : bisnard
 */

#include "UMS_fixtures.hpp"
#include "TMS_testconfig.h"
#include <boost/test/unit_test.hpp>
#include "api_ums.hpp"
#include "api_tms.hpp"
using namespace std;
using namespace vishnu;

char TMSSeD[] = "tmssed";
char ConfigTMSSeD[] = TMSSEDCONF;
char BinDirTMSSeD[] = TMSSEDBINDIR;

class TMSFixture : public UMSSeDFixture {

public:
  TMSFixture():mac(2){

    BOOST_TEST_MESSAGE( "== Test setup [BEGIN]: Initializing client ==" );
    // Name of the test executable
    mav[0]= (char*)"./tms_automTest";
    // Client configuration file
    string dietClientConfigPath = CONFIG_DIR + string("/client_testing.cfg");
    mav[1]= (char*) dietClientConfigPath.c_str();

    if (vishnu::vishnuInitialize(mav[1], mac, mav)) {
      BOOST_TEST_MESSAGE( "Error in VishnuInitialize..." );
    }
    BOOST_TEST_MESSAGE( "== Test setup [END]: Initializing client ==" );

    BOOST_TEST_MESSAGE( "== Test setup [BEGIN]: Initializing database ==" );
    string sqlPath = TMSSQLPATH;
    if (restore(sqlPath + "/cleanall.sql") != 0) {
      cout << "Clean database failed" << endl;
      return;
    }
    if (restore(sqlPath + "/TMSinitTest.sql")!=0) {
      cout << "Database initialization failed" << endl;
      return;
    }
    BOOST_TEST_MESSAGE( "== Test setup [END]: Initializing database ==" );

  }

  ~TMSFixture() {
    BOOST_TEST_MESSAGE( "== Test teardown [BEGIN]: Finalizing client ==" );
    vishnu::vishnuFinalize();
    BOOST_TEST_MESSAGE( "== Test teardown [END]: Finalizing client ==" );
  }

//private:
  int mac;
  char* mav[2];
};

typedef DietSeDFixture<TMSSeD, BinDirTMSSeD, ConfigTMSSeD, TMSFixture> TMSSeDFixture;

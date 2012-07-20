#include <boost/test/unit_test.hpp>

#include "core_testconfig.h"
#include "SOCIDatabase.hpp"


BOOST_AUTO_TEST_SUITE( soci_database_test )


using namespace std;
using namespace vishnu;

const std::string configFileDir(TESTCORECONFIGPATH);
const std::string configFilePath=configFileDir+"/soci/";

static SOCIDatabase * myDatabase=NULL;

BOOST_AUTO_TEST_CASE( initialisation )
{
	//TODO : this should be in a Fixture class

	BOOST_TEST_MESSAGE("initialisation, connection to database");
	ExecConfiguration exConfig;
	std::string configFile=configFilePath+"soci-test_config.cfg";
	try{
		exConfig.initFromFile(configFile);
		DbConfiguration myConfig(exConfig);
		myConfig.check();
		myDatabase = new SOCIDatabase(myConfig);
	} catch( exception const & e)
	{
		BOOST_TEST_MESSAGE(e.what());
		BOOST_CHECK(false);
	}
	BOOST_REQUIRE(myDatabase!=NULL);
	BOOST_CHECK(myDatabase->connect()==0);
	BOOST_CHECK(myDatabase->disconnect()==0);
	BOOST_TEST_MESSAGE("Normal connection to Database -- ok");

	BOOST_CHECK_THROW(myDatabase->disconnect(),VishnuException);
}

BOOST_AUTO_TEST_CASE( getting_single_session )
{
	BOOST_REQUIRE(myDatabase != NULL);

	BOOST_TEST_MESSAGE("getting single session to database");
	// processing with disconnected database
	BOOST_CHECK_THROW(myDatabase->getSingleSession(),VishnuException);
	// connection to database
	BOOST_REQUIRE(myDatabase->connect()==0);
	try {
	SOCISession session;
	SOCISession session2;
	SOCISession session3;
	SOCISession nullSession;
	BOOST_CHECK_NO_THROW(session = myDatabase->getSingleSession());
	// get Single session with requested position
	BOOST_CHECK_NO_THROW(session2 = myDatabase->getSingleSession(2));
	BOOST_CHECK_THROW(session3 = myDatabase->getSingleSession(147),VishnuException);
	BOOST_CHECK(myDatabase->releaseSingleSession(session)==0);
	// single session with requested position does not need to be released
	BOOST_CHECK_THROW(myDatabase->releaseSingleSession(session2),VishnuException);
	BOOST_CHECK_THROW(myDatabase->releaseSingleSession(nullSession), VishnuException);


	// maximum : 5 sessions, set in configuration file
	BOOST_CHECK_NO_THROW(myDatabase->getSingleSession());
	BOOST_CHECK_NO_THROW(myDatabase->getSingleSession());
	BOOST_CHECK_NO_THROW(myDatabase->getSingleSession());
	BOOST_CHECK_NO_THROW(myDatabase->getSingleSession());
	BOOST_CHECK_NO_THROW(myDatabase->getSingleSession());
	BOOST_CHECK_THROW(myDatabase->getSingleSession(),VishnuException);
	} catch(exception const & e)
	{
		BOOST_MESSAGE(e.what());
		BOOST_CHECK(false);
	}

	BOOST_CHECK(myDatabase->disconnect()==0);

}

BOOST_AUTO_TEST_CASE( session_transaction_test )
{
	BOOST_REQUIRE(myDatabase != NULL);
	BOOST_CHECK(myDatabase->connect()==0);
	try{
		SOCISession session;
		BOOST_CHECK_NO_THROW(session = myDatabase->getSingleSession());

		BOOST_CHECK_NO_THROW(session.rollback());
		BOOST_CHECK_NO_THROW(session.commit());
		BOOST_CHECK_NO_THROW(session.begin());
		BOOST_CHECK_NO_THROW(session.commit());
		BOOST_CHECK_NO_THROW(session.rollback());


		BOOST_CHECK_NO_THROW(myDatabase->releaseSingleSession(session));
	} catch (exception const & e)
	{
		BOOST_MESSAGE(e.what());
		BOOST_CHECK(false);
	}

	BOOST_CHECK(myDatabase->disconnect()==0);
}

BOOST_AUTO_TEST_CASE( session_process_test )
{
	BOOST_REQUIRE(myDatabase != NULL);
	BOOST_CHECK(myDatabase->connect()==0);
	try{
		SOCISession session;
		BOOST_CHECK_NO_THROW(session = myDatabase->getSingleSession());


		// executing simple request
		BOOST_TEST_MESSAGE("execute SQL process by creating table");
		BOOST_CHECK_NO_THROW(session.execute("drop table if exists paco"));
		BOOST_CHECK_NO_THROW(session<<"create table paco(id int, nom varchar(255))");


		BOOST_CHECK_NO_THROW(myDatabase->releaseSingleSession(session));
	} catch (exception const & e)
	{
		BOOST_MESSAGE(e.what());
		BOOST_CHECK(false);
	}

	BOOST_CHECK(myDatabase->disconnect()==0);
}


BOOST_AUTO_TEST_CASE( session_exchange_test )
{
	BOOST_REQUIRE(myDatabase != NULL);
	BOOST_CHECK(myDatabase->connect()==0);
	try{
		SOCISession session;
		BOOST_CHECK_NO_THROW(session = myDatabase->getSingleSession());


		// exchanging input data
		int id = 6;
		int id2 = 7;
		int id3 = 9;
		string strId="9";

		string name="Kallstrom";
		string name2="Lisandro";
		string request = "insert into paco(id,nom) values(:id,:name);";
		BOOST_CHECK_NO_THROW(session.execute(request).use(id).use(name));
		soci::indicator null_ind=soci::i_null;
		BOOST_CHECK_NO_THROW(session.execute(request).use(id2).use(name2,null_ind));
		BOOST_CHECK(name=="Kallstrom");
		BOOST_CHECK_NO_THROW(session.execute(request).use(strId).use(name2));
		soci::indicator mindic;

		// insert null string in integer
		string badStr="";
		BOOST_CHECK_THROW(session.execute(request).use(badStr).use(name2),VishnuException);
		// insert non numeric string in integer
		badStr="helloWorld";
		BOOST_CHECK_THROW(session.execute(request).use(badStr).use(name2),VishnuException);

		// exchanging output data
		int reqid = -1;
		string reqname="";
		string request2 = "select id,nom from paco where ";
		// reading a complete unique tuple
		BOOST_CHECK_NO_THROW(session.execute(request2+"id=6").into(reqid).into(reqname));
		BOOST_CHECK(reqid==6);
		BOOST_CHECK(reqname==name);
		//testing boolean got data
		BOOST_CHECK(session.got_data()==true);
		BOOST_CHECK_NO_THROW(session.execute(request2+"id=147").into(reqid).into(reqname));
		BOOST_CHECK(reqid==6);
		BOOST_CHECK(reqname==name);
		BOOST_CHECK(session.got_data()==false);

		// reading null value in 'nom'
		BOOST_CHECK_NO_THROW(session.execute(request2+"id=7").into(reqid).into(reqname));
		BOOST_CHECK(reqid==7);
		// reqname has not changed
		BOOST_CHECK(reqname==name);
		soci::indicator indic;
		BOOST_CHECK_NO_THROW(session.execute(request2+"id=7").into(reqid).into(reqname,indic));
		BOOST_CHECK(indic==soci::i_null);

		//reading many values not null
		vector<int> ids(10);
		vector<string> names(10);
		BOOST_CHECK_NO_THROW(session.execute(request2+"nom is not null").into(ids).into(names));
		BOOST_CHECK(ids.size()==2);
		BOOST_CHECK(names.size()==2);
		BOOST_CHECK(ids[0]==6);
		BOOST_CHECK(ids[1]==9);
		BOOST_CHECK(names[0]=="Kallstrom");
		BOOST_CHECK(names[1]=="Lisandro");

		vector<string> names2;
		// null size for vector is not allowed for into
		BOOST_CHECK_THROW(session.execute(request2+"id is not null").into(ids).into(names2),
				VishnuException);

		// not enought size for results
		// results has null value
		names2.resize(2);
		ids.resize(2);
		// 3 results requested
		BOOST_CHECK_NO_THROW(session.execute(request2+"id is not null").into(ids).into(names2));
		// only 2 gotten results
		BOOST_CHECK(ids.size()==2);
		BOOST_CHECK(names.size()==2);

		// reading result with null value
		names2.resize(3);
		ids.resize(3);
		vector<soci::indicator> indics(3);
		BOOST_CHECK_NO_THROW(session.execute(request2+"id is not null").into(ids).into(names2,indics));
		BOOST_CHECK(indics[0]==soci::i_ok);
		BOOST_CHECK(indics[1]==soci::i_null);
		BOOST_CHECK(indics[2]==soci::i_ok);

		BOOST_CHECK_NO_THROW(myDatabase->releaseSingleSession(session));
	} catch (exception const & e)
	{
		BOOST_MESSAGE(e.what());
		BOOST_CHECK(false);
	}

	BOOST_CHECK(myDatabase->disconnect()==0);
}


BOOST_AUTO_TEST_CASE( clean_test )
{
	BOOST_REQUIRE(myDatabase != NULL);
	BOOST_CHECK(myDatabase->connect()==0);
	BOOST_CHECK(myDatabase->process("drop table if exists paco")==0);
	BOOST_CHECK(myDatabase->disconnect()==0);
}

BOOST_AUTO_TEST_CASE( finalize_test )
{
	// TODO this should be in a Fixture class

	try {
		if(myDatabase != NULL){
			delete myDatabase;
		}
		BOOST_CHECK(true);
	} catch (exception const & e)
	{
		BOOST_MESSAGE(e.what());
		BOOST_REQUIRE(false);
	}


}

BOOST_AUTO_TEST_SUITE_END()
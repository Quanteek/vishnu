/**
* \file ServerUMS.cpp
* \brief This file presents the implementation of the UMS server.
* \author Eugène PAMBA CAPO-CHICHI (eugene.capochichi@sysfera.com)
* \date 31/01/2001
*/

#include "ServerUMS.hpp"
#include "internalApi.hpp"
#include "Authenticator.hpp"
#include "AuthenticatorFactory.hpp"

//{{RELAX<MISRA_0_1_3> Because these variables are used in this class
Database *ServerUMS::mdatabaseVishnu = NULL;
ServerUMS *ServerUMS::minstance = NULL;
UMSMapper *ServerUMS::mmapper = NULL;
TMSMapper *ServerUMS::mmapperTMS = NULL;
FMSMapper *ServerUMS::mmapperFMS = NULL;
IMSMapper *ServerUMS::mmapperIMS = NULL;
Authenticator *ServerUMS::mauthenticator = NULL;

//}}RELAX<MISRA_0_1_3>

/**
 * \brief To get the unique instance of the server
 */
ServerUMS*
ServerUMS::getInstance() {
  if (minstance == NULL) {
    minstance = new ServerUMS();
  }
  return minstance;
}

/**
* \brief To get the vishnuId
* \fn int getVishnuId()
* \return the path of the configuration file
*/
int
ServerUMS::getVishnuId() const {
  return mvishnuId;
}

/**
* \brief To get the path to the sendmail script
* \fn std::string getSendmailScriptPath()
* \return the path of the configuration file
*/
std::string
ServerUMS::getSendmailScriptPath() const {
  return msendmailScriptPath;
}
/**
* \brief Constructor (private)
* \fn ServerUMS()
*/
ServerUMS::ServerUMS() : {
}

/**
* \brief To initialize the UMS server with individual parameters
* \fn int init(std::string vishnuid)
* \param vishnuId The id of the vishnu configuration registered in the database
* \param dbConfig  The configuration of the database
* \param sendmailScriptPath The path to the script for sending emails
* \param authenticatorConfig The configuration of the authenticator
* \return an error code (0 if success and 1 if an error occurs)
*/
int
ServerUMS::init(int vishnuId,
                DbConfiguration dbConfig,
                std::string sendmailScriptPath,
                AuthenticatorConfiguration authenticatorConfig) {

  msendmailScriptPath = sendmailScriptPath;

  DbFactory factory;
  AuthenticatorFactory authfactory;

  try {
    mdatabaseVishnu = factory.createDatabaseInstance(dbConfig);
    mauthenticator = authfactory.createAuthenticatorInstance(authenticatorConfig);

    mvishnuId = vishnuId;

    std::string sqlCommand("SELECT * FROM vishnu where vishnuid="+convertToString(mvishnuId));

    /*connection to the database*/
    mdatabaseVishnu->connect();
    mmapperTMS = new TMSMapper(MapperRegistry::getInstance(), TMSMAPPERNAME);
    mmapperTMS->registerMapper();
    mmapperIMS = new IMSMapper(MapperRegistry::getInstance(), IMSMAPPERNAME);
    mmapperIMS->registerMapper();
    mmapperFMS = new FMSMapper(MapperRegistry::getInstance(), FMSMAPPERNAME);
    mmapperFMS->registerMapper();
    mmapper = new UMSMapper(MapperRegistry::getInstance(), UMSMAPPERNAME);
    mmapper->registerMapper();

    /* Checking of vishnuid on the database */
    boost::scoped_ptr<DatabaseResult> result(mdatabaseVishnu->getResult(sqlCommand.c_str()));
    if (result->getResults().size() == 0) {
      SystemException e(ERRCODE_DBERR, "The vishnuid is unrecognized");
      throw e;
    }

  } catch (VishnuException& e) {
      std::cout << e.what() << std::endl;
      exit(0);
  }

  // initialization of the service table
//  diet_service_table_init(NB_SRV);

  /* solveSessionConnect */

// initialization of the service table
  initMap();

//  /* solveSessionConnect */
//
//  mprofile = diet_profile_desc_alloc(SRV[0], 4, 4, 6);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,4),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,5),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,6),DIET_STRING, DIET_CHAR);
//
//  if (diet_service_table_add(mprofile, NULL, solveSessionConnect)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* solveSessionReconnect */
//
//  mprofile = diet_profile_desc_alloc(SRV[1], 4, 4, 6);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,4),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,5),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,6),DIET_STRING, DIET_CHAR);
//
//  if (diet_service_table_add(mprofile, NULL, solveSessionReconnect)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* solveSessionClose */
//
//  mprofile = diet_profile_desc_alloc(SRV[2], 0, 0, 1);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//
//  if (diet_service_table_add(mprofile, NULL, solveSessionClose)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//  /* solveUserCreate */
//
//  mprofile = diet_profile_desc_alloc(SRV[3], 1, 1, 3);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);
//
//  if (diet_service_table_add(mprofile, NULL, solveUserCreate)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//  /* solveUserUpdate */
//
//  mprofile = diet_profile_desc_alloc(SRV[4], 1, 1, 2);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
//
//  if (diet_service_table_add(mprofile, NULL, solveUserUpdate)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* solveUserDelete */
//
//  mprofile = diet_profile_desc_alloc(SRV[5], 1, 1, 2);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
//
//  if (diet_service_table_add(mprofile, NULL, solveUserDelete)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* solveUserPasswordChange */
//
//  mprofile = diet_profile_desc_alloc(SRV[6], 2, 2, 3);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);
//
//  if (diet_service_table_add(mprofile, NULL, solveUserPasswordChange)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* solveUserPasswordReset */
//
//  mprofile = diet_profile_desc_alloc(SRV[7], 1, 1, 3);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);
//
//  if (diet_service_table_add(mprofile, NULL, solveUserPasswordReset)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* solveMachineCreate */
//
//  mprofile = diet_profile_desc_alloc(SRV[8], 1, 1, 3);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);
//
//  if (diet_service_table_add(mprofile, NULL, solveMachineCreate)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* solveMachineUpdate */
//
//  mprofile = diet_profile_desc_alloc(SRV[9], 1, 1, 2);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
//
//  if (diet_service_table_add(mprofile, NULL, solveMachineUpdate)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* solveMachineDelete */
//
//  mprofile = diet_profile_desc_alloc(SRV[10], 1, 1, 2);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
//
//  if (diet_service_table_add(mprofile, NULL, solveMachineDelete)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* solveLocalAccountCreate */
//
//  mprofile = diet_profile_desc_alloc(SRV[11], 1, 1, 3);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);
//
//  if (diet_service_table_add(mprofile, NULL, solveLocalAccountCreate)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* solveLocalAccountUpdate */
//
//  mprofile = diet_profile_desc_alloc(SRV[12], 1, 1, 2);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
//
//  if (diet_service_table_add(mprofile, NULL, solveLocalAccountUpdate)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* solveLocalAccountDelete */
//
//  mprofile = diet_profile_desc_alloc(SRV[13], 2, 2, 3);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);
//
//  if (diet_service_table_add(mprofile, NULL, solveLocalAccountDelete)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* solveConfigurationSave */
//
//  mprofile = diet_profile_desc_alloc(SRV[14], 0, 0, 2);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
//
//  if (diet_service_table_add(mprofile, NULL, solveConfigurationSave)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* solveConfigurationRestore */
//
//  mprofile = diet_profile_desc_alloc(SRV[15], 1, 1, 2);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
//
//  if (diet_service_table_add(mprofile, NULL, solveConfigurationRestore)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* solveOptionValueSet */
//
//  mprofile = diet_profile_desc_alloc(SRV[16], 1, 1, 2);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
//
//  if (diet_service_table_add(mprofile, NULL, solveOptionValueSet)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* solveOptionValueSetDefault */
//
//  mprofile = diet_profile_desc_alloc(SRV[17], 1, 1, 2);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
//
//  if (diet_service_table_add(mprofile, NULL, solveOptionValueSetDefault)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* solveListSessions */
//
//  mprofile = diet_profile_desc_alloc(SRV[18], 1, 1, 3);
//  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
//
//  if (diet_service_table_add(mprofile, NULL, solveListSessions)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* solveListLocalAccount */
//
//  mprofile = diet_profile_desc_alloc(SRV[19], 1, 1, 3);
//  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
//
//  if (diet_service_table_add(mprofile, NULL, solveListLocalAccount)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* solveListMachines */
//
//  mprofile = diet_profile_desc_alloc(SRV[20], 1, 1, 3);
//  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
//
//  if (diet_service_table_add(mprofile, NULL, solveListMachines)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* solveListHistoryCmd */
//
//  mprofile = diet_profile_desc_alloc(SRV[21], 1, 1, 3);
//  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
//
//  if (diet_service_table_add(mprofile, NULL, solveListHistoryCmd)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* solveListOptions */
//
//  mprofile = diet_profile_desc_alloc(SRV[22], 1, 1, 3);
//  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
//
//  if (diet_service_table_add(mprofile, NULL, solveListOptions)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//  /* solveListUsres */
//
//  mprofile = diet_profile_desc_alloc(SRV[23], 1, 1, 3);
//  diet_generic_desc_set(diet_param_desc(mprofile,0), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2), DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3), DIET_STRING, DIET_CHAR);
//
//  if (diet_service_table_add(mprofile, NULL, solveListUsers)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//  /* solveRestore */
//
//  mprofile = diet_profile_desc_alloc(SRV[24], 0, 0, 1);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  if (diet_service_table_add(mprofile, NULL, solveRestore)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* solveSystemAuthCreate */
//
//  mprofile = diet_profile_desc_alloc(SRV[25], 1, 1, 3);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);
//  if (diet_service_table_add(mprofile, NULL, solveSystemAuthCreate)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//
//  /* solveSystemAuthUpdate */
//
//  mprofile = diet_profile_desc_alloc(SRV[26], 1, 1, 2);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
//  if (diet_service_table_add(mprofile, NULL, solveSystemAuthUpdate)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* solveSystemAuthDelete */
//
//  mprofile = diet_profile_desc_alloc(SRV[27], 1, 1, 2);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
//  if (diet_service_table_add(mprofile, NULL, solveSystemAuthDelete)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//  /* solveSystemAuthList */
//
//  mprofile = diet_profile_desc_alloc(SRV[28], 1, 1, 3);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);
//  if (diet_service_table_add(mprofile, NULL, solveSystemAuthList)) {
//    return 1;
//  }
//  diet_profile_desc_free(mprofile);
//
//
//  /* solveAccountAuthCreate */
//
//  mprofile = diet_profile_desc_alloc(SRV[29], 1, 1, 2);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
//  if (diet_service_table_add(mprofile, NULL, solveAccountAuthCreate)) {
//    return 1;
//  }
//
//  /* solveAccountAuthUpdate */
//
//  mprofile = diet_profile_desc_alloc(SRV[30], 1, 1, 2);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
//  if (diet_service_table_add(mprofile, NULL, solveAccountAuthUpdate)) {
//    return 1;
//  }
//
//  /* solveAccountAuthDelete */
//
//  mprofile = diet_profile_desc_alloc(SRV[31], 2, 2, 3);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
//  if (diet_service_table_add(mprofile, NULL, solveAccountAuthDelete)) {
//    return 1;
//  }
//
//  /* solveAccountAuthList */
//
//  mprofile = diet_profile_desc_alloc(SRV[32], 1, 1, 3);
//  diet_generic_desc_set(diet_param_desc(mprofile,0),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,1),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,2),DIET_STRING, DIET_CHAR);
//  diet_generic_desc_set(diet_param_desc(mprofile,3),DIET_STRING, DIET_CHAR);
//  if (diet_service_table_add(mprofile, NULL, solveAccountAuthList)) {
//    return 1;
//  }

  return 0;
}

/**
* \fn ~ServerUMS()
* \brief Destructor, raises an exception on error
*/
ServerUMS::~ServerUMS() {
  if (mmapper != NULL) {
    delete mmapper;
  }
  if (mdatabaseVishnu != NULL) {
    delete mdatabaseVishnu;
  }
  if (mauthenticator != NULL) {
    delete mauthenticator;
  }
}

void
ServeurUMS::initMap() {
  int (*functionPtr)(diet_profile_t*);

  functionPtr = solveSessionConnect;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[0]),functionPtr));
  functionPtr = solveSessionReconnect;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[1]),functionPtr));
  functionPtr = solveSessionClose;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[2]),functionPtr));
  functionPtr = solveUserCreate;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[3]),functionPtr));
  functionPtr = solveUserUpdate;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[4]),functionPtr));
  functionPtr = solveUserDelete;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[5]),functionPtr));
  functionPtr = solveUserPasswordChange;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[6]),functionPtr));
  functionPtr = solveUserPasswordReset;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[7]),functionPtr));
  functionPtr = solveMachineCreate;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[8]),functionPtr));
  functionPtr = solveMachineUpdate;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[9]),functionPtr));
  functionPtr = solveMachineDelete;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[10]),functionPtr));
  functionPtr = solveLocalAccountCreate;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[11]),functionPtr));
  functionPtr = solveLocalAccountUpdate;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[12]),functionPtr));
  functionPtr = solveLocalAccountDelete;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[13]),functionPtr));
  functionPtr = solveConfigurationSave;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[14]),functionPtr));
  functionPtr = solveConfigurationRestore;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[15]),functionPtr));
  functionPtr = solveOptionValueSet;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[16]),functionPtr));
  functionPtr = solveOptionValueSetDefault;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[17]),functionPtr));
  functionPtr = solveListSessions;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[18]),functionPtr));
  functionPtr = solveListLocalAccount;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[19]),functionPtr));
  functionPtr = solveListMachines;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[20]),functionPtr));
  functionPtr = solveListHistoryCmd;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[21]),functionPtr));
  functionPtr = solveListOptions;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[22]),functionPtr));
  functionPtr = solveListUsers;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[23]),functionPtr));
  functionPtr = solveRestore;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[24]),functionPtr));
  functionPtr = solveSystemAuthCreate;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[25]),functionPtr));
  functionPtr = solveSystemAuthUpdate;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[26]),functionPtr));
  functionPtr = solveSystemAuthDelete;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[27]),functionPtr));
  functionPtr = solveSystemAuthList;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[28]),functionPtr));
  functionPtr = solveAccountAuthCreate;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[29]),functionPtr));
  functionPtr = solveAccountAuthUpdate;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[30]),functionPtr));
  functionPtr = solveAccountAuthDelete;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[31]),functionPtr));
  functionPtr = solveAccountAuthList;
  mcb.insert( pair<string, (*functionPtr)(diet_profile_t*)> (string(SRV[32]),functionPtr));

}

int
ServeurUMS::call(diet_profile_t* profile){
  map<string, (*functionPtr)(diet_profile_t*)>::iterator it;
  it = mcb.find(string(profile->name));
  if (it == std::map::end) {
    return UNKNOWN_SERVICE;
  }
  int (*functionPtr)(diet_profile_t*);
  functionPtr = it->second;
  return (*functionPtr)(profile);
}


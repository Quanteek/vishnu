
#include <iostream>
#include <string>
#include "SysInfoProxy.hpp"
#include "utilClient.hpp"
#include "utilVishnu.hpp"
#include "QueryProxy.hpp"
#include "IMSServices.hpp"

using namespace vishnu;

/**
* \param session The object which encapsulates the session information
* \brief Constructor
*/
SysInfoProxy::SysInfoProxy(const SessionProxy& session): msessionProxy(session) {
}


/**
* \brief Function to set a system information
* \param systemInfo the system information
* \return raises an exception on error
*/
void
SysInfoProxy::setSystemInfo(IMS_Data::SystemInfo systemInfo) {

  diet_profile_t* profile = NULL;
  std::string sessionKey;
  std::string errorInfo;


  std::string serviceName = SERVICES_IMS[INT_SETSYSTEMINFO];

  profile = diet_profile_alloc(serviceName.c_str(), 1, 1, 2);
  sessionKey = msessionProxy.getSessionKey();

  std::string msgErrorDiet = "call of function diet_string_set is rejected ";
  //IN Parameters
  if (diet_string_set(profile,0, sessionKey.c_str())) {
    msgErrorDiet += "with sessionKey parameter "+sessionKey;
    raiseDietMsgException(msgErrorDiet);
  }

  ::ecorecpp::serializer::serializer _ser;
  //To serialize the options object in to optionsInString
  std::string systemInfoToString =  _ser.serialize_str(const_cast<IMS_Data::SystemInfo_ptr>(&systemInfo));

  if (diet_string_set(profile,1, systemInfoToString.c_str())) {
    msgErrorDiet += "with SystemInfo parameter ";
    raiseDietMsgException(msgErrorDiet);
  }

  //OUT Parameters
  diet_string_set(profile,2);

  if(!diet_call(profile)) {
    if(diet_string_get(profile,2, errorInfo)){
      msgErrorDiet += " by receiving errorInfo message";
      raiseDietMsgException(msgErrorDiet);
    }
  }
  else {
    raiseDietMsgException("VISHNU call failure");
  }

  /*To raise a vishnu exception if the receiving message is not empty*/
  raiseExceptionIfNotEmptyMsg(errorInfo);

  diet_profile_free(profile);
}

 /**
* \brief Function to get a system information
* \param options the options data structure to get system information
* \param listSysInfo the list of system information returned
* \return raises an exception on error
*/
int
SysInfoProxy::getSystemInfo(IMS_Data::ListSysInfo& listSysInfo,
                            const IMS_Data::SysInfoOp& options) {

  std::string name = SERVICES_IMS[INT_GETSYSTEMINFO];
  QueryProxy<IMS_Data::SysInfoOp, IMS_Data::ListSysInfo>
  query(options, msessionProxy, name);

  IMS_Data::ListSysInfo_ptr li = query.list();

  if(li != NULL) {
    IMS_Data::IMS_DataFactory_ptr ecoreFactory = IMS_Data::IMS_DataFactory::_instance();
    for(unsigned int i = 0; i < li->getSysInfo().size(); i++) {
      IMS_Data::SystemInfo_ptr systemInfo = ecoreFactory->createSystemInfo();
       //To copy the content and not the pointer
      *systemInfo = *li->getSysInfo().get(i);
      listSysInfo.getSysInfo().push_back(systemInfo);
    }
    delete li;
  }
  return 0;
}

/**
* \brief Destructor
*/
SysInfoProxy::~SysInfoProxy() {
}

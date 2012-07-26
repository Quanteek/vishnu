#ifndef __SED__H__
#define __SED__H__

#include "DIET_client.h"
#include <map>
#include <string>

#define UNKNOWN_SERVICE 1;

typedef int (*functionPtr_t)(diet_profile_t*);
/**
 * \class SeD
 * \brief base class to Server*MS classes
 */
class SeD {
public:
  /**
   * \brief To call a function upon receiving a request
   * \param profile The profile of the service
   * \return the error code of the function
   */
  virtual int
  call(diet_profile_t* prof);

protected:
  /**
   * \brief map with function ptr for callback
   */
  std::map<std::string, functionPtr_t> mcb;
};

int
ZMQServerStart(boost::shared_ptr<SeD> server, std::string addr, int port);

#endif // __SED__H__
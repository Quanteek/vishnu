/**
 * \file set_update_frequency.cpp
 * This file defines the VISHNU command to get the metric history
 * \author Coulomb Kevin (kevin.coulomb@sysfera.com)
 */


#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "api_ums.hpp"
#include "api_ims.hpp"
#include "sessionUtils.hpp"

#include "GenericCli.hpp"

using namespace std;
using namespace vishnu;

struct MachineIdentifierFunc {

  std::string mmachineFormat;

  MachineIdentifierFunc(const std::string& machineFormat):
   mmachineFormat(machineFormat)
  {};

  int operator()(std::string sessionKey) {
    return defineMachineIdentifier(sessionKey, mmachineFormat);
  }
};


int main (int argc, char* argv[]){
  
  /******* Parsed value containers ****************/
  string dietConfig;
  string machineFormat;

  /**************** Describe options *************/
  boost::shared_ptr<Options> opt(new Options(argv[0]));

  // Environement option
  opt->add("dietConfig,c",
           "The diet config file",
           ENV,
           dietConfig);

  // All cli obligatory parameters
  opt->add("format,f",
	   "The id of the machine on which the machine format must be defined",
	   HIDDEN,
	   machineFormat,1);
  opt->setPosition("format",1);

  bool isEmpty;
  //To process list options
  GenericCli().processListOpt(opt, isEmpty, argc, argv, "format");

  //call of the api function
  MachineIdentifierFunc jobIdFunc(machineFormat);
  return GenericCli().run(jobIdFunc, dietConfig, argc, argv); 

}


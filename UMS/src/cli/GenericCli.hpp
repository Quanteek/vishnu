
#include "CLICmd.hpp"
#include "utilVishnu.hpp"
#include "cliError.hpp"
#include "cliUtil.hpp"
#include "sessionUtils.hpp"
#include <boost/bind.hpp>

namespace po = boost::program_options;

using namespace std;
using namespace vishnu;

/**
 * \brief A generic class for common operations in command line
 */
class GenericCli {

  public:
/**
 * \brief generic function to perform api function in command line
 * \param function the api function
 * \param dietConfig the diet client config file path
 * \param ac the number of command line arguments
 * \param av the command line arguments
 * \return 0 if it succeeds or an error code otherwise
 */

    template <class ApiFunc>
    int run(ApiFunc function, std::string dietConfig, int ac, char*  av[]) {

      try{

        // initializing DIET

        if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {

          errorUsage(av[0],dietErrorMsg,EXECERROR);

          return  CLI_ERROR_DIET ;
        }

        // get the sessionKey
        std::string sessionKey=getLastSessionKey(getppid());

        // DIET call 
        if(false==sessionKey.empty()){
          printSessionKeyMessage();
          //call of the api fuction
          function(sessionKey);

          printSuccessMessage(); 

        }
      } catch(VishnuException& e){// catch all Vishnu runtime error
        std::string  msg = e.getMsg()+" ["+e.getMsgComp()+"]";
        errorUsage(av[0], msg,EXECERROR);
        //check the bad session key
        if (checkBadSessionKeyError(e)){
          removeBadSessionKeyFromFile(getppid());
        }
        return e.getMsgI() ;
      } catch(std::exception& e){// catch all std runtime error
        errorUsage(av[0],e.what());
        return CLI_ERROR_RUNTIME;
      }

    }

    /**
 * \brief generic function to perform api function in command line without the
 * session management 
 * \param function the api function
 * \param dietConfig the diet client config file path
 * \param ac the number of command line arguments
 * \param av the command line arguments
 * \return 0 if it succeeds or an error code otherwise
 */


    template <class ApiFunc>
    int runWithoutSessionKey(ApiFunc function, std::string dietConfig, int ac, char*  av[]) {

      try{

        // initializing DIET

        if (vishnuInitialize(const_cast<char*>(dietConfig.c_str()), ac, av)) {

          errorUsage(av[0],dietErrorMsg,EXECERROR);

          return  CLI_ERROR_DIET ;
        }

          //call of the api fuction
          function();

          printSuccessMessage(); 

      } catch(VishnuException& e){// catch all Vishnu runtime error
        std::string  msg = e.getMsg()+" ["+e.getMsgComp()+"]";
        errorUsage(av[0], msg,EXECERROR);
        return e.getMsgI() ;
      } catch(std::exception& e){// catch all std runtime error
        errorUsage(av[0],e.what());
        return CLI_ERROR_RUNTIME;
      }

    }

    /**
     * \brief An helpul function to parse and check command line options
     * \param opt command line options 
     * \param isEmpty in out variable to check if av does not contain options
     * \param ac the the number of command line arguments
     *  \param av the command line arguments
     */

    void processListOpt(const boost::shared_ptr<Options>& opt, bool& isEmpty, int ac, char*  av[]) {
    
      CLICmd cmd = CLICmd (ac, av, opt);
      try {
        opt->parse_cli(ac,av);
      } catch(po::error& e){ // catch all other bad parameter errors
        helpUsage(*opt,"[option]");
        exit(CLI_ERROR_INVALID_PARAMETER);
      }

      isEmpty=opt->empty();//if no value was given in the command line
      // Parse the cli and setting the options found
     
      int ret = cmd.parse(env_name_mapper());

      if (ret != CLI_SUCCESS){
        helpUsage(*opt,"[option]");
        exit(ret);
      }

      // PreProcess (adapt some parameters if necessary)
      checkVishnuConfig(*opt);
      if ( opt->count("help")){
        helpUsage(*opt,"[option]");
        exit(0);
      }

    }
};

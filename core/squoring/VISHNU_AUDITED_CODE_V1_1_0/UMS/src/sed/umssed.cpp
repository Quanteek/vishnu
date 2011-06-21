#include <csignal>
#include <sys/wait.h>
#include <fstream>
#include "ServerUMS.hpp"
#include "MonitorUMS.hpp"
#include "ExecConfiguration.hpp"
#include "DbConfiguration.hpp"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/scoped_ptr.hpp>

using namespace vishnu;

/**
 * \brief To show how to use the sed
 * \fn int usage(char* cmd)
 * \param cmd The name of the program
 * \return Always 1
 */
int
usage(char* cmd) {
  std::cout << "Usage: " << std::string(cmd) << " vishnu_config.cfg\n";
  return 1;
}


/**
 * \brief To catch a signal
 * \param signum is the signal to catch
 */
void
controlSignal (int signum) {
  int res;
  switch (signum) {
    case SIGCHLD:
      res = waitpid (-1, NULL, WNOHANG);
      while (res > 0) {
        res = waitpid (-1, NULL, WNOHANG);
      }
      break;
    default:
     break;
  }
}

/**
 * \brief The main function
 * \fn int main(int argc, char* argv[], char* envp[])
 * \param argc Number of parameter
 * \param argv List of argument
 * \param envp Array of environment variables
 * \return The result of the diet sed call
 */
int main(int argc, char* argv[], char* envp[]) {

  int res = 0;
  int vishnuId = 0;
  ExecConfiguration config;
  DbConfiguration dbConfig(config);
  std::string dietConfigFile;
  std::string sendmailScriptPath;
  struct sigaction action;
  string UMSTYPE = "UMS";

  if (argc != 2) {
    return usage(argv[0]);
  }

  // Read the configuration
  try {
    config.initFromFile(argv[1]);
    config.getRequiredConfigValue<std::string>(vishnu::DIETCONFIGFILE, dietConfigFile);
    config.getRequiredConfigValue<int>(vishnu::VISHNUID, vishnuId);
    dbConfig.check();
    config.getRequiredConfigValue<std::string>(vishnu::SENDMAILSCRIPT, sendmailScriptPath);
    if(!boost::filesystem::is_regular_file(sendmailScriptPath)) {
      std::cerr << "Error: cannot open the script file for sending email" << std::endl;
      exit(1);
    }
  } catch (UserException& e) {
    std::cerr << e.what() << std::endl;
    exit(1);
  }


  // Fork a child for UMS monitoring
  pid_t pid;
  pid_t ppid;
  pid = fork();

  if (pid > 0) {
    //Initialize the UMS Server (Opens a connection to the database)
    boost::scoped_ptr<ServerUMS> server(ServerUMS::getInstance());
    res = server->init(vishnuId, dbConfig, sendmailScriptPath);

    //Declaration of signal handler
    action.sa_handler = controlSignal;
    sigemptyset (&(action.sa_mask));
    action.sa_flags = 0;
    sigaction (SIGCHLD, &action, NULL);

    // Initialize the DIET SeD
    if (!res) {
      diet_print_service_table();
      res = diet_SeD(dietConfigFile.c_str(), argc, argv);
    } else {
      std::cerr << "There was a problem during services initialization" << std::endl;
      exit(1);
    }
  }
  else if (pid == 0) {
    // Initialize the UMS Monitor (Opens a connection to the database)
    MonitorUMS monitor;
    monitor.init(vishnuId, dbConfig);
    ppid = getppid();

    while(kill(ppid,0) == 0) {
      monitor.run();
    }
  } else {
    std::cerr << "There was a problem to initialize the server" << std::endl;
    exit(1);
  }
  return res;
}

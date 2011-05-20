/**
 * \file cmdArgs.cpp
 * This file defines the VISHNU submit job command 
 * \author Daouda Traore (daouda.traore@sysfera.com)
 */

#ifndef CMD_ARGS
#define CMD_ARGS

#include <boost/bind.hpp>

namespace po = boost::program_options;

using namespace std;

/**
 * \brief To build options for the VISHNU cmdArgs command
 * \param pgName : The name of the command
 * \param dietConfig: Represents the VISHNU config file
 */
boost::shared_ptr<Options>
processOpt(string pgName,
           string& dietConfig);

#endif

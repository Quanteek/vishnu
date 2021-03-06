/**
 * \file AnnuaryWorker.hpp
 * \brief This file contains the implementation of the workers for the annuary class
 * \author Haikel Guemar (haikel.guemar@sysfera.com)
 * \date January 2013
 */
#ifndef _ANNUARYWORKERS_HPP_
#define _ANNUARYWORKERS_HPP_

#include "Worker.hpp"
#include "DIET_client.h"
#include "UserException.hpp"
#include "Annuary.hpp"

/**
 * \class AnnuaryWorker
 * \brief Base class for workers having an Annuary
 */
class AnnuaryWorker : public Worker {
public:
  /**
   * \brief Constructor
   * \param ctx the zmq context
   * \param uriInproc the uri for the inproc commands
   * \param id the id of the worker
   * \param ann the annuary it is linked
   */
  explicit AnnuaryWorker(boost::shared_ptr<zmq::context_t> ctx,
                         const std::string& uriInproc, int id,
                         boost::shared_ptr<Annuary> ann)
    : Worker(ctx, uriInproc, id), mann_(ann) {}

protected:
  /**
   * \brief The annuary it is working for
   */
  boost::shared_ptr<Annuary> mann_;
};




/**
 * \class ServiceWorker
 * \brief Base class for workers realizing a service
 */
class ServiceWorker : public AnnuaryWorker {
public:

  /**
   * @brief Constructor
   * @param ctx the zmq context
   * @param uriInproc the uri for the inproc commands
   * @param id the id of the worker
   * @param ann the annuary it is linked
   * @param usessl Sets whether to use SSL
   * @param certCaFile Sets the SSL CA file
   */
  explicit ServiceWorker(boost::shared_ptr<zmq::context_t> ctx,
                         const std::string& uriInproc,
                         int id,
                         boost::shared_ptr<Annuary> ann,
                         bool usessl,
                         const std::string& certCaFile)
    : AnnuaryWorker(ctx, uriInproc, id, ann), useSsl(usessl), cafile(certCaFile) {}

private:
  /**
   * \brief  path to the CA file
  */
  bool useSsl;

  /**
   * \brief  path to the CA file
  */
  std::string cafile;

  /**
   * \brief Call the function
   * \param data the serialized data containing the funcion and its parameters
   * \return the serialized data (out data are updated)
   */
  std::string
  doCall(std::string& data) {
    using boost::format;
    using boost::str;

    boost::shared_ptr<diet_profile_t> profile = my_deserialize(data);
    std::string servname = profile->name;
    std::vector<boost::shared_ptr<Server> > serv = mann_->get(servname);
    std::string uriServer = elect(serv);

    if (!uriServer.empty()) {
      if (useSsl) {
        int port = getPortFromUri(uriServer);
        std::string host = getHostFromUri(uriServer);
        ssl_call_gen(profile.get(), host, port, cafile);
      } else {
        diet_call_gen(profile.get(), uriServer);
      }
      return my_serialize(profile.get());
    } else {
      return str(format("error %1%: the service %2% is not available")
                 % ERRCODE_INVALID_PARAM
                 % servname);
    }
  }

  /**
   * \brief Elect a server
   * \param serv list of eligible servers
   * \return the choosen one
   */
  std::string
  elect(const std::vector<boost::shared_ptr<Server> >& serv){
    if (serv.empty()) {
      return "";
    }
    return serv.at(0)->getURI();
  }

};


/**
 * \class SubscriptionWorker
 * \brief Worker handling the subscription to the Annuary
 */
class SubscriptionWorker : public AnnuaryWorker {
public:

  /**
   * @brief SubscriptionWorker
   * @param ctx
   * @param uriInproc
   * @param id
   * @param ann
   * @param usessl
   * @param certCaFile
   */
  explicit SubscriptionWorker(boost::shared_ptr<zmq::context_t> ctx,
                              const std::string& uriInproc, int id,
                              boost::shared_ptr<Annuary> ann,
                              bool usessl,
                              const std::string& certCaFile)
    : AnnuaryWorker(ctx, uriInproc, id, ann), useSsl(usessl), cafile(certCaFile) {}

private:
  /**
   * \brief  path to the CA file
  */
  bool useSsl;

  /**
   * \brief  path to the CA file
  */
  std::string cafile;
  /**
   * \brief Call the function
   * \param data the serialized data containing the funcion and its parameters
   * \return the serialized data (out data are updated)
   */
  std::string
  doCall(std::string& data) {
    int mode = boost::lexical_cast<int>(data.substr(0,1));

    // Deserialize
    boost::shared_ptr<Server> server = Server::fromString(data.substr(1));

    if (mode == 1) {
      // adding a new server
      mann_->add(server->getName(), server->getURI(),
                 server->getServices());
    } else if (mode == 0) {
      mann_->remove(server->getName(), server->getURI());
    }

    return "OK";
  }

};

#endif /* _ANNUARYWORKERS_HPP_ */

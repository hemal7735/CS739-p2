#include <iostream>
#include <memory>
#include <string>
#include <sys/stat.h>

#include <grpcpp/ext/proto_server_reflection_plugin.h>
#include <grpcpp/grpcpp.h>
#include <grpcpp/health_check_service_interface.h>
#include "afs.grpc.pb.h"

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using namespace afs;
using namespace std;

// Logic and data behind the server's behavior.
class AFSServiceImpl final : public AFS::Service {

  const char *serverPath = "/home/hemalkumar/hemal/server";

  Status DeleteFile(ServerContext* context, const DeleteFileRequest* request,
                  DeleteFileReply* reply) override {
    std:string path = request->path();

    reply->set_error(0);

    return Status::OK;
  }

  Status MakeDir(ServerContext* context, const MakeDirRequest* request,
                  MakeDirReply* reply) override {

    int res = mkdir((serverPath + request->path()).c_str(), request->mode());
    if (res == -1) {
      printf("Error in mkdir ErrorNo: %d\n",errno);
      perror(strerror(errno));
      reply->set_error(errno);
    } else {
      printf("MakeDirectory success\n");
      reply->set_error(0);
     }
    return Status::OK;
  }

  Status DeleteDir(ServerContext* context, const DeleteDirRequest* request,
                  DeleteDirReply* reply) override {

    int res = rmdir((serverPath + request->path()).c_str());
    if (res == -1) {
      cout << "Error in DeleteDir ErrorNo: " << errno << endl;
      perror(strerror(errno));
      reply->set_error(errno);
    } else {
      cout << "DeleteDir success" << endl;
      reply->set_error(0);
     }
    return Status::OK;
  }

};

void RunServer() {
  std::string server_address("0.0.0.0:50051");
  AFSServiceImpl service;

  grpc::EnableDefaultHealthCheckService(true);
  grpc::reflection::InitProtoReflectionServerBuilderPlugin();
  ServerBuilder builder;
  // Listen on the given address without any authentication mechanism.
  builder.AddListeningPort(server_address, grpc::InsecureServerCredentials());
  // Register "service" as the instance through which we'll communicate with
  // clients. In this case it corresponds to an *synchronous* service.
  builder.RegisterService(&service);
  // Finally assemble the server.
  std::unique_ptr<Server> server(builder.BuildAndStart());
  std::cout << "Server listening on " << server_address << std::endl;

  // Wait for the server to shutdown. Note that some other thread must be
  // responsible for shutting down the server for this call to ever return.
  server->Wait();
}

int main(int argc, char** argv) {
  RunServer();

  return 0;
}

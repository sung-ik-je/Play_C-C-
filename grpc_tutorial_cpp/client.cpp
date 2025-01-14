#include <iostream>
#include <memory>
#include <string>
#include <grpcpp/grpcpp.h>
#include "test.grpc.pb.h"

using grpc::Channel;
using grpc::ClientContext;
using grpc::Status;
using grpc_tutorial_cpp::Greeter;
using grpc_tutorial_cpp::HelloReply;
using grpc_tutorial_cpp::HelloRequest;

class GreeterClient {
 public:
  GreeterClient(std::shared_ptr<Channel> channel)
      : stub_(Greeter::NewStub(channel)) {}

  std::string SayHello(const std::string& user) {
    HelloRequest request;
    request.set_name(user);

    HelloReply reply;
    ClientContext context;

    Status status = stub_->SayHello(&context, request, &reply);

    if (status.ok()) {
      return reply.message();
    } else {
      std::cerr << "RPC failed: " << status.error_message() << std::endl;
      return "RPC failed";
    }
  }

 private:
  std::unique_ptr<Greeter::Stub> stub_;
};

int main(int argc, char** argv) {
  GreeterClient greeter(
      grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials()));
  std::string user("World");
  std::string reply = greeter.SayHello(user);
  std::cout << "Greeter received: " << reply << std::endl;

  return 0;
}
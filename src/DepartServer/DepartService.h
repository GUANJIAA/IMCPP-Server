#ifndef __DEPARTSERVICE_H__
#define __DEPARTSERVICE_H__

#include "departmodel.h"
#include "depart.h"
#include "Depart.pb.h"

class DepartService : public DepartProto::DepartServiceRpc
{
public:
    bool CreateDepart(Depart &depart, DepartProto::ResultCode *code);

    bool DelDepart(std::string departName, std::string userName, DepartProto::ResultCode *code);

    bool AddDepart(std::string departName,
                   std::string userName,
                   std::string userRole,
                   DepartProto::ResultCode *code);

    bool QuitDepart(std::string departName, std::string userName, DepartProto::ResultCode *code);

    bool QueryDepart(std::string userName,
                     DepartProto::DepartInfo &Departs,
                     DepartProto::ResultCode *code);

    bool QueryDepartUsers(std::string departName,
                          std::string userName,
                          std::vector<std::string> &departUsers,
                          DepartProto::ResultCode *code);

    virtual void CreateDepart(::google::protobuf::RpcController *controller,
                              const ::DepartProto::CreateDepartRequest *request,
                              ::DepartProto::CreateDepartResponse *response,
                              ::google::protobuf::Closure *done) override;

    virtual void DelDepart(::google::protobuf::RpcController *controller,
                           const ::DepartProto::DelDepartRequest *request,
                           ::DepartProto::DelDepartResponse *response,
                           ::google::protobuf::Closure *done) override;

    virtual void AddDepart(::google::protobuf::RpcController *controller,
                           const ::DepartProto::AddDepartRequest *request,
                           ::DepartProto::AddDepartResponse *response,
                           ::google::protobuf::Closure *done) override;

    virtual void QuitDepart(::google::protobuf::RpcController *controller,
                            const ::DepartProto::QuitDepartRequest *request,
                            ::DepartProto::QuitDepartResponse *response,
                            ::google::protobuf::Closure *done)override;

    virtual void QueryDepart(::google::protobuf::RpcController *controller,
                             const ::DepartProto::QueryDepartRequest *request,
                             ::DepartProto::QueryDepartResponse *response,
                             ::google::protobuf::Closure *done) override;

    virtual void QueryDepartUsers(::google::protobuf::RpcController *controller,
                                  const ::DepartProto::QueryDepartUsersRequest *request,
                                  ::DepartProto::QueryDepartUsersResponse *response,
                                  ::google::protobuf::Closure *done) override;

private:
    DepartModel departmodel;
};

#endif
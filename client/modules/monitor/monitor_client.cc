// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: guoqingao@yunrang.com (Guoqing Ao)

#include "base/yr.h"
#include "infrastructure/client/modules/monitor/monitor_client.h"

namespace infra {
namespace client {
namespace monitor {

MonitorClient::MonitorClient(string server_name, int server_port)
              : server_name_(server_name), server_port_(server_port) {
    Init();
}

MonitorClient::~MonitorClient(){
}

void MonitorClient::Init() {
    CHECK(!server_name_.empty());
}

bool MonitorClient::SetServiceStatus(const ServiceStatus &service_status) {
    alarm_client_.reset(new base::ThriftClient<MonitorServiceClient>(
                        server_name_, server_port_));
    CHECK(alarm_client_ != NULL)
    MonitorServiceClient *my_client = alarm_client_->GetService();
    if (my_client == NULL) {
        LOG(ERROR) << "Init my client failed!";
        return false;
    }

    try {
        my_client->SetServiceStatus(service_status);
    } catch(const apache::thrift::TException &e) {
        alarm_client_->CallServiceFailed();
        LOG(ERROR) << e.what() << "call rpc failed!";
        return false;
    }
    return true;
}

ServiceEntry MonitorClient::GetServiceEntry(const std::string &host_name) {
    call_client_.reset(new base::ThriftClient<MonitorEntryClient>(
                      server_name_, server_port_))
    CHECK(call_client_ != NULL)
    MonitorEntryClient *my_client = call_client_->GetService();
    if (my_client == NULL) {
        LOG(ERROR) << "Init my client failed";
        return false;
    }

    try {
        std::vector<ServiceEntry> entries;
        my_client->GetServiceEntry(entries,host_name);
        Singleton<ConfData>::get()->UpdateCache(entries);
    } catch(const apache::thrift::TException &e) {
        LOG(ERROR) << e.what() << "call rpc failed!";
        return true;
    }
}

} // namespace monitor
} // namespace client
} // namespace infra

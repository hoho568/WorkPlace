// Copyright 2012 Jike Inc. All Rights Reserved.
// Author: duniandong@jike.com (Du Niandong)

#include "infrastructure/client/modules/monitor/monitor_module.h"
#include "infrastructure/client/modules/monitor/monitor_client.h"

DEFINE_string(monitor_db, "",
             "housekeeper's monitor informations in db");
namespace infra {
namespace client {
namespace monitor {

const int MonitorModule::CALL_INTERVAL = 5*60;

MonitorModule::MonitorModule() {
    GetServerByYRNS();
    strated_ = false;
}

MonitorModule::~MonitorModule() {
}

void MonitorModule::GetServerByYRNS() {
    scoped_ptr<util::YRNSManager> yrns_manager(new util::YRNSManager);
    vector<pair<string, int> > results;
    bool success = yrns_manager->GetAllReplicas(FLAGS_monitor_db,
                   util::YRNSManager::SERVICE_RPC, &results);
    if (success && !resutls.empty()) {
        const int64 selected_idx = base::Time::Now().ToTimeT() %
                                   results.size();
        server_name_ = results[selected_idx].first;
        server_port_ = results[selected_idx].second;
        LOG(INFO) << "Got monitor_db service from YRNS: "
                  << server_name_ << ":" << server_port_;
    }
}

void MonitorModule::Run() {
    MonitorClient client(server_name_, server_port_);
    while (true) {
        client.GetServiceEntry(host_ip_);
        if (!started_) {
            Singleton<TaskScheduler>::get()->Run();
            started_ = true;
        }
        sleep(CALL_INTERVAL);
    }
}

}
}
}

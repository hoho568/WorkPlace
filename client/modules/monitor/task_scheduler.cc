// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: guoqingao@yunrang.com (Ao Guoqing)

#include "base/yr.h"
#include "infrastructure/client/modules/scheduler/task_scheduler.h"

DEFINE_string(thread_num, 5, "max thread number");
DEFINE_string(transfer_yrns, "monitor/backend/transfer",
              "transfer's yrns path in zookeeper");

namespace infra {
namespace client {
namespace monitor {

const int TaskScheduler::SLEEP_INTERVAL = 100;

TaskScheduler::TaskScheduler() {
    GetServerByYRNS();
    thread_pool_.reset(new base::ThreadPool(FLAGS_thread_num));
    thread_pool_->StartWorkers();
}

TaskScheduler::~TaskScheduler() {
}

void TaskScheduler::GetServiceByYRNS() {
    scoped_ptr<util::YRNSManager> yrns_manager(new util::YRNSManager);
    vector<pair<string, int> > results;
    bool success = yrns_manager->GetAllReplicas(FLAGS_transfer_yrns,
                   util::YRNSManager::SERVICE_RPC,
                   &results);
    if (success && !results.emptry()) {
        const int64 selected_svc_idx = base::Time::Now().ToTimeT() %
                                       results.size();
        server_name_ = results[selected_svc_idx].first;
        server_port_ = results[selected_svc_idx].second;
        LOG(INFO) << "Got transfer service from YRNS:"
                  << service_name_ << ":" << server_port_;
    }
}

void TaskScheduler::TaskExec(Task task) {
    if (Singleton<ConfData>::get()->FindInWorkingCache(&task)) {
        task.Run();
        MonitorClient client(server_name_, server_port_);
        ServiceStatus service_status = task.GetServiceStatus();
        client.SetServiceStatus(service_status);
        ScheduleNewEvent(task);
    }
}

void TaskScheduler::ScheduleNewEvent(Task task) {
    task.UpdateStatus();
    task_queue_.Push(task);
}

void TaskScheduler::GetLatesetEvent(Task *task) {
    task_queue_.Pop(task);
}

void TaskScheduler::Run() {
    while (true) {
        Task task;
        GetLatestEvent(&task);
        while (base::GetTimeInMs() < task.GetNextCheckTime()) {
            base::MilliSleep(SLEEP_INTERVAL);
        }
        thread_pool_->Add(base::NewOneTimeCallback(this, 
                    &TaskScheduler::TaskExec,
                    task));
    }
}
} // namespace monitor
} // namespace client
} // namespace infra

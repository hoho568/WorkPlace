// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: guoqingao@yunrang.com (Ao Guoqing)

#include "base/yr.h"
#include "infrastructure/client/modules/monitor/conf-data.h"

namespace infra {
namespace client {
namespace monitor {

ConfData::ConfData() {
    check_entry_working_cache_ = &check_entry_cache_[0];
    check_entry_bak_cache_ = &check_entry_cache_[1];
}

ConfData::~ConfData() {
    check_entry_working_cache_->clear();
    check_entry_bak_cache_->clear();
}

void ConfData::UpdateService(const vector<ServiceEntry> &entry) {
    ServiceEntryMap::iterator service_entry_it;
    pair<ServiceEntryMap::iterator, bool> service_ret;
    for (int i = 0; i < entry.size(); ++i) {
        Task task(entry[i]);

        mutex_for_check_entry_cache_.ReaderLock();
        service_entry_it = check_entry_woking_cache_->find(task.GetService());
        if (service_entry_id == check_entry_working_cache_->end()) {
            task.SetRecurring(false);
        }
        mutex_for_check_entry_cache_.Readerunlock();

        service_entry_it = check_entry_bak_cache_->find(task.GetService());
        if (service_entry_it == check_entry_bak_cache_->end()) {
            VLOG(2) << "insert new task into bak cache: "
                    << service_entry_it->second.GetService();
            service_ret = check_entry_bak_cache_->insert(std::make_pair(
                          task.GetService(), task));
            if (!service_ret.second) {
                LOG(WARNING) << "insert service entry failed: " << task.GetService();
                continue;
            }
        } else {
            VLOG(2) << "overwrite task: " << service_entry_it->second.GetService();
            service_entry_it->second = task;
        }
    }
}

void ConfData::SwapCheckEntryCache() {
    mutex_for_check_entry_cache_.WriterLock();
    std::swap(check_entry_working_cache_, check_entry_bak_cache_);
    mutex_for_check_entry_cache_.WriterUnlock();
} 

void ConfData::UpdateCache(const vector<ServiceEntry> &entry) {
    check_entry_bak_cache_->clear();
    UpdateService(entry);
    SwapCheckEntryCache();
    InitCheckQueue();
}

void ConfData::InitCheckQueue() {
    LOG(INFO) << "init check queue";
    mutex_for_check_entry_cache_.ReaderLock();
    ServiceEntryMap::iterator service_entry_it;
    for (service_entry_it = check_entry_working_cache_->begin();
         service_entry_it != check_entry_wroking_cache_->end(); ++service_entry_it) { 
        if (!service_entry_it->second.GetRecurring()) {
            service_entry_it->second.SetRecurring(true);
            Singleton<TaskScheduler>::get()->ScheduleNewEvent(
                                             server_entry_it->second);
            VLOG(2) << "Schedule New task: "
                    << service_entry_it->second.GetService();
        }
    }
    mutex_for_check_entry_cache_.ReaderLock();
}

bool ConfData::FindInWorkingCache(Task *task) {
    bool ret = true;
    ServiceEntryMap::iterator service_entry_it;
    mutex_for_check_entry_cache_.ReaderLock();
    service_entry_it = check_entry_working_cache_->find(task->GetService());
    if (service_entry_it = check_entry_working_cache_->end()) {
        ret = false;
    }
    mutex_for_check_entry_cache_.ReaderUnlock();
    return ret;
}
} // namespace monitor
} // namespace client
} // namespace infra


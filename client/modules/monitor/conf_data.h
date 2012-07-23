// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: guoqingao@yunrang.com (Ao Guoqing)

#ifndef INFRASTRUCTURE_CLIENT_MODULES_MONITOR_CONF_DATA_H_
#define INFRASTRUCTURE_CLIENT_MODULES_MONITOR_CONF_DATA_H_

#include <vector>
#include <string>
#include <utility>
#include <algorithm>

#include "base/hash_tables.h"
#include "base/singleton.h"
#include "infrastructure/client/modules/monitor/task.h"
#include "infrastructure/client/modules/monitor/task_scheduler.h"

namespace infra {
namespace client {
namespace monitor {

typedef base::hash_map<std::string, Task> ServiceEntryMap;

class ConfData {
    public:
        bool FindInWokkingCache(Task *task);
        void UpdateCache(const std::vector<ServiceEntry> &entry);
        void InitCheckQueue();
    private:
        explicit ConfData();
        ~ConfData();
        friend struct DefaultSingletonTraits<ConfData>;

        void UpdateService(const std::vector<ServiceEntry> &entry);
        void SwapCheckEntryCache();
    private:
        ServiceEntryMap* check_entry_woking_cache_;
        ServiceEntryMap* check_entry_bak_cache_;
        ServiceEntryMap  check_entry_cache_[2];
        base::RwMutex    mutex_for_check_entry_cache_;
};
} // namespace monitor
} // namespace client
} // namespace infra
#endif INFRASTRUCTURE_CLIENT_MODULES_MONITOR_CONF_DATA_H_

// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: guoqingao@yunrang.com (Ao Guoqing)

#ifndef INFRASTRUCTURE_CLIENT_MODULES_MONITOR_SCHEDULER_TASK_H_
#define INFRASTRUCTURE_CLIENT_MODULES_MONITOR_SCHEDULER_TASK_H_

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <string>
#include <list>

#include "base/logging.h"
#include "base/time.h"
#include "util/command_executor/command_executor.h"
#include "infrastructure/client/modules/monitor/proto/gen-cpp/monitor_types.h"

namespace infra {
namespace client {
namespace monitor {

enum Status {
    OK = 0,
    WARNING = 1,
    CRITICAL = 2,
    UNKNOWN = 3,
};

struct ServiceEntry {
    std::string host;
    std::string service;
    std::string service_type;
    std::string cmd;
    int timeout;
    int check_interval;
};

class Task {
    public:
        Task();
        ~Task();
        explicit Task(const ServiceEntry &service_entry);
        int Run();
        void UpdateStatus();
        int64 GetNextCheckTime();
        bool GetRecurring();
        void SetRecurring(bool recurring);
        std::string GetHost();
        void SetHost(const std::string &host);
        std::string GetService();
        void SetService(const std::string &service);
        ServiceStatus GetServiceStatus();
        bool operator != (const Task &rhs);
        friend class TaskComparator;
    private:
        ServiceEntry service_entry_;
        std::string output_;
        bool recurring_;
        int return_code_;
        int64 next_check_time_;
};

class TaskComparator {
    public:
        bool operator()(const Task &a, const Task &b) {
            return a.next_check_time_ > b.next_check_time_;
        }
};

} // namespace monitor
} // namespace client
} // namespace infra

#endif // INFRASTRUCTURE_CLIENT_MODULES_MONITOR_SCHEDULER_TASK_H

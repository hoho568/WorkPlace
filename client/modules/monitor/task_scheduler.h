// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: guoqingao@yunrang.com (Ao Guoqing)

#ifndef INFRASTRUCTURE_CLIENT_MODULES_MONITOR_TASK_SCHEDULER_H_
#define INFRASTRUCTURE_CLIENT_MODULES_MONITOR_TASK_SCHEDULER_H_

#include <string>
#include <utility>
#include <vector>

#include "base/flags.h"
#include "base/callback.h"
#include "base/singleton.h"
#include "base/scoped_ptr.h"
#include "base/time.h"
#include "base/thread.h"
#include "base/thread_pool.h"
#include "base/concurrent_priority_queue.h"
#include "util/yrns/yrns_manager.h"
#include "infrastructure/client/modules/monitor/scheduler/task.h"
#include "infrastructure/client/modules/monitor/scheduler/monitor_client.h"

namespace infra {
namespace client {
namespace monitor {
    
class TaskScheduler {
    public:
      static const int SLEEP_INTERVAL;
      void ScheduleNewEvent(Task task);
      void Run();
    private:
      explicit TaskScheduler();
      ~TaskScheduler();
      friend struct DefaultSingletonTraits<TaskScheduler>;
      void TaskExec(Task task);
      void GetLatestEvent(Task* task);
      void GetServerByYRNS();
    private:
      base::ConcurrentPriorityQueue<Task, TaskComparator> task_queue_;
      scoped_ptr<base::ThreadPool> thread_pool_;
      std::string server_name_;
      int server_port_;
};
}  // namespace monitor
}  // namespace client
}  // namespace infra
#endif // INFRASTRUCTURE_CLIENT_MODULES_MONITOR_TASK_SCHEDULER_H_

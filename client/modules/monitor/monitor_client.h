// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: guoqingao@yunrang.com (Guoqing Ao)

#ifndef INFRASTRUCTURE_CLIENT_MODULES_MONITOR_SCHEDULER_MONITOR_CLIENT_H_
#define INFRASTRUCTURE_CLIENT_MODULES_MONITOR_SCHEDULER_MONITOR_CLIENT_H_

#include <string>

#include "base/time.h"
#include "base/thrift.h"
#include "base/basictypes.h"
#include "base/scoped_ptr.h"
#include "infrastructure/client/modules/monitor/task.h"
#include "infrastructure/client/modules/monitor/proto/gen-cpp/monitor_types.h"
#include "infrastructure/client/modules/monitor/proto/gen-cpp/MonitorService.h"

namespace base {
template <typename service> class ThriftClient;
}

namespace infra {
namespace client {
namespace monitor {

class MonitorClient {
    public:
        bool SetServiceStatus(Task &task);
        bool SetServiceStatus(const ServiceStatus &service_status);
        bool GetServiceEntry(const std::string& host_name);
        MonitorClient(std::string servie_name, int server_port);
        ~MonitorClent();

    private:
        void Init();
        scoped_ptr<base::ThriftClient<MonitorServiceClient> >
            alarm_client_;
        scoped_ptr<base::ThriftClient<MonitorEntryClient> >
            call_client_;
        std::string server_name_;
        int server_port_;
        DISALLOW_COPY_AND_ASSIGN(MonitorClient);
};
} // namespace monitor
} // namespace client
} // namespace infra

#endif // INFRASTRUCTURE_MONITOR_SCHEDULER_MONITOR_CLIENT_H_


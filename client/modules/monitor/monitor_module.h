// Copyright 2012 Jike Inc. All Rights Reserved.
// Author: duniandong@jike.com (Du Niandong)

#include <utility>
#include <string>
#include <vector>

#include "base/string_util.h"
#include "base/flags.h"
#include "base/thread.h"
#include "base/time.h"
#include "infrastructure/client/moudles/monitor/task_scheduler.h"
#include "infrastructure/client/modules/monitor/monitor_client.h"



namespace infra {
namespace client {
namespace monitor {

class MonitorModule : public base::Thread {
    public:
        static const int CALL_INTERVAL;
        explicit MonitorModule();
        ~CheckProcessor();
    protected:
        void Run();
    private:
        void GetServerByYRNS();
    private:
        std::string server_name_;
        int server_port_;
        bool started_;
};

} // namespace monitor
} // namespace client
} // namespace infra

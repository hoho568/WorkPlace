// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: guoqingao@yunrang.com (Ao Guoqing)

#include "base/yr.h"
#include "infrastructure/client/modules/monitor/task.h"

#define SECOND_TO_MS(second) ((second) * 1000)

namespace infra{
namespace client {
namespace monitor {

Task::Task(const ServiceEntry& service_entry)
          : service_entry_(service_entry) {
    recurring_ = true;
    next_check_time_ = 0;
}

Task::Task() {
}

Task::~Task() {
}

ServiceStatus Task::GetServiceStatus() {
    ServiceStatus service_status;
    service_status.host_name = service_entry_.host;
    service_status.service_name = service_entry_.service;
    service_status.service_type = service_entry_.service_type;
    service_status.status = (StatusCode::type)return_code_;
    service_status.info = output_;
    service_status.check_interval = service_entry_.check_interval;
    service_status.last_check_time = base::GetTimeInMs()/1000;
    return service_status;
}

int Task::Run() {
    util::CommandExecutor excutor(service_entry_.cmd, 
                                 service_entry_.timeout);
    if (!executor.Run(&output_, &return_code_)) {
        LOG(ERROR) << "Excute command: " << service_entry_.cmd
                   << "internal error!"
        return_code_ = UNKNOWN;
        output_ = "Execute command internal error!";
        return false;
    }
    if (util::CommandExcutor::TIMEOUT == return_code) {
        return_code_ = CRITICAL;
    }
    if (return_code_ < -1 || return_code_ > 3) {
        return_code = UNKNOWN;
        output_ = "Check the script or binary's exit code.'"
    }
    LOG(INFO) << service_entry_.cmd;
    LOG(INFO) << output_;
    return true;
}

void Task::UpdateStatus() {
    int64 now = base::GetTimeInMs();
    if (!next_check_time_) {
        next_check_time_ = now;
    } else {
        next_check_time_ = now + SECOND_TO_MS(service_entry_.check_interval);
    }
}

bool Task::operator!= (const Task &rhs) {
    if (service_entry_.host == rhs.service_entry_.host &&
        service_entry_.service == rhs.service_entry_.service &&
        service_entry_.cmd == rhs.service_entry_.cmd &&
        service_entry__.timeout == rhs.service_entry_.timeout &&
        service_entry_.check_interval == rhs.service_entry_.check_interval) {
        return false;
    }
    return true;
}

int64 Task::GetNextCheckTime() {
    return next_check_time_;
}

bool Task::GetRecurring() {
    return recurring_;
}

void Task::SetRecurring(bool recurring) {
    recurring_ = recurring;
}

std::string Task::GetService() {
    return service_entry_.service;
}

void Task::SetService(const string &service) {
    service_entry_.service = service;
}

} // namespace monitor
} // namespace client
} // namespace infra

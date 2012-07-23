// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: guoqingao@yunrang.com (Guoqing Ao)

namespace cpp infra.monitor
namespace php infra_monitor

struct ServiceEntry {
  1: string service_name;
  2: string service_type;
  3: string check_command;
  4: i32 timeout;
  5: i32 check_interval;
}

service MonitorEntry {
    list<ServiceEntry> GetServiceEntry(1:string host_name);
}

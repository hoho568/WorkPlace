// Copyright 2011 Yunrang Inc. All Rights Reserved.
// Author: guoqingao@yunrang.com (Guoqing Ao)

namespace cpp infra.monitor
namespace php infra_monitor

enum StatusCode {
  OK        = 0,
  WARNING   = 1,
  CRITICAL  = 2,
  UNKNOW    = 3,
}

enum CheckType {
  BASIC     = 0,
  REMOTE    = 1,
  CUSTOM    = 2,
}

enum RecordStatus {
  NORMAL    = 0,
  MAGRATION = 1,
}

struct ServiceStatus {
  1: i64 service_id;
  2: string host_name;
  3: string service_name;
  4: StatusCode status;
  5: string info;
  6: string service_type;
  7: i32 check_interval;
  8: i64 last_check_time;
  9: optional CheckType check_type;
}

struct ServiceRecord {
  1: ServiceStatus service_status;
  2: list<StatusCode> status_history;
  3: RecordStatus record_status = RecordStatus.NORMAL;
}

service MonitorService {
  ServiceStatus GetServiceStatus(1:string host_name, 2: string service_name);
  list<ServiceStatus> GetServiceTypeStatus(1: string service_type, 2: i32 status);
  void SetServiceStatus(1: ServiceStatus service_status);
}

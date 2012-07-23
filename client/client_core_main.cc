// Copyright 2012 Jike Inc. All Rights Reserved.
// Author: duniandong@jike.com (Du Niandong)

#include <signal.h>

#include "util/yrns/yrns_manager.h"
#include "util/global_init/global_init.h"
#include "infrastructure/client/modules/monitor/monitor_module.h"

int main(int argc, char** argv) {
    util::GlobalInit int(&argc, &argc);

    MonitorModule monitor;
    
    
    monitor.Start();

    monitor.Join();

    return 0;



}

#ifndef IBM_H
#define IBM_H
#include <iotp_device.h>
#include "mem_info.h"
#include "args.h"

#define IOTP_SEND_BUFF_SIZE 250

IOTPRC IoTPconnect_toCloud(struct arguments args, IoTPConfig **config, IoTPDevice **device);

IOTPRC IoTPsend_mem_info(IoTPDevice *device, struct memory_info mem_info);

IOTPRC IoTPdisconnect_clear(IoTPConfig *config, IoTPDevice *device);

#endif
#include "ibm.h"


IOTPRC IoTPconnect_toCloud(struct arguments args, IoTPConfig **config, IoTPDevice **device){

        IOTPRC rc = IOTPRC_SUCCESS;

        rc = IoTPConfig_create(config, NULL);
        if ( rc != 0 ) {
                return rc;
        }

        IoTPConfig_setProperty(*config, "identity.orgId", args.orgID_str);
        IoTPConfig_setProperty(*config, "identity.typeId", args.typeID_str);
        IoTPConfig_setProperty(*config, "identity.deviceId", args.deviceID_str);
        IoTPConfig_setProperty(*config, "auth.token", args.token_str);

        rc = IoTPDevice_create(device, *config);
        if ( rc != 0 ) {
                return rc;
        }

        rc = IoTPDevice_connect(*device);

        return rc;
}

IOTPRC IoTPsend_mem_info(IoTPDevice *device, struct memory_info mem_info){

        IOTPRC rc = IOTPRC_SUCCESS;

        char buff[IOTP_SEND_BUFF_SIZE];

        sprintf(buff,"{ \"total ram\":\"%d\",\"free ram\": %lld,\"used ram\": %lld}",
        mem_info.total,  mem_info.free, mem_info.used);

        rc = IoTPDevice_sendEvent(device, "log", buff, "json", QoS0, NULL);

        return rc;
}

IOTPRC IoTPdisconnect_clear(IoTPConfig *config, IoTPDevice *device){

        IOTPRC rc = IOTPRC_SUCCESS;

        rc = IoTPDevice_disconnect(device);

        if ( device!= NULL )
                IoTPDevice_destroy(device);

        if ( config!= NULL )
                IoTPConfig_clear(config);

        return rc;
}
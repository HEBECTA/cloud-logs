#include <stdio.h>
#include <unistd.h>

#include <syslog.h>

#include "args.h"
#include "ubus.h"
#include "ibm.h"

#include "mem_info.h"

#include <signal.h>

#define MAX_FAILS 5
#define LOG_WRITE_INTERVAL_TIME 30
#define CLOUD_RECONNECT_TIME 3

int prog_run;

void sig_handler(int signum){

  prog_run = 0;

  syslog(LOG_NOTICE, "Received a SIGTERM signal.\n");
}

int main(int argc, char *argv[]){

    signal(SIGTERM, sig_handler);

    prog_run = 1;

    int fail_counter = 1;

    int rc = EXIT_SUCCESS;

    struct arguments args = {"-", "-", "-", "-"};

    openlog(NULL, LOG_PID, LOG_USER);

    rc = args_parse (argc, argv, &args);
    if (rc){
      syslog(LOG_ERR, "Failed to parse command arguments\n");
      goto EXIT_PROGRAM;
    }

    IoTPConfig *config = NULL;
    IoTPDevice *device = NULL;

    rc = IoTPconnect_toCloud(args, &config, &device);
    if (rc){
        syslog(LOG_ERR, "Failed to connect to IBM cloud\n");
        goto EXIT_PROGRAM;
    }

    struct ubus_context *ctx;
    uint32_t id;

    rc = connect_ubus(&id, &ctx);
    if ( connect_ubus(&id, &ctx) ){
       syslog(LOG_ERR, "Failed to connect to ubus\n");
       goto EXIT_PROGRAM;
    }

    struct memory_info mem_info;

    while ( prog_run ){

        rc = ubus_invoke(ctx, id, "info", NULL, board_cb, (void *)&mem_info, 3000);

        if ( rc ){

            syslog(LOG_ERR, "Attempt: %d; Failed to invoke info from system. Max attemtps: %d\n", fail_counter, MAX_FAILS);

            if ( fail_counter >= MAX_FAILS){

              prog_run = 0;
              break;
            }

            ++fail_counter;

            sleep(CLOUD_RECONNECT_TIME);

            continue;
        }

        IoTPsend_mem_info(device, mem_info);

        syslog(LOG_INFO, "Total memory -> %lld;\nFree memory -> %lld;\nUsed memory -> %lld;\n",
        mem_info.total,  mem_info.free, mem_info.used );

        sleep(LOG_WRITE_INTERVAL_TIME);
    }

EXIT_PROGRAM:

    rc = IoTPdisconnect_clear(config, device);
    if ( rc )
      syslog(LOG_ERR, "Failed to disconnect from IBM cloud\n");
    
    if ( !ctx )
      ubus_free(ctx);

    closelog();

    return 0;
}
#ifndef UBUS_H
#define UBUS_H

#include <libubox/blobmsg_json.h>
#include <libubus.h>

enum {
	TOTAL_MEMORY,
	FREE_MEMORY,
	SHARED_MEMORY,
	BUFFERED_MEMORY,
	__MEMORY_MAX,
};

enum {
	MEMORY_DATA,
	__INFO_MAX,
};

static const struct blobmsg_policy memory_policy[__MEMORY_MAX];

static const struct blobmsg_policy info_policy[__INFO_MAX];

void board_cb(struct ubus_request *req, int type, struct blob_attr *msg);

int connect_ubus(uint32_t *id, struct ubus_context **ctx);

#endif
#include "ubus.h"
#include "mem_info.h"

static const struct blobmsg_policy memory_policy[__MEMORY_MAX] = {
	[TOTAL_MEMORY] = { .name = "total", .type = BLOBMSG_TYPE_INT64 },
	[FREE_MEMORY] = { .name = "free", .type = BLOBMSG_TYPE_INT64 },
	[SHARED_MEMORY] = { .name = "shared", .type = BLOBMSG_TYPE_INT64 },
	[BUFFERED_MEMORY] = { .name = "buffered", .type = BLOBMSG_TYPE_INT64 },
};

static const struct blobmsg_policy info_policy[__INFO_MAX] = {
	[MEMORY_DATA] = { .name = "memory", .type = BLOBMSG_TYPE_TABLE },
};

void board_cb(struct ubus_request *req, int type, struct blob_attr *msg) {
	//struct blob_buf *buf = (struct blob_buf *)req->priv;
	struct blob_attr *tb[__INFO_MAX];
	struct blob_attr *memory[__MEMORY_MAX];

	blobmsg_parse(info_policy, __INFO_MAX, tb, blob_data(msg), blob_len(msg));

	if (!tb[MEMORY_DATA]) {
                req->status_msg = true;
		req->status_code = ENODATA;
		return;
	}

        struct memory_info *mem_info = (struct memory_info*) req->priv;

	blobmsg_parse(memory_policy, __MEMORY_MAX, memory,
			blobmsg_data(tb[MEMORY_DATA]), blobmsg_data_len(tb[MEMORY_DATA]));

        mem_info->total = blobmsg_get_u64(memory[TOTAL_MEMORY]);
        mem_info->free = blobmsg_get_u64(memory[FREE_MEMORY]);
        mem_info->used = blobmsg_get_u64(memory[TOTAL_MEMORY]) - blobmsg_get_u64(memory[FREE_MEMORY]);
}


int connect_ubus(uint32_t *id, struct ubus_context **ctx){

        *ctx = ubus_connect(NULL);

        if ( !(*ctx) ) 
                return ECONNREFUSED;

        if ( ubus_lookup_id(*ctx, "system", id))
                return ECONNREFUSED;
                
        return EXIT_SUCCESS;
}
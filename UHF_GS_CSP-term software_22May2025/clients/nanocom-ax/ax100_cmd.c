/**
 * NanoCom firmware
 *
 * @author Johan De Claville Christiansen
 * Copyright 2014 GomSpace ApS. All rights reserved.
 */

#include <alloca.h>
#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <command/command.h>
#include <param/param.h>
#include <ax100.h>
#include <csp/csp.h>
#include <csp/csp_endian.h>

int node_com = 5;

int cmd_ax100_node(struct command_context * ctx) {
	if (ctx->argc != 2)
		return CMD_ERROR_SYNTAX;
	node_com = atoi(ctx->argv[1]);
	return CMD_ERROR_NONE;
}

int cmd_ax100_hk(struct command_context * ctx) {
	param_index_t com_hk = {0};
	com_hk.physaddr = alloca(AX100_TELEM_SIZE);
	com_hk.size = AX100_TELEM_SIZE;
	if (ax100_get_hk(&com_hk, node_com, 1000) < 0)
		return CMD_ERROR_FAIL;
	param_list(&com_hk, 1);
	return CMD_ERROR_NONE;
}

int cmd_ax100_gndwdt_clear(struct command_context * ctx) {
	csp_transaction(CSP_PRIO_HIGH, node_com, AX100_PORT_GNDWDT_RESET, 1000, NULL, 0, NULL, 0);
	return CMD_ERROR_NONE;
}

command_t ax100_commands[] = {
	{
		.name = "node",
		.help = "Set node",
		.usage = "<node>",
		.handler = cmd_ax100_node,
	},{
		.name = "hk",
		.help = "Get HK",
		.handler = cmd_ax100_hk,
	},{
		.name = "gndwdt_clear",
		.help = "Clear",
		.handler = cmd_ax100_gndwdt_clear,
	}
};

command_t __root_command ax100_root_command[] = {
	{
		.name = "ax100",
		.help = "client: AX100",
		.chain = INIT_CHAIN(ax100_commands),
	},
};

void cmd_ax100_setup(void) {
	command_register(ax100_root_command);
}


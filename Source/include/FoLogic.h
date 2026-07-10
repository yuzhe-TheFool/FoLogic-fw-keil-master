/*
 * This file is part of the FoLogic-firmware project.
 *
 * Copyright (C) 2014 DreamSourceLab <support@dreamsourcelab.com>
 * Copyright (C) 2026 the fool
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef FOLOGICFW_INCLUDE_FOLOGIC_H
#define FOLOGICFW_INCLUDE_FOLOGIC_H

/*
 * Major and minor DSLogic-firmware version numbers.
 * These can be queried by the host via CMD_GET_FW_VERSION.
 */
#define FOLOGICFW_VER_MAJOR	1
#define FOLOGICFW_VER_MINOR	0

/* Protocol commands */
#define CMD_GET_FW_VERSION		0xb0
#define CMD_GET_REVID_VERSION		0xb1
#define CMD_START			0xb2
#define CMD_CONFIG			0xb3
#define CMD_SETTING			0xb4
#define CMD_CONTROL			0xb5
#define CMD_WR_REG			0xb8	// Write register command

#define CMD_START_FLAGS_WIDE_POS	5
#define CMD_START_FLAGS_CLK_SRC_POS	6
#define CMD_START_FLAGS_STOP_POS	7

#define CMD_START_FLAGS_SAMPLE_8BIT	(0 << CMD_START_FLAGS_WIDE_POS)
#define CMD_START_FLAGS_SAMPLE_16BIT	(1 << CMD_START_FLAGS_WIDE_POS)

#define CMD_START_FLAGS_CLK_30MHZ	(0 << CMD_START_FLAGS_CLK_SRC_POS)
#define CMD_START_FLAGS_CLK_48MHZ	(1 << CMD_START_FLAGS_CLK_SRC_POS)

#define CMD_START_FLAGS_STOP		(1 << CMD_START_FLAGS_STOP_POS)

struct cmd_start {
	BYTE flags;
	BYTE param0;
	BYTE param1;
};

struct cmd_setting_count {
	BYTE byte0;
	BYTE byte1;
	BYTE byte2;
};

struct cmd_cfg_count {
	BYTE byte0;
	BYTE byte1;
	BYTE byte2;
};

struct cmd_control {
    	BYTE byte0;
    	BYTE byte1;
    	BYTE byte2;
    	BYTE byte3;
};

struct cmd_wr_reg {
    	WORD reg_value;        // 16-bit value: [reg_addr_high][data_value_low]
};

#endif

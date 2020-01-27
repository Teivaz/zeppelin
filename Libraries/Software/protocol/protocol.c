#include "protocol.h"
#include <string.h>

static uint8_t s_nextRid = 0;

// Implement this function in your app
extern uint8_t PZ_crc(uint8_t const* data, uint8_t size);

PZ_Package PZ_compose0(uint8_t adr, PZ_Cmd cmd) {
	return PZ_compose(adr, cmd, 0, 0);
}
PZ_Package PZ_compose1(uint8_t adr, PZ_Cmd cmd, uint8_t data0) {
	return PZ_compose(adr, cmd, &data0, 1);
}
PZ_Package PZ_compose2(uint8_t adr, PZ_Cmd cmd, uint8_t data0, uint8_t data1) {
	const uint8_t buf[] = {data0, data1};
	return PZ_compose(adr, cmd, &buf[0], 2);
}
PZ_Package PZ_compose(uint8_t adr, PZ_Cmd cmd, uint8_t const* data, uint8_t dataLen) {
	PZ_Package result;
	result.fpr = PZ_Footrpint_OK;
	result.adr = adr;
	result.len = PZ_STATIC_PAYLOAD_LEN + dataLen + 1;
	result.rid = s_nextRid++;
	result.cmd = cmd;
	memcpy(&result.pld[0], data, dataLen);
	result.crc = PZ_crc((uint8_t*)&result, result.len + PZ_HEADER_LEN - 1);
	return result;
}

PZ_Package PZ_composeRe2(PZ_Package const* pck, uint8_t data0, uint8_t data1) {
	const uint8_t buf[] = {data0, data1};
	return PZ_composeRe(pck, &buf[0], 2);
}

PZ_Package PZ_composeRe(PZ_Package const* pck, uint8_t const* data, uint8_t dataLen) {
	PZ_Package result;
	result.fpr = PZ_Footrpint_OK;
	result.adr = pck->adr;
	result.len = PZ_STATIC_PAYLOAD_LEN + dataLen + 1;
	result.rid = pck->rid;
	result.cmd = pck->cmd | 0x80;
	memcpy(&result.pld[0], data, dataLen);
	result.crc = PZ_crc((uint8_t*)&result, result.len + PZ_HEADER_LEN - 1);
	return result;
}

uint8_t PZ_needsResponse(PZ_Package const* p) {
	switch (p->cmd) {
		case PZ_Cmd_Info:
		case PZ_Cmd_Read_cv:
		case PZ_Cmd_Read_dv:
			return 1;
		default:
			return 0;
	}
}

uint8_t PZ_isResponse(PZ_Package const* p) {
	return !!(p->cmd & 0x80);
}

uint8_t PZ_isAdrValid(uint8_t adr) {
	if (adr < 0x08) {
		return 0;
	}
	if (adr > 0x7b) {
		return 0;
	}
	return 1;
}

PZ_Result PZ_verify(uint8_t const* package, uint8_t size) {
	if (size < PZ_MIN_PACKAGE_LEN) {
		return PZ_ERROR;
	}
	if (size > PZ_MAX_PACKAGE_LEN) {
		return PZ_ERROR;
	}
	PZ_Package* pck = (PZ_Package*)package;

	if (pck->fpr != PZ_Footrpint_OK) {
		return PZ_ERROR;
	}
	if (!PZ_isAdrValid(pck->adr)) {
		return PZ_ERROR;
	}
	if (pck->len + PZ_HEADER_LEN != size) {
		return PZ_ERROR;
	}
	uint8_t crc = package[size-1];
	if (PZ_crc(package, size-1) != crc) {
		return PZ_ERROR_CRC;
	}
	return PZ_OK;
}

void PZ_toData(uint8_t* outData, uint8_t* outDataSize, PZ_Package const* package) {
	*outDataSize = PZ_HEADER_LEN + package->len;
	memcpy(outData, package, PZ_HEADER_LEN + PZ_STATIC_PAYLOAD_LEN);
	memcpy(outData + PZ_HEADER_LEN + PZ_STATIC_PAYLOAD_LEN, &package->pld[0], package->len - PZ_STATIC_PAYLOAD_LEN - 1);
	outData[PZ_HEADER_LEN + package->len - 1] = package->crc;
}

PZ_Package PZ_fromData(uint8_t const* data) {
	PZ_Package package;
	memcpy(&package, data, PZ_HEADER_LEN + PZ_STATIC_PAYLOAD_LEN);
	memcpy(&package.pld[0], data + PZ_HEADER_LEN + PZ_STATIC_PAYLOAD_LEN, package.len - PZ_STATIC_PAYLOAD_LEN - 1);
	package.crc = data[PZ_HEADER_LEN + package.len - 1];
	return package;
}

static char const* cmdToStr(PZ_Cmd cmd) {
	switch (cmd) {
		case PZ_Cmd_Info: return "info";
		case PZ_Cmd_Info_re: return "info-re";
		case PZ_Cmd_Read_cv: return "read-cv";
		case PZ_Cmd_Read_cv_re: return "read-cv-re";
		case PZ_Cmd_Write_cv: return "write-cv";
		case PZ_Cmd_Reset_cv: return "reset-cv";
		case PZ_Cmd_Reset_all_cv: return "reset-all-cv";
		case PZ_Cmd_Read_dv: return "read-dv";
		case PZ_Cmd_Read_dv_re: return "read-dv-re";
		case PZ_Cmd_Write_dv: return "write-dv";
		case PZ_Cmd_Reset_dv: return "reset-dv";
		case PZ_Cmd_Reset_all_dv: return "reset-all-dv";
		default: return "<undefeined>";
	}
}

void PZ_PrintInfo(PZ_printf _printf, PZ_Package const* package) {
	_printf("Package 0x%02x - %s(0x%02x) to Address 0x%02x. Data: ", package->rid, cmdToStr(package->cmd), package->cmd, package->adr);
	if (package->len == 3) {
		_printf("<No Data>", package->pld[0]);
	}
	if (package->len == 4) {
		_printf("[0x%02x]", package->pld[0]);
	}
	if (package->len == 5) {
		_printf("[0x%02x, 0x%02x]", package->pld[0], package->pld[1]);
	}
	_printf("\r\n");
}

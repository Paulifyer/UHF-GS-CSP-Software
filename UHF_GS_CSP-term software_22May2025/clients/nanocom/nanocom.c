/*
 * nanocom.c
 *
 *  Created on: 14/01/2010
 *      Author: johan
 */

#include <nanocom.h>
#include <csp/csp.h>
#include <csp/csp_endian.h>
#include <util/log.h>

int com_set_conf(nanocom_conf_t * config, uint8_t node, uint32_t timeout) {
	config->morse_bat_level = csp_hton16(config->morse_bat_level);
	config->morse_pospone = csp_hton16(config->morse_pospone);
	config->morse_inter_delay = csp_hton16(config->morse_inter_delay);
	config->hk_interval = csp_hton16(config->hk_interval);
	config->preamble_length = csp_hton16(config->preamble_length);
	config->tx_max_temp = csp_hton16(config->tx_max_temp);
	return csp_transaction(CSP_PRIO_HIGH, node, COM_PORT_CONF, timeout, (void *) config, sizeof(nanocom_conf_t), NULL, 0);
}

int com_get_conf(nanocom_conf_t * config, uint8_t node, uint32_t timeout) {
	int status = csp_transaction(CSP_PRIO_HIGH, node, COM_PORT_CONF, timeout, NULL, 0, (void *) config, sizeof(nanocom_conf_t));
	if (status == 0)
		return 0;
	config->morse_bat_level = csp_ntoh16(config->morse_bat_level);
	config->morse_pospone = csp_ntoh16(config->morse_pospone);
	config->morse_inter_delay = csp_ntoh16(config->morse_inter_delay);
	config->hk_interval = csp_ntoh16(config->hk_interval);
	config->preamble_length = csp_ntoh16(config->preamble_length);
	config->tx_max_temp = csp_ntoh16(config->tx_max_temp);
	return status;
}

int com_restore_conf(uint8_t node) {
	return csp_transaction(CSP_PRIO_HIGH, node, COM_PORT_RESTORE, 0, NULL, 0, NULL, 0);
}

int com_get_status(nanocom_data_t * data, uint8_t node, uint32_t timeout) {
	int status = csp_transaction(CSP_PRIO_NORM, node, COM_PORT_STATUS, timeout, NULL, 0, (void *) data, sizeof(nanocom_data_t));
	if (status != sizeof(nanocom_data_t))
		return status;
	data->bit_corr_tot = csp_ntoh32(data->bit_corr_tot);
	data->byte_corr_tot = csp_ntoh32(data->byte_corr_tot);
	data->rx = csp_ntoh32(data->rx);
	data->rx_err = csp_ntoh32(data->rx_err);
	data->tx = csp_ntoh32(data->tx);
	data->last_rferr = csp_ntoh16(data->last_rferr);
	data->last_rssi = csp_ntoh16(data->last_rssi);
	data->last_temp_a = csp_ntoh16(data->last_temp_a);
	data->last_temp_b = csp_ntoh16(data->last_temp_b);
	data->last_txcurrent = csp_ntoh16(data->last_txcurrent);
	data->last_batt_volt = csp_ntoh16(data->last_batt_volt);
	data->bootcount = csp_ntoh32(data->bootcount);
	return status;
}

int com_get_log_rssi(nanocom_rssi_t * data, uint8_t * count, uint8_t node, uint32_t timeout) {
	int status = csp_transaction(CSP_PRIO_NORM, node, COM_PORT_LOG_RSSI, timeout, NULL, 0, (void *) data, -1);
	if (status == 0)
		return 0;
	if (status % sizeof(nanocom_rssi_t))
		return 0;
	unsigned int i;
	for (i = 0; i < status / sizeof(nanocom_rssi_t); i++) {
		nanocom_rssi_t * logd = data + i;
		logd->time = csp_ntoh32(logd->time);
		logd->rssi = csp_ntoh16(logd->rssi);
		logd->rferr = csp_ntoh16(logd->rferr);
	}
	*count = status / sizeof(nanocom_rssi_t);
	return 1;
}

int com_get_hk(nanocom_hk_t * data, uint8_t * count, uint8_t node, uint32_t timeout) {
	int status = csp_transaction(CSP_PRIO_NORM, node, COM_PORT_LOG_HK, timeout, NULL, 0, (void *) data, -1);
	if (status == 0)
		return 0;
	if (status % sizeof(nanocom_hk_t))
		return 0;
	unsigned int i;
	for (i = 0; i < status / sizeof(nanocom_hk_t); i++) {
		nanocom_hk_t * hkd = data + i;
		hkd->batt_volt = csp_ntoh16(hkd->batt_volt);
		hkd->temp_a = csp_ntoh16(hkd->temp_a);
		hkd->temp_b = csp_ntoh16(hkd->temp_b);
		hkd->time = csp_ntoh32(hkd->time);
	}
	*count = status / sizeof(nanocom_hk_t);
	return 1;
}

int com_get_calibration(uint8_t node, nanocom_calibrate_t * com_calibrate, uint32_t timeout) {
	int status = csp_transaction(CSP_PRIO_NORM, node, COM_PORT_CALIBRATE, timeout, NULL, 0, (void *) com_calibrate, sizeof(nanocom_calibrate_t));
	if (status == 0)
		return 0;
	com_calibrate->con_rferr = csp_ntoh16(com_calibrate->con_rferr);
	com_calibrate->raw_rferr = csp_ntoh16(com_calibrate->raw_rferr);
	com_calibrate->con_rssi = csp_ntoh16(com_calibrate->con_rssi);
	com_calibrate->raw_rssi = csp_ntoh16(com_calibrate->raw_rssi);
	return status;
}

int com_set_tx_inhibit(uint8_t node, uint8_t value, uint32_t timeout) {
	uint8_t ret;
	int status = csp_transaction(CSP_PRIO_HIGH, node, COM_PORT_TX_INHIBIT, timeout, &value, sizeof(value), &ret, sizeof(ret));
	if (status != sizeof(ret))
		return -1;
	return 0;
}

void com_print_conf(nanocom_conf_t * com_conf) {
	printf("FEC: rs %u, random %u, viterbi %u\r\n", com_conf->do_rs, com_conf->do_random, com_conf->do_viterbi);
	printf("RADIO: rx %u, tx %u, preamble %u, max temp: %d\r\n", com_conf->rx_baud, com_conf->tx_baud, com_conf->preamble_length, com_conf->tx_max_temp);
	printf("MORSE: enable: %u, mode: %u,  delay %u, pospone %u, wpm %u, batt level %u, text %s\r\n", com_conf->morse_enable, com_conf->morse_mode, com_conf->morse_inter_delay, com_conf->morse_pospone, com_conf->morse_wpm, com_conf->morse_bat_level, com_conf->morse_text);
	printf("MORSE: cycle: %u, volt:%u rxc:%u txc:%u tempa:%u tempb:%u rssi:%u rferr:%u\r\n", com_conf->morse_cycle, com_conf->morse_en_voltage, com_conf->morse_en_rx_count, com_conf->morse_en_tx_count, com_conf->morse_en_temp_a, com_conf->morse_en_temp_b, com_conf->morse_en_rssi, com_conf->morse_en_rf_err);
	printf("HK: interval %u\r\n", com_conf->hk_interval);
}

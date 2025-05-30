/**
 * @file ftp_client.h
 * FTP Client API for X86
 *
 * @author Jeppe Ledet-Pedersen & Johan De Claville Christiansen
 * Copyright 2011 GomSpace ApS. All rights reserved.
 */

#ifndef FTP_CLIENT_H_
#define FTP_CLIENT_H_

#include <stdint.h>
#include <stdbool.h>

#include <ftp/ftp_types.h>

typedef void (*ftp_progress_handler)(uint32_t current_chunk, uint32_t total_chunks, double bps, void *data);
void ftp_set_progress_handler(ftp_progress_handler handler, void *data);

/* FTP list callback */
typedef int (*ftp_list_callback_t)(uint16_t entries, ftp_list_entry_t *entry, void *data);

int ftp_upload(uint8_t host, uint8_t port, const char * path, uint8_t type, int chunk_size, uint32_t addr, const char * remote_path, uint32_t * size, uint32_t * checksum);
int ftp_download(uint8_t host, uint8_t port, const char * path, uint8_t backend, int chunk_size, uint32_t memaddr, uint32_t memsize, const char * remote_path, uint32_t * size);
int ftp_status_request(void);
int ftp_status_reply(void);
int ftp_data(int count);
int ftp_crc(void);
int ftp_done(unsigned int remove_map);
int ftp_move(uint8_t host, uint8_t port, uint8_t backend, char * from, char * to);
int ftp_copy(uint8_t host, uint8_t port, uint8_t backend, char * from, char * to);
int ftp_remove(uint8_t host, uint8_t port, uint8_t backend, char * path);
int ftp_mkfs(uint8_t host, uint8_t port, uint8_t backend, char *path, bool force);
int ftp_mkdir(uint8_t host, uint8_t port, uint8_t backend, char *path, uint32_t mode);
int ftp_rmdir(uint8_t host, uint8_t port, uint8_t backend, char *path);
int ftp_list(uint8_t host, uint8_t port, uint8_t backend, char * path, ftp_list_callback_t callback, void *data);
int ftp_zip(uint8_t host, uint8_t port, uint8_t backend, char * src, char * dest, uint8_t action);
void ftp_set_timeout(int timeout);
int ftp_get_timeout(void);

#endif /* FTP_CLIENT_H_ */

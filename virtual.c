#include <stdio.h>
#include "oslabs.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

int process_page_access_fifo(struct PTE page_table[TABLEMAX], int* table_cnt, int page_number, int frame_pool[POOLMAX], int* frame_cnt, int current_timestamp) {
	if (page_table[page_number].is_valid == true) {
		page_table[page_number].last_access_timestamp = current_timestamp;
		page_table[page_number].reference_count += 1;
		return page_table[page_number].frame_number;
	}
	else if (*frame_cnt > 0) {
		page_table[page_number].is_valid = true;
		page_table[page_number].frame_number = frame_pool[*frame_cnt - 1];
		*frame_cnt -= 1;
		page_table[page_number].arrival_timestamp = current_timestamp;
		page_table[page_number].last_access_timestamp = current_timestamp;
		page_table[page_number].reference_count = 1;
		return page_table[page_number].frame_number;
	}
	else {
		bool flag_first = false;
		int temp_index = 0;
		int temp_frame = 0;
		int temp_arrival_stemp = 0;
		for (int index = 0; index < *table_cnt; index++) {
			if ((page_table[index].is_valid == true) && (flag_first == false)) {
				flag_first = true;
				temp_index = index;
				temp_frame = page_table[index].frame_number;
				temp_arrival_stemp = page_table[index].arrival_timestamp;
			}
			else if ((page_table[index].is_valid == true) && (temp_arrival_stemp > page_table[index].arrival_timestamp) && (flag_first == true)) {
				temp_index = index;
				temp_frame = page_table[index].frame_number;
				temp_arrival_stemp = page_table[index].arrival_timestamp;
			}
		}
		if (flag_first == true) {
			page_table[temp_index].is_valid = false;
			page_table[temp_index].frame_number = -1;
			page_table[temp_index].arrival_timestamp = -1;
			page_table[temp_index].last_access_timestamp = -1;
			page_table[temp_index].reference_count = -1;
			page_table[page_number].is_valid = true;
			page_table[page_number].frame_number = temp_frame;
			page_table[page_number].arrival_timestamp = current_timestamp;
			page_table[page_number].last_access_timestamp = current_timestamp;
			page_table[page_number].reference_count = 1;
		}
		return temp_frame;
	}
}

int count_page_faults_fifo(struct PTE page_table[TABLEMAX], int table_cnt, int refrence_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX], int frame_cnt) {
	int timestamp = 1, page_fault = 0;
	for (int i = 0; i < reference_cnt; i++) {
		if (page_table[refrence_string[i]].is_valid == true) {
			page_table[refrence_string[i]].last_access_timestamp = timestamp;
			page_table[refrence_string[i]].reference_count += 1;
		}
		else if (frame_cnt > 0) {
			page_fault += 1;
			page_table[refrence_string[i]].is_valid = true;
			page_table[refrence_string[i]].frame_number = frame_pool[frame_cnt - 1];
			frame_cnt -= 1;
			page_table[refrence_string[i]].arrival_timestamp = timestamp;
			page_table[refrence_string[i]].last_access_timestamp = timestamp;
			page_table[refrence_string[i]].reference_count += 1;
		}
		else if (frame_cnt == 0) {
			page_fault += 1;
			bool flag_first = false;
			int temp_index = 0;
			int temp_frame = 0;
			int temp_arrivel_time = 0;
			for (int index = 0; index < table_cnt; index++) {
				if ((page_table[index].is_valid == true) && (flag_first == false)) {
					flag_first = true;
					temp_index = index;
					temp_frame = page_table[index].frame_number;
					temp_arrivel_time = page_table[index].arrival_timestamp;
				}
				else if ((page_table[index].is_valid == true) && (temp_arrivel_time > page_table[index].arrival_timestamp) && (flag_first == true)) {
					temp_index = index;
					temp_frame = page_table[index].frame_number;
					temp_arrivel_time = page_table[index].arrival_timestamp;
				}
			}
			if (flag_first == true) {
				page_table[temp_index].arrival_timestamp = -1;
				page_table[temp_index].reference_count = -1;
				page_table[temp_index].last_access_timestamp = -1;
				page_table[temp_index].frame_number = -1;
				page_table[temp_index].is_valid = false;
				page_table[refrence_string[i]].frame_number = temp_frame;
				page_table[refrence_string[i]].arrival_timestamp = timestamp;
				page_table[refrence_string[i]].is_valid = true;
				page_table[refrence_string[i]].last_access_timestamp = timestamp;
				page_table[refrence_string[i]].reference_count = 1;
			}
		}
		timestamp += 1;
	}
	return page_fault;
}

int process_page_access_lru(struct PTE page_table[TABLEMAX], int* table_cnt, int page_number, int frame_pool[POOLMAX], int* frame_cnt, int current_timestamp) {
	if (page_table[page_number].is_valid == true) {
		page_table[page_number].last_access_timestamp = current_timestamp;
		page_table[page_number].reference_count += 1;
		return page_table[page_number].frame_number;
	}
	else if ((page_table[page_number].is_valid == false) && (*frame_cnt > 0)) {
		page_table[page_number].is_valid = true;
		page_table[page_number].frame_number = frame_pool[*frame_cnt - 1];
		*frame_cnt -= 1;
		page_table[page_number].arrival_timestamp = current_timestamp;
		page_table[page_number].last_access_timestamp = current_timestamp;
		page_table[page_number].reference_count = 1;
		return page_table[page_number].frame_number;
	}
	else {
		bool flag_first = false;
		int temp_index = 0;
		int temp_frame = 0;
		int temp_last_access_timestamp = 0;
		for (int index = 0; index < *table_cnt; index++) {
			if ((page_table[index].is_valid == true) && (flag_first == false)) {
				flag_first = true;
				temp_index = index;
				temp_frame = page_table[index].frame_number;
				temp_last_access_timestamp = page_table[index].last_access_timestamp;
			}
			else if ((page_table[index].is_valid == true) && (temp_last_access_timestamp > page_table[index].last_access_timestamp) && (flag_first == true)) {
				temp_index = index;
				temp_frame = page_table[index].frame_number;
				temp_last_access_timestamp = page_table[index].last_access_timestamp;
			}
		}
		if (flag_first == true) {
			page_table[temp_index].is_valid = false;
			page_table[temp_index].frame_number = -1;
			page_table[temp_index].arrival_timestamp = -1;
			page_table[temp_index].last_access_timestamp = -1;
			page_table[temp_index].reference_count = -1;
			page_table[page_number].is_valid = true;
			page_table[page_number].frame_number = temp_frame;
			page_table[page_number].arrival_timestamp = current_timestamp;
			page_table[page_number].last_access_timestamp = current_timestamp;
			page_table[page_number].reference_count = 1;
		}
		return temp_frame;
	}
}

int count_page_faults_lru(struct PTE page_table[TABLEMAX], int table_cnt, int refrence_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX], int frame_cnt) {
	int timestamp = 1, page_fault = 0;
	for (int i = 0; i < reference_cnt; i++) {
		if (page_table[refrence_string[i]].is_valid == true) {
			page_table[refrence_string[i]].last_access_timestamp =
				timestamp;
			page_table[refrence_string[i]].reference_count += 1;
		}
		else if ((page_table[refrence_string[i]].is_valid == false) &&
			(frame_cnt > 0)) {
			page_table[refrence_string[i]].frame_number =
				frame_pool[frame_cnt - 1];
			page_table[refrence_string[i]].last_access_timestamp =
				timestamp;
			page_table[refrence_string[i]].reference_count += 1;
			page_table[refrence_string[i]].is_valid = true;
			frame_cnt -= 1;
			page_fault += 1;
		}
		else if ((page_table[refrence_string[i]].is_valid == false) &&
			(frame_cnt == 0)) {
			page_fault += 1;
			bool flag_first = false;
			int temp_index = 0;
			int temp_frame = 0;
			int temp_last_arrival_stemp = 0;
			for (int index = 0; index < table_cnt; index++) {
				if ((page_table[index].is_valid == true) && (flag_first == false)) {
					flag_first = true;
					temp_index = index;
					temp_frame = page_table[index].frame_number;
					temp_last_arrival_stemp = page_table[index].arrival_timestamp; 
				}
				else if ((page_table[index].is_valid == true) && (temp_last_arrival_stemp > page_table[index].last_access_timestamp) && (flag_first == true)) {
					temp_index = index;
					temp_frame = page_table[index].frame_number;
					temp_last_arrival_stemp = page_table[index].last_access_timestamp;
				}
			}
			if (flag_first == true) {
				page_table[temp_index].arrival_timestamp = -1;
				page_table[temp_index].reference_count = -1;
				page_table[temp_index].last_access_timestamp = -1;
				page_table[temp_index].frame_number = -1;
				page_table[temp_index].is_valid = false;
				page_table[refrence_string[i]].frame_number = temp_frame;
				page_table[refrence_string[i]].arrival_timestamp = timestamp;
				page_table[refrence_string[i]].is_valid = true;
				page_table[refrence_string[i]].last_access_timestamp = timestamp;
				page_table[refrence_string[i]].reference_count = 1;
			}
		}
		timestamp += 1;
	}
	return page_fault;
}

int process_page_access_lfu(struct PTE page_table[TABLEMAX], int* table_cnt, int page_number, int frame_pool[POOLMAX], int* frame_cnt, int current_timestamp) {
	if (page_table[page_number].is_valid == true) {
		page_table[page_number].reference_count += 1;
		page_table[page_number].last_access_timestamp = current_timestamp;
		return page_table[page_number].frame_number;
	}
	else if ((page_table[page_number].is_valid == false) && (*frame_cnt > 0)) {
		page_table[page_number].frame_number = frame_pool[*frame_cnt - 1];
		*frame_cnt -= 1;
		page_table[page_number].last_access_timestamp = current_timestamp;
		page_table[page_number].arrival_timestamp = current_timestamp;
		page_table[page_number].reference_count = 1;
		page_table[page_number].is_valid = true; return page_table[page_number].frame_number;
	}
	else {
		bool flag_first = false;
		int temp_index = 0;
		int temp_frame = 0;
		int temp_f_used = 0;
		int temp_arrivel_time = 0;
		for (int index = 0; index < *table_cnt; index++) {
			if ((page_table[index].is_valid == true) && (flag_first == false)) {
				flag_first = true;
				temp_index = index;
				temp_frame = page_table[index].frame_number;
				temp_arrivel_time = page_table[index].arrival_timestamp;
				temp_f_used = page_table[index].reference_count;
			}
			else if ((page_table[index].is_valid == true) && (temp_f_used >= page_table[index].reference_count) && (flag_first == true)) {
				if (((temp_f_used == page_table[index].reference_count) && (temp_arrivel_time > page_table[index].arrival_timestamp)) || (temp_f_used > page_table[index].reference_count)) {
					temp_index = index;
					temp_frame = page_table[index].frame_number;
					temp_arrivel_time = page_table[index].arrival_timestamp;
					temp_f_used = page_table[index].reference_count;
				}
			}
		}
		if (flag_first == true) {
			page_table[temp_index].is_valid = false;
			page_table[temp_index].frame_number = -1;
			page_table[temp_index].arrival_timestamp = -1;
			page_table[temp_index].last_access_timestamp = -1;
			page_table[temp_index].reference_count = -1;
			page_table[page_number].is_valid = true;
			page_table[page_number].frame_number = temp_frame;
			page_table[page_number].arrival_timestamp = current_timestamp;
			page_table[page_number].last_access_timestamp = current_timestamp;
			page_table[page_number].reference_count = 1;
		}
		return temp_frame;
	}
}

int count_page_faults_lfu(struct PTE page_table[TABLEMAX], int table_cnt, int refrence_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX], int frame_cnt) {
	int timestamp = 1, page_fault = 0;
	for (int i = 0; i < reference_cnt; i++) {
		if (page_table[refrence_string[i]].is_valid == true) {
			page_table[refrence_string[i]].last_access_timestamp = timestamp;
			page_table[refrence_string[i]].reference_count += 1;
		}
		else if ((page_table[refrence_string[i]].is_valid == false) && (frame_cnt > 0)) {
			page_fault += 1;
			page_table[refrence_string[i]].is_valid = true;
			page_table[refrence_string[i]].frame_number = frame_pool[frame_cnt - 1];
			frame_cnt -= 1;
			page_table[refrence_string[i]].arrival_timestamp = timestamp;
			page_table[refrence_string[i]].last_access_timestamp = timestamp;
			page_table[refrence_string[i]].reference_count += 1;
		}
		else if ((page_table[refrence_string[i]].is_valid == false) && (frame_cnt == 0)) {
			page_fault += 1;
			bool flag_first = false, flag_equal = false;
			int temp_index = 0;
			int temp_frame = 0;
			int temp_f_used = 0;
			int temp_arrivel_time = 0;
			for (int index = 0; index < table_cnt; index++) {
				if ((page_table[index].is_valid == true) && (flag_first == false)) {
					flag_first = true;
					temp_index = index;
					temp_frame = page_table[index].frame_number;
					temp_arrivel_time = page_table[index].arrival_timestamp;
					temp_f_used = page_table[index].reference_count;
				}
				else if ((page_table[index].is_valid == true) && (temp_f_used >= page_table[index].reference_count) && (flag_first == true)) {
					if (((temp_f_used == page_table[index].reference_count) && (temp_arrivel_time > page_table[index].arrival_timestamp))) {
						flag_equal = true;
						temp_index = index;
						temp_frame = page_table[index].frame_number;
						temp_arrivel_time = page_table[index].arrival_timestamp;
						temp_f_used = page_table[index].reference_count;
					}
					else if ((temp_f_used > page_table[index].reference_count) && (flag_equal == false)) {
						temp_index = index;
						temp_frame = page_table[index].frame_number;
						temp_arrivel_time = page_table[index].arrival_timestamp;
						temp_f_used = page_table[index].reference_count;
					}
				}
			}
			if (flag_first == true) {
				page_table[temp_index].arrival_timestamp = -1;
				page_table[temp_index].reference_count = -1;
				page_table[temp_index].last_access_timestamp = -1;
				page_table[temp_index].frame_number = -1;
				page_table[temp_index].is_valid = false;
				page_table[refrence_string[i]].frame_number = temp_frame;
				page_table[refrence_string[i]].arrival_timestamp = timestamp;
				page_table[refrence_string[i]].is_valid = true;
				page_table[refrence_string[i]].last_access_timestamp = timestamp;
				page_table[refrence_string[i]].reference_count = 1;
			}
		}
		timestamp += 1;
	}
	return page_fault;
}
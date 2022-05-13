#include <stdio.h>
#include "oslabs.h"
#include <string.h>

struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int* queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp){
	if ((current_process.process_id == 0) && (current_process.total_bursttime == 0) && (current_process.execution_endtime == 0) && (current_process.remaining_bursttime == 0) && (current_process.execution_starttime == 0) && (current_process.arrival_timestamp == 0) && (current_process.process_priority == 0)) {
		new_process.execution_starttime = timestamp;
		new_process.execution_endtime = timestamp + new_process.total_bursttime;
		new_process.remaining_bursttime = new_process.total_bursttime;
		return new_process;
	}
	else if (new_process.process_priority >= current_process.process_priority) {
		new_process.execution_starttime = 0;
		new_process.execution_endtime = 0;
		new_process.remaining_bursttime = new_process.total_bursttime;
		ready_queue[*queue_cnt] = new_process;
		*queue_cnt = *queue_cnt + 1;
		return current_process;
	}
	else {
		new_process.execution_starttime = timestamp;
		new_process.execution_endtime = timestamp + new_process.total_bursttime;
		new_process.remaining_bursttime = new_process.total_bursttime;
		current_process.remaining_bursttime = current_process.execution_endtime - timestamp;
		current_process.execution_endtime = 0;
		ready_queue[*queue_cnt] = current_process;
		*queue_cnt = *queue_cnt + 1;
		return new_process;
	}
}
struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int* queue_cnt, int timestamp) {
	if (*queue_cnt > 0) {
		struct PCB next_process;
		int priorty = ready_queue[0].process_priority;
		int temp_queue = 0;
		for (int i = 1; i <= *queue_cnt - 1; i++) {
			if (priorty > ready_queue[i].process_priority) {
				priorty = ready_queue[i].process_priority;
				temp_queue = i;
			}
		}
		next_process = ready_queue[temp_queue];
		if (*queue_cnt == 1) {
			ready_queue[0].process_id = 0;
			ready_queue[0].arrival_timestamp = 0;
			ready_queue[0].total_bursttime = 0;
			ready_queue[0].execution_starttime = 0;
			ready_queue[0].execution_endtime = 0;
			ready_queue[0].remaining_bursttime = 0;
			ready_queue[0].process_priority = 0;
		}
		else {
			ready_queue[temp_queue] = ready_queue[*queue_cnt - 1];
			*queue_cnt = *queue_cnt - 1;
			next_process.execution_starttime = timestamp;
			next_process.execution_endtime = timestamp + next_process.total_bursttime;
			return next_process;
		}
	}
	else {
		struct PCB null_PCB;
		null_PCB.process_id = 0;
		null_PCB.arrival_timestamp = 0;
		null_PCB.total_bursttime = 0;
		null_PCB.execution_starttime = 0;
		null_PCB.execution_endtime = 0;
		null_PCB.remaining_bursttime = 0;
		null_PCB.process_priority = 0;
		return null_PCB;
	}
}

struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int* queue_cnt, struct PCB current_process, struct PCB new_process, int time_stamp) {
	if ((current_process.process_id == 0) && (current_process.total_bursttime == 0) && (current_process.execution_endtime == 0) && (current_process.remaining_bursttime == 0) && (current_process.execution_starttime == 0) && (current_process.arrival_timestamp == 0) && (current_process.process_priority == 0)) {
		new_process.execution_starttime = time_stamp;
		new_process.execution_endtime = time_stamp + new_process.total_bursttime;
		new_process.remaining_bursttime = new_process.total_bursttime;
		return new_process;
	}
	else if (new_process.total_bursttime > current_process.remaining_bursttime) {
		new_process.execution_starttime = 0;
		new_process.execution_endtime = 0;
		new_process.remaining_bursttime = new_process.total_bursttime;
		ready_queue[*queue_cnt] = new_process;
		*queue_cnt = *queue_cnt + 1;
		return current_process;
	}
	else {
		new_process.execution_starttime = time_stamp;
		new_process.execution_endtime = time_stamp + new_process.total_bursttime;
		new_process.remaining_bursttime = new_process.total_bursttime;
		current_process.remaining_bursttime = current_process.execution_endtime - time_stamp;
		current_process.execution_endtime = 0;
		current_process.execution_starttime = 0;
		ready_queue[*queue_cnt] = current_process;
		*queue_cnt = *queue_cnt + 1;
		return new_process;
	}
}
struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int* queue_cnt, int timestamp) {
	if (*queue_cnt > 0) {
		struct PCB next_process;
		int r_bursttime = ready_queue[0].remaining_bursttime;
		int temp_queue = 0;
		for (int i = 1; i <= *queue_cnt - 1; i++) {
			if (r_bursttime > ready_queue[i].remaining_bursttime) {
				r_bursttime = ready_queue[i].remaining_bursttime;
				temp_queue = i;
			}
		}
		next_process = ready_queue[temp_queue];
		if (*queue_cnt == 1) {
			ready_queue[0].process_id = 0;
			ready_queue[0].arrival_timestamp = 0;
			ready_queue[0].total_bursttime = 0;
			ready_queue[0].execution_starttime = 0;
			ready_queue[0].execution_endtime = 0;
			ready_queue[0].remaining_bursttime = 0;
			ready_queue[0].process_priority = 0;
		}
		else {
			ready_queue[temp_queue] = ready_queue[*queue_cnt - 1];
			*queue_cnt = *queue_cnt - 1;
			next_process.execution_starttime = timestamp;
			next_process.execution_endtime = timestamp + next_process.remaining_bursttime;
			return next_process;
		}
	}
	else {
		struct PCB null_PCB;
		null_PCB.process_id = 0;
		null_PCB.arrival_timestamp = 0;
		null_PCB.total_bursttime = 0;
		null_PCB.execution_starttime = 0;
		null_PCB.execution_endtime = 0;
		null_PCB.remaining_bursttime = 0;
		null_PCB.process_priority = 0;
		return null_PCB;
	}
}

struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int* queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum) {
	if ((current_process.process_id == 0) && (current_process.total_bursttime == 0) && (current_process.execution_endtime == 0) && (current_process.remaining_bursttime == 0) && (current_process.execution_starttime == 0) && (current_process.arrival_timestamp == 0) && (current_process.process_priority == 0)) {
		new_process.execution_starttime = timestamp;
		if (time_quantum <= new_process.total_bursttime) {
			new_process.execution_endtime = timestamp + time_quantum;
		}
		else {
			new_process.execution_endtime = timestamp + new_process.total_bursttime;
		}
		new_process.remaining_bursttime = new_process.total_bursttime;
		return new_process;
	}
	else {
		new_process.execution_starttime = 0;
		new_process.execution_endtime = 0;
		new_process.remaining_bursttime = new_process.total_bursttime;
		ready_queue[*queue_cnt] = new_process;
		*queue_cnt = *queue_cnt + 1;
		return current_process;
	}
}
struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int* queue_cnt, int timestamp, int time_quantum) {
	if (*queue_cnt > 0) {
		struct PCB next_process;
		int arr_timestamp = ready_queue[0].arrival_timestamp;
		int temp_queue = 0;
		for (int i = 1; i <= *queue_cnt - 1; i++) {
			if (arr_timestamp > ready_queue[i].arrival_timestamp) {
				arr_timestamp = ready_queue[i].arrival_timestamp;
				temp_queue = i;
			}
		}
		next_process = ready_queue[temp_queue];
		if (*queue_cnt == 1) {
			ready_queue[0].process_id = 0;
			ready_queue[0].arrival_timestamp = 0;
			ready_queue[0].total_bursttime = 0;
			ready_queue[0].execution_starttime = 0;
			ready_queue[0].execution_endtime = 0;
			ready_queue[0].remaining_bursttime = 0;
			ready_queue[0].process_priority = 0;
		}
		else {
			ready_queue[temp_queue] = ready_queue[*queue_cnt - 1];
			*queue_cnt = *queue_cnt - 1;
			next_process.execution_starttime = timestamp;
			if (time_quantum <= next_process.remaining_bursttime) {
				next_process.execution_endtime = timestamp + time_quantum;
			}
			else {
				next_process.execution_endtime = timestamp + next_process.remaining_bursttime;
			}
			return next_process;
		}
	}
	else {
		struct PCB null_PCB;
		null_PCB.process_id = 0;
		null_PCB.arrival_timestamp = 0;
		null_PCB.total_bursttime = 0;
		null_PCB.execution_starttime = 0;
		null_PCB.execution_endtime = 0;
		null_PCB.remaining_bursttime = 0;
		null_PCB.process_priority = 0;
		return null_PCB;
	}
}
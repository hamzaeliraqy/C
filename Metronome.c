
#ifndef SRC_METRONOME_H_
#define SRC_METRONOME_H_

struct ioattr_t;
#define IOFUNC_ATTR_T struct ioattr_t
struct metro_ocb;
#define IOFUNC_OCB_T struct metro_ocb


#include <sys/iofunc.h>
#include <sys/dispatch.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <math.h>
#include <sys/types.h>
#include <sys/netmgr.h>
#include <sys/neutrino.h>




#define METRO_PULSE_CODE _PULSE_CODE_MINAVAIL
#define PAUSE_PULSE_CODE (_PULSE_CODE_MINAVAIL +1)
#define START_PULSE_CODE (_PULSE_CODE_MINAVAIL +2)
#define STOP_PULSE_CODE (_PULSE_CODE_MINAVAIL +3)
#define QUIT_PULSE_CODE (_PULSE_CODE_MINAVAIL +4)
#define SET_PULSE_CODE (_PULSE_CODE_MINAVAIL +5)

#define METRO_ATTACH "metronome"

#define START 0
#define ERROR -1

typedef union {
	struct _pulse pulse;
	char msg[255];
} my_message_t;

struct DataTableRow {
	int tst;
	int tsb;
	int intervals;
	char pattern[16];
};

struct DataTableRow mTable[] = { { 2, 4, 4, "|1&2&" }, { 3, 4, 6, "|1&2&3&" }, { 4,
		4, 8, "|1&2&3&4&" }, { 5, 4, 10, "|1&2&3&4-5-" },
		{ 3, 8, 6, "|1-2-3-" }, { 6, 8, 6, "|1&a2&a" },
		{ 9, 8, 9, "|1&a2&a3&a" }, { 12, 8, 12, "|1&a2&a3&a4&a" } };

struct Metronome_Properties {
	int bpm;
	int tst;
	int tsb;
} typedef metro_props_t;

struct Timer_Properties {
	double bps;
	double measure;
	double interval;
	double nano_sec;
} typedef timer_props_t;

struct Metronome {
	metro_props_t m_props;
	timer_props_t t_props;
} typedef Metronome_t;

#define DEVICES 2
#define METRONOME_HELP 1

char *devnames[DEVICES] = {
		"/dev/local/metronome", "/dev/local/metronome-help"
};

typedef struct ioattr_t {
	iofunc_attr_t attr;
	int device;
} ioattr_t;

typedef struct metro_ocb {
	iofunc_ocb_t ocb;
	char buffer[50];
} metro_ocb_t;


int io_read(resmgr_context_t *ctp, io_read_t *msg, metro_ocb_t *mocb);
int io_write(resmgr_context_t *ctp, io_write_t *msg, RESMGR_OCB_T *ocb);
int io_open(resmgr_context_t *ctp, io_open_t *msg, RESMGR_HANDLE_T *handle, void *extra);
void *metronome_thread(void*metronome);
void set_timer_props(Metronome_t * Metronome);
int search_idx_table(Metronome_t * Metronome);
void stop_timer(struct itimerspec * itime, timer_t timer_id);
void start_timer(struct itimerspec * itime, timer_t timer_id,
		Metronome_t* Metronome);
metro_ocb_t * metro_ocb_calloc(resmgr_context_t *ctp, IOFUNC_ATTR_T *mtattr);
void metro_ocb_free(IOFUNC_OCB_T *mocb);
void check_arg_count(int argc);
void exit_failure();

#endif

name_attach_t *attach;
Metronome_t Metronome;
int server_coid;
char data[255];
int MAX_NUMBER_OF_ARGUMENTS = 4;



int main(int argc, char *argv[]) {

	dispatch_t *dpp;
	resmgr_io_funcs_t io_funcs;
	resmgr_connect_funcs_t conn_funcs;
	ioattr_t ioattr[DEVICES];
	pthread_attr_t thread_attrib;
	dispatch_context_t *ctp;

	int id = 0;
	int i = 0;

	int server_coid = 0;

	check_arg_count(argc);

	iofunc_funcs_t metro_ocb_funcs = {
		_IOFUNC_NFUNCS, metro_ocb_calloc, metro_ocb_free, };

	iofunc_mount_t metro_mount = {0, 0, 0, 0, &metro_ocb_funcs};

	Metronome.m_props.bpm = atoi(argv[1]);
	Metronome.m_props.tst = atoi(argv[2]);
	Metronome.m_props.tsb = atoi(argv[3]);

	/*Metronome.m_props.bpm = 200;
	Metronome.m_props.tst = 2;
	Metronome.m_props.tsb = 4;*/

	dpp = dispatch_create();

	if(dpp == NULL){
		fprintf(stderr, "%s: Unable to allocate dispatch conext.\n", argv[0]);
		return (EXIT_FAILURE);
	}

	iofunc_func_init(_RESMGR_CONNECT_NFUNCS, &conn_funcs, _RESMGR_IO_NFUNCS, &io_funcs);
	conn_funcs.open = io_open;
	io_funcs.read = io_read;
	io_funcs.write = io_write;

	for(i = 0; i < DEVICES; i++){
		iofunc_attr_init(&ioattr[i].attr, S_IFCHR | 066, NULL, NULL);
		ioattr[i].device = i;
		ioattr[i].attr.mount = &metro_mount;

		id = resmgr_attach(dpp, NULL, devnames[i], _FTYPE_ANY, 0, &conn_funcs, &io_funcs, &ioattr[i]);

		if(id == ERROR){
			fprintf(stderr, "%s; Unable to attach name .\n", argv[0]);
			return (EXIT_FAILURE);
		}
	}


	ctp = dispatch_context_alloc(dpp);

	pthread_attr_init(&thread_attrib);
	pthread_create(NULL, &thread_attrib, &metronome_thread, &Metronome);

	while (1){
		if((ctp = dispatch_block(ctp))){
			dispatch_handler(ctp);
		}else{
			printf("ERROR \n");
		}
	}


	pthread_attr_destroy(&thread_attrib);
	name_detach(attach, 0);
	name_close(server_coid);


	return EXIT_SUCCESS;

}


int io_read(resmgr_context_t *ctp, io_read_t *msg, metro_ocb_t *mocb){

	int index;
	int nb;

	if(data == NULL){
		return 0;
	}

	if(mocb->ocb.attr->device == METRONOME_HELP){

		sprintf(data, "Metronome Resource Manager (Resmgr)\n\nUsage: metronome <bpm> <ts-top> <ts-bottom>\n\nAPI:\n pause[1-9]\t\t\t-pause the metronome for 1-9 seconds\n quit:\t\t\t\t- quit the metronome\n set <bpm> <ts-top> <ts-bottom>\t- set the metronome to <bpm> ts-top/ts-bottom\n start\t\t\t\t- start the metronome from stopped state\n stop\t\t\t\t- stop the metronome; use 'start' to resume\n");

	} else{
		 index = search_idx_table(&Metronome);

		 sprintf(data, "[metronome: %d beats/min, time signature: %d/%d, sec-per-interval: %.2f, nanoSecs: %.0lf]\n",
					Metronome.m_props.bpm, mTable[index].tst, mTable[index].tsb,
					Metronome.t_props.interval, Metronome.t_props.nano_sec);
	}

	nb = strlen(data);

	if(mocb->ocb.offset == nb){
		return 0;
	}

	nb = min(nb, msg->i.nbytes);

	_IO_SET_READ_NBYTES(ctp, nb);

	SETIOV(ctp->iov, data, nb);

	mocb->ocb.offset += nb;

	if(nb > 0){
		mocb->ocb.flags |= IOFUNC_ATTR_ATIME;
	}

	return (_RESMGR_NPARTS(1));

}


int io_write(resmgr_context_t *ctp, io_write_t *msg, metro_ocb_t *mocb){

	int nb = 0;

	if(mocb->ocb.attr->device == METRONOME_HELP){
		printf("\nError: Cannot Write to device /dev/local/metronome-help\n");
		nb = msg->i.nbytes;
		_IO_SET_WRITE_NBYTES(ctp, nb);
		return (_RESMGR_NPARTS(0));
	}

	if(msg->i.nbytes == ctp->info.msglen - (ctp->offset + sizeof(*msg))){

		char *buf;
		char *pause_msg;
		char *set_msg;
		int i, small_integer = 0;
		buf = (char *) (msg + 1);

		if(strstr(buf, "pause") != NULL){
			for(i = 0; i < 2; i++){
				pause_msg = strsep(&buf, " ");
			}
			small_integer = atoi(pause_msg);
			if(small_integer >= 1 && small_integer <= 9 ){
				MsgSendPulse(server_coid, SchedGet(0, 0, NULL), PAUSE_PULSE_CODE, small_integer);

			}else{
				printf("Integer is not between 1 and 9.\n");
			}

		} else if (strstr(buf, "quit") != NULL){
			MsgSendPulse(server_coid, SchedGet(0, 0, NULL), QUIT_PULSE_CODE, small_integer);
		} else if (strstr(buf, "start") != NULL){
			MsgSendPulse(server_coid, SchedGet(0, 0, NULL), START_PULSE_CODE, small_integer);
		} else if (strstr(buf, "stop") != NULL){
			MsgSendPulse(server_coid, SchedGet(0, 0, NULL), STOP_PULSE_CODE, small_integer);
		} else if (strstr(buf, "set") != NULL){
			for(i = 0; i < 4; i++){
				set_msg = strsep(&buf, " ");

				if(i == 1){
					Metronome.m_props.bpm = atoi(set_msg);
				} else if (i == 2){
					Metronome.m_props.tst = atoi(set_msg);
				} else if (i == 3){
					Metronome.m_props.tsb = atoi(set_msg);
				}
			}

			MsgSendPulse(server_coid, SchedGet(0, 0, NULL), SET_PULSE_CODE, small_integer);
		} else {
			printf("\nInvalid Command \n");
			strcpy(data, buf);
		}

		nb = msg->i.nbytes;

	}

	_IO_SET_WRITE_NBYTES(ctp, nb);

	if(msg->i.nbytes > 0){
		mocb->ocb.flags |= IOFUNC_ATTR_MTIME | IOFUNC_ATTR_CTIME;
	}

	return (_RESMGR_NPARTS(0));

}

int io_open(resmgr_context_t *ctp, io_open_t *msg, RESMGR_HANDLE_T *handle, void *extra){

	server_coid = name_open("metronome", 0);
	if(server_coid == ERROR){
		perror("ERROR - name_open failed \n");
		return EXIT_FAILURE;
	}

	return (iofunc_open_default(ctp, msg, &handle->attr, extra));

}

void *metronome_thread(void *ta){
	struct sigevent event;
	struct itimerspec itime;
	int rcvid;
	int index = 0;
	char *tp;
	int timer_status;
	timer_t timer_id;
	my_message_t msg;

	attach = name_attach(NULL, "metronome", 0);

	if(attach == NULL){
		printf("Error - Could not attach \n");
		exit(EXIT_FAILURE);
	}

	event.sigev_notify = SIGEV_PULSE;
	event.sigev_coid = ConnectAttach(ND_LOCAL_NODE, 0, attach ->chid, _NTO_SIDE_CHANNEL, 0);
	event.sigev_priority = SIGEV_PULSE_PRIO_INHERIT;
	event.sigev_code = METRO_PULSE_CODE;

	timer_create(CLOCK_REALTIME, &event, &timer_id);
	index = search_idx_table(&Metronome);
	set_timer_props(&Metronome);
	start_timer(&itime, timer_id, &Metronome);
	tp = mTable[index].pattern;

	for(;;){
		rcvid = MsgReceive(attach->chid, &msg, sizeof(msg), NULL);
		if(rcvid == -1){
			printf("Error - No message recieved");
			exit(EXIT_FAILURE);
		}

		if(rcvid == 0){
			switch (msg.pulse.code){
			case METRO_PULSE_CODE:
				if(*tp == '|'){
					printf("%.2s", tp);
					tp = (tp + 2);
				} else if (*tp == '\0'){
					printf("\n");
					tp = mTable[index].pattern;
				} else {
					printf("%c", *tp++);
				}
				break;
			case PAUSE_PULSE_CODE:
				if(timer_status == 0){
					itime.it_value.tv_sec = msg.pulse.value.sival_int;
					timer_settime(timer_id, 0, &itime, NULL);
				}
				break;
			case QUIT_PULSE_CODE:
				timer_delete(timer_id);
				name_detach(attach, 0);
				name_close(server_coid);
				exit(EXIT_SUCCESS);
				break;
			case SET_PULSE_CODE:
				index = search_idx_table(&Metronome);
				tp = mTable[index].pattern;
				set_timer_props(&Metronome);
				start_timer(&itime, timer_id, &Metronome);
				printf("\n");
				break;
			case START_PULSE_CODE:
				if(timer_status == 1){
					start_timer(&itime, timer_id, &Metronome);
					timer_status = START;
				}
				break;
			case STOP_PULSE_CODE:
				if(timer_status == 0 || timer_status == 2){
					stop_timer(&itime, timer_id);
					timer_status = 1;
				}
				break;
			}
		}
		fflush(stdout);
	}

	return NULL;
}

void set_timer_props(Metronome_t *Metronome){
	Metronome->t_props.bps = (double) 60 / Metronome->m_props.bpm;
	Metronome->t_props.measure = Metronome->t_props.bps * 2;
	Metronome->t_props.interval = Metronome->t_props.measure / Metronome->m_props.tsb;
	Metronome->t_props.nano_sec = (Metronome->t_props.interval - (int) Metronome->t_props.interval) * 1e+9;

}

int search_idx_table(Metronome_t *Metronome){
	int index = 0;
	for(index = 0; index < 8; index++){
		if(mTable[index].tsb == Metronome->m_props.tsb && mTable[index].tst == Metronome->m_props.tst){
			return index;
		}
	}
	return ERROR;
}

void stop_timer(struct itimerspec *itime, timer_t timer_id){
	itime->it_value.tv_sec = 0;
	timer_settime(timer_id, 0, itime, NULL);
}

void start_timer(struct itimerspec *itime, timer_t timer_id, Metronome_t *Metronome){
	itime->it_value.tv_sec = 1;
	itime->it_value.tv_nsec = 0;
	itime->it_interval.tv_nsec = Metronome->t_props.nano_sec;
	timer_settime(timer_id, 0, itime, NULL);
}

metro_ocb_t *metro_ocb_calloc(resmgr_context_t *ctp, ioattr_t *mattr){
	metro_ocb_t *mocb;
	mocb = calloc(1, sizeof(metro_ocb_t));
	mocb->ocb.offset = 0;
	return(mocb);
}

void metro_ocb_free(metro_ocb_t *mocb){
	free(mocb);
}


void check_arg_count(int argc){
	if(argc != MAX_NUMBER_OF_ARGUMENTS){
		printf("ERROR - Not enough arguments \n");
		printf("Command: ./metronome <beats/minute> <time-signature-top> <time-signature-bottom>\n");
		exit_failure();
	}
}

void exit_failure(){
	exit(1);
}













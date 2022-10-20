#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <errno.h>
#include <C:\Users\hamza\cst8244_ass1.ws\des_display\src\des.h>

using namespace std;

#define NOERROR;

void *state_left_scan();
void *state_left_open();
void *state_left_close();
void *state_right_scan();
void *state_right_open();
void *state_right_close();
void *state_guard_right_lock();
void *state_guard_right_unlock();
void *state_guard_left_lock();
void *state_guard_left_unlock();
void *state_weight_scale();
void *state_exit();
void *state_inv();

void initialize();

int coid;
int chid;
int rcvid;

controller_response_struct controller_response;
person_struct person;

int main(int argc, char* argv[]) {


		pid_t displayId;
		int co;


		co = atoi(argv[1]);
		displayId = (pid_t) co;


		chid = ChannelCreate(0);
						if (chid == -1) {
							cout << errorMessages[CTRL_ERR_CHANNEL_CREATE] << endl;
							exit(EXIT_FAILURE);
						}


		coid = ConnectAttach(ND_LOCAL_NODE, displayId, 1, _NTO_SIDE_CHANNEL, 0);



		if (coid == -1) {
			cout << errorMessages[CTRL_ERR_CONN] << endl;
			exit(EXIT_FAILURE);
		}

		cout << outMessage[OUT_START] << getpid() << endl;
		cout << "Waiting for person.." << endl;
		initialize();

		while (1) {
			rcvid = MsgReceive(chid, &person, sizeof(person), NULL); //2
			MsgReply(rcvid, EOK, &controller_response,sizeof(controller_response) + 1);
			if (rcvid == -1) {
				cout << errorMessages[CTRL_ERR_RCV] << endl;
				exit(EXIT_FAILURE);
			}

			if (person.state == 12) {
				state_exit();
				break;
			}
			if(person.state == 1){
				state_left_scan();
				controller_response.person.state = ST_LS;

			}

			if(person.state == 11){
				if(controller_response.person.state == 1 || controller_response.person.state == 8){
					state_guard_left_unlock();
					controller_response.person.state = ST_GLU;
							}
						else{
						state_inv();
						}
			}

			if(person.state == 4){
				if(controller_response.person.state == 11){

					state_left_open();
					controller_response.person.state = ST_LO;
				}
				else{
					state_inv();
				}

			}

			if(person.state == 3){
				if(controller_response.person.state == 4 || controller_response.person.state == 5){

					state_weight_scale();
					controller_response.person.state = ST_WS;
				}
				else{
					state_inv();
				}


			}

			if(person.state == 6){
				if(controller_response.person.state == 3 || controller_response.person.state == 4){

					state_left_close();
					controller_response.person.state = ST_LC;
				}
				else{
					state_inv();
				}

			}
			if(person.state == 10){
				if(controller_response.person.state == 6){

					state_guard_left_lock();
					controller_response.person.state = 10;
				}
				else{
					state_inv();
				}

			}
			if(person.state == 9){
				if(controller_response.person.state == 10 || controller_response.person.state == 2){

					state_guard_right_unlock();
					controller_response.person.state = 9;
				}
				else{
					state_inv();
				}



			}
			if(person.state == 5){
				if(controller_response.person.state == 9){

					state_right_open();
					controller_response.person.state = 5;
				}
				else{
					state_inv();
				}
			}
			if(person.state == 7){
				if(controller_response.person.state == 5 || controller_response.person.state == 3){

					state_right_close();
					controller_response.person.state = 7;
				}
				else{
					state_inv();
				}
			}
			if(person.state == 8){
				if(controller_response.person.state == 7){

					state_guard_right_lock();
					controller_response.person.state = 8;
				}
				else{
					state_inv();
				}
			}
			if(person.state == 2){
				state_right_scan();
				controller_response.person.state = ST_RS;
			}
			if(person.state == 13){
				state_inv();
			}

			controller_response.statusCode = EOK;
		}
		ConnectDetach(coid);
		ChannelDestroy(chid);


		return EXIT_SUCCESS;


}

void *state_inv(){
	person.state = 15;
	MsgSend(coid, &person, sizeof(person) + 1, &controller_response, sizeof(controller_response));

}

void *state_left_scan() {
	MsgSend(coid, &person, sizeof(person) + 1, &controller_response, sizeof(controller_response));

}

void *state_guard_left_unlock() {
	MsgSend(coid, &person, sizeof(person) + 1, &controller_response, sizeof(controller_response));

}

void *state_left_open() {
	MsgSend(coid, &person, sizeof(person) + 1, &controller_response, sizeof(controller_response));

}

void *state_right_scan() {
	MsgSend(coid, &person, sizeof(person) + 1, &controller_response, sizeof(controller_response));

}

void *state_guard_right_unlock() {
	MsgSend(coid, &person, sizeof(person) + 1, &controller_response, sizeof(controller_response));

}

void *state_right_open() {
	MsgSend(coid, &person, sizeof(person) + 1, &controller_response, sizeof(controller_response));

}

void *state_weight_scale() {
	MsgSend(coid, &person, sizeof(person) + 1, &controller_response, sizeof(controller_response));

}

void *state_left_close() {
	MsgSend(coid, &person, sizeof(person) + 1, &controller_response, sizeof(controller_response));

}

void *state_right_close() {
	MsgSend(coid, &person, sizeof(person) + 1, &controller_response, sizeof(controller_response));

}

void *state_guard_right_lock() {
	MsgSend(coid, &person, sizeof(person) + 1, &controller_response, sizeof(controller_response));


}


void *state_guard_left_lock() {
	MsgSend(coid, &person, sizeof(person) + 1, &controller_response, sizeof(controller_response));

}

void *state_exit() {
	MsgSend(coid, &person, sizeof(person) + 1, &controller_response, sizeof(controller_response));
}

void initialize(){
	person.id = 0;
	person.weight = 0;
	person.state = ST_START;
}


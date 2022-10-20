#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <unistd.h>
#include <C:\Users\hamza\cst8244_ass1.ws\des_display\src\des.h>

using namespace std;

int main() {

	person_struct person;
	controller_response_struct response;

	int chid;
	int rcvid;


	chid = ChannelCreate(0);
	if(chid == -1){
		cout << errorMessages[DP_ERR_CHANNEL_CREATE] << endl;
		exit(EXIT_FAILURE);
	}
	cout << "The display is running PID: " << getpid() << endl;

	while(1){
		rcvid = MsgReceive(chid, &person, sizeof(person), NULL);

		MsgReply(rcvid, EOK, &response , sizeof(response) + 1);

		switch (person.state) {
				case ST_LS:
					cout << "Left scan Person ID : " << person.id <<  endl;
					break;
				case ST_RS:
					cout << outMessage[OUT_LS_RS] << person.id <<  endl;
					break;
				case ST_WS:
					cout << outMessage[OUT_WS] <<  person.weight << endl;
					break;
				case ST_LO:
					cout << outMessage[OUT_LO] << endl;
					break;
				case ST_RO:
					cout << outMessage[OUT_RO] << endl;
					break;
				case ST_LC:
					cout << outMessage[OUT_LC] << endl;
					break;
				case ST_RC:
					cout << outMessage[OUT_RC] << endl;
					break;
				case ST_GRL:
					cout << outMessage[OUT_GRL] << endl;
					break;
				case ST_GRU:
					cout << outMessage[OUT_GRU] << endl;
					break;
				case ST_GLL:
					cout << outMessage[OUT_GLL] << endl;
					break;
				case ST_GLU:
					cout << outMessage[OUT_GLU] << endl;
					break;
				case ST_EXIT:
					cout << outMessage[OUT_EXIT] << endl;
					break;
				default:
					cout << "Invalid Entry" << endl;
					break;
				}

				if (person.state == ST_EXIT){
					exit(0);
					break;
				}

	}
	ChannelDestroy(chid);

	return 0;
}

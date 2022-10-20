#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/neutrino.h>
#include <sys/netmgr.h>
#include <unistd.h>
#include <C:\Users\hamza\cst8244_ass1.ws\des_display\src\des.h>

using namespace std;


int main(int argc, char* argv[]) {

	int MAX_NUMBER_SECONDS = 5;
	pid_t controllerPid;
	int coid;
	int chid = 1;
	person_struct person;
	controller_response_struct controller_response;
	int co;


	co = atoi(argv[1]);
	controllerPid = (pid_t) co;




	coid = ConnectAttach(ND_LOCAL_NODE, controllerPid, chid, _NTO_SIDE_CHANNEL, 0);

	if (coid == -1) {
		cout << stderr << " ERROR: COULD NOT CONNECT" << endl;
		exit(EXIT_FAILURE);
	}

	person.state = ST_GLL;


	while (1) {
		char input[5];
		sleep(1);
		cout << "Enter the event type (ls = left scan, rs = right scan, ws = weight scale, lo = left open, ro = right open, lc = left closed, rc = right closed, gru = guard right unlock, grl = guard right lock, gll = guard left lock, glu = guard left unlock, exit = exit programs)" << endl;

		scanf("%s", &input);

		if (strncmp(input, inMessage[IN_LS], strlen(inMessage[IN_LS])) == 0) {
			person.state = ST_LS;
			cout << "Enter the person ID:  " << endl;
			scanf("%d", &person.id);
		} else if (strncmp(input, inMessage[IN_RS], strlen(inMessage[IN_RS]))
				== 0) {
			person.state = ST_RS;
			cout << "Enter ID:  " << endl;
			cin >> person.id;
		} else if (strncmp(input, inMessage[IN_WS], strlen(inMessage[IN_WS]))
				== 0) {
			person.state = ST_WS;
			cout << "Enter your weight:  " << endl;
			cin >> person.weight;
		} else if (strncmp(input, inMessage[IN_LO], strlen(inMessage[IN_LO]))
				== 0) {
			person.state = ST_LO;
		} else if (strncmp(input, inMessage[IN_RO], strlen(inMessage[IN_RO]))
				== 0){
			person.state = ST_RO;
		}
		else if (strncmp(input, inMessage[IN_LC], strlen(inMessage[IN_LC]))
				== 0){
			person.state = ST_LC;
		}
		else if (strncmp(input, inMessage[IN_RC], strlen(inMessage[IN_RC]))
				== 0){
			person.state = ST_RC;
		}
		else if (strncmp(input, inMessage[IN_GRL], strlen(inMessage[IN_GRL]))
				== 0){
			person.state = ST_GRL;
		}
		else if (strncmp(input, inMessage[IN_GRU], strlen(inMessage[IN_GRU]))
				== 0){
			person.state = ST_GRU;
		}
		else if (strncmp(input, inMessage[IN_GLL], strlen(inMessage[IN_GLL]))
				== 0){
			person.state = ST_GLL;
		}
		else if (strncmp(input, inMessage[IN_GLU], strlen(inMessage[IN_GLU]))
				== 0){
			person.state = ST_GLU;
		}
		else if (strncmp(input, inMessage[IN_EXIT], strlen(inMessage[IN_EXIT]))
				== 0){
			person.state = ST_EXIT;
			MsgSend(coid, &person, sizeof(person) + 1, &controller_response,
									sizeof(controller_response));
			break;
		}
		else{
			person.state = ST_INV;
		}

		MsgSend(coid, &person, sizeof(person) + 1, &controller_response,
						sizeof(controller_response));


}
		sleep(MAX_NUMBER_SECONDS);
		ConnectDetach(coid);
		return EXIT_SUCCESS;
}

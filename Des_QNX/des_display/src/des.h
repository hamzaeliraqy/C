/*
 * des.h
 *
 *  Created on: 18 Jun 2022
 *      Author: hamza
 */

#ifndef DES_H_
#define DES_H_

#define RUNNING 1

#define LEFT 0
#define RIGHT 1
#define DEFAULT 2

 //#define NOERROR 5

struct person{
	int id;
	int weight;
	int state;
}typedef person_struct;

struct ctrl_response{
	person_struct person;
	int statusCode;
	char errMsg[128];
}typedef controller_response_struct;

typedef void *(*Fstate)();

#define NUM_STATES 13

typedef enum {
	ST_START = 0,
	ST_LS = 1,
	ST_RS = 2,
	ST_WS = 3,
	ST_LO = 4,
	ST_RO = 5,
	ST_LC = 6,
	ST_RC = 7,
	ST_GRL = 8,
	ST_GRU = 9,
	ST_GLL = 10,
	ST_GLU = 11,
	ST_EXIT = 12,
	ST_INV = 13
}State;

#define NUM_INPUTS 12

typedef enum {
	IN_LS = 0,
	IN_RS = 1,
	IN_WS = 2,
	IN_LO = 3,
	IN_RO = 4,
	IN_LC = 5,
	IN_RC = 6,
	IN_GRL = 7,
	IN_GRU = 8,
	IN_GLL = 9,
	IN_GLU = 10,
	IN_EXIT = 11
}Input;

const char *inMessage[NUM_INPUTS] =
{ "ls", "rs", "ws", "lo", "ro", "lc", "rc",
		"grl", "gru", "gll", "glu", "exit" };

#define NUM_OUTPUTS 12
typedef enum {
	OUT_START = 0,
	OUT_LS_RS = 1,
	OUT_WS = 2,
	OUT_LO = 3,
	OUT_RO = 4,
	OUT_LC = 5,
	OUT_RC = 6,
	OUT_GRL = 7,
	OUT_GRU = 8,
	OUT_GLL = 9,
	OUT_GLU = 10,
	OUT_EXIT = 11,
} Output;

const char *outMessage[NUM_OUTPUTS] = { "The controller is running as PID:",

"Person scanned ID. ID = ", "Person weighed. Weight = ",
		"Person opened left door", "Person opened right door",
		"Left door closed (automatically) ",
		"Right door closed (automatically).", "Right door locked by Guard. ",
		"Right door unlocked by Guard. ", "Left door locked by Guard. ",
		"Left door unlocked by Guard. ", "Exiting Door Entry System..."

};

typedef enum {
	IN_ERR_CONN = 0,
	IN_ERR_SND = 1,
	IN_ERR_RSP = 2,
	IN_ERR_USG = 3,
	ERR_SRVR_MSG = 4,
	DP_ERR_CHANNEL_CREATE = 5,
	DP_ERR_RCV = 6,
	DP_ERR_RPLY = 7,
	CTRL_ERR_USG = 8,
	CTRL_ERR_CHANNEL_CREATE = 9,
	CTRL_ERR_CONN = 10,
	CTRL_ERR_RCV = 11,
	CTRL_ERR_SND = 12

} Error;

#define NUM_ERROR 13
const char *errorMessages[NUM_ERROR] = {
		"ERROR: Could not Connect to Controller ./des-input ", /* 0 */
		"ERROR: Sending Message ./des-input", /* 1 */
		"ERROR: NULL Response from server ./des-input", /*2*/
		"Missing PID to controller",/*3*/
		"GENERAL-SERVER-ERROR: ",/*4*/
		"ERROR: Unable to create Channel ./des-display",/*5*/
		"ERROR: Message not Received ./des-display",/*6*/
		"ERROR: Replying to Message ./des-display",/*7*/
		"Missing PID to display",
		"ERROR: Unable to Create Channel ./des-controller",/*9*/
		"ERROR: Could not connect to des-display ./des-controller",/*10*/
		"ERROR: Message not received ./des-controller",/*11*/
		"ERROR: Could not send Message ./des-controller STATE: ", /*12*/

};

#endif /* DES_H_ */

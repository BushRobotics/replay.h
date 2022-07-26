#include <stdio.h>
#include <stdlib.h>
#include "replay.h"

#define REPLAY_NAME "test_replay"
#define REPLAY_LEN 5

void print_replay(ReplayStep* replay) {
	for (int i = 0; replay[i-1].last != 1; i++) {
		printf("%d: \n\twheels:\n", i);
		
		for (int j = 0; j < 4; j++) {
			printf("\t\t%d: %f\n", j, replay[i].wheels[j]);
		}
		
		printf("\tprong: %f\r\n\n", replay[i].prong);
	}
}

int main() {
	ReplayStep* replay = malloc(sizeof(ReplayStep) * REPLAY_LEN);
	printf("hello world\r\n");
	
	for (int i = 0; i < REPLAY_LEN; i++) {
		printf("loop %d\r\n", i);
		for (int j = 0; j < 4; j++) {
			replay[i].wheels[j] = j + 1;
		}
		
		replay[i].prong = i;
		replay[i].last = 0;		
	}
	replay[REPLAY_LEN-1].last = 1; // indicate this is the last step
	
	print_replay(replay);
	
	write_replay(replay, REPLAY_NAME);
	
	free(replay);
	
	ReplayStep* nreplay = read_replay(REPLAY_NAME);
	
	print_replay(nreplay);
	
	return 0;
}

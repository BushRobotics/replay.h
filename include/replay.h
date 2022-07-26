typedef struct { // edit me!
	double wheels[4]; // four wheels
	double prong; // a prong motor
	
	// leave this one alone
	int last; // indicate last step (important!!)
} ReplayStep;


void write_replay(ReplayStep *step, char* filename) {
	FILE *f = fopen(filename, "wb");
	for (ReplayStep *i = step; (i-1)->last != 1; i++) {
		fwrite(i, sizeof(ReplayStep), 1, f);
	}
	fclose(f);
}

ReplayStep *read_replay(char* filename) {
	FILE *f = fopen(filename, "rb");
	
	fseek(f, 0, SEEK_END);
	size_t size = ftell(f);
	fseek(f, 0, SEEK_SET);
	printf("file size: %zu\r\n", size);
	
	ReplayStep* replay = malloc(size);
	ReplayStep currentstep;
	int i = 0;
	while (fread(&currentstep, sizeof(ReplayStep), 1, f)) {
		replay[i] = currentstep;
		i++;
	}
	replay[i].last = 1; // just in case
	
	fclose(f);
	return replay;
}

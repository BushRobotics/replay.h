# replay.h
> A C header for recording and playing back VEX V5 robot movements using PROS

## usage
1. Place `replay.h` in your `include` directory
2. Customize it.
	The beginning of the file defines what you're replaying.
	```c
	typedef struct {
		double wheels[4]; // four wheels
		
		
		int last; // indicate last step (important!!)
	} ReplayStep;
	```
	values returned by `motor_get_actual_velocity` are `double`s, but you can put anything in it. Make sure to leave `int last`, as it is used to indicate the last step in your sequence
3. use it!

### Creating a replay
To create a replay sequence, first allocate it.
```c
ReplayStep* replay = malloc(sizeof(ReplayStep) * REPLAY_LEN);
int step = 0;
```
This creates an array\* of `ReplaySteps` called `replay` of length `REPLAY_LEN`, and an integer called `step`, which you'll likely want in order to keep track of the steps.

Now, when you want to record, simply set `replay[step]`'s members to whatever you like (such as motor speeds), and increment `step` before each `delay` call.
When step has reached the desired length, set `replay[step].last` to `1`.
Here's a simple example that records the positions of four wheels
```c
void opcontrol() {
	ReplayStep* replay = malloc(sizeof(ReplayStep) * REPLAY_LEN);
	int step = 0;
	bool recording = false;
	/*
		insert controller-based movement and trigger for record here
	*/
	if (recording) {
		for (int i = 0; i < 4; i++) {
			replay[step].wheels[i] = motor_get_actual_velocity(wheels[i]);
		}
		step++;
		if (step == REPLAY_LEN) {
			replay[step - 1].last = 1; // indicate last step
			write_replay(replay, "/usd/replay"); // write the replay to the SD card
			step = 0;
			recording = false;
		}
	}
	delay(2); // make sure the delay is the same during both record and playback
}
	
```
### Saving a replay
Just call `write_replay(replay, filename)`. If you're using an SD card in your VEX brain, the filename should start with `/usd/`

### Replaying a replay
First, load the replay.
```c
ReplayStep* replay = read_replay(REPLAY_FILENAME);
```
Now, you can do whatever you want with it. The following example spins four wheels as they were recorded.
```c
for (int i = 0; replay[i-1].last != 1; i++) {
	for (int j = 0; j < 4; j++) {
		motor_move_velocity(wheel_port[j], replay[i].wheels[j]);
	}
	delay(2);
}
```

\*don't at me

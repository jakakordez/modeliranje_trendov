
// kernel
__kernel void fill_matrix_opencl(__global int *matrix,
								 __global int *states,
								int STATES, int PAST)
{
	int lid = get_local_id(0);
	int gid = get_global_id(0);
	float s = 0;

	int index = 0;
	for (int i = 0; i < PAST; i++) {
		if (states[gid + i] >= STATES) {
			printf("Error %d\n", states[gid + i]);
		}
		index += states[gid + i];
		index *= STATES;
	}
	index += states[gid + PAST];

	//matrix[index] += 1;
	//printf("index: %d", index);
	//out[gid] = index;
	uint old_val = atomic_inc(&matrix[index]);
}

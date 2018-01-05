
// kernel
__kernel void fill_matrix_opencl(__global int *matrix,
								 __global int *states,
								int num_states, int past)
{
	int lid = get_local_id(0);
	int gid = get_global_id(0);
	float s = 0;

	int index = 0;
	for (int i = 0; i < past; i++) {
		if (states[gid + i] >= num_states) {
			printf("Error %d\n", states[gid + i]);
		}
		index += states[gid + i];
		index *= num_states;
	}
	index += states[gid + past];

	//matrix[index] += 1;
	//printf("index: %d", index);
	//out[gid] = index;
	uint old_val = atomic_inc(&matrix[index]);
}

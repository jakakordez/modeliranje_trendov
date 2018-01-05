
// kernel
__kernel void normalizeRow(__global float *matrix,
						 int STATES, __local float* sum)
{
	int lid = get_local_id(0);
	int gid = get_global_id(0);
	float s = 0;

	sum[lid] = matrix[gid];
	barrier(CLK_LOCAL_MEM_FENCE);

	int floorPow2 = STATES;
	if (floorPow2 & (floorPow2 - 1))
	{
		while (floorPow2 & (floorPow2 - 1))
			floorPow2 &= floorPow2 - 1;
		if (lid >= floorPow2)
			sum[lid - floorPow2] += sum[lid];
		barrier(CLK_LOCAL_MEM_FENCE);
	}

	for (int i = (floorPow2 >> 1); i>0; i >>= 1)
	{
		if (lid < i)
			sum[lid] += sum[lid + i];
		barrier(CLK_LOCAL_MEM_FENCE);
	}

	barrier(CLK_LOCAL_MEM_FENCE);
	s = sum[0];

	if (s != 0){
		matrix[gid] /= s;
	}
}														

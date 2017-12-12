
// kernel
__kernel void normalizeRow(__global float *matrix,
						 int STATES)						
{														
	int i = get_global_id(0);

	float sum = 0;
	for (int j = 0; j < STATES; j++) {
		sum += matrix[i*STATES + j];
	}
	if (sum != 0){
		for (int j = 0; j < STATES; j++) {
			matrix[i*STATES + j] /= sum;
		}
	}
}														

#include "stdafx.h"
#include <math.h>
#include <stdlib.h>
#define CL_USE_DEPRECATED_OPENCL_1_1_APIS
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#include <CL\cl.h>

#include "chain.h"
#include "matrix.h"

int fillMatrix(float * matrix, int *data, int n) {
	int num_of_samples = n - PAST - 1;
	for (int i = 0; i < num_of_samples; i++) {
		ulong num = index(&data[i]);
		//printf("index: %d\n", num);
		matrix[num] += 1;
	}
	return 1;
}
#define MAX_SOURCE_SIZE	16384
#define WORKGROUP_SIZE	pow(STATES, PAST)

void normalizeMatrix(float *matrix) {
	FILE *fp;
	char *source_str;
	size_t source_size;
	cl_int ret;

	fp = fopen("kernel.cl", "r");
	if (!fp) fprintf(stderr, "Failed to read OpenCL kernel.\n");
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	source_str[source_size] = '\0';
	fclose(fp);

	// Podatki o platformi
	cl_platform_id	platform_id[10];
	cl_uint			ret_num_platforms;
	char			*buf;
	size_t			buf_len;
	ret = clGetPlatformIDs(10, platform_id, &ret_num_platforms);
	// max. "stevilo platform, kazalec na platforme, dejansko "stevilo platform

	// Podatki o napravi
	cl_device_id	device_id[10];
	cl_uint			ret_num_devices;
	// Delali bomo s platform_id[0] na GPU
	ret = clGetDeviceIDs(platform_id[0], CL_DEVICE_TYPE_GPU, 10,
		device_id, &ret_num_devices);
	// izbrana platforma, tip naprave, koliko naprav nas zanima
	// kazalec na naprave, dejansko "stevilo naprav

	// Kontekst
	cl_context context = clCreateContext(NULL, 1, &device_id[0], NULL, NULL, &ret);
	// kontekst: vklju"cene platforme - NULL je privzeta, "stevilo naprav, 
	// kazalci na naprave, kazalec na call-back funkcijo v primeru napake
	// dodatni parametri funkcije, "stevilka napake

	// Ukazna vrsta
	//cl_command_queue command_queue = clCreateCommandQueue(context, device_id[0], 0, &ret);
	// kontekst, naprava, INORDER/OUTOFORDER, napake

	// Alokacija pomnilnika na napravi
	cl_mem mem_obj = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, pow(STATES, DIMENSIONS) * sizeof(float), matrix, &ret);

	// Priprava programa
	cl_program program = clCreateProgramWithSource(context, 1, (const char **)&source_str,
		NULL, &ret);
	// kontekst, "stevilo kazalcev na kodo, kazalci na kodo,		
	// stringi so NULL terminated, napaka													

	// Prevajanje
	ret = clBuildProgram(program, 1, &device_id[0], NULL, NULL, NULL);
	// program, "stevilo naprav, lista naprav, opcije pri prevajanju,
	// kazalec na funkcijo, uporabni"ski argumenti

	// Ukazna vrsta
	cl_command_queue command_queue = clCreateCommandQueueWithProperties(context, device_id[0], 0, &ret);

	// Log
	size_t build_log_len;
	char *build_log;
	ret = clGetProgramBuildInfo(program, device_id[0], CL_PROGRAM_BUILD_LOG, 0, NULL, &build_log_len);
	// program, naprava, tip izpisa, 
	// maksimalna dolzina niza, kazalec na niz, dejanska dolzina niza
	build_log = (char *)malloc(sizeof(char)*(build_log_len + 1));
	ret = clGetProgramBuildInfo(program, device_id[0], CL_PROGRAM_BUILD_LOG,
		build_log_len, build_log, NULL);
	printf("%s\n", build_log);
	free(build_log);

	// "s"cepec: priprava objekta
	cl_kernel kernel = clCreateKernel(program, "normalizeRow", &ret);

	int states = STATES;
	clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&mem_obj);
	clSetKernelArg(kernel, 1, sizeof(int), (void *)&states);

	int n = pow(STATES, PAST);

	// Delitev dela
	size_t local_item_size = WORKGROUP_SIZE;
	size_t global_item_size = pow(STATES, PAST);

	// "s"cepec: zagon
	ret = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL,
		&global_item_size, &local_item_size, 0, NULL, NULL);
	// vrsta, "s"cepec, dimenzionalnost, mora biti NULL, 
	// kazalec na "stevilo vseh niti, kazalec na lokalno "stevilo niti, 
	// dogodki, ki se morajo zgoditi pred klicem

	// Kopiranje rezultatov
	ret = clEnqueueReadBuffer(command_queue, mem_obj, CL_TRUE, 0, pow(STATES, DIMENSIONS) * sizeof(int), matrix, 0, NULL, NULL);
	// branje v pomnilnik iz naparave, 0 = offset
	// zadnji trije - dogodki, ki se morajo zgoditi prej

	ret = clFlush(command_queue);
	ret = clFinish(command_queue);
	ret = clReleaseKernel(kernel);
	ret = clReleaseProgram(program);
	ret = clReleaseMemObject(mem_obj);
	ret = clReleaseCommandQueue(command_queue);
	ret = clReleaseContext(context);
}


#include "constants.h"
#include "helpers.h"

using namespace aocl_utils;
double kernel_time = 0.0;

char inputMessage[55] = "aaa";
int zeros = 2;

// Device memory buffers
cl_mem d_inputMessage, d_k, d_h;

// Entry point.
int main(int argc, char **argv) {

	if(!init()) {
		return false;
	}

	std::cout << "Input: " << inputMessage << std::endl;

  	// Allocate host memory
	char* input = (char*) malloc(55 * sizeof(char));
    int k[64][32];
    for (int i = 0; i < 64; i++) {
        for (int j = 0; j < 32; j++) {
            k[i][j] = *(initializer(K[i]) + j);
        }
    }
	int h[8][32];
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 32; j++) {
            h[i][j] = *(initializer(H_HEX[i]) + j);
        }
    }

	if (!input) {
		printf("ERROR: Couldn't create host buffers\n");
		return false;
	}

	// Create device buffers - assign the buffers in different banks for more efficient
	d_inputMessage = clCreateBuffer(context, CL_MEM_READ_WRITE, 55 * sizeof(char), NULL, &status);
	checkError(status, "Failed to allocate device inputMessage buffer\n");
	d_k = clCreateBuffer(context, CL_MEM_READ_WRITE, 64 * 32 * sizeof(char), NULL, &status);
	checkError(status, "Failed to allocate device k buffer\n");
	d_h = clCreateBuffer(context, CL_MEM_READ_WRITE, 8 * 32 * sizeof(char), NULL, &status);
	checkError(status, "Failed to allocate device h buffer\n");

	// Copy data from host to device
	status = clEnqueueWriteBuffer(queue, d_inputMessage, CL_TRUE, 0,
								  55 * sizeof(char), inputMessage,
								  0, NULL, NULL);
	checkError(status, "Failed to copy data to device input");
	status = clEnqueueWriteBuffer(queue, d_k, CL_TRUE, 0,
								  64 * 32 * sizeof(char), k,
								  0, NULL, NULL);
	checkError(status, "Failed to copy data to device k");
	status = clEnqueueWriteBuffer(queue, d_h, CL_TRUE, 0,
								  8 * 32 * sizeof(char), h,
								  0, NULL, NULL);
	checkError(status, "Failed to copy data to device h");

	// Set the kernel arguments
	status = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&d_inputMessage);
	checkError(status, "Failed to set kernel arg 0");
	status = clSetKernelArg(kernel, 1, sizeof(int), (void *)&zeros);
	checkError(status, "Failed to set kernel arg 1");
	status = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&d_k);
	checkError(status, "Failed to set kernel arg 2");
	status = clSetKernelArg(kernel, 3, sizeof(cl_mem), (void *)&d_h);
	checkError(status, "Failed to set kernel arg 3");

	// Launch the kernel - we launch a single work item hence enqueue a task
	// Get the time_stepstamp to evaluate performance
	kernel_time = getCurrentTimestamp();
	status = clEnqueueTask(queue, kernel, 0, NULL, NULL);
	checkError(status, "Failed to launch kernel");
	clFinish(queue);

	int *output = sha256(inputMessage);

	status = clFinish(queue);
	checkError(status, "Failed to finish");

	// Record execution time
	kernel_time = getCurrentTimestamp() - kernel_time;

	// Copy results from device to host
	status = clEnqueueReadBuffer(queue, d_inputMessage, CL_TRUE, 0,
								 55 * sizeof(char), inputMessage,
								 0, NULL, NULL);
	checkError(status, "Failed to copy data from device");

	std::cout << "Kernel time: " << kernel_time << std::endl;
	std::cout << "Hash: " << std::endl;
	for (int i = 0; i < 8 * 32; i++) {
        std::cout << output[i] << std::endl;
    }

	// Free the resources allocated
	cleanup();

	return 0;
}

void cleanup() {
	if(kernel) 
		clReleaseKernel(kernel);  
	if(program) 
		clReleaseProgram(program);
	if(queue) 
		clReleaseCommandQueue(queue);
	if(context) 
		clReleaseContext(context);
}
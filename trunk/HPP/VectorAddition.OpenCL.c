#include <wb.h> //@@ wb include opencl.h for you

#define BLOCK_SIZE 256

//@@ OpenCL Kernel
const char* vaddsrc =
"__kernel void vadd(__global const float *d_a, __global const float *d_b, __global float *d_result)"
"{"
"	int id = get_global_id(0);"
"	d_result[id] = d_a[id] + d_b[id];"
"}";

int main(int argc, char **argv) {
  wbArg_t args;
  int inputLength;
  float *hostInput1;
  float *hostInput2;
  float *hostOutput;
  cl_mem d_a = NULL;
  cl_mem d_b = NULL;
  cl_mem d_r = NULL;

  args = wbArg_read(argc, argv);

  wbTime_start(Generic, "Importing data and creating memory on host");
  hostInput1 = (float *)wbImport(wbArg_getInputFile(args, 0), &inputLength);
  hostInput2 = (float *)wbImport(wbArg_getInputFile(args, 1), &inputLength);
  hostOutput = (float *)malloc(inputLength * sizeof(float));
  wbTime_stop(Generic, "Importing data and creating memory on host");

  wbLog(TRACE, "The input length is ", inputLength);
	
  //@@ Setup OpenCL
  cl_int clerr = CL_SUCCESS;
  cl_device_id device_id = NULL;
  cl_platform_id platform_id = NULL;
  cl_uint ret_num_devices;
  cl_uint ret_num_platforms;
	
  clerr = clGetPlatformIDs(1, &platform_id, &ret_num_platforms);
  clerr = clGetDeviceIDs( platform_id, CL_DEVICE_TYPE_DEFAULT, 1, &device_id, &ret_num_devices);
  if(clerr != CL_SUCCESS)
  {
	  wbLog(TRACE, "clGetDeviceIDs failed");
  }
	
  cl_context context = clCreateContext( NULL, 1, &device_id, NULL, NULL, &clerr);
  if(clerr != CL_SUCCESS)
  {
	  wbLog(TRACE, "clCreateContext failed");
  }
	
  cl_command_queue command_queue = NULL;
  command_queue = clCreateCommandQueue(context, device_id, 0, &clerr);
  if(clerr != CL_SUCCESS)
  {
	  wbLog(TRACE, "clCreateContext failed");
  }

  int size = inputLength * sizeof(float);

  wbTime_start(GPU, "Allocating GPU memory.");
  //@@ Allocate GPU memory here
  d_a = clCreateBuffer(context, CL_MEM_READ_WRITE, size, NULL, &clerr);
  d_b = clCreateBuffer(context, CL_MEM_READ_WRITE, size, NULL, &clerr);
  d_r = clCreateBuffer(context, CL_MEM_READ_WRITE, size, NULL, &clerr);
  if(clerr != CL_SUCCESS)
  {
	  wbLog(TRACE, "clCreateBuffer failed");
  }
  wbTime_stop(GPU, "Allocating GPU memory.");

  wbTime_start(GPU, "Copying input memory to the GPU.");
  //@@ Copy memory to the GPU here
  clerr = clEnqueueWriteBuffer(command_queue, d_a, CL_TRUE, 0, size, hostInput1, 0, NULL, NULL);
  clerr = clEnqueueWriteBuffer(command_queue, d_b, CL_TRUE, 0, size, hostInput2, 0, NULL, NULL);
  wbTime_stop(GPU, "Copying input memory to the GPU.");

  //@@ Initialize the grid and block dimensions here
  cl_program program = NULL;
  program = clCreateProgramWithSource(context, 1, (const char **)&vaddsrc, NULL, &clerr);
  if(clerr != CL_SUCCESS)
  {
	  wbLog(TRACE, "clCreateProgramWithSource failed");
  }
  clerr = clBuildProgram(program, 1, &device_id, NULL, NULL, NULL);
  if(clerr != CL_SUCCESS)
  {
	  wbLog(TRACE, "clBuildProgram failed");
  }
	
  wbTime_start(Compute, "Performing OpenCL computation");
  //@@ Launch the GPU Kernel here
  cl_kernel kernel = NULL;
  kernel = clCreateKernel(program, "vadd", &clerr);
  if(clerr != CL_SUCCESS)
  {
	  wbLog(TRACE, "clCreateKernel failed");
  }
	
  clerr = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&d_a);
  clerr = clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&d_b);
  clerr = clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&d_r);
  if(clerr != CL_SUCCESS)
  {
	  wbLog(TRACE, "clSetKernelArg failed");
  }

  size_t local_item_size = 0;
  size_t global_item_size = 0;
  if(inputLength < BLOCK_SIZE)
  {
    local_item_size = inputLength;	
    global_item_size = inputLength;
  }
  else
  {
    local_item_size = BLOCK_SIZE;	
    global_item_size = ((inputLength - 1) / BLOCK_SIZE + 1) * BLOCK_SIZE;
  }
  wbLog(TRACE, "local_item_size = ", local_item_size, " global_item_size = ", global_item_size);

  cl_event event = NULL;
  clerr = clEnqueueNDRangeKernel(command_queue, kernel, 1, NULL, &global_item_size, &local_item_size, 0, NULL, &event);
  if(clerr != CL_SUCCESS)
  {
	  wbLog(TRACE, "clEnqueueNDRangeKernel failed clerr = ", clerr);
  }
  clerr = clWaitForEvents(1, &event);
  wbTime_stop(Compute, "Performing OpenCL computation");

  wbTime_start(Copy, "Copying output memory to the CPU");
  //@@ Copy the GPU memory back to the CPU here
  clerr = clEnqueueReadBuffer(command_queue, d_r, CL_TRUE, 0, size, hostOutput, 0, NULL, NULL);
  wbTime_stop(Copy, "Copying output memory to the CPU");

  //@@ Flush 
  clerr = clFlush(command_queue);
  clerr = clFinish(command_queue);
  clerr = clReleaseKernel(kernel);
  clerr = clReleaseProgram(program);
  if(clerr != CL_SUCCESS)
  {
	  wbLog(TRACE, "clReleaseMemObject failed");
  }
	
  wbTime_start(GPU, "Freeing GPU Memory");
  //@@ Free the GPU memory here
  clerr = clReleaseMemObject(d_r);
  clerr = clReleaseMemObject(d_b);
  clerr = clReleaseMemObject(d_a);
  if(clerr != CL_SUCCESS)
  {
	  wbLog(TRACE, "clReleaseMemObject failed");
  }
  wbTime_stop(GPU, "Freeing GPU Memory");

  wbSolution(args, hostOutput, inputLength);
	
  //@@ Release OpenCL setup resources
  clerr = clReleaseCommandQueue(command_queue);
  clerr = clReleaseContext(context);

  free(hostInput1);
  free(hostInput2);
  free(hostOutput);

  return 0;
}

#include "GaussianFitterGpu.cuh"

__global__
void func_df_kernel(size_t npeaks, size_t vec_stride, double* vec_data,
                    size_t data_size, double* data_t, 
                    size_t mat_cols, double* matrix) {
    //indexes i (index for data->t[i]) and j (index for gsl_vector)
    int i = blockIdx.x*blockDim.x + threadIdx.x;
    int j = blockIdx.y*blockDim.y + threadIdx.y;

    //verify both i and j are within bounds to calculate sums of derivatives
    if (i < data_size && j < npeaks) {
        //get values from vector since gsl is not ported to gpu
        //equivalent to gsl_vector_get(v, 3*j+0)
        double a = vec_data[(3*j + 0) * vec_stride];
        double b = vec_data[(3*j + 1) * vec_stride];
        double c = vec_data[(3*j + 2) * vec_stride];

        //get data->t[i]
        double ti = data_t[i];
        double zi = (ti - b) / c;
        double ei = exp(-0.5 * zi * zi);
        
        //calculate sums of derivatives
        double a_sum = (-1)*ei;
        double b_sum = (-1)*a*(ti-b)*ei*(1/(c*c));
        double c_sum = (-1)*a*(ti-b)*(ti-b) * ei * (1/(c*c*c)); 

        //set values in matrix since gsl set matrix is not ported to gpu
        //gsl_matrix_set(J, i,3*j+ 0, a_sum);
        matrix[i*mat_cols + (3*j+0)] = a_sum;

        //gsl_matrix_set(J, i,3*j+ 1, b_sum);
        matrix[i*mat_cols + (3*j+1)] = b_sum;

        //gsl_matrix_set(J, i,3*j+ 1, c_sum);
        matrix[i*mat_cols + (3*j+2)] = c_sum;
    } 
}

double* array_double_alloc(double* host_arr, size_t host_size) {
    double* device_arr;

    // allocate memory for struct on device (gpu)
    cudaMalloc(&device_arr, sizeof(double)*host_size);

    cudaMemcpy(device_arr, host_arr, sizeof(double)*host_size, cudaMemcpyHostToDevice);

    return device_arr;
}

void func_df_gpu(const gsl_vector* x, double* data_t, size_t data_t_size, gsl_matrix* J) {
    double* device_vec;
    double* device_data_t;
    double* device_matrix;
    size_t npeaks = x->size/3;

    device_vec = array_double_alloc(x->data, x->size);
    device_data_t = array_double_alloc(data_t, data_t_size);

    cudaMalloc(&device_matrix, J->tda*J->size1*sizeof(double));

    //call kernel
    dim3 dimBlock(16,16);
    dim3 dimGrid((data_t_size + dimBlock.x - 1)/dimBlock.x, 
                   (npeaks + dimBlock.y - 1)/dimBlock.y);

    func_df_kernel<<<dimGrid, dimBlock>>>(npeaks, x->stride, device_vec, 
                                          data_t_size, device_data_t, 
                                          J->tda, device_matrix);

    //sync: wait until gpu threads finish
    cudaDeviceSynchronize();

    //copy output matrix back to host
    cudaMemcpy(J->data, device_matrix, J->size1*J->tda*sizeof(double), cudaMemcpyDeviceToHost);

    //clean up gpu memory
    cudaFree(device_vec);
    cudaFree(device_data_t);
    cudaFree(device_matrix);
}


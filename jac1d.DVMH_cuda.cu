#include <cassert>

#include <cdvmh_helpers.h>



__global__ void loop_jac1d_29_cuda_kernel_int(double * DVMH_RESTRICT A, double * DVMH_RESTRICT B, int boundsLow_1, int boundsHigh_1, int blockOffset) {
    /* Parameters */
    /* Supplementary variables for loop handling */
    int restBlocks, curBlocks;
    /* User variables - loop index variables and other private variables */
    int i;

    restBlocks = blockIdx.x + blockOffset;
    curBlocks = restBlocks;
    i = boundsLow_1 + (curBlocks * blockDim.x + threadIdx.x);
    if (i <= boundsHigh_1)
    {
        do
        {
            A[i] = 0;
            if (i == 0 || i == 8 - 1)
                B[i] = 0;
            else
                B[i] = 2 + i;
        }
        while(0);
    }
}
__global__ void loop_jac1d_29_cuda_kernel_llong(double * DVMH_RESTRICT A, double * DVMH_RESTRICT B, long long boundsLow_1, long long boundsHigh_1, long long blockOffset) {
    /* Parameters */
    /* Supplementary variables for loop handling */
    long long restBlocks, curBlocks;
    /* User variables - loop index variables and other private variables */
    int i;

    restBlocks = blockIdx.x + blockOffset;
    curBlocks = restBlocks;
    i = boundsLow_1 + (curBlocks * blockDim.x + threadIdx.x);
    if (i <= boundsHigh_1)
    {
        do
        {
            A[i] = 0;
            if (i == 0 || i == 8 - 1)
                B[i] = 0;
            else
                B[i] = 2 + i;
        }
        while(0);
    }
}

extern "C" void loop_jac1d_29_cuda(DvmType *pLoopRef, DvmType A_hdr[], DvmType B_hdr[]) {
    DvmType tmpVar;
    /* Loop reference and device number */
    DvmType loop_ref = *pLoopRef;
    DvmType device_num = dvmh_loop_get_device_num_C(loop_ref);
    /* Parameters */
    dvmh_loop_autotransform_C(loop_ref, A_hdr);
    double *A = (double *)dvmh_get_natural_base_C(device_num, A_hdr);
    DvmType A_devHdr[64];
    tmpVar = dvmh_fill_header_C(device_num, A, A_hdr, A_devHdr, 0);
    assert(tmpVar == 0 || tmpVar == 1);
    dvmh_loop_autotransform_C(loop_ref, B_hdr);
    double *B = (double *)dvmh_get_natural_base_C(device_num, B_hdr);
    DvmType B_devHdr[64];
    tmpVar = dvmh_fill_header_C(device_num, B, B_hdr, B_devHdr, 0);
    assert(tmpVar == 0 || tmpVar == 1);
    /* Supplementary variables for loop handling */
    DvmType boundsLow[1], boundsHigh[1], loopSteps[1];
    DvmType blocksS[1];
    DvmType restBlocks;
    dim3 blocks(1, 1, 1), threads(0, 0, 0);
    cudaStream_t stream;

    /* Choose index type for CUDA kernel */
    int kernelIndexT = dvmh_loop_guess_index_type_C(loop_ref);
    if (kernelIndexT == rt_LONG) kernelIndexT = (sizeof(long) <= sizeof(int) ? rt_INT : rt_LLONG);
    assert(kernelIndexT == rt_INT || kernelIndexT == rt_LLONG);

    /* Get CUDA configuration parameters */
    extern DvmType loop_jac1d_29_cuda_kernel_int_regs;
    if (kernelIndexT == rt_INT) dvmh_loop_cuda_get_config_C(loop_ref, 0, loop_jac1d_29_cuda_kernel_int_regs, &threads, &stream, 0);
    extern DvmType loop_jac1d_29_cuda_kernel_llong_regs;
    if (kernelIndexT == rt_LLONG) dvmh_loop_cuda_get_config_C(loop_ref, 0, loop_jac1d_29_cuda_kernel_llong_regs, &threads, &stream, 0);

    /* Calculate computation distribution parameters */
    dvmh_loop_fill_bounds_C(loop_ref, boundsLow, boundsHigh, loopSteps);
    blocksS[0] = ((boundsHigh[0] - boundsLow[0] + loopSteps[0]) / loopSteps[0] + (threads.x - 1)) / threads.x;

    /* GPU execution */
    restBlocks = blocksS[0];
    while (restBlocks > 0) {
        blocks.x = (restBlocks <= 65535 ? restBlocks : (restBlocks <= 65535 * 2 ? restBlocks / 2 : 65535));
        if (kernelIndexT == rt_INT) loop_jac1d_29_cuda_kernel_int<<<blocks, threads, 0, stream>>>(A, B, boundsLow[0], boundsHigh[0], blocksS[0] - restBlocks);
        if (kernelIndexT == rt_LLONG) loop_jac1d_29_cuda_kernel_llong<<<blocks, threads, 0, stream>>>(A, B, boundsLow[0], boundsHigh[0], blocksS[0] - restBlocks);
        restBlocks -= blocks.x;
    }
}

__global__ void loop_jac1d_50_cuda_kernel_int(double * DVMH_RESTRICT A, double * DVMH_RESTRICT B, int boundsLow_1, int boundsHigh_1, double eps, double eps_grid[], int blockOffset) {
    /* Parameters */
    /* Supplementary variables for loop handling */
    int restBlocks, curBlocks;
    /* User variables - loop index variables and other private variables */
    int i;

    restBlocks = blockIdx.x + blockOffset;
    curBlocks = restBlocks;
    i = boundsLow_1 + (curBlocks * blockDim.x + threadIdx.x);
    if (i <= boundsHigh_1)
    {
        do
        {
            double tmp = fabs(B[i] - A[i]);
            eps = ((tmp) > (eps) ? (tmp) : (eps));
            A[i] = B[i];
        }
        while(0);
    }

    /* Write reduction values to global memory */
    eps_grid[threadIdx.x + blockDim.x * (threadIdx.y + blockDim.y * (threadIdx.z + blockDim.z * (blockIdx.x + blockOffset)))] = eps;
}
__global__ void loop_jac1d_50_cuda_kernel_llong(double * DVMH_RESTRICT A, double * DVMH_RESTRICT B, long long boundsLow_1, long long boundsHigh_1, double eps, double eps_grid[], long long blockOffset) {
    /* Parameters */
    /* Supplementary variables for loop handling */
    long long restBlocks, curBlocks;
    /* User variables - loop index variables and other private variables */
    int i;

    restBlocks = blockIdx.x + blockOffset;
    curBlocks = restBlocks;
    i = boundsLow_1 + (curBlocks * blockDim.x + threadIdx.x);
    if (i <= boundsHigh_1)
    {
        do
        {
            double tmp = fabs(B[i] - A[i]);
            eps = ((tmp) > (eps) ? (tmp) : (eps));
            A[i] = B[i];
        }
        while(0);
    }

    /* Write reduction values to global memory */
    eps_grid[threadIdx.x + blockDim.x * (threadIdx.y + blockDim.y * (threadIdx.z + blockDim.z * (blockIdx.x + blockOffset)))] = eps;
}

extern "C" void loop_jac1d_50_cuda(DvmType *pLoopRef, DvmType A_hdr[], DvmType B_hdr[]) {
    DvmType tmpVar;
    /* Loop reference and device number */
    DvmType loop_ref = *pLoopRef;
    DvmType device_num = dvmh_loop_get_device_num_C(loop_ref);
    /* Parameters */
    dvmh_loop_autotransform_C(loop_ref, A_hdr);
    double *A = (double *)dvmh_get_natural_base_C(device_num, A_hdr);
    DvmType A_devHdr[64];
    tmpVar = dvmh_fill_header_C(device_num, A, A_hdr, A_devHdr, 0);
    assert(tmpVar == 0 || tmpVar == 1);
    dvmh_loop_autotransform_C(loop_ref, B_hdr);
    double *B = (double *)dvmh_get_natural_base_C(device_num, B_hdr);
    DvmType B_devHdr[64];
    tmpVar = dvmh_fill_header_C(device_num, B, B_hdr, B_devHdr, 0);
    assert(tmpVar == 0 || tmpVar == 1);
    /* Supplementary variables for loop handling */
    DvmType boundsLow[1], boundsHigh[1], loopSteps[1];
    DvmType blocksS[1];
    DvmType restBlocks;
    dim3 blocks(1, 1, 1), threads(0, 0, 0);
    cudaStream_t stream;

    /* Choose index type for CUDA kernel */
    int kernelIndexT = dvmh_loop_guess_index_type_C(loop_ref);
    if (kernelIndexT == rt_LONG) kernelIndexT = (sizeof(long) <= sizeof(int) ? rt_INT : rt_LLONG);
    assert(kernelIndexT == rt_INT || kernelIndexT == rt_LLONG);

    /* Get CUDA configuration parameters */
    extern DvmType loop_jac1d_50_cuda_kernel_int_regs;
    if (kernelIndexT == rt_INT) dvmh_loop_cuda_get_config_C(loop_ref, 0, loop_jac1d_50_cuda_kernel_int_regs, &threads, &stream, 0);
    extern DvmType loop_jac1d_50_cuda_kernel_llong_regs;
    if (kernelIndexT == rt_LLONG) dvmh_loop_cuda_get_config_C(loop_ref, 0, loop_jac1d_50_cuda_kernel_llong_regs, &threads, &stream, 0);

    /* Calculate computation distribution parameters */
    dvmh_loop_fill_bounds_C(loop_ref, boundsLow, boundsHigh, loopSteps);
    blocksS[0] = ((boundsHigh[0] - boundsLow[0] + loopSteps[0]) / loopSteps[0] + (threads.x - 1)) / threads.x;

    /* Reductions-related stuff */
    double eps;
    double *eps_grid;
    dvmh_loop_cuda_register_red_C(loop_ref, 1, (void **)&eps_grid, 0);
    dvmh_loop_red_init_C(loop_ref, 1, &eps, 0);
    dvmh_loop_cuda_red_prepare_C(loop_ref, 1, blocksS[0] * threads.x * threads.y * threads.z, 0);

    /* GPU execution */
    restBlocks = blocksS[0];
    while (restBlocks > 0) {
        blocks.x = (restBlocks <= 65535 ? restBlocks : (restBlocks <= 65535 * 2 ? restBlocks / 2 : 65535));
        if (kernelIndexT == rt_INT) loop_jac1d_50_cuda_kernel_int<<<blocks, threads, 0, stream>>>(A, B, boundsLow[0], boundsHigh[0], eps, eps_grid, blocksS[0] - restBlocks);
        if (kernelIndexT == rt_LLONG) loop_jac1d_50_cuda_kernel_llong<<<blocks, threads, 0, stream>>>(A, B, boundsLow[0], boundsHigh[0], eps, eps_grid, blocksS[0] - restBlocks);
        restBlocks -= blocks.x;
    }

    dvmh_loop_cuda_red_finish_C(loop_ref, 1);
}

__global__ void loop_jac1d_60_cuda_kernel_int(double * DVMH_RESTRICT A, double * DVMH_RESTRICT B, int boundsLow_1, int boundsHigh_1, int blockOffset) {
    /* Parameters */
    /* Supplementary variables for loop handling */
    int restBlocks, curBlocks;
    /* User variables - loop index variables and other private variables */
    int i;

    restBlocks = blockIdx.x + blockOffset;
    curBlocks = restBlocks;
    i = boundsLow_1 + (curBlocks * blockDim.x + threadIdx.x);
    if (i <= boundsHigh_1)
    {
        do
        {
            B[i] = (A[i - 1] + A[i + 1]) / 2.;
        }
        while(0);
    }
}
__global__ void loop_jac1d_60_cuda_kernel_llong(double * DVMH_RESTRICT A, double * DVMH_RESTRICT B, long long boundsLow_1, long long boundsHigh_1, long long blockOffset) {
    /* Parameters */
    /* Supplementary variables for loop handling */
    long long restBlocks, curBlocks;
    /* User variables - loop index variables and other private variables */
    int i;

    restBlocks = blockIdx.x + blockOffset;
    curBlocks = restBlocks;
    i = boundsLow_1 + (curBlocks * blockDim.x + threadIdx.x);
    if (i <= boundsHigh_1)
    {
        do
        {
            B[i] = (A[i - 1] + A[i + 1]) / 2.;
        }
        while(0);
    }
}

extern "C" void loop_jac1d_60_cuda(DvmType *pLoopRef, DvmType A_hdr[], DvmType B_hdr[]) {
    DvmType tmpVar;
    /* Loop reference and device number */
    DvmType loop_ref = *pLoopRef;
    DvmType device_num = dvmh_loop_get_device_num_C(loop_ref);
    /* Parameters */
    dvmh_loop_autotransform_C(loop_ref, A_hdr);
    double *A = (double *)dvmh_get_natural_base_C(device_num, A_hdr);
    DvmType A_devHdr[64];
    tmpVar = dvmh_fill_header_C(device_num, A, A_hdr, A_devHdr, 0);
    assert(tmpVar == 0 || tmpVar == 1);
    dvmh_loop_autotransform_C(loop_ref, B_hdr);
    double *B = (double *)dvmh_get_natural_base_C(device_num, B_hdr);
    DvmType B_devHdr[64];
    tmpVar = dvmh_fill_header_C(device_num, B, B_hdr, B_devHdr, 0);
    assert(tmpVar == 0 || tmpVar == 1);
    /* Supplementary variables for loop handling */
    DvmType boundsLow[1], boundsHigh[1], loopSteps[1];
    DvmType blocksS[1];
    DvmType restBlocks;
    dim3 blocks(1, 1, 1), threads(0, 0, 0);
    cudaStream_t stream;

    /* Choose index type for CUDA kernel */
    int kernelIndexT = dvmh_loop_guess_index_type_C(loop_ref);
    if (kernelIndexT == rt_LONG) kernelIndexT = (sizeof(long) <= sizeof(int) ? rt_INT : rt_LLONG);
    assert(kernelIndexT == rt_INT || kernelIndexT == rt_LLONG);

    /* Get CUDA configuration parameters */
    extern DvmType loop_jac1d_60_cuda_kernel_int_regs;
    if (kernelIndexT == rt_INT) dvmh_loop_cuda_get_config_C(loop_ref, 0, loop_jac1d_60_cuda_kernel_int_regs, &threads, &stream, 0);
    extern DvmType loop_jac1d_60_cuda_kernel_llong_regs;
    if (kernelIndexT == rt_LLONG) dvmh_loop_cuda_get_config_C(loop_ref, 0, loop_jac1d_60_cuda_kernel_llong_regs, &threads, &stream, 0);

    /* Calculate computation distribution parameters */
    dvmh_loop_fill_bounds_C(loop_ref, boundsLow, boundsHigh, loopSteps);
    blocksS[0] = ((boundsHigh[0] - boundsLow[0] + loopSteps[0]) / loopSteps[0] + (threads.x - 1)) / threads.x;

    /* GPU execution */
    restBlocks = blocksS[0];
    while (restBlocks > 0) {
        blocks.x = (restBlocks <= 65535 ? restBlocks : (restBlocks <= 65535 * 2 ? restBlocks / 2 : 65535));
        if (kernelIndexT == rt_INT) loop_jac1d_60_cuda_kernel_int<<<blocks, threads, 0, stream>>>(A, B, boundsLow[0], boundsHigh[0], blocksS[0] - restBlocks);
        if (kernelIndexT == rt_LLONG) loop_jac1d_60_cuda_kernel_llong<<<blocks, threads, 0, stream>>>(A, B, boundsLow[0], boundsHigh[0], blocksS[0] - restBlocks);
        restBlocks -= blocks.x;
    }
}

extern "C" void initCdvmhGlobals_jac1d_410751730();
extern "C" void finishCdvmhGlobals_jac1d_1316048831();
static DvmhModuleInitializer initCdvmhGlobals_jac1d_410751730_initializer(initCdvmhGlobals_jac1d_410751730, finishCdvmhGlobals_jac1d_1316048831);


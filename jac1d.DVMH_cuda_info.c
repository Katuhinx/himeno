#define LOOP_JAC1D_50_CUDA_KERNEL_INT_REGS 16
#define LOOP_JAC1D_50_CUDA_KERNEL_LLONG_REGS 16
#define LOOP_JAC1D_60_CUDA_KERNEL_INT_REGS 12
#define LOOP_JAC1D_60_CUDA_KERNEL_LLONG_REGS 12
#define LOOP_JAC1D_29_CUDA_KERNEL_LLONG_REGS 10
#define LOOP_JAC1D_29_CUDA_KERNEL_INT_REGS 8

#include <dvmhlib2.h>

#ifdef LOOP_JAC1D_29_CUDA_KERNEL_INT_REGS
    DvmType loop_jac1d_29_cuda_kernel_int_regs = LOOP_JAC1D_29_CUDA_KERNEL_INT_REGS;
#else
    DvmType loop_jac1d_29_cuda_kernel_int_regs = 0;
#endif
#ifdef LOOP_JAC1D_29_CUDA_KERNEL_LLONG_REGS
    DvmType loop_jac1d_29_cuda_kernel_llong_regs = LOOP_JAC1D_29_CUDA_KERNEL_LLONG_REGS;
#else
    DvmType loop_jac1d_29_cuda_kernel_llong_regs = 0;
#endif

#ifdef LOOP_JAC1D_50_CUDA_KERNEL_INT_REGS
    DvmType loop_jac1d_50_cuda_kernel_int_regs = LOOP_JAC1D_50_CUDA_KERNEL_INT_REGS;
#else
    DvmType loop_jac1d_50_cuda_kernel_int_regs = 0;
#endif
#ifdef LOOP_JAC1D_50_CUDA_KERNEL_LLONG_REGS
    DvmType loop_jac1d_50_cuda_kernel_llong_regs = LOOP_JAC1D_50_CUDA_KERNEL_LLONG_REGS;
#else
    DvmType loop_jac1d_50_cuda_kernel_llong_regs = 0;
#endif

#ifdef LOOP_JAC1D_60_CUDA_KERNEL_INT_REGS
    DvmType loop_jac1d_60_cuda_kernel_int_regs = LOOP_JAC1D_60_CUDA_KERNEL_INT_REGS;
#else
    DvmType loop_jac1d_60_cuda_kernel_int_regs = 0;
#endif
#ifdef LOOP_JAC1D_60_CUDA_KERNEL_LLONG_REGS
    DvmType loop_jac1d_60_cuda_kernel_llong_regs = LOOP_JAC1D_60_CUDA_KERNEL_LLONG_REGS;
#else
    DvmType loop_jac1d_60_cuda_kernel_llong_regs = 0;
#endif


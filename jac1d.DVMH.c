#include <cdvmh_helpers.h>

#define DVM0C(n) ((DvmType)(n))
#define DVM0C0 DVM0C(0)
#define DVM0C1 DVM0C(1)
#ifdef _OPENMP
    #define OMP_H_TYPE (HANDLER_TYPE_MASTER | HANDLER_TYPE_PARALLEL)
#else
    #define OMP_H_TYPE 0
#endif

void loop_jac1d_29_host(DvmType *, DvmType [], DvmType []);
void loop_jac1d_29_cuda(DvmType *, DvmType [], DvmType []);
void loop_jac1d_50_host(DvmType *, DvmType [], DvmType []);
void loop_jac1d_50_cuda(DvmType *, DvmType [], DvmType []);
void loop_jac1d_60_host(DvmType *, DvmType [], DvmType []);
void loop_jac1d_60_cuda(DvmType *, DvmType [], DvmType []);

/* Jacobi-1 program */

#include <math.h>
#include <stdio.h>

#define Max(a, b) ((a) > (b) ? (a) : (b))

#define L 8
#define ITMAX 20

int i, it;
double eps;
double MAXEPS = 0.5;

FILE *f;

/* 1D arrays block distributed along 1 dimension */
DvmType A[64];
DvmType B[64];

int main(int an, char **as)
{
    dvmh_line_C(23, "jac1d.c");
#ifdef _OPENMP
    dvmh_init_C(INITFLAG_OPENMP, &an, &as);
#else
    dvmh_init_C(0, &an, &as);
#endif

    {
        dvmh_line_C(25, "jac1d.c");
        DvmType cur_region = dvmh_region_create_C(0);
        DvmType cur_loop = 0;
        dvmh_region_register_array_C(cur_region, INTENT_IN | INTENT_OUT, A, "A");
        dvmh_region_register_array_C(cur_region, INTENT_IN | INTENT_OUT, B, "B");
        dvmh_region_execute_on_targets_C(cur_region, DEVICE_TYPE_HOST | DEVICE_TYPE_CUDA);

    /* 1D parallel loop with base array A */
    dvmh_line_C(28, "jac1d.c");
        cur_loop = dvmh_loop_create_C(cur_region, 1, DVM0C0, DVM0C((8) - 1), DVM0C1);
        dvmh_loop_map_C(cur_loop, A, 1, dvmh_alignment_linear_C(1, 1, 0));
        dvmh_loop_set_cuda_block_C(cur_loop, 256, 1, 1);
        dvmh_loop_register_handler_C(cur_loop, DEVICE_TYPE_HOST, OMP_H_TYPE, dvmh_handler_func_C((DvmHandlerFunc)loop_jac1d_29_host, 2, A, B));
        dvmh_loop_register_handler_C(cur_loop, DEVICE_TYPE_CUDA, 0, dvmh_handler_func_C((DvmHandlerFunc)loop_jac1d_29_cuda, 2, A, B));

        dvmh_line_C(29, "jac1d.c");
        dvmh_loop_perform_C(cur_loop);
        cur_loop = 0;

    
        dvmh_line_C(37, "jac1d.c");
        dvmh_region_end_C(cur_region);
    }

    /* iteration loop */
    for (it = 1; it <= ITMAX; it++)
    {
        eps = 0;
        dvmh_line_C(43, "jac1d.c");
        dvmh_actual_variable2_((const void *)&eps);


        {
            dvmh_line_C(45, "jac1d.c");
            DvmType cur_region = dvmh_region_create_C(0);
            DvmType cur_loop = 0;
            dvmh_region_register_array_C(cur_region, INTENT_IN | INTENT_OUT, A, "A");
            dvmh_region_register_array_C(cur_region, INTENT_IN | INTENT_OUT, B, "B");
            dvmh_region_execute_on_targets_C(cur_region, DEVICE_TYPE_HOST | DEVICE_TYPE_CUDA);

        /* Parallel loop with base array A */
        /* calculating maximum in variable eps */
        dvmh_line_C(49, "jac1d.c");
            cur_loop = dvmh_loop_create_C(cur_region, 1, DVM0C1, DVM0C((8 - 1) - 1), DVM0C1);
            dvmh_loop_map_C(cur_loop, A, 1, dvmh_alignment_linear_C(1, 1, 0));
            dvmh_loop_reduction_C(cur_loop, rf_MAX, &eps, rt_DOUBLE, 1, 0, 0);
            dvmh_loop_set_cuda_block_C(cur_loop, 256, 1, 1);
            dvmh_loop_register_handler_C(cur_loop, DEVICE_TYPE_HOST, OMP_H_TYPE, dvmh_handler_func_C((DvmHandlerFunc)loop_jac1d_50_host, 2, A, B));
            dvmh_loop_register_handler_C(cur_loop, DEVICE_TYPE_CUDA, 0, dvmh_handler_func_C((DvmHandlerFunc)loop_jac1d_50_cuda, 2, A, B));

            dvmh_line_C(50, "jac1d.c");
            dvmh_loop_perform_C(cur_loop);
            cur_loop = 0;


        /* Parallel loop with base array B and */
        /* with prior updating shadow elements of array A */
        dvmh_line_C(59, "jac1d.c");
            dvmh_shadow_renew_C(A, 0, 0);
            cur_loop = dvmh_loop_create_C(cur_region, 1, DVM0C1, DVM0C((8 - 1) - 1), DVM0C1);
            dvmh_loop_map_C(cur_loop, B, 1, dvmh_alignment_linear_C(1, 1, 0));
            dvmh_loop_set_cuda_block_C(cur_loop, 256, 1, 1);
            dvmh_loop_register_handler_C(cur_loop, DEVICE_TYPE_HOST, OMP_H_TYPE, dvmh_handler_func_C((DvmHandlerFunc)loop_jac1d_60_host, 2, A, B));
            dvmh_loop_register_handler_C(cur_loop, DEVICE_TYPE_CUDA, 0, dvmh_handler_func_C((DvmHandlerFunc)loop_jac1d_60_cuda, 2, A, B));

            dvmh_line_C(60, "jac1d.c");
            dvmh_loop_perform_C(cur_loop);
            cur_loop = 0;
;
        
            dvmh_line_C(62, "jac1d.c");
            dvmh_region_end_C(cur_region);
        }

        dvmh_line_C(64, "jac1d.c");
        dvmh_get_actual_variable2_((void *)&eps);

        dvmh_void_printf("it=%4i   eps=%e\n", it, eps);
        if (eps < MAXEPS)
            break;
    }

    f = dvmh_fopen("jacobi.dat", "wb");
    dvmh_line_C(71, "jac1d.c");
    dvmh_get_actual_array2_(B);

    dvmh_void_fwrite_distrib(B, sizeof(double), L, f);
    dvmh_fclose(f);

    dvmh_line_C(75, "jac1d.c");
    dvmh_exit_C( 0);
    dvmh_exit_C(0);
}


void initCdvmhGlobals_jac1d_410751730() {
    dvmh_line_C(18, "jac1d.c");
    dvmh_array_create_C(A, 1, -rt_DOUBLE, DVM0C(8), DVM0C1, DVM0C1);
    dvmh_distribute_C(A, 1, dvmh_distribution_block_C(1));

    dvmh_line_C(20, "jac1d.c");
    dvmh_array_create_C(B, 1, -rt_DOUBLE, DVM0C(8), DVM0C1, DVM0C1);
    dvmh_align_C(B, A, 1, dvmh_alignment_linear_C(1, 1, 0));

    dvmh_data_enter_C((const void *)&i, sizeof(i));
    dvmh_data_enter_C((const void *)&it, sizeof(it));
    dvmh_data_enter_C((const void *)&eps, sizeof(eps));
    dvmh_data_enter_C((const void *)&MAXEPS, sizeof(MAXEPS));
}

void finishCdvmhGlobals_jac1d_1316048831() {
    dvmh_delete_object_(B);
    dvmh_delete_object_(A);
    dvmh_data_exit_C((const void *)&MAXEPS, 0);
    dvmh_data_exit_C((const void *)&eps, 0);
    dvmh_data_exit_C((const void *)&it, 0);
    dvmh_data_exit_C((const void *)&i, 0);
}


#ifdef _OPENMP
#include <omp.h>
#endif

void loop_jac1d_29_host(DvmType *pLoopRef, DvmType A_hdr[], DvmType B_hdr[]) {
    /* Loop reference and device number */
    DvmType loop_ref = *pLoopRef;
    DvmType device_num = dvmh_loop_get_device_num_C(loop_ref);
    /* Parameters */
    double (* DVMH_RESTRICT A) = (double (*))dvmh_get_natural_base_C(device_num, A_hdr);
    double (* DVMH_RESTRICT B) = (double (*))dvmh_get_natural_base_C(device_num, B_hdr);
    /* Supplementary variables for loop handling */
    DvmType boundsLow[1], boundsHigh[1], loopSteps[1];
    int slotCount;
    /* User variables - loop index variables and other private variables */
    int i;

    dvmh_loop_fill_bounds_C(loop_ref, boundsLow, boundsHigh, loopSteps);
    slotCount = dvmh_loop_get_slot_count_C(loop_ref);

#ifdef _OPENMP
    #pragma omp parallel num_threads(slotCount), private(i)
#endif
    {
#ifdef _OPENMP
        #pragma omp for schedule(runtime), nowait
#endif
        for (i = boundsLow[0]; i <= boundsHigh[0]; i++)
        {
            A[i] = 0;
            if (i == 0 || i == 8 - 1)
                B[i] = 0;
            else
                B[i] = 2 + i;
        }
    }
}

void loop_jac1d_50_host(DvmType *pLoopRef, DvmType A_hdr[], DvmType B_hdr[]) {
    /* Loop reference and device number */
    DvmType loop_ref = *pLoopRef;
    DvmType device_num = dvmh_loop_get_device_num_C(loop_ref);
    /* Parameters */
    double (* DVMH_RESTRICT A) = (double (*))dvmh_get_natural_base_C(device_num, A_hdr);
    double (* DVMH_RESTRICT B) = (double (*))dvmh_get_natural_base_C(device_num, B_hdr);
    /* Supplementary variables for loop handling */
    DvmType boundsLow[1], boundsHigh[1], loopSteps[1];
    int slotCount;
    /* User variables - loop index variables and other private variables */
    int i;
    double eps;

    dvmh_loop_fill_bounds_C(loop_ref, boundsLow, boundsHigh, loopSteps);
    slotCount = dvmh_loop_get_slot_count_C(loop_ref);

#ifdef _OPENMP
    #pragma omp parallel num_threads(slotCount), private(i), private(eps)
#endif
    {
        dvmh_loop_red_init_C(loop_ref, 1, &eps, 0);
#ifdef _OPENMP
        #pragma omp for schedule(runtime), nowait
#endif
        for (i = boundsLow[0]; i <= boundsHigh[0]; i++)
        {
            double tmp = fabs(B[i] - A[i]);
            eps = ((tmp) > (eps) ? (tmp) : (eps));
            A[i] = B[i];
        }
        dvmh_loop_red_post_C(loop_ref, 1, &eps, 0);
    }

}

void loop_jac1d_60_host(DvmType *pLoopRef, DvmType A_hdr[], DvmType B_hdr[]) {
    /* Loop reference and device number */
    DvmType loop_ref = *pLoopRef;
    DvmType device_num = dvmh_loop_get_device_num_C(loop_ref);
    /* Parameters */
    double (* DVMH_RESTRICT A) = (double (*))dvmh_get_natural_base_C(device_num, A_hdr);
    double (* DVMH_RESTRICT B) = (double (*))dvmh_get_natural_base_C(device_num, B_hdr);
    /* Supplementary variables for loop handling */
    DvmType boundsLow[1], boundsHigh[1], loopSteps[1];
    int slotCount;
    /* User variables - loop index variables and other private variables */
    int i;

    dvmh_loop_fill_bounds_C(loop_ref, boundsLow, boundsHigh, loopSteps);
    slotCount = dvmh_loop_get_slot_count_C(loop_ref);

#ifdef _OPENMP
    #pragma omp parallel num_threads(slotCount), private(i)
#endif
    {
#ifdef _OPENMP
        #pragma omp for schedule(runtime), nowait
#endif
        for (i = boundsLow[0]; i <= boundsHigh[0]; i++)
        {
            B[i] = (A[i - 1] + A[i + 1]) / 2.;
        }
    }
}


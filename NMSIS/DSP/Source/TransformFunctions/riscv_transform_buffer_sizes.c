#include "dsp/transform_functions.h"

/**
  @ingroup groupTransforms
*/

/**
  @defgroup TransformBuffers Transform Buffer Sizes
  @par
        Functions to get the sizes of the buffers in the different transform functions
        according to the datatype and target architecture.

  @par
        The current architecture for the build is defined in `RISCV_MATH_DEFAULT_TARGET_ARCH`

  @note All returned lengths are in real numbers and never in complex. So, for a complex buffer of S samples, the
        returned length will be 2*S.

*/

/**
  @ingroup TransformBuffers
*/

/**
  @defgroup CFFTBuffers Buffer Sizes for CFFT
*/



/**
  @addtogroup CFFTBuffers
  @{
*/

/** 
 * @brief Calculate required length for the temporary buffer
 * @param[in] dt Data type of the input data
 * @param[in] nb_samples Number of samples
 * @return Length in datatype elements (real numbers) for the temporary buffer
 * 
 * @note 0 means not applicable (temporary buffer not needed)
 * @note -1 means error : configuration not supported
 */
int32_t riscv_cfft_tmp_buffer_size(riscv_math_datatype dt,
                              uint32_t nb_samples)
{
    (void)dt;
    /**
     * nb_samples is the CFFT length, also means how many complex numbers are in the input buffer
     * nb_samples * 2U is the real numbers, also the real data length
     * nb_samples * 4U is needed for double buffer.
     */
    return nb_samples * 4U;
}

/** 
 * @brief Calculate required length for the output buffer
 * @param[in] dt Data type of the input data
 * @param[in] nb_samples Number of samples in the input data
 * @return Length in datatype elements (real numbers) for the output buffer
 * 
 * @note 0 means not applicable (temporary buffer not needed)
 * @note -1 means error : configuration not supported
 */
int32_t riscv_cfft_output_buffer_size(riscv_math_datatype dt,
    uint32_t nb_samples)
{
    (void)dt;
    return nb_samples * 2U;
}

/** 
 * @brief Calculate required length for the output buffer
 * @param[in] dt Data type of the input data
 * @param[in] nb_samples Number of samples in the input data
 * @return Length in datatype elements (real numbers) for the output buffer
 * 
 * @note 0 means not applicable (temporary buffer not needed)
 * @note -1 means error : configuration not supported
 */
int32_t riscv_cifft_output_buffer_size(riscv_math_datatype dt,
    uint32_t nb_samples)
{
    (void)dt;
    return nb_samples * 2U;
}

/**
* @} end of CFFTBuffers group
*/

/**
  @ingroup TransformBuffers
*/

/**
  @defgroup RFFTBuffers Buffer Sizes for RFFT
*/


/**
  @addtogroup RFFTBuffers
  @{
*/

/** 
 * @brief Calculate required length for the temporary buffer for both RFFT and RIFFT
 * @param[in] dt Data type of the input data
 * @param[in] nb_samples Number of samples in the input data
 * @return Length in datatype elements (real numbers) for the temporary buffer
 * 
 * @note 0 means not applicable (temporary buffer not needed)
 * @note -1 means error : configuration not supported
 */
int32_t riscv_rfft_tmp_buffer_size(riscv_math_datatype dt,
    uint32_t nb_samples)
{
    (void)dt;
    /**
     * nb_samples is the RFFT length, also means how many real numbers are in the input buffer
     * nb_samples * 2U is the buffer length used for CFFT
     * And there is an extra buffer needed for temp result
     */
    return nb_samples * 3U;
}

/** 
 * @brief Calculate required length for the output buffer
 * @param[in] dt Data type of the input data
 * @param[in] nb_samples Number of samples in the input data
 * @return Length in datatype elements (real numbers) for the output buffer
 * 
 * @note 0 means not applicable (temporary buffer not needed)
 * @note -1 means error : configuration not supported
 */
int32_t riscv_rfft_output_buffer_size(riscv_math_datatype dt,
    uint32_t nb_samples)
{
    switch(dt)
    {
        case RISCV_MATH_F64:
        case RISCV_MATH_F32:
        case RISCV_MATH_F16:
           return(nb_samples);
        break;
        case RISCV_MATH_Q31:
        case RISCV_MATH_Q15:
           return(nb_samples+2);
        default:
           return 0;
    }
}

/** 
 * @brief Calculate required length for the input buffer
 * @param[in] dt Data type of the input data
 * @param[in] nb_samples RFFT length in samples
 * @return Length in datatype elements (real numbers) for the input buffer
 * 
 * @note 0 means not applicable (temporary buffer not needed)
 * @note -1 means error : configuration not supported
 */
int32_t riscv_rifft_input_buffer_size(riscv_math_datatype dt,
    uint32_t nb_samples)
{
    switch(dt)
    {
        case RISCV_MATH_F64:
        case RISCV_MATH_F32:
        case RISCV_MATH_F16:
           return(nb_samples);
        break;
        case RISCV_MATH_Q31:
        case RISCV_MATH_Q15:
            return(nb_samples+2);
        break;
        default:
           return 0;
    }
}

/**
* @} end of RFFTBuffers group
*/

/**
  @ingroup TransformBuffers
*/

/**
  @defgroup MFCCBuffers Buffer Sizes for MFCC
*/


/**
  @addtogroup MFCCBuffers
  @{
*/

/** 
 * @brief Calculate required length for the temporary buffer
 * @param[in] dt Data type of the input data
 * @param[in] nb_samples Number of samples in the input data
 * @param[in] buf_id Identification for the temporary buffer
 * @param[in] use_cfft 1 if implementastion uses CFFT, 0 if RFFT
 * @return Length in datatype elements (real numbers) for the temporary buffer
 * 
 * @note 0 means not applicable (temporary buffer not needed)
 * @note -1 means error : configuration not supported
 * @note The define `RISCV_MFCC_USE_CFFT` can be checked to know the current MFCC mode (CFFT or RFFT) for the build
 */
int32_t riscv_mfcc_tmp_buffer_size(
    riscv_math_datatype dt,
    uint32_t nb_samples,
    uint32_t buf_id,
    uint32_t use_cfft)
{
    if (buf_id != 1)
    {
        return 0;
    }


    switch(buf_id)
    {
           case 1:
              if (use_cfft == 1)
              { 
                return(riscv_cfft_output_buffer_size(dt, nb_samples));
              }
              else
              {
                return(riscv_rfft_output_buffer_size(dt, nb_samples));
              }
           case 2:
               return(riscv_rfft_tmp_buffer_size(dt, nb_samples));
           default:
               return(0);
    }


}

/**
* @} end of MFCCBuffers group
*/

#include "ref.h"

void ref_mat_vec_mult_f32(const riscv_matrix_instance_f32 *pSrcMat,
		const float32_t *pVec, float32_t *pDst) {
    uint32_t numRows = pSrcMat->numRows;
    uint32_t numCols = pSrcMat->numCols;
    const float32_t *pSrcA = pSrcMat->pData;
    const float32_t *pInA; /* input data matrix pointer A of f32 type */ 
    const float32_t *pInVec; /* input data matrix pointer B of f32 type */
    float32_t *px; /* Temporary output data matrix pointer */
    uint16_t i, row, col; /* loop counters */
    float32_t matData, vecData;
    float32_t sum;
    i = 0u;
    px = pDst;

    for (row = 0; row < numRows; row++) {
      col = numCols;
      float32_t sum = 0.0f;
      pInVec = pVec;
      pInA = pSrcA + row * numCols;

      while (col > 0u) {
      vecData = *(pInVec)++;
      matData = *(pInA)++;
      sum += (float32_t) matData * vecData;
      col--;
      }

      *px++ = sum;
    }
}

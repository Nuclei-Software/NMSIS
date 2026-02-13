#include "riscv_math_types.h"

#if defined(RISCV_MATH_VECTOR)
#include "TransformRVVCQ15.h"
#include <stdio.h>
#include "Error.h"
#include "Test.h"           

#define SNR_THRESHOLD 20
#define ABS_ERROR ((q15_t)10)


    void TransformRVVCQ15::test_cfft_q15()
    {
       const q15_t *inp = input.ptr();

       q15_t *infftp = inputfft.ptr();

       q15_t *outfftp = outputfft.ptr();

        memcpy(infftp,inp,sizeof(q15_t)*input.nbSamples());

   
        q15_t *bufferp = bufferfft.ptr();
        riscv_cfft_q15(
             this->varInstCfftQ15Ptr,
             infftp,
             outfftp,
             bufferp,
             this->ifft);

          
        ASSERT_SNR(outputfft,ref,(float32_t)SNR_THRESHOLD);
        ASSERT_NEAR_EQ(outputfft,ref,ABS_ERROR);
        ASSERT_EMPTY_TAIL(outputfft);


        
    }

        void TransformRVVCQ15::test_cifft_q15()
    {
       const q15_t *inp = input.ptr();

       q15_t *outfftp = outputfft.ptr();
       q15_t *refp = ref.ptr();

        memcpy(outfftp,inp,sizeof(q15_t)*input.nbSamples());
   
        q15_t *tmp2p = tmp2.ptr();
        riscv_cfft_q15(
             this->varInstCfftQ15Ptr,
             inp,
             outfftp,
             tmp2p,
             this->ifft);


        for(unsigned long i=0; i < outputfft.nbSamples();i++)
        {
          refp[i] = (q15_t)(1.0*refp[i]/this->scaling);
        }
          
        ASSERT_SNR(outputfft,ref,(float32_t)SNR_THRESHOLD);
        ASSERT_NEAR_EQ(outputfft,ref,ABS_ERROR);
        ASSERT_EMPTY_TAIL(outputfft);

       
        
    } 



   void TransformRVVCQ15::setUp(Testing::testID_t id,std::vector<Testing::param_t>& paramsArgs,Client::PatternMgr *mgr)
   {

       (void)paramsArgs;
       switch(id)
       {


         case TransformRVVCQ15::TEST_CFFT_Q15_1:

            input.reload(TransformRVVCQ15::INPUTS_CFFT_NOISY_32_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::REF_CFFT_NOISY_32_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(32);

            this->ifft=0;

          break;


         case TransformRVVCQ15::TEST_CFFT_Q15_2:

            input.reload(TransformRVVCQ15::INPUTS_CFFT_NOISY_64_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::REF_CFFT_NOISY_64_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(64);

            this->ifft=0;

          break;


         case TransformRVVCQ15::TEST_CFFT_Q15_3:

            input.reload(TransformRVVCQ15::INPUTS_CFFT_NOISY_128_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::REF_CFFT_NOISY_128_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(128);

            this->ifft=0;

          break;


         case TransformRVVCQ15::TEST_CFFT_Q15_4:

            input.reload(TransformRVVCQ15::INPUTS_CFFT_NOISY_256_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::REF_CFFT_NOISY_256_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(256);

            this->ifft=0;

          break;


         case TransformRVVCQ15::TEST_CFFT_Q15_5:

            input.reload(TransformRVVCQ15::INPUTS_CFFT_NOISY_512_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::REF_CFFT_NOISY_512_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(512);

            this->ifft=0;

          break;


         case TransformRVVCQ15::TEST_CFFT_Q15_6:

            input.reload(TransformRVVCQ15::INPUTS_CFFT_NOISY_1024_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::REF_CFFT_NOISY_1024_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(1024);

            this->ifft=0;

          break;


         case TransformRVVCQ15::TEST_CFFT_Q15_7:

            input.reload(TransformRVVCQ15::INPUTS_CFFT_NOISY_2048_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::REF_CFFT_NOISY_2048_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(2048);

            this->ifft=0;

          break;


         case TransformRVVCQ15::TEST_CFFT_Q15_8:

            input.reload(TransformRVVCQ15::INPUTS_CFFT_NOISY_4096_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::REF_CFFT_NOISY_4096_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(4096);

            this->ifft=0;

          break;


         case TransformRVVCQ15::TEST_CFFT_Q15_9:

            input.reload(TransformRVVCQ15::INPUTS_CFFT_NOISY_8192_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::REF_CFFT_NOISY_8192_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(8192);

            this->ifft=0;

          break;


         case TransformRVVCQ15::TEST_CFFT_Q15_10:

            input.reload(TransformRVVCQ15::INPUTS_CFFT_STEP_32_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::REF_CFFT_STEP_32_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(32);

            this->ifft=0;

          break;


         case TransformRVVCQ15::TEST_CFFT_Q15_11:

            input.reload(TransformRVVCQ15::INPUTS_CFFT_STEP_64_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::REF_CFFT_STEP_64_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(64);

            this->ifft=0;

          break;


         case TransformRVVCQ15::TEST_CFFT_Q15_12:

            input.reload(TransformRVVCQ15::INPUTS_CFFT_STEP_128_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::REF_CFFT_STEP_128_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(128);

            this->ifft=0;

          break;


         case TransformRVVCQ15::TEST_CFFT_Q15_13:

            input.reload(TransformRVVCQ15::INPUTS_CFFT_STEP_256_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::REF_CFFT_STEP_256_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(256);

            this->ifft=0;

          break;


         case TransformRVVCQ15::TEST_CFFT_Q15_14:

            input.reload(TransformRVVCQ15::INPUTS_CFFT_STEP_512_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::REF_CFFT_STEP_512_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(512);

            this->ifft=0;

          break;


         case TransformRVVCQ15::TEST_CFFT_Q15_15:

            input.reload(TransformRVVCQ15::INPUTS_CFFT_STEP_1024_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::REF_CFFT_STEP_1024_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(1024);

            this->ifft=0;

          break;


         case TransformRVVCQ15::TEST_CFFT_Q15_16:

            input.reload(TransformRVVCQ15::INPUTS_CFFT_STEP_2048_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::REF_CFFT_STEP_2048_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(2048);

            this->ifft=0;

          break;


         case TransformRVVCQ15::TEST_CFFT_Q15_17:

            input.reload(TransformRVVCQ15::INPUTS_CFFT_STEP_4096_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::REF_CFFT_STEP_4096_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(4096);

            this->ifft=0;

          break;


         case TransformRVVCQ15::TEST_CFFT_Q15_18:

            input.reload(TransformRVVCQ15::INPUTS_CFFT_STEP_8192_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::REF_CFFT_STEP_8192_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(8192);

            this->ifft=0;

          break;


         case TransformRVVCQ15::TEST_CIFFT_Q15_19:

            input.reload(TransformRVVCQ15::INPUTS_CIFFT_NOISY_32_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::INPUTS_CFFT_NOISY_32_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(32);

            this->ifft=1;
            this->scaling = 32;

          break;


         case TransformRVVCQ15::TEST_CIFFT_Q15_20:

            input.reload(TransformRVVCQ15::INPUTS_CIFFT_NOISY_64_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::INPUTS_CFFT_NOISY_64_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(64);

            this->ifft=1;
            this->scaling = 64;

          break;


         case TransformRVVCQ15::TEST_CIFFT_Q15_21:

            input.reload(TransformRVVCQ15::INPUTS_CIFFT_NOISY_128_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::INPUTS_CFFT_NOISY_128_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(128);

            this->ifft=1;
            this->scaling = 128;

          break;


         case TransformRVVCQ15::TEST_CIFFT_Q15_22:

            input.reload(TransformRVVCQ15::INPUTS_CIFFT_NOISY_256_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::INPUTS_CFFT_NOISY_256_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(256);

            this->ifft=1;
            this->scaling = 256;

          break;


         case TransformRVVCQ15::TEST_CIFFT_Q15_23:

            input.reload(TransformRVVCQ15::INPUTS_CIFFT_NOISY_512_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::INPUTS_CFFT_NOISY_512_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(512);

            this->ifft=1;
            this->scaling = 512;

          break;


         case TransformRVVCQ15::TEST_CIFFT_Q15_24:

            input.reload(TransformRVVCQ15::INPUTS_CIFFT_NOISY_1024_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::INPUTS_CFFT_NOISY_1024_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(1024);

            this->ifft=1;
            this->scaling = 1024;

          break;


         case TransformRVVCQ15::TEST_CIFFT_Q15_25:

            input.reload(TransformRVVCQ15::INPUTS_CIFFT_NOISY_2048_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::INPUTS_CFFT_NOISY_2048_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(2048);

            this->ifft=1;
            this->scaling = 2048;

          break;


         case TransformRVVCQ15::TEST_CIFFT_Q15_26:

            input.reload(TransformRVVCQ15::INPUTS_CIFFT_NOISY_4096_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::INPUTS_CFFT_NOISY_4096_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(4096);

            this->ifft=1;
            this->scaling = 4096;

          break;


         case TransformRVVCQ15::TEST_CIFFT_Q15_27:

            input.reload(TransformRVVCQ15::INPUTS_CIFFT_NOISY_8192_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::INPUTS_CFFT_NOISY_8192_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(8192);

            this->ifft=1;
            this->scaling = 8192;

          break;


         case TransformRVVCQ15::TEST_CIFFT_Q15_28:

            input.reload(TransformRVVCQ15::INPUTS_CIFFT_STEP_32_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::INPUTS_CFFT_STEP_32_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(32);

            this->ifft=1;
            this->scaling = 32;

          break;


         case TransformRVVCQ15::TEST_CIFFT_Q15_29:

            input.reload(TransformRVVCQ15::INPUTS_CIFFT_STEP_64_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::INPUTS_CFFT_STEP_64_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(64);

            this->ifft=1;
            this->scaling = 64;

          break;


         case TransformRVVCQ15::TEST_CIFFT_Q15_30:

            input.reload(TransformRVVCQ15::INPUTS_CIFFT_STEP_128_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::INPUTS_CFFT_STEP_128_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(128);

            this->ifft=1;
            this->scaling = 128;

          break;


         case TransformRVVCQ15::TEST_CIFFT_Q15_31:

            input.reload(TransformRVVCQ15::INPUTS_CIFFT_STEP_256_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::INPUTS_CFFT_STEP_256_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(256);

            this->ifft=1;
            this->scaling = 256;

          break;


         case TransformRVVCQ15::TEST_CIFFT_Q15_32:

            input.reload(TransformRVVCQ15::INPUTS_CIFFT_STEP_512_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::INPUTS_CFFT_STEP_512_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(512);

            this->ifft=1;
            this->scaling = 512;

          break;


         case TransformRVVCQ15::TEST_CIFFT_Q15_33:

            input.reload(TransformRVVCQ15::INPUTS_CIFFT_STEP_1024_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::INPUTS_CFFT_STEP_1024_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(1024);

            this->ifft=1;
            this->scaling = 1024;

          break;


         case TransformRVVCQ15::TEST_CIFFT_Q15_34:

            input.reload(TransformRVVCQ15::INPUTS_CIFFT_STEP_2048_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::INPUTS_CFFT_STEP_2048_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(2048);

            this->ifft=1;
            this->scaling = 2048;

          break;


         case TransformRVVCQ15::TEST_CIFFT_Q15_35:

            input.reload(TransformRVVCQ15::INPUTS_CIFFT_STEP_4096_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::INPUTS_CFFT_STEP_4096_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(4096);

            this->ifft=1;
            this->scaling = 4096;

          break;


         case TransformRVVCQ15::TEST_CIFFT_Q15_36:

            input.reload(TransformRVVCQ15::INPUTS_CIFFT_STEP_8192_Q15_ID,mgr);
            ref.reload(  TransformRVVCQ15::INPUTS_CFFT_STEP_8192_Q15_ID,mgr);

            this->varInstCfftQ15Ptr=riscv_cfft_init_dynamic_q15(8192);

            this->ifft=1;
            this->scaling = 8192;

          break;


       }
       inputfft.create(ref.nbSamples(),TransformRVVCQ15::OUTPUT_CFFT_Q15_ID,mgr);

       outputfft.create(ref.nbSamples(),TransformRVVCQ15::OUTPUT_CFFT_Q15_ID,mgr);
       bufferfft.create(ref.nbSamples(),TransformRVVCQ15::OUTPUT_CFFT_Q15_ID,mgr);
       tmp2.create(ref.nbSamples(),TransformRVVCQ15::OUTPUT_CFFT_Q15_ID,mgr);

    }

    void TransformRVVCQ15::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
        (void)id;
        outputfft.dump(mgr);
        if (varInstCfftQ15Ptr)
        {
            free(varInstCfftQ15Ptr);
        }
    }

#endif

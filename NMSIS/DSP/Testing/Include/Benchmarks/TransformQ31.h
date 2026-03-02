#include "Test.h"
#include "Pattern.h"

#include "dsp/transform_functions.h"

class TransformQ31:public Client::Suite
    {
        public:
            TransformQ31(Testing::testID_t id);
            virtual void setUp(Testing::testID_t,std::vector<Testing::param_t>& params,Client::PatternMgr *mgr);
            virtual void tearDown(Testing::testID_t,Client::PatternMgr *mgr);
        private:
            #include "TransformQ31_decl.h"
            Client::Pattern<q31_t> samples;

            Client::LocalPattern<q31_t> output;
            Client::LocalPattern<q31_t> state;

            int nbSamples;
            int ifft;
            int bitRev;

            q31_t *pSrc;
            q31_t *pDst;
            q31_t *pState;

            riscv_cfft_instance_q31 cfftInstance;

            riscv_dct4_instance_q31 dct4Instance;
            riscv_rfft_instance_q31 rfftInstance;
            riscv_cfft_radix4_instance_q31 cfftRadix4Instance;
            riscv_cfft_radix2_instance_q31 cfftRadix2Instance;

    };

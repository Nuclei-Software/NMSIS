#include "Test.h"
#include "Pattern.h"

#include "dsp/transform_functions_f16.h"

class TransformRVVCF16:public Client::Suite
    {
        public:
            TransformRVVCF16(Testing::testID_t id);
            virtual void setUp(Testing::testID_t,std::vector<Testing::param_t>& paramsArgs,Client::PatternMgr *mgr);
            virtual void tearDown(Testing::testID_t,Client::PatternMgr *mgr);
        private:
            #include "TransformRVVCF16_decl.h"
            
            Client::Pattern<float16_t> input;
            Client::LocalPattern<float16_t> outputfft;
            Client::LocalPattern<float16_t> bufferfft;

            Client::LocalPattern<float16_t> inputfft;

            Client::RefPattern<float16_t> ref;

            riscv_cfft_instance_f16 *varInstCfftF16Ptr;

            int ifft;

            riscv_status status;
            
    };
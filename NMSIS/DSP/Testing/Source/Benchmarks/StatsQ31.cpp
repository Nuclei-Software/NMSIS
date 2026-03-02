#include "StatsQ31.h"
#include <stdio.h>
#include "Error.h"
#include "Test.h"



    void StatsQ31::test_max_q31()
    {

        q31_t result;
        uint32_t  indexval;

        riscv_max_q31(inap,
              this->nb,
              &result,
              &indexval);

    }

    void StatsQ31::test_absmax_q31()
    {

        q31_t result;
        uint32_t  indexval;

        riscv_absmax_q31(inap,
              this->nb,
              &result,
              &indexval);

    }



    void StatsQ31::test_min_q31()
    {

        q31_t result;
        uint32_t  indexval;


        riscv_min_q31(inap,
              this->nb,
              &result,
              &indexval);



    }

    void StatsQ31::test_absmin_q31()
    {

        q31_t result;
        uint32_t  indexval;


        riscv_absmin_q31(inap,
              this->nb,
              &result,
              &indexval);



    }

    void StatsQ31::test_mean_q31()
    {
        q31_t result;

        riscv_mean_q31(inap,
              this->nb,
              &result);

    }

    void StatsQ31::test_power_q31()
    {

        q63_t result;


        riscv_power_q31(inap,
              this->nb,
              &result);



    }

    void StatsQ31::test_rms_q31()
    {

        q31_t result;


        riscv_rms_q31(inap,
              this->nb,
              &result);


    }

    void StatsQ31::test_std_q31()
    {

        q31_t result;


        riscv_std_q31(inap,
              this->nb,
              &result);


    }

    void StatsQ31::test_var_q31()
    {

        q31_t result;


        riscv_var_q31(inap,
              this->nb,
              &result);


    }




    void StatsQ31::setUp(Testing::testID_t id,std::vector<Testing::param_t>& paramsArgs,Client::PatternMgr *mgr)
    {
        std::vector<Testing::param_t>::iterator it = paramsArgs.begin();
        this->nb = *it;

        inputA.reload(StatsQ31::INPUT1_Q31_ID,mgr,this->nb);

        inap=inputA.ptr();



    }

    void StatsQ31::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
      (void)id;

    }

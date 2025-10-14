#include "StatsQ15.h"
#include <stdio.h>
#include "Error.h"
#include "Test.h"



    void StatsQ15::test_max_q15()
    {

        q15_t result;
        uint32_t  indexval;

        riscv_max_q15(inap,
              this->nb,
              &result,
              &indexval);

    }

    void StatsQ15::test_absmax_q15()
    {

        q15_t result;
        uint32_t  indexval;

        riscv_absmax_q15(inap,
              this->nb,
              &result,
              &indexval);

    }

    void StatsQ15::test_min_q15()
    {
       
        q15_t result;
        uint32_t  indexval;

       
        riscv_min_q15(inap,
              this->nb,
              &result,
              &indexval);

      

    }

    void StatsQ15::test_absmin_q15()
    {
       
        q15_t result;
        uint32_t  indexval;

       
        riscv_absmin_q15(inap,
              this->nb,
              &result,
              &indexval);

      

    }

    void StatsQ15::test_mean_q15()
    {

        q15_t result;

        riscv_mean_q15(inap,
              this->nb,
              &result);

    }

    void StatsQ15::test_power_q15()
    {
        
        q63_t result;

        
        riscv_power_q15(inap,
              this->nb,
              &result);

       

    }

    void StatsQ15::test_rms_q15()
    {
       
        q15_t result;

       
        riscv_rms_q15(inap,
              this->nb,
              &result);

       
    }

    void StatsQ15::test_std_q15()
    {

        q15_t result;

       
        riscv_std_q15(inap,
              this->nb,
              &result);

       
    }

    void StatsQ15::test_var_q15()
    {

        q15_t result;

       
        riscv_var_q15(inap,
              this->nb,
              &result);

      
    }


   


    void StatsQ15::setUp(Testing::testID_t id,std::vector<Testing::param_t>& paramsArgs,Client::PatternMgr *mgr)
    {
        std::vector<Testing::param_t>::iterator it = paramsArgs.begin();
        this->nb = *it;

        inputA.reload(StatsQ15::INPUT1_Q15_ID,mgr,this->nb);

        inap=inputA.ptr();

        
    }

    void StatsQ15::tearDown(Testing::testID_t id,Client::PatternMgr *mgr)
    {
      (void)id;
     
    }

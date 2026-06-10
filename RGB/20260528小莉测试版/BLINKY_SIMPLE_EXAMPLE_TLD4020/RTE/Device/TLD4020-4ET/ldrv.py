#<checksum>a8281575f6089418bd2d64f9e3e69b4b3a059d6c8220677858d546a8a10a5772</checksum>
#                                                                                
# Copyright (c) 2025, Infineon Technologies AG
# All rights reserved.                                                           
#                                                                                
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:                  
#                                                                                
#  Redistributions of source code must retain the above copyright notice, this   
#  list of conditions and the following disclaimer.                              
#                                                                                
#  Redistributions in binary form must reproduce the above copyright notice,     
#  this list of conditions and the following disclaimer in the documentation     
#  and/or other materials provided with the distribution.                        
#                                                                                
#  Neither the name of the copyright holders nor the names of its contributors   
#  may be used to endorse or promote products derived from this software without 
#  specific prior written permission.                                            
#                                                                                
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
# ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED  
# WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE         
# DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE   
# FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL     
# DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR     
# SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER     
# CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,  
# OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE  
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.           
#
#
# \file     ldrv.py
#
# \brief    Script to process data in icw_cmt_ldrv.xml
#
# \version  V1.2.0
# \date     10. Oct 2025
#

###############################################################################
#                              Author(s) Identity                             #
#*****************************************************************************#
#  Initials     Name                                                          #
#  -------------------------------------------------------------------------- #
#  FP           Federico Pacini                                               #
#  SL           Stefano Lissandron                                            #
#  JO           Julia Ott                                                     #
#  VO           Vanessa Ongaro                                                #
###############################################################################

###############################################################################
#                           Revision Control History                          #
#*****************************************************************************#
#  V1.0.0: 2022-10-21, FP:   Initial version                                  #
#  V1.0.1: 2023-09-28, JO:   [ATVCOMETLL-131] Updated date format             #
#  V1.0.2: 2025-08-18, VO:   [ATVCOMETLL-221] Enable consistency timer ch3    #
#  V1.1.0: 2025-09-11, VO:   [ATVCOMETLL-99] Refactor counter value calcs     #
#  V1.2.0: 2025-10-10, VO:   [ATVCOMETLL-217] Add support for TLD4020_12ES    #
###############################################################################

class Calc8BitCount:
    def __init__(self, name, input_val, input_unit):
        # input
        self.name = name
        self.input_val = int(input_val)
        self.input_unit = int(input_unit)
        self.max_count = int(LDRV_MAX_COUNT.value)

        # output
        self.count = 0
        self.regval = 0
        self.ticks = 0

        if self.input_unit == 0:  # register value given directly
            self.count = self.input_val * 256
        if self.input_unit == 1:  # percentage[%] of maximum counter value
            self.count = (self.input_val * self.max_count) / 100

        self.regval = round(self.count / 256)
        self.ticks = self.regval * 256

    def save(self):
        exec("%s = %s" % ("LDRV_PWM_" + self.name.upper() + "_REGVAL.value", self.regval))
        exec("%s = %s" % ("LDRV_PWM_" + self.name.upper() + "_TICKS.value", self.ticks))

class Calc16BitCount:
    def __init__(self, name, input_val, input_unit):
        # input
        self.name = name
        self.input_val = int(input_val)
        self.input_unit = int(input_unit)
        self.max_count = int(LDRV_MAX_COUNT.value)

        # output
        self.count = 0
        self.regval = 0
        self.ticks = 0

        if self.input_unit == 0:  # register value given directly
            self.count = self.input_val
        if self.input_unit == 1:  # percentage[%] of maximum counter value
            self.count = (self.input_val * self.max_count) / 100

        self.regval = round(self.count)

    def save(self):
        exec("%s = %s" % ("LDRV_PWM_" + self.name.upper() + "_REGVAL.value", self.regval))


class LDRVClass:
    def __init__(self):
        # The PWM_PHASE is considered as the 8 MSBs of a 16-bit register while the 8 LSBs are all zeros.
        # This means that the phase shift can be set with a resolution of 256 with respect to the
        # PWM counter.
        self.phase0 = Calc8BitCount("phase_cfg0", LDRV_CH0_PHASESHIFT.value, LDRV_CH0_PHASESHIFT_UNIT.currentData)
        self.phase1 = Calc8BitCount("phase_cfg1", LDRV_CH1_PHASESHIFT.value, LDRV_CH1_PHASESHIFT_UNIT.currentData)
        self.phase2 = Calc8BitCount("phase_cfg2", LDRV_CH2_PHASESHIFT.value, LDRV_CH2_PHASESHIFT_UNIT.currentData)
        if(UC_FEATURE_4CHANNEL.text == "1"):
            self.phase3 = Calc8BitCount("phase_cfg3", LDRV_CH3_PHASESHIFT.value, LDRV_CH3_PHASESHIFT_UNIT.currentData)
        if(UC_FEATURE_12CHANNEL.text == "1"):
            self.phase4 = Calc8BitCount("phase_cfg4", LDRV_CH4_PHASESHIFT.value, LDRV_CH4_PHASESHIFT_UNIT.currentData)
            self.phase5 = Calc8BitCount("phase_cfg5", LDRV_CH5_PHASESHIFT.value, LDRV_CH5_PHASESHIFT_UNIT.currentData)
            self.phase6 = Calc8BitCount("phase_cfg6", LDRV_CH6_PHASESHIFT.value, LDRV_CH6_PHASESHIFT_UNIT.currentData)
            self.phase7 = Calc8BitCount("phase_cfg7", LDRV_CH7_PHASESHIFT.value, LDRV_CH7_PHASESHIFT_UNIT.currentData)
            self.phase8 = Calc8BitCount("phase_cfg8", LDRV_CH8_PHASESHIFT.value, LDRV_CH8_PHASESHIFT_UNIT.currentData)
            self.phase9 = Calc8BitCount("phase_cfg9", LDRV_CH9_PHASESHIFT.value, LDRV_CH9_PHASESHIFT_UNIT.currentData)
            self.phase10 = Calc8BitCount("phase_cfg10", LDRV_CH10_PHASESHIFT.value, LDRV_CH10_PHASESHIFT_UNIT.currentData)
            self.phase11 = Calc8BitCount("phase_cfg11", LDRV_CH11_PHASESHIFT.value, LDRV_CH11_PHASESHIFT_UNIT.currentData)
        
        # PWM_DC and PWM_DIAG_PHASE are 16-bit bitfields which directly represent counter values or ticks respectively.
        self.dc0 = Calc16BitCount("dc_cfg0", LDRV_CH0_DC.value, LDRV_CH0_DC_UNIT.currentData)
        self.dc1 = Calc16BitCount("dc_cfg1", LDRV_CH1_DC.value, LDRV_CH1_DC_UNIT.currentData)
        self.dc2 = Calc16BitCount("dc_cfg2", LDRV_CH2_DC.value, LDRV_CH2_DC_UNIT.currentData)
        if(UC_FEATURE_4CHANNEL.text == "1"):
            self.dc3 = Calc16BitCount("dc_cfg3", LDRV_CH3_DC.value, LDRV_CH3_DC_UNIT.currentData)
        if(UC_FEATURE_12CHANNEL.text == "1"):
            self.dc4 = Calc16BitCount("dc_cfg4", LDRV_CH4_DC.value, LDRV_CH4_DC_UNIT.currentData)
            self.dc5 = Calc16BitCount("dc_cfg5", LDRV_CH5_DC.value, LDRV_CH5_DC_UNIT.currentData)
            self.dc6 = Calc16BitCount("dc_cfg6", LDRV_CH6_DC.value, LDRV_CH6_DC_UNIT.currentData)
            self.dc7 = Calc16BitCount("dc_cfg7", LDRV_CH7_DC.value, LDRV_CH7_DC_UNIT.currentData)
            self.dc8 = Calc16BitCount("dc_cfg8", LDRV_CH8_DC.value, LDRV_CH8_DC_UNIT.currentData)
            self.dc9 = Calc16BitCount("dc_cfg9", LDRV_CH9_DC.value, LDRV_CH9_DC_UNIT.currentData)
            self.dc10 = Calc16BitCount("dc_cfg10", LDRV_CH10_DC.value, LDRV_CH10_DC_UNIT.currentData)
            self.dc11 = Calc16BitCount("dc_cfg11", LDRV_CH11_DC.value, LDRV_CH11_DC_UNIT.currentData)
        self.dcsynch = Calc16BitCount("dc_synch", LDRV_SYNCH_DC.value, LDRV_SYNCH_DC_UNIT.currentData)

        self.diag_phase0 = Calc16BitCount("diag_phase_cfg0", LDRV_CH0_DIAG_PHASE.value, LDRV_CH0_DIAG_PHASE_UNIT.currentData)
        self.diag_phase1 = Calc16BitCount("diag_phase_cfg1", LDRV_CH1_DIAG_PHASE.value, LDRV_CH1_DIAG_PHASE_UNIT.currentData)
        self.diag_phase2 = Calc16BitCount("diag_phase_cfg2", LDRV_CH2_DIAG_PHASE.value, LDRV_CH2_DIAG_PHASE_UNIT.currentData)
        if(UC_FEATURE_4CHANNEL.text == "1"):
            self.diag_phase3 = Calc16BitCount("diag_phase_cfg3", LDRV_CH3_DIAG_PHASE.value, LDRV_CH3_DIAG_PHASE_UNIT.currentData)
        if(UC_FEATURE_12CHANNEL.text == "1"):
            self.diag_phase4 = Calc16BitCount("diag_phase_cfg4", LDRV_CH4_DIAG_PHASE.value, LDRV_CH4_DIAG_PHASE_UNIT.currentData)
            self.diag_phase5 = Calc16BitCount("diag_phase_cfg5", LDRV_CH5_DIAG_PHASE.value, LDRV_CH5_DIAG_PHASE_UNIT.currentData)
            self.diag_phase6 = Calc16BitCount("diag_phase_cfg6", LDRV_CH6_DIAG_PHASE.value, LDRV_CH6_DIAG_PHASE_UNIT.currentData)
            self.diag_phase7 = Calc16BitCount("diag_phase_cfg7", LDRV_CH7_DIAG_PHASE.value, LDRV_CH7_DIAG_PHASE_UNIT.currentData)
            self.diag_phase8 = Calc16BitCount("diag_phase_cfg8", LDRV_CH8_DIAG_PHASE.value, LDRV_CH8_DIAG_PHASE_UNIT.currentData)
            self.diag_phase9 = Calc16BitCount("diag_phase_cfg9", LDRV_CH9_DIAG_PHASE.value, LDRV_CH9_DIAG_PHASE_UNIT.currentData)
            self.diag_phase10 = Calc16BitCount("diag_phase_cfg10", LDRV_CH10_DIAG_PHASE.value, LDRV_CH10_DIAG_PHASE_UNIT.currentData)
            self.diag_phase11 = Calc16BitCount("diag_phase_cfg11", LDRV_CH11_DIAG_PHASE.value, LDRV_CH11_DIAG_PHASE_UNIT.currentData)

    def save(self):
        self.phase0.save()
        self.phase1.save()
        self.phase2.save()
        if(UC_FEATURE_4CHANNEL.text == "1"):
            self.phase3.save()
        if(UC_FEATURE_12CHANNEL.text == "1"):
            self.phase4.save()
            self.phase5.save()
            self.phase6.save()
            self.phase7.save()
            self.phase8.save()
            self.phase9.save()
            self.phase10.save()
            self.phase11.save()
        self.dc0.save()
        self.dc1.save()
        self.dc2.save()
        if(UC_FEATURE_4CHANNEL.text == "1"):
            self.dc3.save()
        if(UC_FEATURE_12CHANNEL.text == "1"):
            self.dc4.save()
            self.dc5.save()
            self.dc6.save()
            self.dc7.save()
            self.dc8.save()
            self.dc9.save()
            self.dc10.save()
            self.dc11.save()
        self.dcsynch.save()
        self.diag_phase0.save()
        self.diag_phase1.save()
        self.diag_phase2.save()
        if(UC_FEATURE_4CHANNEL.text == "1"):
            self.diag_phase3.save()
        if(UC_FEATURE_12CHANNEL.text == "1"):
            self.diag_phase4.save()
            self.diag_phase5.save()
            self.diag_phase6.save()
            self.diag_phase7.save()
            self.diag_phase8.save()
            self.diag_phase9.save()
            self.diag_phase10.save()
            self.diag_phase11.save()


# main
def calc_ldrv():
    ldrv = LDRVClass()
    ldrv.save()

# connect with GUI elements
LDRV_MAX_COUNT.connect('valueChanged(int)', calc_ldrv)
LDRV_CH0_PHASESHIFT.connect('valueChanged(int)', calc_ldrv)
LDRV_CH0_PHASESHIFT_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
LDRV_CH1_PHASESHIFT.connect('valueChanged(int)', calc_ldrv)
LDRV_CH1_PHASESHIFT_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
LDRV_CH2_PHASESHIFT.connect('valueChanged(int)', calc_ldrv)
LDRV_CH2_PHASESHIFT_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
if(UC_FEATURE_4CHANNEL.text == "1"):
    LDRV_CH3_PHASESHIFT.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH3_PHASESHIFT_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
if(UC_FEATURE_12CHANNEL.text == "1"):
    LDRV_CH4_PHASESHIFT.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH4_PHASESHIFT_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
    LDRV_CH5_PHASESHIFT.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH5_PHASESHIFT_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
    LDRV_CH6_PHASESHIFT.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH6_PHASESHIFT_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
    LDRV_CH7_PHASESHIFT.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH7_PHASESHIFT_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
    LDRV_CH8_PHASESHIFT.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH8_PHASESHIFT_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
    LDRV_CH9_PHASESHIFT.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH9_PHASESHIFT_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
    LDRV_CH10_PHASESHIFT.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH10_PHASESHIFT_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
    LDRV_CH11_PHASESHIFT.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH11_PHASESHIFT_UNIT.connect('currentIndexChanged(int)', calc_ldrv)

LDRV_CH0_DC.connect('valueChanged(int)', calc_ldrv)
LDRV_CH0_DC_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
LDRV_CH1_DC.connect('valueChanged(int)', calc_ldrv)
LDRV_CH1_DC_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
LDRV_CH2_DC.connect('valueChanged(int)', calc_ldrv)
LDRV_CH2_DC_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
if(UC_FEATURE_4CHANNEL.text == "1"):
    LDRV_CH3_DC.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH3_DC_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
if(UC_FEATURE_12CHANNEL.text == "1"):
    LDRV_CH4_DC.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH4_DC_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
    LDRV_CH5_DC.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH5_DC_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
    LDRV_CH6_DC.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH6_DC_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
    LDRV_CH7_DC.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH7_DC_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
    LDRV_CH8_DC.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH8_DC_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
    LDRV_CH9_DC.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH9_DC_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
    LDRV_CH10_DC.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH10_DC_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
    LDRV_CH11_DC.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH11_DC_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
LDRV_SYNCH_DC.connect('valueChanged(int)', calc_ldrv)
LDRV_SYNCH_DC_UNIT.connect('currentIndexChanged(int)', calc_ldrv)

LDRV_CH0_DIAG_PHASE.connect('valueChanged(int)', calc_ldrv)
LDRV_CH0_DIAG_PHASE_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
LDRV_CH1_DIAG_PHASE.connect('valueChanged(int)', calc_ldrv)
LDRV_CH1_DIAG_PHASE_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
LDRV_CH2_DIAG_PHASE.connect('valueChanged(int)', calc_ldrv)
LDRV_CH2_DIAG_PHASE_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
if(UC_FEATURE_4CHANNEL.text == "1"):
    LDRV_CH3_DIAG_PHASE.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH3_DIAG_PHASE_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
if(UC_FEATURE_12CHANNEL.text == "1"):
    LDRV_CH4_DIAG_PHASE.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH4_DIAG_PHASE_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
    LDRV_CH5_DIAG_PHASE.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH5_DIAG_PHASE_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
    LDRV_CH6_DIAG_PHASE.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH6_DIAG_PHASE_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
    LDRV_CH7_DIAG_PHASE.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH7_DIAG_PHASE_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
    LDRV_CH8_DIAG_PHASE.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH8_DIAG_PHASE_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
    LDRV_CH9_DIAG_PHASE.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH9_DIAG_PHASE_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
    LDRV_CH10_DIAG_PHASE.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH10_DIAG_PHASE_UNIT.connect('currentIndexChanged(int)', calc_ldrv)
    LDRV_CH11_DIAG_PHASE.connect('valueChanged(int)', calc_ldrv)
    LDRV_CH11_DIAG_PHASE_UNIT.connect('currentIndexChanged(int)', calc_ldrv)


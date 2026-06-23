#<checksum>b964a1525bbe3d032be9d34628ad6566c3eadea7fe2248973f49b7ea358388a3</checksum>
#                                                                                
# Copyright (c) 2022-2023, Infineon Technologies AG
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
# \version  V1.0.1
# \date     28. Sep 2023
#

###############################################################################
#                              Author(s) Identity                             #
#*****************************************************************************#
#  Initials     Name                                                          #
#  -------------------------------------------------------------------------- #
#  FP           Federico Pacini                                               #
#  SL           Stefano Lissandron                                            #
#  JO           Julia Ott                                                     #
###############################################################################

###############################################################################
#                           Revision Control History                          #
#*****************************************************************************#
#  V1.0.0: 2022-10-21, FP:   Initial version                                  #
#  V1.0.1: 2023-09-28, JO:   [ATVCOMETLL-131] Updated date format             #
###############################################################################

import sys
import logging

# for debugging
loglevel = logging.DEBUG #logging.INFO

filename = "ldrv.log"
logging.basicConfig(filename=filename, level=loglevel,
                    format='%(asctime)s.%(msecs)03d [%(levelname)s] - %(threadName)s: %(message)s', datefmt='%Y-%m-%d,%H:%M:%S')

logging.info(f"python version: {sys.version}")
logging.info(f"python version info: {sys.version_info}")

# This function is used to maintain the consistency among absolute values and percentage values
# E.g, have a look on Phase Delay

# Variable used to overcome the issue of double firing
# E,g: in case the user clicks on absolute, changing the percentage value will end up in triggering its function execution
second_trigger = False

def PWM_PHASE_CFGX_PWM_PHASE_consistency_update(element):
    global second_trigger
    if(second_trigger is False):
        logging.info(f"FIRST TRIGGER")

        element_name = element.__dict__['objectName']
        MAX_VALUE = PWM_FREQ_PWM_MAX.value

        logging.info(f"MAX VALUE: {MAX_VALUE}")
        logging.info(f"OBj name: {element_name}")

        if((element_name == "LDRV.PWM_PHASE_CFG0.PWM_PHASE") or (element_name == "LDRV_PWM_PHASE_CFG0_PWM_PHASE_percentage_value_spinbox")):
            element_absolute_spinbox = LDRV_PWM_PHASE_CFG0_PWM_PHASE
            element_percentage_spinbox = LDRV_PWM_PHASE_CFG0_PWM_PHASE_percentage_value_spinbox
        if((element_name == "LDRV.PWM_PHASE_CFG1.PWM_PHASE") or (element_name == "LDRV_PWM_PHASE_CFG1_PWM_PHASE_percentage_value_spinbox")):
            element_absolute_spinbox = LDRV_PWM_PHASE_CFG1_PWM_PHASE
            element_percentage_spinbox = LDRV_PWM_PHASE_CFG1_PWM_PHASE_percentage_value_spinbox
        if((element_name == "LDRV.PWM_PHASE_CFG2.PWM_PHASE") or (element_name == "LDRV_PWM_PHASE_CFG2_PWM_PHASE_percentage_value_spinbox")):
            element_absolute_spinbox = LDRV_PWM_PHASE_CFG2_PWM_PHASE
            element_percentage_spinbox = LDRV_PWM_PHASE_CFG2_PWM_PHASE_percentage_value_spinbox

        
        logging.info(f"absolute_current_value: {element_absolute_spinbox.value}")
        logging.info(f"relative_current_value: {element_percentage_spinbox.value}")
        logging.info(f"element_name: {element_name}")

        if("percentage" not in element_name):
            #The user has pressed one of the spinboxes of "absolute values"
            if(element_absolute_spinbox.value > int(MAX_VALUE/256)):
                #rounding
                element_absolute_spinbox.value = int(MAX_VALUE/256)
                
            if(MAX_VALUE != 0):
                element_percentage_spinbox.value = (element_absolute_spinbox.value * 256) / MAX_VALUE * 100
        else:
            logging.info(f"!!percentage!!!")
            element_absolute_spinbox.value = int((MAX_VALUE * element_percentage_spinbox.value / 100)/256)
        
        second_trigger = True
    else:
        logging.info(f"BOUNCING TRIGGER")
        second_trigger = False
        
        #else
        #update percentage

def wrapper(element):
    logging.info(f"element: {element}")

    def callback(value):
        PWM_PHASE_CFGX_PWM_PHASE_consistency_update(element)
    return callback

    
def PWM_PHASE_CFG0_PWM_PHASE_absolute_value_update(current_value):
    MAX_VALUE = PWM_FREQ_PWM_MAX.value

    logging.info(f"MAX VALUE: {MAX_VALUE}")
    logging.info(f"current_value: {current_value}")

    if(current_value > MAX_VALUE):
        #rounding
        LDRV_PWM_PHASE_CFG0_PWM_PHASE_absolute_value_spinbox.value = int(MAX_VALUE/256) * 256

    if(MAX_VALUE != 0):
        LDRV_PWM_PHASE_CFG0_PWM_PHASE_percentage_value_spinbox.value = LDRV_PWM_PHASE_CFG0_PWM_PHASE_absolute_value_spinbox.value / MAX_VALUE * 100
    
    #application.runScript('setDef(ADC1.SQSLOT' + str(seqTabIdx) + '.CHSEL' + str(idx) + '.enabled=1)')
    #logging.info(f'{PWM_FREQ_PWM_MAX.value}')
    
    #PWM_PHASE_CFG0_PWM_PHASE.text = PWM_PHASE_CFG0_PWM_PHASE_absolute_value_spinbox.value

#LDRV_PWM_PHASE_CFG0_PWM_PHASE_absolute_value_spinbox.connect('valueChanged(int)', PWM_PHASE_CFGX_PWM_PHASE_absolute_value_update("PWM_PHASE_CFG0"))

# LDRV_PWM_PHASE_CFG0_PWM_PHASE.connect('valueChanged(int)', wrapper(LDRV_PWM_PHASE_CFG0_PWM_PHASE))
# LDRV_PWM_PHASE_CFG1_PWM_PHASE.connect('valueChanged(int)', wrapper(LDRV_PWM_PHASE_CFG1_PWM_PHASE))
# LDRV_PWM_PHASE_CFG2_PWM_PHASE.connect('valueChanged(int)', wrapper(LDRV_PWM_PHASE_CFG2_PWM_PHASE))
# LDRV_PWM_PHASE_CFG0_PWM_PHASE_percentage_value_spinbox.connect('valueChanged(int)', wrapper(LDRV_PWM_PHASE_CFG0_PWM_PHASE_percentage_value_spinbox))
# LDRV_PWM_PHASE_CFG1_PWM_PHASE_percentage_value_spinbox.connect('valueChanged(int)', wrapper(LDRV_PWM_PHASE_CFG1_PWM_PHASE_percentage_value_spinbox))
# LDRV_PWM_PHASE_CFG2_PWM_PHASE_percentage_value_spinbox.connect('valueChanged(int)', wrapper(LDRV_PWM_PHASE_CFG2_PWM_PHASE_percentage_value_spinbox))

#LDRV_PWM_PHASE_CFG0_PWM_PHASE_absolute_value_spinbox.connect('valueChanged(int)', PWM_PHASE_CFG0_PWM_PHASE_absolute_value_update)

logging.info(f'{__name__=}')

#create a general function for absolute spinbox so as for relative
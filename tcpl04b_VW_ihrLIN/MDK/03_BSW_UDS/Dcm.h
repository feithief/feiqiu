#ifndef DCM_H_
#define DCM_H_

/* Minimal AUTOSAR-style DCM boundary.  The transport remains the original
 * IHR cooked LIN TP; this module only owns UDS service dispatch. */
void Dcm_MainFunction(void);

#endif

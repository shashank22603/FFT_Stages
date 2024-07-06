#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <stdlib.h>
#include "platform.h"
#include "xil_printf.h"
#include <xtime_l.h>
#include "xparameters.h"
#include "xaxidma.h"
//#include "dma_init.h"

#define N 16

int DMA_Init(XAxiDma * AxiDMA, u32 device_ID){
    XAxiDma_Config *DMA_confptr;
    int status;
    // Initialize the DMA
    DMA_confptr=XAxiDma_LookupConfig(XPAR_AXI_DMA_0_DEVICE_ID);
    status=XAxiDma_CfgInitialize(AxiDMA,DMA_confptr);
    // Check whether the DMA is initialized
    if(status!=XST_SUCCESS)
    {
    	printf("\n\rDMA Initialization Failed !");
    	return XST_FAILURE;
    }
    return XST_SUCCESS;
}



const int rev16[N] = {0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15};


const float complex W16[N/2] = {
    1.0 - 0.0 * I, cos(-M_PI / 8) + sin(-M_PI / 8) * I,
    cos(-M_PI / 4) + sin(-M_PI / 4) * I, cos(-3 * M_PI / 8) + sin(-3 * M_PI / 8) * I,
    0.0 - 1.0 * I, cos(-5 * M_PI / 8) + sin(-5 * M_PI / 8) * I,
    cos(-3 * M_PI / 4) + sin(-3 * M_PI / 4) * I, cos(-7 * M_PI / 8) + sin(-7 * M_PI / 8) * I
};
const float complex W8[N/4] = {1-0*I, M_SQRT1_2 - M_SQRT1_2 * I, 0-1*I, -M_SQRT1_2 - M_SQRT1_2 * I};
const float complex W4[N/8] = {1-0*I, 0-1*I};


void bitreverse (const float complex dataIn [N], float complex dataOut[N]){
		for(int i=0; i<N;i++){
			dataOut[i]=1+crealf (dataIn[rev32[i]]),1+cimagf(dataIn[rev32[i]]);
			}
	}
void FFT_stages(const float complex FFT_input[N], float complex FFT_output[N]){
    float complex temp1[N], temp2[N], temp3[N], temp4[N], temp5[N];

       stage1 :
       for(int i = 0; i < N; i += 2){
           temp1[i] = FFT_input[i] + FFT_input[i + 1];

           temp1[i + 1] = FFT_input[i] - FFT_input[i + 1];
       }

       stage2 :
       for(int i = 0; i < N ; i+=4){
           temp2[i] = temp1[i] + W4[0] * temp1[i + 2];
           temp2[i+1] = temp1[i+1] + W4[1] * temp1[i + 3];

           temp2[i + 2] = temp1[i] - W4[0] * temp1[i + 2];
           temp2[i + 3] = temp1[i+1] - W4[1] * temp1[i + 3];
       }

       stage3 :
       for(int i = 0; i < N; i+=8){
           temp3[i+0] = temp2[i+0] + W8[0] * temp2[i + 4];
           temp3[i+1] = temp2[i+1] + W8[1] * temp2[i + 5];
           temp3[i+2] = temp2[i+2] + W8[2] * temp2[i + 6];
           temp3[i+3] = temp2[i+3] + W8[3] * temp2[i + 7];

           temp3[i+4] = temp2[i+0] - W8[0] * temp2[i + 4];
           temp3[i+5] = temp2[i+1] - W8[1] * temp2[i + 5];
           temp3[i+6] = temp2[i+2] - W8[2] * temp2[i + 6];
           temp3[i+7] = temp2[i+3] - W8[3] * temp2[i + 7];
       }

    stage4 :
    for(int i = 0; i < N; i += 16){
        temp4[i+0] = temp3[i+0] + W16[0] * temp3[i + 8];
        temp4[i+1] = temp3[i+1] + W16[1] * temp3[i + 9];
        temp4[i+2] = temp3[i+2] + W16[2] * temp3[i + 10];
        temp4[i+3] = temp3[i+3] + W16[3] * temp3[i + 11];
        temp4[i+4] = temp3[i+4] + W16[4] * temp3[i + 12];
        temp4[i+5] = temp3[i+5] + W16[5] * temp3[i + 13];
        temp4[i+6] = temp3[i+6] + W16[6] * temp3[i + 14];
        temp4[i+7] = temp3[i+7] + W16[7] * temp3[i + 15];

        temp4[i+8]  = temp3[i+0] - W16[0] * temp3[i + 8];
        temp4[i+9]  = temp3[i+1] - W16[1] * temp3[i + 9];
        temp4[i+10] = temp3[i+2] - W16[2] * temp3[i + 10];
        temp4[i+11] = temp3[i+3] - W16[3] * temp3[i + 11];
        temp4[i+12] = temp3[i+4] - W16[4] * temp3[i + 12];
        temp4[i+13] = temp3[i+5] - W16[5] * temp3[i + 13];
        temp4[i+14] = temp3[i+6] - W16[6] * temp3[i + 14];
        temp4[i+15] = temp3[i+7] - W16[7] * temp3[i + 15];
    }

	 for(int i = 0; i < N; i++){
        FFT_output[i]=temp4[i];
	}

    
}




int main(){
init_platform();
// Initializing Timer instances for PS and PL
XTime PL_start_time, PL_end_time;
XTime PS_start_time, PS_end_time;

    const float complex FFT_input[N] = {
        40 , 77 , 62 , 85 ,
        50 , 28 , 81 , 19 ,
        36 , 66 , 70 , 42 ,
		54 , 26 , 58 , 61 ,
        40 , 77 , 62 , 85 ,
        50 , 28 , 81 , 19 ,
        36 , 66 , 70 , 42 ,
		54 , 26 , 58 , 61
    };
float complex FFT_output_sw[N], FFT_output_hw[N];
float complex FFT_rev_sw[N];
///// Software 64-point FFT
XTime_SetTime(0); // Setting Timer to value
XTime_GetTime(&PS_start_time); // Get Start Time
bitreverse (FFT_input, FFT_rev_sw);
FFT_stages (FFT_rev_sw, FFT_output_sw);
XTime_GetTime(&PS_end_time); // Get End Time



/////// Hardware 8-point FFT
int status;
XAxiDma AxiDMA;
status=DMA_Init(&AxiDMA, XPAR_AXI_DMA_0_DEVICE_ID);
if(status)
	return 1;// DMA Init Failed

XTime_SetTime(0); // Setting Timer to value
XTime_GetTime(&PL_start_time); // Get Start Time

// Simple DMA Transfers
status=XAxiDma_SimpleTransfer (&AxiDMA, (UINTPTR) FFT_output_hw, (sizeof(float complex) *N), XAXIDMA_DEVICE_TO_DMA);
status=XAxiDma_SimpleTransfer (&AxiDMA, (UINTPTR) FFT_input, (sizeof(float complex) *N), XAXIDMA_DMA_TO_DEVICE);
// POLLING-Check whether the DMA-to-Device and Device-to-DMA transfers are complete
while(XAxiDma_Busy (&AxiDMA, XAXIDMA_DMA_TO_DEVICE));
	//printf("\n\rDMA-to-Device Transfer Done!");
while(XAxiDma_Busy (&AxiDMA, XAXIDMA_DEVICE_TO_DMA));
	//printf("\n\rDevice-to-DMA Transfer Done!");

XTime_GetTime(&PL_end_time); // Get End Time


/////////// Verifying Hardware result with Software
for (int i=0;i<N;i++){
	printf("\n \rPS Output- %f+%fI, PL Output- %f+%fI", crealf (FFT_output_sw[i]), cimagf(FFT_output_sw[i]), crealf (FFT_output_hw[i]), cimagf (FFT_output_hw[i]));
	float diff1=abs (crealf (FFT_output_sw[i])-crealf (FFT_output_hw[i]));
	float diff2=abs (cimagf(FFT_output_sw[i])-cimagf(FFT_output_hw[i]));
//	 if (diff1>=0.01 && diff2>=0.01){
//	 	printf("\n\rData Mismatch found at index %d !",i);
//	 	break;
//	 }
//	 else
//	 	printf("DMA Transfer Successful!");
}
/////////// Software & Hardware Exceution Time calculation printf("\n \r Execution Time Comparison --------");
printf("\n\r------ Execution Time Comparison ------");
float time=0;
time= (float)1.0*(PS_end_time-PS_start_time)/(COUNTS_PER_SECOND/1000000); // Software Exceution Time calculation
printf("\n\r Execution Time for PS in Micro-Seconds : %f",time);

time=0;
time= (float)1.0*(PL_end_time-PL_start_time)/(COUNTS_PER_SECOND/1000000);  // Hardware Exceution Time calculation
printf("\n\r Execution Time for PL in Micro-Seconds : %f",time);
cleanup_platform();
return 0;
}
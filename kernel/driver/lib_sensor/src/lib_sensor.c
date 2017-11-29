/*
 * lib_sensor.c
 *
 *  Created on: 2015. 7. 8.
 *      Author: minsu
 */

#include <stdio.h>
#include <sam4e.h>

#include "itf_ubinos/itf/bsp.h"
#include "itf_ubinos/itf/ubinos.h"

#include "../itf/lib_sensor.h"
#include "../../lib_i2c/itf/lib_i2c.h"
#include "../../../porting/bsp_armcortexm_sam4eek/itf/sam4e16e.h"

static void sensor_irq_handler(void);

static unsigned short exps_sensor_type[ESTK_SENSOR_PORT_SIZE] = { 0 };
static int nxt_sensor_irq_counter = 0;
static int nxt_digital_irq_counter[4] = { 0 };

uint8_t gyroDataA[6] = { 0, };
uint8_t gyroDataB[6] = { 0, };
uint8_t gyroDataC[6] = { 0, };
uint8_t gyroDataD[6] = { 0, };
uint8_t accDataA[9] = { 0, };
uint8_t accDataB[9] = { 0, };
uint8_t accDataC[9] = { 0, };
uint8_t accDataD[9] = { 0, };
gyro gr_0;
gyro gr_1;
gyro gr_2;
gyro gr_3;
accl acc_0;
accl acc_1;
accl acc_2;
accl acc_3;

uint8_t lineDataA[8] = { 0, };
uint8_t lineDataB[8] = { 0, };
uint8_t lineDataC[8] = { 0, };
uint8_t lineDataD[8] = { 0, };
line ln_0;
line ln_1;
line ln_2;
line ln_3;

unsigned char tempeData[4][8] = { 0, } ;
tempe tempe_data[4] ;


//IMU Mode select
int selectMode = 10;
int selectMode_Line = LINE_PID;

int selectMode_Temp = TEMP_AMBI_C ;

int distance_state[4] = { 0 };
int digital_sen_data[4] = { -1, -1, -1, -1 };
int test_gyro_filter = 0;
int test_gyro_filter_1 = 0;
unsigned char softwareVersion[8] = { 0, };

void lib_sensor_component_init(void) {
   printf("library sensor test\n\r");
}
void initIMU() {

   memset(&gr_0, 0, sizeof(gyro));
   memset(&gr_1, 0, sizeof(gyro));
   memset(&gr_2, 0, sizeof(gyro));
   memset(&gr_3, 0, sizeof(gyro));

   memset(&acc_0, 0, sizeof(accl));
   memset(&acc_1, 0, sizeof(accl));
   memset(&acc_2, 0, sizeof(accl));
   memset(&acc_3, 0, sizeof(accl));

}
void sensor_init(unsigned short port0_sensor_type,
      unsigned short port1_sensor_type, unsigned short port2_sensor_type,
      unsigned short port3_sensor_type) {

   unsigned int dummy;

   initIMU();
   exps_sensor_type[0] = port0_sensor_type;
   exps_sensor_type[1] = port1_sensor_type;
   exps_sensor_type[2] = port2_sensor_type;
   exps_sensor_type[3] = port3_sensor_type;

   /// Enable the Peripheral ADC
//   pmc_enable_periph_clk(ID_AFEC1);
   PMC->PMC_PCER0 = 1 << ID_AFEC1;
   /// Disable All ADC Channels( 16 channels )
   AFEC1->AFEC_CHDR = 0xFFFF;

   /// Initialize Control Signals
   PIOC->PIO_PER = SEN_MASK | NXT_MASK;   // Set in PIO mode
   PIOC->PIO_OER = SEN_MASK | NXT_MASK;   // Configure in Output
   PIOC->PIO_SODR = SEN_MASK | NXT_MASK;   // Start status = High( 8.1 V )

//   PIOC->PIO_PER |= NXT_MASK;   ///HJ Set in PIO mode for port C
//   PIOC->PIO_OER |= NXT_MASK;   ///HJ Configure in Output
//   PIOC->PIO_SODR |= NXT_MASK;
   /// Set up ADC Mode Register

   AFEC1->AFEC_MR = AFEC_MR_TRGEN_DIS |
   AFEC_MR_SLEEP_NORMAL | AFEC_MR_PRESCAL(9) | AFEC_MR_TRACKTIM(15)
         | (AFEC_MR_STARTUP_SUT0 & AFEC_MR_STARTUP_Msk);
   AFEC1->AFEC_EMR = AFEC_EMR_RES_LOW_RES;

   AFEC1->AFEC_CHER = AFEC_CHER_CH4 |
   AFEC_CHER_CH5 |
   AFEC_CHER_CH6 |
   AFEC_CHER_CH7
   ;
   AFEC1->AFEC_COCR = 2047;

   /// Enable the Peripheral TC5
//   pmc_enable_periph_clk(ID_TC5);
   PMC->PMC_PCER0 = 1 << ID_TC5;

   TC1->TC_CHANNEL[2].TC_CCR = TC_CCR_CLKDIS;  // Counter Clock Disable Command
   TC1->TC_CHANNEL[2].TC_IDR = 0xFFFFFFFF;   // Timer Interrupt Disable Command
   dummy = TC1->TC_CHANNEL[2].TC_SR;
   TC1->TC_CHANNEL[2].TC_CMR = (TC_CMR_TCCLKS_TIMER_CLOCK1 | TC_CMR_CPCTRG); // Channel Mode ( TCLK1 Select(0) and RC Compare Trigger Enable )
   TC1->TC_CHANNEL[2].TC_CCR = TC_CCR_CLKEN;   // Counter Clock Enable Command
   TC1->TC_CHANNEL[2].TC_IER = TC_IER_CPCS;   // RC Compare Interrupt Enable
   intr_connectisr(ID_TC5, sensor_irq_handler, 0x40, 0);
   intr_enable(ID_TC5);
   TC1->TC_CHANNEL[2].TC_RC = 0x4B80;
   TC1->TC_CHANNEL[2].TC_CCR = TC_CCR_SWTRG;

//   tc_init(TC1,2,TC_CMR_TCCLKS_TIMER_CLOCK1|TC_CMR_CPCTRG);
//   intr_connectisr(ID_TC5, sensor_irq_handler, 0x40, 0);
//   tc_enable_interrupt(TC1, 2, TC_IER_CPCS);
//   intr_enable(ID_TC5);
//   TC1->TC_CHANNEL[2].TC_RC = 0x4B80;
//   tc_start(TC1,2);

   i2c_init();
   unsigned char i;
   for (i = 0; i < 4; i++) {
      if ((exps_sensor_type[i] & ESTK_SENSOR_TYPE_MASK) == NXT_DIGITAL_SENSOR) {
         i2c_enable(i);
      } else {
         i2c_disable(i);
      }
   }

}
void modeSelectIMU(int mode) {
   switch (mode) {
   case IMU_GYRO_ONLY:
      selectMode = mode;
      break;
   case IMU_ACC_ONLY:
      selectMode = mode;
      break;
   case IMU_CMPS_ONLY:
      selectMode = mode;
      break;
   default:
      break;

   }
}

void modeSelectLine(int mode) {
   selectMode_Line = mode;
}

void modeSelectTemp( int mode )
{
	selectMode_Temp = mode ;
}

int twoComplement(int val) {
   val = ~val;
   val += 1;
   val = val & 0x0000FFFF;
   val = -val;
   return val;
}

void getGyro(char port, int *gyro_val_array) {
   switch (port) {
   case 0:
      gr_0.gx = (((gr_0.gx_h << 8) & 0xFF00) | (gr_0.gx_l));
      gr_0.gy = (((gr_0.gy_h << 8) & 0xFF00) | (gr_0.gy_l));
      gr_0.gz = (((gr_0.gz_h << 8) & 0xFF00) | (gr_0.gz_l));

      if ((gr_0.gx & 0x00008000) == 0x00008000)
         gr_0.gx = twoComplement(gr_0.gx);
      if ((gr_0.gy & 0x00008000) == 0x00008000)
         gr_0.gy = twoComplement(gr_0.gy);
      if ((gr_0.gz & 0x00008000) == 0x00008000)
         gr_0.gz = twoComplement(gr_0.gz);

      gyro_val_array[0] = ((gr_0.gx) * DEGREE_250);
      gyro_val_array[1] = ((gr_0.gy) * DEGREE_250);
      gyro_val_array[2] = ((gr_0.gz) * DEGREE_250);

      break;
   case 1:
      gr_1.gx = ((gr_1.gx_h << 8) | (gr_1.gx_l));
      gr_1.gy = ((gr_1.gy_h << 8) | (gr_1.gy_l));
      gr_1.gz = ((gr_1.gz_h << 8) | (gr_1.gz_l));
      if ((gr_1.gx & 0x00008000) == 0x00008000)
         gr_1.gx = twoComplement(gr_1.gx);
      if ((gr_1.gy & 0x00008000) == 0x00008000)
         gr_1.gy = twoComplement(gr_1.gy);
      if ((gr_1.gz & 0x00008000) == 0x00008000)
         gr_1.gz = twoComplement(gr_1.gz);

      gyro_val_array[0] = ((gr_1.gx) * DEGREE_250);
      gyro_val_array[1] = ((gr_1.gy) * DEGREE_250);
      gyro_val_array[2] = ((gr_1.gz) * DEGREE_250);
      break;
   case 2:
      gr_2.gx = (((gr_2.gx_h << 8) & 0xFF00) | (gr_2.gx_l));
      gr_2.gy = (((gr_2.gy_h << 8) & 0xFF00) | (gr_2.gy_l));
      gr_2.gz = (((gr_2.gz_h << 8) & 0xFF00) | (gr_2.gz_l));
      if ((gr_2.gx & 0x00008000) == 0x00008000)
         gr_2.gx = twoComplement(gr_2.gx);
      if ((gr_2.gy & 0x00008000) == 0x00008000)
         gr_2.gy = twoComplement(gr_2.gy);
      if ((gr_2.gz & 0x00008000) == 0x00008000)
         gr_2.gz = twoComplement(gr_2.gz);

      gyro_val_array[0] = ((gr_2.gx) * DEGREE_250);
      gyro_val_array[1] = ((gr_2.gy) * DEGREE_250);
      gyro_val_array[2] = ((gr_2.gz) * DEGREE_250);
      break;
   case 3:
      gr_3.gx = (((gr_3.gx_h << 8) & 0xFF00) | (gr_3.gx_l));
      gr_3.gy = (((gr_3.gy_h << 8) & 0xFF00) | (gr_3.gy_l));
      gr_3.gz = (((gr_3.gz_h << 8) & 0xFF00) | (gr_3.gz_l));
      if ((gr_3.gx & 0x00008000) == 0x00008000)
         gr_3.gx = twoComplement(gr_3.gx);
      if ((gr_3.gy & 0x00008000) == 0x00008000)
         gr_3.gy = twoComplement(gr_3.gy);
      if ((gr_3.gz & 0x00008000) == 0x00008000)
         gr_3.gz = twoComplement(gr_3.gz);

      gyro_val_array[0] = ((gr_3.gx) * DEGREE_250);
      gyro_val_array[1] = ((gr_3.gy) * DEGREE_250);
      gyro_val_array[2] = ((gr_3.gz) * DEGREE_250);
      break;
   default:
      break;
   }
}
void getACC(char port, int *acc_val_array) {
   switch (port) {
   case 0:

      acc_0.ax = (((acc_0.ax_h << 8) & 0xFF00) | (acc_0.ax_l));
      acc_0.ay = (((acc_0.ay_h << 8) & 0xFF00) | (acc_0.ay_l));
      acc_0.az = (((acc_0.az_h << 8) & 0xFF00) | (acc_0.az_l));

      if ((acc_0.ax & 0x00008000) == 0x00008000)
         acc_0.ax = twoComplement(acc_0.ax);
      if ((acc_0.ay & 0x00008000) == 0x00008000)
         acc_0.ay = twoComplement(acc_0.ay);
      if ((acc_0.az & 0x00008000) == 0x00008000)
         acc_0.az = twoComplement(acc_0.az);

      acc_0.tx = (acc_0.tx >= 128) ? acc_0.tx - 256 : acc_0.tx;
      acc_0.ty = (acc_0.ty >= 128) ? acc_0.ty - 256 : acc_0.ty;
      acc_0.tz = (acc_0.tz >= 128) ? acc_0.tz - 256 : acc_0.tz;

      acc_val_array[0] = acc_0.ax;
      acc_val_array[1] = acc_0.ay;
      acc_val_array[2] = acc_0.az;
      acc_val_array[3] = acc_0.tx;
      acc_val_array[4] = acc_0.ty;
      acc_val_array[5] = acc_0.tz;

      break;
   case 1:
      acc_1.ax = (((acc_1.ax_h << 8) & 0xFF00) | (acc_1.ax_l));
      acc_1.ay = (((acc_1.ay_h << 8) & 0xFF00) | (acc_1.ay_l));
      acc_1.az = (((acc_1.az_h << 8) & 0xFF00) | (acc_1.az_l));

      if ((acc_1.ax & 0x00008000) == 0x00008000)
         acc_1.ax = twoComplement(acc_1.ax);
      if ((acc_1.ay & 0x00008000) == 0x00008000)
         acc_1.ay = twoComplement(acc_1.ay);
      if ((acc_1.az & 0x00008000) == 0x00008000)
         acc_1.az = twoComplement(acc_1.az);

      acc_val_array[0] = acc_1.ax;
      acc_val_array[1] = acc_1.ay;
      acc_val_array[2] = acc_1.az;
      acc_val_array[3] = acc_1.tx;
      acc_val_array[4] = acc_1.ty;
      acc_val_array[5] = acc_1.tz;
      break;
   case 2:
      acc_2.ax = (((acc_2.ax_h << 8) & 0xFF00) | (acc_2.ax_l));
      acc_2.ay = (((acc_2.ay_h << 8) & 0xFF00) | (acc_2.ay_l));
      acc_2.az = (((acc_2.az_h << 8) & 0xFF00) | (acc_2.az_l));
      if ((acc_2.ax & 0x00008000) == 0x00008000)
         acc_2.ax = twoComplement(acc_2.ax);
      if ((acc_2.ay & 0x00008000) == 0x00008000)
         acc_2.ay = twoComplement(acc_2.ay);
      if ((acc_2.az & 0x00008000) == 0x00008000)
         acc_2.az = twoComplement(acc_2.az);

      acc_val_array[0] = acc_2.ax;
      acc_val_array[1] = acc_2.ay;
      acc_val_array[2] = acc_2.az;
      acc_val_array[3] = acc_2.tx;
      acc_val_array[4] = acc_2.ty;
      acc_val_array[5] = acc_2.tz;
      break;
   case 3:
      acc_3.ax = (((acc_3.ax_h << 8) & 0xFF00) | (acc_3.ax_l));
      acc_3.ay = (((acc_3.ay_h << 8) & 0xFF00) | (acc_3.ay_l));
      acc_3.az = (((acc_3.az_h << 8) & 0xFF00) | (acc_3.az_l));

      if ((acc_3.ax & 0x00008000) == 0x00008000)
         acc_3.ax = twoComplement(acc_3.ax);
      if ((acc_3.ay & 0x00008000) == 0x00008000)
         acc_3.ay = twoComplement(acc_3.ay);
      if ((acc_3.az & 0x00008000) == 0x00008000)
         acc_3.az = twoComplement(acc_3.az);

      acc_val_array[0] = acc_3.ax;
      acc_val_array[1] = acc_3.ay;
      acc_val_array[2] = acc_3.az;
      acc_val_array[3] = acc_3.tx;
      acc_val_array[4] = acc_3.ty;
      acc_val_array[5] = acc_3.tz;
      break;
   }
}

void line_get(char port, int *line_val_array) {
   switch (port) {
   case 0:
      if (selectMode_Line == LINE_PID) {
         line_val_array[0] = ln_0.steering;
      } else if (selectMode_Line == LINE_PID_ALL) {
         line_val_array[0] = ln_0.steering;
         line_val_array[1] = ln_0.avg;
         line_val_array[2] = ln_0.result;

      } else if (selectMode_Line == LINE_CAL) //|| selectMode_Line == LINE_RAW )
      {
         line_val_array[0] = ln_0.led_0;
         line_val_array[1] = ln_0.led_1;
         line_val_array[2] = ln_0.led_2;
         line_val_array[3] = ln_0.led_3;
         line_val_array[4] = ln_0.led_4;
         line_val_array[5] = ln_0.led_5;
         line_val_array[6] = ln_0.led_6;
         line_val_array[7] = ln_0.led_7;
      }

      break;
   case 1:
      if (selectMode_Line == LINE_PID) {
         line_val_array[0] = ln_1.steering;
      } else if (selectMode_Line == LINE_PID_ALL) {
         line_val_array[0] = ln_1.steering;
         line_val_array[1] = ln_1.avg;
         line_val_array[2] = ln_1.result;

      } else if (selectMode_Line == LINE_CAL) //|| selectMode_Line == LINE_RAW)
      {
         line_val_array[0] = ln_1.led_0;
         line_val_array[1] = ln_1.led_1;
         line_val_array[2] = ln_1.led_2;
         line_val_array[3] = ln_1.led_3;
         line_val_array[4] = ln_1.led_4;
         line_val_array[5] = ln_1.led_5;
         line_val_array[6] = ln_1.led_6;
         line_val_array[7] = ln_1.led_7;
      }
      break;
   case 2:
      if (selectMode_Line == LINE_PID) {
         line_val_array[0] = ln_2.steering;
      } else if (selectMode_Line == LINE_PID_ALL) {
         line_val_array[0] = ln_2.steering;
         line_val_array[1] = ln_2.avg;
         line_val_array[2] = ln_2.result;

      } else if (selectMode_Line == LINE_CAL) //|| selectMode_Line == LINE_RAW)
      {
         line_val_array[0] = ln_2.led_0;
         line_val_array[1] = ln_2.led_1;
         line_val_array[2] = ln_2.led_2;
         line_val_array[3] = ln_2.led_3;
         line_val_array[4] = ln_2.led_4;
         line_val_array[5] = ln_2.led_5;
         line_val_array[6] = ln_2.led_6;
         line_val_array[7] = ln_2.led_7;
      }
      break;
   case 3:
      if (selectMode_Line == LINE_PID) {
         line_val_array[0] = ln_3.steering;
      } else if (selectMode_Line == LINE_PID_ALL) {
         line_val_array[0] = ln_3.steering;
         line_val_array[1] = ln_3.avg;
         line_val_array[2] = ln_3.result;

      } else if (selectMode_Line == LINE_CAL) //|| selectMode_Line == LINE_RAW)
      {
         line_val_array[0] = ln_3.led_0;
         line_val_array[1] = ln_3.led_1;
         line_val_array[2] = ln_3.led_2;
         line_val_array[3] = ln_3.led_3;
         line_val_array[4] = ln_3.led_4;
         line_val_array[5] = ln_3.led_5;
         line_val_array[6] = ln_3.led_6;
         line_val_array[7] = ln_3.led_7;
      }
      break;
   }
}

int ch = 0 ;
void temp_get( char port, int *temp_val_array )
{
	switch (selectMode_Temp) {
	case TEMP_AMBI_C:
		temp_val_array[0] = tempe_data[port].c_ambi ;
		break;
	case TEMP_TARG_C:
		temp_val_array[0] = tempe_data[port].c_targ ;
		break;
	case TEMP_AMBI_F:
		temp_val_array[0] = tempe_data[port].f_ambi ;
		break;
	case TEMP_TARG_F:
		temp_val_array[0] = tempe_data[port].f_targ ;
		break;
	case TEMP_C:
		temp_val_array[0] = tempe_data[port].c_ambi ;
		temp_val_array[1] = tempe_data[port].c_targ ;
		break ;
	case TEMP_F:
		temp_val_array[0] = tempe_data[port].f_ambi ;
		temp_val_array[1] = tempe_data[port].f_targ ;
		break ;
	case TEMP_ALL:
		temp_val_array[0] = tempe_data[port].c_ambi ;
		temp_val_array[1] = tempe_data[port].c_targ ;
		temp_val_array[2] = tempe_data[port].f_ambi ;
		temp_val_array[3] = tempe_data[port].f_targ ;
		if( ch < 10 )
			printf("temp_all ok \r\n") ;
		else
			ch = 10 ;
		ch++ ;
		break ;
	}
}


int sensor_get(char port) {
   int value;

   switch (port) {
   case 0:
      switch (exps_sensor_type[0] & ESTK_SENSOR_TYPE_MASK) {
      case NXT_SENSOR:
         // Assign ADC value from channel 4 (SEN_A)
         AFEC1->AFEC_CSELR = 4;
         value = AFEC1->AFEC_CDR;
         break;
      case NXT_DIGITAL_SENSOR:
         value = digital_sen_data[0];
         break;
      default:
         printf("error : <sensor get> undefined sensor type\r\n");
         return 0;
      }
      break;
   case 1:
      switch (exps_sensor_type[1] & ESTK_SENSOR_TYPE_MASK) {
      case NXT_SENSOR:
         // Assign ADC value from channel 5 (SEN_B)
         AFEC1->AFEC_CSELR = 5;
         value = AFEC1->AFEC_CDR;
         break;
      case NXT_DIGITAL_SENSOR:
         value = digital_sen_data[1];
         break;
      default:
         printf("error : <sensor get> undefined sensor type\r\n");
         return 0;
      }
      break;
   case 2:
      switch (exps_sensor_type[2] & ESTK_SENSOR_TYPE_MASK) {
      case NXT_SENSOR:
         // Assign ADC value from channel 6 (SEN_C)
         AFEC1->AFEC_CSELR = 6;
         value = AFEC1->AFEC_CDR;
         break;
      case NXT_DIGITAL_SENSOR:
         value = digital_sen_data[2];
         break;
      default:
         printf("error : <sensor get> undefined sensor type\r\n");
         return 0;
      }
      break;
   case 3:
      switch (exps_sensor_type[3] & ESTK_SENSOR_TYPE_MASK) {
      case NXT_SENSOR:
         // Assign ADC value from channel 7 (SEN_D)
         AFEC1->AFEC_CSELR = 7;
         value = AFEC1->AFEC_CDR;
         break;
      case NXT_DIGITAL_SENSOR:
         value = digital_sen_data[3];
         break;
      default:
         printf("error : <sensor get> undefined sensor type\r\n");
         return 0;
      }
      break;
   default:
      break;
   }
   return (value & 0x3FF);
}

void sensor_irq_handler(void) {
   unsigned int status;

   nxt_sensor_irq_counter++;

   switch (nxt_sensor_irq_counter) {
   case 1:
      switch (exps_sensor_type[0] & ESTK_SENSOR_TYPE_MASK) {
      case NXT_SENSOR:
         ///HJ Added for Light sensor so-called NXT Light sensor
         // Turn On RED Light
         PIOC->PIO_SODR = NXT_CON_00;
         // Assign ADC value from channel 4 (SEN_A)
         PIOC->PIO_CODR = SEN_CON_00;
         break;
      case NXT_DIGITAL_SENSOR:
         if (exps_sensor_type[0] == NXT_DIGITAL_SENSOR_SONA) {
            nxt_digital_irq_counter[0]++;
            if (nxt_digital_irq_counter[0] == 1)
               set_digital_sensor(0);
            else if (nxt_digital_irq_counter[0] >= 10)
               nxt_digital_irq_counter[0] = 0;

         } else if (exps_sensor_type[0] == EV3_IMU_SENSOR) {
            if (selectMode == IMU_GYRO_ONLY) {
               nxt_digital_irq_counter[0]++;
               if (nxt_digital_irq_counter[0] == 1)
                  getIMUgyroVal(0);
               else if (nxt_digital_irq_counter[0] >= 10)
                  nxt_digital_irq_counter[0] = 0;
            } else if (selectMode == IMU_ACC_ONLY) {
               nxt_digital_irq_counter[0]++;
               if (nxt_digital_irq_counter[0] == 1)
                  getIMUaccVal(0);
               else if (nxt_digital_irq_counter[0] >= 10)
                  nxt_digital_irq_counter[0] = 0;

            } else if (selectMode == IMU_CMPS_ONLY) {

            } else {
               printf("not define selectMode IMU..\r\n");
            }
         } else if (exps_sensor_type[0] == EV3_LINE_FOLLOWER_SENSOR) {
            nxt_digital_irq_counter[0]++;
            if (nxt_digital_irq_counter[0] == 1) {
               getLineVal(0);
               //              printf("get Line Val\r\n" ) ;
            } else if (nxt_digital_irq_counter[0] >= 10)
               nxt_digital_irq_counter[0] = 0;
         }
         else if( exps_sensor_type[0] == EV3_TEMPERATURE_SENSOR )
         {
        	 nxt_digital_irq_counter[0]++;
				if (nxt_digital_irq_counter[0] == 1) {
					getTempVal(0);
					//              printf("get Line Val\r\n" ) ;
				} else if (nxt_digital_irq_counter[0] >= 10)
					nxt_digital_irq_counter[0] = 0;
         }
         break;
      default:
         break;
      }

      AFEC1->AFEC_CHDR = ~0x10;
      AFEC1->AFEC_CHER = AFEC_CHER_CH4;
      break;

   case 2:
		AFEC1->AFEC_CR = AFEC_CR_START;
		break;

	case 3:
		switch (exps_sensor_type[1] & ESTK_SENSOR_TYPE_MASK) {
		case NXT_SENSOR:

			///HJ Added for Light sensor so-called NXT Light sensor
			// Turn Off RED Light
			if (exps_sensor_type[1] == NXT_SENSOR_LIGHT)
				PIOC->PIO_CODR = NXT_CON_00;
			// Assign ADC value from channel 4 (SEN_A)
			PIOC->PIO_CODR = SEN_CON_00;
			break;
		case NXT_DIGITAL_SENSOR:
			if (exps_sensor_type[1] == NXT_DIGITAL_SENSOR_SONA) {
				nxt_digital_irq_counter[1]++;
				if (nxt_digital_irq_counter[1] == 1)
					set_digital_sensor(1);
				else if (nxt_digital_irq_counter[1] >= 10)
					nxt_digital_irq_counter[1] = 0;

			} else if (exps_sensor_type[1] == EV3_IMU_SENSOR) {
				if (selectMode == IMU_GYRO_ONLY) {
					nxt_digital_irq_counter[1]++;
					if (nxt_digital_irq_counter[1] == 1)
						getIMUgyroVal(1);
					else if (nxt_digital_irq_counter[1] >= 10)
						nxt_digital_irq_counter[1] = 0;
				} else if (selectMode == IMU_ACC_ONLY) {
					nxt_digital_irq_counter[1]++;
					if (nxt_digital_irq_counter[1] == 1)
						getIMUaccVal(1);
					else if (nxt_digital_irq_counter[1] >= 10)
						nxt_digital_irq_counter[1] = 0;

				} else if (selectMode == IMU_CMPS_ONLY) {

				} else {
					printf("not define selectMode IMU..\r\n");
				}
			} else if (exps_sensor_type[1] == EV3_LINE_FOLLOWER_SENSOR) {
				nxt_digital_irq_counter[1]++;
				if (nxt_digital_irq_counter[1] == 1) {
					getLineVal(1);
					//              printf("get Line Val\r\n" ) ;
				} else if (nxt_digital_irq_counter[1] >= 10)
					nxt_digital_irq_counter[1] = 0;
			} else if (exps_sensor_type[1] == EV3_TEMPERATURE_SENSOR) {
				nxt_digital_irq_counter[1]++;
				if (nxt_digital_irq_counter[1] == 1) {
					getTempVal(1);
					//              printf("get Line Val\r\n" ) ;
				} else if (nxt_digital_irq_counter[1] >= 10)
					nxt_digital_irq_counter[1] = 0;
			}
			break;
		default:
			break;
		}
		AFEC1->AFEC_CHDR = ~0x20;
		AFEC1->AFEC_CHER = AFEC_CHER_CH5;
		break;

	case 4:
		AFEC1->AFEC_CR = AFEC_CR_START;
		break;

	case 5:
		switch (exps_sensor_type[2] & ESTK_SENSOR_TYPE_MASK) {
		case NXT_SENSOR:
			///HJ Added for Light sensor so-called NXT Light sensor
			// Turn Off RED Light
			if (exps_sensor_type[2] == NXT_SENSOR_LIGHT)
				PIOC->PIO_CODR = NXT_CON_01;
			// Assign ADC value from channel 5 (SEN_B)
			PIOC->PIO_CODR = SEN_CON_01;
			break;
		case NXT_DIGITAL_SENSOR:
			if (exps_sensor_type[2] == NXT_DIGITAL_SENSOR_SONA) {
				nxt_digital_irq_counter[2]++;
				if (nxt_digital_irq_counter[2] == 1)
					set_digital_sensor(2);
				else if (nxt_digital_irq_counter[2] >= 10)
					nxt_digital_irq_counter[2] = 0;
			} else if (exps_sensor_type[2] == EV3_IMU_SENSOR) {
				if (selectMode == IMU_GYRO_ONLY) {
					nxt_digital_irq_counter[2]++;
					if (nxt_digital_irq_counter[2] == 1)
						getIMUgyroVal(2);
					else if (nxt_digital_irq_counter[2] >= 10)
						nxt_digital_irq_counter[2] = 0;
				} else if (selectMode == IMU_ACC_ONLY) {
					nxt_digital_irq_counter[2]++;
					if (nxt_digital_irq_counter[2] == 1)
						getIMUaccVal(2);
					else if (nxt_digital_irq_counter[2] >= 10)
						nxt_digital_irq_counter[2] = 0;
				} else if (selectMode == IMU_CMPS_ONLY) {

				} else {
					printf("not define selectMode IMU..\r\n");
				}
			} else if (exps_sensor_type[2] == EV3_LINE_FOLLOWER_SENSOR) {
				nxt_digital_irq_counter[2]++;
				if (nxt_digital_irq_counter[2] == 1) {
					getLineVal(2);
					//              printf("get Line Val\r\n" ) ;
				} else if (nxt_digital_irq_counter[2] >= 10)
					nxt_digital_irq_counter[2] = 0;
			} else if (exps_sensor_type[2] == EV3_TEMPERATURE_SENSOR) {
				nxt_digital_irq_counter[2]++;
				if (nxt_digital_irq_counter[2] == 1) {
					getTempVal(2);
					//              printf("get Line Val\r\n" ) ;
				} else if (nxt_digital_irq_counter[2] >= 10)
					nxt_digital_irq_counter[2] = 0;
			}
			break;
		default:
			break;
		}

		AFEC1->AFEC_CHDR = ~0x40;
		AFEC1->AFEC_CHER = AFEC_CHER_CH6;
		break;

	case 6:
		AFEC1->AFEC_CR = AFEC_CR_START;
		break;

	case 7:
		switch (exps_sensor_type[3] & ESTK_SENSOR_TYPE_MASK) {
		case NXT_SENSOR:
			///HJ Added for Light sensor so-called NXT Light sensor
			// Turn Off RED Light
			if (exps_sensor_type[3] == NXT_SENSOR_LIGHT)
				PIOC->PIO_CODR = NXT_CON_02;
			// Assign ADC value from channel 6 (SEN_C)
			PIOC->PIO_CODR = SEN_CON_02;
			break;
		case NXT_DIGITAL_SENSOR:
			if (exps_sensor_type[3] == NXT_DIGITAL_SENSOR_SONA) {
				nxt_digital_irq_counter[3]++;
				if (nxt_digital_irq_counter[3] == 1)
					set_digital_sensor(3);
				else if (nxt_digital_irq_counter[3] >= 10)
					nxt_digital_irq_counter[3] = 0;
			} else if (exps_sensor_type[3] == EV3_IMU_SENSOR) {
				if (selectMode == IMU_GYRO_ONLY) {
					nxt_digital_irq_counter[3]++;
					if (nxt_digital_irq_counter[3] == 1)
						getIMUgyroVal(3);
					else if (nxt_digital_irq_counter[3] >= 10)
						nxt_digital_irq_counter[3] = 0;
				} else if (selectMode == IMU_ACC_ONLY) {
					nxt_digital_irq_counter[3]++;
					if (nxt_digital_irq_counter[3] == 1)
						getIMUaccVal(3);
					else if (nxt_digital_irq_counter[3] >= 10)
						nxt_digital_irq_counter[3] = 0;
				} else if (selectMode == IMU_CMPS_ONLY) {

				} else {
					printf("not define selectMode IMU..\r\n");
				}
			} else if (exps_sensor_type[3] == EV3_LINE_FOLLOWER_SENSOR) {
				nxt_digital_irq_counter[3]++;
				if (nxt_digital_irq_counter[3] == 1) {
					getLineVal(3);
					//              printf("get Line Val\r\n" ) ;
				} else if (nxt_digital_irq_counter[3] >= 10)
					nxt_digital_irq_counter[3] = 0;
			} else if (exps_sensor_type[3] == EV3_TEMPERATURE_SENSOR) {
				nxt_digital_irq_counter[3]++;
				if (nxt_digital_irq_counter[3] == 1) {
					getTempVal(3);
					//              printf("get Line Val\r\n" ) ;
				} else if (nxt_digital_irq_counter[3] >= 10)
					nxt_digital_irq_counter[3] = 0;
			}
         break;
      default:
         break;
      }

      AFEC1->AFEC_CHDR = ~0x80;
      AFEC1->AFEC_CHER = AFEC_CHER_CH7;
      break;

   case 8:
      AFEC1->AFEC_CR = AFEC_CR_START;
      break;

   case 9:
      switch (exps_sensor_type[3] & ESTK_SENSOR_TYPE_MASK) {
      case NXT_SENSOR:
         ///HJ Added for Light sensor so-called NXT Light sensor
         // Turn Off RED Light
         if (exps_sensor_type[0] == NXT_SENSOR_LIGHT)
            PIOC->PIO_CODR = NXT_CON_03;
         // Assign ADC value from channel 7 (SEN_D)
         PIOC->PIO_CODR = SEN_CON_03;
         break;
      case NXT_DIGITAL_SENSOR:
         break;
      default:
         break;
      }
      AFEC1->AFEC_CHDR = 0xFFFF;
      break;

   case 10:
      nxt_sensor_irq_counter = 0;
      break;
   }
   status = TC1->TC_CHANNEL[2].TC_SR;
}

void set_digital_sensor(unsigned char port) {
   if (i2c_busy(port) == 0) {
      digital_sen_data[port] = (int) distance_state[port];

      /* i2c_start_transaction just triggers an I2C transaction, actual data transaction
       * between ARM7 and a Ultrasonic sensor is done by an ISR after this, so there is one
       * cycle delay for consistent data acquistion
       */
//      i2c_start_transaction(port, 1, 0x42, 1, &distance_state[port], 1, 0);
//for IMU(IMU : device ID(17))
      i2c_start_transaction(port, 1, 0x42, 1, &distance_state[port], 1, 0);
   }
}
void getIMUgyroVal(unsigned char port) {
   if (i2c_busy(port) == 0) {
      switch (port) {
      case 0:
         gr_0.gx_l = gyroDataA[0];
         gr_0.gx_h = gyroDataA[1];
         gr_0.gy_l = gyroDataA[2];
         gr_0.gy_h = gyroDataA[3];
         gr_0.gz_l = gyroDataA[4];
         gr_0.gz_h = gyroDataA[5];

         i2c_start_transaction(port, 17, 0x53, 1, gyroDataA, 6, 0);

         break;
      case 1:
         gr_1.gx_l = gyroDataB[0];
         gr_1.gx_h = gyroDataB[1];
         gr_1.gy_l = gyroDataB[2];
         gr_1.gy_h = gyroDataB[3];
         gr_1.gz_l = gyroDataB[4];
         gr_1.gz_h = gyroDataB[5];

         i2c_start_transaction(port, 17, 0x53, 1, gyroDataB, 6, 0);

         break;
      case 2:
         gr_2.gx_l = gyroDataC[0];
         gr_2.gx_h = gyroDataC[1];
         gr_2.gy_l = gyroDataC[2];
         gr_2.gy_h = gyroDataC[3];
         gr_2.gz_l = gyroDataC[4];
         gr_2.gz_h = gyroDataC[5];

         i2c_start_transaction(port, 17, 0x53, 1, gyroDataC, 6, 0);

         break;
      case 3:
         gr_3.gx_l = gyroDataD[0];
         gr_3.gx_h = gyroDataD[1];
         gr_3.gy_l = gyroDataD[2];
         gr_3.gy_h = gyroDataD[3];
         gr_3.gz_l = gyroDataD[4];
         gr_3.gz_h = gyroDataD[5];

         i2c_start_transaction(port, 17, 0x53, 1, gyroDataD, 6, 0);

         break;

      }

   }

}
void getIMUaccVal(unsigned char port) {
   if (i2c_busy(port) == 0) {
      switch (port) {
      case 0:
         acc_0.tx = accDataA[0];
         acc_0.ty = accDataA[1];
         acc_0.tz = accDataA[2];
         acc_0.ax_l = accDataA[3];
         acc_0.ax_h = accDataA[4];
         acc_0.ay_l = accDataA[5];
         acc_0.ay_h = accDataA[6];
         acc_0.az_l = accDataA[7];
         acc_0.az_h = accDataA[8];

         i2c_start_transaction(port, 17, 0x42, 1, accDataA, 9, 0);

         break;
      case 1:
         acc_1.tx = accDataB[0];
         acc_1.ty = accDataB[1];
         acc_1.tz = accDataB[2];
         acc_1.ax_l = accDataB[3];
         acc_1.ax_h = accDataB[4];
         acc_1.ay_l = accDataB[5];
         acc_1.ay_h = accDataB[6];
         acc_1.az_l = accDataB[7];
         acc_1.az_h = accDataB[8];

         i2c_start_transaction(port, 17, 0x42, 1, accDataB, 9, 0);

         break;
      case 2:
         acc_2.tx = accDataC[0];
         acc_2.ty = accDataC[1];
         acc_2.tz = accDataC[2];
         acc_2.ax_l = accDataC[3];
         acc_2.ax_h = accDataC[4];
         acc_2.ay_l = accDataC[5];
         acc_2.ay_h = accDataC[6];
         acc_2.az_l = accDataC[7];
         acc_2.az_h = accDataC[8];

         i2c_start_transaction(port, 17, 0x42, 1, accDataC, 9, 0);

         break;

      case 3:
         acc_3.tx = accDataD[0];
         acc_3.ty = accDataD[1];
         acc_3.tz = accDataD[2];
         acc_3.ax_l = accDataD[3];
         acc_3.ax_h = accDataD[4];
         acc_3.ay_l = accDataD[5];
         acc_3.ay_h = accDataD[6];
         acc_3.az_l = accDataD[7];
         acc_3.az_h = accDataD[8];

         i2c_start_transaction(port, 17, 0x42, 1, accDataD, 9, 0);

         break;

      }
   }

}

void getLineVal(unsigned char port) {

   if (i2c_busy(port) == 0) {
      switch (port) {
      case 0:
         ln_0.steering = lineDataA[0];
         ln_0.avg = lineDataA[1];
         ln_0.result = lineDataA[2];

         if (selectMode_Line == LINE_PID) {
            i2c_start_transaction(port, 1, 0x42, 1, &lineDataA[0], 1, 0);
         } else if (selectMode_Line == LINE_PID_ALL) {
            i2c_start_transaction(port, 1, 0x42, 1, &lineDataA[0], 3, 0);
         } else if (selectMode_Line == LINE_CAL) //|| selectMode_Line == LINE_RAW )
         {
            ln_0.led_0 = lineDataA[0];
            ln_0.led_1 = lineDataA[1];
            ln_0.led_2 = lineDataA[2];
            ln_0.led_3 = lineDataA[3];
            ln_0.led_4 = lineDataA[4];
            ln_0.led_5 = lineDataA[5];
            ln_0.led_6 = lineDataA[6];
            ln_0.led_7 = lineDataA[7];

//            if (selectMode_Line == LINE_CAL)
            i2c_start_transaction(port, 1, 0x49, 1, &lineDataA[0], 8, 0);
//            else
//               i2c_start_transaction(port, 1, 0x74, 1, &lineDataA[0], 8, 0);
         }

         break;
      case 1:
         ln_1.steering = lineDataB[0];
         ln_1.avg = lineDataB[1];
         ln_1.result = lineDataB[2];

         if (selectMode_Line == LINE_PID) {
            i2c_start_transaction(port, 1, 0x42, 1, &lineDataB[0], 1, 0);
         } else if (selectMode_Line == LINE_PID_ALL) {
            i2c_start_transaction(port, 1, 0x42, 1, &lineDataB[0], 3, 0);
         } else if (selectMode_Line == LINE_CAL) //|| selectMode_Line == LINE_RAW)
         {
            ln_1.led_0 = lineDataB[0];
            ln_1.led_1 = lineDataB[1];
            ln_1.led_2 = lineDataB[2];
            ln_1.led_3 = lineDataB[3];
            ln_1.led_4 = lineDataB[4];
            ln_1.led_5 = lineDataB[5];
            ln_1.led_6 = lineDataB[6];
            ln_1.led_7 = lineDataB[7];

//            if (selectMode_Line == LINE_CAL)
            i2c_start_transaction(port, 1, 0x49, 1, &lineDataB[0], 8, 0);
//            else
//               i2c_start_transaction(port, 1, 0x74, 1, &lineDataB[0], 8, 0);
         }
         break;
      case 2:
         ln_2.steering = lineDataC[0];
         ln_2.avg = lineDataC[1];
         ln_2.result = lineDataC[2];

         if (selectMode_Line == LINE_PID) {
            i2c_start_transaction(port, 1, 0x42, 1, &lineDataC[0], 1, 0);
         } else if (selectMode_Line == LINE_PID_ALL) {
            i2c_start_transaction(port, 1, 0x42, 1, &lineDataC[0], 3, 0);
         } else if (selectMode_Line == LINE_CAL) //|| selectMode_Line == LINE_RAW)
         {
            ln_2.led_0 = lineDataC[0];
            ln_2.led_1 = lineDataC[1];
            ln_2.led_2 = lineDataC[2];
            ln_2.led_3 = lineDataC[3];
            ln_2.led_4 = lineDataC[4];
            ln_2.led_5 = lineDataC[5];
            ln_2.led_6 = lineDataC[6];
            ln_2.led_7 = lineDataC[7];

//            if( selectMode_Line == LINE_CAL )
            i2c_start_transaction(port, 1, 0x49, 1, &lineDataC[0], 8, 0);
//            else
//               i2c_start_transaction(port, 1, 0x74, 1, &lineDataC[0], 8, 0);
         }
         break;
      case 3:
         ln_3.steering = lineDataD[0];
         ln_3.avg = lineDataD[1];
         ln_3.result = lineDataD[2];

         if (selectMode_Line == LINE_PID) {
            i2c_start_transaction(port, 1, 0x42, 1, &lineDataD[0], 1, 0);
         } else if (selectMode_Line == LINE_PID_ALL) {
            i2c_start_transaction(port, 1, 0x42, 1, &lineDataD[0], 3, 0);
         } else if (selectMode_Line == LINE_CAL) //|| selectMode_Line == LINE_RAW)
         {
            ln_3.led_0 = lineDataD[0];
            ln_3.led_1 = lineDataD[1];
            ln_3.led_2 = lineDataD[2];
            ln_3.led_3 = lineDataD[3];
            ln_3.led_4 = lineDataD[4];
            ln_3.led_5 = lineDataD[5];
            ln_3.led_6 = lineDataD[6];
            ln_3.led_7 = lineDataD[7];

//            if (selectMode_Line == LINE_CAL)
            i2c_start_transaction(port, 1, 0x49, 1, &lineDataD[0], 8, 0);
//            else
//               i2c_start_transaction(port, 1, 0x74, 1, &lineDataD[0], 8, 0);
         }
         break;
      }
   }
}

void getTempVal( unsigned char port )
{
	 if (i2c_busy(port) == 0) {
		 tempe_data[port].c_ambi = tempeData[port][0] + tempeData[port][1]*255 ;
		 tempe_data[port].c_targ = tempeData[port][2] + tempeData[port][3]*255 ;
		 tempe_data[port].f_ambi = tempeData[port][4] + tempeData[port][5]*255 ;
		 tempe_data[port].f_targ = tempeData[port][6] + tempeData[port][7]*255 ;

//		 printf( " %d	%d	%d	%d	%d	%d	%d	%d\r\n", tempeData[port][1], tempeData[port][0],tempeData[port][3],tempeData[port][2],tempeData[port][5],tempeData[port][4],tempeData[port][7],tempeData[port][6] ) ;
		 i2c_start_transaction( port, 21, 0x42, 1, &tempeData[port][0], 8, 0 ) ;
	 }
}

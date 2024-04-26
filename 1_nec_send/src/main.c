//=========================================================
// src/nec_implementation_main.c: generated by Hardware Configurator
//
// This file will be updated when saving a document.
// leave the sections inside the "$[...]" comment tags alone
// or they will be overwritten!!
//=========================================================

//-----------------------------------------------------------------------------
// Includes
//-----------------------------------------------------------------------------
#include <SI_EFM8SB2_Register_Enums.h>                  // SFR declarations
#include "InitDevice.h"
// $[Generated Includes]
// [Generated Includes]$

SI_SBIT(signal_out, P2, 3U);
#define NINE_MS_OF_COUNT 342
#define IMPULSE_NEEDED 21
#define START_BURST_COUNT 16
#define START_NOTHING_COUNT 8
#define FOUR_AND_HALF_MS_OF_COUNT 171
#define ONE 1
#define ZERO 0
#define HIGH 1
#define LOW 0
#define BITS_IN_BYTE 8

uint8_t counter = IMPULSE_NEEDED;
uint8_t data_out[4];
const uint8_t DATA_OUT_LENGTH = 4;

//-----------------------------------------------------------------------------
// SiLabs_Startup() Routine
// ----------------------------------------------------------------------------
// This function is called immediately after reset, before the initialization
// code is run in SILABS_STARTUP.A51 (which runs before main() ). This is a
// useful place to disable the watchdog timer, which is enable by default
// and may trigger before main() in some instances.
//-----------------------------------------------------------------------------
void SiLabs_Startup (void)
{
  // $[SiLabs Startup]
  // [SiLabs Startup]$
}

void FillDataArray(uint8_t address, uint8_t command)
{
	data_out[0] = address;
	data_out[1] = ~address;
	data_out[2] = command;
	data_out[3] = ~command;
}

void SendLogicOneSignal()
{
	counter = IMPULSE_NEEDED;
	while(counter != 0)
	{
		if(TMR2CN0_TF2H)
		{
			TMR2CN0_TF2H = 0x00;
			signal_out = ONE;
			signal_out = ZERO;
			counter--;
		}
	}
}


void SendPulse(uint8_t p_counter)
{
	signal_out = HIGH;
	counter = p_counter;

	while(counter != 0)
	{
		if(TCON_TF0)
		{
			TCON_TF0 = 0x00;
			counter--;
		}
		if(TMR2CN0_TF2H)
		{
			TMR2CN0_TF2H = 0x00;
			signal_out = !signal_out;
		}
	}
}

void SendNothing(uint8_t p_counter)
{
	signal_out = LOW;

	counter = p_counter;
	while(counter != 0)
	{
		if(TCON_TF0)
		{
			TCON_TF0 = 0x00;
			counter--;
		}
	}
}

void SendStartSignal()
{
	SendPulse(START_BURST_COUNT);

	SendNothing(START_NOTHING_COUNT);
}

void SendLogicOne()
{
	SendPulse(1);

	SendNothing(3);
}

void SendLogicZero()
{
	SendPulse(1);

	SendNothing(1);
}

uint8_t index = 0;
uint8_t n = 0;
bit data_bit = 0;
void SendSignalOut()
{
	for(index = 0; index < DATA_OUT_LENGTH; index++)
	{
		for(n = BITS_IN_BYTE; n > 0; n--)
		{
			data_bit = (data_out[index] >> n) & 1U;
			if(data_bit == 0)
			{
				SendLogicZero();
			}
			else
			{
				SendLogicOne();
			}
		}
	}

	SendPulse(1);
}

//-----------------------------------------------------------------------------
// main() Routine
// ----------------------------------------------------------------------------
int main (void)
{
  // Call hardware initialization routine
  enter_DefaultMode_from_RESET();

  while (1) 
  {
    // $[Generated Run-time code]
    // [Generated Run-time code]$
	  FillDataArray(0, 181);
	  SendStartSignal();
	  SendSignalOut();
  }                             
}
/*
 * MIDI_application.c
 *
 *  First created on: 6 dec. 2014
 *      Author: Xavier Halgand
 */

/* Includes ------------------------------------------------------------------*/

#include "MIDI_application.h"

/*-----------------------------------------------------------------------------*/

//int8_t currentNote _DTCMRAM_;
//int8_t velocity _DTCMRAM_;
uint8_t notes_On[128] _DTCMRAM_; /*= {0};*/
int8_t notesCount = 0; // number of notes on (keys pressed)
int8_t currentNote;
int8_t velocity;
extern ADSR_t adsr;
extern bool g_sequencerIsOn;


void Do_____nothing(uint8_t val);

/*------------------------------MIDI CC implementation----------------------------------------*/

void (*ControlChangeFunctionsTable[128])(uint8_t val) =
{	Do_____nothing, 			/* 0 */
	VibratoAmp_set,
	DelayWet_set,
	seq_tempo_set,
	seq_freqMax_set,
	seq_scale_set, 				/* 5 */
	Sound_set,
	Volume_set,					/* 7 Volume */
	Filter1Freq_set,
	Filter1Res_set,
	Do_____nothing, 			/* 10 */
	Do_____nothing,
	Filter1Drive_set,
	Volume_set,
	Delay_time_set,
	DelayFeedback_set, 			/* 15 */
	VibratoAmp_set,
	VibratoFreq_set,
	ChorusRate_set,
	ChorusDelay_set,
	ChorusSweep_set, 			/* 20 */
	ChorusFeedback_set,
	ChorusSecondRate_set,
	Delay_switch,
	Phaser_switch,
	seq_switchMute,				/* 25 */
	seq_switchMovingSeq,
	Filter_Random_switch,
	Chorus_switch,
	ChorusMode_switch,
	ChorusFDBsign_switch,		/* 30 */
	SynthOut_switch,
	Do_____nothing,
	seq_transpM2,
	seq_transpP2,
	seq_transpM7,				/* 35 */
	seq_transpP7,
	Chorus_reset,
	MagicFX,
	MagicPatch,
	Delay_time_dec,				/* 40 */
	Delay_time_inc,
	seq_gateTime_set,
	AttTime_set,
	Do_____nothing,
	Do_____nothing,				/* 45 */
	Do_____nothing,
	Do_____nothing,
	Do_____nothing,
	Do_____nothing,
	DecTime_set,				/* 50 */
	SustLevel_set,
	RelTime_set,
	Filter1Freq_set,
	Filter1Res_set,
	Filter1Drive_set,			/* 55 */
	Filter1Type_set,
	AmpLFO_amp_set,
	AmpLFO_freq_set,
	Drifter_amp_set,
	Drifter_minFreq_set,		/* 60 */
	Drifter_maxFreq_set,
	Drifter_centralFreq_set,
	SynthOut_amp_set,
	Do_____nothing, 			/*  64 Damper pedal */
	Filt1LFO_amp_set,			/* 65 */
	Filt1LFO_freq_set,
	DemoMode_toggle,
	Synth_reset,
	Sequencer_toggle,
	Freeze_toggle,				/* 70 */
	Do_____nothing,
	Do_____nothing,
	Do_____nothing,
	Do_____nothing,
	Do_____nothing,				/* 75 */
	RandSound1,
	RandSound2,
	Do_____nothing,
	Do_____nothing,
	Do_____nothing,				/* 80 */
	MagicFX,
	MagicPatch,
	Do_____nothing,
	Do_____nothing,
	FM_OP1_freq_set,			/* 85 */
	FM_OP2_freq_set,
	FM_OP3_freq_set,
	FM_OP4_freq_set,
	Phaser_Wet_set,
	Filter2Freq_set,			/* 90 */
	Filter2Res_set,
	Filter2Drive_set,
	Filter2Type_set,
	FM_OP1_modInd_set,
	FM_OP2_modInd_set,			/* 95 */
	FM_OP3_modInd_set,
	FM_OP4_modInd_set,
	Do_____nothing,
	Do_____nothing,
	Do_____nothing,				/* 100 */
	Do_____nothing,
	Phaser_Rate_set,
	Phaser_Feedback_set,
	Do_____nothing,
	Filt2LFO_amp_set,			/* 105 */
	Filt2LFO_freq_set,
	Do_____nothing,
	FM_OP2_freqMul_inc,
	FM_OP3_freqMul_inc,
	FM_OP4_freqMul_inc,			/* 110 */
	Do_____nothing,
	Do_____nothing,
	Do_____nothing,
	Do_____nothing,
	Do_____nothing,				/* 115 */
	Do_____nothing,
	FM_OP2_freqMul_dec,
	FM_OP3_freqMul_dec,
	FM_OP4_freqMul_dec,
	Do_____nothing,				/* 120 */
	Do_____nothing,
	Do_____nothing,
	Do_____nothing,
	Do_____nothing,
	Do_____nothing,				/* 125 */
	Do_____nothing,
	Do_____nothing				/* 127 END */

};
/*-----------------------------------------------------------------------------*/
void Do_____nothing(uint8_t val)  { }  /* for ControlFunctionsTable */

/*-----------------------------------------------------------------------------*/
void Reset_notes_On(void)
{
	notesCount = 0;
	for (uint8_t i = 0; i < 128; i++)
		notes_On[i] = 0;
}

/*-----------------------------------------------------------------------------*/
void ProcessReceivedMidiDatas(midi_package_t pack)
{
	if (g_sequencerIsOn == false)
	{
		if ((pack.evnt0 & 0xF0) == 0x80) // Note off ? -------------------------------
		{
			uint8_t noteOff = pack.evnt1;
			notes_On[noteOff] = 0;
			notesCount--;
			if (notesCount <= 0) // no more keys pressed
			{
				ADSR_keyOff(&adsr);
				notesCount = 0;
			}
			else // some keys still pressed... (legato)
			{
				if ((noteOff - LOWEST_NOTE) == currentNote) // then let sound the lowest key pressed
				{
					uint8_t i;
					for (i = 0; i < 128; i++)
					{
						if (notes_On[i] == 1) // find the lowest key pressed
							break;
					}
					currentNote = i - LOWEST_NOTE; // conversion for notesFreq[]
				}
			}
		}
		else if ((pack.evnt0 & 0xF0) == 0x90) // Note on ----------------------------
		{
			uint8_t noteOn = pack.evnt1;
			velocity = pack.evnt2;
			if (velocity > 0) // True note on !
			{
				if (noteOn < LOWEST_NOTE) // filtering notes
				{
					currentNote = 0;
				}
				else
				{
					currentNote = noteOn - LOWEST_NOTE; // conversion for notesFreq[]
				}

				ADSR_keyOn(&adsr);
				notesCount++;
				notes_On[noteOn] = 1;
			}
			else
			{
				// ---------  Sometimes Note On with zero velocity means Note Off !
				notes_On[noteOn] = 0;
				notesCount--;
				if (notesCount <= 0)
				{
					ADSR_keyOff(&adsr);
					notesCount = 0;
				}
				else
				{
					if ((noteOn - LOWEST_NOTE) == currentNote)
					{
						uint8_t i;
						for (i = 0; i < 128; i++)
						{
							if (notes_On[i] == 1) // find the lowest key pressed
								break;
						}
						currentNote = i - LOWEST_NOTE; // conversion for notesFreq[]
					}
				}
			}
		}
		else if ((pack.evnt0 & 0xF0) == 0xA0) // Aftertouch
		{
			// Filter1Res_set(pack.evnt2);
		}
		else if ((pack.evnt0 & 0xF0) == 0xE0) // Pitch Bend
		{
			// int16_t pitchBend = ((pack.evnt1 << 7) + pack.evnt2) - 0x2000;
		}
	}

	/*------------------------------------------------------------------------------------------*/

	if ((pack.evnt0 & 0xF0) == 0xB0) /* If incoming midi message is a Control Change... */
	{
		ControlChangeFunctionsTable[pack.evnt1](pack.evnt2);
	}
}

/*-----------------------------------------------------------------------------*/

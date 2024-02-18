/**
 ******************************************************************************
 * File Name        : soundgen.c
 * Author			: Xavier Halgand
 * Date             : 2024
 * Description      : main synthesizer file : modules & structure
 ******************************************************************************
 */

/*
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA
 *
 */

#include "soundGen.h"
#include "openamp_interface.h"

/*--------------------------------------------------------------*/
#define EPSI				.00002f
/*--------------------------------------------------------------*/

extern float samplerate;

bool g_sequencerIsOn;

extern Sequencer_t seq;
extern NoteGenerator_t noteGen;
extern int8_t currentNote;
extern int8_t velocity;

/*--------------------------------------------------------------*/
static Oscillator_t op1 _DTCMRAM_;
static Oscillator_t op2 _DTCMRAM_;
static Oscillator_t op3 _DTCMRAM_;
static Oscillator_t op4 _DTCMRAM_;

static BlepOscillator_t mbSawOsc _DTCMRAM_;
static BlepOscillator_t mbRectOsc _DTCMRAM_;
static BlepOscillator_t mbTriOsc _DTCMRAM_;
static BlepOscillator_t mbRectOsc2 _DTCMRAM_;

static Add_oscillator_t addosc _DTCMRAM_;
static DriftingOsc_t driftosc _DTCMRAM_;
static Drifter_t d1 _DTCMRAM_;
static Drifter_t d2 _DTCMRAM_;

static ADSR_t adsr _DTCMRAM_;
static ResonantFilter SVFilter1 _DTCMRAM_;
static ResonantFilter SVFilter2 _DTCMRAM_;

static Metro_t metro1, metro2, metro3; /* 3 metronomes which tempi are in rational ratios  */
static float proba1, proba2, proba3;
static bool metro_reset_requested;
static ADSR_t adsr2 _DTCMRAM_;
static ADSR_t adsr3 _DTCMRAM_;
static Oscillator_t oscill2 _DTCMRAM_;
static Oscillator_t oscill3 _DTCMRAM_;
static Oscillator_t amp_lfo2 _DTCMRAM_;

static Oscillator_t vibr_lfo _DTCMRAM_;
static Oscillator_t filt_lfo _DTCMRAM_;
static Oscillator_t filt2_lfo _DTCMRAM_;
static Oscillator_t amp_lfo _DTCMRAM_;

static float f01 _DTCMRAM_;
static float f02 _DTCMRAM_;
static float f03 _DTCMRAM_;
static float vol1 _DTCMRAM_;
static float vol2 _DTCMRAM_;
static float vol3 _DTCMRAM_;

static bool desynkatorON;
static bool demoModeON;
static bool freezeON;
static bool autoFilterON _DTCMRAM_;
static bool delayON _DTCMRAM_;
static bool phaserON _DTCMRAM_;
static bool chorusON _DTCMRAM_;

static Timbre_t sound _DTCMRAM_;
static int8_t autoSound _DTCMRAM_;

static float f0 _DTCMRAM_;
static float vol _DTCMRAM_;

static SynthPatch_t mypatch;

/*============================================== Main Synth initialization =========================================*/

void Synth_Init(void)
{
	samplerate = (float) SAMPLERATE;

	g_sequencerIsOn = true;
	demoModeON = true;
	freezeON = false;
	vol = 1.0f;
	sound = MORPH_SAW;
	autoFilterON = false;
	autoSound = 0;
	chorusON = false;
	delayON = false;
	phaserON = false;
	desynkatorON = false;

	sinetable_init();
	sequencer_init(samplerate);

	Osc_init(&op1, 0.8f, 587.f);
	Osc_init(&op2, 0.8f, 587.f);
	Osc_init(&op3, 0.8f, 587.f);
	Osc_init(&op4, 0.8f, 587.f);
	AddOsc_init(&addosc, 0.8f, 587.f);
	AddOsc_gen_newWaveform(&addosc);
	DriftOsc_init(&driftosc);
	Drifter_init(&d1);
	Drifter_init(&d2);

	BlepOsc_init(&mbSawOsc, BLEP_SAW);
	BlepOsc_init(&mbTriOsc, BLEP_TRI);
	BlepOsc_init(&mbRectOsc, BLEP_RECT);

	SVF_initialize(&SVFilter1);
	SVF_initialize(&SVFilter2);

	Osc_init(&vibr_lfo, 0.0f, VIBRATO_FREQ);
	Osc_init(&filt_lfo, 0, 0);
	Osc_init(&filt2_lfo, 0, 0);
	Osc_init(&amp_lfo, 0, 0);

	ADSR_init(&adsr);

	Delay_init();
	PhaserInit();
	Chorus_init();

	/*---- Desynkator initialization -------*/
	metro_initBPM(&metro1, 90.0f, samplerate);
	metro_initBPM(&metro2, 90.0f, samplerate);
	metro_initBPM(&metro3, 90.0f, samplerate);
	proba1 = proba2 = proba3 = 0.6f;
	ADSR_init(&adsr2);
	ADSR_init(&adsr3);
	Osc_init(&oscill2, 0.8f, 587.f);
	oscill2.type = MSAW;
	BlepOsc_init(&mbRectOsc2, BLEP_RECT);
	mbRectOsc2.waveform = 0.5f; // more rectangle wave
	Osc_init(&oscill3, 0.8f, 587.f);
	Osc_init(&amp_lfo2, 0.3f, VIBRATO_FREQ);
	metro_reset_requested = false;
}

/*--------------------------------------------------------------------------*/
void Synth_patch_save(SynthPatch_t *patch)
{
	patch->desynkatorON_par = desynkatorON;
	patch->autoFilterON_par = autoFilterON;
	patch->delayON_par = delayON;
	patch->phaserON_par = phaserON;
	patch->chorusON_par = chorusON;
	patch->sound_par = sound;
	patch->autoSound_par = autoSound;

	Oscillator_params_save(&op1, &patch->op1_par);
	Oscillator_params_save(&op2, &patch->op2_par);
	Oscillator_params_save(&op3, &patch->op3_par);
	Oscillator_params_save(&op4, &patch->op4_par);
	Oscillator_params_save(&oscill2, &patch->oscill2_par);
	Oscillator_params_save(&oscill3, &patch->oscill3_par);
	Oscillator_params_save(&amp_lfo2, &patch->amp_lfo2_par);
	Oscillator_params_save(&amp_lfo, &patch->amp_lfo_par);
	Oscillator_params_save(&vibr_lfo, &patch->vibr_lfo_par);
	Oscillator_params_save(&filt_lfo, &patch->filt_lfo_par);
	Oscillator_params_save(&filt2_lfo, &patch->filt2_lfo_par);

	BlepOsc_params_save(&mbSawOsc, &patch->saw_osc_par);
	BlepOsc_params_save(&mbTriOsc, &patch->tri_osc_par);
	BlepOsc_params_save(&mbRectOsc, &patch->rect_osc1_par);
	BlepOsc_params_save(&mbRectOsc2, &patch->rect_osc2_par);

	DriftOsc_params_save(&driftosc, &patch->driftosc_par);

	Drifter_params_save(&d1, &patch->d1_par);
	Drifter_params_save(&d2, &patch->d2_par);

	AddOsc_params_save(&addosc, &patch->addosc_par);

	ADSR_params_save(&adsr, &patch->adsr_par);
	ADSR_params_save(&adsr2, &patch->adsr2_par);
	ADSR_params_save(&adsr3, &patch->adsr3_par);

	ResonantFilter_params_save(&SVFilter1, &patch->filt1_par);
	ResonantFilter_params_save(&SVFilter2, &patch->filt2_par);

	Sequencer_params_save(&seq, &noteGen, &patch->seq_par);

	Metro_params_save(&metro1, &patch->metro1_par);
	Metro_params_save(&metro2, &patch->metro2_par);
	Metro_params_save(&metro3, &patch->metro3_par);
	patch->proba1_par = proba1;
	patch->proba2_par = proba2;
	patch->proba3_par = proba3;

	Phaser_params_save(&patch->phaser_par);
	Delay_params_save(&patch->delay_par);
	Chorus_params_save(&patch->chorus_par);

	send_patch_to_CM4(patch);
}

/*--------------------------------------------------------------------------*/
void Synth_patch_load(const SynthPatch_t *patch)
{
	g_sequencerIsOn = true;
	demoModeON = false;

	desynkatorON = patch->desynkatorON_par;
	autoFilterON = patch->autoFilterON_par;
	delayON = patch->delayON_par;
	phaserON = patch->phaserON_par;
	chorusON = patch->chorusON_par;
	sound = patch->sound_par;
	autoSound = patch->autoSound_par;

	Oscillator_params_set(&patch->op1_par, &op1);
	Oscillator_params_set(&patch->op2_par, &op2);
	Oscillator_params_set(&patch->op3_par, &op3);
	Oscillator_params_set(&patch->op4_par, &op4);
	Oscillator_params_set(&patch->oscill2_par, &oscill2);
	Oscillator_params_set(&patch->oscill3_par, &oscill3);
	Oscillator_params_set(&patch->amp_lfo2_par, &amp_lfo2);
	Oscillator_params_set(&patch->amp_lfo_par, &amp_lfo);
	Oscillator_params_set(&patch->vibr_lfo_par, &vibr_lfo);
	Oscillator_params_set(&patch->filt_lfo_par, &filt_lfo);
	Oscillator_params_set(&patch->filt2_lfo_par, &filt2_lfo);

	BlepOsc_params_set(&patch->saw_osc_par, &mbSawOsc);
	BlepOsc_params_set(&patch->tri_osc_par, &mbTriOsc);
	BlepOsc_params_set(&patch->rect_osc1_par, &mbRectOsc);
	BlepOsc_params_set(&patch->rect_osc2_par, &mbRectOsc2);

	DriftOsc_params_set(&patch->driftosc_par, &driftosc);

	Drifter_params_set(&patch->d1_par, &d1);
	Drifter_params_set(&patch->d2_par, &d2);

	AddOsc_params_set(&patch->addosc_par, &addosc);

	ADSR_params_set(&patch->adsr_par, &adsr);
	ADSR_params_set(&patch->adsr2_par, &adsr2);
	ADSR_params_set(&patch->adsr3_par, &adsr3);

	ResonantFilter_params_set(&patch->filt1_par, &SVFilter1);
	ResonantFilter_params_set(&patch->filt2_par, &SVFilter2);

	Sequencer_params_set(&patch->seq_par, &seq, &noteGen);

	Metro_params_set(&patch->metro1_par, &metro1);
	Metro_params_set(&patch->metro2_par, &metro2);
	Metro_params_set(&patch->metro3_par, &metro3);
	proba1 = patch->proba1_par;
	proba2 = patch->proba2_par;
	proba3 = patch->proba3_par;

	Phaser_params_set(&patch->phaser_par);
	Delay_params_set(&patch->delay_par);
	Chorus_params_set(&patch->chorus_par);
}

/*=============================================   MIDI and control functions   ===================================================*/

/******************************************  Sound functions   *******************************/

void Soundpatch_save(uint8_t midival)
{
	if (midival == MIDI_MAXi)
	{
		Synth_patch_save(&mypatch);
	}
}

/*-------------------------------------------------------*/
void Soundpatch_load(uint8_t midival)
{
	if (midival == MIDI_MAXi)
	{
		Synth_patch_load(&mypatch);
	}
}
/*-------------------------------------------------------*/
void nextSound(void)
{
	if (sound < LAST_SOUND)
		(sound)++;
	else
		sound = LAST_SOUND;
}
/*-------------------------------------------------------*/
void prevSound(void)
{
	if (sound > 0)
		(sound)--;
	else
		sound = 0;
}
/*-------------------------------------------------------*/
void Sound_set(uint8_t val)
{
	sound = (uint8_t) rintf((LAST_SOUND - 1) / MIDI_MAX * val);
}
/*-------------------------------------------------------*/
void autoSound_set(int8_t val)
{
	autoSound = val;
}
/*---------------------------------------------------------*/
void RandSound1(uint8_t val) /* random series of tones */
{
	if (val == MIDI_MAXi)
	{
		if (autoSound == 0)
			autoSound = 1;
		else
			autoSound = 0;
	}
}
/*---------------------------------------------------------*/
void RandSound2(uint8_t val) /* random series of tones */
{
	if (val == MIDI_MAXi)
	{
		if (autoSound == 0)
			autoSound = 2;
		else
			autoSound = 0;
	}
}
/*---------------------------------------------------------*/
uint8_t soundNumber_get(void)
{
	return sound;
}

/*************************************** ADSR functions **************************************/
void AttTime_set(uint8_t val)
{
	ADSR_setAttackTime(&adsr, val / MIDI_MAX + 0.0001f);
}

/*---------------------------------------------------------*/
void DecTime_set(uint8_t val)
{
	ADSR_setDecayTime(&adsr, .2 * val / MIDI_MAX + 0.0001f);
}

/*---------------------------------------------------------*/
void SustLevel_set(uint8_t val)
{
	ADSR_setSustainLevel(&adsr, val / MIDI_MAX);
}

/*---------------------------------------------------------*/
void RelTime_set(uint8_t val)
{
	ADSR_setReleaseTime(&adsr, .5f * val / MIDI_MAX + 0.0001f);
}

/*---------------------------------------------------------*/
void ADSRkeyON(void)
{
	ADSR_keyOn(&adsr);
}

/*---------------------------------------------------------*/
void ADSRkeyOFF(void)
{
	ADSR_keyOff(&adsr);
}

/*---------------------------------------------------------*/
void DemoMode_toggle(uint8_t val)
{
	if (val == MIDI_MAXi)
	{
		demoModeON = !demoModeON;
	}
	if (demoModeON)
		desynkatorON = false;
}

/*---------------------------------------------------------*/
void Desynkator_toggle(uint8_t val)
{
	if (val == MIDI_MAXi)
	{
		desynkatorON = !desynkatorON;
	}
}

/*---------------------------------------------------------*/
void Freeze_toggle(uint8_t val)
{
	if (val == MIDI_MAXi)
	{
		freezeON = !freezeON;
	}
}
/*---------------------------------------------------------*/
void Synth_reset(uint8_t val)
{
	if (val == MIDI_MAXi)
	{
		Synth_Init();
		demoModeON = false;
	}
}

/******************************************   FILTERS FUNCTIONS ******************************/

void Filter1Freq_set(uint8_t val)
{
	float freq = Lin2Exp(val, MIN_FREQ, MAX_FREQ) / SAMPLERATE;
	SVF_directSetFilterValue(&SVFilter1, freq);
	SVF_refFreq_set(&SVFilter1, freq);
}
//------------------------------------------------------------------------------------
void Filter1Res_set(uint8_t val)
{
	SVF_setReso(&SVFilter1, val / MIDI_MAX);
}
//------------------------------------------------------------------------------------
void Filter1Drive_set(uint8_t val)
{
	SVF_setDrive(&SVFilter1, val);
}
//------------------------------------------------------------------------------------
void Filter1Type_set(uint8_t val)
{
	SVFilter1.type = (uint8_t) lrintf(FILTER_TYPES * val / MIDI_MAX);
}

//------------------------------------------------------------------------------------
void Filter2Freq_set(uint8_t val)
{
	float freq = Lin2Exp(val, MIN_FREQ, MAX_FREQ) / SAMPLERATE;
	SVF_directSetFilterValue(&SVFilter2, freq);
	SVF_refFreq_set(&SVFilter2, freq);
}
//------------------------------------------------------------------------------------
void Filter2Res_set(uint8_t val)
{
	SVF_setReso(&SVFilter2, val / MIDI_MAX);
}
//------------------------------------------------------------------------------------
void Filter2Drive_set(uint8_t val)
{
	SVF_setDrive(&SVFilter2, val);
}
//------------------------------------------------------------------------------------
void Filter2Type_set(uint8_t val)
{
	SVFilter2.type = (uint8_t) lrintf(FILTER_TYPES * val / MIDI_MAX);
}

/*-------------------------------------------------------*/
void toggleFilter(void)
{
	if (autoFilterON)
		autoFilterON = false;
	else
		autoFilterON = true;
}
/*-------------------------------------------------------*/
void Filter_Random_switch(uint8_t val)
{
	if (val > 63)
		autoFilterON = true;
	else
		autoFilterON = false;
}

/*************************************** LFOs functions **************************************/
void Filt1LFO_amp_set(uint8_t val)
{
	filt_lfo.amp = MAX_FILTER_LFO_AMP / MIDI_MAX * val;
}
/*-------------------------------------------------------*/
void Filt1LFO_freq_set(uint8_t val)
{
	filt_lfo.freq = MAX_VIBRATO_FREQ / MIDI_MAX * val;
}
/*-------------------------------------------------------*/
void Filt2LFO_amp_set(uint8_t val)
{
	filt2_lfo.amp = MAX_FILTER_LFO_AMP / MIDI_MAX * val;
}
/*-------------------------------------------------------*/
void Filt2LFO_freq_set(uint8_t val)
{
	filt2_lfo.freq = MAX_VIBRATO_FREQ / MIDI_MAX * val;
}
/*-------------------------------------------------------*/
void toggleVibrato(void)
{
	if (vibr_lfo.amp != 0)
	{
		vibr_lfo.last_amp = vibr_lfo.amp;
		vibr_lfo.amp = 0;
	}
	else
		vibr_lfo.amp = vibr_lfo.last_amp;
}
/*-------------------------------------------------------*/
void VibratoAmp_set(uint8_t val)
{
	vibr_lfo.amp = MAX_VIBRATO_AMP / MIDI_MAX * val;
}
/*-------------------------------------------------------*/
void VibratoFreq_set(uint8_t val)
{
	vibr_lfo.freq = MAX_VIBRATO_FREQ / MIDI_MAX * val;
}

/*-------------------------------------------------------*/
void AmpLFO_amp_set(uint8_t val)
{
	amp_lfo.amp = val / MIDI_MAX;
}
/*-------------------------------------------------------*/
void AmpLFO_freq_set(uint8_t val)
{
	amp_lfo.freq = MAX_VIBRATO_FREQ / MIDI_MAX * val;
}

/******************************************** Volume functions *******************************/
void toggleSynthOut(void)
{
	if (op1.amp != 0)
	{
		op1.last_amp = op1.amp;
		op1.amp = 0;
		op2.last_amp = op2.amp;
		op2.amp = 0;
		op3.last_amp = op3.amp;
		op3.amp = 0;
	}
	else
	{
		op1.amp = op1.last_amp;
		op2.amp = op2.last_amp;
		op3.amp = op3.last_amp;
	}
}
/*-------------------------------------------------------*/
void SynthOut_switch(uint8_t val)
{

	switch (val)
	{
	case MIDI_MAXi:
		op1.amp = op1.last_amp;
		op2.amp = op2.last_amp;
		op3.amp = op3.last_amp;
		mbSawOsc.amp = mbSawOsc.last_amp;
		mbRectOsc.amp = mbRectOsc.last_amp;
		mbTriOsc.amp = mbTriOsc.last_amp;
		break;

	case 0:
		op1.last_amp = op1.amp;
		op1.amp = 0;
		op2.last_amp = op2.amp;
		op2.amp = 0;
		op3.last_amp = op3.amp;
		op3.amp = 0;
		mbSawOsc.last_amp = mbSawOsc.amp;
		mbSawOsc.amp = 0;
		mbRectOsc.last_amp = mbRectOsc.amp;
		mbRectOsc.amp = 0;
		mbTriOsc.last_amp = mbTriOsc.amp;
		mbTriOsc.amp = 0;
		break;
	}
}
/*-------------------------------------------------------*/
void incSynthOut(void)
{
	op1.amp *= 1.2f;
	op2.amp *= 1.2f;
	op3.amp *= 1.2f;
}
/*-------------------------------------------------------*/
void decSynthOut(void)
{
	op1.amp *= .8f;
	op2.amp *= .8f;
	op3.amp *= .8f;
}
/*-------------------------------------------------------*/
void SynthOut_amp_set(uint8_t val)
{
	float_t amp;
	amp = (val * 5.f / MIDI_MAX) * (val * 5.f / MIDI_MAX);
	op1.amp = amp;
	op2.amp = amp;
	op3.amp = amp;
	op4.amp = amp;
	addosc.amp = amp;
	driftosc.op1.amp = amp;
	driftosc.op2.amp = amp;
	driftosc.op3.amp = amp;
	oscill2.amp = amp;
	oscill3.amp = amp;
	mbSawOsc.amp = amp;
	mbRectOsc.amp = amp;
	mbRectOsc2.amp = amp;
	mbTriOsc.amp = amp;
}

/******************************************** Delay functions ********************************/
void Delay_toggle(void)
{
	if (delayON)
	{
		delayON = false;
		Delay_clean();
	}
	else
		delayON = true;
}
/*-------------------------------------------------------*/
void Delay_switch(uint8_t val)
{

	if (val > 63)
		delayON = true;
	else
	{
		delayON = false;
		Delay_clean();
	}
}

/******************************************** Chorus functions *******************************/
void Chorus_toggle(void)
{
	if (chorusON)
		chorusON = false;
	else
		chorusON = true;
}
/*-------------------------------------------------------*/
void Chorus_switch(uint8_t val)
{

	if (val > 63)
		chorusON = true;
	else
		chorusON = false;
}

/******************************************** Phaser functions *******************************/
void Phaser_switch(uint8_t val)
{

	if (val > 63)
		phaserON = true;
	else
		phaserON = false;
}

/****************************************** Oscillators functions ****************************/

void FM_OP1_freq_set(uint8_t val)
{
	FM_op_freq_set(&op1, val);
}
/*-------------------------------------------------------*/
void FM_OP1_modInd_set(uint8_t val)
{
	FM_op_modInd_set(&op1, val);
}

/*-------------------------------------------------------*/
void FM_OP2_freq_set(uint8_t val)
{
	//FM_op_freq_set(&op2, val);
	op2.mul = Lin2Exp(val, 0.2f, 32.f); // the freq of op2 is a multiple of the main pitch freq (op1)
	//op2.mul = roundf(32 * val/MIDI_MAX);
}
/*-------------------------------------------------------*/
void FM_OP2_freqMul_inc(uint8_t val)
{
	if (val == MIDI_MAXi)
	{
		op2.mul *= 1.01f;
	}
}
/*-------------------------------------------------------*/
void FM_OP2_freqMul_dec(uint8_t val)
{
	if (val == MIDI_MAXi)
	{
		op2.mul *= 0.99f;
	}
}
/*-------------------------------------------------------*/
void FM_OP2_modInd_set(uint8_t val)
{
	FM_op_modInd_set(&op2, val);
}

/*-------------------------------------------------------*/
void FM_OP3_freq_set(uint8_t val)
{
	op3.mul = Lin2Exp(val, 0.2f, 32.f); // the freq of op3 is a multiple of the main pitch freq (op1)
}
/*-------------------------------------------------------*/
void FM_OP3_modInd_set(uint8_t val)
{
	FM_op_modInd_set(&op3, val);
}
/*-------------------------------------------------------*/
void FM_OP3_freqMul_inc(uint8_t val)
{
	if (val == MIDI_MAXi)
	{
		op3.mul *= 1.01f;
	}
}
/*-------------------------------------------------------*/
void FM_OP3_freqMul_dec(uint8_t val)
{
	if (val == MIDI_MAXi)
	{
		op3.mul *= 0.99f;
	}
}
/*-------------------------------------------------------*/
void FM_OP4_freq_set(uint8_t val)
{
	op4.mul = Lin2Exp(val, 0.2f, 32.f); // the freq of op4 is a multiple of the main pitch freq (op1)
}
/*-------------------------------------------------------*/
void FM_OP4_modInd_set(uint8_t val)
{
	FM_op_modInd_set(&op4, val);
}
/*-------------------------------------------------------*/
void FM_OP4_freqMul_inc(uint8_t val)
{
	if (val == MIDI_MAXi)
	{
		op4.mul *= 1.01f;
	}
}
/*-------------------------------------------------------*/
void FM_OP4_freqMul_dec(uint8_t val)
{
	if (val == MIDI_MAXi)
	{
		op4.mul *= 0.99f;
	}
}

/*-----------------------------------------------------------------------------------------------*/
float_t _ITCMRAM_ FM1_sampleCompute(void) // op4 -> op3 -> op2 -> op1 => sound
{
	OpSampleCompute(&op4, WTSIN); // The result is in op4.out !
	op3.phase += op3.modInd * op4.out;
	OpSampleCompute(&op3, WTSIN);
	op2.phase += op2.modInd * op3.out;
	OpSampleCompute(&op2, WTSIN);
	op1.phase += op1.modInd * op2.out;
	OpSampleCompute(&op1, WTSIN);

	return op1.out;
}
/*-----------------------------------------------------------------------------------------------*/
float_t _ITCMRAM_ FM2_sampleCompute(float frq) //  (op2 -> op1) + (op4 -> op3) => sound
{
	float in;
	op1.freq = frq;
	op2.freq = op2.mul * frq;
	op3.freq = op3.mul * frq;
	op4.freq = op4.mul * frq;
	in = Osc_FM_sine_SampleCompute(&op2, 0);
	Osc_FM_sine_SampleCompute(&op1, in);
	in = Osc_FM_sine_SampleCompute(&op4, 0);
	Osc_FM_sine_SampleCompute(&op3, in);

	return 0.5f * (op1.out + op3.out);
}
/*-------------------------------------------------------*/
void DriftOsc1_amp_set(uint8_t val)
{
	DriftOsc_amp_set(&driftosc, val);
}
/*-------------------------------------------------------*/
void DriftOsc1_minFreq_set(uint8_t val)
{
	DriftOsc_minFreq_set(&driftosc, val);
}
/*-------------------------------------------------------*/
void DriftOsc1_maxFreq_set(uint8_t val)
{
	DriftOsc_maxFreq_set(&driftosc, val);
}
/*-------------------------------------------------------*/
void DriftOsc1_centralFreq_set(uint8_t val)
{
	DriftOsc_centralFreq_set(&driftosc, val);
}

/************************************* Desynkator functions **********************************/

void metro_tempo_set(uint8_t val)
{
	float fr1, fr2, fr3, frNew;

	fr1 = metro_getFreq(&metro1);
	fr2 = metro_getFreq(&metro2);
	fr3 = metro_getFreq(&metro3);
	metro_setBPM(&metro1, (float) (800.f * val / MIDI_MAX + 20)); // unit : bpm
	frNew = metro_getFreq(&metro1);
	metro_setFreq(&metro2, fr2 / fr1 * frNew);
	metro_setFreq(&metro3, fr3 / fr1 * frNew);

	seq.steptime = lrintf(seq.samplerate * 60 / seq.tempo);
}

/*--------------------------------------------------------------------------------------------*/
void metro2_tempo_set(uint8_t val)
{
	metro_setBPM(&metro2, rational_midi_get(val) * metro_getBPM(&metro1)); // unit : bpm

}

/*--------------------------------------------------------------------------------------------*/
void metro3_tempo_set(uint8_t val)
{
	metro_setBPM(&metro3, rational_midi_get(val) * metro_getBPM(&metro1)); // unit : bpm

}

/*--------------------------------------------------------------------------------------------*/
void metro1_proba_set(uint8_t val)
{
	proba1 = val / MIDI_MAX;
}
/*-------------------------------------------------------*/
void metro2_proba_set(uint8_t val)
{
	proba2 = val / MIDI_MAX;
}
/*-------------------------------------------------------*/
void metro3_proba_set(uint8_t val)
{
	proba3 = val / MIDI_MAX;
}

/*--------------------------------------------------------------------------------------------*/
void metro_reset_rq(uint8_t val)
{
	if (val == MIDI_MAXi)
	{
		metro_reset_requested = true;
	}
}

/*****************************************  Random sounds and FX functions *******************/

void MagicFX(uint8_t val) /* random effects parameters */
{
	if (val == MIDI_MAXi)
	{
		Delay_switch(MIDI_MAXi);
		Delay_time_set(MIDIrandVal());
		DelayWet_set(MIDIrandVal());
		DelayFeedback_set(MIDIrandVal());

		Chorus_switch(MIDI_MAXi);
		ChorusRate_set(MIDIrandVal());
		ChorusSecondRate_set(MIDIrandVal());
		ChorusDelay_set(MIDIrandVal());
		ChorusSweep_set(MIDIrandVal());
		ChorusFeedback_set(MIDIrandVal());
		ChorusMode_switch(MIDIrandVal());
		ChorusFDBsign_switch(MIDIrandVal());

		Phaser_switch(MIDI_MAXi);
		Phaser_Rate_set(MIDIrandVal());
		Phaser_Feedback_set(MIDIrandVal());
		Phaser_Wet_set(MIDIrandVal());
	}
}
/*-----------------------------------------------------------------------------*/
void MagicPatch(uint8_t val) /* Create a new sound with random sound parameters */
{
	if (val == MIDI_MAXi)
	{
		seq_tempo_set(MIDIrandVal());
		seq_freqMax_set(MIDIrandVal());
		seq_scale_set(MIDIrandVal());
		seq_switchMovingSeq(MIDIrandVal());
		seq_switchMute(MIDIrandVal());
		seq_gateTime_set(MIDIrandVal());
		autoSound_set(rand() % 3);
		Sound_set(MIDIrandVal());
		uint8_t snd = soundNumber_get();

		if (snd == FM2)
		{
			FM_OP1_freq_set(MIDIrandVal());
			FM_OP1_modInd_set(MIDIrandVal());
			FM_OP2_freq_set(MIDIrandVal());
			FM_OP2_modInd_set(MIDIrandVal());
			FM_OP3_freq_set(MIDIrandVal());
			FM_OP3_modInd_set(MIDIrandVal());
			FM_OP4_freq_set(MIDIrandVal());
			FM_OP4_modInd_set(MIDIrandVal());
		}
		else if (snd == DRIFTERS)
		{
			DriftOsc1_amp_set(MIDIrandVal());
			DriftOsc1_minFreq_set(MIDIrandVal());
			DriftOsc1_maxFreq_set(MIDIrandVal());
			DriftOsc1_centralFreq_set(MIDIrandVal());
		}
		else if (snd == ADDITIVE)
		{
			AddOsc_gen_newWaveform(&addosc);
		}

		Filter1Freq_set(MIDIrandVal());
		Filter1Res_set(MIDIrandVal());
		Filter1Drive_set(MIDIrandVal());
		Filter1Type_set(MIDIrandVal());
		Filt1LFO_amp_set(MIDIrandVal());
		Filt1LFO_freq_set(MIDIrandVal());

		Filter2Freq_set(MIDIrandVal());
		Filter2Res_set(MIDIrandVal());
		Filter2Drive_set(MIDIrandVal());
		Filter2Type_set(MIDIrandVal());
		Filt2LFO_amp_set(MIDIrandVal());
		Filt2LFO_freq_set(MIDIrandVal());

		Filter_Random_switch(MIDIrandVal());

		AttTime_set((uint8_t) lrintf(frand_a_b(0, MIDI_MAX / 10)));
		DecTime_set(MIDIrandVal());
		SustLevel_set(MIDIrandVal());
		RelTime_set(MIDIrandVal());

		VibratoAmp_set(MIDIrandVal());
		VibratoFreq_set(MIDIrandVal());

		AmpLFO_amp_set(MIDIrandVal());
		AmpLFO_freq_set(MIDIrandVal());
	}
}

/***************************************** Sequencer functions ***************************************************/

void Sequencer_toggle(uint8_t val)
{ // run or stop sequencer
	if (val == MIDI_MAXi)
	{
		g_sequencerIsOn = !g_sequencerIsOn;
		if (!g_sequencerIsOn)
			ADSR_keyOff(&adsr);
		Reset_notes_On();
	}
}

/*------------------------------------------------------------------------------------------------------*/
void _ITCMRAM_ sequencer_newStep_action(void) // User callback function called by sequencer_process()
{
	if ((noteGen.automaticON || noteGen.chRequested))
	{
		seq_sequence_new();
		AddOsc_gen_newWaveform(&addosc);
		noteGen.chRequested = false;
	}

	if (seq.chgTempoRequested)
	{
		float ratio = (float) (seq.gateTime) / seq.steptime;
		; // keep gate time ratio
		seq_steptime_update(&seq);
		seq.gateTime = roundf(ratio * seq.steptime);
		seq.chgTempoRequested = false;

	}

	if ((noteGen.someNotesMuted) && (!mayTrig(proba1)))
		ADSR_keyOff(&adsr);
	else
		ADSR_keyOn(&adsr);

	if (autoFilterON)
	{
		SVF_directSetFilterValue(&SVFilter1, 600.f / samplerate * powf(5000.f / 600.f, frand_a_b(0, 1)));
		SVF_directSetFilterValue(&SVFilter2, 600.f / samplerate * powf(5000.f / 600.f, frand_a_b(0, 1)));
	}

	if (noteGen.transpose != 0)
	{
		noteGen.rootNote += noteGen.transpose;
		seq_transpose();
	}

	if (autoSound == 1)
	{
		switch (rand() % 4)
		// 4 random timbers
		{
		case 0:
			sound = CHORD15;
			break;
		case 1:
			AddOsc_gen_newWaveform(&addosc);
			sound = ADDITIVE;
			break;
		case 2:
			sound = CHORD13min5;
			break;
		case 3:
			sound = VOICES3;
			break;
		}
	}
	if (autoSound == 2)
	{
		sound = rand() % LAST_SOUND;
		if ((sound == CHORD13min5) || (sound == CHORD135)) // avoided sounds
			sound = VOICES3;
		if (sound == ADDITIVE)
			AddOsc_gen_newWaveform(&addosc);
	}

	f0 = notesFreq[seq.track1.note[seq.step_idx]]; // Main "melody" frequency for this step
	vol = frand_a_b(0.4f, .8f); // slightly random volume for each note
}

/*---------------------------------------------------------------------------------------*/
void sequencer_newSequence_action(void) // User callback function called by sequencer_process()
{
	/* A new sequence begins ... */
	if ((demoModeON == true) && (freezeON == false))
	{
		MagicPatch(MIDI_MAXi);
		MagicFX(MIDI_MAXi);
	}
}

/*==========================================================================================================================*/
float _ITCMRAM_ waveCompute(Timbre_t sound, float frq)
{
	float y;

	OpSetFreq(&op1, frq);

	/* choose waveform generator */
	switch (sound)
	{
	case MORPH_SAW:
		y = 0.8f * OpSampleCompute(&op1, MSAW);
		break;

	case POWER_SINE:
		y = OpSampleCompute(&op1, SIN5);
		break;

	case BLEPTRIANGLE:
		mbTriOsc.freq = frq;
		y = BlepOsc_sampleCompute(&mbTriOsc);
		break;

	case BLEPSQUARE:
		mbRectOsc.freq = frq;
		y = BlepOsc_sampleCompute(&mbRectOsc);
		break;

	case BLEPSAW:
		mbSawOsc.freq = frq;
		y = BlepOsc_sampleCompute(&mbSawOsc);
		break;

	case WT_SINE:
		y = 0.8f * OpSampleCompute(&op1, WTSIN);
		break;

	case ADDITIVE:
		AddOsc_freq_set(&addosc, frq);
		y = AddOsc_SampleCompute(&addosc);
		break;

	case NOISE:
		y = op1.amp * frand_a_b(-.8f, .8f);
		break; // noise !

	case CHORD15:
		// fundamental + fifth : 1 5
		OpSetFreq(&op2, frq * 1.50f);
		y = 0.5f * (OpSampleCompute(&op1, MSAW) + OpSampleCompute(&op2, MSAW));
		break;

	case CHORD135:
		// major chord : 1 3maj 5
		OpSetFreq(&op2, frq * 1.26f);
		OpSetFreq(&op3, frq * 1.5f);
		y = 0.33f * (OpSampleCompute(&op1, MSAW) + OpSampleCompute(&op2, MSAW) + OpSampleCompute(&op3, MSAW));
		break;

	case CHORD13min5:
		// minor chord : 1 3min 5
		OpSetFreq(&op2, frq * 1.1892f);
		OpSetFreq(&op3, frq * 1.5f);
		y = 0.33f * (OpSampleCompute(&op1, MSAW) + OpSampleCompute(&op2, MSAW) + OpSampleCompute(&op3, MSAW));
		break;

	case VOICES3:
		// 3 slightly detuned oscillators with drifters

		OpSetFreq(&op2, frq * (1 + Drifter_nextSample(&d1)));
		OpSetFreq(&op3, frq * (1 + Drifter_nextSample(&d2)));
		y = 0.33f * (OpSampleCompute(&op1, MSAW) + OpSampleCompute(&op2, MSAW) + OpSampleCompute(&op3, MSAW));
		break;

	case DRIFTERS:
		DriftOsc_freq_set(&driftosc, frq);
		y = DriftOsc_sample_compute(&driftosc);
		break;

	case FM2:
		y = FM2_sampleCompute(frq);
		break;

	default:
		y = 0.8f * OpSampleCompute(&op1, WTSIN);
		break;

	}

	return y;
}

/*==========================================================================================================================*/
/*
 *	 Main sound generator function
 *	 buf : pointer to the audio buffer to be filled
 *	 length : is the number of frames. A frame is a 4 bytes data = left 16 bits sample + right 16 bits sample
 */
void _ITCMRAM_ make_sound(uint16_t *buf, uint16_t length) // To be used with the Sequencer
{
	uint16_t *outp;
	float y, y1, y2, y3;
	float yL, yR;
	float f1;
	float env, env1, env2, env3;
	uint16_t valueL, valueR;
	uint8_t clock1, clock2, clock3;

	outp = buf;

	for (uint16_t frame = 0; frame < length; frame++)
	{
		/*--------------------------------------- Desynkator synth -----------------------------------------------------*/

		if (desynkatorON)
		{

			clock1 = metro_process(&metro1);
			clock2 = metro_process(&metro2);
			clock3 = metro_process(&metro3);

			if (metro_reset_requested)
			{
				metro_reset(&metro1);
				metro_reset(&metro2);
				metro_reset(&metro3);
				clock1 = clock2 = clock3 = 1;
				metro_reset_requested = false;
			}
			/*************************************************************/
			if (clock1)
			{
				if ((noteGen.automaticON || noteGen.chRequested))
				{
					seq_sequence_new();
					noteGen.chRequested = false;
					AddOsc_gen_newWaveform(&addosc);
				}

				if ((noteGen.someNotesMuted) && (!mayTrig(proba1)))
					ADSR_keyOff(&adsr);
				else
					ADSR_keyOn(&adsr);

				if (autoFilterON)
				{
					SVF_directSetFilterValue(&SVFilter1, 600.f / samplerate * powf(5000.f / 600.f, frand_a_b(0, 1)));
					SVF_directSetFilterValue(&SVFilter2, 600.f / samplerate * powf(5000.f / 600.f, frand_a_b(0, 1)));
				}
				if (noteGen.transpose != 0)
				{
					noteGen.rootNote += noteGen.transpose;
					seq_transpose();
				}

				if (autoSound == 1)
				{
					switch (rand() % 4)
					// 4 random timbers
					{
					case 0:
						sound = CHORD15;
						break;
					case 1:
						AddOsc_gen_newWaveform(&addosc);
						sound = ADDITIVE;
						break;
					case 2:
						sound = CHORD13min5;
						break;
					case 3:
						sound = VOICES3;
						break;
					}
				}
				if (autoSound == 2)
				{
					sound = rand() % LAST_SOUND;
					if ((sound == CHORD13min5) || (sound == CHORD135))
						sound = VOICES3;
					if (sound == ADDITIVE)
						AddOsc_gen_newWaveform(&addosc);
				}

				f01 = midiNoteToFreq((uint8_t) seq_random_note());
				vol1 = frand_a_b(0.4f, .8f); // slightly random volume for each note
			}
			/*******************************************************************************/
			if (clock2)
			{
				if ((noteGen.someNotesMuted) && (!mayTrig(proba2)))
					ADSR_keyOff(&adsr2);
				else
					ADSR_keyOn(&adsr2);
				f02 = midiNoteToFreq((uint8_t) seq_random_note());
				vol2 = frand_a_b(0.4f, .8f); // slightly random volume for each note

			}
			/******************************************************************************/
			if (clock3)
			{
				if ((noteGen.someNotesMuted) && (!mayTrig(proba3)))
					ADSR_keyOff(&adsr3);
				else
					ADSR_keyOn(&adsr3);
				f03 = midiNoteToFreq((uint8_t) seq_random_note());
				vol3 = frand_a_b(0.4f, .8f); // slightly random volume for each note

			}

			/*--- compute vibrato modulation ---*/

			f1 = f01 * (1 + OpSampleCompute(&vibr_lfo, WTSIN));
			OpSetFreq(&oscill2, f02);
			mbRectOsc2.freq = f03;

			/*--- Generate waveform ---*/

			y1 = waveCompute(sound, f1);
			y2 = OpSampleCompute(&oscill2, MSAW);
			y3 = BlepOsc_sampleCompute(&mbRectOsc2);

			/*--- Apply envelop and tremolo ---*/

			env1 = ADSR_computeSample(&adsr) * (1 + OpSampleCompute(&amp_lfo, WTSIN));
			y1 *= vol1 * env1; // apply volume and envelop

			if (adsr.cnt_ >= lrintf(0.5f * samplerate / metro_getFreq(&metro1))) // 50% gate time
				ADSR_keyOff(&adsr);

			env2 = ADSR_computeSample(&adsr2) * (1 + OpSampleCompute(&amp_lfo2, WTSIN));
			y2 *= vol2 * env2; // apply volume and envelop

			if (adsr2.cnt_ >= lrintf(0.5f * samplerate / metro_getFreq(&metro2))) // 50% gate time
				ADSR_keyOff(&adsr2);

			env3 = ADSR_computeSample(&adsr3);
			y3 *= vol3 * env3; // apply volume and envelop

			if (adsr3.cnt_ >= lrintf(0.5f * samplerate / metro_getFreq(&metro3))) // 50% gate time
				ADSR_keyOff(&adsr3);

			/*--- Apply filter effect ---*/

			/* Update the filters cutoff frequencies */
			if ((!autoFilterON) && (filt_lfo.amp != 0))
				SVF_directSetFilterValue(&SVFilter1,
						SVF_refFreq_get(&SVFilter1) * (1 + OpSampleCompute7bis(&filt_lfo)));

			if (filt2_lfo.amp != 0)
				SVF_directSetFilterValue(&SVFilter2,
						SVF_refFreq_get(&SVFilter2) * (1 + OpSampleCompute7bis(&filt2_lfo)));

			y1 = 0.5f * (SVF_calcSample(&SVFilter1, y1) + SVF_calcSample(&SVFilter2, y1)); // Two filters in parallel

			/*---  Mix 3 oscillators ----*/
			y = 0.4f * (y1 + y2 + y3);

		}
		else
		{
			/*------------------------------------ Dekrispator synth --------------------------------------------------------*/

			/*--- Sequencer actions and update ---*/
			if (g_sequencerIsOn == true)
			{
				sequencer_process(); //computes f0 and calls sequencer_newStep_action() and sequencer_newSequence_action()
			}
			else
			{
				f0 = notesFreq[currentNote];
				vol = (float) velocity / 127.0f;
			}
			/*--- compute vibrato modulation ---*/
			f1 = f0 * (1 + OpSampleCompute(&vibr_lfo, WTSIN));

			/*--- Generate waveform ---*/
			y = waveCompute(sound, f1);

			/*--- Apply envelop and tremolo ---*/
			env = ADSR_computeSample(&adsr) * (1 + OpSampleCompute(&amp_lfo, WTSIN));
			y *= vol * env; // apply volume and envelop

			if (g_sequencerIsOn == true)
			{
				if (adsr.cnt_ >= seq.gateTime)
					ADSR_keyOff(&adsr);
			}

			/*--- Apply filter effect ---*/
			/* Update the filters cutoff frequencies */
			if ((!autoFilterON) && (filt_lfo.amp != 0))
				SVF_directSetFilterValue(&SVFilter1,
						SVF_refFreq_get(&SVFilter1) * (1 + OpSampleCompute7bis(&filt_lfo)));

			if (filt2_lfo.amp != 0)
				SVF_directSetFilterValue(&SVFilter2,
						SVF_refFreq_get(&SVFilter2) * (1 + OpSampleCompute7bis(&filt2_lfo)));

			y = 0.5f * (SVF_calcSample(&SVFilter1, y) + SVF_calcSample(&SVFilter2, y)); // Two filters in parallel

		}

		/*------------------------------------------ Common effects ------------------------------------------------------------*/

		/*---  Apply delay effect ----*/
		if (delayON)
			y = Delay_compute(y);

		/*---  Apply phaser effect ----*/
		if (phaserON)
			y = Phaser_compute(y);

		/*--- Apply stereo chorus/flanger effect ---*/
		if (chorusON)
			stereoChorus_compute(&yL, &yR, y);
		else
			yL = yR = y;

		/*--- clipping ---*/
		yL = (yL > 1.0f) ? 1.0f : yL; //clip too loud left samples
		yL = (yL < -1.0f) ? -1.0f : yL;

		yR = (yR > 1.0f) ? 1.0f : yR; //clip too loud right samples
		yR = (yR < -1.0f) ? -1.0f : yR;

		/****** let's hear the new sample *******/
		valueL = (uint16_t) ((int16_t) ((32767.0f) * yL)); // conversion float -> int16 !!!???
		valueR = (uint16_t) ((int16_t) ((32767.0f) * yR));

		*outp++ = valueL; // left channel sample
		*outp++ = valueR; // right channel sample
	}
}

/******************************************************* END *****************************************************************/

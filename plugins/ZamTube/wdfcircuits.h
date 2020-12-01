#ifndef WDFCIRCUITS_H
#define WDFCIRCUITS_H
#include "glue.h"
#include "triode.h"

#define WARMUP_SAMPLES 48000

class TubeStageCircuit {
	/*Tube Preamp*/
public:
	Triode t;
	bool on;
	int mode;
	int counter;

	TubeStageCircuit() {
		reset_tubes();
		P3_3Gamma1 = 1.;
		P0_3Gamma1 = 1.;
		S1_3Gamma1 = 1.;
		S3_3Gamma1 = 1.;
		P1_3Gamma1 = 1.;
		S0_3Gamma1 = 1.;
		S2_3Gamma1 = 1.;
		P2_3Gamma1 = 1.;
		E500E = 0.;
		ViE = 0.;
		mode = TUBE_MODE_SIXTIES;
	}

	enum tube_mode {
		TUBE_MODE_SIXTIES = 0,
		TUBE_MODE_GRIDLEAK,
	};

	void warmup_tubes(void) {
		counter = WARMUP_SAMPLES;
	}

	void reset_tubes(void) {
		Cia = 0.0;
		Cka = 0.0;
		Coa = 0.0;
		Vg = 0.0;
		Vk = 0.0;
	}

	void updateRValues(Real C_Ci, Real C_Ck, Real C_Co, Real E_E500, Real R_E500, Real R_Rg, Real R_Ri, Real R_Rk, Real R_Vi, Real R_Ro, Real sampleRate) {
		warmup_tubes();
		Real ViR = R_Vi;
		Real CiR = 1.0 / (2.0*C_Ci*sampleRate);
		Real RiR = R_Ri;
		Real RgR = R_Rg;
		Real RoR = R_Ro;
		Real RkR = R_Rk;
		Real CkR = 1.0 / (2.0*C_Ck*sampleRate);
		Real E500R = R_E500;
		E500E = E_E500;
		Real CoR = 1.0 / (2.0*C_Co*sampleRate);
		Real S0_3R = (CiR + ViR);
		S0_3Gamma1 = CiR/(CiR + ViR);
		Assert(S0_3Gamma1 >= 0.0 && S0_3Gamma1 <= 1.0);
		Real P0_1R = S0_3R;
		Real P0_2R = RiR;
		Real P0_3R = 1.0 /(1.0 / P0_1R + 1.0 / P0_2R);
		P0_3Gamma1 = 1.0 / P0_1R/(1.0 / P0_1R + 1.0 / P0_2R);
		Assert(P0_3Gamma1 >= 0.0 && P0_3Gamma1 <= 1.0);
		S1_3Gamma1 = RgR/(RgR + P0_3R);
		Assert(S1_3Gamma1 >= 0.0 && S1_3Gamma1 <= 1.0);
		Real P1_1R = CkR;
		Real P1_2R = RkR;
		Real P1_3R = 1.0 /(1.0 / P1_1R + 1.0 / P1_2R);
		P1_3Gamma1 = 1.0 / P1_1R/(1.0 / P1_1R + 1.0 / P1_2R);
		Assert(P1_3Gamma1 >= 0.0 && P1_3Gamma1 <= 1.0);
		Real S3_3R = (CoR + RoR);
		S3_3Gamma1 = CoR/(CoR + RoR);
		Assert(S3_3Gamma1 >= 0.0 && S3_3Gamma1 <= 1.0);
		Real P2_1R = S3_3R;
		Real P2_2R = E500R;
		Real P2_3R = 1.0 /(1.0 / P2_1R + 1.0 / P2_2R);
		P2_3Gamma1 = 1.0 / P2_1R/(1.0 / P2_1R + 1.0 / P2_2R);
		Assert(P2_3Gamma1 >= 0.0 && P2_3Gamma1 <= 1.0);
		S2_3Gamma1 = P2_3R/(P2_3R + P1_3R);
		Assert(S2_3Gamma1 >= 0.0 && S2_3Gamma1 <= 1.0);
	}

	Real advanc_gridleak(Real ViE) {
		//Get Bs
		//S2_3GetB
		//P2_3GetB
		//S3_3GetB
		Real Cob = Coa;
		//S3_1SetA
		//RoGetB
		//S3_2SetA
		Real S3_3b3 = -(Cob);
		//P2_1SetA
		//E500GetB
		//P2_2SetA
		Real P2_3b3 = E500E - P2_3Gamma1*(E500E - S3_3b3);
		//S2_1SetA
		//P1_3GetB
		Real Ckb = Cka;
		//P1_1SetA
		//RkGetB
		//P1_2SetA
		Real P1_3b3 = P1_3Gamma1*(-Ckb);
		//S2_2SetA
		Real S2_3b3 = (P2_3b3 + P1_3b3);
		//S1_3GetB
		//RgGetB
		//S1_1SetA
		//P0_3GetB
		//S0_3GetB
		Real Cib = Cia;
		//S0_1SetA
		//ViGetB
		//S0_2SetA
		Real S0_3b3 = -(Cib + ViE);
		//P0_1SetA
		//RiGetB
		//P0_2SetA
		Real P0_3b3 = -P0_3Gamma1*(-S0_3b3);
		//S1_2SetA
		Real S1_3b3 = -(P0_3b3);
		//P1_3GetB
		//P1_1SetA
		//RkGetB
		//P1_2SetA
		//Call tube model
		Vg = (S1_3b3);
		Real Vd = Vk+Vg;
		Real Rd = (Vd > 0.) ? 2.7e+3 : 100e+9;
		Real b = t.compute(S2_3b3, S2_3Gamma1, Vg, Vk);
		Vk = -(P1_3b3 + Vd * S1_3Gamma1 / Rd);
		//Set As
		//S2_3SetA
		Real S2_3b1 = P2_3b3 - S2_3Gamma1*(P2_3b3 - (Vk+Vg) + b);
		//P2_3SetA
		Real P2_3b1 = S2_3b1 + E500E - S3_3b3 - P2_3Gamma1*(E500E - S3_3b3);
		//S3_3SetA
		Real S3_3b1 = Cob - S3_3Gamma1*(Cob + P2_3b1);
		Coa = S3_3b1;
		Real S3_3b2 = Cob + P2_3b1 - S3_3Gamma1*(Cob + P2_3b1);
		//RoSetA
		Real Roa = S3_3b2;
		Real S2_3b2 = P2_3b3 + b - S2_3Gamma1*(P2_3b3 - (Vk+Vg) + b);
		//P1_3SetA
		Real P1_3b1 = S2_3b2  - Ckb - P1_3Gamma1*(-Ckb);
		Cka = P1_3b1;
		//RkSetA
		//S1_3SetA
		//RgSetA
		Real S1_3b2 = Vg - S1_3Gamma1*(P0_3b3 + Vg);
		//P0_3SetA
		Real P0_3b1 = S1_3b2  - S0_3b3 - P0_3Gamma1*(-S0_3b3);
		//S0_3SetA
		Real S0_3b1 = Cib - S0_3Gamma1*(Cib + ViE + P0_3b1);
		Cia = S0_3b1;
		//RiSetA
		//printf("Vk=%f Vg=%f Vd=%f in=%f out=%f\n", Vk,Vg,Vd, ViE,Roa);
		return -Roa;
	}

	Real advanc_sixties(Real ViE) {
		//Get Bs
		//S2_3GetB
		//P2_3GetB
		//S3_3GetB
		Real Cob = Coa;
		//S3_1SetA
		//RoGetB
		//S3_2SetA
		Real S3_3b3 = -(Cob);
		//P2_1SetA
		//E500GetB
		//P2_2SetA
		Real P2_3b3 = E500E - P2_3Gamma1*(E500E - S3_3b3);
		//S2_1SetA
		//P1_3GetB
		Real Ckb = Cka;
		//P1_1SetA
		//RkGetB
		//P1_2SetA
		Real P1_3b3 = P1_3Gamma1*(-Ckb);
		//S2_2SetA
		Real S2_3b3 = (P2_3b3 + P1_3b3);
		//S1_3GetB
		//RgGetB
		//S1_1SetA
		//P0_3GetB
		//S0_3GetB
		//Real Cib = Cia;
		//S0_1SetA
		//ViGetB
		//S0_2SetA
		//P0_1SetA
		//RiGetB
		//P0_2SetA
		Real P0_3b3 = -P0_3Gamma1*ViE;
		//S1_2SetA
		Real S1_3b3 = -(P0_3b3);
		//P1_3GetB
		//P1_1SetA
		//RkGetB
		//P1_2SetA
		//Call tube model
		Vg = -S1_3b3;
		Vk = -P1_3b3;
		Real b = -t.compute(S2_3b3, S2_3Gamma1, Vg, Vk);
		//Set As
		//S2_3SetA
		Real S2_3b1 = P2_3b3 - S2_3Gamma1*(P2_3b3 + P1_3b3 + b);
		//P2_3SetA
		Real P2_3b1 = S2_3b1 + E500E - S3_3b3 - P2_3Gamma1*(E500E - S3_3b3);
		//S3_3SetA
		Real S3_3b1 = Cob - S3_3Gamma1*(Cob + P2_3b1);
		Coa = S3_3b1;
		Real S3_3b2 = Cob + P2_3b1 - S3_3Gamma1*(Cob + P2_3b1);
		//RoSetA
		Real Roa = S3_3b2;
		Real S2_3b2 = P2_3b3 + b - S2_3Gamma1*(P2_3b3 + P1_3b3 + b);
		//P1_3SetA
		Real P1_3b1 = S2_3b2  - Ckb - P1_3Gamma1*(-Ckb);
		Cka = P1_3b1;
		//RkSetA
		//S1_3SetA
		//RgSetA
		//P0_3SetA
		//S0_3SetA
		//Cia = S0_3b1;
		//RiSetA
		//printf("Vk=%f Vg=%f Vpk=%f  in=%f out=%f\n", Vk,Vg,S2_3b3, ViE,Roa);
		return -Roa;
	}

	void set_mode(enum tube_mode newmode) {
		warmup_tubes();
		mode = newmode;
	}

	Real run(Real input) {
		Real output = (mode == TUBE_MODE_SIXTIES) ? advanc_sixties(input) : advanc_gridleak(input);
		if (counter > 0) {
			counter--;
			return 0.;
		} else {
			return output;
		}
	}

private:
	//State variables
	Real Coa;
	Real Cia;
	Real Cka;
	Real Vk;
	Real Vg;

	//R values
	Real P3_3Gamma1;
	Real P0_3Gamma1;
	Real S1_3Gamma1;
	Real S3_3Gamma1;
	Real P1_3Gamma1;
	Real ViE;
	Real S0_3Gamma1;
	Real S2_3Gamma1;
	Real P2_3Gamma1;
	Real E500E;
};

#endif

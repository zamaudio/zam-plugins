#ifndef WDFCIRCUITS_H
#define WDFCIRCUITS_H
#include "glue.h"
#include "triode.h"

#define WARMUP_SAMPLES 32

class TubeStageCircuit {
	/*Tube Preamp*/
public:
	Triode t;
	bool on;
	int mode;
	int counter;

	typedef struct Pair {
		Real v;
		Real c;
	} Pair_t;

	TubeStageCircuit() {
		P3_3Gamma1 = 0.;
		P2_3Gamma1 = 0.;
		P0_3Gamma1 = 0.;
		S1_3Gamma1 = 0.;
		S3_3Gamma1 = 0.;
		P1_3Gamma1 = 0.;
		S0_3Gamma1 = 0.;
		St_3Gamma1 = 0.;
		Pt_3Gamma1 = 0.;
		E500E = 303.;
		reset_tubes();
		t = Triode();
	}

	void set_model(int model) {
		t.set_model(model);
	}

	void warmup_tubes(void) {
		counter = WARMUP_SAMPLES;
		reset_tubes();
	}

	void reset_tubes(void) {
		Vk = 0.0124;
		Vg = 0.0;
		Cia = 0.0;
		Coa = 0.0;
		Cka = 0.0062;
	}

	void updateRValues(Real C_Ci, Real C_Ck, Real C_Co, Real E_E500, Real R_E500, Real R_Rg, Real R_Ri, Real R_Rk, Real R_Vi, Real R_Ro, Real sampleRate) { 
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
		Real S0_3R = (ViR + CiR);
		S0_3Gamma1 = ViR/(ViR + CiR);
		Assert(S0_3Gamma1 >= 0.0 && S0_3Gamma1 <= 1.0);
		Real P0_1R = S0_3R;
		Real P0_2R = RiR;
		Real P0_3R = 1.0 /(1.0 / P0_1R + 1.0 / P0_2R);
		P0_3Gamma1 = 1.0 / P0_1R/(1.0 / P0_1R + 1.0 / P0_2R);
		Assert(P0_3Gamma1 >= 0.0 && P0_3Gamma1 <= 1.0);
		Real S1_3R = (P0_3R + RgR);
		S1_3Gamma1 = P0_3R/(P0_3R + RgR);
		Assert(S1_3Gamma1 >= 0.0 && S1_3Gamma1 <= 1.0);
		Real P1_1R = CkR;
		Real P1_2R = RkR;
		Real P1_3R = 1.0 /(1.0 / P1_1R + 1.0 / P1_2R);
		P1_3Gamma1 = 1.0 / P1_1R/(1.0 / P1_1R + 1.0 / P1_2R);
		Assert(P1_3Gamma1 >= 0.0 && P1_3Gamma1 <= 1.0);
		Real S3_3R = (RoR + CoR);
		S3_3Gamma1 = RoR/(RoR + CoR);
		Assert(S3_3Gamma1 >= 0.0 && S3_3Gamma1 <= 1.0);
		Real P2_1R = S3_3R;
		Real P2_2R = E500R;
		Real P2_3R = 1.0 /(1.0 / P2_1R + 1.0 / P2_2R);
		P2_3Gamma1 = 1.0 / P2_1R/(1.0 / P2_1R + 1.0 / P2_2R);
		Assert(P2_3Gamma1 >= 0.0 && P2_3Gamma1 <= 1.0);
		Real Pt_1R = S1_3R;
		Real Pt_2R = P1_3R;
		Real Pt_3R = 1.0 /(1.0 / Pt_1R + 1.0 / Pt_2R);
		Pt_3Gamma1 = 1.0 / Pt_1R/(1.0 / Pt_1R + 1.0 / Pt_2R);
		Assert(Pt_3Gamma1 >= 0.0 && Pt_3Gamma1 <= 1.0);
		St_3Gamma1 = P2_3R/(P2_3R + Pt_3R);
		Assert(St_3Gamma1 >= 0.0 && St_3Gamma1 <= 1.0);
	}

	Pair_t advance(Real ViE, Real CiE) {
		Pair_t out;
		//Get Bs
		//St_3GetB
		//P2_3GetB
		//S3_3GetB
		//RoGetB
		//S3_1SetA
		Real Cob = Coa;
		//S3_2SetA
		Real S3_3b3 = -(0.0 + Cob);
		//P2_1SetA
		//E500GetB
		//P2_2SetA
		Real P2_3b3 = E500E - P2_3Gamma1*(E500E - S3_3b3);
		//St_1SetA
		//Pt_3GetB
		//S1_3GetB
		//P0_3GetB
		//S0_3GetB
		//ViGetB
		//S0_1SetA
		Real Cib = CiE;
		//S0_2SetA
		Real S0_3b3 = -(ViE + Cib);
		//P0_1SetA
		//RiGetB
		//P0_2SetA
		Real P0_3b3 = 0.0 - P0_3Gamma1*(0.0 - S0_3b3);
		//S1_1SetA
		//RgGetB
		//S1_2SetA
		Real S1_3b3 = -(P0_3b3 + 0.0);
		//Pt_1SetA
		//P1_3GetB
		Real Ckb = Cka;
		//P1_1SetA
		//RkGetB
		//P1_2SetA
		Real P1_3b3 = 0.0 - P1_3Gamma1*(0.0 - Ckb);
		//Pt_2SetA
		Real Pt_3b3 = P1_3b3 - Pt_3Gamma1*(P1_3b3 - S1_3b3);
		//St_2SetA
		Real St_3b3 = -(P2_3b3 + Pt_3b3);
		//S1_3GetB
		//P0_3GetB
		//S0_3GetB
		//Call tube model
		Real b = t.compute(-St_3b3, St_3Gamma1, Vg, Vk);
		//Set As
		//St_3SetA
		Real St_3b1 = P2_3b3 - St_3Gamma1*(P2_3b3 + Pt_3b3 + b);
		//P2_3SetA
		Real P2_3b1 = St_3b1 + E500E - S3_3b3 - P2_3Gamma1*(E500E - S3_3b3);
		//S3_3SetA
		Real S3_3b1 = 0.0 - S3_3Gamma1*(0.0 + Cob + P2_3b1);
		//RoSetA
		Real Roa = S3_3b1 * 25.;
		Real S3_3b2 = 0.0 + P2_3b1 - S3_3Gamma1*(0.0 + Cob + P2_3b1);
		Coa = -S3_3b2;
		Real St_3b2 = P2_3b3 + b - St_3Gamma1*(P2_3b3 + Pt_3b3 + b);
		//Pt_3SetA
		Real Pt_3b1 = St_3b2 + P1_3b3 - S1_3b3 - Pt_3Gamma1*(P1_3b3 - S1_3b3);
		//S1_3SetA
		Real S1_3b1 = P0_3b3 - S1_3Gamma1*(P0_3b3 + 0.0 + Pt_3b1);
		//P0_3SetA
		Real P0_3b1 = S1_3b1 + 0.0 - S0_3b3 - P0_3Gamma1*(0.0 - S0_3b3);
		//S0_3SetA
		//Real S0_3b1 = ViE - S0_3Gamma1*(ViE + Cib + P0_3b1);
		Real S0_3b2 = ViE + P0_3b1 - S0_3Gamma1*(ViE + Cib + P0_3b1);
		Cia = S0_3b2;
		//Real P0_3b2 = S1_3b1 - P0_3Gamma1*(0.0 - S0_3b3);
		//RiSetA
		//Real Ria = P0_3b2;
		//Real S1_3b2 = P0_3b3 + Pt_3b1 - S1_3Gamma1*(P0_3b3 + 0.0 + Pt_3b1);
		Real Pt_3b2 = St_3b2 - Pt_3Gamma1*(P1_3b3 - S1_3b3);
		//P1_3SetA
		Real P1_3b1 = Pt_3b2 + 0.0 - Ckb - P1_3Gamma1*(0.0 - Ckb);
		Cka = P1_3b1;
		Real P1_3b2 = Pt_3b2 - P1_3Gamma1*(0.0 - Ckb);
		//RkSetA
		Real Rka = P1_3b2;
		//RgSetA
		//Real Rga = S1_3b2;
		//printf("Vk=%f Vg=%f in=%f out=%f\n", Vk,Vg,ViE,Roa);
		Vg = S1_3b3;
		Vk = Rka;//P1_3b3;
		//printf("Vk=%.4f Vg=%.4f\tVgk=%.4f tb=%.1f\tCia=%.1f Coa=%.1f Cka=%.4f\tin=%.2f out=%.4f\n", Vk,Vg,Vg-Vk,b,Cia,Coa,Cka,ViE,Roa);
		out.v = Roa;
		out.c = Cia;
		return out;
	}

	Pair_t run(Pair_t input) {
		Pair_t zero = {0., input.c};
		Pair_t output = advance(input.v, input.c);
		if (counter > 0) {
			counter--;
			return zero;
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
	Real P2_3Gamma1;
	Real P0_3Gamma1;
	Real S1_3Gamma1;
	Real S3_3Gamma1;
	Real P1_3Gamma1;
	Real S0_3Gamma1;
	Real St_3Gamma1;
	Real Pt_3Gamma1;
	Real E500E;
};

#endif

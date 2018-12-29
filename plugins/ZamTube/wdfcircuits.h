#ifndef WDFCIRCUITS_H
#define WDFCIRCUITS_H
#include "glue.h"
#include "triode.h"

class TubeStageCircuit {
	/*Tube Preamp*/
public:
	Triode t;
	bool on;

	TubeStageCircuit() {
		Cia = 0.0;
		Cka = 0.0;
		Coa = 0.0;
		on = false;
	}
	TubeStageCircuit(Real C_Ci, Real C_Ck, Real C_Co, Real E_E250, Real R_E250, Real R_Rg, Real R_Ri, Real R_Rk, Real R_Ro, Real R_Vi, Real sampleRate, Triode& tube) {
		Cia = 0.0;
		Cka = 0.0;
		Coa = 0.0;
		on = false;
		updateRValues(C_Ci, C_Ck, C_Co, E_E250, R_E250, R_Rg, R_Ri, R_Rk, R_Ro, R_Vi, sampleRate, tube);
	}

	void updateRValues(Real C_Ci, Real C_Ck, Real C_Co, Real E_E250, Real R_E250, Real R_Rg, Real R_Ri, Real R_Rk, Real R_Ro, Real R_Vi, Real sampleRate, Triode& tube) {
		t = tube;
		Real ViR = R_Vi;
		Real CiR = 1.0 / (2.0*C_Ci*sampleRate);
		Real CkR = 1.0 / (2.0*C_Ck*sampleRate);
		Real CoR = 1.0 / (2.0*C_Co*sampleRate);
		Real RoR = R_Ro;
		Real RgR = R_Rg;
		Real RiR = R_Ri;
		Real RkR = R_Rk;
		Real E250R = R_E250;
		E250E = E_E250;
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
		Real I3_1R = CkR;
		Real I3_2R = RkR;
		I3_3Gamma1 = 1.0 / I3_1R/(1.0 / I3_1R + 1.0 / I3_2R);
		Assert(I3_3Gamma1 >= 0.0 && I3_3Gamma1 <= 1.0);
		Real S2_3R = (CoR + RoR);
		S2_3Gamma1 = CoR/(CoR + RoR);
		Assert(S2_3Gamma1 >= 0.0 && S2_3Gamma1 <= 1.0);
		Real P2_1R = S2_3R;
		Real P2_2R = E250R;
		P2_3Gamma1 = 1.0 / P2_1R/(1.0 / P2_1R + 1.0 / P2_2R);
		Assert(P2_3Gamma1 >= 0.0 && P2_3Gamma1 <= 1.0);
		t.Kr = sanitize_denormal(I3_3Gamma1);
		t.Pr = sanitize_denormal(S2_3Gamma1);
		t.Gr = sanitize_denormal(S1_3Gamma1);
		//printf("Kr = %f  Pr = %f  Gr = %f\n", t.Kr, t.Pr, t.Gr);
	}

	void warmup_tubes(void) {
		int i;
		on = false;
		for (i = 0; i < 1000; i++) {
			advanc(0.0);
		}
		on = true;
	}

	Real advanc(Real VE){
		ViE = VE;
		Real Ckb = Cka;
		Real I3_3b3 = I3_3Gamma1 * Ckb;
		Real Cib = Cia;
		Real S0_3b3 = Cib + ViE;
		Real P0_3b3 = P0_3Gamma1*(S0_3b3);
		Real S1_3b3 = P0_3b3;
		Real Cob = Coa;
		Real S2_3b3 = Cob;
		Real P2_3b3 = E250E - P2_3Gamma1*(E250E + S2_3b3);
		//Tube:    K       G      P
		//printf("K=%f G=%f P=%f\n", I3_3b3,-S1_3b3,P2_3b3);
		t.compute(I3_3b3,-S1_3b3,P2_3b3);
		Real b1 = t.getC();
		Real b2 = t.getG();
		Real b3 = t.getP();
		//Set As
		Real I3_3b1 = (b1 + Ckb - I3_3Gamma1*(Ckb));
		Cka = I3_3b1;
		Real S1_3b2 = ((-b2) - S1_3Gamma1*(P0_3b3 + (-b2)));
		Real P0_3b1 = (S1_3b2 + (S0_3b3) - P0_3Gamma1*(S0_3b3));
		Real S0_3b1 = (Cib - S0_3Gamma1*(Cib + ViE + P0_3b1));
		Cia = S0_3b1;
		Real P2_3b1 = (b3 + E250E + (S2_3b3) - P2_3Gamma1*(E250E + S2_3b3));
		Real S2_3b1 = (Cob - S2_3Gamma1*(Cob + P2_3b1));
		Coa = S2_3b1;
		Real S2_3b2 = (Cob + P2_3b1 - S2_3Gamma1*(Cob + P2_3b1));
		Real Roa = S2_3b2;
		return -Roa;
	}

private:
	//State variables
	Real Cia;
	Real Cka;
	Real Coa;

	//R values
	Real I3_3Gamma1;
	Real E250E;
	Real ViE;
	Real P0_3Gamma1;
	Real S0_3Gamma1;
	Real S2_3Gamma1;
	Real S1_3Gamma1;
	Real P2_3Gamma1;
};

#endif

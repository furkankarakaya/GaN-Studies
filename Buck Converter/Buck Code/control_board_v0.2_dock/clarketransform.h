
#ifndef CLARKETRANSFORM_H_
#define CLARKETRANSFORM_H_

typedef struct
{
	float  va;
	float  vb;
	float  vc;
	float  valfa;
	float  vbeta;
} CLARKETRANSFORM;

#define CLARKETRANSFORM_DEFAULTS {		\
		0,			\
		0, 			\
		0,			\
		0, 			\
		0, 			\
}
#define CLARKETRANSFORM_MACRO(v)										\
		v.valfa = 2*v.va/3 - (v.vb+v.vc)/3;		\
		v.beta = (v.vb-v.vc)/sqrt3;		\

#endif /* CLARKETRANSFORM_H_ */

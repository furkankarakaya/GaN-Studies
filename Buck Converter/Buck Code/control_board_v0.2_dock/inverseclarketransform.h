
#ifndef INVERSECLARKETRANSFORM_H_
#define INVERSECLARKETRANSFORM_H_


typedef struct
{
	float  valfa;
	float  vbeta;
	float  va;
	float  vb;
	float  vc;
} INVERSECLARKETRANSFORM;

#define INVERSECLARKETRANSFORM_DEFAULTS {		\
		0,			\
		0, 			\
		0,			\
		0, 			\
		0, 			\
}
#define INVERSECLARKETRANSFORM_MACRO(v)										\
		v.va = valfa;		\
		v.vb = (-valfa+sqrt3*vbeta)/2;		\
		v.vc = (-valfa-sqrt3*vbeta)/2;		\

#endif /* INVERSECLARKETRANSFORM_H_ */

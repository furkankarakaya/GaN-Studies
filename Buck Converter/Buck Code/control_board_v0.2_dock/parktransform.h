
#ifndef PARKTRANSFORM_H_
#define PARKTRANSFORM_H_

typedef struct
{
	float  valfa;
	float  vbeta;
	float  vd;
	float  vq;
	float  theta;

} PARKTRANSFORM;

#define PARKTRANSFORM_DEFAULTS {		\
		0,			\
		0, 			\
		0,			\
		0, 			\
		0, 			\
}
#define PARKTRANSFORM_MACRO(v)									\
		v.vd = v.valfa*cos(v.theta) + v.vbeta*sin(v.theta);		\
		v.vq = v.valfa*sin(v.theta) - v.vbeta*cos(v.theta);		\

#endif /* PARKTRANSFORM_H_ */


#ifndef INVERSEPARKTRANSFORM_H_
#define INVERSEPARKTRANSFORM_H_

typedef struct
{
	float  vd;
	float  vq;
	float  valfa;
	float  vbeta;
	float  theta;

} INVERSEPARKTRANSFORM;

#define INVERSEPARKTRANSFORM_DEFAULTS {		\
		0,			\
		0, 			\
		0,			\
		0, 			\
		0, 			\
}
#define INVERSEPARKTRANSFORM_MACRO(v)							\
		v.valfa = v.vd*cos(v.theta) + v.vq*sin(v.theta);		\
		v.vbeta = v.vd*sin(v.theta) - v.vq*cos(v.theta);		\

#endif /* INVERSEPARKTRANSFORM_H_ */

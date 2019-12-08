
#ifndef PICONTROLLER_H_
#define PICONTROLLER_H_

typedef struct
{
	float  Kp;
	float  Ki;
	float  upperlim;
	float  lowerlim;
	float  error[2];
	float  Uset;
	float  Uin;
	float  Yout[2];

} PICONTROLLER;

#define PICONTROLLER_DEFAULTS {		\
		0, 		\
		0, 		\
		0, 		\
		0, 		\
		0,0, 	\
		0, 		\
		0, 		\
		0,0, 	\
}

#define PICONTROLLER_MACRO(v)										\
		v.error[0] = v.Uset-v.Uin;			\
		v.Yout[0] = v.Yout[1]+v.error[0]*(v.Kp+v.Ki)-v.error[1]*v.Kp;		\
		if (v.Yout[0] > v.upperlim)		 \
			v.Yout[0] = v.upperlim;		\
		else if (v.Yout[0] < v.lowerlim)		\
			v.Yout[0] = v.lowerlim;		\
		v.Yout[1] = v.Yout[0];		\
		v.error[1] = v.error[0];		\

#endif /* PICONTROLLER_H_ */

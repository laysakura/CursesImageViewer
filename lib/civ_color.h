#ifndef _CIV_COLOR_H_
#define _CIV_COLOR_H_


typedef struct civ_RGB {
  unsigned char r;
  unsigned char g;
  unsigned char b;
} civ_RGB;

static inline float
RGB_distance_between(const CvScalar *v1, const CvScalar *v2)
{
  return (v1->val[0] - v2->val[0]) * (v1->val[0] - v2->val[0]) +
    (v1->val[1] - v2->val[1]) * (v1->val[1] - v2->val[1]) +
    (v1->val[2] - v2->val[2]) * (v1->val[2] - v2->val[2]);
}


#endif /* _CIV_COLOR_H_ */

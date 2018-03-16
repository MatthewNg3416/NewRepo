#ifndef _DEF_VISION_EXTERNAL_HEADER
#define _DEF_VISION_EXTERNAL_HEADER

#define DEF_USE_ICOMMON
//#undef DEF_USE_ICOMMON

// Vision Model Type Define
typedef enum
{
	DEF_NONE_MARK	= -1,
    DEF_PM_MARK		= 0,
    DEF_BLOB_MARK,
    DEF_CALIPER_MARK,
} EMarkType;

const int DEF_MAX_MARK_TYPE = 4;

const int ERR_VISION_SUCCESS = 0;

const int DEF_FIXED_OBJECT = 100;
const int DEF_FLOATING_OBJECT = 101;

#endif	// _DEF_VISION_EXTERNAL_HEADER

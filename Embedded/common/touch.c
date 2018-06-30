#include "common.h"

#include <stdio.h>
//#include <linux/input.h>
#include "input.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

static struct finger_info{
	int x;
	int y;
	int event;
} infos[FINGER_NUM_MAX];
static int touch_fd;
static int cur_slot = 0;

void touch_init(char *dev)
{
	touch_fd = open(dev, O_RDONLY);
	if(touch_fd < 0){
		printf("touch_init open %s error!errno = %d\n", dev, errno);
		return;
	}
	return;
}

/*return:
  TOUCH_NO_EVENT
  TOUCH_PRESS
  TOUCH_MOVE
  TOUCH_RELEASE
x: 0~1023
y: 0~599
finger: 0,1,2,3,4
*/
int touch_read(int *x, int *y, int *finger)
{
	struct input_event data;
	int n, ret;
	if((n = read(touch_fd, &data, sizeof(data))) != sizeof(data)){
		printf("touch_read error %d, errno=%d\n", n, errno);
		return;
	}
//	printf("event read: type-code-value = %d-%d-%d\n", data.type, data.code, data.value);
	switch(data.type)
	{
	case EV_ABS:
		switch(data.code)
		{
		case ABS_MT_SLOT:
			if(data.value >= 0 && data.value < FINGER_NUM_MAX){
				int old = cur_slot;
				cur_slot = data.value;
				if(infos[old].event != TOUCH_NO_EVENT){
					*x = infos[old].x;
					*y = infos[old].y;
					*finger = old;
					ret = infos[old].event;
					infos[old].event = TOUCH_NO_EVENT;
					return ret;
				}
			}
			break;
		case ABS_MT_TRACKING_ID:
			if(data.value == -1){
				*x = infos[cur_slot].x;
				*y = infos[cur_slot].y;
				*finger = cur_slot;
				infos[cur_slot].event = TOUCH_NO_EVENT;
				return TOUCH_RELEASE;
			}
			else{
				infos[cur_slot].event = TOUCH_PRESS;
			}
			break;
		case ABS_MT_POSITION_X:
			infos[cur_slot].x = data.value;
			if(infos[cur_slot].event != TOUCH_PRESS){
					infos[cur_slot].event = TOUCH_MOVE;
				}
				break;
			case ABS_MT_POSITION_Y:
				infos[cur_slot].y = data.value;
				if(infos[cur_slot].event != TOUCH_PRESS){
					infos[cur_slot].event = TOUCH_MOVE;
				}
				break;
		}
		break;
	case EV_SYN:
		switch(data.code)
		{
		case SYN_REPORT:
			if(infos[cur_slot].event != TOUCH_NO_EVENT){
				*x = infos[cur_slot].x;
				*y = infos[cur_slot].y;
				*finger = cur_slot;
				ret = infos[cur_slot].event;
				infos[cur_slot].event = TOUCH_NO_EVENT;
				return ret;
			}
			break;
		}
		break;
	}
	return TOUCH_NO_EVENT;
}

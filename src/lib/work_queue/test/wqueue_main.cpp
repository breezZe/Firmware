
#include "wqueue_test.h"

#include <px4_log.h>
#include <px4_middleware.h>
#include <px4_app.h>
#include <stdio.h>

int PX4_MAIN(int argc, char **argv)
{
	px4::init(argc, argv, "wqueue_test");

	PX4_INFO("wqueue hello\n");
	WQueueTest wq;
	wq.main();

	PX4_INFO("goodbye\n");
	return 0;
}


/****************************************************************************
 *
 *   Copyright (c) 2018 PX4 Development Team. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in
 *    the documentation and/or other materials provided with the
 *    distribution.
 * 3. Neither the name PX4 nor the names of its contributors may be
 *    used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED
 * AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 ****************************************************************************/

#pragma once


#include <containers/List.hpp>
#include <containers/Queue.hpp>

#include <px4_defines.h>
#include <px4_sem.h>
#include <px4_tasks.h>

namespace px4
{

class WorkItem;

class WorkQueue : public ListNode<WorkQueue *>
{

public:

	WorkQueue(const char *name);
	~WorkQueue() = default;

	const char *name() { return _name; }

	void add(WorkItem *item);

	void process();

	void set_task_id(px4_task_t task_id) { _task_id = task_id; }

private:

	void work_lock() { px4_sem_wait(&_lock); }
	void work_unlock() { px4_sem_post(&_lock); }

	const char *_name{nullptr};

	px4_sem_t	_lock;

	px4_task_t	_task_id{-1};

	sigset_t	_sigset{};

	Queue<WorkItem *>	_q;

};


// list of all px4 work queues
extern pthread_mutex_t px4_work_queues_list_mutex;
extern List<WorkQueue *> px4_work_queues_list;


WorkQueue *work_queue_create(const char *name, uint8_t priority, int stacksize);

} // namespace px4

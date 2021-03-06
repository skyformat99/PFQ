/***************************************************************
 *
 * (C) 2011-16 Nicola Bonelli <nicola@pfq.io>
 *             Andrea Di Pietro <andrea.dipietro@for.unipi.it>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 *
 * The full GNU General Public License is included in this distribution in
 * the file called "COPYING".
 *
 ****************************************************************/

#ifndef PFQ_QUEUE_H
#define PFQ_QUEUE_H

#include <pfq/group.h>
#include <pfq/sock.h>
#include <pfq/define.h>

#include <pfq/kcompat.h>


extern int pfq_shared_queue_enable(struct pfq_sock *so, unsigned long user_addr, size_t user_size, size_t hugepage_size);
extern int pfq_shared_queue_unmap(struct pfq_sock *so);


static inline size_t pfq_mpsc_queue_mem(struct pfq_sock *so)
{
        return so->rx_queue_len * so->rx_slot_size * 2;
}

static inline size_t pfq_spsc_queue_mem(struct pfq_sock *so)
{
        return so->tx_queue_len * so->tx_slot_size * 2;
}


static inline
size_t pfq_mpsc_queue_len(struct pfq_sock *p)
{
	struct pfq_shared_queue *q = pfq_sock_shared_queue(p);
	unsigned long data;
	if (!q)
		return 0;
	data = __atomic_load_n(&q->rx.shinfo, __ATOMIC_RELAXED);
        return PFQ_SHARED_QUEUE_LEN(data);
}


static inline
int pfq_mpsc_queue_index(struct pfq_sock *p)
{
	struct pfq_shared_queue *q = pfq_sock_shared_queue(p);
	unsigned long data;
	if (!q)
		return 0;
	data = __atomic_load_n(&q->rx.shinfo, __ATOMIC_RELAXED);
        return PFQ_SHARED_QUEUE_VER(data) & 1;
}



static inline
void * pfq_sock_rx_queue_mem(struct pfq_sock *so)
{
	struct pfq_shared_queue *sq = pfq_sock_shared_queue(so);
	if (unlikely(sq == NULL))
		return NULL;

	return (void *)sq + sizeof(struct pfq_shared_queue);
}


static inline
void *pfq_sock_tx_queue_mem(struct pfq_sock *so, int index)
{
	struct pfq_shared_queue *sq = pfq_sock_shared_queue(so);
	if (unlikely(sq == NULL))
		return NULL;

	return (void *)sq + sizeof(struct pfq_shared_queue)
			  + pfq_mpsc_queue_mem(so)
			  + pfq_spsc_queue_mem(so) * (1 + index);
}


static inline
char *pfq_mpsc_slot_ptr(struct pfq_sock *so, size_t qindex, size_t slot)
{
	void *rx_mem = pfq_sock_rx_queue_mem(so);
	if (!rx_mem)
		return NULL;

	return (char *)rx_mem + (so->rx_queue_len * (qindex & 1) + slot) * so->rx_slot_size;
}


#endif /* PFQ_QUEUE_H */

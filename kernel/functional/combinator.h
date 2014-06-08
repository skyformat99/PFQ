/***************************************************************
 *
 * (C) 2011-14 Nicola Bonelli <nicola.bonelli@cnit.it>
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

#ifndef _FUNCTIONAL_COMBINATOR_H_
#define _FUNCTIONAL_COMBINATOR_H_

#include "predicate.h"

static inline
bool not(arguments_t args, struct sk_buff const *skb)
{
	predicate_t p1 = get_data(predicate_t, args);

        return !EVAL_PREDICATE(p1,skb);
}

static inline
bool or(arguments_t args, struct sk_buff const *skb)
{
	predicate_t p1 = get_data0(predicate_t, args);
	predicate_t p2 = get_data1(predicate_t, args);

        return EVAL_PREDICATE(p1,skb) || EVAL_PREDICATE(p2, skb);
}


static inline
bool and(arguments_t args, struct sk_buff const *skb)
{
	predicate_t p1 = get_data0(predicate_t, args);
	predicate_t p2 = get_data1(predicate_t, args);

        return EVAL_PREDICATE(p1, skb) && EVAL_PREDICATE(p2, skb);
}


static inline
bool xor(arguments_t args, struct sk_buff const *skb)
{
	predicate_t p1 = get_data0(predicate_t, args);
	predicate_t p2 = get_data1(predicate_t, args);

        return EVAL_PREDICATE(p1, skb) != EVAL_PREDICATE(p2, skb);
}


#endif /* _FUNCTIONAL_COMBINATOR_H_ */